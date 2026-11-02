/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_panel_actions.h"
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
static __inline uint32_t mi_alu(struct restore_actor_panel_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_actor_panel_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_actor_panel_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


void restore_actor_panel_C_0040a450(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040a450 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x005f8518u)))&0xffu)<<0);
    /* 0040a455 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0040a458 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040a45a */
    if((f->flags&MI_ZF))goto mi_0040a53e;
    /* 0040a460 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0040a464 */
    f->r[1]=(0x00000008u+f->r[4]);
    /* 0040a468 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a469 */
    f->r[2]=(0x00000008u+f->r[4]);
    /* 0040a46d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a46e */
    f->r[0]=(0x00000008u+f->r[4]);
    /* 0040a472 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a473 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a474 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a479u;f->boundary(0x0040a474u,0x00521aa0u,f);
    /* 0040a479 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040a47e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a481 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040a487 */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 0040a48a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea5ed6u+f->r[0])))&0xffu)<<0);
    /* 0040a490 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040a492 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040a498;
    /* 0040a494 */
    MI_U32((0x00000000u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0040a498:;
    /* 0040a498 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea5ed5u+f->r[0])))&0xffu)<<0);
    /* 0040a49e */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040a4a0 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040a4b0;
    /* 0040a4a2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea5ed9u+f->r[0])))&0xffu)<<0);
    /* 0040a4a8 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040a4aa */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040a4b0;
    /* 0040a4ac */
    MI_U32((0x00000004u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0040a4b0:;
    /* 0040a4b0 */
    a=MI_U32((0x00000008u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040a4b5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040a4bf;
    /* 0040a4b7 */
    f->r[0]=MI_U32((0x00000000u+f->r[4]));
    /* 0040a4bb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040a4bd */
    if(!mi_signed_less(f->flags))goto mi_0040a4ce;
mi_0040a4bf:;
    /* 0040a4bf */
    a=MI_U32((0x0000000cu+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040a4c4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040a4f2;
    /* 0040a4c6 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0040a4ca */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040a4cc */
    if(mi_signed_less(f->flags))goto mi_0040a4f2;
mi_0040a4ce:;
    /* 0040a4ce */
    MI_U32((0x00d3b198u))=(uint32_t)(0x00000004u);
    /* 0040a4d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a4ddu;f->boundary(0x0040a4d8u,0x0040a0e0u,f);
    /* 0040a4dd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a4df */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a4e4 */
    a=0x00000066u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a4e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a4ebu;f->boundary(0x0040a4e6u,0x004264d0u,f);
    /* 0040a4eb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a4ee */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a4f1 */
    f->r[4]+=4;return;
mi_0040a4f2:;
    /* 0040a4f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040a4f4 */
    MI_U32((0x00d3b198u))=(uint32_t)(f->r[0]);
mi_0040a4f9:;
    /* 0040a4f9 */
    f->r[1]=(0x00000010u+f->r[4]);
    /* 0040a4fd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a4fe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a4ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a504u;f->boundary(0x0040a4ffu,0x0044d520u,f);
    /* 0040a504 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0040a508 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0040a50c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a50f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040a511 */
    if((f->flags&MI_ZF))goto mi_0040a519;
    /* 0040a513 */
    a=f->r[0];b=MI_U32((0x00000004u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0040a517 */
    if(!(f->flags&MI_ZF))goto mi_0040a527;
mi_0040a519:;
    /* 0040a519 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a51a */
    MI_U32((0x006057e4u))=(uint32_t)(f->r[0]);
    /* 0040a51f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a524u;f->boundary(0x0040a51fu,0x0044c1a0u,f);
    /* 0040a524 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0040a527:;
    /* 0040a527 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040a52c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040a52d */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0040a530 */
    MI_U32((0x00d3b198u))=(uint32_t)(f->r[0]);
    /* 0040a535 */
    if(mi_signed_less(f->flags))goto mi_0040a4f9;
    /* 0040a537 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0040a53e:;
    /* 0040a53e */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a541 */
    f->r[4]+=4;return;
}
void restore_actor_panel_C_0040a560(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040a560 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040a565 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a566 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a56bu;f->boundary(0x0040a566u,0x004bc620u,f);
    /* 0040a56b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a56e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040a570 */
    if((f->flags&MI_ZF))goto mi_0040a5be;
    /* 0040a572 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040a575 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040a578 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040a57a */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040a57d */
    a=MI_U16((0x00bcd72au+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040a585 */
    if(!mi_signed_less(f->flags))goto mi_0040a5be;
    /* 0040a587 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[1]));
    /* 0040a58e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[1]));
    /* 0040a595 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0040a598 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0040a59b */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0040a5a1 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0040a5a3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040a5a6 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0040a5a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a5aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a5afu;f->boundary(0x0040a5aau,0x00430cd0u,f);
    /* 0040a5af */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0040a5b1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a5b4 */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[0]&0xffu));
    /* 0040a5b9 */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[0]&0xffu));
mi_0040a5be:;
    /* 0040a5be */
    f->r[4]+=4;return;
}
void restore_actor_panel_C_0040a5c0(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040a5c0 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040a5c5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a5c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a5cbu;f->boundary(0x0040a5c6u,0x004bc620u,f);
    /* 0040a5cb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a5ce */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040a5d0 */
    if((f->flags&MI_ZF))goto mi_0040a5f5;
    /* 0040a5d2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040a5d5 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040a5d8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040a5da */
    a=MI_U16((0x00bcd72au+f->r[1]*4u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040a5e3 */
    if(!mi_signed_less(f->flags))goto mi_0040a5f5;
    /* 0040a5e5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a5e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a5ebu;f->boundary(0x0040a5e6u,0x004bc670u,f);
    /* 0040a5eb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a5ee */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0040a5f5:;
    /* 0040a5f5 */
    f->r[4]+=4;return;
}
void restore_actor_panel_C_0040a600(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040a600 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a601 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0040a606 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a607 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a608 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a609 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a60eu;f->boundary(0x0040a609u,0x004bc620u,f);
    /* 0040a60e */
    f->r[7]=f->r[0];
    /* 0040a610 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a613 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0040a615 */
    if((f->flags&MI_ZF))goto mi_0040a6b8;
    /* 0040a61b */
    f->r[1]=(0x0000000au+f->r[4]);
    /* 0040a61f */
    f->r[2]=(0x0000000bu+f->r[4]);
    /* 0040a623 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a624 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a625 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a62au;f->boundary(0x0040a625u,0x00521a60u,f);
    /* 0040a62a */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 0040a62d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a630 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040a633 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040a635 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040a638 */
    a=MI_U16((0x00bcd72au+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040a640 */
    if(!mi_signed_less(f->flags))goto mi_0040a6b8;
    /* 0040a642 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000au+f->r[4])))&0xffu)<<0);
    /* 0040a646 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040a648 */
    if(!(f->flags&MI_ZF))goto mi_0040a69b;
    /* 0040a64a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000bu+f->r[4])))&0xffu)<<0);
    /* 0040a64e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a650 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040a652 */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a657 */
    if((f->flags&MI_ZF))goto mi_0040a67a;
    /* 0040a659 */
    a=0x00000026u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a65b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a660u;f->boundary(0x0040a65bu,0x004264d0u,f);
    /* 0040a660 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a663 */
    MI_U8((0x00bcd5fau+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0040a66a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a66fu;f->boundary(0x0040a66au,0x004b8aa0u,f);
    /* 0040a66f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040a670 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040a677 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040a678 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040a679 */
    f->r[4]+=4;return;
mi_0040a67a:;
    /* 0040a67a */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a67c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a681u;f->boundary(0x0040a67cu,0x004264d0u,f);
    /* 0040a681 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a684 */
    MI_U8((0x00bcd5fau+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0040a68b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a690u;f->boundary(0x0040a68bu,0x004b8aa0u,f);
    /* 0040a690 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040a691 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040a698 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040a699 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040a69a */
    f->r[4]+=4;return;
mi_0040a69b:;
    /* 0040a69b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd5fau+f->r[6])))&0xffu)<<0);
    /* 0040a6a1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040a6a3 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0040a6a6 */
    MI_U8((0x00bcd5fau+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0040a6ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a6b1u;f->boundary(0x0040a6acu,0x004b8aa0u,f);
    /* 0040a6b1 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0040a6b8:;
    /* 0040a6b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040a6b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040a6ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040a6bb */
    f->r[4]+=4;return;
}
void restore_actor_panel_C_004bc620(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bc620 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bc621 */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 004bc627 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bc628 */
    f->r[6]=0x00000001u;
    /* 004bc62d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bc62e */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004bc632 */
    f->r[0]=f->r[6];
    /* 004bc634 */
    f->r[1]=0x00bcd786u;
mi_004bc639:;
    /* 004bc639 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[1])))&0xffu)<<0);
    /* 004bc63c */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bc63e */
    if((f->flags&MI_ZF))goto mi_004bc653;
    /* 004bc640 */
    a=MI_U8((0x00000000u+f->r[1]));b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004bc643 */
    if(!(f->flags&MI_ZF))goto mi_004bc653;
    /* 004bc645 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bc647 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000005u+f->r[1])))&0xffu)<<0);
    /* 004bc64a */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004bc64c */
    if(!(f->flags&MI_ZF))goto mi_004bc653;
    /* 004bc64e */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004bc650 */
    if((f->flags&MI_ZF))goto mi_004bc664;
    /* 004bc652 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
mi_004bc653:;
    /* 004bc653 */
    a=f->r[1];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bc659 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bc65a */
    a=f->r[1];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004bc660 */
    if(mi_signed_less(f->flags))goto mi_004bc639;
    /* 004bc662 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004bc664:;
    /* 004bc664 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bc665 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bc666 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004bc667 */
    f->r[4]+=4;return;
}
void restore_actor_panel_C_00521a60(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00521a60 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00521a63 */
    f->r[0]=(0x00000004u+f->r[4]);
    /* 00521a67 */
    f->r[1]=(0x00000000u+f->r[4]);
    /* 00521a6b */
    f->r[2]=(0x00000008u+f->r[4]);
    /* 00521a6f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521a70 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521a71 */
    f->r[0]=(0x00000014u+f->r[4]);
    /* 00521a75 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521a76 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521a77 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00521a7cu;f->boundary(0x00521a77u,0x00521aa0u,f);
    /* 00521a7c */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00521a80 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00521a84 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00521a88 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00521a8a */
    t=(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00521a8d */
    MI_U8((0x00000000u+f->r[2]))=(uint8_t)((f->r[1]&0xffu));
    /* 00521a8f */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 00521a93 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00521a95 */
    t=(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00521a98 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00521a9a */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00521a9d */
    f->r[4]+=4;return;
}
void restore_actor_panel_C_00521aa0(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00521aa0 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00521aa4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521aa5 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 00521aa9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521aaa */
    f->r[5]=MI_U32((0x00000010u+f->r[4]));
    /* 00521aae */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521aaf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521ab0 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 00521ab4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521ab6 */
    f->r[1]=0x00785d70u;
    /* 00521abb */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0xffffffffu);
    /* 00521ac1 */
    MI_U32((0x00000000u+f->r[5]))=(uint32_t)(0xffffffffu);
    /* 00521ac8 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0x00000000u);
    /* 00521ace */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 00521ad4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00521ad9u;f->boundary(0x00521ad4u,0x0044baf0u,f);
    /* 00521ad9 */
    f->r[6]=f->r[0];
    /* 00521adb */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00521add */
    if((f->flags&MI_ZF))goto mi_00521b3f;
mi_00521adf:;
    /* 00521adf */
    f->r[1]=f->r[6];
    /* 00521ae1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00521ae6u;f->boundary(0x00521ae1u,0x0044d4c0u,f);
    /* 00521ae6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00521ae8 */
    if((f->flags&MI_ZF))goto mi_00521b2e;
    /* 00521aea */
    a=MI_U16((0x00000004u+f->r[6]));b=0x00000025u;t=mi_alu(f,a,b,16,1);
    /* 00521aef */
    if(!(f->flags&MI_ZF))goto mi_00521b2e;
    /* 00521af1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 00521af5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00521af7 */
    if(mi_signed_less(f->flags))goto mi_00521b2e;
    /* 00521af9 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 00521afc */
    if(!mi_signed_less(f->flags))goto mi_00521b2e;
    /* 00521afe */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00521b01 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00521b04 */
    a=MI_U8((0x007770f0u+f->r[1]*4u));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00521b0c */
    if(!(f->flags&MI_ZF))goto mi_00521b22;
    /* 00521b0e */
    a=MI_U32((0x00000000u+f->r[5]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 00521b12 */
    if(!mi_signed_less(f->flags))goto mi_00521b1a;
    /* 00521b14 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 00521b17 */
    MI_U32((0x00000000u+f->r[5]))=(uint32_t)(f->r[2]);
mi_00521b1a:;
    /* 00521b1a */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00521b1e */
    a=MI_U32((0x00000000u+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000000u+f->r[0]))=(uint32_t)(t);
    /* 00521b20 */
    goto mi_00521b2e;
mi_00521b22:;
    /* 00521b22 */
    a=MI_U32((0x00000000u+f->r[7]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 00521b25 */
    if(!mi_signed_less(f->flags))goto mi_00521b2c;
    /* 00521b27 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 00521b2a */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[0]);
mi_00521b2c:;
    /* 00521b2c */
    a=MI_U32((0x00000000u+f->r[3]));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000000u+f->r[3]))=(uint32_t)(t);
mi_00521b2e:;
    /* 00521b2e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00521b2f */
    f->r[1]=0x00785d70u;
    /* 00521b34 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00521b39u;f->boundary(0x00521b34u,0x0044baf0u,f);
    /* 00521b39 */
    f->r[6]=f->r[0];
    /* 00521b3b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00521b3d */
    if(!(f->flags&MI_ZF))goto mi_00521adf;
mi_00521b3f:;
    /* 00521b3f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00521b40 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00521b41 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00521b42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00521b43 */
    f->r[4]+=4;return;
}
void restore_actor_panel_C_004b8aa0(struct restore_actor_panel_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b8aa0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8aa1 */
    f->r[6]=MI_U32((0x00eaac28u));
    /* 004b8aa7 */
    f->r[1]=f->r[6];
    /* 004b8aa9 */
    f->r[0]=0x00bcd78bu;
    /* 004b8aae */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004b8ab4 */
    MI_U8((0x00ea5ed9u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004b8abb */
    MI_U8((0x00ea5ed8u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004b8ac2 */
    MI_U8((0x00ea5ed4u+f->r[1]))=(uint8_t)(0x00000000u);
mi_004b8ac9:;
    /* 004b8ac9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffffffu+f->r[0])))&0xffu)<<0);
    /* 004b8acc */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8ace */
    if((f->flags&MI_ZF))goto mi_004b8b08;
    /* 004b8ad0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b8ad2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 004b8ad4 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004b8ad6 */
    if(!(f->flags&MI_ZF))goto mi_004b8b08;
    /* 004b8ad8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffffbu+f->r[0])))&0xffu)<<0);
    /* 004b8adb */
    a=(f->r[2]&0xffu);b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b8ade */
    if(!(f->flags&MI_ZF))goto mi_004b8afd;
    /* 004b8ae0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea5ed8u+f->r[1])))&0xffu)<<0);
    /* 004b8ae6 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b8ae8 */
    MI_U8((0x00ea5ed8u+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 004b8aee */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffff3u+f->r[0])))&0xffu)<<0);
    /* 004b8af1 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8af3 */
    if((f->flags&MI_ZF))goto mi_004b8b08;
    /* 004b8af5 */
    a=MI_U8((0x00ea5ed9u+f->r[1]));b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;MI_U8((0x00ea5ed9u+f->r[1]))=(uint8_t)(t);
    /* 004b8afb */
    goto mi_004b8b08;
mi_004b8afd:;
    /* 004b8afd */
    a=(f->r[2]&0xffu);b=0x0000004du;t=mi_alu(f,a,b,8,1);
    /* 004b8b00 */
    if(!(f->flags&MI_ZF))goto mi_004b8b08;
    /* 004b8b02 */
    a=MI_U8((0x00ea5ed4u+f->r[1]));b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;MI_U8((0x00ea5ed4u+f->r[1]))=(uint8_t)(t);
mi_004b8b08:;
    /* 004b8b08 */
    a=f->r[0];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b8b0d */
    a=f->r[0];b=0x00c8ad47u;t=mi_alu(f,a,b,32,1);
    /* 004b8b12 */
    if(mi_signed_less(f->flags))goto mi_004b8ac9;
    /* 004b8b14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b8b15 */
    f->r[4]+=4;return;
}
