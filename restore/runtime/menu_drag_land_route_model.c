/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "land_route_model.h"
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

static __inline void mi_idiv32(struct restore_menu_input_frame *f,uint32_t divisor_bits)
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

void restore_C_0051bc60(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051bc60 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc61 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc62 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc63 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc64 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc65 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc67 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc6c */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bc71 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0051bc79 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bc7eu;f->boundary(0x0051bc79u,0x0040f8f0u,f);
    /* 0051bc7e */
    f->r[6]=MI_U32((0x0000002cu+f->r[4]));
    /* 0051bc82 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0051bc88 */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 0051bc8c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bc8f */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0051bc92 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 0051bc95 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051bc98 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0051bc9b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0c50u+f->r[0]+f->r[7]*1u)))&0xffu)<<0);
    /* 0051bca2 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051bca4 */
    a=(f->r[1]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051bca7 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051bcac */
    f->r[3]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0051bcb3 */
    if((f->flags&MI_ZF))goto mi_0051bcbd;
    /* 0051bcb5 */
    a=f->r[3];b=0x12040011u;t=mi_alu(f,a,b,32,3);
    /* 0051bcbb */
    if((f->flags&MI_ZF))goto mi_0051bcff;
mi_0051bcbd:;
    /* 0051bcbd */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0051bcc1 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051bcc3 */
    if((f->flags&MI_ZF))goto mi_0051c383;
    /* 0051bcc9 */
    a=((f->r[3]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051bccc */
    if(!(f->flags&MI_ZF))goto mi_0051bcff;
    /* 0051bcce */
    a=(f->r[3]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0051bcd1 */
    if((f->flags&MI_ZF))goto mi_0051c383;
    /* 0051bcd7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[0]*2u));
    /* 0051bcdf */
    f->r[5]=f->r[2];
    /* 0051bce1 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0051bce4 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0051bce6 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[5]*8u)))&0xffffu)<<0);
    /* 0051bcee */
    a=(f->r[2]&0xffffu);b=0x0000005au;t=mi_alu(f,a,b,16,1);
    /* 0051bcf2 */
    if((f->flags&MI_ZF))goto mi_0051bcff;
    /* 0051bcf4 */
    a=(f->r[2]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0051bcf9 */
    if(!(f->flags&MI_ZF))goto mi_0051c383;
mi_0051bcff:;
    /* 0051bcff */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051bd02 */
    if((f->flags&MI_ZF))goto mi_0051c383;
    /* 0051bd08 */
    a=((f->r[3]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051bd0b */
    if((f->flags&MI_ZF))goto mi_0051bd3d;
    /* 0051bd0d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bd0e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bd13u;f->boundary(0x0051bd0eu,0x0051c390u,f);
    /* 0051bd13 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bd16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bd18 */
    if((f->flags&MI_ZF))goto mi_0051bd38;
    /* 0051bd1a */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051bd1e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bd20 */
    if(!(f->flags&MI_ZF))goto mi_0051bd38;
    /* 0051bd22 */
    f->r[0]=MI_U32((0x00eae004u));
    /* 0051bd27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bd28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bd2du;f->boundary(0x0051bd28u,0x0051c390u,f);
    /* 0051bd2d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bd30 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bd32 */
    if((f->flags&MI_ZF))goto mi_0051c383;
mi_0051bd38:;
    /* 0051bd38 */
    f->r[0]=MI_U32((0x00eae004u));
mi_0051bd3d:;
    /* 0051bd3d */
    a=MI_U8((0x00ee0c50u+f->r[0]));b=0x000000fbu;t=mi_alu(f,a,b,8,1);
    /* 0051bd44 */
    if(!(f->flags&MI_ZF))goto mi_0051bd68;
    /* 0051bd46 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0051bd4a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051bd4c */
    if(!(f->flags&MI_ZF))goto mi_0051c383;
    /* 0051bd52 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bd53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bd58u;f->boundary(0x0051bd53u,0x0051c510u,f);
    /* 0051bd58 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bd5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bd5d */
    if((f->flags&MI_ZF))goto mi_0051c383;
    /* 0051bd63 */
    f->r[0]=MI_U32((0x00eae004u));
mi_0051bd68:;
    /* 0051bd68 */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0051bd6b */
    if((f->flags&MI_ZF))goto mi_0051bd86;
    /* 0051bd6d */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0051bd71 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051bd73 */
    if((f->flags&MI_ZF))goto mi_0051bd86;
    /* 0051bd75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bd76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bd7bu;f->boundary(0x0051bd76u,0x0051c630u,f);
    /* 0051bd7b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bd7e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bd80 */
    if((f->flags&MI_ZF))goto mi_0051c383;
mi_0051bd86:;
    /* 0051bd86 */
    f->r[1]=MI_U32((0x00bca2ecu));
    /* 0051bd8c */
    f->r[2]=MI_U32((0x00bca2e8u));
    /* 0051bd92 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051bd94 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bd96 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051bd98 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bd99 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bd9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bd9fu;f->boundary(0x0051bd9au,0x0046f9a0u,f);
    /* 0051bd9f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bda2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051bda4 */
    if(!(f->flags&MI_ZF))goto mi_0051c383;
    /* 0051bdaa */
    f->r[7]=MI_U32((0x00eadfd4u));
    /* 0051bdb0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bdb2 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051bdb3 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bdb8 */
    a=0x00eae010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bdbd */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 0051bdc3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bdc8u;f->boundary(0x0051bdc3u,0x0040f8f0u,f);
    /* 0051bdc8 */
    f->r[0]=MI_U32((0x00eae004u));
    /* 0051bdcd */
    f->r[1]=0x00000001u;
    /* 0051bdd2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bdd5 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0051bddc */
    f->r[2]=MI_U32((0x00eae004u));
    /* 0051bde2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051bde4 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[2]);
    /* 0051bdea */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[0]);
    /* 0051bdef */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[1]);
mi_0051bdf5:;
    /* 0051bdf5 */
    f->r[5]=MI_U32((0x00f203a0u+f->r[0]*4u));
    /* 0051bdfc */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0051be00 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051be01 */
    f->r[0]=MI_U32((0x00eed760u+f->r[5]*4u));
    /* 0051be08 */
    f->r[6]=(0xffffff1cu+f->r[5]);
    /* 0051be0e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051be0f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051be13 */
    f->r[3]=(0x00eed760u+f->r[6]*4u);
    /* 0051be1a */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051be1e */
    a=MI_U32((0x00000000u+f->r[3]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 0051be21 */
    if(!(f->flags&MI_ZF))goto mi_0051bf5a;
    /* 0051be27 */
    f->r[0]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 0051be2e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0051be30 */
    a=((f->r[0]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051be33 */
    if((f->flags&MI_ZF))goto mi_0051be52;
    /* 0051be35 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051be36 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051be3bu;f->boundary(0x0051be36u,0x0051c390u,f);
    /* 0051be3b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051be3e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051be40 */
    if((f->flags&MI_ZF))goto mi_0051be52;
    /* 0051be42 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051be46 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051be48 */
    if(!(f->flags&MI_ZF))goto mi_0051bec9;
    /* 0051be4a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051be4b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051be50u;f->boundary(0x0051be4bu,0x0051c390u,f);
    /* 0051be50 */
    goto mi_0051be6b;
mi_0051be52:;
    /* 0051be52 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0b6cu+f->r[5])))&0xffu)<<0);
    /* 0051be58 */
    a=(f->r[1]&0xffu);b=0x000000fbu;t=mi_alu(f,a,b,8,1);
    /* 0051be5b */
    if(!(f->flags&MI_ZF))goto mi_0051be7a;
    /* 0051be5d */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051be61 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051be63 */
    if(!(f->flags&MI_ZF))goto mi_0051bec4;
    /* 0051be65 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051be66 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051be6bu;f->boundary(0x0051be66u,0x0051c510u,f);
mi_0051be6b:;
    /* 0051be6b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051be6e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051be70 */
    if(!(f->flags&MI_ZF))goto mi_0051bec9;
    /* 0051be72 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0xffffffffu);
    /* 0051be78 */
    goto mi_0051bec4;
mi_0051be7a:;
    /* 0051be7a */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051be7d */
    if((f->flags&MI_ZF))goto mi_0051bec4;
    /* 0051be7f */
    a=(f->r[1]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051be82 */
    if((f->flags&MI_ZF))goto mi_0051bec4;
    /* 0051be84 */
    f->r[0]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 0051be8b */
    a=f->r[0];b=0x12040011u;t=mi_alu(f,a,b,32,3);
    /* 0051be90 */
    if(!(f->flags&MI_ZF))goto mi_0051bec4;
    /* 0051be92 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0051be94 */
    if((f->flags&MI_ZF))goto mi_0051bec9;
    /* 0051be96 */
    a=MI_U32((0x00000024u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051be9b */
    if(!(f->flags&MI_ZF))goto mi_0051beb8;
    /* 0051be9d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[6]*2u));
    /* 0051bea5 */
    f->r[2]=f->r[0];
    /* 0051bea7 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0051beaa */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051beac */
    a=MI_U16((0x007999e8u+f->r[2]*8u));b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0051beb6 */
    if((f->flags&MI_ZF))goto mi_0051bec9;
mi_0051beb8:;
    /* 0051beb8 */
    a=(f->r[1]&0xffu);b=0x000000fau;t=mi_alu(f,a,b,8,1);
    /* 0051bebb */
    if(!(f->flags&MI_ZF))goto mi_0051bec4;
    /* 0051bebd */
    a=MI_U32((0x00000024u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051bec2 */
    if((f->flags&MI_ZF))goto mi_0051bec9;
mi_0051bec4:;
    /* 0051bec4 */
    f->r[7]=0x00000001u;
mi_0051bec9:;
    /* 0051bec9 */
    a=MI_U8((0x00af598cu+f->r[6]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0051bed1 */
    if((f->flags&MI_ZF))goto mi_0051bef0;
    /* 0051bed3 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051bed7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bed9 */
    if((f->flags&MI_ZF))goto mi_0051bef0;
    /* 0051bedb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bedc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bee1u;f->boundary(0x0051bedcu,0x0051c630u,f);
    /* 0051bee1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bee4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bee6 */
    if(!(f->flags&MI_ZF))goto mi_0051bef0;
    /* 0051bee8 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(0xffffffffu);
    /* 0051beee */
    goto mi_0051bf5a;
mi_0051bef0:;
    /* 0051bef0 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0051bef2 */
    if(!(f->flags&MI_ZF))goto mi_0051bf5a;
    /* 0051bef4 */
    f->r[0]=0x8fb823efu;
    /* 0051bef9 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051befb */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0051befd */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051beff */
    f->r[1]=0x000000e4u;
    /* 0051bf04 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0051bf07 */
    f->r[0]=f->r[2];
    /* 0051bf09 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0051bf0c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051bf0e */
    f->r[0]=f->r[6];
    /* 0051bf10 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bf11 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0051bf12 */
    a=f->r[1];mi_idiv32(f,a);
    /* 0051bf14 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bf15 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bf1au;f->boundary(0x0051bf15u,0x0046f9a0u,f);
    /* 0051bf1a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bf1d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051bf1f */
    if(!(f->flags&MI_ZF))goto mi_0051bf5a;
    /* 0051bf21 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0051bf25 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[0]);
    /* 0051bf27 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0b6cu+f->r[5])))&0xffu)<<0);
    /* 0051bf2d */
    a=(f->r[1]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,1);
    /* 0051bf30 */
    if(!(f->flags&MI_ZF))goto mi_0051bf37;
    /* 0051bf32 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051bf35 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[0]);
mi_0051bf37:;
    /* 0051bf37 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051bf3c */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051bf43 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051bf44 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051bf49 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051bf4e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051bf5a;
    /* 0051bf50 */
    MI_U32((0x00eae008u))=(uint32_t)(0x00000000u);
mi_0051bf5a:;
    /* 0051bf5a */
    f->r[6]=(0x00000001u+f->r[5]);
    /* 0051bf5d */
    f->r[0]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051bf64 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bf66 */
    if(!(f->flags&MI_ZF))goto mi_0051c0ac;
    /* 0051bf6c */
    f->r[0]=MI_U32((0x00af5990u+f->r[5]*4u));
    /* 0051bf73 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0051bf75 */
    a=((f->r[0]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051bf78 */
    if((f->flags&MI_ZF))goto mi_0051bf97;
    /* 0051bf7a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bf7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bf80u;f->boundary(0x0051bf7bu,0x0051c390u,f);
    /* 0051bf80 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bf83 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bf85 */
    if((f->flags&MI_ZF))goto mi_0051bf97;
    /* 0051bf87 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051bf8b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bf8d */
    if(!(f->flags&MI_ZF))goto mi_0051c00c;
    /* 0051bf8f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bf90 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bf95u;f->boundary(0x0051bf90u,0x0051c390u,f);
    /* 0051bf95 */
    goto mi_0051bfb0;
mi_0051bf97:;
    /* 0051bf97 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0c51u+f->r[5])))&0xffu)<<0);
    /* 0051bf9d */
    a=(f->r[1]&0xffu);b=0x000000fbu;t=mi_alu(f,a,b,8,1);
    /* 0051bfa0 */
    if(!(f->flags&MI_ZF))goto mi_0051bfc4;
    /* 0051bfa2 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051bfa6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bfa8 */
    if(!(f->flags&MI_ZF))goto mi_0051c007;
    /* 0051bfaa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051bfab */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051bfb0u;f->boundary(0x0051bfabu,0x0051c510u,f);
mi_0051bfb0:;
    /* 0051bfb0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051bfb3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051bfb5 */
    if(!(f->flags&MI_ZF))goto mi_0051c00c;
    /* 0051bfb7 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(0xffffffffu);
    /* 0051bfc2 */
    goto mi_0051c007;
mi_0051bfc4:;
    /* 0051bfc4 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051bfc7 */
    if((f->flags&MI_ZF))goto mi_0051c007;
    /* 0051bfc9 */
    a=(f->r[1]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051bfcc */
    if((f->flags&MI_ZF))goto mi_0051c007;
    /* 0051bfce */
    f->r[0]=MI_U32((0x00af5990u+f->r[5]*4u));
    /* 0051bfd5 */
    a=f->r[0];b=0x12040011u;t=mi_alu(f,a,b,32,3);
    /* 0051bfda */
    if(!(f->flags&MI_ZF))goto mi_0051c007;
    /* 0051bfdc */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0051bfde */
    if((f->flags&MI_ZF))goto mi_0051c00c;
    /* 0051bfe0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6feu+f->r[5]*2u));
    /* 0051bfe8 */
    f->r[2]=f->r[0];
    /* 0051bfea */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0051bfed */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051bfef */
    a=MI_U16((0x007999e8u+f->r[2]*8u));b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0051bff9 */
    if((f->flags&MI_ZF))goto mi_0051c00c;
    /* 0051bffb */
    a=(f->r[1]&0xffu);b=0x000000fcu;t=mi_alu(f,a,b,8,1);
    /* 0051bffe */
    if(!(f->flags&MI_ZF))goto mi_0051c007;
    /* 0051c000 */
    a=MI_U32((0x00000024u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051c005 */
    if((f->flags&MI_ZF))goto mi_0051c00c;
mi_0051c007:;
    /* 0051c007 */
    f->r[7]=0x00000001u;
mi_0051c00c:;
    /* 0051c00c */
    a=MI_U8((0x00af5990u+f->r[5]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0051c014 */
    if((f->flags&MI_ZF))goto mi_0051c038;
    /* 0051c016 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051c01a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c01c */
    if((f->flags&MI_ZF))goto mi_0051c038;
    /* 0051c01e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c01f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c024u;f->boundary(0x0051c01fu,0x0051c630u,f);
    /* 0051c024 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c027 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c029 */
    if(!(f->flags&MI_ZF))goto mi_0051c038;
    /* 0051c02b */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(0xffffffffu);
    /* 0051c036 */
    goto mi_0051c0ac;
mi_0051c038:;
    /* 0051c038 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0051c03a */
    if(!(f->flags&MI_ZF))goto mi_0051c0ac;
    /* 0051c03c */
    f->r[0]=0x8fb823efu;
    /* 0051c041 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c043 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0051c045 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c047 */
    f->r[1]=0x000000e4u;
    /* 0051c04c */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0051c04f */
    f->r[0]=f->r[2];
    /* 0051c051 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0051c054 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c056 */
    f->r[0]=f->r[6];
    /* 0051c058 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c059 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0051c05a */
    a=f->r[1];mi_idiv32(f,a);
    /* 0051c05c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c05d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c062u;f->boundary(0x0051c05du,0x0046f9a0u,f);
    /* 0051c062 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c065 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051c067 */
    if(!(f->flags&MI_ZF))goto mi_0051c0ac;
    /* 0051c069 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0051c06d */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051c074 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0c51u+f->r[5])))&0xffu)<<0);
    /* 0051c07a */
    a=(f->r[1]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,1);
    /* 0051c07d */
    if(!(f->flags&MI_ZF))goto mi_0051c089;
    /* 0051c07f */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051c082 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
mi_0051c089:;
    /* 0051c089 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051c08e */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051c095 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051c096 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051c09b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051c0a0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051c0ac;
    /* 0051c0a2 */
    MI_U32((0x00eae008u))=(uint32_t)(0x00000000u);
mi_0051c0ac:;
    /* 0051c0ac */
    f->r[6]=(0x000000e4u+f->r[5]);
    /* 0051c0b2 */
    f->r[0]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051c0b9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c0bb */
    if(!(f->flags&MI_ZF))goto mi_0051c201;
    /* 0051c0c1 */
    f->r[0]=MI_U32((0x00af5d1cu+f->r[5]*4u));
    /* 0051c0c8 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0051c0ca */
    a=((f->r[0]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051c0cd */
    if((f->flags&MI_ZF))goto mi_0051c0ec;
    /* 0051c0cf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c0d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c0d5u;f->boundary(0x0051c0d0u,0x0051c390u,f);
    /* 0051c0d5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c0d8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c0da */
    if((f->flags&MI_ZF))goto mi_0051c0ec;
    /* 0051c0dc */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051c0e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c0e2 */
    if(!(f->flags&MI_ZF))goto mi_0051c161;
    /* 0051c0e4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c0e5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c0eau;f->boundary(0x0051c0e5u,0x0051c390u,f);
    /* 0051c0ea */
    goto mi_0051c105;
mi_0051c0ec:;
    /* 0051c0ec */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0d34u+f->r[5])))&0xffu)<<0);
    /* 0051c0f2 */
    a=(f->r[1]&0xffu);b=0x000000fbu;t=mi_alu(f,a,b,8,1);
    /* 0051c0f5 */
    if(!(f->flags&MI_ZF))goto mi_0051c119;
    /* 0051c0f7 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051c0fb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c0fd */
    if(!(f->flags&MI_ZF))goto mi_0051c15c;
    /* 0051c0ff */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c100 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c105u;f->boundary(0x0051c100u,0x0051c510u,f);
mi_0051c105:;
    /* 0051c105 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c108 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c10a */
    if(!(f->flags&MI_ZF))goto mi_0051c161;
    /* 0051c10c */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(0xffffffffu);
    /* 0051c117 */
    goto mi_0051c15c;
mi_0051c119:;
    /* 0051c119 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051c11c */
    if((f->flags&MI_ZF))goto mi_0051c15c;
    /* 0051c11e */
    a=(f->r[1]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051c121 */
    if((f->flags&MI_ZF))goto mi_0051c15c;
    /* 0051c123 */
    f->r[0]=MI_U32((0x00af5d1cu+f->r[5]*4u));
    /* 0051c12a */
    a=f->r[0];b=0x12040011u;t=mi_alu(f,a,b,32,3);
    /* 0051c12f */
    if(!(f->flags&MI_ZF))goto mi_0051c15c;
    /* 0051c131 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0051c133 */
    if((f->flags&MI_ZF))goto mi_0051c161;
    /* 0051c135 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e8c4u+f->r[5]*2u));
    /* 0051c13d */
    f->r[2]=f->r[0];
    /* 0051c13f */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0051c142 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c144 */
    a=MI_U16((0x007999e8u+f->r[2]*8u));b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0051c14e */
    if((f->flags&MI_ZF))goto mi_0051c161;
    /* 0051c150 */
    a=(f->r[1]&0xffu);b=0x000000fau;t=mi_alu(f,a,b,8,1);
    /* 0051c153 */
    if(!(f->flags&MI_ZF))goto mi_0051c15c;
    /* 0051c155 */
    a=MI_U32((0x00000024u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051c15a */
    if((f->flags&MI_ZF))goto mi_0051c161;
mi_0051c15c:;
    /* 0051c15c */
    f->r[7]=0x00000001u;
mi_0051c161:;
    /* 0051c161 */
    a=MI_U8((0x00af5d1cu+f->r[5]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0051c169 */
    if((f->flags&MI_ZF))goto mi_0051c18d;
    /* 0051c16b */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051c16f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c171 */
    if((f->flags&MI_ZF))goto mi_0051c18d;
    /* 0051c173 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c174 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c179u;f->boundary(0x0051c174u,0x0051c630u,f);
    /* 0051c179 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c17c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c17e */
    if(!(f->flags&MI_ZF))goto mi_0051c18d;
    /* 0051c180 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(0xffffffffu);
    /* 0051c18b */
    goto mi_0051c201;
mi_0051c18d:;
    /* 0051c18d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0051c18f */
    if(!(f->flags&MI_ZF))goto mi_0051c201;
    /* 0051c191 */
    f->r[0]=0x8fb823efu;
    /* 0051c196 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c198 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0051c19a */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c19c */
    f->r[1]=0x000000e4u;
    /* 0051c1a1 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0051c1a4 */
    f->r[0]=f->r[2];
    /* 0051c1a6 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0051c1a9 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c1ab */
    f->r[0]=f->r[6];
    /* 0051c1ad */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c1ae */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0051c1af */
    a=f->r[1];mi_idiv32(f,a);
    /* 0051c1b1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c1b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c1b7u;f->boundary(0x0051c1b2u,0x0046f9a0u,f);
    /* 0051c1b7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c1ba */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051c1bc */
    if(!(f->flags&MI_ZF))goto mi_0051c201;
    /* 0051c1be */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0051c1c2 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051c1c9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0d34u+f->r[5])))&0xffu)<<0);
    /* 0051c1cf */
    a=(f->r[1]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,1);
    /* 0051c1d2 */
    if(!(f->flags&MI_ZF))goto mi_0051c1de;
    /* 0051c1d4 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051c1d7 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
mi_0051c1de:;
    /* 0051c1de */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051c1e3 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051c1ea */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051c1eb */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051c1f0 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051c1f5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051c201;
    /* 0051c1f7 */
    MI_U32((0x00eae008u))=(uint32_t)(0x00000000u);
mi_0051c201:;
    /* 0051c201 */
    f->r[0]=MI_U32((0x00eed75cu+f->r[5]*4u));
    /* 0051c208 */
    f->r[3]=(0x00eed760u+f->r[5]*4u);
    /* 0051c20f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c211 */
    if(!(f->flags&MI_ZF))goto mi_0051c353;
    /* 0051c217 */
    f->r[0]=MI_U32((0x00af5988u+f->r[5]*4u));
    /* 0051c21e */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0051c220 */
    a=((f->r[0]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0051c223 */
    if((f->flags&MI_ZF))goto mi_0051c245;
    /* 0051c225 */
    f->r[7]=(0xffffffffu+f->r[5]);
    /* 0051c228 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c229 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c22eu;f->boundary(0x0051c229u,0x0051c390u,f);
    /* 0051c22e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c231 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c233 */
    if((f->flags&MI_ZF))goto mi_0051c245;
    /* 0051c235 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051c239 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c23b */
    if(!(f->flags&MI_ZF))goto mi_0051c2b9;
    /* 0051c23d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c23e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c243u;f->boundary(0x0051c23eu,0x0051c390u,f);
    /* 0051c243 */
    goto mi_0051c261;
mi_0051c245:;
    /* 0051c245 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0c4fu+f->r[5])))&0xffu)<<0);
    /* 0051c24b */
    a=(f->r[1]&0xffu);b=0x000000fbu;t=mi_alu(f,a,b,8,1);
    /* 0051c24e */
    if(!(f->flags&MI_ZF))goto mi_0051c271;
    /* 0051c250 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051c254 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c256 */
    if(!(f->flags&MI_ZF))goto mi_0051c2b4;
    /* 0051c258 */
    f->r[2]=(0xffffffffu+f->r[5]);
    /* 0051c25b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c25c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c261u;f->boundary(0x0051c25cu,0x0051c510u,f);
mi_0051c261:;
    /* 0051c261 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c264 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c266 */
    if(!(f->flags&MI_ZF))goto mi_0051c2b9;
    /* 0051c268 */
    MI_U32((0xfffffffcu+f->r[3]))=(uint32_t)(0xffffffffu);
    /* 0051c26f */
    goto mi_0051c2b4;
mi_0051c271:;
    /* 0051c271 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051c274 */
    if((f->flags&MI_ZF))goto mi_0051c2b4;
    /* 0051c276 */
    a=(f->r[1]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051c279 */
    if((f->flags&MI_ZF))goto mi_0051c2b4;
    /* 0051c27b */
    f->r[0]=MI_U32((0x00af5988u+f->r[5]*4u));
    /* 0051c282 */
    a=f->r[0];b=0x12040011u;t=mi_alu(f,a,b,32,3);
    /* 0051c287 */
    if(!(f->flags&MI_ZF))goto mi_0051c2b4;
    /* 0051c289 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0051c28b */
    if((f->flags&MI_ZF))goto mi_0051c2b9;
    /* 0051c28d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fau+f->r[5]*2u));
    /* 0051c295 */
    f->r[2]=f->r[0];
    /* 0051c297 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0051c29a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c29c */
    a=MI_U16((0x007999e8u+f->r[2]*8u));b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0051c2a6 */
    if((f->flags&MI_ZF))goto mi_0051c2b9;
    /* 0051c2a8 */
    a=(f->r[1]&0xffu);b=0x000000fcu;t=mi_alu(f,a,b,8,1);
    /* 0051c2ab */
    if(!(f->flags&MI_ZF))goto mi_0051c2b4;
    /* 0051c2ad */
    a=MI_U32((0x00000024u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051c2b2 */
    if((f->flags&MI_ZF))goto mi_0051c2b9;
mi_0051c2b4:;
    /* 0051c2b4 */
    f->r[6]=0x00000001u;
mi_0051c2b9:;
    /* 0051c2b9 */
    a=MI_U8((0x00af5988u+f->r[5]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0051c2c1 */
    if((f->flags&MI_ZF))goto mi_0051c2e4;
    /* 0051c2c3 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051c2c7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c2c9 */
    if((f->flags&MI_ZF))goto mi_0051c2e4;
    /* 0051c2cb */
    f->r[0]=(0xffffffffu+f->r[5]);
    /* 0051c2ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c2cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c2d4u;f->boundary(0x0051c2cfu,0x0051c630u,f);
    /* 0051c2d4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c2d7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051c2d9 */
    if(!(f->flags&MI_ZF))goto mi_0051c2e4;
    /* 0051c2db */
    MI_U32((0xfffffffcu+f->r[3]))=(uint32_t)(0xffffffffu);
    /* 0051c2e2 */
    goto mi_0051c353;
mi_0051c2e4:;
    /* 0051c2e4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0051c2e6 */
    if(!(f->flags&MI_ZF))goto mi_0051c353;
    /* 0051c2e8 */
    f->r[6]=(0xffffffffu+f->r[5]);
    /* 0051c2eb */
    f->r[0]=0x8fb823efu;
    /* 0051c2f0 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0051c2f2 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c2f4 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c2f6 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0051c2f9 */
    f->r[1]=f->r[2];
    /* 0051c2fb */
    f->r[0]=f->r[6];
    /* 0051c2fd */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 0051c300 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051c302 */
    f->r[1]=0x000000e4u;
    /* 0051c307 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c308 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0051c309 */
    a=f->r[1];mi_idiv32(f,a);
    /* 0051c30b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051c30c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051c311u;f->boundary(0x0051c30cu,0x0046f9a0u,f);
    /* 0051c311 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051c314 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051c316 */
    if(!(f->flags&MI_ZF))goto mi_0051c353;
    /* 0051c318 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0051c31c */
    MI_U32((0xfffffffcu+f->r[3]))=(uint32_t)(f->r[0]);
    /* 0051c31f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ee0c4fu+f->r[5])))&0xffu)<<0);
    /* 0051c325 */
    a=(f->r[1]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,1);
    /* 0051c328 */
    if(!(f->flags&MI_ZF))goto mi_0051c330;
    /* 0051c32a */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051c32d */
    MI_U32((0xfffffffcu+f->r[3]))=(uint32_t)(f->r[0]);
mi_0051c330:;
    /* 0051c330 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051c335 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051c33c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051c33d */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051c342 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051c347 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051c353;
    /* 0051c349 */
    MI_U32((0x00eae008u))=(uint32_t)(0x00000000u);
mi_0051c353:;
    /* 0051c353 */
    f->r[0]=MI_U32((0x00eae00cu));
    /* 0051c358 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051c359 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051c35e */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[0]);
    /* 0051c363 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051c36c;
    /* 0051c365 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051c367 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[0]);
mi_0051c36c:;
    /* 0051c36c */
    a=f->r[0];b=MI_U32((0x00eae008u));t=mi_alu(f,a,b,32,1);
    /* 0051c372 */
    if(!(f->flags&MI_ZF))goto mi_0051bdf5;
    /* 0051c378 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051c379 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051c37a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051c37b */
    f->r[0]=0x00000001u;
    /* 0051c380 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051c381 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0051c382 */
    f->r[4]+=4;return;
mi_0051c383:;
    /* 0051c383 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051c384 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051c385 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051c386 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051c388 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051c389 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0051c38a */
    f->r[4]+=4;return;
}
