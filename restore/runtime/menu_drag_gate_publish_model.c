/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "map_gates.h"

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

void restore_C_0047d870(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047d870 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0047d874 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d875 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d876 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d877 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047d879 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d87a */
    if((f->flags&MI_ZF))goto mi_0047d8d4;
    /* 0047d87c */
    f->r[0]=MI_U32((0x00bca31cu));
    /* 0047d881 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047d884 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0047d887 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 0047d88d */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047d890 */
    f->r[0]=MI_U32((0x00bca318u));
    /* 0047d895 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0047d898 */
    f->r[2]=0xdfcffffdu;
    /* 0047d89d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047d89f */
    f->r[7]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0047d8a6 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0047d8a8 */
    MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0047d8af */
    f->r[0]=MI_U32((0x00bca324u));
    /* 0047d8b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047d8b5 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047d8b8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[6]*2u);
    /* 0047d8bb */
    f->r[6]=MI_U32((0x00bca320u));
    /* 0047d8c1 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047d8c4 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 0047d8c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047d8c8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047d8ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047d8cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047d8cc */
    a=MI_U32((0x00af598cu+f->r[0]*4u));b=f->r[2];t=mi_alu(f,a,b,32,3);MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(t);
    /* 0047d8d3 */
    f->r[4]+=4;return;
mi_0047d8d4:;
    /* 0047d8d4 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00784c64u)))&0xffffu)<<0);
    /* 0047d8db */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0047d8e0 */
    f->r[7]=MI_U32((0x00bca2d0u));
    /* 0047d8e6 */
    a=(f->r[2]&0xffffu);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0047d8e9 */
    if(!(f->flags&MI_ZF))goto mi_0047d8f2;
    /* 0047d8eb */
    f->r[6]=0x00000002u;
    /* 0047d8f0 */
    goto mi_0047d925;
mi_0047d8f2:;
    /* 0047d8f2 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 0047d8f5 */
    f->r[1]=(0xffffffffu+f->r[7]);
    /* 0047d8f8 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047d8fa */
    if(!(f->flags&MI_ZF))goto mi_0047d903;
    /* 0047d8fc */
    f->r[6]=0x00000006u;
    /* 0047d901 */
    goto mi_0047d925;
mi_0047d903:;
    /* 0047d903 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00784c66u)))&0xffffu)<<0);
    /* 0047d90a */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0047d90d */
    if(!(f->flags&MI_ZF))goto mi_0047d913;
    /* 0047d90f */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0047d911 */
    goto mi_0047d925;
mi_0047d913:;
    /* 0047d913 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0047d916 */
    f->r[3]=(0xffffffffu+f->r[0]);
    /* 0047d919 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0047d91b */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[6]=t;
    /* 0047d91d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,2);f->r[6]=t;
    /* 0047d91f */
    a=f->r[6];b=0xfffffffbu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0047d922 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_0047d925:;
    /* 0047d925 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x00784c68u)))&0xffffu)<<0);
    /* 0047d92c */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00784c6au)))&0xffffu)<<0);
    /* 0047d933 */
    a=(f->r[5]&0xffffu);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0047d936 */
    if(!(f->flags&MI_ZF))goto mi_0047d93f;
    /* 0047d938 */
    f->r[3]=0x00000002u;
    /* 0047d93d */
    goto mi_0047d967;
mi_0047d93f:;
    /* 0047d93f */
    f->r[3]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 0047d942 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0047d943 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0047d945 */
    if(!(f->flags&MI_ZF))goto mi_0047d94e;
    /* 0047d947 */
    f->r[3]=0x00000006u;
    /* 0047d94c */
    goto mi_0047d967;
mi_0047d94e:;
    /* 0047d94e */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0047d951 */
    if(!(f->flags&MI_ZF))goto mi_0047d957;
    /* 0047d953 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0047d955 */
    goto mi_0047d967;
mi_0047d957:;
    /* 0047d957 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0047d95a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047d95b */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0047d95d */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[3]=t;
    /* 0047d95f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,2);f->r[3]=t;
    /* 0047d961 */
    a=f->r[3];b=0xfffffffbu;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 0047d964 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0047d967:;
    /* 0047d967 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047d969 */
    if(mi_signed_less(f->flags))goto mi_0047da33;
    /* 0047d96f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c66u));
    /* 0047d976 */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 0047d97c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047d97f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0047d982 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 0047d985 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047d988 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0047d98a */
    f->r[5]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 0047d98d */
    f->r[7]=0x00000001u;
    /* 0047d992 */
    goto mi_0047d99b;
mi_0047d994:;
    /* 0047d994 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00784c64u)))&0xffffu)<<0);
mi_0047d99b:;
    /* 0047d99b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c66u));
    /* 0047d9a2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d9a3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d9a4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 0047d9a7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d9a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d9aa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d9ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d9b0u;f->boundary(0x0047d9abu,0x00482810u,f);
    /* 0047d9b0 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047d9b3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047d9b5 */
    if(!(f->flags&MI_ZF))goto mi_0047d9bd;
    /* 0047d9b7 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0047d9b8 */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0047d9bb */
    if(mi_signed_less(f->flags))goto mi_0047d994;
mi_0047d9bd:;
    /* 0047d9bd */
    f->r[0]=MI_U32((0x00d3b4e0u));
    /* 0047d9c2 */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 0047d9c8 */
    MI_U32((0x00bca31cu))=(uint32_t)(f->r[0]);
    /* 0047d9cd */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047d9d0 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0047d9d3 */
    f->r[2]=MI_U32((0x00d3b4e4u));
    /* 0047d9d9 */
    MI_U32((0x00bca318u))=(uint32_t)(f->r[2]);
    /* 0047d9df */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0047d9e2 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0047d9e5 */
    f->r[2]=MI_U32((0x00e94e04u));
    /* 0047d9eb */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047d9ed */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0047d9f0 */
    MI_U32((0x00bc96c4u))=(uint32_t)(f->r[1]);
    /* 0047d9f6 */
    f->r[5]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 0047d9fd */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,4);f->r[5]=t;
    /* 0047da00 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0047da03 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[5]);
    /* 0047da0a */
    if(mi_signed_less(f->flags))goto mi_0047da0f;
    /* 0047da0c */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0047da0f:;
    /* 0047da0f */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0047da10 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x00784c68u)))&0xffffu)<<0);
    /* 0047da17 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047da19 */
    f->r[2]=MI_U32((0x00f56bfcu));
    /* 0047da1f */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0047da21 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047da23 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[0]);
    /* 0047da2a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00784c6au)))&0xffffu)<<0);
    /* 0047da31 */
    goto mi_0047da39;
mi_0047da33:;
    /* 0047da33 */
    f->r[7]=MI_U32((0x00bca2d8u));
mi_0047da39:;
    /* 0047da39 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0047da3b */
    if(mi_signed_less(f->flags))goto mi_0047daf0;
    /* 0047da41 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0047da44 */
    f->r[6]=0x00000001u;
    /* 0047da49 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047da4c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0047da4f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 0047da52 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047da55 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0047da57 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 0047da5a */
    goto mi_0047da6a;
mi_0047da5c:;
    /* 0047da5c */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00784c6au)))&0xffffu)<<0);
    /* 0047da63 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x00784c68u)))&0xffffu)<<0);
mi_0047da6a:;
    /* 0047da6a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0047da6d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047da6e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047da6f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 0047da72 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047da74 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047da75 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047da76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047da7bu;f->boundary(0x0047da76u,0x00482810u,f);
    /* 0047da7b */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047da7e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047da80 */
    if(!(f->flags&MI_ZF))goto mi_0047da88;
    /* 0047da82 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047da83 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0047da86 */
    if(mi_signed_less(f->flags))goto mi_0047da5c;
mi_0047da88:;
    /* 0047da88 */
    f->r[0]=MI_U32((0x00d3b4e0u));
    /* 0047da8d */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 0047da93 */
    MI_U32((0x00bca324u))=(uint32_t)(f->r[0]);
    /* 0047da98 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047da9b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0047da9e */
    f->r[2]=MI_U32((0x00d3b4e4u));
    /* 0047daa4 */
    MI_U32((0x00bca320u))=(uint32_t)(f->r[2]);
    /* 0047daaa */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0047daad */
    f->r[1]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0047dab0 */
    f->r[2]=MI_U32((0x00e94e04u));
    /* 0047dab6 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047dab8 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
    /* 0047dabb */
    MI_U32((0x00bc96c0u))=(uint32_t)(f->r[1]);
    /* 0047dac1 */
    f->r[6]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 0047dac8 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
    /* 0047dacb */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0047dace */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0047dad5 */
    if(mi_signed_less(f->flags))goto mi_0047dada;
    /* 0047dad7 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0047dada:;
    /* 0047dada */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0047dadb */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047dadd */
    f->r[2]=MI_U32((0x00f56bfcu));
    /* 0047dae3 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0047dae5 */
    f->r[0]=(0x00000004u+f->r[0]+f->r[2]*1u);
    /* 0047dae9 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[0]);
mi_0047daf0:;
    /* 0047daf0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047daf1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047daf2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047daf3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047daf4 */
    f->r[4]+=4;return;
}
