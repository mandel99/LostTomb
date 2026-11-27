#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pharaoh/runtime/crt_ieee_raise.h"
#include "pharaoh/runtime/crt_x87_math.h"

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error This Native x87/Win32 exception owner requires MSVC Win32.
#endif
_Static_assert(sizeof(PharaohCrtIeeeRecord)==88,"Native IEEE scratch record");
_Static_assert(sizeof(ULONG_PTR)==4,"Native exception argument width");

/* Reused partial-field operations from current/old CrtAsinMath. */
static void ieee_word_write(struct PharaohCrtIeeeRecord *record,uint32_t index,uint32_t value)
{
    uint32_t i;volatile uint8_t *Byte=record->bytes+index*4u;
    for(i=0;i<4u;++i)Byte[i]=(uint8_t)(value>>(i*8u));
}

static uint32_t ieee_word_read(const struct PharaohCrtIeeeRecord *record,uint32_t index)
{
    uint32_t i,value=0;const volatile uint8_t *Byte=record->bytes+index*4u;
    for(i=0;i<4u;++i)value|=(uint32_t)Byte[i]<<(i*8u);
    return value;
}

static void ieee_word_update(struct PharaohCrtIeeeRecord *record,uint32_t index,
                             uint32_t preserve,uint32_t bits)
{
    uint32_t i;volatile uint8_t *Byte=record->bytes+index*4u;
    for(i=0;i<4u;++i){
        uint8_t mask=(uint8_t)(preserve>>(i*8u)),set=(uint8_t)(bits>>(i*8u));
        /* Skip wholly unwritten bytes. Partial native bitfields retain their
           owned, unspecified reserved bits through byte accesses only. */
        if(mask==0xffu&&set==0)continue;
        if(mask==0)Byte[i]=set;
        else Byte[i]=(uint8_t)((Byte[i]&mask)|set);
    }
}

/* Preserve the three Native FLD/FSTP qword transfers at68137/68159/68201. */
static __declspec(noinline) void copy_binary64_live(const void *source,void *destination)
{
    __asm {
        mov eax,source
        fld qword ptr[eax]
        mov eax,destination
        fstp qword ptr[eax]
    }
}

/* Whole00567F58, reused from current/old CrtAsinMath C. */
void pharaoh_crt_ieee_raise(struct PharaohCrtIeeeRecord *record,uint32_t *control,
                            uint32_t exceptions,uint32_t operation,
                            const uint64_t *operand,uint64_t *result)
{
    uint32_t Code=exceptions,enabled=~*control,status,field;
    ULONG_PTR argument;
    ieee_word_write(record,1,0);ieee_word_write(record,2,0);ieee_word_write(record,3,0);
    if(exceptions&0x10u){Code=0xc000008fu;ieee_word_update(record,1,0xffffffffu,1);}
    if(exceptions&2u){Code=0xc0000093u;ieee_word_update(record,1,0xffffffffu,2);}
    if(exceptions&1u){Code=0xc0000091u;ieee_word_update(record,1,0xffffffffu,4);}
    if(exceptions&4u){Code=0xc000008eu;ieee_word_update(record,1,0xffffffffu,8);}
    if(exceptions&8u){Code=0xc0000090u;ieee_word_update(record,1,0xffffffffu,0x10u);}
    ieee_word_write(record,2,((enabled&1u)<<4)|((enabled&4u)<<1)|((enabled>>1)&4u)|
                            ((enabled>>3)&2u)|((enabled>>5)&1u));
    status=(uint32_t)pharaoh_crt_x87_status_wait();
    if(status&1u)ieee_word_update(record,3,0xffffffffu,0x10u);
    if(status&4u)ieee_word_update(record,3,0xffffffffu,8);
    if(status&8u)ieee_word_update(record,3,0xffffffffu,4);
    if(status&0x10u)ieee_word_update(record,3,0xffffffffu,2);
    if(status&0x20u)ieee_word_update(record,3,0xffffffffu,1);
    ieee_word_update(record,0,0xfffffffcu,(*control>>10)&3u);
    switch(*control&0x300u){
    case 0:ieee_word_update(record,0,0xffffffebu,8);break;
    case 0x200:ieee_word_update(record,0,0xffffffe7u,4);break;
    case 0x300:ieee_word_update(record,0,0xffffffe3u,0);break;
    default:break;
    }
    ieee_word_update(record,0,0xfffe001fu,(operation&0xfffu)<<5);
    ieee_word_update(record,8,0xffffffe3u,3);
    copy_binary64_live(operand,record->bytes+16);
    ieee_word_update(record,20,0xffffffe3u,3);
    copy_binary64_live(result,record->bytes+64);
    (void)pharaoh_crt_x87_clear_status();
    argument=(ULONG_PTR)record;
    RaiseException(Code,0,1,&argument);
    field=ieee_word_read(record,2);
    if(field&0x10u)*control&=~1u;
    if(field&8u)*control&=~4u;
    if(field&4u)*control&=~8u;
    if(field&2u)*control&=~0x10u;
    if(field&1u)*control&=~0x20u;
    field=((const volatile uint8_t *)record->bytes)[0];
    *control=(*control&~0xc00u)|((field&3u)<<10);
    /* Deliberately retain the ORIGINAL's 0xFFFFF3FF mask here. It clears RC,
       not PC. Correcting this native behavior would alter handler resumption. */
    switch((field>>2)&7u){
    case 0:*control=(*control&0xfffff3ffu)|0x300u;break;
    case 1:*control=(*control&0xfffff3ffu)|0x200u;break;
    case 2:*control&=0xfffff3ffu;break;
    default:break;
    }
    copy_binary64_live(record->bytes+64,result);
}
