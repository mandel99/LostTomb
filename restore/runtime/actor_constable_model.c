/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_constable.h"
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
static __inline uint32_t mi_alu(struct restore_constable_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_constable_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_constable_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


void restore_constable_C_004abe40(struct restore_constable_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004abe40 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004abe45 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004abe47 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abe48 */
    f->r[3]=0x00000001u;
    /* 004abe4d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004abe50 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abe51 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004abe54 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004abe56 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abe57 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004abe5a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abe5b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[1])))&0xffu)<<0);
    /* 004abe61 */
    MI_U8((0x00bcd66au+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004abe67 */
    MI_U8((0x00bcd604u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004abe6e */
    MI_U16((0x00bcd63au+f->r[1]))=(uint16_t)(0x00000280u);
    /* 004abe77 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd652u+f->r[1]));
    /* 004abe7e */
    f->r[6]=(0x00000000u+f->r[2]*8u);
    /* 004abe85 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004abe87 */
    f->r[5]=MI_U32((0x007090ecu+f->r[6]*8u));
    /* 004abe8e */
    f->r[6]=f->r[7];
    /* 004abe90 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004abe93 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004abe95 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004abe98 */
    a=MI_U8((0x007999d8u+f->r[6]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004abe9e */
    if(!(f->flags&MI_ZF))goto mi_004abeab;
    /* 004abea0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a02u+f->r[6]));
    /* 004abea7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004abea9 */
    if((f->flags&MI_ZF))goto mi_004abeb2;
mi_004abeab:;
    /* 004abeab */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000002u);
mi_004abeb2:;
    /* 004abeb2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd5f9u+f->r[1])))&0xffu)<<0);
    /* 004abeb8 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004abeba */
    a=(f->r[2]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004abebd */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 004abec3 */
    if((f->flags&MI_CF))goto mi_004abeda;
    /* 004abec5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abec6 */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004abecd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004abed2u;f->boundary(0x004abecdu,0x0048e6f0u,f);
    /* 004abed2 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004abed7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004abeda:;
    /* 004abeda */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004abedd */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004abee0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004abee2 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004abee5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[1])))&0xffu)<<0);
    /* 004abeeb */
    a=(f->r[2]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004abeee */
    if(!(f->flags&MI_ZF))goto mi_004abf0a;
    /* 004abef0 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abef2 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abef4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abef5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004abefau;f->boundary(0x004abef5u,0x0053b4a0u,f);
    /* 004abefa */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004abeff */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abf00 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004abf05u;f->boundary(0x004abf00u,0x004b17d0u,f);
    /* 004abf05 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004abf08 */
    goto mi_004abf18;
mi_004abf0a:;
    /* 004abf0a */
    a=(f->r[2]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004abf0d */
    if(!(f->flags&MI_ZF))goto mi_004abf43;
    /* 004abf0f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abf10 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004abf15u;f->boundary(0x004abf10u,0x004b1d50u,f);
mi_004abf15:;
    /* 004abf15 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004abf18:;
    /* 004abf18 */
    f->r[0]=MI_U32((0x00d056f8u));
mi_004abf1d:;
    /* 004abf1d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004abf20 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004abf23 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004abf25 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004abf28 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[1])))&0xffu)<<0);
    /* 004abf2e */
    a=(f->r[3]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004abf31 */
    if(!(f->flags&MI_ZF))goto mi_004ac360;
    /* 004abf37 */
    f->r[6]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00bcd60bu+f->r[1]));
    /* 004abf3e */
    goto mi_004ac377;
mi_004abf43:;
    /* 004abf43 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004abf46 */
    if(!(f->flags&MI_ZF))goto mi_004ac02e;
    /* 004abf4c */
    MI_U8((0x00bcd65du+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004abf52 */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004abf59 */
    a=MI_U16((0x00bcd62eu+f->r[1]));b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 004abf60 */
    a=MI_U16((0x00bcd62eu+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004abf68 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004abf1d;
    /* 004abf6a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004abf71 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004abf78 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004abf7a */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abf7c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004abf82 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abf83 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abf84 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abf85 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004abf8au;f->boundary(0x004abf85u,0x0047fe60u,f);
    /* 004abf8a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004abf8d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004abf8f */
    if(!(f->flags&MI_ZF))goto mi_004abfab;
    /* 004abf91 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004abf96 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004abf99 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004abf9c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004abf9e */
    MI_U8((0x00bcd606u+f->r[1]*4u))=(uint8_t)(0x00000002u);
    /* 004abfa6 */
    goto mi_004abf1d;
mi_004abfab:;
    /* 004abfab */
    f->r[1]=MI_U32((0x00d056f8u));
    /* 004abfb1 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b4e4u)))&0xffffu)<<0);
    /* 004abfb8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abfba */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004abfbd */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004abfc0 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004abfc2 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004abfc5 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 004abfcc */
    MI_U16((0x00bcd61cu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004abfd3 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b4e0u)))&0xffffu)<<0);
    /* 004abfda */
    MI_U16((0x00bcd61eu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004abfe1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 004abfe4 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004abfe7 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004abfea */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abfeb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[0]));
    /* 004abff2 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004abff5 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004abff8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004abff9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd644u+f->r[0]));
    /* 004ac000 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd642u+f->r[0]));
    /* 004ac007 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac008 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac009 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac00a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac00fu;f->boundary(0x004ac00au,0x004b4220u,f);
    /* 004ac00f */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004ac012:;
    /* 004ac012 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac017 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac01a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac01d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac01f */
    MI_U16((0x00bcd63cu+f->r[1]*4u))=(uint16_t)(0x00000000u);
    /* 004ac029 */
    goto mi_004abf1d;
mi_004ac02e:;
    /* 004ac02e */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004ac031 */
    if(!(f->flags&MI_ZF))goto mi_004ac091;
    /* 004ac033 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac034 */
    MI_U8((0x00bcd604u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ac03a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac03b */
    MI_U8((0x00bcd65du+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 004ac041 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac046u;f->boundary(0x004ac041u,0x004b3ff0u,f);
    /* 004ac046 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac049 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004ac04b */
    if(!(f->flags&MI_ZF))goto mi_004abf18;
    /* 004ac051 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac056 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac059 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac05c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac05e */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac061 */
    f->r[2]=MI_U32((0x00bcd618u+f->r[1]));
    /* 004ac067 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[2]*2u));
    /* 004ac06f */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004ac071 */
    if(!(f->flags&MI_ZF))goto mi_004ac07f;
mi_004ac073:;
    /* 004ac073 */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000002u);
    /* 004ac07a */
    goto mi_004abf1d;
mi_004ac07f:;
    /* 004ac07f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac080 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000au);
    /* 004ac087 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac08cu;f->boundary(0x004ac087u,0x004b1df0u,f);
    /* 004ac08c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac08f */
    goto mi_004ac012;
mi_004ac091:;
    /* 004ac091 */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004ac094 */
    if(!(f->flags&MI_ZF))goto mi_004ac158;
    /* 004ac09a */
    MI_U8((0x00bcd65du+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004ac0a1 */
    a=MI_U16((0x00bcd63cu+f->r[1]));b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd63cu+f->r[1]))=(uint16_t)(t);
    /* 004ac0a8 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00bcd63cu+f->r[1])))&0xffffu)<<0);
    /* 004ac0af */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00bcd63au+f->r[1]));t=mi_alu(f,a,b,16,1);
    /* 004ac0b6 */
    if(mi_signed_less(f->flags))goto mi_004ac149;
    /* 004ac0bc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004ac0c3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004ac0ca */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ac0cc */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac0ce */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004ac0d4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac0d5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac0d6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac0d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac0dcu;f->boundary(0x004ac0d7u,0x0047fe60u,f);
    /* 004ac0dc */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac0df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ac0e1 */
    if(!(f->flags&MI_ZF))goto mi_004ac107;
    /* 004ac0e3 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac0e8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac0e9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac0ea */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac0ed */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ac0f0 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ac0f2 */
    MI_U8((0x00bcd606u+f->r[2]*4u))=(uint8_t)(0x00000002u);
    /* 004ac0fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac0ffu;f->boundary(0x004ac0fau,0x004b1ef0u,f);
    /* 004ac0ff */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac102 */
    goto mi_004abf18;
mi_004ac107:;
    /* 004ac107 */
    f->r[1]=MI_U32((0x00d056f8u));
    /* 004ac10d */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b4e4u)))&0xffffu)<<0);
    /* 004ac114 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac115 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ac118 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ac11b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ac11d */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ac120 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 004ac127 */
    MI_U16((0x00bcd61cu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ac12e */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b4e0u)))&0xffffu)<<0);
    /* 004ac135 */
    MI_U16((0x00bcd61eu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ac13c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac141u;f->boundary(0x004ac13cu,0x004b3250u,f);
    /* 004ac141 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac146 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004ac149:;
    /* 004ac149 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac14a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac14b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac150u;f->boundary(0x004ac14bu,0x004b1ef0u,f);
    /* 004ac150 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac153 */
    goto mi_004abf18;
mi_004ac158:;
    /* 004ac158 */
    a=(f->r[2]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004ac15b */
    if(!(f->flags&MI_ZF))goto mi_004ac20e;
    /* 004ac161 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac162 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac163 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac168u;f->boundary(0x004ac163u,0x004a5da0u,f);
    /* 004ac168 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac16d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac170 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac173 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac176 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac178 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac17b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd609u+f->r[1])))&0xffu)<<0);
    /* 004ac181 */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004ac184 */
    if((f->flags&MI_ZF))goto mi_004ac073;
    /* 004ac18a */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004ac18d */
    if((f->flags&MI_ZF))goto mi_004ac073;
    /* 004ac193 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004ac196 */
    if(!(f->flags&MI_ZF))goto mi_004abf1d;
    /* 004ac19c */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999deu+f->r[6])))&0xffffu)<<0);
    /* 004ac1a3 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x007999e0u+f->r[6])))&0xffffu)<<0);
    /* 004ac1aa */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000009u);
    /* 004ac1b1 */
    MI_U16((0x00bcd61cu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ac1b8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 004ac1bb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac1bd */
    MI_U16((0x00bcd61eu+f->r[1]))=(uint16_t)((f->r[6]&0xffffu));
    /* 004ac1c4 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004ac1c7 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004ac1ca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac1cb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[1]));
    /* 004ac1d2 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004ac1d5 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004ac1d8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac1d9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd644u+f->r[1]));
    /* 004ac1e0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd642u+f->r[1]));
    /* 004ac1e7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac1e8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac1e9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac1ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac1efu;f->boundary(0x004ac1eau,0x004b4220u,f);
    /* 004ac1ef */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac1f4 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac1f7 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac1fa */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ac1fd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ac1ff */
    MI_U16((0x00bcd63cu+f->r[2]*4u))=(uint16_t)(0x00000000u);
    /* 004ac209 */
    goto mi_004abf1d;
mi_004ac20e:;
    /* 004ac20e */
    a=(f->r[2]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 004ac211 */
    if(!(f->flags&MI_ZF))goto mi_004ac354;
    /* 004ac217 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac218 */
    MI_U8((0x00bcd66au+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004ac21f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac224u;f->boundary(0x004ac21fu,0x004b46b0u,f);
    /* 004ac224 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac227 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ac229 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac22e */
    if(!(f->flags&MI_ZF))goto mi_004ac2e0;
    /* 004ac234 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac237 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ac239 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac23c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac23e */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac240 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd652u+f->r[1]*4u));
    /* 004ac248 */
    f->r[0]=f->r[1];
    /* 004ac24a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ac24d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ac24f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ac252 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[0])))&0xffu)<<0);
    /* 004ac258 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004ac25f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac260 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac261 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004ac268 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac269 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac26eu;f->boundary(0x004ac269u,0x0047fe60u,f);
    /* 004ac26e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac271 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ac273 */
    if(!(f->flags&MI_ZF))goto mi_004ac28c;
    /* 004ac275 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac27a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac27d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac280 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac282 */
    MI_U8((0x00bcd606u+f->r[1]*4u))=(uint8_t)(0x00000002u);
    /* 004ac28a */
    goto mi_004ac2e0;
mi_004ac28c:;
    /* 004ac28c */
    f->r[1]=MI_U32((0x00d056f8u));
    /* 004ac292 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b4e4u)))&0xffffu)<<0);
    /* 004ac299 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac29a */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ac29d */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ac2a0 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ac2a2 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ac2a5 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 004ac2ac */
    MI_U16((0x00bcd61cu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ac2b3 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b4e0u)))&0xffffu)<<0);
    /* 004ac2ba */
    MI_U16((0x00bcd61eu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ac2c1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac2c6u;f->boundary(0x004ac2c1u,0x004b3250u,f);
    /* 004ac2c6 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac2cb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac2ce */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac2d1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac2d4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac2d6 */
    MI_U16((0x00bcd63cu+f->r[1]*4u))=(uint16_t)(0x00000000u);
mi_004ac2e0:;
    /* 004ac2e0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac2e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac2e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac2e7u;f->boundary(0x004ac2e2u,0x004a5da0u,f);
    /* 004ac2e7 */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac2ec */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac2ef */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac2f2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac2f5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac2f7 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac2fa */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd609u+f->r[1])))&0xffu)<<0);
    /* 004ac300 */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004ac303 */
    if((f->flags&MI_ZF))goto mi_004ac073;
    /* 004ac309 */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004ac30c */
    if((f->flags&MI_ZF))goto mi_004ac073;
    /* 004ac312 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004ac315 */
    if(!(f->flags&MI_ZF))goto mi_004abf1d;
    /* 004ac31b */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd67au+f->r[1]));
    /* 004ac322 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac323 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004ac326 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ac329 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ac32b */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ac32e */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00bcd60cu+f->r[2])))&0xffffu)<<0);
    /* 004ac335 */
    MI_U16((0x00bcd61cu+f->r[1]))=(uint16_t)((f->r[6]&0xffffu));
    /* 004ac33c */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00bcd60eu+f->r[2])))&0xffffu)<<0);
    /* 004ac343 */
    MI_U16((0x00bcd61eu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ac34a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac34fu;f->boundary(0x004ac34au,0x004b3250u,f);
    /* 004ac34f */
    goto mi_004abf15;
mi_004ac354:;
    /* 004ac354 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
    /* 004ac35b */
    goto mi_004abf1d;
mi_004ac360:;
    /* 004ac360 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd609u+f->r[1])))&0xffu)<<0);
    /* 004ac366 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004ac369 */
    if(mi_signed_less(f->flags))goto mi_004ac374;
    /* 004ac36b */
    f->r[6]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00bcd60au+f->r[1]));
    /* 004ac372 */
    goto mi_004ac377;
mi_004ac374:;
    /* 004ac374 */
    f->r[6]=(uint32_t)(int32_t)(int8_t)(f->r[2]&0xffu);
mi_004ac377:;
    /* 004ac377 */
    a=f->r[6];b=MI_U32((0x00bca2e0u));t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004ac37d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004ac37f */
    MI_U32((0x00e94e00u))=(uint32_t)(f->r[6]);
    /* 004ac385 */
    if(!mi_signed_less(f->flags))goto mi_004ac390;
    /* 004ac387 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004ac38a */
    MI_U32((0x00e94e00u))=(uint32_t)(f->r[6]);
mi_004ac390:;
    /* 004ac390 */
    a=(f->r[3]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004ac393 */
    if(!(f->flags&MI_ZF))goto mi_004ac3fd;
    /* 004ac395 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac397 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac399 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ac39a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ac39fu;f->boundary(0x004ac39au,0x0053b4a0u,f);
    /* 004ac39f */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004ac3a4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ac3a7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ac3aa */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac3ad */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ac3af */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ac3b2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd662u+f->r[1])))&0xffu)<<0);
    /* 004ac3b8 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004ac3ba */
    if(!(f->flags&MI_CF))goto mi_004ac3d4;
    /* 004ac3bc */
    f->r[0]=MI_U32((0x00e94e00u));
    /* 004ac3c1 */
    f->r[2]=MI_U32((0x00f55d9cu));
    /* 004ac3c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ac3c8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ac3ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ac3cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ac3cc */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004ac3d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ac3d3 */
    f->r[4]+=4;return;
mi_004ac3d4:;
    /* 004ac3d4 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ac3d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ac3da */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ac3dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ac3de */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ac3df */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ac3e1 */
    f->r[2]=MI_U32((0x00f55d9cu));
    /* 004ac3e7 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004ac3e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ac3ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ac3eb */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*8u);
    /* 004ac3ee */
    f->r[2]=MI_U32((0x00e94e00u));
    /* 004ac3f4 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ac3f6 */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004ac3fc */
    f->r[4]+=4;return;
mi_004ac3fd:;
    /* 004ac3fd */
    a=(f->r[3]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004ac400 */
    if(!(f->flags&MI_ZF))goto mi_004ac428;
    /* 004ac402 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd62eu+f->r[1]));
    /* 004ac409 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ac40a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ac40c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ac40e */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004ac410 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ac411 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ac412 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x005d048cu+f->r[0])))&0xffu)<<0);
    /* 004ac418 */
    f->r[0]=MI_U32((0x00f55d94u));
    /* 004ac41d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ac41f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ac420 */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004ac426 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ac427 */
    f->r[4]+=4;return;
mi_004ac428:;
    /* 004ac428 */
    f->r[2]=MI_U32((0x00f55d8cu));
    /* 004ac42e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ac430 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd5f9u+f->r[1])))&0xffu)<<0);
    /* 004ac436 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ac437 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*8u);
    /* 004ac43a */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ac43c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ac43d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ac43e */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004ac444 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ac445 */
    f->r[4]+=4;return;
}

void restore_constable_C_0048e6f0(struct restore_constable_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048e6f0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0048e6f5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e6f6 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0048e6fc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e6fd */
    f->r[1]=MI_U32((0x00ea5db4u+f->r[0]));
    /* 0048e703 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0048e705 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0048e715;
    /* 0048e707 */
    f->r[0]=MI_U32((0x00d0563cu));
    /* 0048e70c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048e70e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0048e715;
    /* 0048e710 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e711 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e713 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e714 */
    f->r[4]+=4;return;
mi_0048e715:;
    /* 0048e715 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 0048e719 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 0048e71c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048e71f */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048e721 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048e724 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[6])))&0xffu)<<0);
    /* 0048e72a */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0048e72c */
    if(!(f->flags&MI_ZF))goto mi_0048e733;
    /* 0048e72e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e72f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e731 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e732 */
    f->r[4]+=4;return;
mi_0048e733:;
    /* 0048e733 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0048e735 */
    if(!(f->flags&MI_ZF))goto mi_0048e73c;
    /* 0048e737 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e738 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e73a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e73b */
    f->r[4]+=4;return;
mi_0048e73c:;
    /* 0048e73c */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048e73e */
    if(!(f->flags&MI_ZF))goto mi_0048e745;
    /* 0048e740 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e741 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e743 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e744 */
    f->r[4]+=4;return;
mi_0048e745:;
    /* 0048e745 */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048e747 */
    if(!(f->flags&MI_ZF))goto mi_0048e74e;
    /* 0048e749 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e74a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e74c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e74d */
    f->r[4]+=4;return;
mi_0048e74e:;
    /* 0048e74e */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048e750 */
    if(!(f->flags&MI_ZF))goto mi_0048e757;
    /* 0048e752 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e753 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e755 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e756 */
    f->r[4]+=4;return;
mi_0048e757:;
    /* 0048e757 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 0048e759 */
    if(!(f->flags&MI_ZF))goto mi_0048e760;
    /* 0048e75b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e75c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e75e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e75f */
    f->r[4]+=4;return;
mi_0048e760:;
    /* 0048e760 */
    a=(f->r[0]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 0048e762 */
    if(!(f->flags&MI_ZF))goto mi_0048e769;
    /* 0048e764 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e765 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e767 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e768 */
    f->r[4]+=4;return;
mi_0048e769:;
    /* 0048e769 */
    a=(f->r[0]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 0048e76b */
    if(!(f->flags&MI_ZF))goto mi_0048e772;
    /* 0048e76d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e76e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e770 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e771 */
    f->r[4]+=4;return;
mi_0048e772:;
    /* 0048e772 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd678u+f->r[6])))&0xffu)<<0);
    /* 0048e778 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0048e77a */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 0048e77c */
    MI_U8((0x00bcd678u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0048e782 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048e784 */
    if(!(f->flags&MI_CF))goto mi_0048e78b;
    /* 0048e786 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e787 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e789 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e78a */
    f->r[4]+=4;return;
mi_0048e78b:;
    /* 0048e78b */
    MI_U8((0x00bcd678u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0048e792 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e794 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 0048e79b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 0048e7a2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e7a3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e7a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048e7a9u;f->boundary(0x0048e7a4u,0x0048e820u,f);
    /* 0048e7a9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048e7ac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048e7ae */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0048e7b5;
    /* 0048e7b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e7b1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e7b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e7b4 */
    f->r[4]+=4;return;
mi_0048e7b5:;
    /* 0048e7b5 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048e7b8 */
    MI_U8((0x00bcd678u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0048e7bf */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048e7c2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048e7c4 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 0048e7cb */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048e7ce */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e7cf */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00bcd60cu+f->r[1])))&0xffffu)<<0);
    /* 0048e7d6 */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048e7dd */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00bcd60eu+f->r[1])))&0xffffu)<<0);
    /* 0048e7e4 */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048e7eb */
    MI_U16((0x00bcd67au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048e7f2 */
    MI_U16((0x00bcd67cu+f->r[1]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0048e7f9 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00bcd67eu+f->r[1])))&0xffffu)<<0);
    /* 0048e800 */
    MI_U16((0x00bcd680u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048e807 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048e80cu;f->boundary(0x0048e807u,0x004b3250u,f);
    /* 0048e80c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048e80f */
    f->r[0]=0x00000001u;
    /* 0048e814 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e815 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e816 */
    f->r[4]+=4;return;
}

void restore_constable_C_0048e820(struct restore_constable_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048e820 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e821 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e822 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e823 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 0048e827 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e828 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0048e82a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e82b */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 0048e82f */
    MI_U32((0x00bcd5ecu))=(uint32_t)(0x00002710u);
    /* 0048e839 */
    MI_U32((0x00bca844u))=(uint32_t)(f->r[3]);
    /* 0048e83f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0048e847 */
    f->r[6]=0x00bcd786u;
mi_0048e84c:;
    /* 0048e84c */
    a=MI_U8((0x00000004u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0048e850 */
    if(!(f->flags&MI_ZF))goto mi_0048e98d;
    /* 0048e856 */
    a=MI_U16((0x0000007au+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0048e85b */
    if(!(f->flags&MI_ZF))goto mi_0048e98d;
    /* 0048e861 */
    a=f->r[3];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0048e864 */
    if(!mi_signed_less(f->flags))goto mi_0048e9a8;
    /* 0048e86a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 0048e86c */
    a=(f->r[0]&0xffu);b=0x00000029u;t=mi_alu(f,a,b,8,1);
    /* 0048e86e */
    if(!(f->flags&MI_ZF))goto mi_0048e8c0;
    /* 0048e870 */
    a=MI_U8((0x0000002eu+f->r[6]));b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 0048e874 */
    if(!(f->flags&MI_ZF))goto mi_0048e8c0;
    /* 0048e876 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000au+f->r[6]));
    /* 0048e87a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0048e87e */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048e880 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048e888;
    /* 0048e882 */
    f->r[2]=f->r[7];
    /* 0048e884 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0048e886 */
    goto mi_0048e892;
mi_0048e888:;
    /* 0048e888 */
    if(!mi_signed_less(f->flags))goto mi_0048e890;
    /* 0048e88a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048e88c */
    f->r[2]=f->r[0];
    /* 0048e88e */
    goto mi_0048e892;
mi_0048e890:;
    /* 0048e890 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0048e892:;
    /* 0048e892 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048e894 */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 0048e89a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048e8a2;
    /* 0048e89c */
    f->r[0]=f->r[5];
    /* 0048e89e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048e8a0 */
    goto mi_0048e8ac;
mi_0048e8a2:;
    /* 0048e8a2 */
    if(!mi_signed_less(f->flags))goto mi_0048e8aa;
    /* 0048e8a4 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0048e8a6 */
    f->r[0]=f->r[1];
    /* 0048e8a8 */
    goto mi_0048e8ac;
mi_0048e8aa:;
    /* 0048e8aa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0048e8ac:;
    /* 0048e8ac */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048e8ae */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[0]);
    /* 0048e8b3 */
    if(mi_signed_less(f->flags))goto mi_0048e971;
    /* 0048e8b9 */
    f->r[0]=f->r[2];
    /* 0048e8bb */
    goto mi_0048e971;
mi_0048e8c0:;
    /* 0048e8c0 */
    a=(f->r[0]&0xffu);b=0x0000002bu;t=mi_alu(f,a,b,8,1);
    /* 0048e8c2 */
    if((f->flags&MI_CF))goto mi_0048e8c8;
    /* 0048e8c4 */
    a=(f->r[0]&0xffu);b=0x00000039u;t=mi_alu(f,a,b,8,1);
    /* 0048e8c6 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_0048e8cc;
mi_0048e8c8:;
    /* 0048e8c8 */
    a=(f->r[0]&0xffu);b=0x00000063u;t=mi_alu(f,a,b,8,1);
    /* 0048e8ca */
    if(!(f->flags&MI_ZF))goto mi_0048e911;
mi_0048e8cc:;
    /* 0048e8cc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000au+f->r[6]));
    /* 0048e8d0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0048e8d4 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048e8d6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048e8de;
    /* 0048e8d8 */
    f->r[2]=f->r[7];
    /* 0048e8da */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0048e8dc */
    goto mi_0048e8e8;
mi_0048e8de:;
    /* 0048e8de */
    if(!mi_signed_less(f->flags))goto mi_0048e8e6;
    /* 0048e8e0 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048e8e2 */
    f->r[2]=f->r[0];
    /* 0048e8e4 */
    goto mi_0048e8e8;
mi_0048e8e6:;
    /* 0048e8e6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0048e8e8:;
    /* 0048e8e8 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048e8ea */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 0048e8f0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048e8f8;
    /* 0048e8f2 */
    f->r[0]=f->r[5];
    /* 0048e8f4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048e8f6 */
    goto mi_0048e902;
mi_0048e8f8:;
    /* 0048e8f8 */
    if(!mi_signed_less(f->flags))goto mi_0048e900;
    /* 0048e8fa */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0048e8fc */
    f->r[0]=f->r[1];
    /* 0048e8fe */
    goto mi_0048e902;
mi_0048e900:;
    /* 0048e900 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0048e902:;
    /* 0048e902 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048e904 */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[0]);
    /* 0048e909 */
    if(mi_signed_less(f->flags))goto mi_0048e90d;
    /* 0048e90b */
    f->r[0]=f->r[2];
mi_0048e90d:;
    /* 0048e90d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048e90f */
    goto mi_0048e971;
mi_0048e911:;
    /* 0048e911 */
    a=(f->r[0]&0xffu);b=0x00000052u;t=mi_alu(f,a,b,8,1);
    /* 0048e913 */
    if((f->flags&MI_ZF))goto mi_0048e92d;
    /* 0048e915 */
    a=(f->r[0]&0xffu);b=0x00000053u;t=mi_alu(f,a,b,8,1);
    /* 0048e917 */
    if((f->flags&MI_ZF))goto mi_0048e92d;
    /* 0048e919 */
    a=(f->r[0]&0xffu);b=0x00000054u;t=mi_alu(f,a,b,8,1);
    /* 0048e91b */
    if((f->flags&MI_ZF))goto mi_0048e92d;
    /* 0048e91d */
    a=(f->r[0]&0xffu);b=0x00000067u;t=mi_alu(f,a,b,8,1);
    /* 0048e91f */
    if((f->flags&MI_ZF))goto mi_0048e92d;
    /* 0048e921 */
    a=(f->r[0]&0xffu);b=0x00000068u;t=mi_alu(f,a,b,8,1);
    /* 0048e923 */
    if((f->flags&MI_ZF))goto mi_0048e92d;
    /* 0048e925 */
    a=(f->r[0]&0xffu);b=0x00000066u;t=mi_alu(f,a,b,8,1);
    /* 0048e927 */
    if((f->flags&MI_ZF))goto mi_0048e92d;
    /* 0048e929 */
    a=(f->r[0]&0xffu);b=0x0000006du;t=mi_alu(f,a,b,8,1);
    /* 0048e92b */
    if(!(f->flags&MI_ZF))goto mi_0048e98d;
mi_0048e92d:;
    /* 0048e92d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000au+f->r[6]));
    /* 0048e931 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0048e935 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048e937 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048e93f;
    /* 0048e939 */
    f->r[2]=f->r[7];
    /* 0048e93b */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0048e93d */
    goto mi_0048e949;
mi_0048e93f:;
    /* 0048e93f */
    if(!mi_signed_less(f->flags))goto mi_0048e947;
    /* 0048e941 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048e943 */
    f->r[2]=f->r[0];
    /* 0048e945 */
    goto mi_0048e949;
mi_0048e947:;
    /* 0048e947 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0048e949:;
    /* 0048e949 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048e94b */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 0048e951 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048e959;
    /* 0048e953 */
    f->r[0]=f->r[5];
    /* 0048e955 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048e957 */
    goto mi_0048e963;
mi_0048e959:;
    /* 0048e959 */
    if(!mi_signed_less(f->flags))goto mi_0048e961;
    /* 0048e95b */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0048e95d */
    f->r[0]=f->r[1];
    /* 0048e95f */
    goto mi_0048e963;
mi_0048e961:;
    /* 0048e961 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0048e963:;
    /* 0048e963 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048e965 */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[0]);
    /* 0048e96a */
    if(mi_signed_less(f->flags))goto mi_0048e96e;
    /* 0048e96c */
    f->r[0]=f->r[2];
mi_0048e96e:;
    /* 0048e96e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
mi_0048e971:;
    /* 0048e971 */
    a=f->r[0];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0048e975 */
    if(!mi_signed_less(f->flags))goto mi_0048e98d;
    /* 0048e977 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0048e97b */
    f->r[1]=MI_U32((0x00000016u+f->r[6]));
    /* 0048e97e */
    MI_U32((0x00bca6b4u+f->r[3]*4u))=(uint32_t)(f->r[0]);
    /* 0048e985 */
    MI_U32((0x00bca524u+f->r[3]*4u))=(uint32_t)(f->r[1]);
    /* 0048e98c */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
mi_0048e98d:;
    /* 0048e98d */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0048e991 */
    a=f->r[6];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048e997 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0048e998 */
    a=f->r[6];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 0048e99e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048e9a2 */
    if(mi_signed_less(f->flags))goto mi_0048e84c;
mi_0048e9a8:;
    /* 0048e9a8 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0048e9aa */
    MI_U32((0x00bca844u))=(uint32_t)(f->r[3]);
    /* 0048e9b0 */
    if((f->flags&MI_ZF))goto mi_0048e9d3;
    /* 0048e9b2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e9b3 */
    a=0x00bca524u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e9b8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e9b9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048e9ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048e9bfu;f->boundary(0x0048e9bau,0x00518b30u,f);
    /* 0048e9bf */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048e9c2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048e9c4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048e9d3;
    /* 0048e9c6 */
    f->r[0]=MI_U32((0x00bca6b0u+f->r[0]*4u));
    /* 0048e9cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e9ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e9cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048e9d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048e9d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0048e9d2 */
    f->r[4]+=4;return;
mi_0048e9d3:;
    /* 0048e9d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048e9d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048e9d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048e9d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048e9d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048e9d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0048e9da */
    f->r[4]+=4;return;
}

void restore_constable_C_00518b30(struct restore_constable_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00518b30 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518b31 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518b32 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518b33 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518b34 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518b36 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518b3b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518b40 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518b45u;f->boundary(0x00518b40u,0x0040f8f0u,f);
    /* 00518b45 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00518b49 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518b4c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00518b4f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00518b52 */
    f->r[1]=MI_U32((0x00eadfd4u));
    /* 00518b58 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00518b5b */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 00518b60 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00518b63 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00518b67 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00518b69 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00518b6a */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00518b6f */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 00518b75 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00518b80 */
    f->r[1]=MI_U32((0x00eae004u));
    /* 00518b86 */
    f->r[0]=0x00000001u;
    /* 00518b8b */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 00518b91 */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
    /* 00518b9b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00518ba0:;
    /* 00518ba0 */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 00518ba6 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00518baa */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00518bac */
    f->r[7]=MI_U32((0x00f203a0u+f->r[1]*4u));
    /* 00518bb3 */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 00518bb6 */
    if((f->flags&MI_ZF))goto mi_00518bd1;
    /* 00518bb8 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00518bbc */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
mi_00518bbf:;
    /* 00518bbf */
    a=MI_U32((0x00000000u+f->r[2]));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00518bc1 */
    if((f->flags&MI_ZF))goto mi_00518dbc;
    /* 00518bc7 */
    f->r[6]=f->r[1];
    /* 00518bc9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00518bca */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00518bcd */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00518bcf */
    if(!(f->flags&MI_ZF))goto mi_00518bbf;
mi_00518bd1:;
    /* 00518bd1 */
    f->r[3]=MI_U32((0x00eed760u+f->r[7]*4u));
    /* 00518bd8 */
    f->r[5]=(0x00eed760u+f->r[7]*4u);
    /* 00518bdf */
    f->r[6]=(0xffffff1cu+f->r[7]);
    /* 00518be5 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00518be6 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00518bed */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518bef */
    if(!(f->flags&MI_ZF))goto mi_00518c4e;
    /* 00518bf1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518bf2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518bf7u;f->boundary(0x00518bf2u,0x0051aff0u,f);
    /* 00518bf7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518bfa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518bfc */
    if((f->flags&MI_ZF))goto mi_00518c49;
    /* 00518bfe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0b6cu+f->r[7])))&0xffu)<<0);
    /* 00518c04 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518c06 */
    if(!mi_signed_less(f->flags))goto mi_00518c1c;
    /* 00518c08 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518c0a */
    if((f->flags&MI_ZF))goto mi_00518c1c;
    /* 00518c0c */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518c0e */
    if(!(f->flags&MI_ZF))goto mi_00518c49;
    /* 00518c10 */
    f->r[1]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 00518c17 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518c1a */
    if((f->flags&MI_ZF))goto mi_00518c49;
mi_00518c1c:;
    /* 00518c1c */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518c1e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518c49;
    /* 00518c20 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518c27 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00518c2c */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518c33 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518c34 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518c39 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518c3e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518c4e;
    /* 00518c40 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518c42 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518c47 */
    goto mi_00518c4e;
mi_00518c49:;
    /* 00518c49 */
    f->r[0]=MI_U32((0x00eae008u));
mi_00518c4e:;
    /* 00518c4e */
    f->r[6]=(0x00000001u+f->r[7]);
    /* 00518c51 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00518c58 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518c5a */
    if(!(f->flags&MI_ZF))goto mi_00518cb9;
    /* 00518c5c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518c5d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518c62u;f->boundary(0x00518c5du,0x0051aff0u,f);
    /* 00518c62 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518c65 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518c67 */
    if((f->flags&MI_ZF))goto mi_00518cb4;
    /* 00518c69 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c51u+f->r[7])))&0xffu)<<0);
    /* 00518c6f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518c71 */
    if(!mi_signed_less(f->flags))goto mi_00518c87;
    /* 00518c73 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518c75 */
    if((f->flags&MI_ZF))goto mi_00518c87;
    /* 00518c77 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518c79 */
    if(!(f->flags&MI_ZF))goto mi_00518cb4;
    /* 00518c7b */
    f->r[1]=MI_U32((0x00af5990u+f->r[7]*4u));
    /* 00518c82 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518c85 */
    if((f->flags&MI_ZF))goto mi_00518cb4;
mi_00518c87:;
    /* 00518c87 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518c89 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518cb4;
    /* 00518c8b */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518c92 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00518c97 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518c9e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518c9f */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518ca4 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518ca9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518cb9;
    /* 00518cab */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518cad */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518cb2 */
    goto mi_00518cb9;
mi_00518cb4:;
    /* 00518cb4 */
    f->r[0]=MI_U32((0x00eae008u));
mi_00518cb9:;
    /* 00518cb9 */
    f->r[6]=(0x000000e4u+f->r[7]);
    /* 00518cbf */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00518cc6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518cc8 */
    if(!(f->flags&MI_ZF))goto mi_00518d27;
    /* 00518cca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518ccb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518cd0u;f->boundary(0x00518ccbu,0x0051aff0u,f);
    /* 00518cd0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518cd3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518cd5 */
    if((f->flags&MI_ZF))goto mi_00518d22;
    /* 00518cd7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0d34u+f->r[7])))&0xffu)<<0);
    /* 00518cdd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518cdf */
    if(!mi_signed_less(f->flags))goto mi_00518cf5;
    /* 00518ce1 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518ce3 */
    if((f->flags&MI_ZF))goto mi_00518cf5;
    /* 00518ce5 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518ce7 */
    if(!(f->flags&MI_ZF))goto mi_00518d22;
    /* 00518ce9 */
    f->r[1]=MI_U32((0x00af5d1cu+f->r[7]*4u));
    /* 00518cf0 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518cf3 */
    if((f->flags&MI_ZF))goto mi_00518d22;
mi_00518cf5:;
    /* 00518cf5 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518cf7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518d22;
    /* 00518cf9 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518d00 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00518d05 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518d0c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518d0d */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518d12 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518d17 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518d27;
    /* 00518d19 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518d1b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518d20 */
    goto mi_00518d27;
mi_00518d22:;
    /* 00518d22 */
    f->r[0]=MI_U32((0x00eae008u));
mi_00518d27:;
    /* 00518d27 */
    f->r[1]=MI_U32((0xfffffffcu+f->r[5]));
    /* 00518d2a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518d2c */
    if(!(f->flags&MI_ZF))goto mi_00518d8a;
    /* 00518d2e */
    f->r[6]=(0xffffffffu+f->r[7]);
    /* 00518d31 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518d32 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518d37u;f->boundary(0x00518d32u,0x0051aff0u,f);
    /* 00518d37 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518d3a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518d3c */
    if((f->flags&MI_ZF))goto mi_00518d85;
    /* 00518d3e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c4fu+f->r[7])))&0xffu)<<0);
    /* 00518d44 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518d46 */
    if(!mi_signed_less(f->flags))goto mi_00518d5c;
    /* 00518d48 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518d4a */
    if((f->flags&MI_ZF))goto mi_00518d5c;
    /* 00518d4c */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518d4e */
    if(!(f->flags&MI_ZF))goto mi_00518d85;
    /* 00518d50 */
    f->r[1]=MI_U32((0x00af5988u+f->r[7]*4u));
    /* 00518d57 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518d5a */
    if((f->flags&MI_ZF))goto mi_00518d85;
mi_00518d5c:;
    /* 00518d5c */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518d5e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518d85;
    /* 00518d60 */
    MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(f->r[3]);
    /* 00518d63 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00518d68 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518d6f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518d70 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518d75 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518d7a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518d8a;
    /* 00518d7c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518d7e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518d83 */
    goto mi_00518d8a;
mi_00518d85:;
    /* 00518d85 */
    f->r[0]=MI_U32((0x00eae008u));
mi_00518d8a:;
    /* 00518d8a */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 00518d90 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00518d91 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518d97 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 00518d9d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518da9;
    /* 00518d9f */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
mi_00518da9:;
    /* 00518da9 */
    a=MI_U32((0x00eae00cu));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00518daf */
    if(!(f->flags&MI_ZF))goto mi_00518ba0;
mi_00518db5:;
    /* 00518db5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00518db6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00518db7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00518db8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518dba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00518dbb */
    f->r[4]+=4;return;
mi_00518dbc:;
    /* 00518dbc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518dbe */
    if(mi_signed_less(f->flags))goto mi_00518db5;
    /* 00518dc0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00518dc1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00518dc2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00518dc3 */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 00518dc6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00518dc7 */
    f->r[4]+=4;return;
}
