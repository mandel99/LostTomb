/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "road_paint_model.h"

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

void restore_C_0047b310(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047b310 */
    f->r[0]=MI_U32((0x00bca2ecu));
    /* 0047b315 */
    f->r[1]=MI_U32((0x00bca2e8u));
    /* 0047b31b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b31c */
    f->r[6]=MI_U32((0x0000000cu+f->r[4]));
    /* 0047b320 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b321 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 0047b325 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047b327 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047b329 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b32a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b32b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b330u;f->boundary(0x0047b32bu,0x0046f930u,f);
    /* 0047b330 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047b333 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047b335 */
    if((f->flags&MI_ZF))goto mi_0047b47d;
    /* 0047b33b */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0047b33e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b33f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b340 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[2]*2u);
    /* 0047b343 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0047b349 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047b34c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0047b34f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047b351 */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0047b358 */
    a=(f->r[1]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0047b35b */
    if(!(f->flags&MI_ZF))goto mi_0047b363;
    /* 0047b35d */
    a=MI_U32((0x00bc971cu));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00bc971cu))=(uint32_t)(t);
mi_0047b363:;
    /* 0047b363 */
    a=MI_U32((0x0000001cu+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047b368 */
    if(!(f->flags&MI_ZF))goto mi_0047b36f;
    /* 0047b36a */
    a=f->r[1];b=0x00000040u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 0047b36d */
    goto mi_0047b372;
mi_0047b36f:;
    /* 0047b36f */
    a=f->r[1];b=0xffffffbfu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
mi_0047b372:;
    /* 0047b372 */
    MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0047b379 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 0047b37f */
    a=(f->r[1]&0xffu);b=0x000000efu;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047b382 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0047b383 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047b389 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047b38a */
    f->r[1]=0x00000003u;
    /* 0047b38f */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047b391 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047b395 */
    if(!mi_signed_less(f->flags))goto mi_0047b3a1;
    /* 0047b397 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0047b399 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0047b39d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0047b39f */
    goto mi_0047b3b3;
mi_0047b3a1:;
    /* 0047b3a1 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 0047b3a6 */
    f->r[3]=(0x00000003u+f->r[7]);
    /* 0047b3a9 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047b3ab */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047b3b3;
    /* 0047b3ad */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047b3af */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0047b3b3:;
    /* 0047b3b3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047b3b5 */
    if(!mi_signed_less(f->flags))goto mi_0047b3be;
    /* 0047b3b7 */
    f->r[1]=(0x00000003u+f->r[6]);
    /* 0047b3ba */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0047b3bc */
    goto mi_0047b3ce;
mi_0047b3be:;
    /* 0047b3be */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0047b3c3 */
    f->r[3]=(0x00000003u+f->r[6]);
    /* 0047b3c6 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047b3c8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047b3ce;
    /* 0047b3ca */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047b3cc */
    f->r[1]=f->r[0];
mi_0047b3ce:;
    /* 0047b3ce */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0047b3d1 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0047b3d4 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[6]);
    /* 0047b3da */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 0047b3dd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047b3e0 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0047b3e3 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047b3e5 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047b3e7 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047b3ec */
    if(!mi_signed_less(f->flags))goto mi_0047b46c;
    /* 0047b3ee */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0047b3f2 */
    f->r[3]=(0x00000000u+f->r[1]+f->r[7]*1u);
mi_0047b3f5:;
    /* 0047b3f5 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047b3f7 */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[7]);
    /* 0047b3fd */
    if(!mi_signed_less(f->flags))goto mi_0047b443;
mi_0047b3ff:;
    /* 0047b3ff */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0047b406 */
    a=(f->r[1]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0047b409 */
    if((f->flags&MI_ZF))goto mi_0047b42c;
    /* 0047b40b */
    a=(f->r[1]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,3);
    /* 0047b40e */
    if(!(f->flags&MI_ZF))goto mi_0047b42c;
    /* 0047b410 */
    a=0x08000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b415 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b41au;f->boundary(0x0047b415u,0x0047db00u,f);
    /* 0047b41a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047b41d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b422u;f->boundary(0x0047b41du,0x0047e5c0u,f);
    /* 0047b422 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b427u;f->boundary(0x0047b422u,0x0047b670u,f);
    /* 0047b427 */
    f->r[0]=MI_U32((0x00bc9768u));
mi_0047b42c:;
    /* 0047b42c */
    f->r[1]=MI_U32((0x00bc9764u));
    /* 0047b432 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047b433 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047b434 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047b436 */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[1]);
    /* 0047b43c */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047b441 */
    if(mi_signed_less(f->flags))goto mi_0047b3ff;
mi_0047b443:;
    /* 0047b443 */
    f->r[2]=MI_U32((0x00bc9760u));
    /* 0047b449 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0047b44d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047b44e */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[2]);
    /* 0047b454 */
    f->r[2]=0x000000e4u;
    /* 0047b459 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0047b45b */
    f->r[1]=MI_U32((0x00bc9760u));
    /* 0047b461 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047b463 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047b465 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047b46a */
    if(mi_signed_less(f->flags))goto mi_0047b3f5;
mi_0047b46c:;
    /* 0047b46c */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0047b470 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b471 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b472 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b473 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b478u;f->boundary(0x0047b473u,0x004746d0u,f);
    /* 0047b478 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047b47b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047b47c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0047b47d:;
    /* 0047b47d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047b47e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047b47f */
    f->r[4]+=4;return;
}
