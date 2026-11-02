/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "widget_position.h"
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
static __inline uint32_t mi_alu(struct restore_widget_position_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_widget_position_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_widget_position_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static __inline void mi_idiv32(struct restore_widget_position_frame *f,uint32_t divisor_bits)
{
 int32_t divisor=(int32_t)divisor_bits;
 int64_t dividend=(int64_t)(int32_t)f->r[2]*INT64_C(4294967296)+(int64_t)f->r[0];
 int64_t quotient,remainder;
 /* Native #DE cases explicitly fail; no C division overflow or fake result. */
 if(!divisor||(dividend==INT64_MIN&&divisor==-1))abort();
 quotient=dividend/(int64_t)divisor;
 if(quotient<INT32_MIN||quotient>INT32_MAX)abort();
 remainder=dividend%(int64_t)divisor;
 f->r[0]=(uint32_t)(int32_t)quotient;f->r[2]=(uint32_t)(int32_t)remainder;
 /* IDIV leaves arithmetic flags undefined. Keep their previous frame bits. */
}

void restore_widget_C_0041f910(struct restore_widget_position_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0041f910 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0041f914 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 0041f918 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0041f91b */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0041f921 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0041f925 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041f926 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 0041f92c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041f92d */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0041f933 */
    a=MI_U16((0x00000010u+f->r[1]));b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0041f938 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041f939 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041f93a */
    if((f->flags&MI_ZF))goto mi_0041fabf;
    /* 0041f940 */
    f->r[6]=MI_U32((0x00000000u+f->r[1]));
    /* 0041f942 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0041f944 */
    if(mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f94a */
    a=f->r[6];b=0x000000fdu;t=mi_alu(f,a,b,32,1);
    /* 0041f950 */
    if(!mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f956 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 0041f95b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041f95c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041f95e */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*4u);
    /* 0041f961 */
    f->r[6]=MI_U32((0x00000004u+f->r[1]));
    /* 0041f964 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0041f966 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0041f968 */
    f->r[3]=(0x005be9b8u+f->r[2]*8u);
    /* 0041f96f */
    if(mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f975 */
    a=f->r[6];b=MI_U32((0x00000014u+f->r[3]));t=mi_alu(f,a,b,32,1);
    /* 0041f978 */
    if(!mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f97e */
    f->r[7]=MI_U32((0x00000010u+f->r[3]));
    /* 0041f981 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[1])))&0xffu)<<0);
    /* 0041f984 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 0041f987 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0041f98a */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0041f98c */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041f98e */
    if((f->flags&MI_ZF))goto mi_0041f996;
    /* 0041f990 */
    f->r[2]=MI_U32((0x00000020u+f->r[6]+f->r[0]*4u));
    /* 0041f994 */
    goto mi_0041f99a;
mi_0041f996:;
    /* 0041f996 */
    f->r[2]=MI_U32((0x00000010u+f->r[6]+f->r[0]*4u));
mi_0041f99a:;
    /* 0041f99a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041f99c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0041f9a0 */
    if(mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f9a6 */
    f->r[7]=MI_U32((0x00000008u+f->r[6]));
    /* 0041f9a9 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0041f9ab */
    if(mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f9b1 */
    a=f->r[7];b=MI_U32((0x0000000cu+f->r[3]));t=mi_alu(f,a,b,32,1);
    /* 0041f9b4 */
    if(!mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f9ba */
    f->r[2]=MI_U32((0x00000008u+f->r[3]));
    /* 0041f9bd */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0041f9c0 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0041f9c2 */
    f->r[2]=MI_U32((0x00000000u+f->r[6]));
    /* 0041f9c4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041f9c6 */
    if(mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f9cc */
    f->r[5]=MI_U32((0x00000004u+f->r[3]));
    /* 0041f9cf */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0041f9d1 */
    if(!mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f9d7 */
    f->r[2]=MI_U32((0x00000004u+f->r[6]));
    /* 0041f9da */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041f9dc */
    if(mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f9e2 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0041f9e4 */
    if(!mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041f9ea */
    f->r[2]=MI_U32((0x00000000u+f->r[6]));
    /* 0041f9ec */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 0041f9ee */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0041f9f1 */
    f->r[5]=(0x00000000u+f->r[5]+f->r[2]*8u);
    /* 0041f9f5 */
    f->r[2]=MI_U32((0x00000004u+f->r[6]));
    /* 0041f9f8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0041f9fb */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 0041f9fd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*8u);
    /* 0041fa00 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0041fa04 */
    f->r[0]=MI_U32((0x00000008u+f->r[1]));
    /* 0041fa07 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0041fa09 */
    if(mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041fa0f */
    a=f->r[0];b=MI_U32((0x0000000cu+f->r[6]));t=mi_alu(f,a,b,32,1);
    /* 0041fa12 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0041fabf;
    /* 0041fa18 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041fa19 */
    a=MI_U32((0x00000008u+f->r[7]));mi_idiv32(f,a);
    /* 0041fa1c */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 0041fa1e */
    a=f->r[2];b=MI_U32((0x0000000cu+f->r[7]));t=mi_imul(f,a,b);f->r[2]=t;
    /* 0041fa22 */
    a=f->r[2];b=MI_U32((0x00f55bb4u+f->r[0]*8u));t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041fa29 */
    f->r[0]=MI_U32((0x00000004u+f->r[7]));
    /* 0041fa2c */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0041fa30 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041fa32 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0041fa36 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041fa38 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0041fa3a */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0041fa3e */
    f->r[7]=MI_U32((0x00000000u+f->r[5]));
    /* 0041fa41 */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 0041fa43 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041fa45 */
    a=f->r[0];b=MI_U32((0x00000008u+f->r[1]));t=mi_imul(f,a,b);f->r[0]=t;
    /* 0041fa49 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041fa4a */
    a=MI_U32((0x0000000cu+f->r[6]));mi_idiv32(f,a);
    /* 0041fa4d */
    f->r[2]=MI_U32((0x00000020u+f->r[3]));
    /* 0041fa50 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fa52 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0041fa56 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fa58 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0041fa5a */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0041fa5e */
    f->r[7]=MI_U32((0x00000004u+f->r[5]));
    /* 0041fa61 */
    f->r[5]=MI_U32((0x00000024u+f->r[3]));
    /* 0041fa64 */
    f->r[0]=MI_U32((0x00000004u+f->r[0]));
    /* 0041fa67 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041fa69 */
    a=f->r[0];b=MI_U32((0x00000008u+f->r[1]));t=mi_imul(f,a,b);f->r[0]=t;
    /* 0041fa6d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041fa6e */
    a=MI_U32((0x0000000cu+f->r[6]));mi_idiv32(f,a);
    /* 0041fa71 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0041fa75 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0041fa79 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fa7b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fa7d */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0041fa7f */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 0041fa81 */
    f->r[6]=MI_U32((0x0126517cu));
    /* 0041fa87 */
    f->r[1]=f->r[0];
    /* 0041fa89 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0041fa8b */
    if(mi_signed_less(f->flags))goto mi_0041fa8f;
    /* 0041fa8d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0041fa8f:;
    /* 0041fa8f */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0041fa92 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 0041fa99 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 0041fa9d */
    f->r[7]=MI_U32((0x00000000u+f->r[1]));
    /* 0041fa9f */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0041faa1 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[7]);
    /* 0041faa3 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0041faa9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0041faab */
    if(mi_signed_less(f->flags))goto mi_0041faaf;
    /* 0041faad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0041faaf:;
    /* 0041faaf */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 0041fab1 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0041fab4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[0]));
    /* 0041fabb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0041fabd */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
mi_0041fabf:;
    /* 0041fabf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0041fac0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0041fac1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0041fac2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0041fac3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041fac6 */
    f->r[4]+=16;return;
}

void restore_widget_C_0041fad0(struct restore_widget_position_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0041fad0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041fad1 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 0041fad5 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0041fad9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041fada */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041fadb */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0041fae1 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0041fae5 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 0041faeb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041faec */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0041faf2 */
    a=MI_U16((0x00000010u+f->r[1]));b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0041faf7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041faf8 */
    if((f->flags&MI_ZF))goto mi_0041fc89;
    /* 0041fafe */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 0041fb00 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0041fb02 */
    if(mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb08 */
    a=f->r[0];b=0x000000fdu;t=mi_alu(f,a,b,32,1);
    /* 0041fb0d */
    if(!mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb13 */
    f->r[7]=MI_U32((0x00000004u+f->r[1]));
    /* 0041fb16 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0041fb19 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0041fb1b */
    f->r[3]=(0x005be9b8u+f->r[2]*8u);
    /* 0041fb22 */
    if(mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb28 */
    a=f->r[7];b=MI_U32((0x00000014u+f->r[3]));t=mi_alu(f,a,b,32,1);
    /* 0041fb2b */
    if(!mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb31 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 0041fb36 */
    f->r[5]=MI_U32((0x00000010u+f->r[3]));
    /* 0041fb39 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041fb3a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041fb3c */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 0041fb3f */
    f->r[2]=f->r[0];
    /* 0041fb41 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0041fb45 */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0041fb48 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0041fb4a */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0041fb4c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041fb4e */
    if(!(f->flags&MI_SF))goto mi_0041fb53;
    /* 0041fb50 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0041fb53:;
    /* 0041fb53 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[1])))&0xffu)<<0);
    /* 0041fb56 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041fb58 */
    if((f->flags&MI_ZF))goto mi_0041fb60;
    /* 0041fb5a */
    f->r[2]=MI_U32((0x00000020u+f->r[7]+f->r[0]*4u));
    /* 0041fb5e */
    goto mi_0041fb64;
mi_0041fb60:;
    /* 0041fb60 */
    f->r[2]=MI_U32((0x00000010u+f->r[7]+f->r[0]*4u));
mi_0041fb64:;
    /* 0041fb64 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041fb66 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0041fb6a */
    if(mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb70 */
    f->r[6]=MI_U32((0x00000008u+f->r[7]));
    /* 0041fb73 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0041fb75 */
    if(mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb7b */
    a=f->r[6];b=MI_U32((0x0000000cu+f->r[3]));t=mi_alu(f,a,b,32,1);
    /* 0041fb7e */
    if(!mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb84 */
    f->r[2]=MI_U32((0x00000008u+f->r[3]));
    /* 0041fb87 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0041fb8a */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0041fb8c */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 0041fb8e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041fb90 */
    if(mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fb96 */
    f->r[5]=MI_U32((0x00000004u+f->r[3]));
    /* 0041fb99 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0041fb9b */
    if(!mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fba1 */
    f->r[2]=MI_U32((0x00000004u+f->r[7]));
    /* 0041fba4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041fba6 */
    if(mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fbac */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0041fbae */
    if(!mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fbb4 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 0041fbb6 */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 0041fbb8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0041fbbb */
    f->r[5]=(0x00000000u+f->r[5]+f->r[2]*8u);
    /* 0041fbbf */
    f->r[2]=MI_U32((0x00000004u+f->r[7]));
    /* 0041fbc2 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0041fbc5 */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 0041fbc7 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*8u);
    /* 0041fbca */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0041fbce */
    f->r[0]=MI_U32((0x00000008u+f->r[1]));
    /* 0041fbd1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0041fbd3 */
    if(mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fbd9 */
    a=f->r[0];b=MI_U32((0x0000000cu+f->r[7]));t=mi_alu(f,a,b,32,1);
    /* 0041fbdc */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0041fc89;
    /* 0041fbe2 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041fbe3 */
    a=MI_U32((0x00000008u+f->r[6]));mi_idiv32(f,a);
    /* 0041fbe6 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 0041fbe8 */
    a=f->r[2];b=MI_U32((0x0000000cu+f->r[6]));t=mi_imul(f,a,b);f->r[2]=t;
    /* 0041fbec */
    a=f->r[2];b=MI_U32((0x00f55bb4u+f->r[0]*8u));t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041fbf3 */
    f->r[0]=MI_U32((0x00000004u+f->r[6]));
    /* 0041fbf6 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 0041fbfa */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041fbfc */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0041fc00 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041fc02 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0041fc04 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0041fc08 */
    f->r[6]=MI_U32((0x00000000u+f->r[5]));
    /* 0041fc0b */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 0041fc0d */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041fc0f */
    a=f->r[0];b=MI_U32((0x00000008u+f->r[1]));t=mi_imul(f,a,b);f->r[0]=t;
    /* 0041fc13 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041fc14 */
    a=MI_U32((0x0000000cu+f->r[7]));mi_idiv32(f,a);
    /* 0041fc17 */
    f->r[2]=MI_U32((0x00000020u+f->r[3]));
    /* 0041fc1a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fc1c */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0041fc20 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fc22 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0041fc24 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0041fc28 */
    f->r[6]=MI_U32((0x00000004u+f->r[5]));
    /* 0041fc2b */
    f->r[5]=MI_U32((0x00000024u+f->r[3]));
    /* 0041fc2e */
    f->r[0]=MI_U32((0x00000004u+f->r[0]));
    /* 0041fc31 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041fc33 */
    a=f->r[0];b=MI_U32((0x00000008u+f->r[1]));t=mi_imul(f,a,b);f->r[0]=t;
    /* 0041fc37 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041fc38 */
    a=MI_U32((0x0000000cu+f->r[7]));mi_idiv32(f,a);
    /* 0041fc3b */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0041fc3f */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 0041fc43 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fc45 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041fc47 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0041fc49 */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 0041fc4b */
    f->r[6]=MI_U32((0x0126517cu));
    /* 0041fc51 */
    f->r[1]=f->r[0];
    /* 0041fc53 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0041fc55 */
    if(mi_signed_less(f->flags))goto mi_0041fc59;
    /* 0041fc57 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0041fc59:;
    /* 0041fc59 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0041fc5c */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 0041fc63 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0041fc67 */
    f->r[7]=MI_U32((0x00000000u+f->r[1]));
    /* 0041fc69 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0041fc6b */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[7]);
    /* 0041fc6d */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0041fc73 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0041fc75 */
    if(mi_signed_less(f->flags))goto mi_0041fc79;
    /* 0041fc77 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0041fc79:;
    /* 0041fc79 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 0041fc7b */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0041fc7e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[0]));
    /* 0041fc85 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0041fc87 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
mi_0041fc89:;
    /* 0041fc89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0041fc8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0041fc8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0041fc8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0041fc8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0041fc8e */
    f->r[4]+=20;return;
}
