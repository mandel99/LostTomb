/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "tooltip_controller.h"
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
static __inline uint32_t mi_alu(struct restore_tooltip_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_tooltip_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_tooltip_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_tooltip_C_004c8750(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c8750 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c8751 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38dcau)))&0xffu)<<0);
    /* 004c8756 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c8757 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c8758 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004c875a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c875b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c875c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c875e */
    if(!(f->flags&MI_ZF))goto mi_004c8855;
    /* 004c8764 */
    a=MI_U32((0x00e38eacu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c876a */
    if(!(f->flags&MI_ZF))goto mi_004c877e;
mi_004c876c:;
    /* 004c876c */
    f->r[0]=MI_U32((0x00e92e0cu));
    /* 004c8771 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c8772 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c8773 */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[0]);
    /* 004c8778 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c8779 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c877b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c877c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c877d */
    f->r[4]+=4;return;
mi_004c877e:;
    /* 004c877e */
    f->r[6]=MI_U32((0x00e92e0cu));
    /* 004c8784 */
    f->r[1]=MI_U32((0x00d3a6c8u));
    /* 004c878a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004c8790 */
    f->r[7]=f->r[6];
    /* 004c8792 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004c8794 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004c8797 */
    f->r[3]=0x00000002u;
    /* 004c879c */
    if(!(f->flags&MI_ZF))goto mi_004c8acf;
    /* 004c87a2 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c87a9 */
    if(!(f->flags&MI_ZF))goto mi_004c88a7;
    /* 004c87af */
    f->r[1]=MI_U32((0x00bca2c4u));
    /* 004c87b5 */
    f->r[0]=MI_U32((0x00605a38u));
    /* 004c87ba */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c87bc */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[1]*2u));
    /* 004c87c4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004c87df;
    /* 004c87c6 */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c87cb */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c87ce */
    if(mi_signed_less(f->flags))goto mi_004c87df;
    /* 004c87d0 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004c87d3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004c87df;
    /* 004c87d5 */
    a=f->r[0];b=0x000000a6u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c87da */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
mi_004c87df:;
    /* 004c87df */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c87e4 */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 004c87e7 */
    if(!(f->flags&MI_ZF))goto mi_004c87f5;
    /* 004c87e9 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x000000b1u);
    /* 004c87f3 */
    goto mi_004c882b;
mi_004c87f5:;
    /* 004c87f5 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c87f8 */
    if(mi_signed_less(f->flags))goto mi_004c880b;
    /* 004c87fa */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004c87fd */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004c880b;
    /* 004c87ff */
    f->r[2]=(0x00000090u+f->r[0]);
    /* 004c8805 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[2]);
mi_004c880b:;
    /* 004c880b */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004c880e */
    if(!(f->flags&MI_ZF))goto mi_004c881c;
    /* 004c8810 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x000000afu);
    /* 004c881a */
    goto mi_004c882b;
mi_004c881c:;
    /* 004c881c */
    a=f->r[0];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 004c881f */
    if(!(f->flags&MI_ZF))goto mi_004c882b;
    /* 004c8821 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x000000b0u);
mi_004c882b:;
    /* 004c882b */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c882d */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c882f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((0x00000024u)&0xffu)<<0);
mi_004c8831:;
    /* 004c8831 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004c8834 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c8837 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8839 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c883c */
    a=MI_U8((0x00bcd602u+f->r[0]));b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004c8842 */
    if((f->flags&MI_ZF))goto mi_004c885d;
    /* 004c8844 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[0]));
    /* 004c884b */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c884d */
    if(!(f->flags&MI_ZF))goto mi_004c8831;
mi_004c884f:;
    /* 004c884f */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[6]);
mi_004c8855:;
    /* 004c8855 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c8856 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c8857 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c8858 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c885a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c885b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c885c */
    f->r[4]+=4;return;
mi_004c885d:;
    /* 004c885d */
    f->r[1]=MI_U32((0x00d3b130u));
    /* 004c8863 */
    f->r[2]=MI_U32((0x00d3b13cu));
    /* 004c8869 */
    MI_U32((0x00d3a6bcu))=(uint32_t)(f->r[1]);
    /* 004c886f */
    f->r[1]=MI_U32((0x00d3b138u));
    /* 004c8875 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c8876 */
    f->r[0]=0x00000001u;
    /* 004c887b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c887c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c887d */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8882 */
    MI_U32((0x00d3a6c0u))=(uint32_t)(0x00000004u);
    /* 004c888c */
    MI_U32((0x00d3a6c4u))=(uint32_t)(f->r[0]);
    /* 004c8891 */
    MI_U32((0x00d3a6b8u))=(uint32_t)(f->r[2]);
    /* 004c8897 */
    MI_U32((0x00d3a6b4u))=(uint32_t)(f->r[1]);
    /* 004c889d */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000002u);
    /* 004c88a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c88a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c88a6 */
    f->r[4]+=4;return;
mi_004c88a7:;
    /* 004c88a7 */
    f->r[0]=MI_U32((0x00f53f78u));
    /* 004c88ac */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c88ae */
    if(!(f->flags&MI_ZF))goto mi_004c8a2a;
    /* 004c88b4 */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c88b9 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004c88bc */
    if(!(f->flags&MI_ZF))goto mi_004c88da;
    /* 004c88be */
    f->r[1]=MI_U32((0x00705698u));
    /* 004c88c4 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c88c6 */
    if((f->flags&MI_ZF))goto mi_004c8f81;
    /* 004c88cc */
    a=f->r[1];b=0x0000002bu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c88cf */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[1]);
    /* 004c88d5 */
    goto mi_004c9211;
mi_004c88da:;
    /* 004c88da */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c88dd */
    if(mi_signed_less(f->flags))goto mi_004c88ee;
    /* 004c88df */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c88e2 */
    if(!mi_signed_less(f->flags))goto mi_004c88f3;
    /* 004c88e4 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c88e9 */
    goto mi_004c9211;
mi_004c88ee:;
    /* 004c88ee */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c88f1 */
    if(mi_signed_less(f->flags))goto mi_004c8916;
mi_004c88f3:;
    /* 004c88f3 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004c88f6 */
    if(!mi_signed_less(f->flags))goto mi_004c891b;
    /* 004c88f8 */
    a=f->r[0];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 004c88fb */
    if(!(f->flags&MI_ZF))goto mi_004c8909;
    /* 004c88fd */
    a=MI_U32((0x00e38ebcu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8903 */
    if(!(f->flags&MI_ZF))goto mi_004c9211;
mi_004c8909:;
    /* 004c8909 */
    a=f->r[0];b=0xfffffff6u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c890c */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8911 */
    goto mi_004c9211;
mi_004c8916:;
    /* 004c8916 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004c8919 */
    if(mi_signed_less(f->flags))goto mi_004c8924;
mi_004c891b:;
    /* 004c891b */
    a=f->r[0];b=0x00000046u;t=mi_alu(f,a,b,32,1);
    /* 004c891e */
    if(mi_signed_less(f->flags))goto mi_004c8f7e;
mi_004c8924:;
    /* 004c8924 */
    a=MI_U32((0x007056a0u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c892a */
    if((f->flags&MI_ZF))goto mi_004c893b;
    /* 004c892c */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000024u);
    /* 004c8936 */
    goto mi_004c9211;
mi_004c893b:;
    /* 004c893b */
    f->r[0]=MI_U32((0x007056a8u));
    /* 004c8940 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8942 */
    if((f->flags&MI_ZF))goto mi_004c8951;
    /* 004c8944 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8947 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c894c */
    goto mi_004c9211;
mi_004c8951:;
    /* 004c8951 */
    f->r[0]=MI_U32((0x0070569cu));
    /* 004c8956 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8958 */
    if((f->flags&MI_ZF))goto mi_004c8967;
    /* 004c895a */
    a=f->r[0];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c895d */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8962 */
    goto mi_004c9211;
mi_004c8967:;
    /* 004c8967 */
    f->r[1]=MI_U32((0x00bca2c4u));
    /* 004c896d */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c896f */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8975 */
    f->r[0]=MI_U32((0x00e94e0cu));
    /* 004c897a */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c897c */
    if((f->flags&MI_ZF))goto mi_004c8987;
mi_004c897e:;
    /* 004c897e */
    a=f->r[0];b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 004c8981 */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
mi_004c8987:;
    /* 004c8987 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00af598cu+f->r[1]*4u)))&0xffu)<<0);
    /* 004c898e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[1]*2u));
    /* 004c8996 */
    a=(f->r[2]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004c8999 */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c899f */
    f->r[2]=f->r[0];
    /* 004c89a1 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c89a4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c89a6 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[2]*8u)))&0xffffu)<<0);
    /* 004c89ae */
    a=(f->r[1]&0xffffu);b=0x000000d1u;t=mi_alu(f,a,b,16,1);
    /* 004c89b3 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c89b9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c89bb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004c884f;
    /* 004c89c1 */
    a=(f->r[1]&0xffffu);b=0x00000055u;t=mi_alu(f,a,b,16,1);
    /* 004c89c5 */
    if((f->flags&MI_ZF))goto mi_004c89e0;
    /* 004c89c7 */
    a=(f->r[1]&0xffffu);b=0x000000bbu;t=mi_alu(f,a,b,16,1);
    /* 004c89cc */
    if((f->flags&MI_ZF))goto mi_004c89e0;
    /* 004c89ce */
    a=(f->r[1]&0xffffu);b=0x000000bcu;t=mi_alu(f,a,b,16,1);
    /* 004c89d3 */
    if((f->flags&MI_ZF))goto mi_004c89e0;
    /* 004c89d5 */
    a=(f->r[1]&0xffffu);b=0x000000bdu;t=mi_alu(f,a,b,16,1);
    /* 004c89da */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
mi_004c89e0:;
    /* 004c89e0 */
    f->r[1]=MI_U32((0x00d3b130u));
    /* 004c89e6 */
    f->r[2]=MI_U32((0x00d3b13cu));
    /* 004c89ec */
    f->r[3]=0x00000002u;
    /* 004c89f1 */
    MI_U32((0x00d3a6bcu))=(uint32_t)(f->r[1]);
    /* 004c89f7 */
    f->r[1]=MI_U32((0x00d3b138u));
    /* 004c89fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c89fe */
    f->r[0]=0x00000001u;
    /* 004c8a03 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c8a04 */
    MI_U32((0x00d3a6c0u))=(uint32_t)(f->r[3]);
    /* 004c8a0a */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[3]&0xffu));
    /* 004c8a10 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c8a11 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8a16 */
    MI_U32((0x00d3a6c4u))=(uint32_t)(f->r[0]);
    /* 004c8a1b */
    MI_U32((0x00d3a6b8u))=(uint32_t)(f->r[2]);
    /* 004c8a21 */
    MI_U32((0x00d3a6b4u))=(uint32_t)(f->r[1]);
    /* 004c8a27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c8a28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c8a29 */
    f->r[4]+=4;return;
mi_004c8a2a:;
    /* 004c8a2a */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8a2d */
    if(!(f->flags&MI_ZF))goto mi_004c8855;
    /* 004c8a33 */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c8a38 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004c8a3b */
    if(!(f->flags&MI_ZF))goto mi_004c8a53;
    /* 004c8a3d */
    f->r[0]=MI_U32((0x00705698u));
    /* 004c8a42 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8a44 */
    if((f->flags&MI_ZF))goto mi_004c8a5d;
    /* 004c8a46 */
    a=f->r[0];b=0x0000002bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8a49 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8a4e */
    goto mi_004c9211;
mi_004c8a53:;
    /* 004c8a53 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c8a56 */
    if(mi_signed_less(f->flags))goto mi_004c8a6c;
    /* 004c8a58 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c8a5b */
    if(!mi_signed_less(f->flags))goto mi_004c8a71;
mi_004c8a5d:;
    /* 004c8a5d */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000000cu);
    /* 004c8a67 */
    goto mi_004c9211;
mi_004c8a6c:;
    /* 004c8a6c */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c8a6f */
    if(mi_signed_less(f->flags))goto mi_004c8a83;
mi_004c8a71:;
    /* 004c8a71 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004c8a74 */
    if(!mi_signed_less(f->flags))goto mi_004c8a83;
    /* 004c8a76 */
    a=f->r[0];b=0xfffffff6u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8a79 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8a7e */
    goto mi_004c9211;
mi_004c8a83:;
    /* 004c8a83 */
    f->r[0]=MI_U32((0x007056a8u));
    /* 004c8a88 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8a8a */
    if((f->flags&MI_ZF))goto mi_004c8a99;
    /* 004c8a8c */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8a8f */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8a94 */
    goto mi_004c9211;
mi_004c8a99:;
    /* 004c8a99 */
    f->r[0]=MI_U32((0x0070569cu));
    /* 004c8a9e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8aa0 */
    if((f->flags&MI_ZF))goto mi_004c8aaf;
    /* 004c8aa2 */
    a=f->r[0];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8aa5 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8aaa */
    goto mi_004c9211;
mi_004c8aaf:;
    /* 004c8aaf */
    f->r[1]=MI_U32((0x00bca2c4u));
    /* 004c8ab5 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8ab7 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8abd */
    f->r[0]=MI_U32((0x00e94e0cu));
    /* 004c8ac2 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8ac4 */
    if((f->flags&MI_ZF))goto mi_004c8987;
    /* 004c8aca */
    goto mi_004c897e;
mi_004c8acf:;
    /* 004c8acf */
    a=(f->r[2]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004c8ad2 */
    if(!(f->flags&MI_ZF))goto mi_004c8d0d;
    /* 004c8ad8 */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c8add */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c8ae0 */
    if(mi_signed_less(f->flags))goto mi_004c8af4;
    /* 004c8ae2 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 004c8ae5 */
    if(!mi_signed_less(f->flags))goto mi_004c8af9;
    /* 004c8ae7 */
    a=f->r[0];b=0x0000003du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8aea */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8aef */
    goto mi_004c9211;
mi_004c8af4:;
    /* 004c8af4 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 004c8af7 */
    if(mi_signed_less(f->flags))goto mi_004c8b0b;
mi_004c8af9:;
    /* 004c8af9 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c8afc */
    if(!mi_signed_less(f->flags))goto mi_004c8b0b;
    /* 004c8afe */
    a=f->r[0];b=0xffffffe8u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8b01 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8b06 */
    goto mi_004c9211;
mi_004c8b0b:;
    /* 004c8b0b */
    f->r[0]=MI_U32((0x00605a44u));
    /* 004c8b10 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8b13 */
    if(!(f->flags&MI_ZF))goto mi_004c8b47;
    /* 004c8b15 */
    a=MI_U32((0x007056c8u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8b1b */
    if((f->flags&MI_ZF))goto mi_004c8b2c;
    /* 004c8b1d */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000005cu);
    /* 004c8b27 */
    goto mi_004c9211;
mi_004c8b2c:;
    /* 004c8b2c */
    a=MI_U32((0x007056b0u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8b32 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8b38 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000005du);
    /* 004c8b42 */
    goto mi_004c9211;
mi_004c8b47:;
    /* 004c8b47 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c8b4a */
    if(!(f->flags&MI_ZF))goto mi_004c8ba2;
    /* 004c8b4c */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c8b51 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8b53 */
    if(!(f->flags&MI_ZF))goto mi_004c8b71;
    /* 004c8b55 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004c8b5b */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004c8b61 */
    a=MI_U32((0x00ea620cu+f->r[1]));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8b67 */
    if(!(f->flags&MI_ZF))goto mi_004c8b7d;
    /* 004c8b69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c8b6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c8b6b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c8b6c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c8b6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c8b6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c8b70 */
    f->r[4]+=4;return;
mi_004c8b71:;
    /* 004c8b71 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8b73 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8b79 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8b7b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004c8b8a;
mi_004c8b7d:;
    /* 004c8b7d */
    a=f->r[0];b=0x0000005fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8b80 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8b85 */
    goto mi_004c9211;
mi_004c8b8a:;
    /* 004c8b8a */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004c8b8d */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8b93 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000085u);
    /* 004c8b9d */
    goto mi_004c9211;
mi_004c8ba2:;
    /* 004c8ba2 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004c8ba5 */
    if(!(f->flags&MI_ZF))goto mi_004c8bcb;
    /* 004c8ba7 */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c8bac */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8baf */
    if(mi_signed_less(f->flags))goto mi_004c884f;
    /* 004c8bb5 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c8bb8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004c884f;
    /* 004c8bbe */
    a=f->r[0];b=0x00000067u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8bc1 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8bc6 */
    goto mi_004c9211;
mi_004c8bcb:;
    /* 004c8bcb */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8bcd */
    if(!(f->flags&MI_ZF))goto mi_004c8beb;
    /* 004c8bcf */
    a=MI_U32((0x007056c8u));b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 004c8bd6 */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8bdc */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000090u);
    /* 004c8be6 */
    goto mi_004c9211;
mi_004c8beb:;
    /* 004c8beb */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004c8bee */
    if(!(f->flags&MI_ZF))goto mi_004c8c0c;
    /* 004c8bf0 */
    a=MI_U32((0x007056c8u));b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 004c8bf7 */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8bfd */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000008fu);
    /* 004c8c07 */
    goto mi_004c9211;
mi_004c8c0c:;
    /* 004c8c0c */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004c8c0f */
    if(!(f->flags&MI_ZF))goto mi_004c8ca9;
    /* 004c8c15 */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c8c1a */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004c8c1d */
    if(!(f->flags&MI_ZF))goto mi_004c8c6b;
    /* 004c8c1f */
    f->r[2]=MI_U32((0x00d3b130u));
    /* 004c8c25 */
    f->r[1]=MI_U32((0x00d3b138u));
    /* 004c8c2b */
    MI_U32((0x00d3a6c0u))=(uint32_t)(f->r[0]);
    /* 004c8c30 */
    f->r[0]=MI_U32((0x00d3b13cu));
    /* 004c8c35 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c8c36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c8c37 */
    MI_U32((0x00d3a6b8u))=(uint32_t)(f->r[0]);
    /* 004c8c3c */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[3]&0xffu));
    /* 004c8c42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c8c43 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000001u);
    /* 004c8c4d */
    MI_U32((0x00d3a6c4u))=(uint32_t)(0x00000007u);
    /* 004c8c57 */
    MI_U32((0x00d3a6bcu))=(uint32_t)(f->r[2]);
    /* 004c8c5d */
    MI_U32((0x00d3a6b4u))=(uint32_t)(f->r[1]);
    /* 004c8c63 */
    f->r[0]=0x00000001u;
    /* 004c8c68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c8c69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c8c6a */
    f->r[4]+=4;return;
mi_004c8c6b:;
    /* 004c8c6b */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8c6e */
    if(!(f->flags&MI_ZF))goto mi_004c8c7f;
    /* 004c8c70 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000006cu);
    /* 004c8c7a */
    goto mi_004c9211;
mi_004c8c7f:;
    /* 004c8c7f */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8c81 */
    if(!(f->flags&MI_ZF))goto mi_004c8c92;
    /* 004c8c83 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000002au);
    /* 004c8c8d */
    goto mi_004c9211;
mi_004c8c92:;
    /* 004c8c92 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8c94 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8c9a */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000006du);
    /* 004c8ca4 */
    goto mi_004c9211;
mi_004c8ca9:;
    /* 004c8ca9 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004c8cac */
    if(!(f->flags&MI_ZF))goto mi_004c8cc9;
    /* 004c8cae */
    a=MI_U32((0x007056c8u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8cb4 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8cba */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000071u);
    /* 004c8cc4 */
    goto mi_004c9211;
mi_004c8cc9:;
    /* 004c8cc9 */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004c8ccc */
    if(!(f->flags&MI_ZF))goto mi_004c8ce9;
    /* 004c8cce */
    a=MI_U32((0x007056c8u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8cd4 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8cda */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000072u);
    /* 004c8ce4 */
    goto mi_004c9211;
mi_004c8ce9:;
    /* 004c8ce9 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004c8cec */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8cf2 */
    a=MI_U32((0x007056b0u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8cf8 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8cfe */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000007au);
    /* 004c8d08 */
    goto mi_004c9211;
mi_004c8d0d:;
    /* 004c8d0d */
    a=(f->r[2]&0xffu);b=0x0000002au;t=mi_alu(f,a,b,8,1);
    /* 004c8d10 */
    if(!(f->flags&MI_ZF))goto mi_004c8d42;
    /* 004c8d12 */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c8d17 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8d1a */
    if(!(f->flags&MI_ZF))goto mi_004c8d2b;
    /* 004c8d1c */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000005eu);
    /* 004c8d26 */
    goto mi_004c9211;
mi_004c8d2b:;
    /* 004c8d2b */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8d2d */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8d33 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000005fu);
    /* 004c8d3d */
    goto mi_004c9211;
mi_004c8d42:;
    /* 004c8d42 */
    a=(f->r[2]&0xffu);b=0x0000002cu;t=mi_alu(f,a,b,8,1);
    /* 004c8d45 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8d4b */
    a=(f->r[2]&0xffu);b=0x0000004du;t=mi_alu(f,a,b,8,1);
    /* 004c8d4e */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8d54 */
    a=(f->r[2]&0xffu);b=0x0000002du;t=mi_alu(f,a,b,8,1);
    /* 004c8d57 */
    if(!(f->flags&MI_ZF))goto mi_004c8db3;
    /* 004c8d59 */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c8d5e */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8d61 */
    if(!(f->flags&MI_ZF))goto mi_004c8d72;
    /* 004c8d63 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000064u);
    /* 004c8d6d */
    goto mi_004c9211;
mi_004c8d72:;
    /* 004c8d72 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8d74 */
    if(!(f->flags&MI_ZF))goto mi_004c8d85;
    /* 004c8d76 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000065u);
    /* 004c8d80 */
    goto mi_004c9211;
mi_004c8d85:;
    /* 004c8d85 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8d87 */
    if((f->flags&MI_ZF))goto mi_004c8d98;
    /* 004c8d89 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000066u);
    /* 004c8d93 */
    goto mi_004c9211;
mi_004c8d98:;
    /* 004c8d98 */
    a=MI_U32((0x007056b0u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8d9e */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8da4 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000067u);
    /* 004c8dae */
    goto mi_004c9211;
mi_004c8db3:;
    /* 004c8db3 */
    a=(f->r[2]&0xffu);b=0x00000028u;t=mi_alu(f,a,b,8,1);
    /* 004c8db6 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8dbc */
    a=(f->r[2]&0xffu);b=0x0000002bu;t=mi_alu(f,a,b,8,1);
    /* 004c8dbf */
    if(!(f->flags&MI_ZF))goto mi_004c8e5c;
    /* 004c8dc5 */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c8dca */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c8dcd */
    if(!(f->flags&MI_ZF))goto mi_004c8dde;
    /* 004c8dcf */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000001u);
    /* 004c8dd9 */
    goto mi_004c9211;
mi_004c8dde:;
    /* 004c8dde */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004c8de1 */
    if((f->flags&MI_ZF))goto mi_004c920b;
    /* 004c8de7 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004c8dea */
    if(!(f->flags&MI_ZF))goto mi_004c8dfb;
    /* 004c8dec */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000073u);
    /* 004c8df6 */
    goto mi_004c9211;
mi_004c8dfb:;
    /* 004c8dfb */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004c8dfe */
    if(!(f->flags&MI_ZF))goto mi_004c8e0f;
    /* 004c8e00 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000074u);
    /* 004c8e0a */
    goto mi_004c9211;
mi_004c8e0f:;
    /* 004c8e0f */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c8e14 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8e17 */
    if((f->flags&MI_ZF))goto mi_004c8eb9;
    /* 004c8e1d */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8e1f */
    if((f->flags&MI_ZF))goto mi_004c8ed9;
    /* 004c8e25 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004c8e28 */
    if((f->flags&MI_ZF))goto mi_004c8efa;
    /* 004c8e2e */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c8e31 */
    if((f->flags&MI_ZF))goto mi_004c8f1f;
mi_004c8e37:;
    /* 004c8e37 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004c8e3a */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8e40 */
    a=MI_U16((0x007848eau));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004c8e47 */
    if((f->flags&MI_ZF))goto mi_004c9211;
    /* 004c8e4d */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000079u);
    /* 004c8e57 */
    goto mi_004c9211;
mi_004c8e5c:;
    /* 004c8e5c */
    a=(f->r[2]&0xffu);b=0x00000052u;t=mi_alu(f,a,b,8,1);
    /* 004c8e5f */
    if(!(f->flags&MI_ZF))goto mi_004c8f3b;
    /* 004c8e65 */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c8e6a */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c8e6d */
    if(!(f->flags&MI_ZF))goto mi_004c8e7e;
    /* 004c8e6f */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000001u);
    /* 004c8e79 */
    goto mi_004c9211;
mi_004c8e7e:;
    /* 004c8e7e */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004c8e81 */
    if((f->flags&MI_ZF))goto mi_004c920b;
    /* 004c8e87 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004c8e8a */
    if(!(f->flags&MI_ZF))goto mi_004c8e9b;
    /* 004c8e8c */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000073u);
    /* 004c8e96 */
    goto mi_004c9211;
mi_004c8e9b:;
    /* 004c8e9b */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004c8e9e */
    if(!(f->flags&MI_ZF))goto mi_004c8eaf;
    /* 004c8ea0 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000074u);
    /* 004c8eaa */
    goto mi_004c9211;
mi_004c8eaf:;
    /* 004c8eaf */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c8eb4 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c8eb7 */
    if(!(f->flags&MI_ZF))goto mi_004c8ed5;
mi_004c8eb9:;
    /* 004c8eb9 */
    a=MI_U16((0x007848e2u));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004c8ec0 */
    if((f->flags&MI_ZF))goto mi_004c9211;
    /* 004c8ec6 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000075u);
    /* 004c8ed0 */
    goto mi_004c9211;
mi_004c8ed5:;
    /* 004c8ed5 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8ed7 */
    if(!(f->flags&MI_ZF))goto mi_004c8ef5;
mi_004c8ed9:;
    /* 004c8ed9 */
    a=MI_U16((0x007848e4u));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004c8ee0 */
    if((f->flags&MI_ZF))goto mi_004c9211;
    /* 004c8ee6 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000076u);
    /* 004c8ef0 */
    goto mi_004c9211;
mi_004c8ef5:;
    /* 004c8ef5 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004c8ef8 */
    if(!(f->flags&MI_ZF))goto mi_004c8f16;
mi_004c8efa:;
    /* 004c8efa */
    a=MI_U16((0x007848e6u));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004c8f01 */
    if((f->flags&MI_ZF))goto mi_004c9211;
    /* 004c8f07 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000077u);
    /* 004c8f11 */
    goto mi_004c9211;
mi_004c8f16:;
    /* 004c8f16 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c8f19 */
    if(!(f->flags&MI_ZF))goto mi_004c8e37;
mi_004c8f1f:;
    /* 004c8f1f */
    a=MI_U16((0x007848e8u));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004c8f26 */
    if((f->flags&MI_ZF))goto mi_004c9211;
    /* 004c8f2c */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000078u);
    /* 004c8f36 */
    goto mi_004c9211;
mi_004c8f3b:;
    /* 004c8f3b */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004c8f3e */
    if(!(f->flags&MI_ZF))goto mi_004c9008;
    /* 004c8f44 */
    f->r[1]=MI_U32((0x00eadd50u));
    /* 004c8f4a */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c8f4d */
    if(!(f->flags&MI_ZF))goto mi_004c8f56;
    /* 004c8f4f */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c8f54 */
    goto mi_004c8f6c;
mi_004c8f56:;
    /* 004c8f56 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004c8f59 */
    if(!(f->flags&MI_ZF))goto mi_004c8f62;
    /* 004c8f5b */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c8f60 */
    goto mi_004c8f6c;
mi_004c8f62:;
    /* 004c8f62 */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c8f67 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004c8f6a */
    if(!(f->flags&MI_ZF))goto mi_004c8f8b;
mi_004c8f6c:;
    /* 004c8f6c */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c8f6f */
    if(mi_signed_less(f->flags))goto mi_004c9211;
    /* 004c8f75 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c8f78 */
    if(!mi_signed_less(f->flags))goto mi_004c9211;
mi_004c8f7e:;
    /* 004c8f7e */
    a=f->r[0];b=0xfffffff7u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004c8f81:;
    /* 004c8f81 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8f86 */
    goto mi_004c9211;
mi_004c8f8b:;
    /* 004c8f8b */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c8f8e */
    if(mi_signed_less(f->flags))goto mi_004c8fa2;
    /* 004c8f90 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c8f93 */
    if(!mi_signed_less(f->flags))goto mi_004c8fa2;
    /* 004c8f95 */
    a=f->r[0];b=0xfffffff7u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c8f98 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8f9d */
    goto mi_004c9211;
mi_004c8fa2:;
    /* 004c8fa2 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 004c8fa5 */
    if(mi_signed_less(f->flags))goto mi_004c8fbd;
    /* 004c8fa7 */
    a=f->r[0];b=0x00000037u;t=mi_alu(f,a,b,32,1);
    /* 004c8faa */
    if(!mi_signed_less(f->flags))goto mi_004c8fbd;
    /* 004c8fac */
    f->r[0]=MI_U32((0x005e0274u+f->r[0]*4u));
    /* 004c8fb3 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c8fb8 */
    goto mi_004c9211;
mi_004c8fbd:;
    /* 004c8fbd */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c8fbf */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8fc5 */
    f->r[0]=MI_U32((0x00eadd6cu));
    /* 004c8fca */
    f->r[1]=f->r[0];
    /* 004c8fcc */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c8fcf */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c8fd1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[1]*8u)))&0xffffu)<<0);
    /* 004c8fd9 */
    a=(f->r[0]&0xffffu);b=0x0000000au;t=mi_alu(f,a,b,16,1);
    /* 004c8fdd */
    if(mi_signed_less(f->flags))goto mi_004c884f;
    /* 004c8fe3 */
    a=(f->r[0]&0xffffu);b=0x0000001du;t=mi_alu(f,a,b,16,1);
    /* 004c8fe7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004c884f;
    /* 004c8fed */
    a=MI_U32((0x007056c8u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c8ff3 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c8ff9 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x0000007bu);
    /* 004c9003 */
    goto mi_004c9211;
mi_004c9008:;
    /* 004c9008 */
    a=(f->r[2]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004c900b */
    if((f->flags&MI_ZF))goto mi_004c8855;
    /* 004c9011 */
    a=(f->r[2]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 004c9014 */
    if((f->flags&MI_ZF))goto mi_004c8855;
    /* 004c901a */
    a=(f->r[2]&0xffu);b=0x00000014u;t=mi_alu(f,a,b,8,1);
    /* 004c901d */
    if(!(f->flags&MI_ZF))goto mi_004c9076;
    /* 004c901f */
    f->r[2]=(0x00000013u+f->r[4]);
    /* 004c9023 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9024 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9029u;f->boundary(0x004c9024u,0x004c9340u,f);
    /* 004c9029 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c902c */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c902e */
    if((f->flags&MI_ZF))goto mi_004c903a;
    /* 004c9030 */
    a=f->r[0];b=0x00000085u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c9035 */
    goto mi_004c90de;
mi_004c903a:;
    /* 004c903a */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c903f */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004c9042 */
    if(!(f->flags&MI_ZF))goto mi_004c905f;
    /* 004c9044 */
    f->r[6]=MI_U32((0x00e92e0cu));
    /* 004c904a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004c9050 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000046u);
    /* 004c905a */
    goto mi_004c9211;
mi_004c905f:;
    /* 004c905f */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c9062 */
    if(mi_signed_less(f->flags))goto mi_004c876c;
    /* 004c9068 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c906b */
    if(!mi_signed_less(f->flags))goto mi_004c876c;
    /* 004c9071 */
    a=f->r[0];b=0xfffffff7u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c9074 */
    goto mi_004c90de;
mi_004c9076:;
    /* 004c9076 */
    a=(f->r[2]&0xffu);b=0x00000015u;t=mi_alu(f,a,b,8,1);
    /* 004c9079 */
    if(!(f->flags&MI_ZF))goto mi_004c9128;
    /* 004c907f */
    f->r[1]=(0x00000013u+f->r[4]);
    /* 004c9083 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9084 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9089u;f->boundary(0x004c9084u,0x004c95d0u,f);
    /* 004c9089 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c908c */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c908e */
    if((f->flags&MI_ZF))goto mi_004c9097;
    /* 004c9090 */
    a=f->r[0];b=0x00000085u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c9095 */
    goto mi_004c90de;
mi_004c9097:;
    /* 004c9097 */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c909c */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004c909f */
    if(!(f->flags&MI_ZF))goto mi_004c90bc;
    /* 004c90a1 */
    f->r[6]=MI_U32((0x00e92e0cu));
    /* 004c90a7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004c90ad */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000046u);
    /* 004c90b7 */
    goto mi_004c9211;
mi_004c90bc:;
    /* 004c90bc */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c90bf */
    if(mi_signed_less(f->flags))goto mi_004c90cb;
    /* 004c90c1 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004c90c4 */
    if(!mi_signed_less(f->flags))goto mi_004c90cb;
    /* 004c90c6 */
    a=f->r[0];b=0xfffffff7u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c90c9 */
    goto mi_004c90de;
mi_004c90cb:;
    /* 004c90cb */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c90d0 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c90d2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004c90f4;
    /* 004c90d4 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004c90d7 */
    if(!mi_signed_less(f->flags))goto mi_004c90f4;
    /* 004c90d9 */
    a=f->r[0];b=0x0000009du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004c90de:;
    /* 004c90de */
    f->r[6]=MI_U32((0x00e92e0cu));
    /* 004c90e4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004c90ea */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[0]);
    /* 004c90ef */
    goto mi_004c9211;
mi_004c90f4:;
    /* 004c90f4 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c90f7 */
    if(!(f->flags&MI_ZF))goto mi_004c9114;
    /* 004c90f9 */
    f->r[6]=MI_U32((0x00e92e0cu));
    /* 004c90ff */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004c9105 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x000000a6u);
    /* 004c910f */
    goto mi_004c9211;
mi_004c9114:;
    /* 004c9114 */
    f->r[2]=MI_U32((0x00e92e0cu));
    /* 004c911a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c911b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c911c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c911d */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[2]);
    /* 004c9123 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9125 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9126 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c9127 */
    f->r[4]+=4;return;
mi_004c9128:;
    /* 004c9128 */
    a=(f->r[2]&0xffu);b=0x00000011u;t=mi_alu(f,a,b,8,1);
    /* 004c912b */
    if(!(f->flags&MI_ZF))goto mi_004c9187;
    /* 004c912d */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c9132 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c9135 */
    if(!(f->flags&MI_ZF))goto mi_004c9146;
    /* 004c9137 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000001u);
    /* 004c9141 */
    goto mi_004c9211;
mi_004c9146:;
    /* 004c9146 */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004c9149 */
    if((f->flags&MI_ZF))goto mi_004c920b;
    /* 004c914f */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004c9154 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c9156 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c915c */
    f->r[1]=MI_U32((0x00e95120u));
    /* 004c9162 */
    f->r[0]=(0xffffffffu+f->r[1]+f->r[0]*1u);
    /* 004c9166 */
    f->r[1]=MI_U32((0x00e951ecu));
    /* 004c916c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004c916e */
    MI_U32((0x00e951e8u))=(uint32_t)(f->r[0]);
    /* 004c9173 */
    if(mi_signed_less(f->flags))goto mi_004c9211;
    /* 004c9179 */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[6]);
    /* 004c917f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9180 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9181 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9182 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9184 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9185 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c9186 */
    f->r[4]+=4;return;
mi_004c9187:;
    /* 004c9187 */
    a=(f->r[2]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004c918a */
    if(!(f->flags&MI_ZF))goto mi_004c91eb;
    /* 004c918c */
    f->r[0]=MI_U32((0x007056b8u));
    /* 004c9191 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004c9194 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c919a */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004c919d */
    if(!(f->flags&MI_ZF))goto mi_004c91bf;
    /* 004c919f */
    f->r[0]=MI_U32((0x00d3b124u));
    /* 004c91a4 */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[6]);
    /* 004c91aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c91ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c91ac */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c91af */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c91b2 */
    a=MI_U16((0x00e209b8u+f->r[2]));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004c91b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c91ba */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c91bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c91bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c91be */
    f->r[4]+=4;return;
mi_004c91bf:;
    /* 004c91bf */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004c91c2 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c91c8 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004c91cb */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c91d1 */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 004c91d4 */
    if((f->flags&MI_ZF))goto mi_004c884f;
    /* 004c91da */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[6]);
    /* 004c91e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c91e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c91e2 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004c91e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c91e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c91e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c91e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c91ea */
    f->r[4]+=4;return;
mi_004c91eb:;
    /* 004c91eb */
    a=(f->r[2]&0xffu);b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004c91ee */
    if(!(f->flags&MI_ZF))goto mi_004c884f;
    /* 004c91f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c91f9u;f->boundary(0x004c91f4u,0x004c92a0u,f);
    /* 004c91f9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004c91fb */
    if((f->flags&MI_ZF))goto mi_004c876c;
    /* 004c9201 */
    a=f->r[0];b=0x00000085u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c9206 */
    goto mi_004c90de;
mi_004c920b:;
    /* 004c920b */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[3]);
mi_004c9211:;
    /* 004c9211 */
    a=MI_U32((0x00e38eacu));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004c9217 */
    if((f->flags&MI_ZF))goto mi_004c922d;
    /* 004c9219 */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[6]);
    /* 004c921f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9220 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[5]);
    /* 004c9226 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9227 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9228 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c922a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c922b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c922c */
    f->r[4]+=4;return;
mi_004c922d:;
    /* 004c922d */
    a=f->r[7];b=0x00000096u;t=mi_alu(f,a,b,32,1);
    /* 004c9233 */
    if(!mi_signed_less(f->flags))goto mi_004c9243;
    /* 004c9235 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9236 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(f->r[5]);
    /* 004c923c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c923d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c923e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9240 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9241 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c9242 */
    f->r[4]+=4;return;
mi_004c9243:;
    /* 004c9243 */
    f->r[0]=MI_U32((0x00d3b13cu));
    /* 004c9248 */
    f->r[1]=MI_U32((0x00d3b130u));
    /* 004c924e */
    MI_U32((0x00d3a6b8u))=(uint32_t)(f->r[0]);
    /* 004c9253 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9255 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[2]&0xffu))&0xffu)<<0);
    /* 004c9257 */
    MI_U32((0x00d3a6bcu))=(uint32_t)(f->r[1]);
    /* 004c925d */
    f->r[1]=MI_U32((0x00d3b138u));
    /* 004c9263 */
    MI_U32((0x00d3a6c4u))=(uint32_t)(f->r[0]);
    /* 004c9268 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38dcau)))&0xffu)<<0);
    /* 004c926d */
    MI_U32((0x00d3a6b4u))=(uint32_t)(f->r[1]);
    /* 004c9273 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c9275 */
    MI_U32((0x00d3a6e0u))=(uint32_t)(f->r[5]);
    /* 004c927b */
    MI_U32((0x00d3a6c0u))=(uint32_t)(f->r[5]);
    /* 004c9281 */
    if(!(f->flags&MI_ZF))goto mi_004c9289;
    /* 004c9283 */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[3]&0xffu));
mi_004c9289:;
    /* 004c9289 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c928a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c928b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c928c */
    f->r[0]=0x00000001u;
    /* 004c9291 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9292 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c9293 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004ca930(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ca930 */
    f->r[0]=MI_U32((0x00d3a6e4u));
    /* 004ca935 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ca937 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004ca93c;
    /* 004ca939 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ca93b */
    f->r[4]+=4;return;
mi_004ca93c:;
    /* 004ca93c */
    f->r[0]=MI_U32((0x00d3a6bcu));
    /* 004ca941 */
    f->r[1]=MI_U32((0x00d3a6b8u));
    /* 004ca947 */
    f->r[2]=MI_U32((0x00d3a6b4u));
    /* 004ca94d */
    MI_U32((0x00d3b130u))=(uint32_t)(f->r[0]);
    /* 004ca952 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38dcau)))&0xffu)<<0);
    /* 004ca957 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000000u);
    /* 004ca961 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ca963 */
    MI_U32((0x00d3b13cu))=(uint32_t)(f->r[1]);
    /* 004ca969 */
    MI_U32((0x00d3b138u))=(uint32_t)(f->r[2]);
    /* 004ca96f */
    if(!(f->flags&MI_ZF))goto mi_004ca978;
    /* 004ca971 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000002u);
mi_004ca978:;
    /* 004ca978 */
    f->r[0]=0x00000001u;
    /* 004ca97d */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004ca980(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ca980 */
    f->r[0]=MI_U32((0x00e92e0cu));
    /* 004ca985 */
    MI_U32((0x00d3a6e4u))=(uint32_t)(0x00000000u);
    /* 004ca98f */
    MI_U32((0x00d3a6c8u))=(uint32_t)(f->r[0]);
    /* 004ca994 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004cabd0(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004cabd0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cabd1 */
    f->r[0]=MI_U32((0x00d3a6c0u));
    /* 004cabd6 */
    MI_U8((0x00000003u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004cabdb */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004cabde */
    if(!(f->flags&MI_ZF))goto mi_004cabe7;
    /* 004cabe0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cabe5u;f->boundary(0x004cabe0u,0x004caf10u,f);
    /* 004cabe5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004cabe6 */
    f->r[4]+=4;return;
mi_004cabe7:;
    /* 004cabe7 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004cabea */
    if(!(f->flags&MI_ZF))goto mi_004cabf3;
    /* 004cabec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cabf1u;f->boundary(0x004cabecu,0x004cb0b0u,f);
    /* 004cabf1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004cabf2 */
    f->r[4]+=4;return;
mi_004cabf3:;
    /* 004cabf3 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004cabf6 */
    if(!(f->flags&MI_ZF))goto mi_004cabff;
    /* 004cabf8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cabfdu;f->boundary(0x004cabf8u,0x004cb300u,f);
    /* 004cabfd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004cabfe */
    f->r[4]+=4;return;
mi_004cabff:;
    /* 004cabff */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004cac02 */
    if(!(f->flags&MI_ZF))goto mi_004cac0b;
    /* 004cac04 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cac09u;f->boundary(0x004cac04u,0x004ca9a0u,f);
    /* 004cac09 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004cac0a */
    f->r[4]+=4;return;
mi_004cac0b:;
    /* 004cac0b */
    f->r[0]=MI_U32((0x00f56f84u));
    /* 004cac10 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cac11 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cac12 */
    MI_U32((0x00d3b168u))=(uint32_t)(f->r[0]);
    /* 004cac17 */
    MI_U32((0x00d3b164u))=(uint32_t)(f->r[0]);
    /* 004cac1c */
    f->r[0]=MI_U32((0x00e92dfcu));
    /* 004cac21 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cac22 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004cac24 */
    f->r[3]=0x0000001eu;
    /* 004cac29 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cac2c */
    MI_U32((0x00d3a6d0u))=(uint32_t)(0x000000c8u);
    /* 004cac36 */
    MI_U32((0x00d3b130u))=(uint32_t)(f->r[7]);
    /* 004cac3c */
    MI_U32((0x00d3b13cu))=(uint32_t)(f->r[3]);
    /* 004cac42 */
    MI_U16((0x00d3b51cu))=(uint16_t)(0x00001c82u);
    /* 004cac4b */
    if(!(f->flags&MI_ZF))goto mi_004cac56;
    /* 004cac4d */
    MI_U16((0x00d3b51cu))=(uint16_t)(0x00003922u);
mi_004cac56:;
    /* 004cac56 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004cac5b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004cac5d */
    a=(f->r[0]&0xffu);b=0x00000014u;t=mi_alu(f,a,b,8,1);
    /* 004cac5f */
    if(!(f->flags&MI_ZF))goto mi_004cac78;
    /* 004cac61 */
    f->r[0]=MI_U32((0x00784f14u));
    /* 004cac66 */
    f->r[1]=(0x0000000fu+f->r[4]);
    /* 004cac6a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cac6b */
    f->r[6]=(0xffffffffu+f->r[0]);
    /* 004cac6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cac73u;f->boundary(0x004cac6eu,0x004c9340u,f);
    /* 004cac73 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cac76 */
    goto mi_004cac9f;
mi_004cac78:;
    /* 004cac78 */
    a=(f->r[0]&0xffu);b=0x00000015u;t=mi_alu(f,a,b,8,1);
    /* 004cac7a */
    if(!(f->flags&MI_ZF))goto mi_004cac91;
    /* 004cac7c */
    f->r[6]=MI_U32((0x0077591cu));
    /* 004cac82 */
    f->r[2]=(0x0000000fu+f->r[4]);
    /* 004cac86 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cac87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cac8cu;f->boundary(0x004cac87u,0x004c95d0u,f);
    /* 004cac8c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cac8f */
    goto mi_004cac9f;
mi_004cac91:;
    /* 004cac91 */
    a=(f->r[0]&0xffu);b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004cac93 */
    if(!(f->flags&MI_ZF))goto mi_004cacd8;
    /* 004cac95 */
    MI_U8((0x0000000fu+f->r[4]))=(uint8_t)(0x00000001u);
    /* 004cac9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cac9fu;f->boundary(0x004cac9au,0x004c92a0u,f);
mi_004cac9f:;
    /* 004cac9f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004caca1 */
    if((f->flags&MI_ZF))goto mi_004cacd8;
    /* 004caca3 */
    a=f->r[0];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 004caca6 */
    if(!(f->flags&MI_ZF))goto mi_004cacd3;
    /* 004caca8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000fu+f->r[4])))&0xffu)<<0);
    /* 004cacac */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004cacae */
    if(!(f->flags&MI_ZF))goto mi_004caccd;
    /* 004cacb0 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004cacb3 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004cacb6 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cacb8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004cacba */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[0]*2u)))&0xffu)<<0);
    /* 004cacc1 */
    f->r[2]=MI_U32((0x005d2a3cu+f->r[1]*8u));
    /* 004cacc8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cacc9 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caccb */
    goto mi_004cace0;
mi_004caccd:;
    /* 004caccd */
    a=0x00000026u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caccf */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cacd1 */
    goto mi_004cace0;
mi_004cacd3:;
    /* 004cacd3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cacd4 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cacd6 */
    goto mi_004cace0;
mi_004cacd8:;
    /* 004cacd8 */
    f->r[0]=MI_U32((0x00d3a6e4u));
    /* 004cacdd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cacde */
    a=0x00000044u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004cace0:;
    /* 004cace0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cace5u;f->boundary(0x004cace0u,0x004cc110u,f);
    /* 004cace5 */
    f->r[1]=MI_U32((0x00d3a6d0u));
    /* 004caceb */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004cacf1 */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004cacf6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cacf9 */
    a=f->r[1];b=0xffffffecu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cacfc */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004cacff */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad00 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad02 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad03 */
    f->r[1]=MI_U32((0x00e20768u));
    /* 004cad09 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cad0c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad0d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad0e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad0f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cad14u;f->boundary(0x004cad0fu,0x004c8070u,f);
    /* 004cad14 */
    f->r[0]=MI_U32((0x00d3b138u));
    /* 004cad19 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cad1c */
    f->r[1]=f->r[0];
    /* 004cad1e */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004cad21 */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cad24 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004cad27 */
    MI_U32((0x00d3a6ccu))=(uint32_t)(f->r[1]);
    /* 004cad2d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cad93;
    /* 004cad2f */
    f->r[2]=MI_U32((0x00d3a6e4u));
    /* 004cad35 */
    MI_U32((0x00d3a6d0u))=(uint32_t)(0x0000012cu);
    /* 004cad3f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad40 */
    a=0x00000044u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad42 */
    MI_U32((0x00d3b130u))=(uint32_t)(f->r[7]);
    /* 004cad48 */
    MI_U32((0x00d3b13cu))=(uint32_t)(f->r[3]);
    /* 004cad4e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cad53u;f->boundary(0x004cad4eu,0x004cc110u,f);
    /* 004cad53 */
    f->r[0]=MI_U32((0x00d3a6d0u));
    /* 004cad58 */
    f->r[1]=MI_U32((0x00d3a6d4u));
    /* 004cad5e */
    f->r[2]=MI_U32((0x00d3a6d8u));
    /* 004cad64 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad65 */
    a=f->r[0];b=0xffffffecu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cad68 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad6a */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cad6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad6e */
    f->r[0]=MI_U32((0x00e20768u));
    /* 004cad73 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004cad76 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad77 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad78 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cad79 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cad7eu;f->boundary(0x004cad79u,0x004c8070u,f);
    /* 004cad7e */
    f->r[1]=MI_U32((0x00d3b138u));
    /* 004cad84 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cad87 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004cad8a */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cad8d */
    MI_U32((0x00d3a6ccu))=(uint32_t)(f->r[1]);
mi_004cad93:;
    /* 004cad93 */
    f->r[3]=MI_U32((0x00d3a6d0u));
    /* 004cad99 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004cad9f */
    f->r[7]=MI_U32((0x00e38e2cu));
    /* 004cada5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cada6 */
    f->r[0]=(0x00000064u+f->r[2]+f->r[3]*1u);
    /* 004cadaa */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004cadac */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cadae */
    f->r[5]=MI_U32((0x00e38e28u));
    /* 004cadb4 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cadb7 */
    f->r[0]=f->r[2];
    /* 004cadb9 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004cadbf */
    f->r[6]=(0x000000c8u+f->r[2]);
    /* 004cadc5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004cadc7 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cadc9 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cadcc */
    f->r[6]=f->r[2];
    /* 004cadce */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004cadd4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cadd6 */
    if((f->flags&MI_ZF))goto mi_004cadec;
    /* 004cadd8 */
    a=(f->r[2]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004caddb */
    if(!(f->flags&MI_ZF))goto mi_004cade7;
    /* 004caddd */
    f->r[0]=(0x00000032u+f->r[7]);
    /* 004cade0 */
    MI_U32((0x00d3a6d8u))=(uint32_t)(f->r[0]);
    /* 004cade5 */
    goto mi_004cadfd;
mi_004cade7:;
    /* 004cade7 */
    f->r[0]=(0x00000014u+f->r[7]);
    /* 004cadea */
    goto mi_004cadf3;
mi_004cadec:;
    /* 004cadec */
    f->r[0]=f->r[7];
    /* 004cadee */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cadf0 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_004cadf3:;
    /* 004cadf3 */
    a=(f->r[2]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004cadf6 */
    MI_U32((0x00d3a6d8u))=(uint32_t)(f->r[0]);
    /* 004cadfb */
    if(!(f->flags&MI_ZF))goto mi_004cae36;
mi_004cadfd:;
    /* 004cadfd */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004cae03 */
    f->r[6]=(0x000001b0u+f->r[2]);
    /* 004cae09 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cae0b */
    if(!mi_signed_less(f->flags))goto mi_004cae74;
    /* 004cae0d */
    f->r[2]=MI_U32((0x00605a44u));
    /* 004cae13 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004cae16 */
    if(!(f->flags&MI_ZF))goto mi_004cae1d;
    /* 004cae18 */
    f->r[6]=(0xffffffb6u+f->r[5]);
    /* 004cae1b */
    goto mi_004cae74;
mi_004cae1d:;
    /* 004cae1d */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004cae20 */
    if(!(f->flags&MI_ZF))goto mi_004cae27;
    /* 004cae22 */
    f->r[6]=(0xffffffcau+f->r[5]);
    /* 004cae25 */
    goto mi_004cae74;
mi_004cae27:;
    /* 004cae27 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004cae2a */
    if(!(f->flags&MI_ZF))goto mi_004cae31;
    /* 004cae2c */
    f->r[6]=(0xffffffc6u+f->r[5]);
    /* 004cae2f */
    goto mi_004cae74;
mi_004cae31:;
    /* 004cae31 */
    f->r[6]=(0xffffffc0u+f->r[5]);
    /* 004cae34 */
    goto mi_004cae74;
mi_004cae36:;
    /* 004cae36 */
    a=(f->r[2]&0xffu);b=0x0000002du;t=mi_alu(f,a,b,8,1);
    /* 004cae39 */
    if(!(f->flags&MI_ZF))goto mi_004cae40;
    /* 004cae3b */
    f->r[6]=(0xffffffccu+f->r[5]);
    /* 004cae3e */
    goto mi_004cae74;
mi_004cae40:;
    /* 004cae40 */
    a=(f->r[2]&0xffu);b=0x0000002au;t=mi_alu(f,a,b,8,1);
    /* 004cae43 */
    if(!(f->flags&MI_ZF))goto mi_004cae6a;
    /* 004cae45 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004cae47 */
    f->r[6]=(0x00000028u+f->r[5]);
    /* 004cae4a */
    if(!(f->flags&MI_ZF))goto mi_004cae4f;
    /* 004cae4c */
    f->r[6]=(0xffffffb8u+f->r[5]);
mi_004cae4f:;
    /* 004cae4f */
    f->r[0]=f->r[3];
    /* 004cae51 */
    MI_U32((0x00d3a6d4u))=(uint32_t)(f->r[6]);
    /* 004cae57 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004cae58 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cae5a */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004cae5c */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004cae5e */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004cae61 */
    f->r[0]=f->r[7];
    /* 004cae63 */
    MI_U32((0x00d3a6d8u))=(uint32_t)(f->r[0]);
    /* 004cae68 */
    goto mi_004cae7a;
mi_004cae6a:;
    /* 004cae6a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004cae6c */
    f->r[6]=(0x00000028u+f->r[5]);
    /* 004cae6f */
    if(!(f->flags&MI_ZF))goto mi_004cae74;
    /* 004cae71 */
    f->r[6]=(0xffffffc2u+f->r[5]);
mi_004cae74:;
    /* 004cae74 */
    MI_U32((0x00d3a6d4u))=(uint32_t)(f->r[6]);
mi_004cae7a:;
    /* 004cae7a */
    f->r[2]=MI_U32((0x00e92dfcu));
    /* 004cae80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cae81 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004cae84 */
    if((f->flags&MI_ZF))goto mi_004cae92;
    /* 004cae86 */
    f->r[7]=0x00005dc8u;
    /* 004cae8b */
    f->r[2]=0x00002c40u;
    /* 004cae90 */
    goto mi_004cae9c;
mi_004cae92:;
    /* 004cae92 */
    f->r[7]=0x0000bb88u;
    /* 004cae97 */
    f->r[2]=0x000058a0u;
mi_004cae9c:;
    /* 004cae9c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cae9d */
    a=f->r[3];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004caea0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caea1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caea2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caea3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caea4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004caea9u;f->boundary(0x004caea4u,0x004cd680u,f);
    /* 004caea9 */
    f->r[0]=MI_U32((0x00d3a6ccu));
    /* 004caeae */
    f->r[1]=MI_U32((0x00d3a6d0u));
    /* 004caeb4 */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004caeba */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caebd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caebe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caebf */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004caec4 */
    a=f->r[1];b=0x00000012u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004caec7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004caec8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caec9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004caeca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caecb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caecc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004caed1u;f->boundary(0x004caeccu,0x004cd9d0u,f);
    /* 004caed1 */
    f->r[1]=MI_U32((0x00d3a6d0u));
    /* 004caed7 */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004caedd */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004caee2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caee4 */
    a=f->r[1];b=0xffffffecu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004caee7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caee9 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004caeec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caeed */
    f->r[1]=MI_U32((0x00e20768u));
    /* 004caef3 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caef6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caef7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caef8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caef9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004caefeu;f->boundary(0x004caef9u,0x004c8070u,f);
    /* 004caefe */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004caf01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004caf02 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004caf03 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004caf04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004caf05 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004caf10(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004caf10 */
    f->r[0]=MI_U32((0x00f56f84u));
    /* 004caf15 */
    f->r[1]=MI_U32((0x00d3a6d8u));
    /* 004caf1b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf1c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf1d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf1e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf1f */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004caf21 */
    MI_U32((0x00d3b168u))=(uint32_t)(f->r[0]);
    /* 004caf26 */
    MI_U32((0x00d3b164u))=(uint32_t)(f->r[0]);
    /* 004caf2b */
    f->r[0]=MI_U32((0x00d3a6d4u));
    /* 004caf30 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf31 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf33 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caf36 */
    a=0x000000beu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf3b */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004caf3e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf3f */
    f->r[6]=0x0000001eu;
    /* 004caf44 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf45 */
    a=0x00d3a4c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf4a */
    MI_U32((0x00d3a6d0u))=(uint32_t)(0x000000dcu);
    /* 004caf54 */
    MI_U32((0x00d3b130u))=(uint32_t)(f->r[7]);
    /* 004caf5a */
    MI_U32((0x00d3b13cu))=(uint32_t)(f->r[6]);
    /* 004caf60 */
    MI_U16((0x00d3b51cu))=(uint16_t)(0x00004208u);
    /* 004caf69 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004caf6eu;f->boundary(0x004caf69u,0x004c8070u,f);
    /* 004caf6e */
    f->r[1]=MI_U32((0x00d3b138u));
    /* 004caf74 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004caf77 */
    f->r[0]=f->r[1];
    /* 004caf79 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004caf7c */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caf7f */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004caf82 */
    MI_U32((0x00d3a6ccu))=(uint32_t)(f->r[0]);
    /* 004caf87 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cafd7;
    /* 004caf89 */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004caf8f */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004caf94 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf95 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf97 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004caf9a */
    a=0x0000010eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caf9f */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cafa2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cafa3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cafa4 */
    a=0x00d3a4c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cafa9 */
    MI_U32((0x00d3a6d0u))=(uint32_t)(0x0000012cu);
    /* 004cafb3 */
    MI_U32((0x00d3b130u))=(uint32_t)(f->r[7]);
    /* 004cafb9 */
    MI_U32((0x00d3b13cu))=(uint32_t)(f->r[6]);
    /* 004cafbf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cafc4u;f->boundary(0x004cafbfu,0x004c8070u,f);
    /* 004cafc4 */
    f->r[0]=MI_U32((0x00d3b138u));
    /* 004cafc9 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cafcc */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004cafcf */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cafd2 */
    MI_U32((0x00d3a6ccu))=(uint32_t)(f->r[0]);
mi_004cafd7:;
    /* 004cafd7 */
    f->r[5]=MI_U32((0x00d3a6d0u));
    /* 004cafdd */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004cafe3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004cafe5 */
    f->r[6]=(0x00000014u+f->r[5]);
    /* 004cafe8 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cafea */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cafed */
    f->r[7]=f->r[2];
    /* 004cafef */
    f->r[2]=MI_U32((0x00e38e30u));
    /* 004caff5 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004caff7 */
    a=f->r[2];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 004caffd */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cb000 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004cb002 */
    if((f->flags&MI_ZF))goto mi_004cb009;
    /* 004cb004 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cb007 */
    goto mi_004cb00e;
mi_004cb009:;
    /* 004cb009 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004cb00b */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
mi_004cb00e:;
    /* 004cb00e */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004cb010 */
    MI_U32((0x00d3a6d8u))=(uint32_t)(f->r[1]);
    /* 004cb016 */
    f->r[3]=(0x00000032u+f->r[2]);
    /* 004cb019 */
    if(!(f->flags&MI_ZF))goto mi_004cb01e;
    /* 004cb01b */
    f->r[3]=(0xffffffb8u+f->r[2]);
mi_004cb01e:;
    /* 004cb01e */
    f->r[2]=MI_U32((0x00e92dfcu));
    /* 004cb024 */
    MI_U32((0x00d3a6d4u))=(uint32_t)(f->r[3]);
    /* 004cb02a */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004cb02d */
    if((f->flags&MI_ZF))goto mi_004cb03b;
    /* 004cb02f */
    f->r[7]=0x00005dc8u;
    /* 004cb034 */
    f->r[2]=0x00002c40u;
    /* 004cb039 */
    goto mi_004cb045;
mi_004cb03b:;
    /* 004cb03b */
    f->r[7]=0x0000bb88u;
    /* 004cb040 */
    f->r[2]=0x000058a0u;
mi_004cb045:;
    /* 004cb045 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb046 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb047 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb048 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb049 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb04a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb04fu;f->boundary(0x004cb04au,0x004cd680u,f);
    /* 004cb04f */
    f->r[0]=MI_U32((0x00d3a6ccu));
    /* 004cb054 */
    f->r[1]=MI_U32((0x00d3a6d0u));
    /* 004cb05a */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004cb060 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cb063 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb064 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb065 */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004cb06a */
    a=f->r[1];b=0x00000012u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cb06d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004cb06e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb06f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004cb070 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb071 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb072 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb077u;f->boundary(0x004cb072u,0x004cd9d0u,f);
    /* 004cb077 */
    f->r[1]=MI_U32((0x00d3a6d0u));
    /* 004cb07d */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004cb083 */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004cb088 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb08a */
    a=f->r[1];b=0xffffffe2u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cb08d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb08f */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004cb092 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb093 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cb096 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb097 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb098 */
    a=0x00d3a4c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb09d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb0a2u;f->boundary(0x004cb09du,0x004c8070u,f);
    /* 004cb0a2 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb0a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cb0a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cb0a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cb0a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cb0a9 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004c92a0(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c92a0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92a1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92a2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92a3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92a4 */
    f->r[3]=0x00576104u;
mi_004c92a9:;
    /* 004c92a9 */
    f->r[6]=MI_U32((0xffffffbcu+f->r[3]));
    /* 004c92ac */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004c92b2 */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 004c92b4 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92b6 */
    f->r[0]=(0x00000000u+f->r[6]*8u);
    /* 004c92bd */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92bf */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c92c1 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c92c4 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004c92c9 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c92cc */
    f->r[2]=(0x00000015u+f->r[7]+f->r[1]*1u);
    /* 004c92d0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92d1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92d2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c92d7u;f->boundary(0x004c92d2u,0x004cf620u,f);
    /* 004c92d7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c92da */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c92dc */
    if(!(f->flags&MI_ZF))goto mi_004c9324;
    /* 004c92de */
    a=f->r[5];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 004c92e1 */
    if((f->flags&MI_ZF))goto mi_004c9312;
    /* 004c92e3 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004c92e5 */
    if(mi_signed_less(f->flags))goto mi_004c9312;
    /* 004c92e7 */
    a=f->r[5];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c92ea */
    if(!mi_signed_less(f->flags))goto mi_004c9312;
    /* 004c92ec */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004c92f1 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004c92f7 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c92f9 */
    a=f->r[0];b=0x00000086u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c92fe */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9300 */
    f->r[2]=(0x00000015u+f->r[7]+f->r[1]*1u);
    /* 004c9304 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9305 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9306 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c930bu;f->boundary(0x004c9306u,0x004cf620u,f);
    /* 004c930b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c930e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c9310 */
    if(!(f->flags&MI_ZF))goto mi_004c932b;
mi_004c9312:;
    /* 004c9312 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c9315 */
    a=f->r[3];b=0x00576144u;t=mi_alu(f,a,b,32,1);
    /* 004c931b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004c92a9;
    /* 004c931d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c931e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c931f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9320 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9322 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9323 */
    f->r[4]+=4;return;
mi_004c9324:;
    /* 004c9324 */
    f->r[0]=f->r[6];
    /* 004c9326 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9327 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9328 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9329 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c932a */
    f->r[4]+=4;return;
mi_004c932b:;
    /* 004c932b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c932c */
    f->r[0]=f->r[5];
    /* 004c932e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c932f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9330 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9331 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004c9340(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c9340 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c9343 */
    f->r[0]=MI_U32((0x00784f14u));
    /* 004c9348 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9349 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c934a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c934b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c934f */
    f->r[0]=MI_U32((0x00784f18u));
    /* 004c9354 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9355 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9356 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004c9359 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004c935c */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c935e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004c9360 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9362 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c9364 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de02u+f->r[1])))&0xffu)<<0);
    /* 004c936a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c936b */
    if((f->flags&MI_ZF))goto mi_004c937b;
    /* 004c936d */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c9370 */
    if((f->flags&MI_ZF))goto mi_004c937b;
    /* 004c9372 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c9375 */
    if(!(f->flags&MI_ZF))goto mi_004c95bd;
mi_004c937b:;
    /* 004c937b */
    f->r[2]=MI_U32((0x00e38db0u));
    /* 004c9381 */
    f->r[0]=(0xfffffe0cu+f->r[2]);
    /* 004c9387 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c9388 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c938a */
    f->r[3]=f->r[0];
    /* 004c938c */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004c9391 */
    a=f->r[3];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[3]=t;
    /* 004c9393 */
    f->r[7]=(0xffffff94u+f->r[0]);
    /* 004c9396 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de05u+f->r[1])))&0xffu)<<0);
    /* 004c939c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c939e */
    if(!(f->flags&MI_ZF))goto mi_004c948c;
    /* 004c93a4 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004c93a6 */
    f->r[6]=0x00000001u;
    /* 004c93ab */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[5]);
mi_004c93b1:;
    /* 004c93b1 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004c93b5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93b6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c93bcu;f->boundary(0x004c93b7u,0x00443000u,f);
    /* 004c93bc */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c93bf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c93c1 */
    if((f->flags&MI_ZF))goto mi_004c93fd;
    /* 004c93c3 */
    f->r[2]=MI_U32((0x00784f18u));
    /* 004c93c9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93ca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93cb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c93d0u;f->boundary(0x004c93cbu,0x00447790u,f);
    /* 004c93d0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c93d3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c93d5 */
    if((f->flags&MI_ZF))goto mi_004c93fd;
    /* 004c93d7 */
    f->r[1]=MI_U32((0x00e20778u));
    /* 004c93dd */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93df */
    f->r[0]=(0x0000001cu+f->r[7]);
    /* 004c93e2 */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93e4 */
    f->r[2]=(0x00000064u+f->r[1]+f->r[3]*1u);
    /* 004c93e8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93e9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c93ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c93efu;f->boundary(0x004c93eau,0x004cf620u,f);
    /* 004c93ef */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c93f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c93f4 */
    if(!(f->flags&MI_ZF))goto mi_004c946a;
    /* 004c93f6 */
    a=MI_U32((0x00e20778u));b=0x00000020u;t=mi_alu(f,a,b,32,0);MI_U32((0x00e20778u))=(uint32_t)(t);
mi_004c93fd:;
    /* 004c93fd */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c93fe */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c9401 */
    if(mi_signed_less(f->flags))goto mi_004c93b1;
    /* 004c9403 */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[5]);
    /* 004c9409 */
    f->r[6]=0x00000001u;
mi_004c940e:;
    /* 004c940e */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004c9412 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9413 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9414 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9419u;f->boundary(0x004c9414u,0x00443040u,f);
    /* 004c9419 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c941c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c941e */
    if((f->flags&MI_ZF))goto mi_004c945a;
    /* 004c9420 */
    f->r[2]=MI_U32((0x00784f18u));
    /* 004c9426 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9427 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9428 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c942du;f->boundary(0x004c9428u,0x00447790u,f);
    /* 004c942d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9430 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c9432 */
    if((f->flags&MI_ZF))goto mi_004c945a;
    /* 004c9434 */
    f->r[1]=MI_U32((0x00e20778u));
    /* 004c943a */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c943c */
    f->r[0]=(0x00000032u+f->r[7]);
    /* 004c943f */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9441 */
    f->r[2]=(0x00000064u+f->r[1]+f->r[3]*1u);
    /* 004c9445 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9446 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9447 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c944cu;f->boundary(0x004c9447u,0x004cf620u,f);
    /* 004c944c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c944f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c9451 */
    if(!(f->flags&MI_ZF))goto mi_004c947b;
    /* 004c9453 */
    a=MI_U32((0x00e20778u));b=0x00000020u;t=mi_alu(f,a,b,32,0);MI_U32((0x00e20778u))=(uint32_t)(t);
mi_004c945a:;
    /* 004c945a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c945b */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c945e */
    if(mi_signed_less(f->flags))goto mi_004c940e;
    /* 004c9460 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9461 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9462 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9463 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9465 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9466 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9469 */
    f->r[4]+=4;return;
mi_004c946a:;
    /* 004c946a */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004c946e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c946f */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004c9472 */
    f->r[0]=f->r[6];
    /* 004c9474 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9475 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9476 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9477 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c947a */
    f->r[4]+=4;return;
mi_004c947b:;
    /* 004c947b */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004c947f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9480 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 004c9483 */
    f->r[0]=f->r[6];
    /* 004c9485 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9486 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9487 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9488 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c948b */
    f->r[4]+=4;return;
mi_004c948c:;
    /* 004c948c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c948e */
    a=f->r[7];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c9491 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c9495 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004c949d */
    f->r[5]=f->r[7];
mi_004c949f:;
    /* 004c949f */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004c94a3 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004c94a7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94a8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c94aeu;f->boundary(0x004c94a9u,0x00443000u,f);
    /* 004c94ae */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c94b1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c94b3 */
    if((f->flags&MI_ZF))goto mi_004c950b;
    /* 004c94b5 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c94b9 */
    f->r[1]=MI_U32((0x00784f18u));
    /* 004c94bf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94c0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94c1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c94c6u;f->boundary(0x004c94c1u,0x00447790u,f);
    /* 004c94c6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c94c9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c94cb */
    if((f->flags&MI_ZF))goto mi_004c950b;
    /* 004c94cd */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004c94d0 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94d2 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94d4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94d5 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c94d8 */
    f->r[2]=(0x00000000u+f->r[3]+f->r[0]*8u);
    /* 004c94db */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c94dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c94de */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c94e3u;f->boundary(0x004c94deu,0x004cf620u,f);
    /* 004c94e3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c94e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c94e8 */
    if(!(f->flags&MI_ZF))goto mi_004c9599;
    /* 004c94ee */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004c94f2 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c94f3 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c94f4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c94f8 */
    a=f->r[0];b=0x80000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004c94fd */
    if(!(f->flags&MI_SF))goto mi_004c9504;
    /* 004c94ff */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c9500 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004c9503 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_004c9504:;
    /* 004c9504 */
    if(!(f->flags&MI_ZF))goto mi_004c950b;
    /* 004c9506 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c9508 */
    a=f->r[5];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_004c950b:;
    /* 004c950b */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c950f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c9510 */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c9513 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c9517 */
    if(mi_signed_less(f->flags))goto mi_004c949f;
    /* 004c9519 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c951b */
    f->r[5]=0x00000001u;
    /* 004c9520 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004c9524:;
    /* 004c9524 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004c9528 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9529 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c952a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c952fu;f->boundary(0x004c952au,0x00443040u,f);
    /* 004c952f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9532 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c9534 */
    if((f->flags&MI_ZF))goto mi_004c9589;
    /* 004c9536 */
    f->r[2]=MI_U32((0x00784f18u));
    /* 004c953c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c953d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c953e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9543u;f->boundary(0x004c953eu,0x00447790u,f);
    /* 004c9543 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9546 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c9548 */
    if((f->flags&MI_ZF))goto mi_004c9589;
    /* 004c954a */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004c954d */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c954f */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9551 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9552 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c9555 */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*8u);
    /* 004c9558 */
    f->r[2]=(0x00000118u+f->r[0]+f->r[1]*1u);
    /* 004c955f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9560 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9565u;f->boundary(0x004c9560u,0x004cf620u,f);
    /* 004c9565 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9568 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c956a */
    if(!(f->flags&MI_ZF))goto mi_004c95ac;
    /* 004c956c */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004c9570 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c9571 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c9572 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c9576 */
    a=f->r[0];b=0x80000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004c957b */
    if(!(f->flags&MI_SF))goto mi_004c9582;
    /* 004c957d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c957e */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004c9581 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_004c9582:;
    /* 004c9582 */
    if(!(f->flags&MI_ZF))goto mi_004c9589;
    /* 004c9584 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c9586 */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004c9589:;
    /* 004c9589 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004c958a */
    a=f->r[5];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c958d */
    if(mi_signed_less(f->flags))goto mi_004c9524;
    /* 004c958f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9590 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9591 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c9592 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9594 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9595 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9598 */
    f->r[4]+=4;return;
mi_004c9599:;
    /* 004c9599 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004c959d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c959e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c959f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c95a0 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004c95a3 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004c95a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c95a8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c95ab */
    f->r[4]+=4;return;
mi_004c95ac:;
    /* 004c95ac */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004c95b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c95b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c95b2 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 004c95b5 */
    f->r[0]=f->r[5];
    /* 004c95b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c95b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c95b9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c95bc */
    f->r[4]+=4;return;
mi_004c95bd:;
    /* 004c95bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c95be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c95bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c95c0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c95c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c95c3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c95c6 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004c95d0(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c95d0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c95d1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c95d2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c95d3 */
    f->r[5]=MI_U32((0x0077591cu));
    /* 004c95d9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c95da */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c95db */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004c95df */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c95e2 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c95e4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0077f760u+f->r[0]*2u)))&0xffu)<<0);
    /* 004c95eb */
    f->r[0]=f->r[1];
    /* 004c95ed */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004c95f2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c95f3 */
    if((f->flags&MI_ZF))goto mi_004c9603;
    /* 004c95f5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c95f8 */
    if((f->flags&MI_ZF))goto mi_004c9603;
    /* 004c95fa */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c95fd */
    if(!(f->flags&MI_ZF))goto mi_004c96d5;
mi_004c9603:;
    /* 004c9603 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004c960b */
    f->r[7]=0x005c404au;
mi_004c9610:;
    /* 004c9610 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 004c9613 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[7]));
    /* 004c9617 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c961b */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004c9621 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9622 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9623 */
    a=f->r[6];b=0x000000dfu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c9629 */
    f->r[3]=(0xffffff8eu+f->r[1]+f->r[2]*1u);
    /* 004c962d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9632u;f->boundary(0x004c962du,0x00443000u,f);
    /* 004c9632 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9635 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c9637 */
    if((f->flags&MI_ZF))goto mi_004c964e;
    /* 004c9639 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c963b */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c963d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c963e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c963f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9644u;f->boundary(0x004c963fu,0x004cf620u,f);
    /* 004c9644 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9647 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c9649 */
    if(!(f->flags&MI_ZF))goto mi_004c96b5;
    /* 004c964b */
    a=f->r[7];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004c964e:;
    /* 004c964e */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c9652 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c9653 */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c9656 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c965a */
    if(mi_signed_less(f->flags))goto mi_004c9610;
    /* 004c965c */
    f->r[3]=0x00000001u;
    /* 004c9661 */
    f->r[7]=0x005c412au;
mi_004c9666:;
    /* 004c9666 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 004c9669 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004c966e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c966f */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[7]));
    /* 004c9673 */
    f->r[1]=(0xffffff8eu+f->r[2]+f->r[0]*1u);
    /* 004c9677 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9678 */
    a=f->r[6];b=0x000000dfu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c967e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c9682 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9687u;f->boundary(0x004c9682u,0x00443040u,f);
    /* 004c9687 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c968a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c968c */
    if((f->flags&MI_ZF))goto mi_004c96a7;
    /* 004c968e */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004c9692 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9694 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9696 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9697 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9698 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c969du;f->boundary(0x004c9698u,0x004cf620u,f);
    /* 004c969d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c96a0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c96a2 */
    if(!(f->flags&MI_ZF))goto mi_004c96c6;
    /* 004c96a4 */
    a=f->r[7];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004c96a7:;
    /* 004c96a7 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004c96a8 */
    a=f->r[3];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c96ab */
    if(mi_signed_less(f->flags))goto mi_004c9666;
    /* 004c96ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c96ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c96af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c96b0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c96b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c96b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c96b4 */
    f->r[4]+=4;return;
mi_004c96b5:;
    /* 004c96b5 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004c96b9 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c96bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c96be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c96bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c96c0 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004c96c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c96c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c96c5 */
    f->r[4]+=4;return;
mi_004c96c6:;
    /* 004c96c6 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004c96ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c96cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c96cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c96cd */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 004c96d0 */
    f->r[0]=f->r[3];
    /* 004c96d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c96d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c96d4 */
    f->r[4]+=4;return;
mi_004c96d5:;
    /* 004c96d5 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c96d7 */
    if(!(f->flags&MI_ZF))goto mi_004c972b;
    /* 004c96d9 */
    f->r[3]=0x00000001u;
    /* 004c96de */
    f->r[7]=0x005c3ec2u;
mi_004c96e3:;
    /* 004c96e3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 004c96e6 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004c96ec */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c96ed */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[7]));
    /* 004c96f1 */
    f->r[0]=(0xffffff8eu+f->r[1]+f->r[2]*1u);
    /* 004c96f5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c96f6 */
    a=f->r[6];b=0x000000dfu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c96fc */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c9700 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c9705u;f->boundary(0x004c9700u,0x00443000u,f);
    /* 004c9705 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c9708 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c970a */
    if((f->flags&MI_ZF))goto mi_004c9725;
    /* 004c970c */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004c9710 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9712 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9714 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9715 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c9716 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c971bu;f->boundary(0x004c9716u,0x004cf620u,f);
    /* 004c971b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c971e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c9720 */
    if(!(f->flags&MI_ZF))goto mi_004c9733;
    /* 004c9722 */
    a=f->r[7];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004c9725:;
    /* 004c9725 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004c9726 */
    a=f->r[3];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 004c9729 */
    if(mi_signed_less(f->flags))goto mi_004c96e3;
mi_004c972b:;
    /* 004c972b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c972c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c972d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c972e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c9730 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9731 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c9732 */
    f->r[4]+=4;return;
mi_004c9733:;
    /* 004c9733 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004c9737 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c9738 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c9739 */
    f->r[0]=f->r[3];
    /* 004c973b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c973c */
    MI_U8((0x00000000u+f->r[2]))=(uint8_t)(0x00000001u);
    /* 004c973f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c9740 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c9741 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004ca9a0(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ca9a0 */
    f->r[0]=MI_U32((0x00bca2c4u));
    /* 004ca9a5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ca9a6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ca9a7 */
    f->r[6]=0x00000030u;
    /* 004ca9ac */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[0]*2u));
    /* 004ca9b4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ca9b6 */
    if((f->flags&MI_ZF))goto mi_004cabc6;
    /* 004ca9bc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((0x00000024u)&0xffu)<<0);
mi_004ca9be:;
    /* 004ca9be */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ca9c1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ca9c4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ca9c6 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ca9c9 */
    a=MI_U8((0x00bcd602u+f->r[1]));b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004ca9cf */
    if((f->flags&MI_ZF))goto mi_004ca9df;
    /* 004ca9d1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[1]));
    /* 004ca9d8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ca9da */
    if(!(f->flags&MI_ZF))goto mi_004ca9be;
    /* 004ca9dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ca9dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ca9de */
    f->r[4]+=4;return;
mi_004ca9df:;
    /* 004ca9df */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ca9e2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ca9e5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ca9e7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd603u+f->r[1]*4u)))&0xffu)<<0);
    /* 004ca9ee */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004ca9f0 */
    if(!(f->flags&MI_ZF))goto mi_004caa01;
    /* 004ca9f2 */
    f->r[6]=0x00000031u;
    /* 004ca9f7 */
    f->r[0]=0x00000003u;
    /* 004ca9fc */
    goto mi_004caaa2;
mi_004caa01:;
    /* 004caa01 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004caa03 */
    if(!(f->flags&MI_ZF))goto mi_004caa0f;
    /* 004caa05 */
    f->r[0]=0x00000005u;
    /* 004caa0a */
    goto mi_004caaa2;
mi_004caa0f:;
    /* 004caa0f */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004caa11 */
    if(!(f->flags&MI_ZF))goto mi_004caa1d;
    /* 004caa13 */
    f->r[0]=0x00000006u;
    /* 004caa18 */
    goto mi_004caaa2;
mi_004caa1d:;
    /* 004caa1d */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004caa1f */
    if(!(f->flags&MI_ZF))goto mi_004caa28;
    /* 004caa21 */
    f->r[0]=0x00000012u;
    /* 004caa26 */
    goto mi_004caaa2;
mi_004caa28:;
    /* 004caa28 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004caa2a */
    if(!(f->flags&MI_ZF))goto mi_004caa33;
    /* 004caa2c */
    f->r[0]=0x00000013u;
    /* 004caa31 */
    goto mi_004caaa2;
mi_004caa33:;
    /* 004caa33 */
    a=(f->r[0]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 004caa35 */
    if((f->flags&MI_CF))goto mi_004caa45;
    /* 004caa37 */
    a=(f->r[0]&0xffu);b=0x00000015u;t=mi_alu(f,a,b,8,1);
    /* 004caa39 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004caa45;
    /* 004caa3b */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004caa40 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caa43 */
    goto mi_004caaa2;
mi_004caa45:;
    /* 004caa45 */
    a=(f->r[0]&0xffu);b=0x00000016u;t=mi_alu(f,a,b,8,1);
    /* 004caa47 */
    if((f->flags&MI_CF))goto mi_004caa57;
    /* 004caa49 */
    a=(f->r[0]&0xffu);b=0x00000019u;t=mi_alu(f,a,b,8,1);
    /* 004caa4b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004caa57;
    /* 004caa4d */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004caa52 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caa55 */
    goto mi_004caaa2;
mi_004caa57:;
    /* 004caa57 */
    a=(f->r[0]&0xffu);b=0x0000001au;t=mi_alu(f,a,b,8,1);
    /* 004caa59 */
    if((f->flags&MI_CF))goto mi_004caa69;
    /* 004caa5b */
    a=(f->r[0]&0xffu);b=0x0000001du;t=mi_alu(f,a,b,8,1);
    /* 004caa5d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004caa69;
    /* 004caa5f */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004caa64 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caa67 */
    goto mi_004caaa2;
mi_004caa69:;
    /* 004caa69 */
    a=(f->r[0]&0xffu);b=0x0000001eu;t=mi_alu(f,a,b,8,1);
    /* 004caa6b */
    if((f->flags&MI_CF))goto mi_004caa7b;
    /* 004caa6d */
    a=(f->r[0]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,1);
    /* 004caa6f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004caa7b;
    /* 004caa71 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004caa76 */
    a=f->r[0];b=0x0000001au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caa79 */
    goto mi_004caaa2;
mi_004caa7b:;
    /* 004caa7b */
    a=(f->r[0]&0xffu);b=0x00000021u;t=mi_alu(f,a,b,8,1);
    /* 004caa7d */
    if((f->flags&MI_CF))goto mi_004caa8d;
    /* 004caa7f */
    a=(f->r[0]&0xffu);b=0x00000022u;t=mi_alu(f,a,b,8,1);
    /* 004caa81 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004caa8d;
    /* 004caa83 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004caa88 */
    a=f->r[0];b=0x0000001au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caa8b */
    goto mi_004caaa2;
mi_004caa8d:;
    /* 004caa8d */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004caa92 */
    f->r[2]=(0xffffffceu+f->r[0]);
    /* 004caa95 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004caa98 */
    if(!mi_signed_less(f->flags))goto mi_004caa9f;
    /* 004caa9a */
    a=f->r[0];b=0xffffffd8u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caa9d */
    goto mi_004caaa2;
mi_004caa9f:;
    /* 004caa9f */
    a=f->r[0];b=0xfffffff6u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004caaa2:;
    /* 004caaa2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caaa3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caaa4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caaa5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caaa6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004caaabu;f->boundary(0x004caaa6u,0x004cc110u,f);
    /* 004caaab */
    f->r[0]=MI_U32((0x00f56f84u));
    /* 004caab0 */
    f->r[6]=MI_U32((0x00e20768u));
    /* 004caab6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caab7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caab8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004caabdu;f->boundary(0x004caab8u,0x004cbf30u,f);
    /* 004caabd */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004caac3 */
    f->r[5]=MI_U32((0x00e38e30u));
    /* 004caac9 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caacc */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004caacf */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004caad1 */
    MI_U32((0x00d3a6d0u))=(uint32_t)(f->r[0]);
    /* 004caad6 */
    f->r[2]=(0x00000014u+f->r[0]);
    /* 004caad9 */
    MI_U32((0x00d3a6ccu))=(uint32_t)(0x00000014u);
    /* 004caae3 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004caae5 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004caae8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004caaea */
    a=f->r[5];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 004caaf0 */
    f->r[7]=f->r[3];
    /* 004caaf2 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004caaf5 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004caaf7 */
    if((f->flags&MI_ZF))goto mi_004caafe;
    /* 004caaf9 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004caafc */
    goto mi_004cab03;
mi_004caafe:;
    /* 004caafe */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004cab00 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
mi_004cab03:;
    /* 004cab03 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004cab05 */
    MI_U32((0x00d3a6d8u))=(uint32_t)(f->r[1]);
    /* 004cab0b */
    if((f->flags&MI_ZF))goto mi_004cab12;
    /* 004cab0d */
    a=f->r[5];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004cab10 */
    goto mi_004cab15;
mi_004cab12:;
    /* 004cab12 */
    a=f->r[5];b=0xffffffe0u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_004cab15:;
    /* 004cab15 */
    f->r[2]=MI_U32((0x00e92dfcu));
    /* 004cab1b */
    MI_U32((0x00d3a6d4u))=(uint32_t)(f->r[5]);
    /* 004cab21 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00d3b51cu));
    /* 004cab28 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004cab2b */
    if((f->flags&MI_ZF))goto mi_004cab42;
    /* 004cab2d */
    MI_U16((0x00d3b51cu))=(uint16_t)(0x00001c82u);
    /* 004cab36 */
    f->r[7]=0x00005dc8u;
    /* 004cab3b */
    f->r[2]=0x00002c40u;
    /* 004cab40 */
    goto mi_004cab55;
mi_004cab42:;
    /* 004cab42 */
    MI_U16((0x00d3b51cu))=(uint16_t)(0x00003922u);
    /* 004cab4b */
    f->r[7]=0x0000bb88u;
    /* 004cab50 */
    f->r[2]=0x000058a0u;
mi_004cab55:;
    /* 004cab55 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab56 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab58 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab59 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab5a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cab60u;f->boundary(0x004cab5bu,0x004cd680u,f);
    /* 004cab60 */
    f->r[0]=MI_U32((0x00d3a6ccu));
    /* 004cab65 */
    f->r[1]=MI_U32((0x00d3a6d0u));
    /* 004cab6b */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004cab71 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cab74 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab76 */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004cab7b */
    a=f->r[1];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cab7e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004cab7f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab80 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004cab81 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab82 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cab88u;f->boundary(0x004cab83u,0x004cd9d0u,f);
    /* 004cab88 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00d3b51cu));
    /* 004cab8f */
    f->r[0]=MI_U32((0x00d3a6d4u));
    /* 004cab94 */
    f->r[2]=MI_U32((0x00f56f84u));
    /* 004cab9a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cab9b */
    f->r[1]=MI_U32((0x00d3a6d8u));
    /* 004caba1 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004caba4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caba5 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004caba8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004caba9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cabaa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cabab */
    MI_U32((0x00e20778u))=(uint32_t)(0x00000000u);
    /* 004cabb5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cabbau;f->boundary(0x004cabb5u,0x004cc770u,f);
    /* 004cabba */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cabbd */
    MI_U16((0x00d3b51cu))=(uint16_t)((f->r[3]&0xffffu));
    /* 004cabc4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cabc5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_004cabc6:;
    /* 004cabc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cabc7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cabc8 */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004cb0b0(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004cb0b0 */
    a=f->r[4];b=0x00000054u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004cb0b3 */
    f->r[1]=MI_U32((0x00e38e30u));
    /* 004cb0b9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004cb0bb */
    MI_U32((0x00d3a6d0u))=(uint32_t)(0x000000b4u);
    /* 004cb0c5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb0c6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb0c7 */
    f->r[5]=MI_U32((0x00e38e34u));
    /* 004cb0cd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb0ce */
    a=f->r[5];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 004cb0d4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb0d5 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cb0d8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004cb0da */
    a=f->r[1];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 004cb0e0 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cb0e3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cb0e5 */
    MI_U32((0x00d3a6ccu))=(uint32_t)(0x00000078u);
    /* 004cb0ef */
    if((f->flags&MI_ZF))goto mi_004cb0f6;
    /* 004cb0f1 */
    a=f->r[5];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004cb0f4 */
    goto mi_004cb0fc;
mi_004cb0f6:;
    /* 004cb0f6 */
    a=f->r[5];b=0xffffff38u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_004cb0fc:;
    /* 004cb0fc */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004cb0fe */
    MI_U32((0x00d3a6d8u))=(uint32_t)(f->r[5]);
    /* 004cb104 */
    f->r[3]=(0x0000000au+f->r[1]);
    /* 004cb107 */
    if(!(f->flags&MI_ZF))goto mi_004cb10c;
    /* 004cb109 */
    f->r[3]=(0xffffffe0u+f->r[1]);
mi_004cb10c:;
    /* 004cb10c */
    f->r[0]=MI_U32((0x00e92dfcu));
    /* 004cb111 */
    MI_U32((0x00d3a6d4u))=(uint32_t)(f->r[3]);
    /* 004cb117 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cb11a */
    if((f->flags&MI_ZF))goto mi_004cb138;
    /* 004cb11c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00001c82u);
    /* 004cb124 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00007f9au);
    /* 004cb12c */
    f->r[6]=0x00005dc8u;
    /* 004cb131 */
    f->r[1]=0x00002c40u;
    /* 004cb136 */
    goto mi_004cb152;
mi_004cb138:;
    /* 004cb138 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00003922u);
    /* 004cb140 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000ff3au);
    /* 004cb148 */
    f->r[6]=0x0000bb88u;
    /* 004cb14d */
    f->r[1]=0x000058a0u;
mi_004cb152:;
    /* 004cb152 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004cb157 */
    f->r[7]=MI_U32((0x00f56f84u));
    /* 004cb15d */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004cb163 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb164 */
    a=0x00000078u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb166 */
    f->r[2]=MI_U32((0x00ea446cu+f->r[0]));
    /* 004cb16c */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb171 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb175 */
    f->r[2]=MI_U32((0x00ea5b3cu+f->r[0]));
    /* 004cb17b */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb17f */
    f->r[2]=MI_U32((0x00ea5b40u+f->r[0]));
    /* 004cb185 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb189 */
    f->r[2]=MI_U32((0x00ea5b44u+f->r[0]));
    /* 004cb18f */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb193 */
    f->r[2]=MI_U32((0x00ea5b48u+f->r[0]));
    /* 004cb199 */
    f->r[0]=MI_U32((0x00ea1878u+f->r[0]));
    /* 004cb19f */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb1a3 */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004cb1a7 */
    f->r[2]=0x005e03fcu;
    /* 004cb1ac */
    f->r[0]=0x0057c048u;
    /* 004cb1b1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb1b2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb1b3 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(0x00000087u);
    /* 004cb1bb */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(0x00000088u);
    /* 004cb1c3 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(0x00000089u);
    /* 004cb1cb */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(0x0000008au);
    /* 004cb1d3 */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(0x0000008bu);
    /* 004cb1db */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(0x0000008eu);
    /* 004cb1e3 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb1e7 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004cb1eb */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004cb1ef */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004cb1f3 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004cb1f7 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb1fb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb200u;f->boundary(0x004cb1fbu,0x004cd680u,f);
    /* 004cb200 */
    f->r[1]=MI_U32((0x00d3a6ccu));
    /* 004cb206 */
    f->r[2]=MI_U32((0x00d3a6d0u));
    /* 004cb20c */
    f->r[0]=MI_U32((0x00d3a6d4u));
    /* 004cb211 */
    a=f->r[1];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cb214 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb215 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb216 */
    f->r[1]=MI_U32((0x00d3a6d8u));
    /* 004cb21c */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004cb21f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004cb220 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb221 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004cb222 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb223 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb224 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb229u;f->boundary(0x004cb224u,0x004cd9d0u,f);
    /* 004cb229 */
    f->r[2]=(0x00000044u+f->r[4]);
    /* 004cb22d */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb230 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004cb232 */
    f->r[3]=(0x00000038u+f->r[4]);
    /* 004cb236 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004cb23a:;
    /* 004cb23a */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004cb23e */
    f->r[1]=MI_U32((0x00d3a6d4u));
    /* 004cb244 */
    f->r[5]=MI_U32((0xfffffffcu+f->r[3]));
    /* 004cb247 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb248 */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004cb24d */
    f->r[2]=(0x00000005u+f->r[6]+f->r[1]*1u);
    /* 004cb251 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb252 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cb255 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb256 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb257 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb258 */
    a=0x00000044u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb25a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb25fu;f->boundary(0x004cb25au,0x004ccbb0u,f);
    /* 004cb25f */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004cb263 */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004cb269 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb26a */
    f->r[1]=MI_U32((0x00d3a6d8u));
    /* 004cb270 */
    f->r[0]=(0x00000004u+f->r[6]+f->r[2]*1u);
    /* 004cb274 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb275 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cb278 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb279 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb27a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb27b */
    a=0x00000044u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb27d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb282u;f->boundary(0x004cb27du,0x004ccbb0u,f);
    /* 004cb282 */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 004cb286 */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 004cb28a */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 004cb28c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb28d */
    f->r[5]=MI_U32((0x00000000u+f->r[2]));
    /* 004cb28f */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004cb295 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb296 */
    f->r[1]=(0x00000005u+f->r[6]+f->r[2]*1u);
    /* 004cb29a */
    f->r[2]=MI_U32((0x00d3a6d8u));
    /* 004cb2a0 */
    a=f->r[2];b=0x0000008cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004cb2a6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2a7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2a8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2a9 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2ab */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb2b1u;f->boundary(0x004cb2acu,0x004cce00u,f);
    /* 004cb2b1 */
    f->r[0]=MI_U32((0x00000060u+f->r[4]));
    /* 004cb2b5 */
    f->r[1]=MI_U32((0x00d3a6d4u));
    /* 004cb2bb */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb2be */
    f->r[2]=(0x00000004u+f->r[6]+f->r[1]*1u);
    /* 004cb2c2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2c3 */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004cb2c8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2c9 */
    a=f->r[0];b=0x0000008bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cb2ce */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2cf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2d0 */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 004cb2d2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2d3 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2d5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb2d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb2dbu;f->boundary(0x004cb2d6u,0x004cce00u,f);
    /* 004cb2db */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 004cb2df */
    a=f->r[6];b=0x0000000eu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004cb2e2 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb2e5 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004cb2e8 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004cb2eb */
    a=f->r[6];b=0x00000054u;t=mi_alu(f,a,b,32,1);
    /* 004cb2ee */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb2f2 */
    if(mi_signed_less(f->flags))goto mi_004cb23a;
    /* 004cb2f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cb2f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cb2fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cb2fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cb2fc */
    a=f->r[4];b=0x00000054u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb2ff */
    f->r[4]+=4;return;
}
void restore_tooltip_C_004cb300(struct restore_tooltip_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004cb300 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004cb303 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb304 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb305 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb306 */
    f->r[0]=(0x00000010u+f->r[4]);
    /* 004cb30a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb30b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004cb30d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb30e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb312 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb316 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb31a */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb31e */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb322 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb326 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb32a */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb32e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb332 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb336 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb33a */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb33e */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb342 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb346 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb34a */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004cb34e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb353u;f->boundary(0x004cb34eu,0x00559fc0u,f);
    /* 004cb353 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004cb356 */
    f->r[5]=MI_U32((0x00e38e30u));
    /* 004cb35c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb35f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004cb361 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004cb364 */
    MI_U32((0x00d3a6ccu))=(uint32_t)(0x00000014u);
    /* 004cb36e */
    f->r[0]=(0x00000032u+f->r[1]*4u);
    /* 004cb375 */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004cb37b */
    MI_U32((0x00d3a6d0u))=(uint32_t)(f->r[0]);
    /* 004cb380 */
    f->r[2]=(0x00000014u+f->r[0]);
    /* 004cb383 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004cb385 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cb388 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004cb38a */
    a=f->r[5];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    /* 004cb390 */
    f->r[7]=f->r[3];
    /* 004cb392 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cb395 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cb397 */
    if((f->flags&MI_ZF))goto mi_004cb39e;
    /* 004cb399 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cb39c */
    goto mi_004cb3a3;
mi_004cb39e:;
    /* 004cb39e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004cb3a0 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
mi_004cb3a3:;
    /* 004cb3a3 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cb3a5 */
    MI_U32((0x00d3a6d8u))=(uint32_t)(f->r[1]);
    /* 004cb3ab */
    f->r[3]=(0x0000000au+f->r[5]);
    /* 004cb3ae */
    if(!(f->flags&MI_ZF))goto mi_004cb3b3;
    /* 004cb3b0 */
    f->r[3]=(0xffffffe0u+f->r[5]);
mi_004cb3b3:;
    /* 004cb3b3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00d3b51cu));
    /* 004cb3ba */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004cb3be */
    f->r[2]=MI_U32((0x00e92dfcu));
    /* 004cb3c4 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004cb3c7 */
    MI_U32((0x00d3a6d4u))=(uint32_t)(f->r[3]);
    /* 004cb3cd */
    if((f->flags&MI_ZF))goto mi_004cb3e4;
    /* 004cb3cf */
    MI_U16((0x00d3b51cu))=(uint16_t)(0x00001c82u);
    /* 004cb3d8 */
    f->r[7]=0x00005dc8u;
    /* 004cb3dd */
    f->r[2]=0x00002c40u;
    /* 004cb3e2 */
    goto mi_004cb3f7;
mi_004cb3e4:;
    /* 004cb3e4 */
    MI_U16((0x00d3b51cu))=(uint16_t)(0x00003922u);
    /* 004cb3ed */
    f->r[7]=0x0000bb88u;
    /* 004cb3f2 */
    f->r[2]=0x000058a0u;
mi_004cb3f7:;
    /* 004cb3f7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb3f8 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb3fa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb3fb */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb3fc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb3fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb402u;f->boundary(0x004cb3fdu,0x004cd680u,f);
    /* 004cb402 */
    f->r[0]=MI_U32((0x00d3a6ccu));
    /* 004cb407 */
    f->r[1]=MI_U32((0x00d3a6d0u));
    /* 004cb40d */
    f->r[2]=MI_U32((0x00d3a6d4u));
    /* 004cb413 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cb416 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb417 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb418 */
    f->r[0]=MI_U32((0x00d3a6d8u));
    /* 004cb41d */
    a=f->r[1];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004cb420 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004cb421 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb422 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004cb423 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb424 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb425 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb42au;f->boundary(0x004cb425u,0x004cd9d0u,f);
    /* 004cb42a */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb42d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004cb42f */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[6]);
    /* 004cb435 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004cb437 */
    f->r[5]=(0x00000014u+f->r[4]);
mi_004cb43b:;
    /* 004cb43b */
    a=MI_U32((0x00000000u+f->r[5]));b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cb43e */
    if((f->flags&MI_ZF))goto mi_004cb4b2;
    /* 004cb440 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cb442 */
    if((f->flags&MI_ZF))goto mi_004cb47a;
    /* 004cb444 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00d3b51cu));
    /* 004cb44b */
    f->r[2]=MI_U32((0x00f56f84u));
    /* 004cb451 */
    f->r[0]=MI_U32((0x00d3a6d4u));
    /* 004cb456 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb457 */
    f->r[1]=MI_U32((0x00d3a6d8u));
    /* 004cb45d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb45e */
    f->r[2]=MI_U32((0x00e20778u));
    /* 004cb464 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cb467 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb468 */
    f->r[0]=(0x00000005u+f->r[2]+f->r[1]*1u);
    /* 004cb46c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb46d */
    a=0x005e0400u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb472 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb477u;f->boundary(0x004cb472u,0x004cc770u,f);
    /* 004cb477 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004cb47a:;
    /* 004cb47a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00d3b51cu));
    /* 004cb481 */
    f->r[2]=MI_U32((0x00f56f84u));
    /* 004cb487 */
    f->r[0]=MI_U32((0x00d3a6d4u));
    /* 004cb48c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb48d */
    f->r[1]=MI_U32((0x00d3a6d8u));
    /* 004cb493 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb494 */
    f->r[2]=MI_U32((0x00e20778u));
    /* 004cb49a */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cb49d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb49e */
    f->r[0]=(0x00000005u+f->r[2]+f->r[1]*1u);
    /* 004cb4a2 */
    f->r[1]=(0x00000027u+f->r[7]);
    /* 004cb4a5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb4a6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb4a7 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cb4a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cb4aeu;f->boundary(0x004cb4a9u,0x004ccbb0u,f);
    /* 004cb4ae */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb4b1 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
mi_004cb4b2:;
    /* 004cb4b2 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004cb4b3 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004cb4b6 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004cb4b9 */
    if(mi_signed_less(f->flags))goto mi_004cb43b;
    /* 004cb4bb */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000010u+f->r[4])))&0xffffu)<<0);
    /* 004cb4c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cb4c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cb4c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cb4c3 */
    MI_U16((0x00d3b51cu))=(uint16_t)((f->r[2]&0xffffu));
    /* 004cb4ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cb4cb */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cb4ce */
    f->r[4]+=4;return;
}
