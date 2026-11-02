/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "small_constructor_model.h"

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

void restore_C_00440ed0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00440ed0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00440ed5 */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 00440ed8 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00440ede */
    a=MI_U32((0x00ea6010u+f->r[0]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00440ee4 */
    if((f->flags&MI_ZF))goto mi_00440ef1;
    /* 00440ee6 */
    a=MI_U32((0x00ea6014u+f->r[0]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00440eec */
    if((f->flags&MI_ZF))goto mi_00440ef1;
    /* 00440eee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00440ef0 */
    f->r[4]+=4;return;
mi_00440ef1:;
    /* 00440ef1 */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ef6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440efbu;f->boundary(0x00440ef6u,0x0043f410u,f);
    /* 00440efb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440efe */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440f00 */
    if(!(f->flags&MI_ZF))goto mi_00440f03;
    /* 00440f02 */
    f->r[4]+=4;return;
mi_00440f03:;
    /* 00440f03 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00440f07 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440f09 */
    if((f->flags&MI_ZF))goto mi_00440f11;
    /* 00440f0b */
    f->r[0]=0x00000001u;
    /* 00440f10 */
    f->r[4]+=4;return;
mi_00440f11:;
    /* 00440f11 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f12 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f13 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f14 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f19 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f1e */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440f28u;f->boundary(0x00440f23u,0x0040f860u,f);
    /* 00440f28 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f2d */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f32 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f37 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440f3cu;f->boundary(0x00440f37u,0x0040f860u,f);
    /* 00440f3c */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f41 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f46 */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f4b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440f50u;f->boundary(0x00440f4bu,0x0040f860u,f);
    /* 00440f50 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f55 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f5a */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440f64u;f->boundary(0x00440f5fu,0x0040f860u,f);
    /* 00440f64 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f66 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440f6bu;f->boundary(0x00440f66u,0x00485830u,f);
    /* 00440f6b */
    f->r[3]=MI_U32((0x00000050u+f->r[4]));
    /* 00440f6f */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 00440f74 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440f77 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00440f79 */
    if(mi_signed_less(f->flags))goto mi_00440f7e;
    /* 00440f7b */
    f->r[3]=(0xffffffffu+f->r[0]);
mi_00440f7e:;
    /* 00440f7e */
    f->r[6]=MI_U32((0x00000020u+f->r[4]));
    /* 00440f82 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 00440f87 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00440f89 */
    if(mi_signed_less(f->flags))goto mi_00440f8e;
    /* 00440f8b */
    f->r[6]=(0xffffffffu+f->r[0]);
mi_00440f8e:;
    /* 00440f8e */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 00440f91 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00440f97 */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f9c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440f9d */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 00440fa0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fa1 */
    MI_U32((0x00bc971cu))=(uint32_t)(0x00000001u);
    /* 00440fab */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00440fae */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00440fb3 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00440fb9 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00440fbc */
    MI_U32((0x00ea6010u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00440fc2 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00440fc4 */
    MI_U32((0x00ea6014u+f->r[0]))=(uint32_t)(f->r[6]);
    /* 00440fca */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440fcfu;f->boundary(0x00440fcau,0x004413b0u,f);
    /* 00440fcf */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00440fd4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fd6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fd8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fd9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fda */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fdf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fe0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440fe5u;f->boundary(0x00440fe0u,0x0046a400u,f);
    /* 00440fe5 */
    f->r[6]=f->r[0];
    /* 00440fe7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440fe8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440fedu;f->boundary(0x00440fe8u,0x00485100u,f);
    /* 00440fed */
    f->r[0]=f->r[6];
    /* 00440fef */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ff0 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00440ff3 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00440ff5 */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ffa */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00440ffd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ffe */
    MI_U32((0x00799aa8u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 00441004 */
    MI_U32((0x00799ab0u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0044100e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00441013u;f->boundary(0x0044100eu,0x00440060u,f);
    /* 00441013 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00441019 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044101c */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00441022 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00441023 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00441024 */
    MI_U16((0x00ea6084u+f->r[1]))=(uint16_t)(0x00000001u);
    /* 0044102d */
    f->r[0]=0x00000001u;
    /* 00441032 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00441033 */
    f->r[4]+=4;return;
}
