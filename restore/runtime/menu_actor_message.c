#include "storage_delivery.h"
/* Whole integer actor-message controller and genuine dependency bodies.
 * Pinned PE SHA256 fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
 * Code-address tokens below are readonly table data or dispatcher keys, never native code pointers. */
#include "menu_actor_message.h"
#include "town_menu.h"
#include "actor_predicate.h"
#include "training_queries.h"
#include "menu_list_scroll.h"
#include "menu_building_kind.h"
#include <stdio.h>
#include <stdlib.h>
#define MI_U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MI_U16(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
enum {MI_CF=1,MI_PF=4,MI_AF=16,MI_ZF=64,MI_SF=128,MI_OF=2048};
static __inline uint32_t mi_mask(uint32_t width){return width==32?UINT32_MAX:((1u<<width)-1u);}
static __inline uint32_t mi_szp(uint32_t value,uint32_t width)
{
 uint32_t parity=value&255u;parity^=parity>>4;parity&=15;
 return (value==0?MI_ZF:0)|((value>>(width-1u))&1u?MI_SF:0)|
        (((0x6996u>>parity)&1u)?0:MI_PF);
}
static __inline uint32_t mi_alu(struct restore_actor_message_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
{
 uint32_t mask=mi_mask(width),sign=1u<<(width-1u),result,flags,carry;
 uint64_t wide;a&=mask;b&=mask;carry=operation==2?(f->flags&MI_CF):0;
 if(operation==0){wide=(uint64_t)a+b;result=(uint32_t)wide&mask;
  flags=(wide>mask?MI_CF:0)|((~(a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);
 }else if(operation<=2){wide=(uint64_t)b+carry;result=(a-b-carry)&mask;
  flags=((uint64_t)a<wide?MI_CF:0)|(((a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);
 }else{result=(operation==3?(a&b):operation==4?(a|b):(a^b))&mask;flags=f->flags&MI_AF;}
 f->flags=(f->flags&~(MI_CF|MI_PF|MI_AF|MI_ZF|MI_SF|MI_OF))|flags|mi_szp(result,width);
 return result;
}
static __inline uint32_t mi_shift(struct restore_actor_message_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
{
 uint32_t mask=mi_mask(width),sign=1u<<(width-1u),result,carry,flags;
 count&=31u;a&=mask;if(!count)return a;
 if(arithmetic){carry=count>=width?(a&sign)!=0:((a>>(count-1u))&1u);
  result=count>=width?((a&sign)?mask:0):((a>>count)|((a&sign)?(mask^(mask>>count)):0));
 }else{carry=count>width?0:((a>>(width-count))&1u);result=(uint32_t)((uint64_t)a<<count)&mask;}
 flags=carry|mi_szp(result,width);
 if(count==1){if(!arithmetic&&(((result&sign)!=0)^carry))flags|=MI_OF;}
 else flags|=f->flags&MI_OF;
 f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|flags;return result;
}
static __inline uint32_t mi_imul(struct restore_actor_message_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


static const uint8_t mi_rom_004592c8[]={0x9a,0x92,0x45,0x00,0x96,0x92,0x45,0x00};
static const uint8_t mi_rom_004592d0[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00};
static const uint8_t mi_rom_0053cef8[]={0xcd,0xc1,0x53,0x00,0xfa,0xc1,0x53,0x00,0x75,0xc2,0x53,0x00,0x24,0xc5,0x53,0x00,0xd0,0xc3,0x53,0x00,0xcc,0xcb,0x53,0x00,0xef,0xca,0x53,0x00,0xdc,0xc5,0x53,0x00,0xb5,0xca,0x53,0x00,0x7a,0xce,0x53,0x00,0xb8,0xc7,0x53,0x00,0x07,0xc7,0x53,0x00,0x89,0xc2,0x53,0x00,0xbd,0xc2,0x53,0x00,0x2e,0xc3,0x53,0x00,0xd1,0xc4,0x53,0x00,0xd7,0xc8,0x53,0x00,0x43,0xca,0x53,0x00,0x90,0xca,0x53,0x00,0x58,0xca,0x53,0x00,0x85,0xc6,0x53,0x00,0x81,0xc4,0x53,0x00,0xe0,0xcd,0x53,0x00,0xf0,0xc4,0x53,0x00,0x0d,0xc3,0x53,0x00,0x24,0xc4,0x53,0x00,0x75,0xcd,0x53,0x00,0xd9,0xcc,0x53,0x00,0x62,0xc4,0x53,0x00,0x43,0xc4,0x53,0x00,0x5e,0xc3,0x53,0x00,0x18,0xcb,0x53,0x00,0x66,0xcc,0x53,0x00,0xb8,0xc8,0x53,0x00,0xb1,0xc3,0x53,0x00,0xf1,0xc2,0x53,0x00,0xdf,0xce,0x53,0x00};
static const uint8_t mi_rom_0053cf8c[]={0x00,0x01,0x02,0x03,0x04,0x24,0x05,0x06,0x07,0x08,0x24,0x24,0x24,0x24,0x09,0x09,0x09,0x09,0x0a,0x0b,0x0a,0x24,0x0c,0x0d,0x0e,0x0f,0x10,0x24,0x09,0x09,0x09,0x11,0x12,0x13,0x24,0x24,0x24,0x14,0x15,0x09,0x24,0x16,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x17,0x24,0x24,0x24,0x24,0x24,0x24,0x18,0x24,0x19,0x24,0x1a,0x1b,0x1c,0x1d,0x1c,0x24,0x24,0x24,0x1e,0x24,0x09,0x1f,0x20,0x19,0x21,0x24,0x24,0x24,0x24,0x22,0x24,0x23,0x24,0x24,0x24,0x24,0x24,0x24,0x09,0x24,0x24,0x1c};
static const uint8_t mi_rom_0053cff8[]={0x6d,0xc6,0x53,0x00,0x49,0xc3,0x53,0x00,0x49,0xc3,0x53,0x00,0x5c,0xc5,0x53,0x00,0x6d,0xc6,0x53,0x00,0x5c,0xc5,0x53,0x00};
static const uint8_t mi_rom_0053d010[]={0x7d,0xc3,0x53,0x00,0x81,0xc3,0x53,0x00};
static const uint8_t mi_rom_0053d018[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0053d028[]={0xf7,0xc8,0x53,0x00,0x30,0xc9,0x53,0x00,0x68,0xc9,0x53,0x00,0xa0,0xc9,0x53,0x00,0xc7,0xc9,0x53,0x00};
static const uint8_t mi_rom_0053d03c[]={0xab,0xce,0x53,0x00,0x22,0xc9,0x53,0x00};
static const uint8_t mi_rom_0053d044[]={0x00,0x01,0x00,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0053d710[]={0x52,0xd4,0x53,0x00,0x8c,0xd4,0x53,0x00,0xc5,0xd4,0x53,0x00,0x05,0xd5,0x53,0x00,0x8e,0xd5,0x53,0x00,0xde,0xd5,0x53,0x00,0x38,0xd6,0x53,0x00,0x8c,0xd6,0x53,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{uint32_t offset,k,result;
 if(address>=0x004592c8u&&width<=8u&&(offset=address-0x004592c8u)<=8u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004592c8[offset+k]<<(k*8u);return result;}
 if(address>=0x004592d0u&&width<=126u&&(offset=address-0x004592d0u)<=126u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004592d0[offset+k]<<(k*8u);return result;}
 if(address>=0x0053cef8u&&width<=148u&&(offset=address-0x0053cef8u)<=148u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053cef8[offset+k]<<(k*8u);return result;}
 if(address>=0x0053cf8cu&&width<=108u&&(offset=address-0x0053cf8cu)<=108u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053cf8c[offset+k]<<(k*8u);return result;}
 if(address>=0x0053cff8u&&width<=24u&&(offset=address-0x0053cff8u)<=24u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053cff8[offset+k]<<(k*8u);return result;}
 if(address>=0x0053d010u&&width<=8u&&(offset=address-0x0053d010u)<=8u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053d010[offset+k]<<(k*8u);return result;}
 if(address>=0x0053d018u&&width<=16u&&(offset=address-0x0053d018u)<=16u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053d018[offset+k]<<(k*8u);return result;}
 if(address>=0x0053d028u&&width<=20u&&(offset=address-0x0053d028u)<=20u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053d028[offset+k]<<(k*8u);return result;}
 if(address>=0x0053d03cu&&width<=8u&&(offset=address-0x0053d03cu)<=8u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053d03c[offset+k]<<(k*8u);return result;}
 if(address>=0x0053d044u&&width<=101u&&(offset=address-0x0053d044u)<=101u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053d044[offset+k]<<(k*8u);return result;}
 if(address>=0x0053d710u&&width<=32u&&(offset=address-0x0053d710u)<=32u-width){
 result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0053d710[offset+k]<<(k*8u);return result;}
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);}

static __inline void mi_idiv32(struct restore_actor_message_frame *f,uint32_t divisor_bits)
{
 int32_t divisor=(int32_t)divisor_bits;
 int64_t dividend=(int64_t)(int32_t)f->r[2]*INT64_C(4294967296)+(int64_t)f->r[0];
 int64_t quotient,remainder;
 /* Native #DE cases explicitly fail; no C division overflow or fake result. */
 if(!divisor||(dividend==INT64_MIN&&divisor==-1))abort();
 quotient=dividend/(int64_t)divisor;
 if(quotient<INT32_MIN||quotient>INT32_MAX)abort();
 remainder=dividend%(int64_t)divisor;
 f->r[0]=(uint32_t)(int32_t)quotient;f->r[2]=(uint32_t)(int32_t)remainder;
 /* IDIV leaves arithmetic flags undefined. Keep their previous frame bits. */
}


void restore_actor_message_C_004434e0(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004434e0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004434e1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004434e2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004434e3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004434e4 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004434e8 */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 004434ec */
    a=f->r[7];b=0x000024ecu;t=mi_imul(f,a,b);f->r[7]=t;
    /* 004434f2 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004434f4 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004434f6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea4096u+f->r[7])))&0xffffu)<<0);
    /* 004434fd */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 00443501 */
    if((f->flags&MI_ZF))goto mi_00443510;
    /* 00443503 */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 00443507 */
    if((f->flags&MI_ZF))goto mi_00443510;
    /* 00443509 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044350a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044350b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044350c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044350e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044350f */
    f->r[4]+=4;return;
mi_00443510:;
    /* 00443510 */
    a=f->r[3];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 00443513 */
    if((f->flags&MI_ZF))goto mi_0044351e;
    /* 00443515 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00443517 */
    if(mi_signed_less(f->flags))goto mi_0044351e;
    /* 00443519 */
    a=f->r[3];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0044351c */
    if(mi_signed_less(f->flags))goto mi_00443525;
mi_0044351e:;
    /* 0044351e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044351f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00443520 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00443521 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00443523 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00443524 */
    f->r[4]+=4;return;
mi_00443525:;
    /* 00443525 */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
    /* 00443529 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044352b */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 0044352f */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 00443532 */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00443534 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00443536 */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00443538 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de04u+f->r[6])))&0xffu)<<0);
    /* 0044353e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00443541 */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 00443544 */
    f->r[0]=MI_U32((0x0077646cu+f->r[1]*4u));
    /* 0044354b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044354c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00443551u;f->boundary(0x0044354cu,0x004c5240u,f);
    /* 00443551 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00443554 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00443556 */
    if((f->flags&MI_ZF))goto mi_00443576;
    /* 00443558 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044355a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0077de04u+f->r[6])))&0xffu)<<0);
    /* 00443560 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00443563 */
    f->r[1]=(0x00000000u+f->r[3]+f->r[2]*4u);
    /* 00443566 */
    a=MI_U32((0x0077592cu+f->r[1]*4u));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044356d */
    if(mi_signed_less(f->flags))goto mi_00443576;
    /* 0044356f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00443570 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00443571 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00443572 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00443574 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00443575 */
    f->r[4]+=4;return;
mi_00443576:;
    /* 00443576 */
    a=MI_U16((0x00ea4096u+f->r[7]));b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0044357e */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00443582 */
    if(!(f->flags&MI_ZF))goto mi_004435ae;
    /* 00443584 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00ea40deu+f->r[7]));
    /* 0044358b */
    a=f->r[0];b=0x00001276u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00443591 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 00443594 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00443596 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 00443599 */
    f->r[1]=MI_U32((0x00ea6160u+f->r[0]*4u));
    /* 004435a0 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004435a3 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004435a5 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004435cf;
    /* 004435a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004435a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004435a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004435aa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004435ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004435ad */
    f->r[4]+=4;return;
mi_004435ae:;
    /* 004435ae */
    a=f->r[0];b=0x00001276u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004435b4 */
    f->r[1]=MI_U32((0x00775810u+f->r[3]*4u));
    /* 004435bb */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004435bd */
    f->r[0]=MI_U32((0x00ea6160u+f->r[0]*4u));
    /* 004435c4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004435c6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004435cf;
    /* 004435c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004435c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004435ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004435cb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004435cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004435ce */
    f->r[4]+=4;return;
mi_004435cf:;
    /* 004435cf */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004435d1 */
    if((f->flags&MI_ZF))goto mi_004435e5;
    /* 004435d3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de06u+f->r[6]+f->r[3]*1u)))&0xffu)<<0);
    /* 004435da */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004435dc */
    if(!(f->flags&MI_ZF))goto mi_004435e5;
    /* 004435de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004435df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004435e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004435e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004435e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004435e4 */
    f->r[4]+=4;return;
mi_004435e5:;
    /* 004435e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004435e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004435e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004435e8 */
    f->r[0]=0x00000001u;
    /* 004435ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004435ee */
    f->r[4]+=4;return;
}

void restore_actor_message_C_004435f0(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004435f0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004435f5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004435f6 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004435fc */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004435fd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004435fe */
    f->r[1]=MI_U32((0x00ea187cu+f->r[0]));
    /* 00443604 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00443605 */
    a=f->r[1];b=0xffffec78u;t=mi_alu(f,a,b,32,1);
    /* 0044360b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00443614;
    /* 0044360d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044360e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044360f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00443610 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00443612 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00443613 */
    f->r[4]+=4;return;
mi_00443614:;
    /* 00443614 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 00443618 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 0044361c */
    f->r[3]=f->r[5];
    /* 0044361e */
    a=f->r[3];b=0x000024ecu;t=mi_imul(f,a,b);f->r[3]=t;
    /* 00443624 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00443626 */
    a=f->r[3];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 00443628 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea4096u+f->r[3])))&0xffffu)<<0);
    /* 0044362f */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 00443633 */
    if((f->flags&MI_ZF))goto mi_00443642;
    /* 00443635 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 00443639 */
    if((f->flags&MI_ZF))goto mi_00443642;
    /* 0044363b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044363c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044363d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044363e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00443640 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00443641 */
    f->r[4]+=4;return;
mi_00443642:;
    /* 00443642 */
    a=f->r[7];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 00443645 */
    if((f->flags&MI_ZF))goto mi_00443650;
    /* 00443647 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00443649 */
    if(mi_signed_less(f->flags))goto mi_00443650;
    /* 0044364b */
    a=f->r[7];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0044364e */
    if(mi_signed_less(f->flags))goto mi_00443657;
mi_00443650:;
    /* 00443650 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00443651 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00443652 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00443653 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00443655 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00443656 */
    f->r[4]+=4;return;
mi_00443657:;
    /* 00443657 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0044365b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044365e */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00443661 */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00443663 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00443665 */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00443667 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de2au+f->r[6]+f->r[7]*1u)))&0xffu)<<0);
    /* 0044366e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00443670 */
    if(!(f->flags&MI_ZF))goto mi_00443679;
    /* 00443672 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00443673 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00443674 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00443675 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00443677 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00443678 */
    f->r[4]+=4;return;
mi_00443679:;
    /* 00443679 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044367b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de04u+f->r[6])))&0xffu)<<0);
    /* 00443681 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00443684 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*4u);
    /* 00443687 */
    f->r[0]=MI_U32((0x0077646cu+f->r[2]*4u));
    /* 0044368e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044368f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00443694u;f->boundary(0x0044368fu,0x004c5240u,f);
    /* 00443694 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00443696 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00443699 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0077de04u+f->r[6])))&0xffu)<<0);
    /* 0044369f */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004436a2 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*4u);
    /* 004436a5 */
    a=MI_U32((0x0077592cu+f->r[2]*4u));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004436ac */
    if(mi_signed_less(f->flags))goto mi_004436b5;
    /* 004436ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004436af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004436b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004436b1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004436b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004436b4 */
    f->r[4]+=4;return;
mi_004436b5:;
    /* 004436b5 */
    a=f->r[5];b=0x00001276u;t=mi_imul(f,a,b);f->r[5]=t;
    /* 004436bb */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004436bd */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004436c0 */
    f->r[5]=MI_U32((0x00ea6160u+f->r[5]*4u));
    /* 004436c7 */
    if(!mi_signed_less(f->flags))goto mi_004436d0;
    /* 004436c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004436ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004436cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004436cc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004436ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004436cf */
    f->r[4]+=4;return;
mi_004436d0:;
    /* 004436d0 */
    a=MI_U16((0x00ea4096u+f->r[3]));b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004436d8 */
    if(!(f->flags&MI_ZF))goto mi_004436f8;
    /* 004436da */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00ea40deu+f->r[3]));
    /* 004436e1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004436e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004436e4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004436e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004436e8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004436eb */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004436ee */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004436f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004436f1 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004436f4 */
    f->r[0]=f->r[1];
    /* 004436f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004436f7 */
    f->r[4]+=4;return;
mi_004436f8:;
    /* 004436f8 */
    f->r[1]=MI_U32((0x00775810u+f->r[7]*4u));
    /* 004436ff */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00443701 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00443702 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00443704 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00443705 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00443706 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00443709 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044370a */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0044d6d0(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044d6d0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000014u+f->r[1]));
    /* 0044d6d4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007848d8u));
    /* 0044d6db */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044d6dd */
    f->r[2]=MI_U32((0x0078d7bcu));
    /* 0044d6e3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044d6e5 */
    if(!mi_signed_less(f->flags))goto mi_0044d6ea;
    /* 0044d6e7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0044d6e9 */
    f->r[4]+=4;return;
mi_0044d6ea:;
    /* 0044d6ea */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044d6ef;
    /* 0044d6ec */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d6ee */
    f->r[4]+=4;return;
mi_0044d6ef:;
    /* 0044d6ef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044d6f1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000003u+f->r[1])))&0xffu)<<0);
    /* 0044d6f4 */
    f->r[1]=MI_U32((0x0078d7c0u));
    /* 0044d6fa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044d6fc */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d6ff */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0044d700(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044d700 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d701 */
    f->r[6]=f->r[1];
    /* 0044d703 */
    a=MI_U8((0x00000028u+f->r[6]));b=0x00000011u;t=mi_alu(f,a,b,8,3);
    /* 0044d707 */
    if((f->flags&MI_ZF))goto mi_0044d70d;
    /* 0044d709 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d70b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d70c */
    f->r[4]+=4;return;
mi_0044d70d:;
    /* 0044d70d */
    f->r[1]=f->r[6];
    /* 0044d70f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d714u;f->boundary(0x0044d70fu,0x0044d6d0u,f);
    /* 0044d714 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d716 */
    if((f->flags&MI_ZF))goto mi_0044d71c;
    /* 0044d718 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d71a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d71b */
    f->r[4]+=4;return;
mi_0044d71c:;
    /* 0044d71c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000014u+f->r[6]));
    /* 0044d720 */
    f->r[0]=MI_U32((0x0078d7bcu));
    /* 0044d725 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007848d8u));
    /* 0044d72c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044d72e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044d730 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000003u+f->r[6])))&0xffu)<<0);
    /* 0044d733 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044d735 */
    f->r[2]=MI_U32((0x0078d7c0u));
    /* 0044d73b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044d73e */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044d741 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044d743 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044d745 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000002cu+f->r[6]));
    /* 0044d749 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044d74b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d74c */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d74f */
    f->r[4]+=4;return;
}

void restore_actor_message_C_00459240(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00459240 */
    f->r[1]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00459244 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459245 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459246 */
    f->r[6]=f->r[1];
    /* 00459248 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045924b */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045924d */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0045924f */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00459252 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 00459259 */
    a=(f->r[0]&0xffffu);b=0x0000006au;t=mi_alu(f,a,b,16,1);
    /* 0045925d */
    if(!mi_signed_less(f->flags))goto mi_00459265;
    /* 0045925f */
    a=(f->r[0]&0xffffu);b=0x00000072u;t=mi_alu(f,a,b,16,1);
    /* 00459263 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00459267;
mi_00459265:;
    /* 00459265 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
mi_00459267:;
    /* 00459267 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459268 */
    f->r[1]=0x00bca350u;
    /* 0045926d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459272u;f->boundary(0x0045926du,0x00488030u,f);
    /* 00459272 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00459274 */
    if((f->flags&MI_ZF))goto mi_00459278;
    /* 00459276 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
mi_00459278:;
    /* 00459278 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[6]),2u);
    /* 0045927f */
    a=f->r[0];b=0xffffff94u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00459282 */
    a=f->r[0];b=0x0000007du;t=mi_alu(f,a,b,32,1);
    /* 00459285 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00459296;
    /* 00459287 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00459289 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004592d0u+f->r[0]),1u))&0xffu)<<0);
    /* 0045928f */
    switch(mi_read((0x004592c8u+f->r[1]*4u),4u)){case 0x00459296u:goto mi_00459296;case 0x0045929au:goto mi_0045929a;default:abort();}
mi_00459296:;
    /* 00459296 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00459298 */
    if(!(f->flags&MI_ZF))goto mi_004592c2;
mi_0045929a:;
    /* 0045929a */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0045929f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004592a1 */
    a=f->r[0];b=0x000024ecu;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004592a7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a1bu+f->r[6]),1u))&0xffu)<<0);
    /* 004592ad */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004592af */
    a=mi_read((0x00ea4126u+f->r[2]*2u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004592b8 */
    if((f->flags&MI_ZF))goto mi_004592c2;
    /* 004592ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004592bb */
    f->r[0]=0x00000001u;
    /* 004592c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004592c1 */
    f->r[4]+=4;return;
mi_004592c2:;
    /* 004592c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004592c3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004592c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004592c6 */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0045b230(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_storage_limit_exceeded((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_actor_message_C_0045cf50(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045cf50 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cf51 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cf52 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cf53 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cf54 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045cf58 */
    f->r[0]=f->r[7];
    /* 0045cf5a */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0045cf5e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045cf61 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045cf63 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[0]*8u)))&0xffffu)<<0);
    /* 0045cf6b */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0045cf6f */
    if((f->flags&MI_ZF))goto mi_0045cf77;
    /* 0045cf71 */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 0045cf75 */
    if(!(f->flags&MI_ZF))goto mi_0045cff4;
mi_0045cf77:;
    /* 0045cf77 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0045cf7b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045cf7e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045cf81 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045cf83 */
    a=MI_U8((0x00bcd667u+f->r[1]*4u));b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0045cf8b */
    if(!(f->flags&MI_CF))goto mi_0045cff4;
    /* 0045cf8d */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 0045cf91 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_0045cf93:;
    /* 0045cf93 */
    f->r[2]=f->r[7];
    /* 0045cf95 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045cf98 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045cf9a */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[2]*8u));
    /* 0045cfa2 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0045cfa4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045cfee;
    /* 0045cfa6 */
    f->r[6]=f->r[7];
    /* 0045cfa8 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045cfab */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045cfad */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045cfb0 */
    a=MI_U16((0x00799a14u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045cfb8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045cfee;
    /* 0045cfba */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0045cfc1 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0045cfc5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cfc6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cfc7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cfc8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045cfcdu;f->boundary(0x0045cfc8u,0x004434e0u,f);
    /* 0045cfcd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045cfd0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045cfd2 */
    if((f->flags&MI_ZF))goto mi_0045cfee;
    /* 0045cfd4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0045cfdb */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045cfdf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cfe0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045cfe1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045cfe6u;f->boundary(0x0045cfe1u,0x0045d510u,f);
    /* 0045cfe6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045cfe9 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0045cfec */
    if(!mi_signed_less(f->flags))goto mi_0045cffb;
mi_0045cfee:;
    /* 0045cfee */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0045cfef */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0045cff2 */
    if(mi_signed_less(f->flags))goto mi_0045cf93;
mi_0045cff4:;
    /* 0045cff4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045cff5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045cff6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045cff7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045cff9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045cffa */
    f->r[4]+=4;return;
mi_0045cffb:;
    /* 0045cffb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045cffc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045cffd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045cffe */
    f->r[0]=0x00000001u;
    /* 0045d003 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045d004 */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0045d180(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045d180 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d181 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d182 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 0045d186 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d187 */
    f->r[6]=f->r[5];
    /* 0045d189 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d18a */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045d18d */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045d18f */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045d192 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[6])))&0xffffu)<<0);
    /* 0045d199 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0045d19d */
    if((f->flags&MI_ZF))goto mi_0045d1a9;
    /* 0045d19f */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 0045d1a3 */
    if(!(f->flags&MI_ZF))goto mi_0045d2f3;
mi_0045d1a9:;
    /* 0045d1a9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a18u+f->r[6]));
    /* 0045d1b0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0045d1b3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0045d1b6 */
    f->r[1]=MI_U32((0x00705eb8u+f->r[0]*8u));
    /* 0045d1bd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d1be */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d1bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045d1c4u;f->boundary(0x0045d1bfu,0x004cde60u,f);
    /* 0045d1c4 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045d1c8 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045d1cb */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045d1ce */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045d1d1 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045d1d3 */
    a=MI_U16((0x00bcd66cu+f->r[1]*4u));b=0x00000008u;t=mi_alu(f,a,b,16,1);
    /* 0045d1dc */
    if(!mi_signed_less(f->flags))goto mi_0045d2f3;
    /* 0045d1e2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045d1e4 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0045d1e6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a99u+f->r[6])))&0xffu)<<0);
    /* 0045d1ec */
    f->r[6]=0x00000001u;
    /* 0045d1f1 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0045d1f4 */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0045d1f7 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045d1f9 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
mi_0045d1fc:;
    /* 0045d1fc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007900c2u+f->r[7]+f->r[6]*1u)))&0xffu)<<0);
    /* 0045d203 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045d205 */
    if((f->flags&MI_ZF))goto mi_0045d243;
    /* 0045d207 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0045d209 */
    if((f->flags&MI_ZF))goto mi_0045d243;
    /* 0045d20b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d20c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d20e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d20f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045d214u;f->boundary(0x0045d20fu,0x0045b230u,f);
    /* 0045d214 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045d217 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045d219 */
    if(!(f->flags&MI_ZF))goto mi_0045d243;
    /* 0045d21b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d21c */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d21e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d21f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045d224u;f->boundary(0x0045d21fu,0x0045b230u,f);
    /* 0045d224 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045d227 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045d229 */
    if(!(f->flags&MI_ZF))goto mi_0045d243;
    /* 0045d22b */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0045d22f */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0045d233 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d234 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d235 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d236 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045d23bu;f->boundary(0x0045d236u,0x004435f0u,f);
    /* 0045d23b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045d23e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045d240 */
    if((f->flags&MI_ZF))goto mi_0045d243;
    /* 0045d242 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
mi_0045d243:;
    /* 0045d243 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0045d244 */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0045d247 */
    if(mi_signed_less(f->flags))goto mi_0045d1fc;
    /* 0045d249 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0045d24b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045d2f3;
    /* 0045d251 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0045d255 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0045d259 */
    f->r[6]=f->r[3];
    /* 0045d25b */
    a=f->r[6];b=0x000049d8u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 0045d261 */
    f->r[1]=MI_U32((0x00ea5b34u+f->r[6]));
    /* 0045d267 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d268 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d269 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d26a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045d26fu;f->boundary(0x0045d26au,0x004435f0u,f);
    /* 0045d26f */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0045d275 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045d278 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0045d27e */
    f->r[2]=MI_U32((0x00ea5b34u+f->r[1]));
    /* 0045d284 */
    a=MI_U8((0x007900c2u+f->r[2]+f->r[7]*1u));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045d28c */
    if((f->flags&MI_ZF))goto mi_0045d292;
    /* 0045d28e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045d290 */
    if(!(f->flags&MI_ZF))goto mi_0045d2fe;
mi_0045d292:;
    /* 0045d292 */
    f->r[3]=0x00000001u;
mi_0045d297:;
    /* 0045d297 */
    f->r[1]=MI_U32((0x00ea5b34u+f->r[6]));
    /* 0045d29d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0045d29e */
    f->r[0]=f->r[1];
    /* 0045d2a0 */
    MI_U32((0x00ea5b34u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0045d2a6 */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0045d2a9 */
    if(mi_signed_less(f->flags))goto mi_0045d2b5;
    /* 0045d2ab */
    MI_U32((0x00ea5b34u+f->r[6]))=(uint32_t)(0x00000001u);
mi_0045d2b5:;
    /* 0045d2b5 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0045d2ba */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045d2c0 */
    f->r[1]=MI_U32((0x00ea5b34u+f->r[0]));
    /* 0045d2c6 */
    a=MI_U8((0x007900c2u+f->r[1]+f->r[7]*1u));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045d2ce */
    if((f->flags&MI_ZF))goto mi_0045d2ed;
    /* 0045d2d0 */
    f->r[2]=MI_U32((0x00ea5b34u+f->r[6]));
    /* 0045d2d6 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0045d2da */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0045d2de */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d2df */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d2e0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d2e1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045d2e6u;f->boundary(0x0045d2e1u,0x004435f0u,f);
    /* 0045d2e6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045d2e9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045d2eb */
    if(!(f->flags&MI_ZF))goto mi_0045d2fa;
mi_0045d2ed:;
    /* 0045d2ed */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0045d2ee */
    a=f->r[3];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0045d2f1 */
    if(mi_signed_less(f->flags))goto mi_0045d297;
mi_0045d2f3:;
    /* 0045d2f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045d2f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045d2f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045d2f6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045d2f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045d2f9 */
    f->r[4]+=4;return;
mi_0045d2fa:;
    /* 0045d2fa */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
mi_0045d2fe:;
    /* 0045d2fe */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0045d300:;
    /* 0045d300 */
    f->r[2]=f->r[5];
    /* 0045d302 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045d305 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045d307 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[2]*8u));
    /* 0045d30f */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0045d311 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045d34a;
    /* 0045d313 */
    f->r[0]=f->r[5];
    /* 0045d315 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045d318 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045d31a */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045d31d */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00799a14u+f->r[0])))&0xffffu)<<0);
    /* 0045d324 */
    a=(f->r[1]&0xffffu);b=0x00000190u;t=mi_alu(f,a,b,16,1);
    /* 0045d329 */
    if(!mi_signed_less(f->flags))goto mi_0045d34a;
    /* 0045d32b */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0045d32e */
    if((f->flags&MI_ZF))goto mi_0045d357;
    /* 0045d330 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 0045d337 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 0045d33b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d33c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d33d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045d33e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045d343u;f->boundary(0x0045d33eu,0x004435f0u,f);
    /* 0045d343 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045d346 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045d348 */
    if(!(f->flags&MI_ZF))goto mi_0045d357;
mi_0045d34a:;
    /* 0045d34a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0045d34b */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0045d34e */
    if(mi_signed_less(f->flags))goto mi_0045d300;
    /* 0045d350 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045d351 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045d352 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045d353 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045d355 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045d356 */
    f->r[4]+=4;return;
mi_0045d357:;
    /* 0045d357 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045d358 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045d359 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045d35a */
    f->r[0]=0x00000001u;
    /* 0045d35f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045d360 */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0045d510(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_resource_amount((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u));f->r[4]=sp+4u;
}

void restore_actor_message_C_004893c0(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004893c0 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004893c4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004893c5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004893c6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004893c7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004893ca */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004893cb */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004893d3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004893d6 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004893da */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004893dd */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 004893e2 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 004893e5 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004893e9 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004893eb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004893ed */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 004893f3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00489467;
    /* 004893f5 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004893f9 */
    f->r[1]=0x005cc8c0u;
    /* 004893fe */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00489402:;
    /* 00489402 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00489404 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00489406 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00489442;
    /* 00489408 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 0048940c */
    a=f->r[6];b=0x00010000u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
mi_00489412:;
    /* 00489412 */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 00489414 */
    f->r[0]=f->r[3];
    /* 00489416 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00489418 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0048941a */
    if((f->flags&MI_ZF))goto mi_0048942d;
    /* 0048941c */
    f->r[2]=MI_U32((0x00d05720u+f->r[0]*4u));
    /* 00489423 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00489425 */
    if((f->flags&MI_ZF))goto mi_0048942d;
    /* 00489427 */
    a=MI_U8((0x00000018u+f->r[2]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0048942b */
    if((f->flags&MI_ZF))goto mi_00489460;
mi_0048942d:;
    /* 0048942d */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00489431 */
    a=MI_U32((0x00af598cu+f->r[0]*4u));b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00489438 */
    if(!(f->flags&MI_ZF))goto mi_00489460;
    /* 0048943a */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0048943b */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048943e */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00489440 */
    if(mi_signed_less(f->flags))goto mi_00489412;
mi_00489442:;
    /* 00489442 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00489446 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0048944a */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0048944e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048944f */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00489452 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00489454 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00489458 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0048945c */
    if(!mi_signed_less(f->flags))goto mi_00489467;
    /* 0048945e */
    goto mi_00489402;
mi_00489460:;
    /* 00489460 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00489461 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00489462 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00489463 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00489465 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00489466 */
    f->r[4]+=4;return;
mi_00489467:;
    /* 00489467 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00489468 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00489469 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048946a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0048946c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048946d */
    f->r[4]+=4;return;
}

void restore_actor_message_C_00489470(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00489470 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00489474 */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 00489478 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 0048947c */
    a=0x00010000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489481 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489482 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 00489486 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489487 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489488 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489489 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048948eu;f->boundary(0x00489489u,0x004893c0u,f);
    /* 0048948e */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00489491 */
    f->r[4]+=20;return;
}

void restore_actor_message_C_004894a0(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004894a0 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004894a4 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894a6 */
    f->r[0]=f->r[2];
    /* 004894a8 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894aa */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004894ad */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004894af */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004894b2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004894b9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004894c0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894c1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004894c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004894c7u;f->boundary(0x004894c2u,0x00489470u,f);
    /* 004894c7 */
    f->r[4]+=8;return;
}

void restore_actor_message_C_00495b60(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00495b60 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00495b64 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00495b65 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00495b66 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00495b69 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00495b6c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00495b6e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd656u+f->r[1]*4u));
    /* 00495b76 */
    f->r[1]=f->r[0];
    /* 00495b78 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00495b7b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00495b7d */
    f->r[0]=0x00000001u;
    /* 00495b82 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00495b85 */
    a=MI_U8((0x007999d8u+f->r[1]));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00495b8b */
    if(!(f->flags&MI_ZF))goto mi_00495bfc;
    /* 00495b8d */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[1])))&0xffffu)<<0);
    /* 00495b94 */
    a=(f->r[2]&0xffffu);b=0x0000004bu;t=mi_alu(f,a,b,16,1);
    /* 00495b98 */
    if((f->flags&MI_ZF))goto mi_00495ba1;
    /* 00495b9a */
    a=(f->r[2]&0xffffu);b=0x000000c0u;t=mi_alu(f,a,b,16,1);
    /* 00495b9f */
    if(!(f->flags&MI_ZF))goto mi_00495bfc;
mi_00495ba1:;
    /* 00495ba1 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00495ba3 */
    f->r[6]=(0x00799a86u+f->r[1]);
mi_00495ba9:;
    /* 00495ba9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 00495bac */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00495bae */
    if((f->flags&MI_ZF))goto mi_00495bf1;
    /* 00495bb0 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00495bb3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00495bb6 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00495bb8 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00495bbb */
    a=MI_U8((0x00bcd606u+f->r[1]));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00495bc1 */
    if(!(f->flags&MI_ZF))goto mi_00495bf1;
    /* 00495bc3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[1])))&0xffu)<<0);
    /* 00495bc9 */
    a=(f->r[1]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 00495bcc */
    if((f->flags&MI_ZF))goto mi_00495bfc;
    /* 00495bce */
    a=(f->r[1]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 00495bd1 */
    if((f->flags&MI_ZF))goto mi_00495bfc;
    /* 00495bd3 */
    a=(f->r[1]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 00495bd6 */
    if((f->flags&MI_ZF))goto mi_00495bfc;
    /* 00495bd8 */
    a=(f->r[1]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 00495bdb */
    if((f->flags&MI_ZF))goto mi_00495bfc;
    /* 00495bdd */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 00495be0 */
    if((f->flags&MI_ZF))goto mi_00495bff;
    /* 00495be2 */
    a=(f->r[1]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 00495be5 */
    if((f->flags&MI_ZF))goto mi_00495bff;
    /* 00495be7 */
    a=(f->r[1]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 00495bea */
    if((f->flags&MI_ZF))goto mi_00495bff;
    /* 00495bec */
    a=(f->r[1]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 00495bef */
    if((f->flags&MI_ZF))goto mi_00495bff;
mi_00495bf1:;
    /* 00495bf1 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00495bf2 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00495bf5 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00495bf8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00495ba9;
    /* 00495bfa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00495bfc:;
    /* 00495bfc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00495bfd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00495bfe */
    f->r[4]+=4;return;
mi_00495bff:;
    /* 00495bff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00495c00 */
    f->r[0]=0x00000002u;
    /* 00495c05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00495c06 */
    f->r[4]+=4;return;
}

void restore_actor_message_C_004bf9d0(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bf9d0 */
    f->r[0]=MI_U32((0x00032fb4u+f->r[1]));
    /* 004bf9d6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bf9d7 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004bf9da */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bf9db */
    if((f->flags&MI_ZF))goto mi_004bfa3a;
    /* 004bf9dd */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004bf9e0 */
    if((f->flags&MI_ZF))goto mi_004bfa3a;
    /* 004bf9e2 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004bf9e5 */
    if((f->flags&MI_ZF))goto mi_004bfa3a;
    /* 004bf9e7 */
    f->r[7]=0x00000001u;
    /* 004bf9ec */
    f->r[6]=0x00799b94u;
mi_004bf9f1:;
    /* 004bf9f1 */
    a=MI_U8((0xffffff4cu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004bf9f8 */
    if(!(f->flags&MI_ZF))goto mi_004bfa2b;
    /* 004bf9fa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bf9fb */
    f->r[1]=0x00bca350u;
    /* 004bfa00 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bfa05u;f->boundary(0x004bfa00u,0x00488030u,f);
    /* 004bfa05 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bfa07 */
    if((f->flags&MI_ZF))goto mi_004bfa2b;
    /* 004bfa09 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfa0a */
    f->r[1]=0x00bca350u;
    /* 004bfa0f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bfa14u;f->boundary(0x004bfa0fu,0x004894a0u,f);
    /* 004bfa14 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bfa16 */
    if((f->flags&MI_ZF))goto mi_004bfa2b;
    /* 004bfa18 */
    a=MI_U16((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004bfa1c */
    if(!(f->flags&MI_ZF))goto mi_004bfa2b;
    /* 004bfa1e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfa1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bfa24u;f->boundary(0x004bfa1fu,0x00459240u,f);
    /* 004bfa24 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bfa27 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bfa29 */
    if((f->flags&MI_ZF))goto mi_004bfa3f;
mi_004bfa2b:;
    /* 004bfa2b */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004bfa31 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004bfa32 */
    a=f->r[6];b=0x0089b78cu;t=mi_alu(f,a,b,32,1);
    /* 004bfa38 */
    if(mi_signed_less(f->flags))goto mi_004bf9f1;
mi_004bfa3a:;
    /* 004bfa3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bfa3b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004bfa3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bfa3e */
    f->r[4]+=4;return;
mi_004bfa3f:;
    /* 004bfa3f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bfa40 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004bfa42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bfa43 */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0053c060(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0053c060 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c061 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c062 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c063 */
    f->r[0]=(0x0000000bu+f->r[4]);
    /* 0053c067 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c068 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c069 */
    a=0x00f55ba4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c06e */
    MI_U8((0x00000017u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 0053c073 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c078u;f->boundary(0x0053c073u,0x0053d3a0u,f);
    /* 0053c078 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c07b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0053c07d */
    f->r[7]=f->r[0];
    /* 0053c07f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0053c081:;
    /* 0053c081 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c083 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00f55ba4u+f->r[1])))&0xffu)<<0);
    /* 0053c089 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053c08c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053c08f */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0053c092 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0053c093 */
    a=f->r[7];mi_idiv32(f,a);
    /* 0053c095 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0053c097 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0053c098 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0053c09b */
    if(mi_signed_less(f->flags))goto mi_0053c081;
    /* 0053c09d */
    f->r[5]=MI_U32((0x00d3b374u));
    /* 0053c0a3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000fu+f->r[4])))&0xffu)<<0);
    /* 0053c0a7 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0053c0a9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0053c0ab */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053c0ad */
    if((f->flags&MI_ZF))goto mi_0053c0b9;
    /* 0053c0af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c0b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c0b1 */
    f->r[0]=0x00000009u;
    /* 0053c0b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c0b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0053c0b8 */
    f->r[4]+=4;return;
mi_0053c0b9:;
    /* 0053c0b9 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0053c0bb:;
    /* 0053c0bb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c0bd */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00f55ba4u+f->r[6])))&0xffu)<<0);
    /* 0053c0c3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053c0c6 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053c0c9 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0053c0cc */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0053c0cd */
    a=f->r[7];mi_idiv32(f,a);
    /* 0053c0cf */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0053c0d1 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0053c0d3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c0e4;
    /* 0053c0d5 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0053c0d6 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0053c0d9 */
    if(mi_signed_less(f->flags))goto mi_0053c0bb;
    /* 0053c0db */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0053c0dd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c0e4;
    /* 0053c0df */
    f->r[6]=0x00000007u;
mi_0053c0e4:;
    /* 0053c0e4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00f55ba4u+f->r[6])))&0xffu)<<0);
    /* 0053c0ea */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0053c0ec */
    f->r[0]=0x00000008u;
    /* 0053c0f1 */
    if((f->flags&MI_ZF))goto mi_0053c0f5;
    /* 0053c0f3 */
    f->r[0]=f->r[6];
mi_0053c0f5:;
    /* 0053c0f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c0f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c0f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c0f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0053c0f9 */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0053c100(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0053c100 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0053c103 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c104 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c105 */
    f->r[5]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0053c109 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c10a */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0053c10c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c10d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053cef0;
    /* 0053c113 */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 0053c117 */
    f->r[0]=0x00000001u;
    /* 0053c11c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0053c11f */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0053c121 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c123 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0053c126 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c128 */
    f->r[1]=0x00785d70u;
    /* 0053c12d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000008u);
    /* 0053c135 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x0000000du);
    /* 0053c13d */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 0053c145 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0053c149 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x0000000bu);
    /* 0053c151 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0x00000005u);
    /* 0053c159 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0053c15d */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x0000000au);
    /* 0053c165 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0053c16c */
    MI_U32((0x00eadcdcu))=(uint32_t)(0xffffffffu);
    /* 0053c176 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c178 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c17du;f->boundary(0x0053c178u,0x0044bb90u,f);
    /* 0053c17d */
    f->r[7]=f->r[0];
    /* 0053c17f */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0053c181 */
    if((f->flags&MI_ZF))goto mi_0053c1a5;
mi_0053c183:;
    /* 0053c183 */
    f->r[1]=f->r[7];
    /* 0053c185 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c18au;f->boundary(0x0053c185u,0x0044d700u,f);
    /* 0053c18a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053c18c */
    if(!(f->flags&MI_ZF))goto mi_0053c1a3;
    /* 0053c18e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c190 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c191 */
    f->r[1]=0x00785d70u;
    /* 0053c196 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c19bu;f->boundary(0x0053c196u,0x0044bb90u,f);
    /* 0053c19b */
    f->r[7]=f->r[0];
    /* 0053c19d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0053c19f */
    if(!(f->flags&MI_ZF))goto mi_0053c183;
    /* 0053c1a1 */
    goto mi_0053c1a5;
mi_0053c1a3:;
    /* 0053c1a3 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
mi_0053c1a5:;
    /* 0053c1a5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c1a7 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0053c1ae */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c1b4 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053c1b5 */
    a=f->r[0];b=0x0000006bu;t=mi_alu(f,a,b,32,1);
    /* 0053c1b8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0053cedf;
    /* 0053c1be */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0053c1c0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x0053cf8cu+f->r[0]),1u))&0xffu)<<0);
    /* 0053c1c6 */
    switch(mi_read((0x0053cef8u+f->r[1]*4u),4u)){case 0x0053c1cdu:goto mi_0053c1cd;case 0x0053c1fau:goto mi_0053c1fa;case 0x0053c275u:goto mi_0053c275;case 0x0053c289u:goto mi_0053c289;case 0x0053c2bdu:goto mi_0053c2bd;case 0x0053c2f1u:goto mi_0053c2f1;case 0x0053c30du:goto mi_0053c30d;case 0x0053c32eu:goto mi_0053c32e;case 0x0053c35eu:goto mi_0053c35e;case 0x0053c3b1u:goto mi_0053c3b1;case 0x0053c3d0u:goto mi_0053c3d0;case 0x0053c424u:goto mi_0053c424;case 0x0053c443u:goto mi_0053c443;case 0x0053c462u:goto mi_0053c462;case 0x0053c481u:goto mi_0053c481;case 0x0053c4d1u:goto mi_0053c4d1;case 0x0053c4f0u:goto mi_0053c4f0;case 0x0053c524u:goto mi_0053c524;case 0x0053c5dcu:goto mi_0053c5dc;case 0x0053c685u:goto mi_0053c685;case 0x0053c707u:goto mi_0053c707;case 0x0053c7b8u:goto mi_0053c7b8;case 0x0053c8b8u:goto mi_0053c8b8;case 0x0053c8d7u:goto mi_0053c8d7;case 0x0053ca43u:goto mi_0053ca43;case 0x0053ca58u:goto mi_0053ca58;case 0x0053ca90u:goto mi_0053ca90;case 0x0053cab5u:goto mi_0053cab5;case 0x0053caefu:goto mi_0053caef;case 0x0053cb18u:goto mi_0053cb18;case 0x0053cbccu:goto mi_0053cbcc;case 0x0053cc66u:goto mi_0053cc66;case 0x0053ccd9u:goto mi_0053ccd9;case 0x0053cd75u:goto mi_0053cd75;case 0x0053cde0u:goto mi_0053cde0;case 0x0053ce7au:goto mi_0053ce7a;case 0x0053cedfu:goto mi_0053cedf;default:abort();}
mi_0053c1cd:;
    /* 0053c1cd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c1d3 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c1d5 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053c1d7 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053c1dd */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0053c1df */
    if((f->flags&MI_CF))goto mi_0053c1e8;
    /* 0053c1e1 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)(0x00000000u);
mi_0053c1e8:;
    /* 0053c1e8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c1ea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c1f0 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c1f5 */
    goto mi_0053ce7f;
mi_0053c1fa:;
    /* 0053c1fa */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053c200 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0053c206 */
    f->r[0]=mi_read((0x00ea5db0u+f->r[2]),4u);
    /* 0053c20c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053c20d */
    if((f->flags&MI_ZF))goto mi_0053c257;
    /* 0053c20f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053c210 */
    if((f->flags&MI_ZF))goto mi_0053c538;
    /* 0053c216 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053c217 */
    if((f->flags&MI_ZF))goto mi_0053c238;
    /* 0053c219 */
    f->r[0]=0x00000003u;
    /* 0053c21e */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0053c225 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c22b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c22c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c22d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c22e */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c233 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c234 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c237 */
    f->r[4]+=4;return;
mi_0053c238:;
    /* 0053c238 */
    f->r[0]=0x00000002u;
    /* 0053c23d */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000cu);
    /* 0053c244 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c24a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c24b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c24c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c24d */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c252 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c253 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c256 */
    f->r[4]+=4;return;
mi_0053c257:;
    /* 0053c257 */
    f->r[0]=0x00000001u;
    /* 0053c25c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c25d */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c263 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c269 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c26a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c26b */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c270 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c271 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c274 */
    f->r[4]+=4;return;
mi_0053c275:;
    /* 0053c275 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c27b */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c27d */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053c27f */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053c285 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0053c287 */
    goto mi_0053c29b;
mi_0053c289:;
    /* 0053c289 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c28f */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c291 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053c293 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053c299 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
mi_0053c29b:;
    /* 0053c29b */
    if((f->flags&MI_CF))goto mi_0053c2a4;
    /* 0053c29d */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)(0x00000000u);
mi_0053c2a4:;
    /* 0053c2a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c2a6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c2ac */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000du);
    /* 0053c2b3 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c2b8 */
    goto mi_0053ce7f;
mi_0053c2bd:;
    /* 0053c2bd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c2c3 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c2c5 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053c2c7 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053c2cd */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0053c2cf */
    if((f->flags&MI_CF))goto mi_0053c2d8;
    /* 0053c2d1 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)(0x00000000u);
mi_0053c2d8:;
    /* 0053c2d8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c2da */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c2e0 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 0053c2e7 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c2ec */
    goto mi_0053ce7f;
mi_0053c2f1:;
    /* 0053c2f1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c2f3 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 0053c2fa */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c300 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c301 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c302 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c303 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c308 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c309 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c30c */
    f->r[4]+=4;return;
mi_0053c30d:;
    /* 0053c30d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c313 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c315 */
    a=(f->r[1]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0053c318 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c319 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c31c */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c322 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c323 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c324 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c329 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c32a */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c32d */
    f->r[4]+=4;return;
mi_0053c32e:;
    /* 0053c32e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c330 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c336 */
    a=f->r[0];b=0xfffffff8u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053c339 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0053c33c */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0053ce7a;
    /* 0053c342 */
    switch(mi_read((0x0053cff8u+f->r[0]*4u),4u)){case 0x0053c349u:goto mi_0053c349;case 0x0053c55cu:goto mi_0053c55c;case 0x0053c66du:goto mi_0053c66d;default:abort();}
mi_0053c349:;
    /* 0053c349 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c34b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c34c */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c352 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c353 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c354 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c359 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c35a */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c35d */
    f->r[4]+=4;return;
mi_0053c35e:;
    /* 0053c35e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c360 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c366 */
    a=f->r[0];b=0xfffffff1u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053c369 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0053c36c */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0053c381;
    /* 0053c36e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0053c370 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x0053d018u+f->r[0]),1u))&0xffu)<<0);
    /* 0053c376 */
    switch(mi_read((0x0053d010u+f->r[1]*4u),4u)){case 0x0053c37du:goto mi_0053c37d;case 0x0053c381u:goto mi_0053c381;default:abort();}
mi_0053c37d:;
    /* 0053c37d */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0053c37f */
    goto mi_0053c383;
mi_0053c381:;
    /* 0053c381 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
mi_0053c383:;
    /* 0053c383 */
    f->r[1]=0x00d05720u;
    /* 0053c388 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c38du;f->boundary(0x0053c388u,0x004bf9d0u,f);
    /* 0053c38d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053c38f */
    if((f->flags&MI_ZF))goto mi_0053c3a9;
    /* 0053c391 */
    f->r[0]=0x0000000bu;
    /* 0053c396 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c397 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c39d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c39e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c39f */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c3a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c3a5 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c3a8 */
    f->r[4]+=4;return;
mi_0053c3a9:;
    /* 0053c3a9 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053c3ab */
    if((f->flags&MI_ZF))goto mi_0053ce7a;
mi_0053c3b1:;
    /* 0053c3b1 */
    f->r[0]=0x0000000cu;
    /* 0053c3b6 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 0053c3bd */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c3c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c3c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c3c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c3c6 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c3cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c3cc */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c3cf */
    f->r[4]+=4;return;
mi_0053c3d0:;
    /* 0053c3d0 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053c3d5 */
    f->r[1]=0x00000005u;
    /* 0053c3da */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053c3e0 */
    a=mi_read((0x00ea446cu+f->r[0]),4u);b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0053c3e6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c400;
mi_0053c3e8:;
    /* 0053c3e8 */
    f->r[0]=0x0000000au;
    /* 0053c3ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c3ee */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c3f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c3f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c3f6 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c3fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c3fc */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c3ff */
    f->r[4]+=4;return;
mi_0053c400:;
    /* 0053c400 */
    a=mi_read((0x00ea4474u+f->r[0]),4u);b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0053c406 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053ce7a;
    /* 0053c40c */
    f->r[0]=0x0000000bu;
    /* 0053c411 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c412 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c418 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c419 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c41a */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c41f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c420 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c423 */
    f->r[4]+=4;return;
mi_0053c424:;
    /* 0053c424 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c42a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c42c */
    a=(f->r[1]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 0053c42f */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c436 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c439 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c43e */
    goto mi_0053ce7f;
mi_0053c443:;
    /* 0053c443 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c449 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c44b */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0053c44e */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 0053c455 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c458 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c45d */
    goto mi_0053ce7f;
mi_0053c462:;
    /* 0053c462 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c468 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c46a */
    a=(f->r[1]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0053c46d */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 0053c474 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c477 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c47c */
    goto mi_0053ce7f;
mi_0053c481:;
    /* 0053c481 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c487 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0053c489 */
    if(!(f->flags&MI_ZF))goto mi_0053c4aa;
    /* 0053c48b */
    f->r[0]=0x0000000au;
    /* 0053c490 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c497 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c49d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c49e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c49f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c4a0 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c4a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c4a6 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c4a9 */
    f->r[4]+=4;return;
mi_0053c4aa:;
    /* 0053c4aa */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0053c4ac */
    if(!(f->flags&MI_ZF))goto mi_0053ce7a;
    /* 0053c4b2 */
    f->r[0]=0x0000000bu;
    /* 0053c4b7 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c4be */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c4c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c4c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c4c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c4c7 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c4cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c4cd */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c4d0 */
    f->r[4]+=4;return;
mi_0053c4d1:;
    /* 0053c4d1 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c4d7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c4d9 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0053c4dc */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c4e3 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c4e6 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c4eb */
    goto mi_0053ce7f;
mi_0053c4f0:;
    /* 0053c4f0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c4f6 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c4f8 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053c4fa */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053c500 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0053c502 */
    if((f->flags&MI_CF))goto mi_0053c50b;
    /* 0053c504 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)(0x00000000u);
mi_0053c50b:;
    /* 0053c50b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c50d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c513 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c51a */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c51f */
    goto mi_0053ce7f;
mi_0053c524:;
    /* 0053c524 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c52a */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0053c52c */
    if(!(f->flags&MI_ZF))goto mi_0053c574;
    /* 0053c52e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 0053c534 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0053c536 */
    if(!(f->flags&MI_ZF))goto mi_0053c554;
mi_0053c538:;
    /* 0053c538 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c53a */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0053c541 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c547 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c548 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c549 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c54a */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c54f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c550 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c553 */
    f->r[4]+=4;return;
mi_0053c554:;
    /* 0053c554 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0053c556 */
    if(!(f->flags&MI_ZF))goto mi_0053ce7a;
mi_0053c55c:;
    /* 0053c55c */
    f->r[0]=0x00000001u;
    /* 0053c561 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c562 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c568 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c569 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c56a */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c56f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c570 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c573 */
    f->r[4]+=4;return;
mi_0053c574:;
    /* 0053c574 */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0053c576 */
    if((f->flags&MI_ZF))goto mi_0053c584;
    /* 0053c578 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0053c57a */
    if((f->flags&MI_ZF))goto mi_0053c584;
    /* 0053c57c */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 0053c57e */
    if(!(f->flags&MI_ZF))goto mi_0053ce7a;
mi_0053c584:;
    /* 0053c584 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd624u+f->r[6]),2u);
    /* 0053c58b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 0053c592 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd626u+f->r[6]),2u);
    /* 0053c599 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 0053c5a0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0053c5a2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c5aa;
    /* 0053c5a4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0053c5a6 */
    f->r[0]=f->r[1];
    /* 0053c5a8 */
    goto mi_0053c5b2;
mi_0053c5aa:;
    /* 0053c5aa */
    if(!mi_signed_less(f->flags))goto mi_0053c5b0;
    /* 0053c5ac */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0053c5ae */
    goto mi_0053c5b2;
mi_0053c5b0:;
    /* 0053c5b0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0053c5b2:;
    /* 0053c5b2 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0053c5b4 */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[0]);
    /* 0053c5b9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c5c1;
    /* 0053c5bb */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0053c5bd */
    f->r[2]=f->r[7];
    /* 0053c5bf */
    goto mi_0053c5c9;
mi_0053c5c1:;
    /* 0053c5c1 */
    if(!mi_signed_less(f->flags))goto mi_0053c5c7;
    /* 0053c5c3 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0053c5c5 */
    goto mi_0053c5c9;
mi_0053c5c7:;
    /* 0053c5c7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0053c5c9:;
    /* 0053c5c9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0053c5cb */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[2]);
    /* 0053c5d1 */
    if(!mi_signed_less(f->flags))goto mi_0053c664;
    /* 0053c5d7 */
    goto mi_0053c662;
mi_0053c5dc:;
    /* 0053c5dc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c5e2 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0053c5e4 */
    if(!(f->flags&MI_ZF))goto mi_0053c60b;
    /* 0053c5e6 */
    a=mi_read((0x00bcd65eu+f->r[6]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0053c5ed */
    if(!(f->flags&MI_ZF))goto mi_0053ce7a;
    /* 0053c5f3 */
    f->r[0]=0x00000001u;
    /* 0053c5f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c5f9 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c5ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c600 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c601 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c606 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c607 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c60a */
    f->r[4]+=4;return;
mi_0053c60b:;
    /* 0053c60b */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0053c60d */
    if(!(f->flags&MI_ZF))goto mi_0053ce7a;
    /* 0053c613 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd624u+f->r[6]),2u);
    /* 0053c61a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 0053c621 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd626u+f->r[6]),2u);
    /* 0053c628 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 0053c62f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0053c631 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c639;
    /* 0053c633 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0053c635 */
    f->r[0]=f->r[1];
    /* 0053c637 */
    goto mi_0053c641;
mi_0053c639:;
    /* 0053c639 */
    if(!mi_signed_less(f->flags))goto mi_0053c63f;
    /* 0053c63b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0053c63d */
    goto mi_0053c641;
mi_0053c63f:;
    /* 0053c63f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0053c641:;
    /* 0053c641 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0053c643 */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[0]);
    /* 0053c648 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c650;
    /* 0053c64a */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0053c64c */
    f->r[2]=f->r[7];
    /* 0053c64e */
    goto mi_0053c658;
mi_0053c650:;
    /* 0053c650 */
    if(!mi_signed_less(f->flags))goto mi_0053c656;
    /* 0053c652 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0053c654 */
    goto mi_0053c658;
mi_0053c656:;
    /* 0053c656 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0053c658:;
    /* 0053c658 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0053c65a */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[2]);
    /* 0053c660 */
    if(!mi_signed_less(f->flags))goto mi_0053c664;
mi_0053c662:;
    /* 0053c662 */
    f->r[0]=f->r[2];
mi_0053c664:;
    /* 0053c664 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0053c667 */
    if(mi_signed_less(f->flags))goto mi_0053ce7a;
mi_0053c66d:;
    /* 0053c66d */
    f->r[0]=0x00000002u;
    /* 0053c672 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c673 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c679 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c67a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c67b */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c680 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c681 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c684 */
    f->r[4]+=4;return;
mi_0053c685:;
    /* 0053c685 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c68b */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 0053c68d */
    if((f->flags&MI_ZF))goto mi_0053c697;
    /* 0053c68f */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0053c691 */
    if(!(f->flags&MI_ZF))goto mi_0053ce7a;
mi_0053c697:;
    /* 0053c697 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd624u+f->r[6]),2u);
    /* 0053c69e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 0053c6a5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd626u+f->r[6]),2u);
    /* 0053c6ac */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 0053c6b3 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0053c6b5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c6bd;
    /* 0053c6b7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0053c6b9 */
    f->r[0]=f->r[1];
    /* 0053c6bb */
    goto mi_0053c6c5;
mi_0053c6bd:;
    /* 0053c6bd */
    if(!mi_signed_less(f->flags))goto mi_0053c6c3;
    /* 0053c6bf */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0053c6c1 */
    goto mi_0053c6c5;
mi_0053c6c3:;
    /* 0053c6c3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0053c6c5:;
    /* 0053c6c5 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0053c6c7 */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[0]);
    /* 0053c6cc */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053c6d4;
    /* 0053c6ce */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0053c6d0 */
    f->r[2]=f->r[7];
    /* 0053c6d2 */
    goto mi_0053c6dc;
mi_0053c6d4:;
    /* 0053c6d4 */
    if(!mi_signed_less(f->flags))goto mi_0053c6da;
    /* 0053c6d6 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0053c6d8 */
    goto mi_0053c6dc;
mi_0053c6da:;
    /* 0053c6da */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0053c6dc:;
    /* 0053c6dc */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0053c6de */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[2]);
    /* 0053c6e4 */
    if(!mi_signed_less(f->flags))goto mi_0053c6e8;
    /* 0053c6e6 */
    f->r[0]=f->r[2];
mi_0053c6e8:;
    /* 0053c6e8 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0053c6eb */
    if(mi_signed_less(f->flags))goto mi_0053c8a7;
    /* 0053c6f1 */
    f->r[0]=0x00000002u;
    /* 0053c6f6 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c6fd */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c702 */
    goto mi_0053ce7f;
mi_0053c707:;
    /* 0053c707 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c70d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c70f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd677u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c715 */
    a=(f->r[1]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 0053c718 */
    if(!(f->flags&MI_ZF))goto mi_0053c763;
    /* 0053c71a */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053c71d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0053c720 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0053c723 */
    f->r[1]=mi_read((0x0077af04u+f->r[0]),4u);
    /* 0053c729 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053c72b */
    if(!(f->flags&MI_ZF))goto mi_0053c74d;
    /* 0053c72d */
    f->r[1]=mi_read((0x0077af00u+f->r[0]),4u);
    /* 0053c733 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053c735 */
    if(!(f->flags&MI_ZF))goto mi_0053c74d;
    /* 0053c737 */
    f->r[0]=0x00000002u;
    /* 0053c73c */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c743 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c748 */
    goto mi_0053ce7f;
mi_0053c74d:;
    /* 0053c74d */
    f->r[0]=0x00000004u;
    /* 0053c752 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c759 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c75e */
    goto mi_0053ce7f;
mi_0053c763:;
    /* 0053c763 */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0053c766 */
    if(!(f->flags&MI_ZF))goto mi_0053c7a2;
    /* 0053c768 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c769 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c76eu;f->boundary(0x0053c769u,0x00495b60u,f);
    /* 0053c76e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c771 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0053c774 */
    if(!(f->flags&MI_ZF))goto mi_0053c787;
    /* 0053c776 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c77b */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c782 */
    goto mi_0053ce7f;
mi_0053c787:;
    /* 0053c787 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0053c78a */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c791 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0053c793 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0053c795 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0053c798 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c79d */
    goto mi_0053ce7f;
mi_0053c7a2:;
    /* 0053c7a2 */
    f->r[0]=0x00000003u;
    /* 0053c7a7 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c7ae */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c7b3 */
    goto mi_0053ce7f;
mi_0053c7b8:;
    /* 0053c7b8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c7be */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053c7c0 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053c7c2 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053c7c8 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0053c7ca */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_0053c7d3;
    /* 0053c7cc */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)(0x00000001u);
mi_0053c7d3:;
    /* 0053c7d3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c7d9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c7db */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c7e1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0053c7e3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd677u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c7e9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053c7ea */
    a=(f->r[2]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 0053c7ed */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c7f2 */
    if(!(f->flags&MI_ZF))goto mi_0053c82c;
    /* 0053c7f4 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0053c7f7 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 0053c7fa */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0053c7fd */
    f->r[2]=mi_read((0x0077af04u+f->r[1]),4u);
    /* 0053c803 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0053c805 */
    if(!(f->flags&MI_ZF))goto mi_0053c8ac;
    /* 0053c80b */
    f->r[2]=mi_read((0x0077af00u+f->r[1]),4u);
    /* 0053c811 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0053c813 */
    if(!(f->flags&MI_ZF))goto mi_0053c8ac;
    /* 0053c819 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c81b */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c822 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c827 */
    goto mi_0053ce7f;
mi_0053c82c:;
    /* 0053c82c */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0053c82f */
    if(!(f->flags&MI_ZF))goto mi_0053c8ac;
    /* 0053c831 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[6]),2u);
    /* 0053c838 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053c83e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c840 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd666u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c846 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c847 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c848 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c849 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c84a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c84fu;f->boundary(0x0053c84au,0x0045cf50u,f);
    /* 0053c84f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c852 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053c854 */
    if((f->flags&MI_ZF))goto mi_0053c86c;
    /* 0053c856 */
    f->r[0]=0x00000004u;
    /* 0053c85b */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c862 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c867 */
    goto mi_0053ce7f;
mi_0053c86c:;
    /* 0053c86c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[6]),2u);
    /* 0053c873 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053c879 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053c87b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd666u+f->r[6]),1u))&0xffu)<<0);
    /* 0053c881 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c882 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c883 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c884 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c885 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c88au;f->boundary(0x0053c885u,0x0045d180u,f);
    /* 0053c88a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c88d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053c88f */
    if((f->flags&MI_ZF))goto mi_0053c8a7;
    /* 0053c891 */
    f->r[0]=0x00000003u;
    /* 0053c896 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c89d */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c8a2 */
    goto mi_0053ce7f;
mi_0053c8a7:;
    /* 0053c8a7 */
    f->r[0]=mi_read((0x00eadcdcu),4u);
mi_0053c8ac:;
    /* 0053c8ac */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000006u);
    /* 0053c8b3 */
    goto mi_0053ce7f;
mi_0053c8b8:;
    /* 0053c8b8 */
    f->r[0]=0x0000000au;
    /* 0053c8bd */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000bu);
    /* 0053c8c4 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c8ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c8cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c8cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c8cd */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c8d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c8d3 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c8d6 */
    f->r[4]+=4;return;
mi_0053c8d7:;
    /* 0053c8d7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[6]),2u);
    /* 0053c8de */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c8df */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c8e4u;f->boundary(0x0053c8dfu,0x004c5120u,f);
    /* 0053c8e4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c8e7 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0053c8ea */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0053ce7a;
    /* 0053c8f0 */
    switch(mi_read((0x0053d028u+f->r[0]*4u),4u)){case 0x0053c8f7u:goto mi_0053c8f7;case 0x0053c930u:goto mi_0053c930;case 0x0053c968u:goto mi_0053c968;case 0x0053c9a0u:goto mi_0053c9a0;case 0x0053c9c7u:goto mi_0053c9c7;default:abort();}
mi_0053c8f7:;
    /* 0053c8f7 */
    a=mi_read((0x00d38374u),4u);b=0x0000003cu;t=mi_alu(f,a,b,32,1);
    /* 0053c8fe */
    if(!mi_signed_less(f->flags))goto mi_0053ce7a;
    /* 0053c904 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00d386dcu),1u))&0xffu)<<0);
    /* 0053c909 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053c90b */
    if((f->flags&MI_ZF))goto mi_0053ce7a;
mi_0053c911:;
    /* 0053c911 */
    f->r[0]=0x0000000bu;
mi_0053c916:;
    /* 0053c916 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
mi_0053c91b:;
    /* 0053c91b */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000du);
mi_0053c922:;
    /* 0053c922 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c928 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c929 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c92a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c92b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c92c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c92f */
    f->r[4]+=4;return;
mi_0053c930:;
    /* 0053c930 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0053c936 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0053c93c */
    a=mi_read((0x00ea5b48u+f->r[1]),4u);b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0053c943 */
    if(!mi_signed_less(f->flags))goto mi_0053ce7a;
    /* 0053c949 */
    f->r[0]=0x0000000bu;
    /* 0053c94e */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000005u);
    /* 0053c955 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c95b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c95c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c95d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c95e */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c963 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c964 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c967 */
    f->r[4]+=4;return;
mi_0053c968:;
    /* 0053c968 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053c96e */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0053c974 */
    a=mi_read((0x00ea4474u+f->r[2]),4u);b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0053c97b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053ce7a;
    /* 0053c981 */
    f->r[0]=0x0000000bu;
    /* 0053c986 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0053c98d */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c993 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c994 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c995 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c996 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c99b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c99c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c99f */
    f->r[4]+=4;return;
mi_0053c9a0:;
    /* 0053c9a0 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053c9a2 */
    if((f->flags&MI_ZF))goto mi_0053ce7a;
    /* 0053c9a8 */
    f->r[0]=0x0000000bu;
    /* 0053c9ad */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0053c9b4 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c9ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c9bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c9bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c9bd */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c9c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c9c3 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c9c6 */
    f->r[4]+=4;return;
mi_0053c9c7:;
    /* 0053c9c7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[6]),2u);
    /* 0053c9ce */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c9d0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053c9d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053c9d6u;f->boundary(0x0053c9d1u,0x004c52d0u,f);
    /* 0053c9d6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c9d9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053c9db */
    if((f->flags&MI_ZF))goto mi_0053c9fc;
    /* 0053c9dd */
    f->r[0]=0x0000000du;
    /* 0053c9e2 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 0053c9e9 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053c9ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053c9f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053c9f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053c9f2 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053c9f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053c9f8 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053c9fb */
    f->r[4]+=4;return;
mi_0053c9fc:;
    /* 0053c9fc */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053ca01 */
    f->r[1]=0x0000003cu;
    /* 0053ca06 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053ca0c */
    a=mi_read((0x00ea1880u+f->r[0]),4u);b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0053ca12 */
    if(mi_signed_less(f->flags))goto mi_0053c911;
    /* 0053ca18 */
    a=mi_read((0x00ea5bf4u+f->r[0]),4u);b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0053ca1e */
    if(!mi_signed_less(f->flags))goto mi_0053ce7a;
    /* 0053ca24 */
    f->r[0]=0x0000000cu;
    /* 0053ca29 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000au);
    /* 0053ca30 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ca36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053ca37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ca38 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053ca39 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ca3e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053ca3f */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053ca42 */
    f->r[4]+=4;return;
mi_0053ca43:;
    /* 0053ca43 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0053ca49 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0053ca4f */
    a=mi_read((0x00ea1888u+f->r[1]),4u);b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 0053ca56 */
    goto mi_0053ca6b;
mi_0053ca58:;
    /* 0053ca58 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053ca5e */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0053ca64 */
    a=mi_read((0x00ea1888u+f->r[2]),4u);b=0x0000001eu;t=mi_alu(f,a,b,32,1);
mi_0053ca6b:;
    /* 0053ca6b */
    if(!mi_signed_less(f->flags))goto mi_0053ce7a;
    /* 0053ca71 */
    f->r[0]=0x0000000au;
    /* 0053ca76 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 0053ca7d */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ca83 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053ca84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ca85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053ca86 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ca8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053ca8c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053ca8f */
    f->r[4]+=4;return;
mi_0053ca90:;
    /* 0053ca90 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0053ca92 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053ca94 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 0053ca9a */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 0053caa1 */
    a=f->r[1];b=0x00000320u;t=mi_alu(f,a,b,32,1);
    /* 0053caa7 */
    t=(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053caaa */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053caab */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cab0 */
    goto mi_0053ce7f;
mi_0053cab5:;
    /* 0053cab5 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053cabb */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053cabd */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053cabf */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053cac5 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 0053cac7 */
    if((f->flags&MI_CF))goto mi_0053cad0;
    /* 0053cac9 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)(0x00000000u);
mi_0053cad0:;
    /* 0053cad0 */
    a=mi_read((0x00bcd630u+f->r[6]),1u);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 0053cad7 */
    if(!(f->flags&MI_ZF))goto mi_0053ce7a;
    /* 0053cadd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053cadf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053cae5 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053caea */
    goto mi_0053ce7f;
mi_0053caef:;
    /* 0053caef */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0053caf1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053caf3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 0053caf9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053cafa */
    a=f->r[2];b=0x00000258u;t=mi_alu(f,a,b,32,1);
    /* 0053cb00 */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053cb03 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053cb06 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053cb0c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053cb0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053cb0e */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cb13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053cb14 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053cb17 */
    f->r[4]+=4;return;
mi_0053cb18:;
    /* 0053cb18 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053cb1e */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053cb20 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0053cb22 */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053cb28 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0053cb2a */
    if((f->flags&MI_CF))goto mi_0053cb33;
    /* 0053cb2c */
    MI_U8((0x00bcd674u+f->r[6]))=(uint8_t)(0x00000000u);
mi_0053cb33:;
    /* 0053cb33 */
    a=mi_read((0x00bcd630u+f->r[6]),1u);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0053cb3a */
    if(!(f->flags&MI_ZF))goto mi_0053cb9f;
    /* 0053cb3c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd67au+f->r[6]),2u);
    /* 0053cb43 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0053cb46 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0053cb49 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0053cb4b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0053cb52 */
    a=(f->r[1]&0xffu);b=0x00000017u;t=mi_alu(f,a,b,8,1);
    /* 0053cb55 */
    if((f->flags&MI_ZF))goto mi_0053ccb7;
    /* 0053cb5b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd67cu+f->r[6]),2u);
    /* 0053cb62 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0053cb65 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0053cb68 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0053cb6a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 0053cb71 */
    a=(f->r[0]&0xffu);b=0x00000017u;t=mi_alu(f,a,b,8,1);
    /* 0053cb73 */
    if((f->flags&MI_ZF))goto mi_0053ccb7;
    /* 0053cb79 */
    a=(f->r[1]&0xffu);b=0x00000016u;t=mi_alu(f,a,b,8,1);
    /* 0053cb7c */
    if((f->flags&MI_ZF))goto mi_0053ccb7;
    /* 0053cb82 */
    a=(f->r[0]&0xffu);b=0x00000016u;t=mi_alu(f,a,b,8,1);
    /* 0053cb84 */
    if((f->flags&MI_ZF))goto mi_0053ccb7;
    /* 0053cb8a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053cb8c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd674u+f->r[6]),1u))&0xffu)<<0);
    /* 0053cb92 */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053cb95 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cb9a */
    goto mi_0053ce7f;
mi_0053cb9f:;
    /* 0053cb9f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 0053cba5 */
    f->r[1]=f->r[0];
    /* 0053cba7 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0053cbad */
    a=f->r[1];b=0x000001f4u;t=mi_alu(f,a,b,32,1);
    /* 0053cbb3 */
    if(mi_signed_less(f->flags))goto mi_0053cbbf;
    /* 0053cbb5 */
    f->r[0]=0x0000000cu;
    /* 0053cbba */
    goto mi_0053c916;
mi_0053cbbf:;
    /* 0053cbbf */
    a=(f->r[0]&0xffu);b=0x00000064u;t=mi_alu(f,a,b,8,1);
    /* 0053cbc1 */
    if((f->flags&MI_CF))goto mi_0053c3e8;
    /* 0053cbc7 */
    goto mi_0053c911;
mi_0053cbcc:;
    /* 0053cbcc */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053cbd1 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053cbd7 */
    f->r[1]=mi_read((0x00ea4494u+f->r[0]),4u);
    /* 0053cbdd */
    f->r[2]=mi_read((0x00ea4490u+f->r[0]),4u);
    /* 0053cbe3 */
    f->r[7]=mi_read((0x00ea449cu+f->r[0]),4u);
    /* 0053cbe9 */
    f->r[5]=mi_read((0x00ea4498u+f->r[0]),4u);
    /* 0053cbef */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0053cbf1 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0053cbf3 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0053cbf5 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0053cbf8 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[2]*8u);
    /* 0053cbfb */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0053cbfe */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0053cbff */
    a=f->r[7];mi_idiv32(f,a);
    /* 0053cc01 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 0053cc04 */
    if(!mi_signed_less(f->flags))goto mi_0053cc1c;
    /* 0053cc06 */
    f->r[0]=0x0000000au;
    /* 0053cc0b */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000cu);
    /* 0053cc12 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cc17 */
    goto mi_0053ce7f;
mi_0053cc1c:;
    /* 0053cc1c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 0053cc22 */
    a=(f->r[0]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 0053cc24 */
    if((f->flags&MI_CF))goto mi_0053cc3c;
    /* 0053cc26 */
    f->r[0]=0x0000000bu;
    /* 0053cc2b */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000cu);
    /* 0053cc32 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cc37 */
    goto mi_0053ce7f;
mi_0053cc3c:;
    /* 0053cc3c */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0053cc3e */
    if((f->flags&MI_CF))goto mi_0053cc55;
    /* 0053cc40 */
    f->r[0]=0x0000000cu;
    /* 0053cc45 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cc4a */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053cc50 */
    goto mi_0053ce7f;
mi_0053cc55:;
    /* 0053cc55 */
    f->r[0]=mi_read((0x00eadcdcu),4u);
    /* 0053cc5a */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x0000000cu);
    /* 0053cc61 */
    goto mi_0053ce7f;
mi_0053cc66:;
    /* 0053cc66 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053cc6b */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053cc71 */
    f->r[1]=mi_read((0x00ea6090u+f->r[0]),4u);
    /* 0053cc77 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053cc79 */
    if((f->flags&MI_ZF))goto mi_0053cc93;
    /* 0053cc7b */
    f->r[0]=0x0000000bu;
    /* 0053cc80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053cc81 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053cc87 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053cc88 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053cc89 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cc8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053cc8f */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053cc92 */
    f->r[4]+=4;return;
mi_0053cc93:;
    /* 0053cc93 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 0053cc99 */
    f->r[1]=f->r[0];
    /* 0053cc9b */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0053cca1 */
    a=f->r[1];b=0x000001f4u;t=mi_alu(f,a,b,32,1);
    /* 0053cca7 */
    if(mi_signed_less(f->flags))goto mi_0053ccb3;
    /* 0053cca9 */
    f->r[0]=0x0000000eu;
    /* 0053ccae */
    goto mi_0053c916;
mi_0053ccb3:;
    /* 0053ccb3 */
    a=(f->r[0]&0xffu);b=0x00000064u;t=mi_alu(f,a,b,8,1);
    /* 0053ccb5 */
    if((f->flags&MI_CF))goto mi_0053ccc1;
mi_0053ccb7:;
    /* 0053ccb7 */
    f->r[0]=0x0000000du;
    /* 0053ccbc */
    goto mi_0053c916;
mi_0053ccc1:;
    /* 0053ccc1 */
    f->r[0]=0x0000000cu;
    /* 0053ccc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053ccc7 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053cccd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ccce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053cccf */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ccd4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053ccd5 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053ccd8 */
    f->r[4]+=4;return;
mi_0053ccd9:;
    /* 0053ccd9 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053ccde */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053cce4 */
    f->r[1]=mi_read((0x00ea5ea4u+f->r[0]),4u);
    /* 0053ccea */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053ccec */
    if((f->flags&MI_ZF))goto mi_0053cd2d;
    /* 0053ccee */
    a=mi_read((0x00bcd630u+f->r[6]),1u);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0053ccf5 */
    if(!(f->flags&MI_ZF))goto mi_0053cd0a;
    /* 0053ccf7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053ccf9 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0053cd00 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cd05 */
    goto mi_0053ce7f;
mi_0053cd0a:;
    /* 0053cd0a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053cd0c */
    if((f->flags&MI_ZF))goto mi_0053cd2d;
    /* 0053cd0e */
    a=mi_read((0x00bcd630u+f->r[6]),1u);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0053cd15 */
    if(!(f->flags&MI_ZF))goto mi_0053cd2d;
    /* 0053cd17 */
    f->r[0]=0x00000001u;
    /* 0053cd1c */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0053cd23 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cd28 */
    goto mi_0053ce7f;
mi_0053cd2d:;
    /* 0053cd2d */
    f->r[1]=mi_read((0x00ea6210u+f->r[0]),4u);
    /* 0053cd33 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053cd35 */
    if((f->flags&MI_ZF))goto mi_0053cd5b;
    /* 0053cd37 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0053cd3d */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0053cd3f */
    if((f->flags&MI_ZF))goto mi_0053cd5b;
    /* 0053cd41 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0053cd43 */
    if((f->flags&MI_ZF))goto mi_0053cd5b;
    /* 0053cd45 */
    f->r[0]=0x00000002u;
    /* 0053cd4a */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0053cd51 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cd56 */
    goto mi_0053ce7f;
mi_0053cd5b:;
    /* 0053cd5b */
    a=(f->r[3]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053cd5d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,2);f->r[3]=t;
    /* 0053cd5f */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0053cd66 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0053cd69 */
    f->r[0]=f->r[3];
    /* 0053cd6b */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cd70 */
    goto mi_0053ce7f;
mi_0053cd75:;
    /* 0053cd75 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053cd7a */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053cd80 */
    f->r[1]=mi_read((0x00ea5ea4u+f->r[0]),4u);
    /* 0053cd86 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053cd88 */
    if((f->flags&MI_ZF))goto mi_0053cda6;
    /* 0053cd8a */
    a=mi_read((0x00bcd630u+f->r[6]),1u);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0053cd91 */
    if(!(f->flags&MI_ZF))goto mi_0053cda6;
    /* 0053cd93 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053cd95 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0053cd9c */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cda1 */
    goto mi_0053ce7f;
mi_0053cda6:;
    /* 0053cda6 */
    f->r[1]=mi_read((0x00ea6210u+f->r[0]),4u);
    /* 0053cdac */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0053cdae */
    if((f->flags&MI_ZF))goto mi_0053cdc6;
    /* 0053cdb0 */
    f->r[0]=0x00000001u;
    /* 0053cdb5 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0053cdbc */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cdc1 */
    goto mi_0053ce7f;
mi_0053cdc6:;
    /* 0053cdc6 */
    a=(f->r[3]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0053cdc8 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,2);f->r[3]=t;
    /* 0053cdca */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0053cdd1 */
    a=f->r[3];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0053cdd4 */
    f->r[0]=f->r[3];
    /* 0053cdd6 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053cddb */
    goto mi_0053ce7f;
mi_0053cde0:;
    /* 0053cde0 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053cde6 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0053cdec */
    f->r[0]=mi_read((0x00ea5ea4u+f->r[2]),4u);
    /* 0053cdf2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053cdf4 */
    if(!(f->flags&MI_ZF))goto mi_0053ce10;
    /* 0053cdf6 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0053cdfd */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ce03 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053ce04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ce05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053ce06 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ce0b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053ce0c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053ce0f */
    f->r[4]+=4;return;
mi_0053ce10:;
    /* 0053ce10 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053ce12 */
    if((f->flags&MI_ZF))goto mi_0053ce33;
    /* 0053ce14 */
    f->r[0]=0x00000001u;
    /* 0053ce19 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0053ce20 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ce26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053ce27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ce28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053ce29 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ce2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053ce2f */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053ce32 */
    f->r[4]+=4;return;
mi_0053ce33:;
    /* 0053ce33 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0053ce36 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0053ce56;
    /* 0053ce38 */
    f->r[0]=0x00000002u;
    /* 0053ce3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053ce3e */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ce44 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ce4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ce4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053ce4c */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ce51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053ce52 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053ce55 */
    f->r[4]+=4;return;
mi_0053ce56:;
    /* 0053ce56 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0053ce59 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0053ce7a;
    /* 0053ce5b */
    f->r[0]=0x00000003u;
    /* 0053ce60 */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0053ce67 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ce6d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053ce6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ce6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053ce70 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ce75 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053ce76 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053ce79 */
    f->r[4]+=4;return;
mi_0053ce7a:;
    /* 0053ce7a */
    f->r[0]=mi_read((0x00eadcdcu),4u);
mi_0053ce7f:;
    /* 0053ce7f */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0053ce82 */
    if(!(f->flags&MI_ZF))goto mi_0053c922;
    /* 0053ce88 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0053ce8a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[6]),1u))&0xffu)<<0);
    /* 0053ce90 */
    a=f->r[1];b=0xfffffffbu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0053ce93 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0053ce96 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0053c922;
    /* 0053ce9c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0053ce9e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0053d044u+f->r[1]),1u))&0xffu)<<0);
    /* 0053cea4 */
    switch(mi_read((0x0053d03cu+f->r[2]*4u),4u)){case 0x0053c922u:goto mi_0053c922;case 0x0053ceabu:goto mi_0053ceab;default:abort();}
mi_0053ceab:;
    /* 0053ceab */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053ceb0u;f->boundary(0x0053ceabu,0x0053c060u,f);
    /* 0053ceb0 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0053ceb3 */
    MI_U32((0x00eadcdcu))=(uint32_t)(f->r[0]);
    /* 0053ceb8 */
    if((f->flags&MI_ZF))goto mi_0053c91b;
    /* 0053cebe */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0053cec1 */
    if((f->flags&MI_ZF))goto mi_0053c91b;
    /* 0053cec7 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000010u+f->r[4]+f->r[0]*4u),1u))&0xffu)<<0);
    /* 0053cecb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053cecc */
    MI_U8((0x00bcd729u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0053ced2 */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0053ced8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053ced9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053ceda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053cedb */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053cede */
    f->r[4]+=4;return;
mi_0053cedf:;
    /* 0053cedf */
    MI_U8((0x00bcd675u+f->r[6]))=(uint8_t)(0x000000ffu);
    /* 0053cee6 */
    MI_U32((0x00eadcdcu))=(uint32_t)(0xffffffffu);
mi_0053cef0:;
    /* 0053cef0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053cef1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053cef2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053cef3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053cef4 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053cef7 */
    f->r[4]+=4;return;
}

void restore_actor_message_C_0053d3a0(struct restore_actor_message_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0053d3a0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0053d3a3 */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0053d3a7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053d3a9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3aa */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3ab */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0053d3ad */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3ae */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0053d3b0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3b1 */
    MI_U32((0x00000004u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0053d3b4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3b5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3b6 */
    f->r[1]=0x00785d70u;
    /* 0053d3bb */
    MI_U8((0x0000001bu+f->r[4]))=(uint8_t)(0x00000000u);
    /* 0053d3c0 */
    MI_U8((0x0000001au+f->r[4]))=(uint8_t)(0x00000000u);
    /* 0053d3c5 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0053d3c7 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0053d3cb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053d3d0u;f->boundary(0x0053d3cbu,0x0044bb90u,f);
    /* 0053d3d0 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0053d3d2 */
    if((f->flags&MI_ZF))goto mi_0053d404;
mi_0053d3d4:;
    /* 0053d3d4 */
    f->r[2]=mi_read((0x00000028u+f->r[0]),4u);
    /* 0053d3d7 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 0053d3da */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0053d3dd */
    if(!(f->flags&MI_ZF))goto mi_0053d3f1;
    /* 0053d3df */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3e0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d3e1 */
    f->r[1]=0x00785d70u;
    /* 0053d3e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053d3ebu;f->boundary(0x0053d3e6u,0x0044bb90u,f);
    /* 0053d3eb */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0053d3ed */
    if(!(f->flags&MI_ZF))goto mi_0053d3d4;
    /* 0053d3ef */
    goto mi_0053d404;
mi_0053d3f1:;
    /* 0053d3f1 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0053d3f3 */
    if((f->flags&MI_ZF))goto mi_0053d404;
    /* 0053d3f5 */
    f->r[0]=mi_read((0x00000028u+f->r[0]),4u);
    /* 0053d3f8 */
    a=f->r[0];t=a>>2u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>1u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0053d3fb */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0053d3fd */
    if((f->flags&MI_ZF))goto mi_0053d404;
    /* 0053d3ff */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
mi_0053d404:;
    /* 0053d404 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d405 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d406 */
    f->r[1]=0x00785d70u;
    /* 0053d40b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053d410u;f->boundary(0x0053d40bu,0x0044bb90u,f);
    /* 0053d410 */
    f->r[6]=f->r[0];
    /* 0053d412 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0053d414 */
    if((f->flags&MI_ZF))goto mi_0053d43a;
mi_0053d416:;
    /* 0053d416 */
    f->r[1]=f->r[6];
    /* 0053d418 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053d41du;f->boundary(0x0053d418u,0x0044d700u,f);
    /* 0053d41d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053d41f */
    if(!(f->flags&MI_ZF))goto mi_0053d435;
    /* 0053d421 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d422 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053d423 */
    f->r[1]=0x00785d70u;
    /* 0053d428 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053d42du;f->boundary(0x0053d428u,0x0044bb90u,f);
    /* 0053d42d */
    f->r[6]=f->r[0];
    /* 0053d42f */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0053d431 */
    if(!(f->flags&MI_ZF))goto mi_0053d416;
    /* 0053d433 */
    goto mi_0053d43a;
mi_0053d435:;
    /* 0053d435 */
    MI_U8((0x00000012u+f->r[4]))=(uint8_t)(0x00000001u);
mi_0053d43a:;
    /* 0053d43a */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
mi_0053d43e:;
    /* 0053d43e */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0053d440 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053d442 */
    a=f->r[5];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0053d445 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0053d6bf;
    /* 0053d44b */
    switch(mi_read((0x0053d710u+f->r[5]*4u),4u)){case 0x0053d452u:goto mi_0053d452;case 0x0053d48cu:goto mi_0053d48c;case 0x0053d4c5u:goto mi_0053d4c5;case 0x0053d505u:goto mi_0053d505;case 0x0053d58eu:goto mi_0053d58e;case 0x0053d5deu:goto mi_0053d5de;case 0x0053d638u:goto mi_0053d638;case 0x0053d68cu:goto mi_0053d68c;default:abort();}
mi_0053d452:;
    /* 0053d452 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053d458 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0053d45e */
    f->r[0]=mi_read((0x00ea1888u+f->r[2]),4u);
    /* 0053d464 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0053d467 */
    if(!mi_signed_less(f->flags))goto mi_0053d476;
    /* 0053d469 */
    f->r[3]=0x00000008u;
    /* 0053d46e */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d471 */
    goto mi_0053d6bf;
mi_0053d476:;
    /* 0053d476 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0053d479 */
    if(!mi_signed_less(f->flags))goto mi_0053d622;
    /* 0053d47f */
    f->r[3]=0x00000006u;
    /* 0053d484 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d487 */
    goto mi_0053d6bf;
mi_0053d48c:;
    /* 0053d48c */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053d491 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053d497 */
    f->r[0]=mi_read((0x00ea4388u+f->r[0]),4u);
    /* 0053d49d */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0053d4a0 */
    if(!mi_signed_less(f->flags))goto mi_0053d4af;
    /* 0053d4a2 */
    f->r[3]=0x00000006u;
    /* 0053d4a7 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d4aa */
    goto mi_0053d6bf;
mi_0053d4af:;
    /* 0053d4af */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0053d4b2 */
    if(!mi_signed_less(f->flags))goto mi_0053d6ba;
    /* 0053d4b8 */
    f->r[3]=0x00000004u;
    /* 0053d4bd */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d4c0 */
    goto mi_0053d6bf;
mi_0053d4c5:;
    /* 0053d4c5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000012u+f->r[4]),1u))&0xffu)<<0);
    /* 0053d4c9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053d4cb */
    if((f->flags&MI_ZF))goto mi_0053d6ba;
    /* 0053d4d1 */
    f->r[0]=mi_read((0x00bcb9b4u),4u);
    /* 0053d4d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053d4d8 */
    if(!(f->flags&MI_ZF))goto mi_0053d6bf;
    /* 0053d4de */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053d4e4 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0053d4ea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ea5ed8u+f->r[2]),1u))&0xffu)<<0);
    /* 0053d4f0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053d4f2 */
    if(!(f->flags&MI_ZF))goto mi_0053d6bf;
    /* 0053d4f8 */
    f->r[3]=0x00000008u;
    /* 0053d4fd */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d500 */
    goto mi_0053d6bf;
mi_0053d505:;
    /* 0053d505 */
    f->r[7]=mi_read((0x00eaac28u),4u);
    /* 0053d50b */
    f->r[3]=0x0000000au;
    /* 0053d510 */
    a=f->r[7];b=0x000049d8u;t=mi_imul(f,a,b);f->r[7]=t;
    /* 0053d516 */
    f->r[2]=(0x00ea43a0u+f->r[7]);
mi_0053d51c:;
    /* 0053d51c */
    a=f->r[0];b=mi_read((0x00000000u+f->r[2]),4u);t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053d51e */
    a=f->r[2];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0053d521 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0053d522 */
    if(!(f->flags&MI_ZF))goto mi_0053d51c;
    /* 0053d524 */
    f->r[2]=(0x00ea439cu+f->r[7]);
    /* 0053d52a */
    f->r[7]=0x0000000au;
mi_0053d52f:;
    /* 0053d52f */
    f->r[3]=mi_read((0x00000000u+f->r[2]),4u);
    /* 0053d531 */
    a=f->r[2];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0053d534 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0053d536 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0053d537 */
    if(!(f->flags&MI_ZF))goto mi_0053d52f;
    /* 0053d539 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0053d53b */
    if((f->flags&MI_ZF))goto mi_0053d6ba;
    /* 0053d541 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053d544 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053d547 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0053d54a */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0053d54b */
    a=f->r[6];mi_idiv32(f,a);
    /* 0053d54d */
    f->r[2]=0x00000064u;
    /* 0053d552 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0053d554 */
    a=f->r[2];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0053d557 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053d566;
    /* 0053d559 */
    f->r[3]=0x00000008u;
    /* 0053d55e */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d561 */
    goto mi_0053d6bf;
mi_0053d566:;
    /* 0053d566 */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0053d569 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053d578;
    /* 0053d56b */
    f->r[3]=0x00000006u;
    /* 0053d570 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d573 */
    goto mi_0053d6bf;
mi_0053d578:;
    /* 0053d578 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0053d57b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053d6ba;
    /* 0053d581 */
    f->r[3]=0x00000004u;
    /* 0053d586 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d589 */
    goto mi_0053d6bf;
mi_0053d58e:;
    /* 0053d58e */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053d593 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0053d595 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053d59b */
    f->r[6]=0x00000064u;
    /* 0053d5a0 */
    f->r[7]=(0x00ea5c0au+f->r[0]);
mi_0053d5a6:;
    /* 0053d5a6 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00000000u+f->r[7]+f->r[2]*1u),1u);
    /* 0053d5aa */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0053d5ac */
    if(!mi_signed_less(f->flags))goto mi_0053d5b0;
    /* 0053d5ae */
    f->r[6]=f->r[0];
mi_0053d5b0:;
    /* 0053d5b0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0053d5b1 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0053d5b4 */
    if(mi_signed_less(f->flags))goto mi_0053d5a6;
    /* 0053d5b6 */
    a=f->r[6];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0053d5b9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0053d5c8;
    /* 0053d5bb */
    f->r[3]=0x00000006u;
    /* 0053d5c0 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d5c3 */
    goto mi_0053d6bf;
mi_0053d5c8:;
    /* 0053d5c8 */
    a=f->r[6];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0053d5cb */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0053d6ba;
    /* 0053d5d1 */
    f->r[3]=0x00000004u;
    /* 0053d5d6 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d5d9 */
    goto mi_0053d6bf;
mi_0053d5de:;
    /* 0053d5de */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 0053d5e2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0053d5e4 */
    if((f->flags&MI_ZF))goto mi_0053d5ed;
    /* 0053d5e6 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0053d5e8 */
    goto mi_0053d6bf;
mi_0053d5ed:;
    /* 0053d5ed */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0053d5f3 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0053d5f9 */
    f->r[0]=mi_read((0x00ea5b48u+f->r[2]),4u);
    /* 0053d5ff */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053d601 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0053d610;
    /* 0053d603 */
    f->r[3]=0x00000008u;
    /* 0053d608 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d60b */
    goto mi_0053d6bf;
mi_0053d610:;
    /* 0053d610 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0053d613 */
    if(!mi_signed_less(f->flags))goto mi_0053d622;
    /* 0053d615 */
    f->r[3]=0x00000006u;
    /* 0053d61a */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d61d */
    goto mi_0053d6bf;
mi_0053d622:;
    /* 0053d622 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 0053d625 */
    if(!mi_signed_less(f->flags))goto mi_0053d6ba;
    /* 0053d62b */
    f->r[3]=0x00000004u;
    /* 0053d630 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d633 */
    goto mi_0053d6bf;
mi_0053d638:;
    /* 0053d638 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053d63d */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053d643 */
    f->r[6]=mi_read((0x00ea1894u+f->r[0]),4u);
    /* 0053d649 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0053d64b */
    if((f->flags&MI_ZF))goto mi_0053d6ba;
    /* 0053d64d */
    f->r[0]=mi_read((0x00ea4468u+f->r[0]),4u);
    /* 0053d653 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053d656 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0053d659 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0053d65c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0053d65d */
    a=f->r[6];mi_idiv32(f,a);
    /* 0053d65f */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 0053d662 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053d66e;
    /* 0053d664 */
    f->r[3]=0x00000008u;
    /* 0053d669 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d66c */
    goto mi_0053d6bf;
mi_0053d66e:;
    /* 0053d66e */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0053d671 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053d67d;
    /* 0053d673 */
    f->r[3]=0x00000006u;
    /* 0053d678 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d67b */
    goto mi_0053d6bf;
mi_0053d67d:;
    /* 0053d67d */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0053d680 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053d6ba;
    /* 0053d682 */
    f->r[3]=0x00000004u;
    /* 0053d687 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d68a */
    goto mi_0053d6bf;
mi_0053d68c:;
    /* 0053d68c */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0053d691 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0053d697 */
    f->r[0]=mi_read((0x00ea5bf4u+f->r[0]),4u);
    /* 0053d69d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053d69f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0053d6ab;
    /* 0053d6a1 */
    f->r[3]=0x00000006u;
    /* 0053d6a6 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d6a9 */
    goto mi_0053d6bf;
mi_0053d6ab:;
    /* 0053d6ab */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0053d6ae */
    if(!mi_signed_less(f->flags))goto mi_0053d6ba;
    /* 0053d6b0 */
    f->r[3]=0x00000004u;
    /* 0053d6b5 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0053d6b8 */
    goto mi_0053d6bf;
mi_0053d6ba:;
    /* 0053d6ba */
    f->r[3]=0x00000001u;
mi_0053d6bf:;
    /* 0053d6bf */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0053d6c3 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0053d6c7 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0053d6c9 */
    MI_U8((0x00000000u+f->r[6]+f->r[5]*1u))=(uint8_t)((f->r[3]&0xffu));
    /* 0053d6cc */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0053d6cd */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0053d6d0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0053d6d4 */
    if(mi_signed_less(f->flags))goto mi_0053d43e;
    /* 0053d6da */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000003u+f->r[6]),1u))&0xffu)<<0);
    /* 0053d6dd */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0053d6df */
    if(!(f->flags&MI_ZF))goto mi_0053d6f7;
    /* 0053d6e1 */
    a=mi_read((0x00000006u+f->r[6]),1u);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0053d6e4 */
    if((f->flags&MI_CF))goto mi_0053d6f5;
    /* 0053d6e6 */
    MI_U8((0x00000003u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0053d6ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053d6eb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0053d6ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053d6ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053d6ee */
    f->r[0]=f->r[2];
    /* 0053d6f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053d6f1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053d6f4 */
    f->r[4]+=4;return;
mi_0053d6f5:;
    /* 0053d6f5 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
mi_0053d6f7:;
    /* 0053d6f7 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_0053d704;
    /* 0053d6f9 */
    a=mi_read((0x00000006u+f->r[6]),1u);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0053d6fd */
    if(!(f->flags&MI_ZF))goto mi_0053d704;
    /* 0053d6ff */
    MI_U8((0x00000006u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0053d703 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_0053d704:;
    /* 0053d704 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0053d705 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0053d706 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053d707 */
    f->r[0]=f->r[2];
    /* 0053d709 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053d70a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053d70d */
    f->r[4]+=4;return;
}

__declspec(noinline) __declspec(noreturn) void restore_actor_message_failure(uint32_t target)
{fprintf(stderr,"Unsupported actor-message boundary %08lx\n",(unsigned long)target);abort();}
void restore_actor_message_dispatch(uint32_t site,uint32_t target,struct restore_actor_message_frame *frame)
{
 uint32_t sp=frame->r[4];(void)site;
 switch(target){

 case 0x004434e0u:restore_actor_message_C_004434e0(frame);return;
 case 0x004435f0u:restore_actor_message_C_004435f0(frame);return;
 case 0x0044d6d0u:restore_actor_message_C_0044d6d0(frame);return;
 case 0x0044d700u:restore_actor_message_C_0044d700(frame);return;
 case 0x00459240u:restore_actor_message_C_00459240(frame);return;
 case 0x0045b230u:restore_actor_message_C_0045b230(frame);return;
 case 0x0045cf50u:restore_actor_message_C_0045cf50(frame);return;
 case 0x0045d180u:restore_actor_message_C_0045d180(frame);return;
 case 0x0045d510u:restore_actor_message_C_0045d510(frame);return;
 case 0x004893c0u:restore_actor_message_C_004893c0(frame);return;
 case 0x00489470u:restore_actor_message_C_00489470(frame);return;
 case 0x004894a0u:restore_actor_message_C_004894a0(frame);return;
 case 0x00495b60u:restore_actor_message_C_00495b60(frame);return;
 case 0x004bf9d0u:restore_actor_message_C_004bf9d0(frame);return;
 case 0x0053c060u:restore_actor_message_C_0053c060(frame);return;
 case 0x0053c100u:restore_actor_message_C_0053c100(frame);return;
 case 0x0053d3a0u:restore_actor_message_C_0053d3a0(frame);return;
 /* Native iterator ECX is explicit;RET8. Only EAX is consumed by these callers. */
 case 0x44bb90u:frame->r[0]=(uint32_t)(uintptr_t)restore_town_next((RestoreTownList *)(uintptr_t)frame->r[1],(const int16_t *)(uintptr_t)MI_U32(sp+4),(uint8_t)MI_U32(sp+8));frame->r[4]=sp+12;return;
 /* These existing typed providers have RET0 and only EAX consumed here. */
 case 0x4c5120u:frame->r[0]=FUN_004c5120((int32_t)MI_U32(sp+4));frame->r[4]=sp+4;return;
 case 0x4c5240u:frame->r[0]=(uint32_t)FUN_004c5240((int32_t)MI_U32(sp+4));frame->r[4]=sp+4;return;
 case 0x4c5190u:frame->r[0]=FUN_004c5190();frame->r[4]=sp+4;return;
 case 0x4cde60u:frame->r[0]=(uint32_t)FUN_004cde60((int32_t)MI_U32(sp+4),(int32_t)MI_U32(sp+8));frame->r[4]=sp+4;return;
 /* AL-only native contract. Each caller overwrites EAX before consuming its high bits. */
 case 0x4c52d0u:frame->r[0]=(frame->r[0]&0xffffff00u)|FUN_004c52d0((int32_t)MI_U32(sp+4),MI_U32(sp+8));frame->r[4]=sp+4;return;
 /* Native RET4; receiver ECX is ignored by the genuine WORD wrapper. */
 case 0x488030u:frame->r[0]=FUN_00488030(MI_U32(sp+4));frame->r[4]=sp+8;return;
 default:restore_actor_message_failure(target);
 }
}
static uint32_t actor_message_run(uint32_t target,uint32_t argument,uint32_t has_argument)
{
 uint32_t stack[1024],i,entry;struct restore_actor_message_frame frame={{0},2,restore_actor_message_dispatch};
 typedef char pointer_is_32[sizeof(void*)==4?1:-1];(void)sizeof(pointer_is_32);
 for(i=0;i<1024;++i)stack[i]=0xa5a55a5au;
 entry=(uint32_t)(uintptr_t)&stack[1000];frame.r[4]=entry;stack[1000]=0;if(has_argument)stack[1001]=argument;
 switch(target){
 case 0x53c100u:restore_actor_message_C_0053c100(&frame);break;
 case 0x495b60u:restore_actor_message_C_00495b60(&frame);break;
 case 0x53c060u:restore_actor_message_C_0053c060(&frame);break;
 default:restore_actor_message_failure(target);
 }
 if(frame.r[4]!=entry+4u)restore_actor_message_failure(frame.r[4]);
 for(i=0;i<16;++i)if(stack[i]!=0xa5a55a5au)restore_actor_message_failure(target);
 for(i=1001+has_argument;i<1024;++i)if(stack[i]!=0xa5a55a5au)restore_actor_message_failure(target);
 return frame.r[0];
}
/* Native caller4f7e50 discards EAX from53c100; the nonpositive-actor return
 * also retains incoming EAX. Its canonical C interface therefore stays void. */
void FUN_0053c100(int32_t actor){(void)actor_message_run(0x53c100u,(uint32_t)actor,1);}
uint32_t FUN_00495b60(uint32_t actor){return actor_message_run(0x495b60u,actor,1);}
uint32_t FUN_0053c060(void){return actor_message_run(0x53c060u,0,0);}
