/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_profile_model.h"
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
static __inline uint32_t mi_alu(struct restore_menu_input_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_menu_input_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_menu_input_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_004b62cc[]={0x8e,0x62,0x4b,0x00,0x6b,0x5b,0x4b,0x00,0xce,0x5e,0x4b,0x00,0x68,0x5e,0x4b,0x00,0x35,0x5e,0x4b,0x00,0x03,0x5d,0x4b,0x00,0x36,0x5d,0x4b,0x00,0x69,0x5d,0x4b,0x00,0xd0,0x5c,0x4b,0x00,0x9d,0x5c,0x4b,0x00,0x9e,0x5b,0x4b,0x00,0x37,0x5c,0x4b,0x00,0x9c,0x5d,0x4b,0x00,0xcf,0x5d,0x4b,0x00,0x02,0x5e,0x4b,0x00,0xd1,0x5b,0x4b,0x00,0x67,0x5f,0x4b,0x00,0x9a,0x5f,0x4b,0x00,0x6a,0x5c,0x4b,0x00,0x04,0x5c,0x4b,0x00,0x34,0x5f,0x4b,0x00,0x9b,0x5e,0x4b,0x00,0x01,0x5f,0x4b,0x00,0x30,0x62,0x4b,0x00,0x5f,0x62,0x4b,0x00,0xc0,0x62,0x4b,0x00};
static const uint8_t mi_rom_004b6334[]={0x00,0x00,0x01,0x00,0x00,0x19,0x02,0x03,0x00,0x04,0x19,0x19,0x19,0x19,0x05,0x06,0x07,0x05,0x08,0x09,0x08,0x0a,0x0a,0x19,0x19,0x0b,0x19,0x19,0x0c,0x0c,0x0d,0x0d,0x0d,0x0e,0x00,0x19,0x19,0x00,0x0b,0x0f,0x19,0x10,0x11,0x11,0x11,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x00,0x00,0x00,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x12,0x19,0x19,0x19,0x19,0x19,0x19,0x13,0x19,0x00,0x14,0x19,0x19,0x00,0x00,0x00,0x19,0x19,0x19,0x00,0x19,0x00,0x15,0x16,0x00,0x19,0x17,0x17,0x19,0x19,0x00,0x19,0x00,0x18,0x17,0x17,0x19,0x19,0x19,0x05,0x19,0x19,0x00};
static const uint8_t mi_rom_004b63a0[]={0x86,0x60,0x4b,0x00,0xb8,0x61,0x4b,0x00,0xb9,0x60,0x4b,0x00,0xeb,0x61,0x4b,0x00,0x53,0x60,0x4b,0x00,0x20,0x60,0x4b,0x00,0xed,0x5f,0x4b,0x00,0xba,0x5f,0x4b,0x00,0xec,0x60,0x4b,0x00,0x1f,0x61,0x4b,0x00,0x52,0x61,0x4b,0x00,0x85,0x61,0x4b,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004b62ccu&&width<=104u&&(offset=address-0x004b62ccu)<=104u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b62cc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b6334u&&width<=108u&&(offset=address-0x004b6334u)<=108u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b6334[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b63a0u&&width<=48u&&(offset=address-0x004b63a0u)<=48u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b63a0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_004b5af0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b5af0 */
    f->r[1]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004b5af4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b5af5 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b5af8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b5afb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b5afd */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b5b00 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[0]),1u))&0xffu)<<0);
    /* 004b5b06 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd658u+f->r[0]),2u);
    /* 004b5b0d */
    a=(f->r[1]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004b5b10 */
    if((f->flags&MI_ZF))goto mi_004b62ca;
    /* 004b5b16 */
    a=(f->r[1]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 004b5b19 */
    if((f->flags&MI_ZF))goto mi_004b62ca;
    /* 004b5b1f */
    a=(f->r[1]&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,1);
    /* 004b5b22 */
    if((f->flags&MI_ZF))goto mi_004b62ca;
    /* 004b5b28 */
    a=(f->r[1]&0xffu);b=0x0000005fu;t=mi_alu(f,a,b,8,1);
    /* 004b5b2b */
    if((f->flags&MI_ZF))goto mi_004b62ca;
    /* 004b5b31 */
    a=(f->r[1]&0xffu);b=0x0000003fu;t=mi_alu(f,a,b,8,1);
    /* 004b5b34 */
    if((f->flags&MI_ZF))goto mi_004b62ca;
    /* 004b5b3a */
    a=(f->r[1]&0xffu);b=0x00000048u;t=mi_alu(f,a,b,8,1);
    /* 004b5b3d */
    if((f->flags&MI_ZF))goto mi_004b62ca;
    /* 004b5b43 */
    a=(f->r[1]&0xffu);b=0x00000061u;t=mi_alu(f,a,b,8,1);
    /* 004b5b46 */
    if((f->flags&MI_ZF))goto mi_004b62ca;
    /* 004b5b4c */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004b5b52 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5b53 */
    a=f->r[1];b=0x0000006bu;t=mi_alu(f,a,b,32,1);
    /* 004b5b56 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b62c0;
    /* 004b5b5c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b5b5e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x004b6334u+f->r[1]),1u))&0xffu)<<0);
    /* 004b5b64 */
    switch(mi_read((0x004b62ccu+f->r[2]*4u),4u)){case 0x004b5b6bu:goto mi_004b5b6b;case 0x004b5b9eu:goto mi_004b5b9e;case 0x004b5bd1u:goto mi_004b5bd1;case 0x004b5c04u:goto mi_004b5c04;case 0x004b5c37u:goto mi_004b5c37;case 0x004b5c6au:goto mi_004b5c6a;case 0x004b5c9du:goto mi_004b5c9d;case 0x004b5cd0u:goto mi_004b5cd0;case 0x004b5d03u:goto mi_004b5d03;case 0x004b5d36u:goto mi_004b5d36;case 0x004b5d69u:goto mi_004b5d69;case 0x004b5d9cu:goto mi_004b5d9c;case 0x004b5dcfu:goto mi_004b5dcf;case 0x004b5e02u:goto mi_004b5e02;case 0x004b5e35u:goto mi_004b5e35;case 0x004b5e68u:goto mi_004b5e68;case 0x004b5e9bu:goto mi_004b5e9b;case 0x004b5eceu:goto mi_004b5ece;case 0x004b5f01u:goto mi_004b5f01;case 0x004b5f34u:goto mi_004b5f34;case 0x004b5f67u:goto mi_004b5f67;case 0x004b5f9au:goto mi_004b5f9a;case 0x004b6230u:goto mi_004b6230;case 0x004b625fu:goto mi_004b625f;case 0x004b628eu:goto mi_004b628e;case 0x004b62c0u:goto mi_004b62c0;default:abort();}
mi_004b5b6b:;
    /* 004b5b6b */
    f->r[1]=mi_read((0x00d056dcu),4u);
    /* 004b5b71 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5b78 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5b79 */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5b7c */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x000000ffu);
    /* 004b5b86 */
    MI_U32((0x00d056dcu))=(uint32_t)(f->r[1]);
    /* 004b5b8c */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5b92 */
    MI_U32((0x00d056dcu))=(uint32_t)(0x00000000u);
    /* 004b5b9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5b9d */
    f->r[4]+=4;return;
mi_004b5b9e:;
    /* 004b5b9e */
    f->r[1]=mi_read((0x00d056d8u),4u);
    /* 004b5ba4 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5bab */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5bac */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5baf */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000100u);
    /* 004b5bb9 */
    MI_U32((0x00d056d8u))=(uint32_t)(f->r[1]);
    /* 004b5bbf */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5bc5 */
    MI_U32((0x00d056d8u))=(uint32_t)(0x00000000u);
    /* 004b5bcf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5bd0 */
    f->r[4]+=4;return;
mi_004b5bd1:;
    /* 004b5bd1 */
    f->r[1]=mi_read((0x00d056d4u),4u);
    /* 004b5bd7 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5bde */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5bdf */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5be2 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000101u);
    /* 004b5bec */
    MI_U32((0x00d056d4u))=(uint32_t)(f->r[1]);
    /* 004b5bf2 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5bf8 */
    MI_U32((0x00d056d4u))=(uint32_t)(0x00000000u);
    /* 004b5c02 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5c03 */
    f->r[4]+=4;return;
mi_004b5c04:;
    /* 004b5c04 */
    f->r[1]=mi_read((0x00d056d0u),4u);
    /* 004b5c0a */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5c11 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5c12 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5c15 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000102u);
    /* 004b5c1f */
    MI_U32((0x00d056d0u))=(uint32_t)(f->r[1]);
    /* 004b5c25 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5c2b */
    MI_U32((0x00d056d0u))=(uint32_t)(0x00000000u);
    /* 004b5c35 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5c36 */
    f->r[4]+=4;return;
mi_004b5c37:;
    /* 004b5c37 */
    f->r[1]=mi_read((0x00d056ccu),4u);
    /* 004b5c3d */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5c44 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5c45 */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5c48 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000103u);
    /* 004b5c52 */
    MI_U32((0x00d056ccu))=(uint32_t)(f->r[1]);
    /* 004b5c58 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5c5e */
    MI_U32((0x00d056ccu))=(uint32_t)(0x00000000u);
    /* 004b5c68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5c69 */
    f->r[4]+=4;return;
mi_004b5c6a:;
    /* 004b5c6a */
    f->r[1]=mi_read((0x00d056c8u),4u);
    /* 004b5c70 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5c77 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5c78 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5c7b */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000104u);
    /* 004b5c85 */
    MI_U32((0x00d056c8u))=(uint32_t)(f->r[1]);
    /* 004b5c8b */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5c91 */
    MI_U32((0x00d056c8u))=(uint32_t)(0x00000000u);
    /* 004b5c9b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5c9c */
    f->r[4]+=4;return;
mi_004b5c9d:;
    /* 004b5c9d */
    f->r[1]=mi_read((0x00d056c4u),4u);
    /* 004b5ca3 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5caa */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5cab */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5cae */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000105u);
    /* 004b5cb8 */
    MI_U32((0x00d056c4u))=(uint32_t)(f->r[1]);
    /* 004b5cbe */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5cc4 */
    MI_U32((0x00d056c4u))=(uint32_t)(0x00000000u);
    /* 004b5cce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5ccf */
    f->r[4]+=4;return;
mi_004b5cd0:;
    /* 004b5cd0 */
    f->r[1]=mi_read((0x00d056c0u),4u);
    /* 004b5cd6 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5cdd */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5cde */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5ce1 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000106u);
    /* 004b5ceb */
    MI_U32((0x00d056c0u))=(uint32_t)(f->r[1]);
    /* 004b5cf1 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5cf7 */
    MI_U32((0x00d056c0u))=(uint32_t)(0x00000000u);
    /* 004b5d01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5d02 */
    f->r[4]+=4;return;
mi_004b5d03:;
    /* 004b5d03 */
    f->r[1]=mi_read((0x00d056bcu),4u);
    /* 004b5d09 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5d10 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5d11 */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5d14 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000107u);
    /* 004b5d1e */
    MI_U32((0x00d056bcu))=(uint32_t)(f->r[1]);
    /* 004b5d24 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5d2a */
    MI_U32((0x00d056bcu))=(uint32_t)(0x00000000u);
    /* 004b5d34 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5d35 */
    f->r[4]+=4;return;
mi_004b5d36:;
    /* 004b5d36 */
    f->r[1]=mi_read((0x00d056b8u),4u);
    /* 004b5d3c */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5d43 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5d44 */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5d47 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000108u);
    /* 004b5d51 */
    MI_U32((0x00d056b8u))=(uint32_t)(f->r[1]);
    /* 004b5d57 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5d5d */
    MI_U32((0x00d056b8u))=(uint32_t)(0x00000000u);
    /* 004b5d67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5d68 */
    f->r[4]+=4;return;
mi_004b5d69:;
    /* 004b5d69 */
    f->r[1]=mi_read((0x00d056b4u),4u);
    /* 004b5d6f */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5d76 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5d77 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5d7a */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000109u);
    /* 004b5d84 */
    MI_U32((0x00d056b4u))=(uint32_t)(f->r[1]);
    /* 004b5d8a */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5d90 */
    MI_U32((0x00d056b4u))=(uint32_t)(0x00000000u);
    /* 004b5d9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5d9b */
    f->r[4]+=4;return;
mi_004b5d9c:;
    /* 004b5d9c */
    f->r[1]=mi_read((0x00d0569cu),4u);
    /* 004b5da2 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5da9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5daa */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5dad */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000010fu);
    /* 004b5db7 */
    MI_U32((0x00d0569cu))=(uint32_t)(f->r[1]);
    /* 004b5dbd */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5dc3 */
    MI_U32((0x00d0569cu))=(uint32_t)(0x00000000u);
    /* 004b5dcd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5dce */
    f->r[4]+=4;return;
mi_004b5dcf:;
    /* 004b5dcf */
    f->r[1]=mi_read((0x00d05698u),4u);
    /* 004b5dd5 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5ddc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5ddd */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5de0 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000110u);
    /* 004b5dea */
    MI_U32((0x00d05698u))=(uint32_t)(f->r[1]);
    /* 004b5df0 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5df6 */
    MI_U32((0x00d05698u))=(uint32_t)(0x00000000u);
    /* 004b5e00 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5e01 */
    f->r[4]+=4;return;
mi_004b5e02:;
    /* 004b5e02 */
    f->r[1]=mi_read((0x00d05694u),4u);
    /* 004b5e08 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5e0f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5e10 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5e13 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000111u);
    /* 004b5e1d */
    MI_U32((0x00d05694u))=(uint32_t)(f->r[1]);
    /* 004b5e23 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5e29 */
    MI_U32((0x00d05694u))=(uint32_t)(0x00000000u);
    /* 004b5e33 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5e34 */
    f->r[4]+=4;return;
mi_004b5e35:;
    /* 004b5e35 */
    f->r[1]=mi_read((0x00d05690u),4u);
    /* 004b5e3b */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5e42 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5e43 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5e46 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000112u);
    /* 004b5e50 */
    MI_U32((0x00d05690u))=(uint32_t)(f->r[1]);
    /* 004b5e56 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5e5c */
    MI_U32((0x00d05690u))=(uint32_t)(0x00000000u);
    /* 004b5e66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5e67 */
    f->r[4]+=4;return;
mi_004b5e68:;
    /* 004b5e68 */
    f->r[1]=mi_read((0x00d0568cu),4u);
    /* 004b5e6e */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5e75 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5e76 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5e79 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000113u);
    /* 004b5e83 */
    MI_U32((0x00d0568cu))=(uint32_t)(f->r[1]);
    /* 004b5e89 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5e8f */
    MI_U32((0x00d0568cu))=(uint32_t)(0x00000000u);
    /* 004b5e99 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5e9a */
    f->r[4]+=4;return;
mi_004b5e9b:;
    /* 004b5e9b */
    f->r[1]=mi_read((0x00d05688u),4u);
    /* 004b5ea1 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5ea8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5ea9 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5eac */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000114u);
    /* 004b5eb6 */
    MI_U32((0x00d05688u))=(uint32_t)(f->r[1]);
    /* 004b5ebc */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5ec2 */
    MI_U32((0x00d05688u))=(uint32_t)(0x00000000u);
    /* 004b5ecc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5ecd */
    f->r[4]+=4;return;
mi_004b5ece:;
    /* 004b5ece */
    f->r[1]=mi_read((0x00d05684u),4u);
    /* 004b5ed4 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5edb */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5edc */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5edf */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000115u);
    /* 004b5ee9 */
    MI_U32((0x00d05684u))=(uint32_t)(f->r[1]);
    /* 004b5eef */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5ef5 */
    MI_U32((0x00d05684u))=(uint32_t)(0x00000000u);
    /* 004b5eff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5f00 */
    f->r[4]+=4;return;
mi_004b5f01:;
    /* 004b5f01 */
    f->r[1]=mi_read((0x00d05680u),4u);
    /* 004b5f07 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5f0e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5f0f */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5f12 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000116u);
    /* 004b5f1c */
    MI_U32((0x00d05680u))=(uint32_t)(f->r[1]);
    /* 004b5f22 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5f28 */
    MI_U32((0x00d05680u))=(uint32_t)(0x00000000u);
    /* 004b5f32 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5f33 */
    f->r[4]+=4;return;
mi_004b5f34:;
    /* 004b5f34 */
    f->r[1]=mi_read((0x00d0567cu),4u);
    /* 004b5f3a */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5f41 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5f42 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5f45 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000117u);
    /* 004b5f4f */
    MI_U32((0x00d0567cu))=(uint32_t)(f->r[1]);
    /* 004b5f55 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5f5b */
    MI_U32((0x00d0567cu))=(uint32_t)(0x00000000u);
    /* 004b5f65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5f66 */
    f->r[4]+=4;return;
mi_004b5f67:;
    /* 004b5f67 */
    f->r[1]=mi_read((0x00d05678u),4u);
    /* 004b5f6d */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5f74 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5f75 */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 004b5f78 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000118u);
    /* 004b5f82 */
    MI_U32((0x00d05678u))=(uint32_t)(f->r[1]);
    /* 004b5f88 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5f8e */
    MI_U32((0x00d05678u))=(uint32_t)(0x00000000u);
    /* 004b5f98 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5f99 */
    f->r[4]+=4;return;
mi_004b5f9a:;
    /* 004b5f9a */
    f->r[1]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004b5f9d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b5f9f */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b5fa2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcba32u+f->r[1]),1u))&0xffu)<<0);
    /* 004b5fa8 */
    f->r[1]=f->r[2];
    /* 004b5faa */
    a=f->r[1];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004b5fad */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b621e;
    /* 004b5fb3 */
    switch(mi_read((0x004b63a0u+f->r[1]*4u),4u)){case 0x004b5fbau:goto mi_004b5fba;case 0x004b5fedu:goto mi_004b5fed;case 0x004b6020u:goto mi_004b6020;case 0x004b6053u:goto mi_004b6053;case 0x004b6086u:goto mi_004b6086;case 0x004b60b9u:goto mi_004b60b9;case 0x004b60ecu:goto mi_004b60ec;case 0x004b611fu:goto mi_004b611f;case 0x004b6152u:goto mi_004b6152;case 0x004b6185u:goto mi_004b6185;case 0x004b61b8u:goto mi_004b61b8;case 0x004b61ebu:goto mi_004b61eb;default:abort();}
mi_004b5fba:;
    /* 004b5fba */
    f->r[1]=mi_read((0x00d05674u),4u);
    /* 004b5fc0 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5fc7 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5fc8 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5fcb */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000119u);
    /* 004b5fd5 */
    MI_U32((0x00d05674u))=(uint32_t)(f->r[1]);
    /* 004b5fdb */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b5fe1 */
    MI_U32((0x00d05674u))=(uint32_t)(0x00000000u);
    /* 004b5feb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b5fec */
    f->r[4]+=4;return;
mi_004b5fed:;
    /* 004b5fed */
    f->r[1]=mi_read((0x00d05670u),4u);
    /* 004b5ff3 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b5ffa */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b5ffb */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b5ffe */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000011au);
    /* 004b6008 */
    MI_U32((0x00d05670u))=(uint32_t)(f->r[1]);
    /* 004b600e */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6014 */
    MI_U32((0x00d05670u))=(uint32_t)(0x00000000u);
    /* 004b601e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b601f */
    f->r[4]+=4;return;
mi_004b6020:;
    /* 004b6020 */
    f->r[1]=mi_read((0x00d0566cu),4u);
    /* 004b6026 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b602d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b602e */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6031 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000011bu);
    /* 004b603b */
    MI_U32((0x00d0566cu))=(uint32_t)(f->r[1]);
    /* 004b6041 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6047 */
    MI_U32((0x00d0566cu))=(uint32_t)(0x00000000u);
    /* 004b6051 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b6052 */
    f->r[4]+=4;return;
mi_004b6053:;
    /* 004b6053 */
    f->r[1]=mi_read((0x00d05668u),4u);
    /* 004b6059 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6060 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b6061 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6064 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000011cu);
    /* 004b606e */
    MI_U32((0x00d05668u))=(uint32_t)(f->r[1]);
    /* 004b6074 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b607a */
    MI_U32((0x00d05668u))=(uint32_t)(0x00000000u);
    /* 004b6084 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b6085 */
    f->r[4]+=4;return;
mi_004b6086:;
    /* 004b6086 */
    f->r[1]=mi_read((0x00d05664u),4u);
    /* 004b608c */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6093 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b6094 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6097 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000011du);
    /* 004b60a1 */
    MI_U32((0x00d05664u))=(uint32_t)(f->r[1]);
    /* 004b60a7 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b60ad */
    MI_U32((0x00d05664u))=(uint32_t)(0x00000000u);
    /* 004b60b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b60b8 */
    f->r[4]+=4;return;
mi_004b60b9:;
    /* 004b60b9 */
    f->r[1]=mi_read((0x00d05660u),4u);
    /* 004b60bf */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b60c6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b60c7 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b60ca */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000011eu);
    /* 004b60d4 */
    MI_U32((0x00d05660u))=(uint32_t)(f->r[1]);
    /* 004b60da */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b60e0 */
    MI_U32((0x00d05660u))=(uint32_t)(0x00000000u);
    /* 004b60ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b60eb */
    f->r[4]+=4;return;
mi_004b60ec:;
    /* 004b60ec */
    f->r[1]=mi_read((0x00d0564cu),4u);
    /* 004b60f2 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b60f9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b60fa */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b60fd */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000012cu);
    /* 004b6107 */
    MI_U32((0x00d0564cu))=(uint32_t)(f->r[1]);
    /* 004b610d */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6113 */
    MI_U32((0x00d0564cu))=(uint32_t)(0x00000000u);
    /* 004b611d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b611e */
    f->r[4]+=4;return;
mi_004b611f:;
    /* 004b611f */
    f->r[1]=mi_read((0x00d05648u),4u);
    /* 004b6125 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b612c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b612d */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6130 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000012du);
    /* 004b613a */
    MI_U32((0x00d05648u))=(uint32_t)(f->r[1]);
    /* 004b6140 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6146 */
    MI_U32((0x00d05648u))=(uint32_t)(0x00000000u);
    /* 004b6150 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b6151 */
    f->r[4]+=4;return;
mi_004b6152:;
    /* 004b6152 */
    f->r[1]=mi_read((0x00d05644u),4u);
    /* 004b6158 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b615f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b6160 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6163 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000012eu);
    /* 004b616d */
    MI_U32((0x00d05644u))=(uint32_t)(f->r[1]);
    /* 004b6173 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6179 */
    MI_U32((0x00d05644u))=(uint32_t)(0x00000000u);
    /* 004b6183 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b6184 */
    f->r[4]+=4;return;
mi_004b6185:;
    /* 004b6185 */
    f->r[1]=mi_read((0x00d05640u),4u);
    /* 004b618b */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6192 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b6193 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6196 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000012fu);
    /* 004b61a0 */
    MI_U32((0x00d05640u))=(uint32_t)(f->r[1]);
    /* 004b61a6 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b61ac */
    MI_U32((0x00d05640u))=(uint32_t)(0x00000000u);
    /* 004b61b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b61b7 */
    f->r[4]+=4;return;
mi_004b61b8:;
    /* 004b61b8 */
    f->r[1]=mi_read((0x00d0565cu),4u);
    /* 004b61be */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b61c5 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b61c6 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b61c9 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x0000011fu);
    /* 004b61d3 */
    MI_U32((0x00d0565cu))=(uint32_t)(f->r[1]);
    /* 004b61d9 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b61df */
    MI_U32((0x00d0565cu))=(uint32_t)(0x00000000u);
    /* 004b61e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b61ea */
    f->r[4]+=4;return;
mi_004b61eb:;
    /* 004b61eb */
    f->r[1]=mi_read((0x00d05658u),4u);
    /* 004b61f1 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b61f8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b61f9 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b61fc */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000120u);
    /* 004b6206 */
    MI_U32((0x00d05658u))=(uint32_t)(f->r[1]);
    /* 004b620c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6212 */
    MI_U32((0x00d05658u))=(uint32_t)(0x00000000u);
    /* 004b621c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b621d */
    f->r[4]+=4;return;
mi_004b621e:;
    /* 004b621e */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 004b6221 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b6222 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6229 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004b622f */
    f->r[4]+=4;return;
mi_004b6230:;
    /* 004b6230 */
    f->r[1]=mi_read((0x00d05654u),4u);
    /* 004b6236 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b623d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b623e */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6241 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000121u);
    /* 004b624b */
    MI_U32((0x00d05654u))=(uint32_t)(f->r[1]);
    /* 004b6251 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6253 */
    MI_U32((0x00d05654u))=(uint32_t)(0x00000000u);
    /* 004b625d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b625e */
    f->r[4]+=4;return;
mi_004b625f:;
    /* 004b625f */
    f->r[1]=mi_read((0x00d05650u),4u);
    /* 004b6265 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b626c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b626d */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b6270 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x00000122u);
    /* 004b627a */
    MI_U32((0x00d05650u))=(uint32_t)(f->r[1]);
    /* 004b6280 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b6282 */
    MI_U32((0x00d05650u))=(uint32_t)(0x00000000u);
    /* 004b628c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b628d */
    f->r[4]+=4;return;
mi_004b628e:;
    /* 004b628e */
    f->r[1]=mi_read((0x00d056e0u),4u);
    /* 004b6294 */
    MI_U16((0x00bcd668u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b629b */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b629c */
    a=f->r[1];b=0x00000080u;t=mi_alu(f,a,b,32,1);
    /* 004b62a2 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0x000000feu);
    /* 004b62ac */
    MI_U32((0x00d056e0u))=(uint32_t)(f->r[1]);
    /* 004b62b2 */
    if(mi_signed_less(f->flags))goto mi_004b62ca;
    /* 004b62b4 */
    MI_U32((0x00d056e0u))=(uint32_t)(0x00000000u);
    /* 004b62be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b62bf */
    f->r[4]+=4;return;
mi_004b62c0:;
    /* 004b62c0 */
    MI_U32((0x00bcd690u+f->r[0]))=(uint32_t)(0xfffffffeu);
mi_004b62ca:;
    /* 004b62ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b62cb */
    f->r[4]+=4;return;
}
