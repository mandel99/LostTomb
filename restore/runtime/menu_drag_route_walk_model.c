/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "route_walk_model.h"

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

void restore_C_0051c930(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051c930 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0051c933 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c934 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c935 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c936 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c937 */
    f->r[7]=MI_U32((0x0000002cu+f->r[4]));
    /* 0051c93b */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0051c941 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 0051c945 */
    MI_U32((0x00bc971cu))=(uint32_t)(0x00000000u);
    /* 0051c94f */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 0051c952 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0051c95a */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*2u);
    /* 0051c95d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051c960 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0051c963 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051c965 */
    MI_U32((0x00eae000u))=(uint32_t)(f->r[6]);
mi_0051c96b:;
    /* 0051c96b */
    f->r[0]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051c972 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c974 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051c978 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051c999;
    /* 0051c97a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[6]*2u));
    /* 0051c982 */
    f->r[1]=f->r[0];
    /* 0051c984 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0051c987 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051c989 */
    a=MI_U16((0x007999e8u+f->r[1]*8u));b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0051c993 */
    if(!(f->flags&MI_ZF))goto mi_0051cad1;
mi_0051c999:;
    /* 0051c999 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 0051c99d */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0051c9a0 */
    if((f->flags&MI_ZF))goto mi_0051c9dd;
    /* 0051c9a2 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0051c9a5 */
    if((f->flags&MI_ZF))goto mi_0051c9d5;
    /* 0051c9a7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051c9a8 */
    if(!(f->flags&MI_ZF))goto mi_0051ca06;
    /* 0051c9aa */
    a=MI_U8((0x00af598cu+f->r[6]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0051c9b2 */
    if((f->flags&MI_ZF))goto mi_0051c9c5;
    /* 0051c9b4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c9bau;f->boundary(0x0051c9b5u,0x0051c630u,f);
    /* 0051c9ba */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c9bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c9bf */
    if((f->flags&MI_ZF))goto mi_0051cad1;
mi_0051c9c5:;
    /* 0051c9c5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9c7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9c9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9ca */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9cb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c9d0u;f->boundary(0x0051c9cbu,0x0047b960u,f);
    /* 0051c9d0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c9d3 */
    goto mi_0051ca06;
mi_0051c9d5:;
    /* 0051c9d5 */
    a=MI_U32((0x00bc971cu));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00bc971cu))=(uint32_t)(t);
    /* 0051c9db */
    goto mi_0051ca06;
mi_0051c9dd:;
    /* 0051c9dd */
    f->r[0]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 0051c9e4 */
    a=((f->r[0]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051c9e7 */
    if((f->flags&MI_ZF))goto mi_0051c9fa;
    /* 0051c9e9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c9efu;f->boundary(0x0051c9eau,0x0051c390u,f);
    /* 0051c9ef */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c9f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c9f4 */
    if((f->flags&MI_ZF))goto mi_0051cad1;
mi_0051c9fa:;
    /* 0051c9fa */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9fc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9fd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c9fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ca03u;f->boundary(0x0051c9feu,0x0047b310u,f);
    /* 0051ca03 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0051ca06:;
    /* 0051ca06 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0051ca0a */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0051ca0e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ca0f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ca10 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ca11 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ca12 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ca17u;f->boundary(0x0051ca12u,0x0051cba0u,f);
    /* 0051ca17 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0051ca1c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051ca1f */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0051ca22 */
    if((f->flags&MI_ZF))goto mi_0051cac4;
    /* 0051ca28 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0051ca2b */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0051ca2d */
    a=f->r[0];b=0x005f815cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051ca32 */
    f->r[5]=f->r[0];
mi_0051ca34:;
    /* 0051ca34 */
    f->r[7]=MI_U32((0x00000000u+f->r[5]));
    /* 0051ca37 */
    f->r[2]=MI_U32((0x00eae000u));
    /* 0051ca3d */
    f->r[6]=MI_U32((0x005f80fcu+f->r[7]*4u));
    /* 0051ca44 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051ca46 */
    f->r[0]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051ca4d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051ca4f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ca73;
    /* 0051ca51 */
    a=f->r[0];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0051ca55 */
    if(!mi_signed_less(f->flags))goto mi_0051ca73;
    /* 0051ca57 */
    f->r[1]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 0051ca5e */
    a=f->r[1];t=a>>8u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>7u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 0051ca61 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051ca64 */
    if((f->flags&MI_ZF))goto mi_0051ca7e;
    /* 0051ca66 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ca67 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ca6cu;f->boundary(0x0051ca67u,0x0051c390u,f);
    /* 0051ca6c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051ca6f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051ca71 */
    if(!(f->flags&MI_ZF))goto mi_0051ca7e;
mi_0051ca73:;
    /* 0051ca73 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0051ca74 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0051ca77 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0051ca7a */
    if(!mi_signed_less(f->flags))goto mi_0051cad1;
    /* 0051ca7c */
    goto mi_0051ca34;
mi_0051ca7e:;
    /* 0051ca7e */
    f->r[2]=MI_U32((0x005f811cu+f->r[7]*8u));
    /* 0051ca85 */
    f->r[5]=MI_U32((0x00000028u+f->r[4]));
    /* 0051ca89 */
    f->r[0]=MI_U32((0x005f8120u+f->r[7]*8u));
    /* 0051ca90 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0051ca94 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0051ca96 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051ca98 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0051ca9b */
    MI_U32((0x00eae000u))=(uint32_t)(f->r[6]);
    /* 0051caa1 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051caa5 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051caa9 */
    if(!mi_signed_less(f->flags))goto mi_0051cad1;
    /* 0051caab */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0051caaf */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051cab0 */
    a=f->r[0];b=0x00000190u;t=mi_alu(f,a,b,32,1);
    /* 0051cab5 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051cab9 */
    if(!mi_signed_less(f->flags))goto mi_0051cad1;
    /* 0051cabb */
    f->r[3]=f->r[5];
    /* 0051cabd */
    f->r[7]=f->r[1];
    /* 0051cabf */
    goto mi_0051c96b;
mi_0051cac4:;
    /* 0051cac4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051cac5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051cac6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051cac7 */
    f->r[0]=0x00000001u;
    /* 0051cacc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051cacd */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051cad0 */
    f->r[4]+=4;return;
mi_0051cad1:;
    /* 0051cad1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051cad2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051cad3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051cad4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051cad6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051cad7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051cada */
    f->r[4]+=4;return;
}
