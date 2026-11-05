#include "crt_asin_math.h"
#include <windows.h>
#include <stddef.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* These two existing C definitions are already correct and remain owners. */
extern int32_t FUN_00560909(struct restore_crt_math_exception *record);
extern void FUN_00568422(int32_t type);
typedef char restore_math_exception_size[(sizeof(struct restore_crt_math_exception)==32)?1:-1];
typedef char restore_math_exception_result[(offsetof(struct restore_crt_math_exception,result)==24)?1:-1];
typedef char restore_ieee_record_size[(sizeof(struct restore_crt_ieee_record)==88)?1:-1];

/* Pinned image raw bytes, NOT host long-double constants. */
static const uint8_t asin_indefinite80[10]={0,0,0,0,0,0,0,0xc0,0xff,0xff};
static const uint8_t asin_half_pi80[10]={0x35,0xc2,0x68,0x21,0xa2,0xda,0x0f,0xc9,0xff,0x3f};
static const uint8_t exception_huge80[10]={0,0,0,0,0,0,0,0x80,0x10,0x44};
static const uint8_t exception_tiny80[10]={1,0,0,0,0,0,0,0x80,0x00,0x30};
static const uint64_t binary64_one=UINT64_C(0x3ff0000000000000);
static const uint64_t binary64_zero=0;
static const uint64_t overflow_infinity=UINT64_C(0x7ff0000000000000);
static const uint64_t overflow_maximum=UINT64_C(0x7fefffffffffffff);

/* Live ST0 primitives intentionally have no C floating-point types. Integer
   decisions between them do not spill or round the inherited x87 value. */
static uint16_t read_control_wait(void)
{
    uint16_t control;
    __asm { fstcw control }
    return control;
}
static void load_control(uint16_t control)
{
    __asm { fldcw control }
}
static uint64_t roundtrip64(uint64_t value)
{
    __asm {
        fld qword ptr value
        fstp qword ptr value
    }
    return value;
}
static void load64_live(uint64_t value)
{
    __asm { fld qword ptr value }
}
static uint64_t pop64_live(void)
{
    uint64_t value;
    __asm { fstp qword ptr value }
    return value;
}
static uint16_t compare_zero64(uint64_t value)
{
    uint16_t status;
    __asm {
        fld qword ptr value
        fcomp qword ptr binary64_zero
        fnstsw status
    }
    return status;
}
static void replace_asin_indefinite_live(void)
{
    __asm {
        fstp st(0)
        fld tbyte ptr asin_indefinite80
    }
}
static void replace_half_pi_live(uint32_t negative)
{
    __asm {
        fstp st(0)
        fld tbyte ptr asin_half_pi80
    }
    if(negative){__asm { fchs }}
}
static void asin_interior_live(void)
{
    /* Native 562D38..562D44. Keep the order and intermediate extended values:
       atan2(x,sqrt((1+x)*(1-x))), with current PC and normalized RC. */
    __asm {
        fld1
        fadd st(0),st(1)
        fld1
        fsub st(0),st(2)
        fmulp st(1),st(0)
        fsqrt
        fpatan
    }
}

uint32_t restore_crt_classify_high(uint32_t high)
{
    uint32_t exponent=high&0x7ff00000u;
    return exponent==0x7ff00000u?high:exponent;
}
uint32_t restore_crt_normalize_math_live(uint32_t *slot)
{
    uint16_t normalized=(uint16_t)((*slot&0x300u)|0x7fu);
    /* 567575 writes the UPPER WORD of the saved-CW DWORD, later POPped into
       EDX by the shared tail. Its low WORD remains the caller's actual CW. */
    ((uint16_t *)(void *)slot)[1]=normalized;
    load_control(normalized);
    return normalized;
}
uint32_t restore_crt_quiet_nan_live(uint32_t classification)
{
    if(classification&0x80000u)return 7;
    __asm { fadd qword ptr binary64_one }
    return 1;
}

void restore_crt_one_arg_error_live(uint32_t type,uint32_t name,uint32_t operation,
                                   uint64_t operand,uint16_t *saved_control)
{
    struct restore_crt_math_exception record;
    record.type=(int32_t)type;
    record.name=name;
    record.arg1=operand;
    /* Native does not write unary arg2. Its owned bytes remain unspecified;
       they are neither initialized nor exposed as an extra logical input. */
    record.result=pop64_live();
    restore_crt_math_dispatch(operation,&record,saved_control);
    load64_live(record.result);
    if(*saved_control!=0x27fu)load_control(*saved_control);
}
void restore_crt_math_finish_live(uint32_t saved_control,uint64_t operand,
                                 uint32_t operation,uint32_t name)
{
    uint16_t control=(uint16_t)saved_control;
    if(control==0x27fu)return;
    if((control&0x20u)&&((uint32_t)restore_crt_status_wait()&0x20u)){
        restore_crt_one_arg_error_live(8,name,operation,operand,&control);
        return;
    }
    load_control(control);
}
uint32_t restore_crt_asin_core_live(uint32_t classification,uint32_t incoming_flags,
                                  uint64_t operand,uint32_t incoming_edx)
{
    uint16_t control=read_control_wait();
    uint32_t scratch=(incoming_edx&0xffff0000u)|control,type;
    uint32_t high=(uint32_t)(operand>>32),low=(uint32_t)operand;
    if(incoming_flags&0x40u){
        if((classification&0xfffffu)||low)type=restore_crt_quiet_nan_live(classification);
        else { replace_asin_indefinite_live();type=1; }
        if(U(0x12663c0u)==0)
            restore_crt_one_arg_error_live(type,0x604960u,0xeu,operand,&control);
        else if(control!=0x27fu)load_control(control);
        return scratch;
    }
    if(control!=0x27fu)(void)restore_crt_normalize_math_live(&scratch);
    if(classification<0x3ff00000u)asin_interior_live();
    else if(classification==0x3ff00000u&&(high&0xfffffu)==0&&low==0)
        replace_half_pi_live(high&0x80000000u);
    else {
        replace_asin_indefinite_live();
        if(U(0x12663c0u)==0)
            restore_crt_one_arg_error_live(1,0x604960u,0xeu,operand,&control);
        else if(control!=0x27fu)load_control(control);
        return scratch;
    }
    if(U(0x12663c0u)==0)restore_crt_math_finish_live(control,operand,0xeu,0x604960u);
    else if(control!=0x27fu)load_control(control);
    return scratch;
}

int32_t restore_crt_status_wait(void)
{
    uint16_t status;
    __asm { fstsw status }
    return (int32_t)(int16_t)status;
}
int32_t restore_crt_clear_status(void)
{
    uint16_t status;
    __asm {
        fnstsw status
        fnclex
    }
    return (int32_t)(int16_t)status;
}
int32_t restore_crt_control_word(uint32_t value,uint32_t mask)
{
    uint16_t old_control=read_control_wait();
    load_control((uint16_t)(((uint32_t)old_control&~mask)|(value&mask)));
    return (int32_t)(int16_t)old_control;
}
int32_t restore_crt_control_word_slot(uint32_t value,uint32_t mask,uint32_t incoming_ecx,
                                     uint32_t *native_mask_slot)
{
    uint16_t old_control=read_control_wait();
    uint32_t old_slot=(incoming_ecx&0xffff0000u)|old_control;
    *native_mask_slot=(old_slot&~mask)|(value&mask);
    load_control((uint16_t)*native_mask_slot);
    return (int32_t)(int16_t)old_control;
}
void restore_crt_generate_exceptions(uint32_t exceptions)
{
    uint64_t spill;int32_t integer;
    if(exceptions&1u){
        __asm {
            fld tbyte ptr exception_huge80
            fistp integer
            fwait
        }
    }
    if(exceptions&8u){
        __asm {
            fstsw ax
            fld tbyte ptr exception_huge80
            fstp qword ptr spill
            fwait
            fstsw ax
        }
    }
    if(exceptions&0x10u){
        __asm {
            fld tbyte ptr exception_tiny80
            fstp qword ptr spill
            fwait
        }
    }
    if(exceptions&4u){
        __asm {
            fldz
            fld1
            fdivrp st(1),st(0)
            fstp st(0)
            fwait
        }
    }
    if(exceptions&0x20u){
        __asm {
            fldpi
            fstp qword ptr spill
            fwait
        }
    }
}

void restore_crt_set_exponent_live(uint64_t value,int32_t exponent)
{
    uint16_t high;
    value=roundtrip64(value);
    high=(uint16_t)((((uint32_t)exponent+0x3feu)<<4)|((value>>48)&0x800fu));
    value=(value&UINT64_C(0x0000ffffffffffff))|((uint64_t)high<<48);
    load64_live(value);
}
void restore_crt_frexp_live(uint64_t value,int32_t *exponent)
{
    uint64_t fraction;int32_t scale;uint16_t status=compare_zero64(value);
    if(status&0x4000u){ /* Native JZ includes unordered. */
        fraction=0;scale=0;
    }else if((value&UINT64_C(0x7ff0000000000000))==0&&
             (value&UINT64_C(0x000fffffffffffff))!=0){
        uint32_t negative=(compare_zero64(value)&0x100u)!=0;
        scale=-0x3fd;
        while((value&UINT64_C(0x0010000000000000))==0){value<<=1;--scale;}
        value&=~UINT64_C(0x0010000000000000);
        if(negative)value|=UINT64_C(0x8000000000000000);
        /* Native caller materializes this binary64 argument with FLD/FSTP
           before the set-exponent helper's own binary64 round trip. */
        value=roundtrip64(value);
        restore_crt_set_exponent_live(value,0);
        fraction=pop64_live();
    }else{
        scale=(int32_t)((value>>52)&0x7ffu)-0x3fe;
        value=roundtrip64(value);
        restore_crt_set_exponent_live(value,0);
        fraction=pop64_live();
    }
    *exponent=scale;
    load64_live(fraction);
}

uint32_t restore_crt_handle_masked_error(uint32_t exceptions,uint64_t *result,uint32_t control)
{
    uint32_t remaining=exceptions&0x1fu;
    if((exceptions&8u)&&(control&1u)){
        restore_crt_generate_exceptions(1);remaining&=~8u;
    }else if((exceptions&4u)&&(control&4u)){
        restore_crt_generate_exceptions(4);remaining&=~4u;
    }else if((exceptions&1u)&&(control&8u)){
        uint32_t rounding=control&0xc00u;
        uint16_t status;
        uint64_t replacement;
        restore_crt_generate_exceptions(8);
        status=compare_zero64(*result);
        /* JA: positive, ordered. NaNs use the non-positive branch. */
        if(rounding==0x400u)replacement=(status&0x4100u)?overflow_infinity:overflow_maximum;
        else if(rounding==0x800u)replacement=(status&0x4100u)?overflow_maximum:overflow_infinity;
        else if(rounding==0xc00u)replacement=overflow_maximum;
        else replacement=overflow_infinity;
        if(status&0x4100u)replacement|=UINT64_C(0x8000000000000000);
        /* Native FLD/FSTP local followed by FLD/FSTP result. */
        *result=roundtrip64(roundtrip64(replacement));
        remaining&=~1u;
    }else if((exceptions&2u)&&(control&0x10u)){
        uint32_t inexact=(exceptions&0x10u)!=0;
        uint64_t value=roundtrip64(*result);
        if(compare_zero64(value)&0x4000u)inexact=1;
        else {
            int32_t scale;uint32_t negative;
            value=roundtrip64(value); /* Native FLD/FSTP call argument. */
            restore_crt_frexp_live(value,&scale);
            value=pop64_live();scale-=0x600;
            if(scale< -0x432){value=0;inexact=1;}
            else {
                negative=(compare_zero64(value)&0x100u)!=0;
                value=(value&UINT64_C(0x000fffffffffffff))|UINT64_C(0x0010000000000000);
                if(scale< -0x3fd){
                    int32_t shifts=-0x3fd-scale;
                    do {if(value&1u)inexact=1;value>>=1;}while(--shifts);
                }
                if(negative){
                    load64_live(value);
                    __asm { fchs }
                    value=pop64_live();
                }
            }
            *result=roundtrip64(value);
        }
        if(inexact)restore_crt_generate_exceptions(0x10u);
        remaining&=~2u;
    }
    if((exceptions&0x10u)&&(control&0x20u)){
        restore_crt_generate_exceptions(0x20u);remaining&=~0x10u;
    }
    return remaining==0;
}

static void ieee_word_write(struct restore_crt_ieee_record *record,uint32_t index,uint32_t value)
{
    uint32_t i;volatile uint8_t *byte=record->byte+index*4u;
    for(i=0;i<4u;++i)byte[i]=(uint8_t)(value>>(i*8u));
}
static uint32_t ieee_word_read(const struct restore_crt_ieee_record *record,uint32_t index)
{
    uint32_t i,value=0;const volatile uint8_t *byte=record->byte+index*4u;
    for(i=0;i<4u;++i)value|=(uint32_t)byte[i]<<(i*8u);
    return value;
}
static void ieee_word_update(struct restore_crt_ieee_record *record,uint32_t index,
                             uint32_t preserve,uint32_t bits)
{
    uint32_t i;volatile uint8_t *byte=record->byte+index*4u;
    for(i=0;i<4u;++i){
        uint8_t mask=(uint8_t)(preserve>>(i*8u)),set=(uint8_t)(bits>>(i*8u));
        /* Skip wholly unwritten bytes. Partial native bitfields retain their
           owned, unspecified reserved bits through byte accesses only. */
        if(mask==0xffu&&set==0)continue;
        if(mask==0)byte[i]=set;
        else byte[i]=(uint8_t)((byte[i]&mask)|set);
    }
}

void restore_crt_raise_ieee(struct restore_crt_ieee_record *record,uint32_t *control,
                            uint32_t exceptions,uint32_t operation,
                            const uint64_t *operand,uint64_t *result)
{
    uint32_t code=exceptions,enabled=~*control,status,field;
    ULONG_PTR argument;
    ieee_word_write(record,1,0);ieee_word_write(record,2,0);ieee_word_write(record,3,0);
    if(exceptions&0x10u){code=0xc000008fu;ieee_word_update(record,1,0xffffffffu,1);}
    if(exceptions&2u){code=0xc0000093u;ieee_word_update(record,1,0xffffffffu,2);}
    if(exceptions&1u){code=0xc0000091u;ieee_word_update(record,1,0xffffffffu,4);}
    if(exceptions&4u){code=0xc000008eu;ieee_word_update(record,1,0xffffffffu,8);}
    if(exceptions&8u){code=0xc0000090u;ieee_word_update(record,1,0xffffffffu,0x10u);}
    ieee_word_write(record,2,((enabled&1u)<<4)|((enabled&4u)<<1)|((enabled>>1)&4u)|
                            ((enabled>>3)&2u)|((enabled>>5)&1u));
    status=(uint32_t)restore_crt_status_wait();
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
    {uint64_t value=roundtrip64(*operand);ieee_word_write(record,4,(uint32_t)value);ieee_word_write(record,5,(uint32_t)(value>>32));}
    ieee_word_update(record,20,0xffffffe3u,3);
    {uint64_t value=roundtrip64(*result);ieee_word_write(record,16,(uint32_t)value);ieee_word_write(record,17,(uint32_t)(value>>32));}
    (void)restore_crt_clear_status();
    argument=(ULONG_PTR)record;
    RaiseException(code,0,1,&argument);
    field=ieee_word_read(record,2);
    if(field&0x10u)*control&=~1u;
    if(field&8u)*control&=~4u;
    if(field&4u)*control&=~8u;
    if(field&2u)*control&=~0x10u;
    if(field&1u)*control&=~0x20u;
    field=((const volatile uint8_t *)record->byte)[0];
    *control=(*control&~0xc00u)|((field&3u)<<10);
    /* Deliberately retain the ORIGINAL's 0xFFFFF3FF mask here. It clears RC,
       not PC. Correcting this native behavior would alter handler resumption. */
    switch((field>>2)&7u){
    case 0:*control=(*control&0xfffff3ffu)|0x300u;break;
    case 1:*control=(*control&0xfffff3ffu)|0x200u;break;
    case 2:*control&=0xfffff3ffu;break;
    default:break;
    }
    *result=roundtrip64((uint64_t)ieee_word_read(record,16)|((uint64_t)ieee_word_read(record,17)<<32));
}

void restore_crt_math_dispatch(uint32_t operation,struct restore_crt_math_exception *record,
                               const uint16_t *saved_control)
{
    uint32_t control=*saved_control,exceptions=0;
    switch(record->type){
    case 1:case 5:exceptions=8;break;
    case 2:exceptions=4;break;
    case 3:exceptions=0x11;break;
    case 4:exceptions=0x12;break;
    case 7:record->type=1;break;
    case 8:exceptions=0x10;break;
    default:break;
    }
    if(exceptions&&!restore_crt_handle_masked_error(exceptions,&record->result,control)){
        /* 56971F owns uninitialized88-byte scratch. Write only the native
           fields/bits; unspecified reserved bytes remain owned and untouched. */
        struct restore_crt_ieee_record ieee;
        if(operation==0x10u||operation==0x16u||operation==0x1du){
            uint64_t value=roundtrip64(record->arg2);
            ieee_word_write(&ieee,10,(uint32_t)value);ieee_word_write(&ieee,11,(uint32_t)(value>>32));
            ieee_word_update(&ieee,14,0xffffffe3u,3);
        }else ieee_word_update(&ieee,14,0xfffffffeu,0);
        restore_crt_raise_ieee(&ieee,&control,exceptions,operation,&record->arg1,&record->result);
    }
    (void)restore_crt_control_word(control,0xffffu);
    if(record->type!=8&&U(0x605490u)==0&&FUN_00560909(record)!=0)return;
    FUN_00568422(record->type);
}
