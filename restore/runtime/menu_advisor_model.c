#include "city_supply.h"
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_advisor_actions.h"
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
static __inline uint32_t mi_alu(struct restore_advisor_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_advisor_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_advisor_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static __inline void mi_idiv32(struct restore_advisor_frame *f,uint32_t divisor_bits)
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
static const uint8_t mi_rom_0045d994[]={0xfd,0xd6,0x45,0x00,0x08,0xd7,0x45,0x00,0x0f,0xd7,0x45,0x00,0x16,0xd7,0x45,0x00,0x1d,0xd7,0x45,0x00,0x01,0xd7,0x45,0x00,0x2b,0xd7,0x45,0x00,0x24,0xd7,0x45,0x00,0x4e,0xd7,0x45,0x00,0x4e,0xd7,0x45,0x00,0x4e,0xd7,0x45,0x00,0x4e,0xd7,0x45,0x00,0x47,0xd7,0x45,0x00,0x4e,0xd7,0x45,0x00,0x40,0xd7,0x45,0x00,0x4e,0xd7,0x45,0x00,0x32,0xd7,0x45,0x00,0x4e,0xd7,0x45,0x00,0x39,0xd7,0x45,0x00};
static const uint8_t mi_rom_004c07e8[]={0x0b,0x07,0x4c,0x00,0x11,0x07,0x4c,0x00,0x17,0x07,0x4c,0x00,0x1d,0x07,0x4c,0x00,0x23,0x07,0x4c,0x00,0x29,0x07,0x4c,0x00,0x2f,0x07,0x4c,0x00,0x35,0x07,0x4c,0x00,0x3b,0x07,0x4c,0x00,0x41,0x07,0x4c,0x00};
static const uint8_t mi_rom_004c0810[]={0x5b,0x07,0x4c,0x00,0x61,0x07,0x4c,0x00,0x67,0x07,0x4c,0x00,0x6d,0x07,0x4c,0x00,0x73,0x07,0x4c,0x00,0x79,0x07,0x4c,0x00,0x7f,0x07,0x4c,0x00,0x85,0x07,0x4c,0x00,0x8b,0x07,0x4c,0x00,0x91,0x07,0x4c,0x00};
static const uint8_t mi_rom_004c0838[]={0xa7,0x07,0x4c,0x00,0xad,0x07,0x4c,0x00,0xb3,0x07,0x4c,0x00,0xb9,0x07,0x4c,0x00,0xbf,0x07,0x4c,0x00,0xc5,0x07,0x4c,0x00,0xcb,0x07,0x4c,0x00,0xd1,0x07,0x4c,0x00,0xd7,0x07,0x4c,0x00,0xdd,0x07,0x4c,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0045d994u&&width<=76u&&(offset=address-0x0045d994u)<=76u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045d994[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c07e8u&&width<=40u&&(offset=address-0x004c07e8u)<=40u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004c07e8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c0810u&&width<=40u&&(offset=address-0x004c0810u)<=40u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004c0810[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c0838u&&width<=40u&&(offset=address-0x004c0838u)<=40u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004c0838[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_advisor_C_0040b520(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040b520 */
    f->r[0]=MI_U32((0x00e38ebcu));
    /* 0040b525 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040b527 */
    if(!(f->flags&MI_ZF))goto mi_0040b549;
    /* 0040b529 */
    a=0x00e38ea4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b52e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b533u;f->boundary(0x0040b52eu,0x004179a0u,f);
    /* 0040b533 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040b536 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040b538 */
    if(!(f->flags&MI_ZF))goto mi_0040b549;
    /* 0040b53a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b53c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b53e */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b540 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b545u;f->boundary(0x0040b540u,0x004e2970u,f);
    /* 0040b545 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040b548 */
    f->r[4]+=4;return;
mi_0040b549:;
    /* 0040b549 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040b54e */
    f->r[1]=MI_U32((0x00e38ea4u));
    /* 0040b554 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040b55a */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000007u);
    /* 0040b561 */
    MI_U32((0x00605a44u))=(uint32_t)(f->r[1]);
    /* 0040b567 */
    MI_U8((0x00ea623cu+f->r[0]))=(uint8_t)(0x00000000u);
    /* 0040b56e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b573u;f->boundary(0x0040b56eu,0x0040a160u,f);
    /* 0040b573 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040b57a */
    MI_U32((0x00f53f48u))=(uint32_t)(0x00000006u);
    /* 0040b584 */
    f->r[4]+=4;return;
}

void restore_advisor_C_004179a0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004179a0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004179a4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004179a5 */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 004179a7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x006fdc88u+f->r[0])))&0xffu)<<0);
    /* 004179ad */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004179af */
    if((f->flags&MI_ZF))goto mi_004179b5;
mi_004179b1:;
    /* 004179b1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004179b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004179b4 */
    f->r[4]+=4;return;
mi_004179b5:;
    /* 004179b5 */
    f->r[2]=f->r[0];
    /* 004179b7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004179b8 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004179bb */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004179bd */
    if(mi_signed_less(f->flags))goto mi_004179c5;
    /* 004179bf */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(0x00000001u);
mi_004179c5:;
    /* 004179c5 */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 004179c7 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004179c9 */
    if((f->flags&MI_ZF))goto mi_004179e9;
mi_004179cb:;
    /* 004179cb */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x006fdc88u+f->r[0])))&0xffu)<<0);
    /* 004179d1 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004179d3 */
    if(!(f->flags&MI_ZF))goto mi_004179b1;
    /* 004179d5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004179d6 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004179d9 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004179db */
    if(mi_signed_less(f->flags))goto mi_004179e3;
    /* 004179dd */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(0x00000001u);
mi_004179e3:;
    /* 004179e3 */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 004179e5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004179e7 */
    if(!(f->flags&MI_ZF))goto mi_004179cb;
mi_004179e9:;
    /* 004179e9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004179eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004179ec */
    f->r[4]+=4;return;
}

void restore_advisor_C_0040a160(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040a160 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a165u;f->boundary(0x0040a160u,0x00459400u,f);
    /* 0040a165 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a16au;f->boundary(0x0040a165u,0x004595a0u,f);
    /* 0040a16a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a16fu;f->boundary(0x0040a16au,0x0051f780u,f);
    /* 0040a16f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a174u;f->boundary(0x0040a16fu,0x0051f780u,f);
    /* 0040a174 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a179u;f->boundary(0x0040a174u,0x00459670u,f);
    /* 0040a179 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a17eu;f->boundary(0x0040a179u,0x00459d20u,f);
    /* 0040a17e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a183u;f->boundary(0x0040a17eu,0x0045a8c0u,f);
    /* 0040a183 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a188u;f->boundary(0x0040a183u,0x0045a930u,f);
    /* 0040a188 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a18du;f->boundary(0x0040a188u,0x0045a970u,f);
    /* 0040a18d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a192u;f->boundary(0x0040a18du,0x0045a990u,f);
    /* 0040a192 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a197u;f->boundary(0x0040a192u,0x0045a7b0u,f);
    /* 0040a197 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a19cu;f->boundary(0x0040a197u,0x004e9500u,f);
    /* 0040a19c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a1a1u;f->boundary(0x0040a19cu,0x00468170u,f);
    /* 0040a1a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a1a6u;f->boundary(0x0040a1a1u,0x0045d5d0u,f);
    /* 0040a1a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a1abu;f->boundary(0x0040a1a6u,0x004b7c80u,f);
    /* 0040a1ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a1b0u;f->boundary(0x0040a1abu,0x004b8aa0u,f);
    /* 0040a1b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a1b5u;f->boundary(0x0040a1b0u,0x0046e1d0u,f);
    /* 0040a1b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a1bau;f->boundary(0x0040a1b5u,0x0046e410u,f);
    /* 0040a1ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a1bfu;f->boundary(0x0040a1bau,0x0046ec70u,f);
    /* 0040a1bf */
    f->boundary(0x0040a1bfu,0x004f6ad0u,f);return;
}

void restore_advisor_C_0045a8c0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045a8c0 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0045a8c6 */
    f->r[0]=0x66666667u;
    /* 0045a8cb */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0045a8d1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a8d2 */
    f->r[6]=MI_U32((0x0078d7c0u));
    /* 0045a8d8 */
    f->r[2]=MI_U32((0x00ea4478u+f->r[1]));
    /* 0045a8de */
    a=f->r[2];b=MI_U32((0x00ea4464u+f->r[1]));t=mi_imul(f,a,b);f->r[2]=t;
    /* 0045a8e5 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0045a8e7 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0045a8ea */
    f->r[0]=f->r[2];
    /* 0045a8ec */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0045a8ef */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045a8f1 */
    f->r[0]=0x2aaaaaabu;
    /* 0045a8f6 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0045a8f8 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0045a8fa */
    f->r[0]=f->r[2];
    /* 0045a8fc */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0045a8ff */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045a901 */
    f->r[0]=MI_U32((0x00ea4484u+f->r[1]));
    /* 0045a907 */
    MI_U32((0x00ea4488u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a90d */
    f->r[0]=0x0000000cu;
    /* 0045a912 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045a914 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045a915 */
    a=f->r[0];b=f->r[2];t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045a918 */
    a=f->r[0];b=MI_U32((0x00ea4484u+f->r[1]));t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045a91e */
    MI_U32((0x00ea5f10u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0045a924 */
    f->r[4]+=4;return;
}

void restore_advisor_C_0045a930(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045a930 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0045a935 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045a93b */
    a=MI_U16((0x00ea5ce8u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045a943 */
    if((f->flags&MI_ZF))goto mi_0045a954;
    /* 0045a945 */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[0]));
    /* 0045a94b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045a94c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045a951u;f->boundary(0x0045a94cu,0x004c5120u,f);
    /* 0045a951 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0045a954:;
    /* 0045a954 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0045a959 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045a95f */
    f->r[1]=MI_U32((0x00ea44e4u+f->r[0]));
    /* 0045a965 */
    MI_U32((0x00ea44ecu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0045a96b */
    f->r[4]+=4;return;
}

void restore_advisor_C_0045d5d0(struct restore_advisor_frame *f)
{
 uint32_t sp=f->r[4];restore_city_update_food();f->r[4]=sp+4u;
}

void restore_advisor_C_0045d630(struct restore_advisor_frame *f)
{
 uint32_t sp=f->r[4];restore_city_rebuild_food((int32_t)MI_U32(sp+4u));f->r[4]=sp+4u;
}

void restore_advisor_C_00468170(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00468170 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00468173 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00468178 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468179 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046817f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468180 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468181 */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 00468187 */
    f->r[0]=MI_U32((0x007057b8u));
    /* 0046818c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046818d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0046818f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468192 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00468194 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468195 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00468199 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046819c */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004681a0 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004681a3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004681a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004681a9u;f->boundary(0x004681a4u,0x004cde60u,f);
    /* 004681a9 */
    MI_U32((0x0078d5f0u))=(uint32_t)(f->r[0]);
    /* 004681ae */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004681b3 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004681b9 */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 004681bf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004681c0 */
    f->r[1]=MI_U32((0x007057b4u));
    /* 004681c6 */
    f->r[0]=(0x00000000u+f->r[1]*8u);
    /* 004681cd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004681cf */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004681d2 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004681d5 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004681d8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004681d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004681deu;f->boundary(0x004681d9u,0x004cde60u,f);
    /* 004681de */
    MI_U32((0x0078d5f4u))=(uint32_t)(f->r[0]);
    /* 004681e3 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004681e8 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004681ee */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 004681f4 */
    f->r[0]=MI_U32((0x007057c0u));
    /* 004681f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004681fa */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004681fd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468200 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468203 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00468206 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468207 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046820cu;f->boundary(0x00468207u,0x004cde60u,f);
    /* 0046820c */
    MI_U32((0x0078d5f8u))=(uint32_t)(f->r[0]);
    /* 00468211 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00468216 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046821c */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 00468222 */
    f->r[0]=MI_U32((0x007057bcu));
    /* 00468227 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468228 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046822b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046822e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468231 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468234 */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00468237 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468238 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046823du;f->boundary(0x00468238u,0x004cde60u,f);
    /* 0046823d */
    MI_U32((0x0078d5fcu))=(uint32_t)(f->r[0]);
    /* 00468242 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00468247 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046824d */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 00468253 */
    f->r[0]=MI_U32((0x00705ac4u));
    /* 00468258 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468259 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046825c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046825f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468262 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468265 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468268 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046826b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046826c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468271u;f->boundary(0x0046826cu,0x004cde60u,f);
    /* 00468271 */
    f->r[1]=MI_U32((0x0078d5f0u));
    /* 00468277 */
    f->r[5]=0x00000064u;
    /* 0046827c */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046827f */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00468281 */
    MI_U32((0x0078d600u))=(uint32_t)(f->r[0]);
    /* 00468286 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046828e;
    /* 00468288 */
    MI_U32((0x0078d5f0u))=(uint32_t)(f->r[5]);
mi_0046828e:;
    /* 0046828e */
    a=MI_U32((0x0078d5f4u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00468294 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046829c;
    /* 00468296 */
    MI_U32((0x0078d5f4u))=(uint32_t)(f->r[5]);
mi_0046829c:;
    /* 0046829c */
    a=MI_U32((0x0078d5f8u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004682a2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004682aa;
    /* 004682a4 */
    MI_U32((0x0078d5f8u))=(uint32_t)(f->r[5]);
mi_004682aa:;
    /* 004682aa */
    a=MI_U32((0x0078d5fcu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004682b0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004682b8;
    /* 004682b2 */
    MI_U32((0x0078d5fcu))=(uint32_t)(f->r[5]);
mi_004682b8:;
    /* 004682b8 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004682ba */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004682c2;
    /* 004682bc */
    MI_U32((0x0078d600u))=(uint32_t)(f->r[5]);
mi_004682c2:;
    /* 004682c2 */
    f->r[0]=0x007848e2u;
    /* 004682c7 */
    f->r[7]=0x0078d620u;
    /* 004682cc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004682d0:;
    /* 004682d0 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[0])))&0xffffu)<<0);
    /* 004682d3 */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 004682d7 */
    if(!(f->flags&MI_ZF))goto mi_00468355;
    /* 004682d9 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004682de */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004682e4 */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 004682ea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004682eb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004682ed */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004682ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004682f3u;f->boundary(0x004682eeu,0x004c06e0u,f);
    /* 004682f3 */
    f->r[0]=MI_U32((0x0070573cu+f->r[0]*4u));
    /* 004682fa */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004682fc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004682fd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468300 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468303 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468306 */
    a=f->r[5];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00468309 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046830eu;f->boundary(0x00468309u,0x004c06e0u,f);
    /* 0046830e */
    f->r[0]=MI_U32((0x0070573cu+f->r[0]*4u));
    /* 00468315 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468317 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468318 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046831b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046831e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468321 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468324 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00468326 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046832bu;f->boundary(0x00468326u,0x004c06e0u,f);
    /* 0046832b */
    f->r[0]=MI_U32((0x0070573cu+f->r[0]*4u));
    /* 00468332 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00468335 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00468338 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046833b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046833e */
    f->r[1]=(0x00000000u+f->r[5]+f->r[0]*2u);
    /* 00468342 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00468343:;
    /* 00468343 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468348u;f->boundary(0x00468343u,0x004cde60u,f);
    /* 00468348 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046834b */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 0046834d */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046834e */
    f->r[5]=0x00000064u;
    /* 00468353 */
    goto mi_004683d3;
mi_00468355:;
    /* 00468355 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 00468359 */
    if(!(f->flags&MI_ZF))goto mi_004683cd;
    /* 0046835b */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00468361 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00468367 */
    f->r[0]=MI_U32((0x00ea1894u+f->r[2]));
    /* 0046836d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046836e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468370 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468371 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468376u;f->boundary(0x00468371u,0x004c06e0u,f);
    /* 00468376 */
    f->r[0]=MI_U32((0x0070573cu+f->r[0]*4u));
    /* 0046837d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046837f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468380 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468383 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468386 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468389 */
    a=f->r[5];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0046838c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468391u;f->boundary(0x0046838cu,0x004c06e0u,f);
    /* 00468391 */
    f->r[0]=MI_U32((0x0070573cu+f->r[0]*4u));
    /* 00468398 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046839a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046839b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046839e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004683a1 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004683a4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004683a7 */
    f->r[5]=(0x00000000u+f->r[5]+f->r[1]*2u);
    /* 004683ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004683b0u;f->boundary(0x004683abu,0x004c06e0u,f);
    /* 004683b0 */
    f->r[0]=MI_U32((0x0070573cu+f->r[0]*4u));
    /* 004683b7 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004683ba */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004683bd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004683c0 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004683c3 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[2]*4u);
    /* 004683c7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004683c8 */
    goto mi_00468343;
mi_004683cd:;
    /* 004683cd */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000000u);
mi_004683d3:;
    /* 004683d3 */
    a=MI_U32((0x00000000u+f->r[7]));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004683d5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004683d9;
    /* 004683d7 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[5]);
mi_004683d9:;
    /* 004683d9 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004683db */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004683df */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004683e1 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004683e2 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004683e6 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004683ea */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004683ed */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004683f0 */
    a=f->r[0];b=0x007848ecu;t=mi_alu(f,a,b,32,1);
    /* 004683f5 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004683f9 */
    if(mi_signed_less(f->flags))goto mi_004682d0;
    /* 004683ff */
    p=(0x00000010u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 00468403 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00468407 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fidiv dword ptr [eax] }
    /* 0046840b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468410u;f->boundary(0x0046840bu,0x0056133cu,f);
    /* 00468410 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00468416 */
    MI_U32((0x0078d60cu))=(uint32_t)(f->r[0]);
    /* 0046841b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046841c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468421u;f->boundary(0x0046841cu,0x004e94a0u,f);
    /* 00468421 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00468427 */
    f->r[2]=f->r[1];
    /* 00468429 */
    f->r[6]=f->r[1];
    /* 0046842b */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00468431 */
    a=f->r[6];b=0x000049d8u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 00468437 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468438 */
    MI_U32((0x00ea189cu+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0046843e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468443u;f->boundary(0x0046843eu,0x004e94d0u,f);
    /* 00468443 */
    MI_U32((0x00ea18a0u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00468449 */
    f->r[0]=MI_U32((0x00ea189cu+f->r[6]));
    /* 0046844f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468450 */
    f->r[0]=MI_U32((0x00705808u));
    /* 00468455 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00468458 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046845b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046845e */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00468461 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00468462 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468467u;f->boundary(0x00468462u,0x004cde60u,f);
    /* 00468467 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0046846d */
    MI_U32((0x0078d604u))=(uint32_t)(f->r[0]);
    /* 00468472 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00468478 */
    f->r[0]=MI_U32((0x00ea1894u+f->r[2]));
    /* 0046847e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046847f */
    f->r[0]=MI_U32((0x00705810u));
    /* 00468484 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00468487 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0046848a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046848d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046848e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00468493u;f->boundary(0x0046848eu,0x004cde60u,f);
    /* 00468493 */
    f->r[1]=MI_U32((0x0078d604u));
    /* 00468499 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046849c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046849e */
    MI_U32((0x0078d608u))=(uint32_t)(f->r[0]);
    /* 004684a3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004684ab;
    /* 004684a5 */
    MI_U32((0x0078d604u))=(uint32_t)(f->r[5]);
mi_004684ab:;
    /* 004684ab */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004684ad */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004684b5;
    /* 004684af */
    MI_U32((0x0078d608u))=(uint32_t)(f->r[5]);
mi_004684b5:;
    /* 004684b5 */
    a=0x0078d610u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004684ba */
    a=0x0078d618u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004684bf */
    a=0x0078d61cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004684c4 */
    a=0x0078d614u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004684c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004684ceu;f->boundary(0x004684c9u,0x004e7e60u,f);
    /* 004684ce */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004684d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004684d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004684d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004684d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004684d5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004684d8 */
    f->r[4]+=4;return;
}

void restore_advisor_C_0046e1d0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046e1d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e1d5u;f->boundary(0x0046e1d0u,0x0046e1c0u,f);
    /* 0046e1d5 */
    f->boundary(0x0046e1d5u,0x00426b00u,f);return;
}

void restore_advisor_C_0046e1c0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046e1c0 */
    f->boundary(0x0046e1c0u,0x004269b0u,f);return;
}

void restore_advisor_C_004269b0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004269b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004269b5u;f->boundary(0x004269b0u,0x00426a00u,f);
    /* 004269b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004269bau;f->boundary(0x004269b5u,0x00426a80u,f);
    /* 004269ba */
    f->r[0]=MI_U32((0x005c9348u));
    /* 004269bf */
    f->r[1]=MI_U32((0x00705734u));
    /* 004269c5 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004269c7 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004269ca */
    if(!mi_signed_less(f->flags))goto mi_004269d9;
    /* 004269cc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004269ce */
    if(!mi_signed_less(f->flags))goto mi_004269d4;
    /* 004269d0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004269d2 */
    goto mi_004269de;
mi_004269d4:;
    /* 004269d4 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004269d7 */
    if(mi_signed_less(f->flags))goto mi_004269de;
mi_004269d9:;
    /* 004269d9 */
    f->r[0]=0x00000064u;
mi_004269de:;
    /* 004269de */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004269e4 */
    f->r[2]=MI_U32((0x00705738u));
    /* 004269ea */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004269f0 */
    MI_U32((0x00ea5b3cu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004269f6 */
    MI_U32((0x00ea5ca4u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004269fc */
    f->r[4]+=4;return;
}

void restore_advisor_C_00426a00(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00426a00 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00426a01 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00426a02 */
    f->r[6]=0x00000064u;
    /* 00426a07 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00426a09 */
    f->r[0]=0x005c9370u;
mi_00426a0e:;
    /* 00426a0e */
    f->r[1]=MI_U32((0xffffffe0u+f->r[0]));
    /* 00426a11 */
    f->r[7]=MI_U32((0x00000000u+f->r[0]));
    /* 00426a13 */
    f->r[1]=MI_U32((0x0078d5f0u+f->r[1]*4u));
    /* 00426a1a */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00426a1c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00426a23;
    /* 00426a1e */
    f->r[1]=MI_U32((0xffffffe4u+f->r[0]));
    /* 00426a21 */
    goto mi_00426a58;
mi_00426a23:;
    /* 00426a23 */
    a=f->r[1];b=MI_U32((0x00000004u+f->r[0]));t=mi_alu(f,a,b,32,1);
    /* 00426a26 */
    if(!mi_signed_less(f->flags))goto mi_00426a2d;
    /* 00426a28 */
    f->r[1]=MI_U32((0xffffffe8u+f->r[0]));
    /* 00426a2b */
    goto mi_00426a58;
mi_00426a2d:;
    /* 00426a2d */
    a=f->r[1];b=MI_U32((0x00000008u+f->r[0]));t=mi_alu(f,a,b,32,1);
    /* 00426a30 */
    if(!mi_signed_less(f->flags))goto mi_00426a37;
    /* 00426a32 */
    f->r[1]=MI_U32((0xffffffecu+f->r[0]));
    /* 00426a35 */
    goto mi_00426a58;
mi_00426a37:;
    /* 00426a37 */
    a=f->r[1];b=MI_U32((0x0000000cu+f->r[0]));t=mi_alu(f,a,b,32,1);
    /* 00426a3a */
    if(!mi_signed_less(f->flags))goto mi_00426a41;
    /* 00426a3c */
    f->r[1]=MI_U32((0xfffffff0u+f->r[0]));
    /* 00426a3f */
    goto mi_00426a58;
mi_00426a41:;
    /* 00426a41 */
    a=f->r[1];b=MI_U32((0x00000010u+f->r[0]));t=mi_alu(f,a,b,32,1);
    /* 00426a44 */
    if(!mi_signed_less(f->flags))goto mi_00426a4b;
    /* 00426a46 */
    f->r[1]=MI_U32((0xfffffff4u+f->r[0]));
    /* 00426a49 */
    goto mi_00426a58;
mi_00426a4b:;
    /* 00426a4b */
    a=f->r[1];b=MI_U32((0x00000014u+f->r[0]));t=mi_alu(f,a,b,32,1);
    /* 00426a4e */
    if(!mi_signed_less(f->flags))goto mi_00426a55;
    /* 00426a50 */
    f->r[1]=MI_U32((0xfffffff8u+f->r[0]));
    /* 00426a53 */
    goto mi_00426a58;
mi_00426a55:;
    /* 00426a55 */
    f->r[1]=MI_U32((0xfffffffcu+f->r[0]));
mi_00426a58:;
    /* 00426a58 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00426a5a */
    if(!mi_signed_less(f->flags))goto mi_00426a64;
    /* 00426a5c */
    MI_U32((0x00705738u))=(uint32_t)(f->r[2]);
    /* 00426a62 */
    f->r[6]=f->r[1];
mi_00426a64:;
    /* 00426a64 */
    a=f->r[0];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00426a67 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00426a68 */
    a=f->r[0];b=0x005c95d8u;t=mi_alu(f,a,b,32,1);
    /* 00426a6d */
    if(mi_signed_less(f->flags))goto mi_00426a0e;
    /* 00426a6f */
    MI_U32((0x005c9348u))=(uint32_t)(f->r[6]);
    /* 00426a75 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00426a76 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00426a77 */
    f->r[4]+=4;return;
}

void restore_advisor_C_00426a80(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00426a80 */
    f->r[2]=MI_U32((0x005c9348u));
    /* 00426a86 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00426a87 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00426a89 */
    f->r[1]=0x005c9354u;
mi_00426a8e:;
    /* 00426a8e */
    f->r[0]=MI_U32((0xfffffffcu+f->r[1]));
    /* 00426a91 */
    f->r[0]=MI_U32((0x0078d5f0u+f->r[0]*4u));
    /* 00426a98 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00426a9a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00426aa0;
    /* 00426a9c */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 00426a9e */
    goto mi_00426ad5;
mi_00426aa0:;
    /* 00426aa0 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 00426aa3 */
    if(!mi_signed_less(f->flags))goto mi_00426aaa;
    /* 00426aa5 */
    f->r[0]=MI_U32((0x00000004u+f->r[1]));
    /* 00426aa8 */
    goto mi_00426ad5;
mi_00426aaa:;
    /* 00426aaa */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 00426aad */
    if(!mi_signed_less(f->flags))goto mi_00426ab4;
    /* 00426aaf */
    f->r[0]=MI_U32((0x00000008u+f->r[1]));
    /* 00426ab2 */
    goto mi_00426ad5;
mi_00426ab4:;
    /* 00426ab4 */
    a=f->r[0];b=0x0000003cu;t=mi_alu(f,a,b,32,1);
    /* 00426ab7 */
    if(!mi_signed_less(f->flags))goto mi_00426abe;
    /* 00426ab9 */
    f->r[0]=MI_U32((0x0000000cu+f->r[1]));
    /* 00426abc */
    goto mi_00426ad5;
mi_00426abe:;
    /* 00426abe */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,1);
    /* 00426ac1 */
    if(!mi_signed_less(f->flags))goto mi_00426ac8;
    /* 00426ac3 */
    f->r[0]=MI_U32((0x00000010u+f->r[1]));
    /* 00426ac6 */
    goto mi_00426ad5;
mi_00426ac8:;
    /* 00426ac8 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00426acb */
    if(!mi_signed_less(f->flags))goto mi_00426ad2;
    /* 00426acd */
    f->r[0]=MI_U32((0x00000014u+f->r[1]));
    /* 00426ad0 */
    goto mi_00426ad5;
mi_00426ad2:;
    /* 00426ad2 */
    f->r[0]=MI_U32((0x00000018u+f->r[1]));
mi_00426ad5:;
    /* 00426ad5 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00426ad8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00426ada */
    if(!mi_signed_less(f->flags))goto mi_00426adf;
    /* 00426adc */
    a=f->r[6];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_00426adf:;
    /* 00426adf */
    a=f->r[1];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00426ae2 */
    a=f->r[1];b=0x005c95bcu;t=mi_alu(f,a,b,32,1);
    /* 00426ae8 */
    if(mi_signed_less(f->flags))goto mi_00426a8e;
    /* 00426aea */
    MI_U32((0x00705734u))=(uint32_t)(f->r[6]);
    /* 00426af0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00426af1 */
    f->r[4]+=4;return;
}

void restore_advisor_C_00426b00(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00426b00 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00426b05 */
    f->r[1]=MI_U32((0x00705738u));
    /* 00426b0b */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00426b11 */
    MI_U32((0x00ea5ca4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00426b17 */
    f->r[4]+=4;return;
}

void restore_advisor_C_0046e410(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046e410 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0046e413 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e414 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e415 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e416 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e417 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e41cu;f->boundary(0x0046e417u,0x004c38f0u,f);
    /* 0046e41c */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0046e422 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046e426 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0046e42c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046e42e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 0046e436 */
    f->r[7]=MI_U32((0x00ea601cu+f->r[1]));
    /* 0046e43c */
    f->r[2]=MI_U32((0x00ea5cc4u+f->r[1]));
    /* 0046e442 */
    f->r[5]=MI_U32((0x00ea44c0u+f->r[1]));
    /* 0046e448 */
    f->r[6]=MI_U32((0x00ea44f8u+f->r[1]));
    /* 0046e44e */
    f->r[0]=MI_U32((0x00ea44d8u+f->r[1]));
    /* 0046e454 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046e456 */
    f->r[2]=MI_U32((0x00ea4500u+f->r[1]));
    /* 0046e45c */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046e45e */
    f->r[5]=MI_U32((0x00ea44e8u+f->r[1]));
    /* 0046e464 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046e466 */
    f->r[2]=MI_U32((0x00ea448cu+f->r[1]));
    /* 0046e46c */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046e46e */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046e470 */
    f->r[0]=MI_U32((0x00ea44e0u+f->r[1]));
    /* 0046e476 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046e478 */
    f->r[5]=f->r[7];
    /* 0046e47a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046e47c */
    f->r[0]=MI_U32((0x00ea44fcu+f->r[1]));
    /* 0046e482 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0046e484 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046e488 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0046e48a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046e48e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e497;
    /* 0046e490 */
    f->r[7]=0x00000005u;
    /* 0046e495 */
    goto mi_0046e4ec;
mi_0046e497:;
    /* 0046e497 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046e498 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e49a */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046e49c */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e49e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046e4a0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046e4a2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e4ae;
    /* 0046e4a4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 0046e4ac */
    goto mi_0046e4e8;
mi_0046e4ae:;
    /* 0046e4ae */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0046e4b0 */
    if(!mi_signed_less(f->flags))goto mi_0046e4c9;
    /* 0046e4b2 */
    f->r[3]=0x00000008u;
    /* 0046e4b7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 0046e4bf */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0xfffffff8u);
    /* 0046e4c7 */
    goto mi_0046e4e8;
mi_0046e4c9:;
    /* 0046e4c9 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0046e4cd */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e4cf */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046e4d1 */
    if(!(f->flags&MI_SF))goto mi_0046e4e8;
    /* 0046e4d3 */
    f->r[3]=0x00000005u;
    /* 0046e4d8 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 0046e4e0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0xfffffffbu);
mi_0046e4e8:;
    /* 0046e4e8 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
mi_0046e4ec:;
    /* 0046e4ec */
    f->r[0]=MI_U32((0x00ea41e4u+f->r[1]));
    /* 0046e4f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046e4f4 */
    if(mi_signed_less(f->flags))goto mi_0046e4fc;
    /* 0046e4f6 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046e4f9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046e4fc;
    /* 0046e4fb */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046e4fc:;
    /* 0046e4fc */
    f->r[0]=MI_U32((0x00ea44d8u+f->r[1]));
    /* 0046e502 */
    f->r[2]=MI_U32((0x00ea44e0u+f->r[1]));
    /* 0046e508 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0046e50a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e50f;
    /* 0046e50c */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e50d */
    goto mi_0046e537;
mi_0046e50f:;
    /* 0046e50f */
    if(!mi_signed_less(f->flags))goto mi_0046e526;
    /* 0046e511 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046e514 */
    if(!mi_signed_less(f->flags))goto mi_0046e523;
    /* 0046e516 */
    f->r[3]=0x00000001u;
    /* 0046e51b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000000bu);
mi_0046e523:;
    /* 0046e523 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e524 */
    goto mi_0046e537;
mi_0046e526:;
    /* 0046e526 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0046e528 */
    if(!(f->flags&MI_ZF))goto mi_0046e537;
    /* 0046e52a */
    f->r[3]=0x00000001u;
    /* 0046e52f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000000bu);
mi_0046e537:;
    /* 0046e537 */
    f->r[0]=MI_U32((0x00ea4474u+f->r[1]));
    /* 0046e53d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046e53f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e55b;
    /* 0046e541 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046e544 */
    if(!mi_signed_less(f->flags))goto mi_0046e553;
    /* 0046e546 */
    f->r[3]=0x00000001u;
    /* 0046e54b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000000au);
mi_0046e553:;
    /* 0046e553 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e554 */
    f->r[6]=0x00000005u;
    /* 0046e559 */
    goto mi_0046e5b6;
mi_0046e55b:;
    /* 0046e55b */
    f->r[0]=MI_U32((0x00ea446cu+f->r[1]));
    /* 0046e561 */
    f->r[6]=0x00000005u;
    /* 0046e566 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046e568 */
    if(!mi_signed_less(f->flags))goto mi_0046e56f;
    /* 0046e56a */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046e56d */
    goto mi_0046e5b6;
mi_0046e56f:;
    /* 0046e56f */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0046e572 */
    if(!mi_signed_less(f->flags))goto mi_0046e57f;
    /* 0046e574 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0046e576 */
    if(!(f->flags&MI_ZF))goto mi_0046e5b6;
    /* 0046e578 */
    f->r[3]=0x00000001u;
    /* 0046e57d */
    goto mi_0046e5b2;
mi_0046e57f:;
    /* 0046e57f */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0046e582 */
    if(!mi_signed_less(f->flags))goto mi_0046e591;
    /* 0046e584 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e585 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046e588 */
    if(!mi_signed_less(f->flags))goto mi_0046e5b6;
    /* 0046e58a */
    f->r[3]=0x00000001u;
    /* 0046e58f */
    goto mi_0046e5b2;
mi_0046e591:;
    /* 0046e591 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0046e594 */
    if(!mi_signed_less(f->flags))goto mi_0046e5a5;
    /* 0046e596 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0046e599 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046e59c */
    if(!mi_signed_less(f->flags))goto mi_0046e5b6;
    /* 0046e59e */
    f->r[3]=0x00000002u;
    /* 0046e5a3 */
    goto mi_0046e5b2;
mi_0046e5a5:;
    /* 0046e5a5 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0046e5a8 */
    a=f->r[3];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0046e5ab */
    if(!mi_signed_less(f->flags))goto mi_0046e5b6;
    /* 0046e5ad */
    f->r[3]=0x00000003u;
mi_0046e5b2:;
    /* 0046e5b2 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
mi_0046e5b6:;
    /* 0046e5b6 */
    f->r[2]=MI_U32((0x00ea5e18u+f->r[1]));
    /* 0046e5bc */
    f->r[0]=0x2aaaaaabu;
    /* 0046e5c1 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046e5c3 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0046e5c5 */
    f->r[0]=f->r[2];
    /* 0046e5c7 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 0046e5cb */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0046e5ce */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046e5d0 */
    f->r[0]=f->r[2];
    /* 0046e5d2 */
    f->r[2]=MI_U32((0x00ea447cu+f->r[1]));
    /* 0046e5d8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e5da */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e5dc */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046e5de */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e5e7;
    /* 0046e5e0 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046e5e3 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0046e5e5 */
    goto mi_0046e637;
mi_0046e5e7:;
    /* 0046e5e7 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0046e5e9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e5eb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e5f0;
    /* 0046e5ed */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e5ee */
    goto mi_0046e637;
mi_0046e5f0:;
    /* 0046e5f0 */
    a=f->r[0];b=0xfffffff6u;t=mi_alu(f,a,b,32,1);
    /* 0046e5f3 */
    if(!mi_signed_less(f->flags))goto mi_0046e60c;
    /* 0046e5f5 */
    a=f->r[3];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0046e5f8 */
    if(!mi_signed_less(f->flags))goto mi_0046e607;
    /* 0046e5fa */
    f->r[3]=0x00000003u;
    /* 0046e5ff */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000006u);
mi_0046e607:;
    /* 0046e607 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0046e60a */
    goto mi_0046e637;
mi_0046e60c:;
    /* 0046e60c */
    a=f->r[0];b=0xfffffffbu;t=mi_alu(f,a,b,32,1);
    /* 0046e60f */
    if(!mi_signed_less(f->flags))goto mi_0046e620;
    /* 0046e611 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0046e614 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046e617 */
    if(!mi_signed_less(f->flags))goto mi_0046e637;
    /* 0046e619 */
    f->r[3]=0x00000002u;
    /* 0046e61e */
    goto mi_0046e62f;
mi_0046e620:;
    /* 0046e620 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e622 */
    if(!mi_signed_less(f->flags))goto mi_0046e637;
    /* 0046e624 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e625 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046e628 */
    if(!mi_signed_less(f->flags))goto mi_0046e637;
    /* 0046e62a */
    f->r[3]=0x00000001u;
mi_0046e62f:;
    /* 0046e62f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000006u);
mi_0046e637:;
    /* 0046e637 */
    f->r[2]=MI_U32((0x00ea1894u+f->r[1]));
    /* 0046e63d */
    f->r[0]=MI_U32((0x00ea3f78u+f->r[1]));
    /* 0046e643 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e644 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e645 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e64au;f->boundary(0x0046e645u,0x004cde60u,f);
    /* 0046e64a */
    f->r[6]=f->r[0];
    /* 0046e64c */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046e651 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046e657 */
    f->r[1]=MI_U32((0x00ea1894u+f->r[0]));
    /* 0046e65d */
    f->r[2]=MI_U32((0x00ea3f7cu+f->r[0]));
    /* 0046e663 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e664 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e665 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e66au;f->boundary(0x0046e665u,0x004cde60u,f);
    /* 0046e66a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e66d */
    a=f->r[6];b=0x0000005fu;t=mi_alu(f,a,b,32,1);
    /* 0046e670 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e685;
    /* 0046e672 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046e675 */
    if(!mi_signed_less(f->flags))goto mi_0046e684;
    /* 0046e677 */
    f->r[3]=0x00000001u;
    /* 0046e67c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000007u);
mi_0046e684:;
    /* 0046e684 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046e685:;
    /* 0046e685 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0046e688 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e68d;
    /* 0046e68a */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_0046e68d:;
    /* 0046e68d */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046e692 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046e698 */
    a=MI_U32((0x00ea5d58u+f->r[0]));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e69e */
    if((f->flags&MI_ZF))goto mi_0046e6b3;
    /* 0046e6a0 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e6a1 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046e6a4 */
    if(!mi_signed_less(f->flags))goto mi_0046e6b3;
    /* 0046e6a6 */
    f->r[3]=0x00000001u;
    /* 0046e6ab */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000008u);
mi_0046e6b3:;
    /* 0046e6b3 */
    a=MI_U32((0x00ea5be8u+f->r[0]));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e6b9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e6bc;
    /* 0046e6bb */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046e6bc:;
    /* 0046e6bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e6c1u;f->boundary(0x0046e6bcu,0x00451b10u,f);
    /* 0046e6c1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046e6c3 */
    if((f->flags&MI_ZF))goto mi_0046e6c6;
    /* 0046e6c5 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046e6c6:;
    /* 0046e6c6 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046e6cb */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046e6d1 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e6d3 */
    f->r[1]=MI_U32((0x00ea5b40u+f->r[0]));
    /* 0046e6d9 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 0046e6dc */
    if((f->flags&MI_ZF))goto mi_0046e79d;
    /* 0046e6e2 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e6e4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046e6f9;
    /* 0046e6e6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007848d8u));
    /* 0046e6ed */
    a=MI_U32((0x0078d7bcu));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046e6f3 */
    if((f->flags&MI_ZF))goto mi_0046e79d;
mi_0046e6f9:;
    /* 0046e6f9 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e6fb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e781;
    /* 0046e701 */
    MI_U32((0x00ea5b54u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 0046e707 */
    f->r[1]=MI_U32((0x00ea5f18u+f->r[0]));
    /* 0046e70d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046e70f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e75d;
    /* 0046e711 */
    f->r[6]=MI_U32((0x00ea5b40u+f->r[0]));
    /* 0046e717 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046e719 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e737;
    /* 0046e71b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046e71c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046e71d */
    MI_U32((0x00ea5b54u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0046e727 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046e728 */
    MI_U32((0x00ea5ca8u+f->r[0]))=(uint32_t)(0x00000003u);
    /* 0046e732 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046e733 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e736 */
    f->r[4]+=4;return;
mi_0046e737:;
    /* 0046e737 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0046e739 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046e73b */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0046e73e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046e73f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046e740 */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046e743 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046e744 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046e747 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046e748 */
    MI_U32((0x00ea5b54u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0046e74e */
    MI_U32((0x00ea5ca8u+f->r[0]))=(uint32_t)(0x00000003u);
    /* 0046e758 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046e759 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e75c */
    f->r[4]+=4;return;
mi_0046e75d:;
    /* 0046e75d */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0046e75f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046e773;
    /* 0046e761 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0046e765 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046e766 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046e767 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046e768 */
    MI_U32((0x00ea5ca8u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0046e76e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046e76f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e772 */
    f->r[4]+=4;return;
mi_0046e773:;
    /* 0046e773 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046e774 */
    MI_U32((0x00ea5ca8u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 0046e77a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046e77b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046e77c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046e77d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e780 */
    f->r[4]+=4;return;
mi_0046e781:;
    /* 0046e781 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0046e785 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046e786 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046e787 */
    MI_U32((0x00ea5b54u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0046e791 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046e792 */
    MI_U32((0x00ea5ca8u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0046e798 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046e799 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e79c */
    f->r[4]+=4;return;
mi_0046e79d:;
    /* 0046e79d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046e79e */
    MI_U32((0x00ea5ca8u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 0046e7a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046e7a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046e7a6 */
    MI_U32((0x00ea5b54u+f->r[0]))=(uint32_t)(0x00000002u);
    /* 0046e7b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046e7b1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e7b4 */
    f->r[4]+=4;return;
}

void restore_advisor_C_004c38f0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c38f0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004c38f5 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004c38fb */
    a=MI_U16((0x00ea5ce8u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004c3903 */
    if((f->flags&MI_ZF))goto mi_004c3940;
    /* 004c3905 */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[0]));
    /* 004c390b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c390c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3911u;f->boundary(0x004c390cu,0x004c5120u,f);
    /* 004c3911 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3914 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c3917 */
    if(!(f->flags&MI_ZF))goto mi_004c3940;
    /* 004c3919 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004c391f */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3921 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004c3927 */
    f->r[2]=MI_U32((0x00ea5f9cu+f->r[1]));
    /* 004c392d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c392e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3933u;f->boundary(0x004c392eu,0x004c52d0u,f);
    /* 004c3933 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3936 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c3938 */
    if((f->flags&MI_ZF))goto mi_004c3940;
    /* 004c393a */
    f->r[0]=0xfffffffeu;
    /* 004c393f */
    f->r[4]+=4;return;
mi_004c3940:;
    /* 004c3940 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3942 */
    f->r[4]+=4;return;
}

void restore_advisor_C_00451b10(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00451b10 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451b11 */
    f->r[6]=0x00799ba8u;
mi_00451b16:;
    /* 00451b16 */
    a=MI_U8((0xffffff38u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00451b1d */
    if(!(f->flags&MI_ZF))goto mi_00451ba4;
    /* 00451b23 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0xffffff48u+f->r[6])))&0xffffu)<<0);
    /* 00451b2a */
    a=(f->r[0]&0xffffu);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 00451b2e */
    if(!(f->flags&MI_ZF))goto mi_00451b88;
    /* 00451b30 */
    a=MI_U16((0x00000000u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 00451b34 */
    if(!mi_signed_less(f->flags))goto mi_00451ba4;
    /* 00451b36 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00451b38 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00451b3e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff8cu+f->r[6])))&0xffu)<<0);
    /* 00451b41 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451b43 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451b45 */
    f->r[1]=MI_U32((0x005f5d08u+f->r[0]*4u));
    /* 00451b4c */
    f->r[0]=MI_U32((0xffffff44u+f->r[6]));
    /* 00451b52 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00451b54 */
    f->r[0]=0x8fb823efu;
    /* 00451b59 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00451b5b */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 00451b5d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00451b5f */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00451b62 */
    f->r[0]=f->r[2];
    /* 00451b64 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00451b67 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00451b69 */
    f->r[0]=f->r[1];
    /* 00451b6b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451b6c */
    f->r[1]=0x000000e4u;
    /* 00451b71 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00451b72 */
    a=f->r[1];mi_idiv32(f,a);
    /* 00451b74 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451b75 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451b7au;f->boundary(0x00451b75u,0x004ed360u,f);
    /* 00451b7a */
    f->r[0]=MI_U32((0x00eaae90u));
    /* 00451b7f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00451b82 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00451b84 */
    if((f->flags&MI_ZF))goto mi_00451bba;
    /* 00451b86 */
    goto mi_00451ba4;
mi_00451b88:;
    /* 00451b88 */
    a=(f->r[0]&0xffffu);b=0x000000d2u;t=mi_alu(f,a,b,16,1);
    /* 00451b8c */
    if(!(f->flags&MI_ZF))goto mi_00451ba4;
    /* 00451b8e */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00451b94 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00451b9a */
    a=MI_U16((0x00ea5baau+f->r[2]));b=0x00000010u;t=mi_alu(f,a,b,16,1);
    /* 00451ba2 */
    if(!mi_signed_less(f->flags))goto mi_00451bba;
mi_00451ba4:;
    /* 00451ba4 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00451baa */
    a=f->r[6];b=0x0089b7a0u;t=mi_alu(f,a,b,32,1);
    /* 00451bb0 */
    if(mi_signed_less(f->flags))goto mi_00451b16;
    /* 00451bb6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00451bb8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00451bb9 */
    f->r[4]+=4;return;
mi_00451bba:;
    /* 00451bba */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00451bbc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00451bbd */
    f->r[4]+=4;return;
}

void restore_advisor_C_004e9500(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e9500 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e9505 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e9506 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e950c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e950d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e950e */
    f->r[1]=MI_U32((0x00ea5db0u+f->r[0]));
    /* 004e9514 */
    f->r[3]=0x00000001u;
    /* 004e9519 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004e951b */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004e951d */
    f->r[5]=0x00000002u;
    /* 004e9522 */
    if(!(f->flags&MI_ZF))goto mi_004e952c;
    /* 004e9524 */
    MI_U32((0x00ea5c80u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 004e952a */
    goto mi_004e956d;
mi_004e952c:;
    /* 004e952c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004e952e */
    if(!(f->flags&MI_ZF))goto mi_004e9538;
    /* 004e9530 */
    MI_U32((0x00ea5c80u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e9536 */
    goto mi_004e956d;
mi_004e9538:;
    /* 004e9538 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004e953b */
    if(!(f->flags&MI_ZF))goto mi_004e9545;
    /* 004e953d */
    MI_U32((0x00ea5c80u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e9543 */
    goto mi_004e956d;
mi_004e9545:;
    /* 004e9545 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004e9548 */
    if(!(f->flags&MI_ZF))goto mi_004e9552;
    /* 004e954a */
    MI_U32((0x00ea5c80u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004e9550 */
    goto mi_004e956d;
mi_004e9552:;
    /* 004e9552 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004e9555 */
    if(!(f->flags&MI_ZF))goto mi_004e9563;
    /* 004e9557 */
    MI_U32((0x00ea5c80u+f->r[0]))=(uint32_t)(0x00000004u);
    /* 004e9561 */
    goto mi_004e956d;
mi_004e9563:;
    /* 004e9563 */
    MI_U32((0x00ea5c80u+f->r[0]))=(uint32_t)(0x00000005u);
mi_004e956d:;
    /* 004e956d */
    MI_U32((0x00ea5c8cu+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004e9573 */
    f->r[2]=MI_U32((0x00ea5b98u+f->r[0]));
    /* 004e9579 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e957b */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e957d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e9587;
    /* 004e957f */
    a=MI_U32((0x00ea5fbcu+f->r[0]));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e9585 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e95ad;
mi_004e9587:;
    /* 004e9587 */
    a=f->r[2];b=MI_U32((0x00ea5fbcu+f->r[0]));t=mi_alu(f,a,b,32,1);
    /* 004e958d */
    if(!mi_signed_less(f->flags))goto mi_004e95a1;
    /* 004e958f */
    MI_U32((0x00ea5c8cu+f->r[0]))=(uint32_t)(0x00000004u);
    /* 004e9599 */
    f->r[1]=MI_U32((0x00ea5fbcu+f->r[0]));
    /* 004e959f */
    goto mi_004e95ad;
mi_004e95a1:;
    /* 004e95a1 */
    MI_U32((0x00ea5c8cu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e95a7 */
    f->r[1]=MI_U32((0x00ea5b98u+f->r[0]));
mi_004e95ad:;
    /* 004e95ad */
    a=MI_U32((0x00ea5b94u+f->r[0]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e95b3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e95c1;
    /* 004e95b5 */
    MI_U32((0x00ea5c8cu+f->r[0]))=(uint32_t)(f->r[5]);
    /* 004e95bb */
    f->r[1]=MI_U32((0x00ea5b94u+f->r[0]));
mi_004e95c1:;
    /* 004e95c1 */
    a=MI_U32((0x00ea5bacu+f->r[0]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e95c7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e95d3;
    /* 004e95c9 */
    MI_U32((0x00ea5c8cu+f->r[0]))=(uint32_t)(0x00000003u);
mi_004e95d3:;
    /* 004e95d3 */
    MI_U32((0x00ea5c94u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004e95d9 */
    f->r[1]=MI_U32((0x00ea5b80u+f->r[0]));
    /* 004e95df */
    f->r[2]=MI_U32((0x00ea5b84u+f->r[0]));
    /* 004e95e5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004e95e7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e95f1;
    /* 004e95e9 */
    MI_U32((0x00ea5c94u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e95ef */
    goto mi_004e960d;
mi_004e95f1:;
    /* 004e95f1 */
    if(!mi_signed_less(f->flags))goto mi_004e95fb;
    /* 004e95f3 */
    MI_U32((0x00ea5c94u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 004e95f9 */
    goto mi_004e960d;
mi_004e95fb:;
    /* 004e95fb */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e95fd */
    if(!(f->flags&MI_ZF))goto mi_004e9603;
    /* 004e95ff */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e9601 */
    if((f->flags&MI_ZF))goto mi_004e960d;
mi_004e9603:;
    /* 004e9603 */
    MI_U32((0x00ea5c94u+f->r[0]))=(uint32_t)(0x00000003u);
mi_004e960d:;
    /* 004e960d */
    MI_U32((0x00ea5c98u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004e9613 */
    f->r[1]=MI_U32((0x00ea5b7cu+f->r[0]));
    /* 004e9619 */
    a=MI_U32((0x00ea5bf8u+f->r[0]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e961f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e9629;
    /* 004e9621 */
    MI_U32((0x00ea5c98u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e9627 */
    goto mi_004e9633;
mi_004e9629:;
    /* 004e9629 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e962b */
    if((f->flags&MI_ZF))goto mi_004e9633;
    /* 004e962d */
    MI_U32((0x00ea5c98u+f->r[0]))=(uint32_t)(f->r[5]);
mi_004e9633:;
    /* 004e9633 */
    MI_U32((0x00ea5c90u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004e9639 */
    f->r[2]=MI_U32((0x00ea5bc0u+f->r[0]));
    /* 004e963f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e9641 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e9643 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e9651;
    /* 004e9645 */
    MI_U32((0x00ea5c90u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e964b */
    f->r[1]=MI_U32((0x00ea5bc0u+f->r[0]));
mi_004e9651:;
    /* 004e9651 */
    a=MI_U32((0x00ea5bc4u+f->r[0]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e9657 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e9665;
    /* 004e9659 */
    MI_U32((0x00ea5c90u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 004e965f */
    f->r[1]=MI_U32((0x00ea5bc4u+f->r[0]));
mi_004e9665:;
    /* 004e9665 */
    f->r[2]=MI_U32((0x00ea5bc8u+f->r[0]));
    /* 004e966b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e966c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e966d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e966f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e9670 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e967c;
    /* 004e9672 */
    MI_U32((0x00ea5c90u+f->r[0]))=(uint32_t)(0x00000003u);
mi_004e967c:;
    /* 004e967c */
    f->r[4]+=4;return;
}

void restore_advisor_C_004c06e0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c06e0 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004c06e4 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c06e7 */
    if((f->flags&MI_ZF))goto mi_004c0797;
    /* 004c06ed */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c06ee */
    if((f->flags&MI_ZF))goto mi_004c0747;
    /* 004c06f0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c06f1 */
    if(!(f->flags&MI_ZF))goto mi_004c07e3;
    /* 004c06f7 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004c06fb */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004c06fe */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004c07e3;
    /* 004c0704 */
    switch(mi_read((0x004c07e8u+f->r[0]*4u),4u)){case 0x004c070bu:goto mi_004c070b;case 0x004c0711u:goto mi_004c0711;case 0x004c0717u:goto mi_004c0717;case 0x004c071du:goto mi_004c071d;case 0x004c0723u:goto mi_004c0723;case 0x004c0729u:goto mi_004c0729;case 0x004c072fu:goto mi_004c072f;case 0x004c0735u:goto mi_004c0735;case 0x004c073bu:goto mi_004c073b;case 0x004c0741u:goto mi_004c0741;default:abort();}
mi_004c070b:;
    /* 004c070b */
    f->r[0]=0x0000008cu;
    /* 004c0710 */
    f->r[4]+=4;return;
mi_004c0711:;
    /* 004c0711 */
    f->r[0]=0x0000008du;
    /* 004c0716 */
    f->r[4]+=4;return;
mi_004c0717:;
    /* 004c0717 */
    f->r[0]=0x0000008eu;
    /* 004c071c */
    f->r[4]+=4;return;
mi_004c071d:;
    /* 004c071d */
    f->r[0]=0x0000008fu;
    /* 004c0722 */
    f->r[4]+=4;return;
mi_004c0723:;
    /* 004c0723 */
    f->r[0]=0x00000090u;
    /* 004c0728 */
    f->r[4]+=4;return;
mi_004c0729:;
    /* 004c0729 */
    f->r[0]=0x00000091u;
    /* 004c072e */
    f->r[4]+=4;return;
mi_004c072f:;
    /* 004c072f */
    f->r[0]=0x00000092u;
    /* 004c0734 */
    f->r[4]+=4;return;
mi_004c0735:;
    /* 004c0735 */
    f->r[0]=0x00000093u;
    /* 004c073a */
    f->r[4]+=4;return;
mi_004c073b:;
    /* 004c073b */
    f->r[0]=0x00000094u;
    /* 004c0740 */
    f->r[4]+=4;return;
mi_004c0741:;
    /* 004c0741 */
    f->r[0]=0x00000095u;
    /* 004c0746 */
    f->r[4]+=4;return;
mi_004c0747:;
    /* 004c0747 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004c074b */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004c074e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004c07e3;
    /* 004c0754 */
    switch(mi_read((0x004c0810u+f->r[0]*4u),4u)){case 0x004c075bu:goto mi_004c075b;case 0x004c0761u:goto mi_004c0761;case 0x004c0767u:goto mi_004c0767;case 0x004c076du:goto mi_004c076d;case 0x004c0773u:goto mi_004c0773;case 0x004c0779u:goto mi_004c0779;case 0x004c077fu:goto mi_004c077f;case 0x004c0785u:goto mi_004c0785;case 0x004c078bu:goto mi_004c078b;case 0x004c0791u:goto mi_004c0791;default:abort();}
mi_004c075b:;
    /* 004c075b */
    f->r[0]=0x00000041u;
    /* 004c0760 */
    f->r[4]+=4;return;
mi_004c0761:;
    /* 004c0761 */
    f->r[0]=0x00000042u;
    /* 004c0766 */
    f->r[4]+=4;return;
mi_004c0767:;
    /* 004c0767 */
    f->r[0]=0x00000043u;
    /* 004c076c */
    f->r[4]+=4;return;
mi_004c076d:;
    /* 004c076d */
    f->r[0]=0x00000044u;
    /* 004c0772 */
    f->r[4]+=4;return;
mi_004c0773:;
    /* 004c0773 */
    f->r[0]=0x00000045u;
    /* 004c0778 */
    f->r[4]+=4;return;
mi_004c0779:;
    /* 004c0779 */
    f->r[0]=0x0000009cu;
    /* 004c077e */
    f->r[4]+=4;return;
mi_004c077f:;
    /* 004c077f */
    f->r[0]=0x0000009du;
    /* 004c0784 */
    f->r[4]+=4;return;
mi_004c0785:;
    /* 004c0785 */
    f->r[0]=0x0000009eu;
    /* 004c078a */
    f->r[4]+=4;return;
mi_004c078b:;
    /* 004c078b */
    f->r[0]=0x0000009fu;
    /* 004c0790 */
    f->r[4]+=4;return;
mi_004c0791:;
    /* 004c0791 */
    f->r[0]=0x000000a0u;
    /* 004c0796 */
    f->r[4]+=4;return;
mi_004c0797:;
    /* 004c0797 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004c079b */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004c079e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004c07e3;
    /* 004c07a0 */
    switch(mi_read((0x004c0838u+f->r[0]*4u),4u)){case 0x004c07a7u:goto mi_004c07a7;case 0x004c07adu:goto mi_004c07ad;case 0x004c07b3u:goto mi_004c07b3;case 0x004c07b9u:goto mi_004c07b9;case 0x004c07bfu:goto mi_004c07bf;case 0x004c07c5u:goto mi_004c07c5;case 0x004c07cbu:goto mi_004c07cb;case 0x004c07d1u:goto mi_004c07d1;case 0x004c07d7u:goto mi_004c07d7;case 0x004c07ddu:goto mi_004c07dd;default:abort();}
mi_004c07a7:;
    /* 004c07a7 */
    f->r[0]=0x0000003cu;
    /* 004c07ac */
    f->r[4]+=4;return;
mi_004c07ad:;
    /* 004c07ad */
    f->r[0]=0x0000003du;
    /* 004c07b2 */
    f->r[4]+=4;return;
mi_004c07b3:;
    /* 004c07b3 */
    f->r[0]=0x0000003eu;
    /* 004c07b8 */
    f->r[4]+=4;return;
mi_004c07b9:;
    /* 004c07b9 */
    f->r[0]=0x0000003fu;
    /* 004c07be */
    f->r[4]+=4;return;
mi_004c07bf:;
    /* 004c07bf */
    f->r[0]=0x00000040u;
    /* 004c07c4 */
    f->r[4]+=4;return;
mi_004c07c5:;
    /* 004c07c5 */
    f->r[0]=0x00000097u;
    /* 004c07ca */
    f->r[4]+=4;return;
mi_004c07cb:;
    /* 004c07cb */
    f->r[0]=0x00000098u;
    /* 004c07d0 */
    f->r[4]+=4;return;
mi_004c07d1:;
    /* 004c07d1 */
    f->r[0]=0x00000099u;
    /* 004c07d6 */
    f->r[4]+=4;return;
mi_004c07d7:;
    /* 004c07d7 */
    f->r[0]=0x0000009au;
    /* 004c07dc */
    f->r[4]+=4;return;
mi_004c07dd:;
    /* 004c07dd */
    f->r[0]=0x0000009bu;
    /* 004c07e2 */
    f->r[4]+=4;return;
mi_004c07e3:;
    /* 004c07e3 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004c07e6 */
    f->r[4]+=4;return;
}

void restore_advisor_C_004e94a0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e94a0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004e94a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e94a6 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e94ac */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e94ad */
    a=f->r[1];b=0x00ea3e34u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e94b3 */
    f->r[2]=0x0000000eu;
mi_004e94b8:;
    /* 004e94b8 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[1]));
    /* 004e94bb */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e94bd */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e94c0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004e94c1 */
    if(!(f->flags&MI_ZF))goto mi_004e94b8;
    /* 004e94c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e94c4 */
    f->r[4]+=4;return;
}

void restore_advisor_C_004e94d0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e94d0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004e94d4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e94d6 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e94dc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e94dd */
    a=f->r[1];b=0x00ea3e50u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e94e3 */
    f->r[2]=0x00000007u;
mi_004e94e8:;
    /* 004e94e8 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[1]));
    /* 004e94eb */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e94ed */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e94f0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004e94f1 */
    if(!(f->flags&MI_ZF))goto mi_004e94e8;
    /* 004e94f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e94f4 */
    f->r[4]+=4;return;
}

void restore_advisor_C_004e7e60(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e7e60 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7e61 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7e62 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7e63 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7e64 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e7e66 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004e7e68 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004e7e6a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004e7e6c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004e7e6e */
    f->r[0]=0x00799afcu;
mi_004e7e73:;
    /* 004e7e73 */
    a=MI_U8((0xffffffe4u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e7e77 */
    if(!(f->flags&MI_ZF))goto mi_004e7ead;
    /* 004e7e79 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffffe9u+f->r[0])))&0xffu)<<0);
    /* 004e7e7c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e7e7e */
    if((f->flags&MI_ZF))goto mi_004e7ead;
    /* 004e7e80 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[0])))&0xffffu)<<0);
    /* 004e7e83 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004e7e86 */
    if((f->flags&MI_ZF))goto mi_004e7ead;
    /* 004e7e88 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 004e7e8b */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e7e8d */
    a=MI_U8((0x0000005au+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004e7e91 */
    if((f->flags&MI_ZF))goto mi_004e7e95;
    /* 004e7e93 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_004e7e95:;
    /* 004e7e95 */
    a=MI_U8((0x0000005cu+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004e7e99 */
    if((f->flags&MI_ZF))goto mi_004e7e9d;
    /* 004e7e9b */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_004e7e9d:;
    /* 004e7e9d */
    a=MI_U8((0x00000059u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004e7ea1 */
    if((f->flags&MI_ZF))goto mi_004e7ea5;
    /* 004e7ea3 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_004e7ea5:;
    /* 004e7ea5 */
    a=MI_U8((0x00000052u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004e7ea9 */
    if((f->flags&MI_ZF))goto mi_004e7ead;
    /* 004e7eab */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004e7ead:;
    /* 004e7ead */
    a=f->r[0];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e7eb2 */
    a=f->r[0];b=0x0089b6f4u;t=mi_alu(f,a,b,32,1);
    /* 004e7eb7 */
    if(mi_signed_less(f->flags))goto mi_004e7e73;
    /* 004e7eb9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7eba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ebb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7ec0u;f->boundary(0x004e7ebbu,0x004cde60u,f);
    /* 004e7ec0 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004e7ec4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ec5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ec6 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e7ec8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7ecdu;f->boundary(0x004e7ec8u,0x004cde60u,f);
    /* 004e7ecd */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004e7ed1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ed2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ed3 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 004e7ed5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7edau;f->boundary(0x004e7ed5u,0x004cde60u,f);
    /* 004e7eda */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 004e7ede */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7edf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ee0 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004e7ee2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7ee7u;f->boundary(0x004e7ee2u,0x004cde60u,f);
    /* 004e7ee7 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 004e7eeb */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7eee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e7eef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e7ef0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e7ef1 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 004e7ef3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e7ef4 */
    f->r[4]+=4;return;
}

void restore_advisor_C_004f6ad0(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f6ad0 */
    f->r[0]=MI_U32((0x00784c38u));
    /* 004f6ad5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f6ad6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f6adbu;f->boundary(0x004f6ad6u,0x004ea280u,f);
    /* 004f6adb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f6ade */
    MI_U32((0x00eadb28u))=(uint32_t)(f->r[0]);
    /* 004f6ae3 */
    f->r[4]+=4;return;
}

void restore_advisor_C_004ea280(struct restore_advisor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ea280 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004ea284 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ea285 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ea286 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ea288 */
    f->r[1]=0x00799ae5u;
mi_004ea28d:;
    /* 004ea28d */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0xfffffffbu+f->r[1])))&0xffu)<<0);
    /* 004ea290 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ea292 */
    if((f->flags&MI_ZF))goto mi_004ea2ac;
    /* 004ea294 */
    a=MI_U8((0x00000000u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004ea297 */
    if((f->flags&MI_ZF))goto mi_004ea2ac;
    /* 004ea299 */
    a=MI_U16((0x00000017u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004ea29e */
    if((f->flags&MI_ZF))goto mi_004ea2ac;
    /* 004ea2a0 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000bu+f->r[1]));
    /* 004ea2a4 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004ea2a7 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004ea2a9 */
    if(mi_signed_less(f->flags))goto mi_004ea2ac;
    /* 004ea2ab */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_004ea2ac:;
    /* 004ea2ac */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ea2b2 */
    a=f->r[1];b=0x0089b6ddu;t=mi_alu(f,a,b,32,1);
    /* 004ea2b8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ea28d;
    /* 004ea2ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ea2bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ea2bc */
    f->r[4]+=4;return;
}
