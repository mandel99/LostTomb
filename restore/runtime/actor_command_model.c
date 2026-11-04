#include <stdlib.h>
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_command.h"

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
static __inline uint32_t mi_alu(struct restore_actor_command_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_actor_command_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_actor_command_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


static __inline void mi_idiv32(struct restore_actor_command_frame *f,uint32_t divisor_bits)
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

void restore_actor_command_C_0041cd00(struct restore_actor_command_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0041cd00 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 0041cd05 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0041cd08 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0041cd0a */
    if(!(f->flags&MI_ZF))goto mi_0041cfb9;
    /* 0041cd10 */
    f->r[0]=MI_U32((0x00bca2ccu));
    /* 0041cd15 */
    f->r[1]=MI_U32((0x00bca2c8u));
    /* 0041cd1b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd1c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd1d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd1f */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd21 */
    a=0x00000054u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041cd28u;f->boundary(0x0041cd23u,0x004b6ff0u,f);
    /* 0041cd28 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041cd2b */
    MI_U32((0x00000004u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0041cd2f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0041cd31 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0041cfb9;
    /* 0041cd37 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0041cd3a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd3b */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0041cd3e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd3f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd40 */
    MI_U8((0x00bcba31u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 0041cd47 */
    MI_U16((0x00bcba16u+f->r[0]))=(uint16_t)(0x00000018u);
    /* 0041cd50 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cd51 */
    MI_U8((0x00bcb9f1u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0041cd58 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_0041cd5a:;
    /* 0041cd5a */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 0041cd5f */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 0041cd65 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0041cd6c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0041cd6d */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0041cd70 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 0041cd75 */
    if(mi_signed_less(f->flags))goto mi_0041cd81;
    /* 0041cd77 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_0041cd81:;
    /* 0041cd81 */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 0041cd87 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 0041cd8c */
    f->r[7]=0x0000001fu;
mi_0041cd91:;
    /* 0041cd91 */
    f->r[2]=f->r[1];
    /* 0041cd93 */
    f->r[6]=f->r[1];
    /* 0041cd95 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 0041cd98 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0041cd9b */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0041cd9e */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0041cda0 */
    f->r[6]=f->r[0];
    /* 0041cda2 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 0041cda5 */
    f->r[5]=f->r[0];
    /* 0041cda7 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0041cdaa */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0041cdad */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0041cdaf */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 0041cdb1 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 0041cdb3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041cdb5 */
    if((f->flags&MI_ZF))goto mi_0041cdbd;
    /* 0041cdb7 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_0041cdbd:;
    /* 0041cdbd */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0041cdbf */
    if((f->flags&MI_ZF))goto mi_0041cdc6;
    /* 0041cdc1 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0041cdc6:;
    /* 0041cdc6 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0041cdc7 */
    if(!(f->flags&MI_ZF))goto mi_0041cd91;
    /* 0041cdc9 */
    f->r[2]=f->r[1];
    /* 0041cdcb */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 0041cdd1 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0041cdd7 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 0041cddc */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 0041cde2 */
    f->r[2]=f->r[1];
    /* 0041cde4 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0041cde7 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0041cdea */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[2]);
    /* 0041cdf0 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 0041cdf6 */
    f->r[1]=f->r[0];
    /* 0041cdf8 */
    f->r[2]=f->r[0];
    /* 0041cdfa */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0041ce00 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0041ce03 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0041ce06 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 0041ce0c */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 0041ce12 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 0041ce17 */
    f->r[6]=f->r[3];
    /* 0041ce19 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0041ce1b:;
    /* 0041ce1b */
    f->r[0]=MI_U32((0x005cd97cu+f->r[6]*8u));
    /* 0041ce22 */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 0041ce28 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041ce2a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0041ce2d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0041ce30 */
    f->r[1]=MI_U32((0x00bca2ccu));
    /* 0041ce36 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0041ce39 */
    f->r[0]=MI_U32((0x005cd978u+f->r[6]*8u));
    /* 0041ce40 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0041ce43 */
    f->r[2]=MI_U32((0x00bca2c8u));
    /* 0041ce49 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041ce4b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041ce4d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041ce4f */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0041ce56 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041ce57 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041ce5cu;f->boundary(0x0041ce57u,0x004ec410u,f);
    /* 0041ce5c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041ce5f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041ce61 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0041ce64 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0041ce65 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0041ce68 */
    if(mi_signed_less(f->flags))goto mi_0041ce6c;
    /* 0041ce6a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0041ce6c:;
    /* 0041ce6c */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0041ce6d */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0041ce70 */
    if(!mi_signed_less(f->flags))goto mi_0041ce76;
    /* 0041ce72 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041ce74 */
    if((f->flags&MI_ZF))goto mi_0041ce1b;
mi_0041ce76:;
    /* 0041ce76 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 0041ce7b */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 0041ce81 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0041ce88 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0041ce89 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0041ce8c */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 0041ce91 */
    if(mi_signed_less(f->flags))goto mi_0041ce9d;
    /* 0041ce93 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_0041ce9d:;
    /* 0041ce9d */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 0041cea3 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 0041cea8 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_0041ceb0:;
    /* 0041ceb0 */
    f->r[2]=f->r[1];
    /* 0041ceb2 */
    f->r[7]=f->r[1];
    /* 0041ceb4 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 0041ceb7 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0041ceba */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0041cebd */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0041cebf */
    f->r[7]=f->r[0];
    /* 0041cec1 */
    a=f->r[7];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[7]=t;
    /* 0041cec4 */
    f->r[5]=f->r[0];
    /* 0041cec6 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0041cec9 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0041cecc */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0041cece */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 0041ced0 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 0041ced2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0041ced4 */
    if((f->flags&MI_ZF))goto mi_0041cedc;
    /* 0041ced6 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_0041cedc:;
    /* 0041cedc */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0041cede */
    if((f->flags&MI_ZF))goto mi_0041cee5;
    /* 0041cee0 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0041cee5:;
    /* 0041cee5 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0041cee9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0041ceea */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0041ceee */
    if(!(f->flags&MI_ZF))goto mi_0041ceb0;
    /* 0041cef0 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 0041cef6 */
    f->r[2]=f->r[1];
    /* 0041cef8 */
    f->r[7]=f->r[1];
    /* 0041cefa */
    f->r[5]=MI_U32((0x00bca2ccu));
    /* 0041cf00 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0041cf03 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0041cf09 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 0041cf0f */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 0041cf15 */
    f->r[1]=f->r[0];
    /* 0041cf17 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0041cf1a */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0041cf20 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cf21 */
    f->r[2]=MI_U32((0x005cd97cu+f->r[6]*8u));
    /* 0041cf28 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 0041cf2e */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 0041cf33 */
    f->r[1]=f->r[0];
    /* 0041cf35 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0041cf38 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041cf3a */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 0041cf3f */
    f->r[0]=MI_U32((0x005cd978u+f->r[6]*8u));
    /* 0041cf46 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cf47 */
    f->r[2]=MI_U32((0x00bca2c8u));
    /* 0041cf4d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041cf4f */
    a=f->r[7];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0041cf52 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cf53 */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0041cf56 */
    a=0x00000054u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cf58 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cf5a */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[7]);
    /* 0041cf60 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[1]);
    /* 0041cf66 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041cf6bu;f->boundary(0x0041cf66u,0x004b4800u,f);
    /* 0041cf6b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0041cf6e */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 0041cf73 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0041cf76 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041cf78 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041cf79 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0041cf7c */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000019u);
    /* 0041cf83 */
    MI_U16((0x00bcd658u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0041cf8a */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(0x000003e8u);
    /* 0041cf93 */
    MI_U8((0x00bcd65au+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 0041cf99 */
    MI_U32((0x00bcd73cu+f->r[1]))=(uint32_t)(0x00000007u);
    /* 0041cfa3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041cfa8u;f->boundary(0x0041cfa3u,0x004ec990u,f);
    /* 0041cfa8 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041cfab */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0041cfac */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0041cfaf */
    if(mi_signed_less(f->flags))goto mi_0041cd5a;
    /* 0041cfb5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0041cfb6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0041cfb7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0041cfb8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0041cfb9:;
    /* 0041cfb9 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041cfbc */
    f->r[4]+=4;return;
}
