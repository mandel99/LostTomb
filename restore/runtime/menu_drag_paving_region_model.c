/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "paving_region_model.h"

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

void restore_C_004754a0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004754a0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754a1 */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 004754a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754a6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754a7 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 004754ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754ac */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004754ae */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004754c0;
    /* 004754b0 */
    f->r[0]=f->r[3];
    /* 004754b2 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004754b6 */
    f->r[6]=f->r[3];
    /* 004754b8 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 004754bc */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004754c0:;
    /* 004754c0 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004754c4 */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 004754c8 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004754ca */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004754d2;
    /* 004754cc */
    f->r[0]=f->r[7];
    /* 004754ce */
    f->r[7]=f->r[5];
    /* 004754d0 */
    f->r[5]=f->r[0];
mi_004754d2:;
    /* 004754d2 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754d7 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754dc */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754e1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004754e6u;f->boundary(0x004754e1u,0x0040f860u,f);
    /* 004754e6 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754eb */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754f0 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754f5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004754fau;f->boundary(0x004754f5u,0x0040f860u,f);
    /* 004754fa */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004754ff */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475504 */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475509 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047550eu;f->boundary(0x00475509u,0x0040f860u,f);
    /* 0047550e */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475513 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475518 */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047551d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475522u;f->boundary(0x0047551du,0x0040f860u,f);
    /* 00475522 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475524 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475529u;f->boundary(0x00475524u,0x00485830u,f);
    /* 00475529 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047552c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0047552e */
    if(!mi_signed_less(f->flags))goto mi_0047553c;
    /* 00475530 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00475538 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
mi_0047553c:;
    /* 0047553c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047553e */
    if(!mi_signed_less(f->flags))goto mi_00475542;
    /* 00475540 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_00475542:;
    /* 00475542 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 00475548 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047554a */
    if(mi_signed_less(f->flags))goto mi_00475555;
    /* 0047554c */
    f->r[0]=(0xffffffffu+f->r[1]);
    /* 0047554f */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00475553 */
    f->r[6]=f->r[0];
mi_00475555:;
    /* 00475555 */
    f->r[2]=MI_U32((0x00bca2d4u));
    /* 0047555b */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0047555d */
    if(mi_signed_less(f->flags))goto mi_00475562;
    /* 0047555f */
    f->r[5]=(0xffffffffu+f->r[2]);
mi_00475562:;
    /* 00475562 */
    f->r[0]=f->r[1];
    /* 00475564 */
    MI_U32((0x00bc971cu))=(uint32_t)(0x00000000u);
    /* 0047556e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00475570 */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 00475576 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[3]*1u);
    /* 0047557a */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0047557e */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 00475581 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*2u);
    /* 00475584 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00475587 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 0047558a */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0047558c */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047558e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047564d;
    /* 00475594 */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00475596 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00475597 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
mi_0047559b:;
    /* 0047559b */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0047559f */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004755a1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047562a;
    /* 004755a7 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004755ab */
    f->r[5]=f->r[0];
    /* 004755ad */
    f->r[7]=0x00b67d1cu;
    /* 004755b2 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004755b4 */
    a=f->r[7];b=0x00b285ccu;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004755ba */
    f->r[3]=(0x00b4e6fcu+f->r[6]*2u);
    /* 004755c1 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
mi_004755c2:;
    /* 004755c2 */
    f->r[0]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 004755c9 */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004755cb */
    if((f->flags&MI_ZF))goto mi_00475619;
    /* 004755cd */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,3);
    /* 004755cf */
    if(!(f->flags&MI_ZF))goto mi_00475619;
    /* 004755d1 */
    a=MI_U16((0x00000000u+f->r[3]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004755d5 */
    if(!(f->flags&MI_ZF))goto mi_00475619;
    /* 004755d7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004755d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004755ddu;f->boundary(0x004755d8u,0x0047b8e0u,f);
    /* 004755dd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004755e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004755e2 */
    if((f->flags&MI_ZF))goto mi_00475619;
    /* 004755e4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[6])))&0xffu)<<0);
    /* 004755ea */
    a=(f->r[0]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 004755ec */
    if(!(f->flags&MI_ZF))goto mi_004755f4;
    /* 004755ee */
    a=MI_U32((0x00bc971cu));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00bc971cu))=(uint32_t)(t);
mi_004755f4:;
    /* 004755f4 */
    a=(f->r[0]&0xffu);b=0x000000f0u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004755f6 */
    MI_U32((0x00b7482cu+f->r[6]*4u))=(uint32_t)(0x00000000u);
    /* 00475601 */
    a=(f->r[0]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00475603 */
    MI_U8((0x00b285ccu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 00475609 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[7]+f->r[6]*1u)))&0xffu)<<0);
    /* 00475610 */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00475612 */
    MI_U8((0x00b285ccu+f->r[7]+f->r[6]*1u))=(uint8_t)((f->r[0]&0xffu));
mi_00475619:;
    /* 00475619 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047561a */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0047561d */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0047561e */
    if(!(f->flags&MI_ZF))goto mi_004755c2;
    /* 00475620 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 00475624 */
    f->r[1]=MI_U32((0x00bca2d0u));
mi_0047562a:;
    /* 0047562a */
    f->r[2]=MI_U32((0x00bca2dcu));
    /* 00475630 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00475634 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00475636 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047563a */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0047563c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047563d */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00475641 */
    if(!(f->flags&MI_ZF))goto mi_0047559b;
    /* 00475647 */
    f->r[2]=MI_U32((0x00bca2d4u));
mi_0047564d:;
    /* 0047564d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047564e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475650 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00475651 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475652 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475653 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475655 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475657 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047565cu;f->boundary(0x00475657u,0x00475720u,f);
    /* 0047565c */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047565f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00475660 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00475661 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00475662 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00475663 */
    f->r[4]+=4;return;
}
