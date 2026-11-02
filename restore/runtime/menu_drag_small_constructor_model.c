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

void restore_C_00440700(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00440700 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 00440705 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440706 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0044070a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044070b */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044070d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044070e */
    if(mi_signed_less(f->flags))goto mi_00440713;
    /* 00440710 */
    f->r[3]=(0xffffffffu+f->r[0]);
mi_00440713:;
    /* 00440713 */
    f->r[6]=MI_U32((0x00000020u+f->r[4]));
    /* 00440717 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0044071c */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044071e */
    if(mi_signed_less(f->flags))goto mi_00440723;
    /* 00440720 */
    f->r[6]=(0xffffffffu+f->r[0]);
mi_00440723:;
    /* 00440723 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00440729 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0044072c */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044072e */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 00440731 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00440734 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00440737 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00440739 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044073eu;f->boundary(0x00440739u,0x0043f410u,f);
    /* 0044073e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440741 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440743 */
    if(!(f->flags&MI_ZF))goto mi_00440749;
    /* 00440745 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440746 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440747 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00440748 */
    f->r[4]+=4;return;
mi_00440749:;
    /* 00440749 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044074b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044074c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044074d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440752u;f->boundary(0x0044074du,0x004413b0u,f);
    /* 00440752 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440754 */
    f->r[0]=(0x0000002cu+f->r[4]);
    /* 00440758 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044075a */
    f->r[1]=(0x00000034u+f->r[4]);
    /* 0044075e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044075f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440760 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440761 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440762 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440767u;f->boundary(0x00440762u,0x00441130u,f);
    /* 00440767 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044076a */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0044076d */
    if(!(f->flags&MI_ZF))goto mi_00440775;
    /* 0044076f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440770 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440771 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00440773 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00440774 */
    f->r[4]+=4;return;
mi_00440775:;
    /* 00440775 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00440779 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044077b */
    if((f->flags&MI_ZF))goto mi_00440786;
    /* 0044077d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044077e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044077f */
    f->r[0]=0x00000001u;
    /* 00440784 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00440785 */
    f->r[4]+=4;return;
mi_00440786:;
    /* 00440786 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044078b */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440790 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440795 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044079au;f->boundary(0x00440795u,0x0040f860u,f);
    /* 0044079a */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044079f */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407a4 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004407aeu;f->boundary(0x004407a9u,0x0040f860u,f);
    /* 004407ae */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407b3 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407b8 */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004407c2u;f->boundary(0x004407bdu,0x0040f860u,f);
    /* 004407c2 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407c7 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407cc */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004407d6u;f->boundary(0x004407d1u,0x0040f860u,f);
    /* 004407d6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004407ddu;f->boundary(0x004407d8u,0x00485830u,f);
    /* 004407dd */
    f->r[2]=MI_U32((0x00000050u+f->r[4]));
    /* 004407e1 */
    f->r[0]=MI_U32((0x00000054u+f->r[4]));
    /* 004407e5 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004407eb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407ed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407ef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407f0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407f1 */
    f->r[3]=0x00000001u;
    /* 004407f6 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407f8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004407f9 */
    MI_U32((0x00bc971cu))=(uint32_t)(f->r[3]);
    /* 004407ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440804u;f->boundary(0x004407ffu,0x0046a400u,f);
    /* 00440804 */
    f->r[2]=MI_U32((0x00f567acu));
    /* 0044080a */
    f->r[6]=f->r[0];
    /* 0044080c */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044080f */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00440811 */
    f->r[1]=MI_U32((0x0000006cu+f->r[4]));
    /* 00440815 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440818 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044081b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044081d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044081e */
    MI_U32((0x00799ab0u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00440824 */
    MI_U32((0x00799aa8u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 0044082a */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0044082e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044082f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440830 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440831 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440832 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440833 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440838u;f->boundary(0x00440833u,0x0047bf50u,f);
    /* 00440838 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440839 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044083eu;f->boundary(0x00440839u,0x00485100u,f);
    /* 0044083e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044083f */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440841 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440842 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440847u;f->boundary(0x00440842u,0x00440060u,f);
    /* 00440847 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044084a */
    f->r[0]=f->r[3];
    /* 0044084c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044084d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044084e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044084f */
    f->r[4]+=4;return;
}
