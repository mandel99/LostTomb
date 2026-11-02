/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "mission_record_actions.h"
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
static __inline uint32_t mi_alu(struct restore_mission_record_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_mission_record_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_mission_record_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


void restore_mission_record_C_00418bf0(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00418bf0 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00e38ea8u));
    /* 00418bf7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418bf8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418bf9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418bfa */
    f->r[1]=0x0000000au;
    /* 00418bff */
    f->r[6]=0x006fdcb8u;
    /* 00418c04 */
    f->r[7]=0x006fdce0u;
    /* 00418c09 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c0a */
    a=0x00000133u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c0f */
    while(f->r[1]){a=MI_U32(f->r[6]);MI_U32(f->r[7])=a;f->r[6]+=(f->flags&0x400u)?0xfffffffcu:4u;f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 00418c11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418c16u;f->boundary(0x00418c11u,0x004cc110u,f);
    /* 00418c16 */
    f->r[0]=MI_U32((0x00e20768u));
    /* 00418c1b */
    f->r[2]=MI_U32((0x006fdce0u));
    /* 00418c21 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00418c24 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00418c26 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000031u;t=mi_alu(f,a,b,8,1);
    /* 00418c29 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00418c2c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00418c2d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00418c2f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00418c31 */
    MI_U32((0x006fdce0u))=(uint32_t)(f->r[3]);
    /* 00418c37 */
    MI_U32((0x00e38eb4u))=(uint32_t)(f->r[1]);
    /* 00418c3d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[0])))&0xffu)<<0);
    /* 00418c40 */
    a=(f->r[0]&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,1);
    /* 00418c42 */
    if(!(f->flags&MI_ZF))goto mi_00418da7;
    /* 00418c48 */
    f->r[0]=MI_U32((0x00702798u));
    /* 00418c4d */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418c50 */
    if((f->flags&MI_ZF))goto mi_00418cb1;
    /* 00418c52 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0070279au)))&0xffffu)<<0);
    /* 00418c59 */
    a=(f->r[1]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418c5c */
    if((f->flags&MI_ZF))goto mi_00418cb1;
    /* 00418c5e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00418c61 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c62 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c63 */
    a=f->r[0];b=0x00001fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00418c68 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c69 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c6a */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00418c6f */
    a=0x000000deu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c74 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c75 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418c7au;f->boundary(0x00418c75u,0x0046a400u,f);
    /* 00418c7a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0070279au));
    /* 00418c81 */
    f->r[2]=MI_U32((0x00702798u));
    /* 00418c87 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c89 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c8b */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c8d */
    a=f->r[2];b=0x00001fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00418c93 */
    f->r[6]=f->r[0];
    /* 00418c95 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00418c9a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c9b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c9c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418c9d */
    a=0x000000deu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ca2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ca3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418ca8u;f->boundary(0x00418ca3u,0x00470990u,f);
    /* 00418ca8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ca9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418caeu;f->boundary(0x00418ca9u,0x004df2c0u,f);
    /* 00418cae */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00418cb1:;
    /* 00418cb1 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0070279cu)))&0xffffu)<<0);
    /* 00418cb8 */
    a=(f->r[1]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418cbb */
    if((f->flags&MI_ZF))goto mi_00418d24;
    /* 00418cbd */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0070279eu)))&0xffffu)<<0);
    /* 00418cc3 */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418cc6 */
    if((f->flags&MI_ZF))goto mi_00418d24;
    /* 00418cc8 */
    a=MI_U8((0x00e38ea8u));b=0x00000033u;t=mi_alu(f,a,b,8,1);
    /* 00418ccf */
    if(!(f->flags&MI_ZF))goto mi_00418d24;
    /* 00418cd1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00418cd4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00418cd7 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418cdd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418cde */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418cdf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ce0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ce1 */
    a=0x000000e1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ce6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ce7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418cecu;f->boundary(0x00418ce7u,0x0046a400u,f);
    /* 00418cec */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0070279eu));
    /* 00418cf3 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418cf9 */
    f->r[6]=f->r[0];
    /* 00418cfb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0070279cu));
    /* 00418d02 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d04 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d06 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d08 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d09 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d0a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d0b */
    a=0x000000e1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418d16u;f->boundary(0x00418d11u,0x00470990u,f);
    /* 00418d16 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00418d19 */
    f->r[1]=0x006fd5d8u;
    /* 00418d1e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418d24u;f->boundary(0x00418d1fu,0x0040d7f0u,f);
mi_00418d24:;
    /* 00418d24 */
    f->r[1]=MI_U32((0x007027a0u));
    /* 00418d2a */
    a=(f->r[1]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418d2d */
    if((f->flags&MI_ZF))goto mi_00418eff;
    /* 00418d33 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007027a2u)))&0xffffu)<<0);
    /* 00418d39 */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418d3c */
    if((f->flags&MI_ZF))goto mi_00418eff;
    /* 00418d42 */
    a=MI_U8((0x00e38ea8u));b=0x00000033u;t=mi_alu(f,a,b,8,1);
    /* 00418d49 */
    if(!(f->flags&MI_ZF))goto mi_00418eff;
    /* 00418d4f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00418d52 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00418d55 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418d5b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d5c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d5d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d5e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d5f */
    a=0x000000e4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d64 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d65 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418d6au;f->boundary(0x00418d65u,0x0046a400u,f);
    /* 00418d6a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007027a2u));
    /* 00418d71 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418d77 */
    f->r[6]=f->r[0];
    /* 00418d79 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007027a0u));
    /* 00418d80 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d82 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d84 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d86 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d88 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d89 */
    a=0x000000e4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d8e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d8f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418d94u;f->boundary(0x00418d8fu,0x00470990u,f);
    /* 00418d94 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00418d97 */
    f->r[1]=0x006de548u;
    /* 00418d9c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418d9d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418da2u;f->boundary(0x00418d9du,0x004e65a0u,f);
    /* 00418da2 */
    goto mi_00418eff;
mi_00418da7:;
    /* 00418da7 */
    a=(f->r[0]&0xffu);b=0x00000056u;t=mi_alu(f,a,b,8,1);
    /* 00418da9 */
    if(!(f->flags&MI_ZF))goto mi_00418eff;
    /* 00418daf */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0070278cu)))&0xffffu)<<0);
    /* 00418db6 */
    a=(f->r[1]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418db9 */
    if((f->flags&MI_ZF))goto mi_00418e19;
    /* 00418dbb */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0070278eu)))&0xffffu)<<0);
    /* 00418dc1 */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418dc4 */
    if((f->flags&MI_ZF))goto mi_00418e19;
    /* 00418dc6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00418dc9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00418dcc */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418dd2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dd3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dd4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dd5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dd6 */
    a=0x000000e5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ddb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ddc */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418de1u;f->boundary(0x00418ddcu,0x0046a400u,f);
    /* 00418de1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0070278eu));
    /* 00418de8 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418dee */
    f->r[6]=f->r[0];
    /* 00418df0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0070278cu));
    /* 00418df7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418df9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dfb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dfd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dfe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418dff */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e00 */
    a=0x000000e5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e05 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e06 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418e0bu;f->boundary(0x00418e06u,0x00470990u,f);
    /* 00418e0b */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00418e0e */
    f->r[1]=0x006f9320u;
    /* 00418e13 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418e19u;f->boundary(0x00418e14u,0x00547ad0u,f);
mi_00418e19:;
    /* 00418e19 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00702790u)))&0xffffu)<<0);
    /* 00418e20 */
    a=(f->r[1]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418e23 */
    if((f->flags&MI_ZF))goto mi_00418e8c;
    /* 00418e25 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00702792u)))&0xffffu)<<0);
    /* 00418e2b */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418e2e */
    if((f->flags&MI_ZF))goto mi_00418e8c;
    /* 00418e30 */
    a=MI_U8((0x00e38ea8u));b=0x00000027u;t=mi_alu(f,a,b,8,1);
    /* 00418e37 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00418e8c;
    /* 00418e39 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00418e3c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00418e3f */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418e45 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e46 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e47 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e48 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e49 */
    a=0x000000eau;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e4e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e4f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418e54u;f->boundary(0x00418e4fu,0x0046a400u,f);
    /* 00418e54 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00702792u));
    /* 00418e5b */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418e61 */
    f->r[6]=f->r[0];
    /* 00418e63 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00702790u));
    /* 00418e6a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e6c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e6e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e70 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e71 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e72 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e73 */
    a=0x000000eau;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e78 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e79 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418e7eu;f->boundary(0x00418e79u,0x00470990u,f);
    /* 00418e7e */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00418e81 */
    f->r[1]=0x006f5648u;
    /* 00418e86 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418e87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418e8cu;f->boundary(0x00418e87u,0x00547ad0u,f);
mi_00418e8c:;
    /* 00418e8c */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00702794u)))&0xffffu)<<0);
    /* 00418e93 */
    a=(f->r[1]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418e96 */
    if((f->flags&MI_ZF))goto mi_00418eff;
    /* 00418e98 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00702796u)))&0xffffu)<<0);
    /* 00418e9e */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00418ea1 */
    if((f->flags&MI_ZF))goto mi_00418eff;
    /* 00418ea3 */
    a=MI_U8((0x00e38ea8u));b=0x00000028u;t=mi_alu(f,a,b,8,1);
    /* 00418eaa */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00418eff;
    /* 00418eac */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00418eaf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00418eb2 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418eb8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418eb9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418eba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ebb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ebc */
    a=0x000000ebu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ec1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ec2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418ec7u;f->boundary(0x00418ec2u,0x0046a400u,f);
    /* 00418ec7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00702796u));
    /* 00418ece */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00418ed4 */
    f->r[6]=f->r[0];
    /* 00418ed6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00702794u));
    /* 00418edd */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418edf */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ee1 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ee3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ee4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ee5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418ee6 */
    a=0x000000ebu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418eeb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418eec */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418ef1u;f->boundary(0x00418eecu,0x00470990u,f);
    /* 00418ef1 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00418ef4 */
    f->r[1]=0x006de9e0u;
    /* 00418ef9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00418efa */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418effu;f->boundary(0x00418efau,0x00547ad0u,f);
mi_00418eff:;
    /* 00418eff */
    f->r[2]=MI_U32((0x00e20768u));
    /* 00418f05 */
    a=MI_U8((0x00000002u+f->r[2]));b=0x00000031u;t=mi_alu(f,a,b,8,1);
    /* 00418f09 */
    if((f->flags&MI_ZF))goto mi_00418f53;
    /* 00418f0b */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 00418f0e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x000000ffu)&0xffu)<<0);
    /* 00418f10 */
    MI_U32((0x006fdce8u))=(uint32_t)(f->r[1]);
    /* 00418f16 */
    MI_U8((0x006fdcecu))=(uint8_t)((f->r[3]&0xffu));
    /* 00418f1c */
    MI_U8((0x006fdce4u))=(uint8_t)((f->r[0]&0xffu));
    /* 00418f21 */
    MI_U32((0x006fdcf4u))=(uint32_t)(f->r[1]);
    /* 00418f27 */
    MI_U8((0x006fdcf8u))=(uint8_t)((f->r[3]&0xffu));
    /* 00418f2d */
    MI_U8((0x006fdcf0u))=(uint8_t)((f->r[0]&0xffu));
    /* 00418f32 */
    MI_U32((0x006fdd00u))=(uint32_t)(f->r[1]);
    /* 00418f38 */
    MI_U8((0x006fdd04u))=(uint8_t)((f->r[3]&0xffu));
    /* 00418f3e */
    MI_U8((0x006fdcfcu))=(uint8_t)((f->r[0]&0xffu));
    /* 00418f43 */
    MI_U8((0x006fdcb4u))=(uint8_t)(0x00000001u);
    /* 00418f4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418f4fu;f->boundary(0x00418f4au,0x004dea50u,f);
    /* 00418f4f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00418f50 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00418f51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00418f52 */
    f->r[4]+=4;return;
mi_00418f53:;
    /* 00418f53 */
    MI_U8((0x006fdcb4u))=(uint8_t)((f->r[3]&0xffu));
    /* 00418f59 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00418f5eu;f->boundary(0x00418f59u,0x004dea50u,f);
    /* 00418f5e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00418f5f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00418f60 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00418f61 */
    f->r[4]+=4;return;
}

void restore_mission_record_C_004e1770(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e1770 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1771 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1772 */
    f->r[0]=0x00e95238u;
    /* 004e1777 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004e1779:;
    /* 004e1779 */
    MI_U16((0xfffffffau+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 004e177d */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)(0x0000ffffu);
    /* 004e1782 */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e1785 */
    a=f->r[0];b=0x00ea0db8u;t=mi_alu(f,a,b,32,1);
    /* 004e178a */
    if(mi_signed_less(f->flags))goto mi_004e1779;
    /* 004e178c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e178e */
    f->r[1]=0x00000014u;
    /* 004e1793 */
    MI_U32((0x00e951c4u))=(uint32_t)(f->r[0]);
    /* 004e1798 */
    f->r[7]=0x00e95174u;
    /* 004e179d */
    MI_U32((0x00e951c8u))=(uint32_t)(f->r[0]);
    /* 004e17a2 */
    MI_U32((0x00ea0da8u))=(uint32_t)(f->r[6]);
    /* 004e17a8 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 004e17aa */
    f->r[1]=0x00000014u;
    /* 004e17af */
    f->r[7]=0x00e95124u;
    /* 004e17b4 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 004e17b6 */
    f->r[1]=0x00000014u;
    /* 004e17bb */
    f->r[7]=0x00e95080u;
    /* 004e17c0 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 004e17c2 */
    MI_U16((0x00e951ccu))=(uint16_t)((f->r[0]&0xffffu));
    /* 004e17c8 */
    MI_U32((0x00e951f0u))=(uint32_t)(f->r[6]);
    /* 004e17ce */
    MI_U32((0x00e951ecu))=(uint32_t)(f->r[6]);
    /* 004e17d4 */
    MI_U32((0x00e951e8u))=(uint32_t)(0xffffffffu);
    /* 004e17de */
    MI_U32((0x00e951e4u))=(uint32_t)(f->r[6]);
    /* 004e17e4 */
    MI_U32((0x00e9511cu))=(uint32_t)(f->r[6]);
    /* 004e17ea */
    MI_U32((0x00e95120u))=(uint32_t)(f->r[6]);
    /* 004e17f0 */
    MI_U32((0x00e950e0u))=(uint32_t)(0x00000001u);
    /* 004e17fa */
    MI_U32((0x00e950f0u))=(uint32_t)(f->r[6]);
    /* 004e1800 */
    MI_U32((0x00e950ecu))=(uint32_t)(f->r[6]);
    /* 004e1806 */
    MI_U32((0x00e950e8u))=(uint32_t)(f->r[6]);
    /* 004e180c */
    MI_U32((0x00e950e4u))=(uint32_t)(f->r[6]);
    /* 004e1812 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1817u;f->boundary(0x004e1812u,0x004e1830u,f);
    /* 004e1817 */
    MI_U32((0x00e950d4u))=(uint32_t)(f->r[6]);
    /* 004e181d */
    MI_U32((0x00e950d0u))=(uint32_t)(f->r[6]);
    /* 004e1823 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e1824 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e1825 */
    f->r[4]+=4;return;
}

void restore_mission_record_C_004e1830(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e1830 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e1832 */
    MI_U32((0x00e951d8u))=(uint32_t)(f->r[0]);
    /* 004e1837 */
    MI_U32((0x00e951d4u))=(uint32_t)(f->r[0]);
    /* 004e183c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1842u;f->boundary(0x004e183cu,0x0056f22cu,f);
    /* 004e1842 */
    a=f->r[0];b=0x00002710u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e1847 */
    MI_U32((0x00e951d0u))=(uint32_t)(f->r[0]);
    /* 004e184c */
    f->r[4]+=4;return;
}

void restore_mission_record_C_004df2c0(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004df2c0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004df2c4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df2c5 */
    f->r[1]=f->r[0];
    /* 004df2c7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df2c8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004df2cb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004df2cd */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]*8u));
    /* 004df2d5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004df2d7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004df2d9 */
    f->r[1]=f->r[0];
    /* 004df2db */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004df2de */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004df2e0 */
    f->r[0]=(0x00799a30u+f->r[1]*8u);
    /* 004df2e7 */
    f->r[1]=0x005e2ae4u;
mi_004df2ec:;
    /* 004df2ec */
    f->r[5]=MI_U32((0x00000000u+f->r[1]));
    /* 004df2ee */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004df2f0 */
    a=f->r[5];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004df2f6 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004df2f9 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004df2fc */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004df2fd */
    a=f->r[1];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004df300 */
    a=f->r[3];b=0xffff0003u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004df306 */
    a=f->r[0];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004df30b */
    a=f->r[3];b=0x0000ffffu;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004df311 */
    a=f->r[1];b=0x005e4274u;t=mi_alu(f,a,b,32,1);
    /* 004df317 */
    MI_U32((0xfffffef8u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004df31d */
    MI_U32((0xffffff04u+f->r[0]))=(uint32_t)(0x000000ffu);
    /* 004df327 */
    MI_U32((0xffffff00u+f->r[0]))=(uint32_t)(0x00000003u);
    /* 004df331 */
    MI_U8((0xffffff14u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004df337 */
    MI_U16((0xfffffeb6u+f->r[0]))=(uint16_t)(0x0000ffffu);
    /* 004df340 */
    if(mi_signed_less(f->flags))goto mi_004df2ec;
    /* 004df342 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004df346 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004df347 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004df34cu;f->boundary(0x004df347u,0x004e0c80u,f);
    /* 004df34c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004df34f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004df350 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004df351 */
    f->r[4]+=4;return;
}

void restore_mission_record_C_0040d7f0(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040d7f0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d7f1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d7f2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d7f3 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0040d7f7 */
    f->r[5]=f->r[1];
    /* 0040d7f9 */
    f->r[3]=(0x00000019u+f->r[7]);
    /* 0040d7fc */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0040d7fe */
    if(!mi_signed_less(f->flags))goto mi_0040d83b;
    /* 0040d800 */
    f->r[0]=f->r[7];
    /* 0040d802 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d803 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040d806 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040d808 */
    f->r[6]=(0x00799a38u+f->r[0]*8u);
mi_0040d80f:;
    /* 0040d80f */
    MI_U32((0xfffffff8u+f->r[6]))=(uint32_t)(0x00000015u);
    /* 0040d816 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(0x00000001u);
    /* 0040d81c */
    MI_U8((0x00000008u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0040d820 */
    MI_U16((0xffffffb6u+f->r[6]))=(uint16_t)(0x0000ffffu);
    /* 0040d826 */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 0040d829 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040d82a */
    f->r[1]=f->r[5];
    /* 0040d82c */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x0040d82fu;f->boundary(0x0040d82cu,p,f);
    /* 0040d82f */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0040d830 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040d836 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0040d838 */
    if(mi_signed_less(f->flags))goto mi_0040d80f;
    /* 0040d83a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0040d83b:;
    /* 0040d83b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040d83c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040d83d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040d83e */
    f->r[4]+=8;return;
}

void restore_mission_record_C_004e65a0(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e65a0 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004e65a4 */
    f->r[0]=f->r[2];
    /* 004e65a6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e65a7 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e65aa */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e65ac */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e65af */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(0x00000009u);
    /* 004e65b9 */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(0x00000001u);
    /* 004e65c3 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(0x00000003u);
    /* 004e65cd */
    MI_U8((0x00799a40u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 004e65d4 */
    MI_U16((0x007999eeu+f->r[0]))=(uint16_t)(0x0000ffffu);
    /* 004e65dd */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 004e65df */
    p=MI_U32((0x00000018u+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x004e65e2u;f->boundary(0x004e65dfu,p,f);
    /* 004e65e2 */
    f->r[4]+=8;return;
}

void restore_mission_record_C_00547ad0(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00547ad0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00547ad4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547ad5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547ad6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547ad7 */
    f->r[7]=f->r[1];
    /* 00547ad9 */
    f->r[1]=f->r[0];
    /* 00547adb */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00547ade */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547ae0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]*8u));
    /* 00547ae8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00547aea */
    f->r[1]=f->r[7];
    /* 00547aec */
    f->r[6]=f->r[0];
    /* 00547aee */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 00547af0 */
    p=MI_U32((0x0000003cu+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x00547af3u;f->boundary(0x00547af0u,p,f);
    /* 00547af3 */
    f->r[1]=f->r[6];
    /* 00547af5 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[6]*1u);
    /* 00547af8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00547afb */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547afd */
    f->r[2]=0x00000001u;
    /* 00547b02 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00547b05 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00547b07 */
    MI_U32((0x00799a70u+f->r[1]))=(uint32_t)(0x00000190u);
    /* 00547b11 */
    MI_U32((0x00799a30u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00547b17 */
    if(!mi_signed_less(f->flags))goto mi_00547b51;
    /* 00547b19 */
    f->r[0]=(0x00799a38u+f->r[1]);
    /* 00547b1f */
    f->r[1]=f->r[3];
    /* 00547b21 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[1]=t;
mi_00547b23:;
    /* 00547b23 */
    a=MI_U32((0x00000008u+f->r[0]));b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 00547b27 */
    if((f->flags&MI_ZF))goto mi_00547b2f;
    /* 00547b29 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000004u);
mi_00547b2f:;
    /* 00547b2f */
    MI_U16((0xffffffb6u+f->r[0]))=(uint16_t)(0x0000ffffu);
    /* 00547b35 */
    MI_U8((0x0000003du+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00547b38 */
    a=f->r[0];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00547b3d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00547b3e */
    if(!(f->flags&MI_ZF))goto mi_00547b23;
    /* 00547b40 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00547b42 */
    if(!mi_signed_less(f->flags))goto mi_00547b51;
mi_00547b44:;
    /* 00547b44 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 00547b46 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00547b47 */
    f->r[1]=f->r[7];
    /* 00547b49 */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00547b4cu;f->boundary(0x00547b49u,p,f);
    /* 00547b4c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00547b4d */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00547b4f */
    if(mi_signed_less(f->flags))goto mi_00547b44;
mi_00547b51:;
    /* 00547b51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00547b52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547b53 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00547b54 */
    f->r[4]+=8;return;
}

void restore_mission_record_C_00545f70(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00545f70 */
    f->r[0]=0x000000a7u;
    /* 00545f75 */
    f->r[4]+=4;return;
}

void restore_mission_record_C_00559470(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00559470 */
    f->r[0]=0x0000009eu;
    /* 00559475 */
    f->r[4]+=4;return;
}

void restore_mission_record_C_005567c0(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005567c0 */
    f->r[0]=0x000001b5u;
    /* 005567c5 */
    f->r[4]+=4;return;
}

void restore_mission_record_C_00481f20(struct restore_mission_record_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00481f20 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00481f21 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00481f22 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00481f23 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00481f24 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00481f25 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00481f2au;f->boundary(0x00481f25u,0x0046f8d0u,f);
    /* 00481f2a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00481f2c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00481f2e:;
    /* 00481f2e */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x005c9b00u+f->r[6]*2u));
    /* 00481f36 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00481f38 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00481f3a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048208b;
    /* 00481f40 */
    f->r[7]=(0x0070c8ecu+f->r[1]*2u);
    /* 00481f47 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00481f49 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00481f4d:;
    /* 00481f4d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00481f4f */
    if(!(f->flags&MI_ZF))goto mi_00481f66;
    /* 00481f51 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bf7cu+f->r[0]*2u));
    /* 00481f59 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bf7du+f->r[0]*2u));
    /* 00481f61 */
    goto mi_0048206b;
mi_00481f66:;
    /* 00481f66 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00481f69 */
    if(!(f->flags&MI_ZF))goto mi_00481f80;
    /* 00481f6b */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bf5cu+f->r[0]*2u));
    /* 00481f73 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bf5du+f->r[0]*2u));
    /* 00481f7b */
    goto mi_0048206b;
mi_00481f80:;
    /* 00481f80 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00481f83 */
    if(!(f->flags&MI_ZF))goto mi_00481f9a;
    /* 00481f85 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bf2cu+f->r[0]*2u));
    /* 00481f8d */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bf2du+f->r[0]*2u));
    /* 00481f95 */
    goto mi_0048206b;
mi_00481f9a:;
    /* 00481f9a */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00481f9d */
    if(!(f->flags&MI_ZF))goto mi_00481fb4;
    /* 00481f9f */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070beecu+f->r[0]*2u));
    /* 00481fa7 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070beedu+f->r[0]*2u));
    /* 00481faf */
    goto mi_0048206b;
mi_00481fb4:;
    /* 00481fb4 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00481fb7 */
    if(!(f->flags&MI_ZF))goto mi_00481fce;
    /* 00481fb9 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070be9cu+f->r[0]*2u));
    /* 00481fc1 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070be9du+f->r[0]*2u));
    /* 00481fc9 */
    goto mi_0048206b;
mi_00481fce:;
    /* 00481fce */
    a=f->r[6];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00481fd1 */
    if(!(f->flags&MI_ZF))goto mi_00481fe8;
    /* 00481fd3 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070be3cu+f->r[0]*2u));
    /* 00481fdb */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070be3du+f->r[0]*2u));
    /* 00481fe3 */
    goto mi_0048206b;
mi_00481fe8:;
    /* 00481fe8 */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00481feb */
    if(!(f->flags&MI_ZF))goto mi_00481fff;
    /* 00481fed */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bdccu+f->r[0]*2u));
    /* 00481ff5 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bdcdu+f->r[0]*2u));
    /* 00481ffd */
    goto mi_0048206b;
mi_00481fff:;
    /* 00481fff */
    a=f->r[6];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 00482002 */
    if(!(f->flags&MI_ZF))goto mi_00482016;
    /* 00482004 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bd4cu+f->r[0]*2u));
    /* 0048200c */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bd4du+f->r[0]*2u));
    /* 00482014 */
    goto mi_0048206b;
mi_00482016:;
    /* 00482016 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00482019 */
    if(!(f->flags&MI_ZF))goto mi_0048202d;
    /* 0048201b */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bcbcu+f->r[0]*2u));
    /* 00482023 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bcbdu+f->r[0]*2u));
    /* 0048202b */
    goto mi_0048206b;
mi_0048202d:;
    /* 0048202d */
    a=f->r[6];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 00482030 */
    if(!(f->flags&MI_ZF))goto mi_00482044;
    /* 00482032 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bc1cu+f->r[0]*2u));
    /* 0048203a */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bc1du+f->r[0]*2u));
    /* 00482042 */
    goto mi_0048206b;
mi_00482044:;
    /* 00482044 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00482047 */
    if(!(f->flags&MI_ZF))goto mi_0048205b;
    /* 00482049 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bb6cu+f->r[0]*2u));
    /* 00482051 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070bb6du+f->r[0]*2u));
    /* 00482059 */
    goto mi_0048206b;
mi_0048205b:;
    /* 0048205b */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070baacu+f->r[0]*2u));
    /* 00482063 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x0070baadu+f->r[0]*2u));
mi_0048206b:;
    /* 0048206b */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0048206e */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00482071 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00482072 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 00482075 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00482077 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048207a */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0048207d */
    MI_U16((0xfffffffeu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00482081 */
    if(mi_signed_less(f->flags))goto mi_00481f4d;
    /* 00482087 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_0048208b:;
    /* 0048208b */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0048208c */
    a=f->r[6];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0048208f */
    if(mi_signed_less(f->flags))goto mi_00481f2e;
    /* 00482095 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00482097 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00482099:;
    /* 00482099 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x005c9b18u+f->r[6]*2u));
    /* 004820a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004820a3 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004820a5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048215a;
    /* 004820ab */
    f->r[7]=(0x0070c76cu+f->r[1]*2u);
    /* 004820b2 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004820b4 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004820b8:;
    /* 004820b8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004820ba */
    if(!(f->flags&MI_ZF))goto mi_004820ce;
    /* 004820bc */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9ba8u+f->r[0]*2u));
    /* 004820c4 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9ba9u+f->r[0]*2u));
    /* 004820cc */
    goto mi_0048213a;
mi_004820ce:;
    /* 004820ce */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004820d1 */
    if(!(f->flags&MI_ZF))goto mi_004820e5;
    /* 004820d3 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9bc0u+f->r[0]*2u));
    /* 004820db */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9bc1u+f->r[0]*2u));
    /* 004820e3 */
    goto mi_0048213a;
mi_004820e5:;
    /* 004820e5 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004820e8 */
    if(!(f->flags&MI_ZF))goto mi_004820fc;
    /* 004820ea */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9be8u+f->r[0]*2u));
    /* 004820f2 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9be9u+f->r[0]*2u));
    /* 004820fa */
    goto mi_0048213a;
mi_004820fc:;
    /* 004820fc */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004820ff */
    if(!(f->flags&MI_ZF))goto mi_00482113;
    /* 00482101 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9c20u+f->r[0]*2u));
    /* 00482109 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9c21u+f->r[0]*2u));
    /* 00482111 */
    goto mi_0048213a;
mi_00482113:;
    /* 00482113 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00482116 */
    if(!(f->flags&MI_ZF))goto mi_0048212a;
    /* 00482118 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9c68u+f->r[0]*2u));
    /* 00482120 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9c69u+f->r[0]*2u));
    /* 00482128 */
    goto mi_0048213a;
mi_0048212a:;
    /* 0048212a */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9cc0u+f->r[0]*2u));
    /* 00482132 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9cc1u+f->r[0]*2u));
mi_0048213a:;
    /* 0048213a */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0048213d */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00482140 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00482141 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 00482144 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00482146 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00482149 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0048214c */
    MI_U16((0xfffffffeu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00482150 */
    if(mi_signed_less(f->flags))goto mi_004820b8;
    /* 00482156 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_0048215a:;
    /* 0048215a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0048215b */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0048215e */
    if(mi_signed_less(f->flags))goto mi_00482099;
    /* 00482164 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00482166 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00482168:;
    /* 00482168 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x005c9b24u+f->r[6]*2u));
    /* 00482170 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00482172 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00482174 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482229;
    /* 0048217a */
    f->r[7]=(0x0070c5bcu+f->r[1]*2u);
    /* 00482181 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482183 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00482187:;
    /* 00482187 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00482189 */
    if(!(f->flags&MI_ZF))goto mi_0048219d;
    /* 0048218b */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9d28u+f->r[0]*2u));
    /* 00482193 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9d29u+f->r[0]*2u));
    /* 0048219b */
    goto mi_00482209;
mi_0048219d:;
    /* 0048219d */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004821a0 */
    if(!(f->flags&MI_ZF))goto mi_004821b4;
    /* 004821a2 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9d48u+f->r[0]*2u));
    /* 004821aa */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9d49u+f->r[0]*2u));
    /* 004821b2 */
    goto mi_00482209;
mi_004821b4:;
    /* 004821b4 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004821b7 */
    if(!(f->flags&MI_ZF))goto mi_004821cb;
    /* 004821b9 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9d78u+f->r[0]*2u));
    /* 004821c1 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9d79u+f->r[0]*2u));
    /* 004821c9 */
    goto mi_00482209;
mi_004821cb:;
    /* 004821cb */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004821ce */
    if(!(f->flags&MI_ZF))goto mi_004821e2;
    /* 004821d0 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9db8u+f->r[0]*2u));
    /* 004821d8 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9db9u+f->r[0]*2u));
    /* 004821e0 */
    goto mi_00482209;
mi_004821e2:;
    /* 004821e2 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004821e5 */
    if(!(f->flags&MI_ZF))goto mi_004821f9;
    /* 004821e7 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9e08u+f->r[0]*2u));
    /* 004821ef */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9e09u+f->r[0]*2u));
    /* 004821f7 */
    goto mi_00482209;
mi_004821f9:;
    /* 004821f9 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9e68u+f->r[0]*2u));
    /* 00482201 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9e69u+f->r[0]*2u));
mi_00482209:;
    /* 00482209 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0048220c */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0048220f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00482210 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 00482213 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00482215 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00482218 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0048221b */
    MI_U16((0xfffffffeu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048221f */
    if(mi_signed_less(f->flags))goto mi_00482187;
    /* 00482225 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_00482229:;
    /* 00482229 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0048222a */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0048222d */
    if(mi_signed_less(f->flags))goto mi_00482168;
    /* 00482233 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00482235 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00482237:;
    /* 00482237 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x005c9b30u+f->r[6]*2u));
    /* 0048223f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00482241 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00482243 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004822f8;
    /* 00482249 */
    f->r[7]=(0x0070c3dcu+f->r[1]*2u);
    /* 00482250 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482252 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00482256:;
    /* 00482256 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00482258 */
    if(!(f->flags&MI_ZF))goto mi_0048226c;
    /* 0048225a */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9ed8u+f->r[0]*2u));
    /* 00482262 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9ed9u+f->r[0]*2u));
    /* 0048226a */
    goto mi_004822d8;
mi_0048226c:;
    /* 0048226c */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0048226f */
    if(!(f->flags&MI_ZF))goto mi_00482283;
    /* 00482271 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9f00u+f->r[0]*2u));
    /* 00482279 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9f01u+f->r[0]*2u));
    /* 00482281 */
    goto mi_004822d8;
mi_00482283:;
    /* 00482283 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00482286 */
    if(!(f->flags&MI_ZF))goto mi_0048229a;
    /* 00482288 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9f38u+f->r[0]*2u));
    /* 00482290 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9f39u+f->r[0]*2u));
    /* 00482298 */
    goto mi_004822d8;
mi_0048229a:;
    /* 0048229a */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0048229d */
    if(!(f->flags&MI_ZF))goto mi_004822b1;
    /* 0048229f */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9f80u+f->r[0]*2u));
    /* 004822a7 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9f81u+f->r[0]*2u));
    /* 004822af */
    goto mi_004822d8;
mi_004822b1:;
    /* 004822b1 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004822b4 */
    if(!(f->flags&MI_ZF))goto mi_004822c8;
    /* 004822b6 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9fd8u+f->r[0]*2u));
    /* 004822be */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005c9fd9u+f->r[0]*2u));
    /* 004822c6 */
    goto mi_004822d8;
mi_004822c8:;
    /* 004822c8 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca040u+f->r[0]*2u));
    /* 004822d0 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca041u+f->r[0]*2u));
mi_004822d8:;
    /* 004822d8 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004822db */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004822de */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004822df */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 004822e2 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004822e4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004822e7 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 004822ea */
    MI_U16((0xfffffffeu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004822ee */
    if(mi_signed_less(f->flags))goto mi_00482256;
    /* 004822f4 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_004822f8:;
    /* 004822f8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004822f9 */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004822fc */
    if(mi_signed_less(f->flags))goto mi_00482237;
    /* 00482302 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00482304 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00482306:;
    /* 00482306 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x005c9b3cu+f->r[6]*2u));
    /* 0048230e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00482310 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00482312 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004823c7;
    /* 00482318 */
    f->r[7]=(0x0070c1ccu+f->r[1]*2u);
    /* 0048231f */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482321 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00482325:;
    /* 00482325 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00482327 */
    if(!(f->flags&MI_ZF))goto mi_0048233b;
    /* 00482329 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca0b8u+f->r[0]*2u));
    /* 00482331 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca0b9u+f->r[0]*2u));
    /* 00482339 */
    goto mi_004823a7;
mi_0048233b:;
    /* 0048233b */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0048233e */
    if(!(f->flags&MI_ZF))goto mi_00482352;
    /* 00482340 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca0e8u+f->r[0]*2u));
    /* 00482348 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca0e9u+f->r[0]*2u));
    /* 00482350 */
    goto mi_004823a7;
mi_00482352:;
    /* 00482352 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00482355 */
    if(!(f->flags&MI_ZF))goto mi_00482369;
    /* 00482357 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca128u+f->r[0]*2u));
    /* 0048235f */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca129u+f->r[0]*2u));
    /* 00482367 */
    goto mi_004823a7;
mi_00482369:;
    /* 00482369 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0048236c */
    if(!(f->flags&MI_ZF))goto mi_00482380;
    /* 0048236e */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca178u+f->r[0]*2u));
    /* 00482376 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca179u+f->r[0]*2u));
    /* 0048237e */
    goto mi_004823a7;
mi_00482380:;
    /* 00482380 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00482383 */
    if(!(f->flags&MI_ZF))goto mi_00482397;
    /* 00482385 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca1d8u+f->r[0]*2u));
    /* 0048238d */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca1d9u+f->r[0]*2u));
    /* 00482395 */
    goto mi_004823a7;
mi_00482397:;
    /* 00482397 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca248u+f->r[0]*2u));
    /* 0048239f */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca249u+f->r[0]*2u));
mi_004823a7:;
    /* 004823a7 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004823aa */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004823ad */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004823ae */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 004823b1 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004823b3 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004823b6 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 004823b9 */
    MI_U16((0xfffffffeu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004823bd */
    if(mi_signed_less(f->flags))goto mi_00482325;
    /* 004823c3 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_004823c7:;
    /* 004823c7 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004823c8 */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004823cb */
    if(mi_signed_less(f->flags))goto mi_00482306;
    /* 004823d1 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004823d3 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004823d5:;
    /* 004823d5 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x005c9b48u+f->r[6]*2u));
    /* 004823dd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004823df */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004823e1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482496;
    /* 004823e7 */
    f->r[7]=(0x0070bf8cu+f->r[1]*2u);
    /* 004823ee */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004823f0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004823f4:;
    /* 004823f4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004823f6 */
    if(!(f->flags&MI_ZF))goto mi_0048240a;
    /* 004823f8 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca2c8u+f->r[0]*2u));
    /* 00482400 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca2c9u+f->r[0]*2u));
    /* 00482408 */
    goto mi_00482476;
mi_0048240a:;
    /* 0048240a */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0048240d */
    if(!(f->flags&MI_ZF))goto mi_00482421;
    /* 0048240f */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca300u+f->r[0]*2u));
    /* 00482417 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca301u+f->r[0]*2u));
    /* 0048241f */
    goto mi_00482476;
mi_00482421:;
    /* 00482421 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00482424 */
    if(!(f->flags&MI_ZF))goto mi_00482438;
    /* 00482426 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca348u+f->r[0]*2u));
    /* 0048242e */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca349u+f->r[0]*2u));
    /* 00482436 */
    goto mi_00482476;
mi_00482438:;
    /* 00482438 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0048243b */
    if(!(f->flags&MI_ZF))goto mi_0048244f;
    /* 0048243d */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca3a0u+f->r[0]*2u));
    /* 00482445 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca3a1u+f->r[0]*2u));
    /* 0048244d */
    goto mi_00482476;
mi_0048244f:;
    /* 0048244f */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00482452 */
    if(!(f->flags&MI_ZF))goto mi_00482466;
    /* 00482454 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca408u+f->r[0]*2u));
    /* 0048245c */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca409u+f->r[0]*2u));
    /* 00482464 */
    goto mi_00482476;
mi_00482466:;
    /* 00482466 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca480u+f->r[0]*2u));
    /* 0048246e */
    f->r[1]=(uint32_t)(int32_t)(int8_t)MI_U8((0x005ca481u+f->r[0]*2u));
mi_00482476:;
    /* 00482476 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00482479 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0048247c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048247d */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 00482480 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00482482 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00482485 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00482488 */
    MI_U16((0xfffffffeu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048248c */
    if(mi_signed_less(f->flags))goto mi_004823f4;
    /* 00482492 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_00482496:;
    /* 00482496 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00482497 */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0048249a */
    if(mi_signed_less(f->flags))goto mi_004823d5;
    /* 004824a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004824a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004824a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004824a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004824a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004824a5 */
    f->r[4]+=4;return;
}
