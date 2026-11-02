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


static const uint8_t mi_rom_004ec968[]={0xf7,0xc7,0x4e,0x00,0x10,0xc8,0x4e,0x00,0x30,0xc8,0x4e,0x00,0x4c,0xc8,0x4e,0x00};
static const uint8_t mi_rom_004ec978[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x00,0x02};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004ec968u&&width<=16u&&(offset=address-0x004ec968u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004ec968[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004ec978u&&width<=19u&&(offset=address-0x004ec978u)<=19u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004ec978[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


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

void restore_actor_command_C_004ec990(struct restore_actor_command_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ec990 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec991 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec992 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec993 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004ec997 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ec999 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004ec99c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ec99f */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004ec9a1 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ec9a4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 004ec9aa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec9ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ec9b0u;f->boundary(0x004ec9abu,0x004ea9c0u,f);
    /* 004ec9b0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec9b3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ec9b5 */
    if((f->flags&MI_ZF))goto mi_004eca6b;
    /* 004ec9bb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd606u+f->r[6])))&0xffu)<<0);
    /* 004ec9c1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ec9c3 */
    if((f->flags&MI_ZF))goto mi_004eca6b;
    /* 004ec9c9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[6])))&0xffu)<<0);
    /* 004ec9cf */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004ec9d1 */
    if((f->flags&MI_ZF))goto mi_004eca6b;
    /* 004ec9d7 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004ec9d9 */
    if((f->flags&MI_ZF))goto mi_004eca6b;
    /* 004ec9df */
    f->r[1]=MI_U32((0x00784cb0u));
    /* 004ec9e5 */
    a=0x004eae20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec9ea */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784b94u));
    /* 004ec9f1 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004ec9f7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec9f8 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 004ec9fb */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 004ec9ff */
    f->r[2]=(0x00000010u+f->r[4]);
    /* 004eca03 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004eca06 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004eca09 */
    f->r[0]=MI_U32((0x005e4cd4u+f->r[0]));
    /* 004eca0f */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004eca11 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eca12 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eca13 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004eca1a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004eca21 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eca22 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eca23 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eca24 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004eca29u;f->boundary(0x004eca24u,0x004ec0f0u,f);
    /* 004eca29 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004eca2c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eca2e */
    if((f->flags&MI_ZF))goto mi_004eca62;
    /* 004eca30 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004eca33 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eca34 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004eca37 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004eca39 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004eca3c */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00bcd60cu+f->r[1])))&0xffffu)<<0);
    /* 004eca43 */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004eca4a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00bcd60eu+f->r[1])))&0xffffu)<<0);
    /* 004eca51 */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004eca58 */
    MI_U16((0x00bcd656u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004eca5f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eca60 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004eca61 */
    f->r[4]+=4;return;
mi_004eca62:;
    /* 004eca62 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eca63 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004eca68u;f->boundary(0x004eca63u,0x004ec7c0u,f);
    /* 004eca68 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004eca6b:;
    /* 004eca6b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eca6c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eca6d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004eca6e */
    f->r[4]+=4;return;
}

void restore_actor_command_C_004ec0f0(struct restore_actor_command_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ec0f0 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004ec0f4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec0f5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec0f6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec0f7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004ec0fa */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 004ec0fe */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec0ff */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004ec103 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004ec106 */
    f->r[5]=0x00000001u;
    /* 004ec10b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004ec113 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ec116 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004ec11c */
    f->r[2]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 004ec11f */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004ec123 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ec125 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004ec127 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004ec12b */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
mi_004ec12d:;
    /* 004ec12d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ec12f */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec135 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004ec137 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ec193;
mi_004ec139:;
    /* 004ec139 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec13b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ec13d */
    if(mi_signed_less(f->flags))goto mi_004ec17c;
    /* 004ec13f */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec141 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec143 */
    if(mi_signed_less(f->flags))goto mi_004ec17c;
    /* 004ec145 */
    a=f->r[1];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004ec14b */
    if(!mi_signed_less(f->flags))goto mi_004ec17c;
    /* 004ec14d */
    a=f->r[0];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004ec153 */
    if(!mi_signed_less(f->flags))goto mi_004ec17c;
    /* 004ec155 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004ec15a */
    a=MI_U32((0x00af598cu+f->r[0]*4u));b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004ec165 */
    if(!(f->flags&MI_ZF))goto mi_004ec17c;
    /* 004ec167 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004ec16b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec16c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec16d */
    p=MI_U32((0x00000034u+f->r[4]));f->r[4]-=4;MI_U32(f->r[4])=0x004ec171u;f->boundary(0x004ec16du,p,f);
    /* 004ec171 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec174 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec176 */
    if(!(f->flags&MI_ZF))goto mi_004ec34d;
mi_004ec17c:;
    /* 004ec17c */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec182 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec183 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004ec184 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec18a */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec18c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec18d */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004ec18f */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004ec191 */
    if(mi_signed_less(f->flags))goto mi_004ec139;
mi_004ec193:;
    /* 004ec193 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec199 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ec19b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004ec19c */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec1a2 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec1a4 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec1a5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004ec1a7 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004ec1a9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ec20a;
mi_004ec1ab:;
    /* 004ec1ab */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec1ad */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ec1af */
    if(mi_signed_less(f->flags))goto mi_004ec1ee;
    /* 004ec1b1 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec1b3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec1b5 */
    if(mi_signed_less(f->flags))goto mi_004ec1ee;
    /* 004ec1b7 */
    a=f->r[1];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004ec1bd */
    if(!mi_signed_less(f->flags))goto mi_004ec1ee;
    /* 004ec1bf */
    a=f->r[0];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004ec1c5 */
    if(!mi_signed_less(f->flags))goto mi_004ec1ee;
    /* 004ec1c7 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004ec1cc */
    a=MI_U32((0x00af598cu+f->r[0]*4u));b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004ec1d7 */
    if(!(f->flags&MI_ZF))goto mi_004ec1ee;
    /* 004ec1d9 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004ec1dd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec1de */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec1df */
    p=MI_U32((0x00000034u+f->r[4]));f->r[4]-=4;MI_U32(f->r[4])=0x004ec1e3u;f->boundary(0x004ec1dfu,p,f);
    /* 004ec1e3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec1e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec1e8 */
    if(!(f->flags&MI_ZF))goto mi_004ec34d;
mi_004ec1ee:;
    /* 004ec1ee */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec1f4 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec1f5 */
    a=f->r[2];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ec1fb */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec201 */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec203 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec204 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004ec206 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004ec208 */
    if(mi_signed_less(f->flags))goto mi_004ec1ab;
mi_004ec20a:;
    /* 004ec20a */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec210 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ec212 */
    a=f->r[2];b=0x000000e4u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004ec218 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec21e */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec220 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec221 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004ec223 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004ec225 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ec281;
mi_004ec227:;
    /* 004ec227 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec229 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ec22b */
    if(mi_signed_less(f->flags))goto mi_004ec26a;
    /* 004ec22d */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec22f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec231 */
    if(mi_signed_less(f->flags))goto mi_004ec26a;
    /* 004ec233 */
    a=f->r[1];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004ec239 */
    if(!mi_signed_less(f->flags))goto mi_004ec26a;
    /* 004ec23b */
    a=f->r[0];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004ec241 */
    if(!mi_signed_less(f->flags))goto mi_004ec26a;
    /* 004ec243 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004ec248 */
    a=MI_U32((0x00af598cu+f->r[0]*4u));b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004ec253 */
    if(!(f->flags&MI_ZF))goto mi_004ec26a;
    /* 004ec255 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004ec259 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec25a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec25b */
    p=MI_U32((0x00000034u+f->r[4]));f->r[4]-=4;MI_U32(f->r[4])=0x004ec25fu;f->boundary(0x004ec25bu,p,f);
    /* 004ec25f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec262 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec264 */
    if(!(f->flags&MI_ZF))goto mi_004ec34d;
mi_004ec26a:;
    /* 004ec26a */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec270 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec271 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004ec272 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec278 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec27a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec27b */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004ec27d */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004ec27f */
    if(mi_signed_less(f->flags))goto mi_004ec227;
mi_004ec281:;
    /* 004ec281 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec287 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ec289 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004ec28a */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec290 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec292 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec293 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004ec295 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004ec297 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ec2f4;
mi_004ec299:;
    /* 004ec299 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec29b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ec29d */
    if(mi_signed_less(f->flags))goto mi_004ec2d8;
    /* 004ec29f */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec2a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec2a3 */
    if(mi_signed_less(f->flags))goto mi_004ec2d8;
    /* 004ec2a5 */
    a=f->r[1];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004ec2ab */
    if(!mi_signed_less(f->flags))goto mi_004ec2d8;
    /* 004ec2ad */
    a=f->r[0];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004ec2b3 */
    if(!mi_signed_less(f->flags))goto mi_004ec2d8;
    /* 004ec2b5 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004ec2ba */
    a=MI_U32((0x00af598cu+f->r[0]*4u));b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004ec2c5 */
    if(!(f->flags&MI_ZF))goto mi_004ec2d8;
    /* 004ec2c7 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004ec2cb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec2cc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec2cd */
    p=MI_U32((0x00000034u+f->r[4]));f->r[4]-=4;MI_U32(f->r[4])=0x004ec2d1u;f->boundary(0x004ec2cdu,p,f);
    /* 004ec2d1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec2d4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec2d6 */
    if(!(f->flags&MI_ZF))goto mi_004ec34d;
mi_004ec2d8:;
    /* 004ec2d8 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec2de */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec2df */
    a=f->r[2];b=0x000000e4u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004ec2e5 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec2eb */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec2ed */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec2ee */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004ec2f0 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004ec2f2 */
    if(mi_signed_less(f->flags))goto mi_004ec299;
mi_004ec2f4:;
    /* 004ec2f4 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ec2fa */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004ec2fe */
    a=f->r[2];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ec304 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004ec30a */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec30c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec30d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec30f */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004ec311 */
    if((f->flags&MI_ZF))goto mi_004ec34b;
    /* 004ec313 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004ec317 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004ec31b */
    f->r[3]=MI_U32((0x00000000u+f->r[7]));
    /* 004ec31d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec31e */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004ec321 */
    a=f->r[2];b=0x000000e5u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004ec327 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec328 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ec32c */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[3]);
    /* 004ec32e */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 004ec330 */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 004ec334 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec335 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec336 */
    a=f->r[0];b=0x000000e4u;t=mi_alu(f,a,b,32,1);
    /* 004ec33b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004ec33f */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004ec341 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004ec345 */
    if(mi_signed_less(f->flags))goto mi_004ec12d;
mi_004ec34b:;
    /* 004ec34b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004ec34d:;
    /* 004ec34d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ec34e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ec34f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ec350 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ec351 */
    f->r[4]+=4;return;
}

void restore_actor_command_C_004ec7c0(struct restore_actor_command_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ec7c0 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004ec7c4 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004ec7c7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ec7ca */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec7cb */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ec7ce */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ec7d0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ec7d2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec7d3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec7d4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004ec7db */
    f->r[1]=0x00000001u;
    /* 004ec7e0 */
    a=f->r[0];b=0xfffffff8u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ec7e3 */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 004ec7e6 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ec84c;
    /* 004ec7e8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ec7ea */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x004ec978u+f->r[0]),1u))&0xffu)<<0);
    /* 004ec7f0 */
    switch(mi_read((0x004ec968u+f->r[2]*4u),4u)){case 0x004ec7f7u:goto mi_004ec7f7;case 0x004ec810u:goto mi_004ec810;case 0x004ec830u:goto mi_004ec830;case 0x004ec84cu:goto mi_004ec84c;default:abort();}
mi_004ec7f7:;
    /* 004ec7f7 */
    f->r[0]=0x00000032u;
    /* 004ec7fc */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004ec7fe */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ec802 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ec806 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004ec80a */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004ec80e */
    goto mi_004ec866;
mi_004ec810:;
    /* 004ec810 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004ec812 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000004bu);
    /* 004ec81a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000005u);
    /* 004ec822 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000014u);
    /* 004ec82a */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004ec82e */
    goto mi_004ec866;
mi_004ec830:;
    /* 004ec830 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004ec832 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000005au);
    /* 004ec83a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004ec83e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000009u);
    /* 004ec846 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004ec84a */
    goto mi_004ec866;
mi_004ec84c:;
    /* 004ec84c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004ec84e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001du);
    /* 004ec856 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000046u);
    /* 004ec85e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004ec862 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_004ec866:;
    /* 004ec866 */
    f->r[0]=mi_read((0x00d3b1c4u),4u);
    /* 004ec86b */
    f->r[1]=mi_read((0x00d3b374u),4u);
    /* 004ec871 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004ec878 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec879 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004ec87c */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004ec881 */
    if(mi_signed_less(f->flags))goto mi_004ec889;
    /* 004ec883 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[7]);
mi_004ec889:;
    /* 004ec889 */
    f->r[2]=mi_read((0x00d3b384u),4u);
    /* 004ec88f */
    f->r[1]=mi_read((0x00d3b380u),4u);
    /* 004ec895 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004ec89d:;
    /* 004ec89d */
    f->r[0]=f->r[2];
    /* 004ec89f */
    f->r[6]=f->r[2];
    /* 004ec8a1 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004ec8a4 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ec8a7 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec8aa */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ec8ac */
    f->r[6]=f->r[1];
    /* 004ec8ae */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004ec8b1 */
    f->r[3]=f->r[1];
    /* 004ec8b3 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec8b6 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004ec8b9 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004ec8bb */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 004ec8bd */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004ec8bf */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004ec8c1 */
    if((f->flags&MI_ZF))goto mi_004ec8c9;
    /* 004ec8c3 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_004ec8c9:;
    /* 004ec8c9 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004ec8cb */
    if((f->flags&MI_ZF))goto mi_004ec8d3;
    /* 004ec8cd */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004ec8d3:;
    /* 004ec8d3 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004ec8d7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec8d8 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ec8dc */
    if(!(f->flags&MI_ZF))goto mi_004ec89d;
    /* 004ec8de */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 004ec8e4 */
    f->r[0]=f->r[2];
    /* 004ec8e6 */
    f->r[6]=f->r[2];
    /* 004ec8e8 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec8eb */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004ec8f1 */
    f->r[2]=f->r[1];
    /* 004ec8f3 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec8f9 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004ec8ff */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004ec905 */
    f->r[2]=f->r[1];
    /* 004ec907 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ec90c */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec90f */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004ec912 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004ec917 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004ec91d */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004ec923 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ec924 */
    f->r[1]=0x00000064u;
    /* 004ec929 */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec92c */
    a=f->r[1];mi_idiv32(f,a);
    /* 004ec92e */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004ec934 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004ec936 */
    f->r[0]=(0x00000010u+f->r[4]);
mi_004ec93a:;
    /* 004ec93a */
    a=f->r[7];b=mi_read((0x00000000u+f->r[0]),4u);t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ec93c */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004ec93e */
    if(mi_signed_less(f->flags))goto mi_004ec949;
    /* 004ec940 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ec941 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ec944 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004ec947 */
    if(mi_signed_less(f->flags))goto mi_004ec93a;
mi_004ec949:;
    /* 004ec949 */
    f->r[2]=mi_read((0x005e4e70u+f->r[1]*4u),4u);
    /* 004ec950 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 004ec954 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec955 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec956 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ec95bu;f->boundary(0x004ec956u,0x004ec4a0u,f);
    /* 004ec95b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec95e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ec95f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ec960 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ec961 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec964 */
    f->r[4]+=4;return;
}

void restore_actor_command_C_004ec4a0(struct restore_actor_command_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ec4a0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004ec4a4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec4a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec4a6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec4a7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec4a8 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004ec4ac */
    f->r[7]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004ec4af */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004ec4b2 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ec4b4 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004ec4b7 */
    MI_U8((0x00bcd630u+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 004ec4bd */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ec4c0 */
    if((f->flags&MI_ZF))goto mi_004ec6d3;
    /* 004ec4c6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec4c7 */
    if((f->flags&MI_ZF))goto mi_004ec51b;
    /* 004ec4c9 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004ec4cc */
    if((f->flags&MI_ZF))goto mi_004ec4f9;
    /* 004ec4ce */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec4cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ec4d4u;f->boundary(0x004ec4cfu,0x004ec990u,f);
    /* 004ec4d4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec4d7 */
    MI_U32((0x00bcd73cu+f->r[7]))=(uint32_t)(0x00000007u);
    /* 004ec4e1 */
    MI_U16((0x00bcd62eu+f->r[7]))=(uint16_t)(0x000000c8u);
    /* 004ec4ea */
    MI_U32((0x00bcd740u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 004ec4f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ec4f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ec4f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ec4f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ec4f8 */
    f->r[4]+=4;return;
mi_004ec4f9:;
    /* 004ec4f9 */
    MI_U32((0x00bcd73cu+f->r[7]))=(uint32_t)(0x0000000cu);
    /* 004ec503 */
    MI_U16((0x00bcd62eu+f->r[7]))=(uint16_t)(0x00000014u);
    /* 004ec50c */
    MI_U32((0x00bcd740u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 004ec516 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ec517 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ec518 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ec519 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ec51a */
    f->r[4]+=4;return;
mi_004ec51b:;
    /* 004ec51b */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004ec520 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004ec526 */
    MI_U32((0x00bcd73cu+f->r[7]))=(uint32_t)(0x0000000cu);
    /* 004ec530 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004ec537 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec538 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004ec53b */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004ec540 */
    if(mi_signed_less(f->flags))goto mi_004ec54c;
    /* 004ec542 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004ec54c:;
    /* 004ec54c */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 004ec552 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 004ec557 */
    f->r[3]=0x0000001fu;
mi_004ec55c:;
    /* 004ec55c */
    f->r[2]=f->r[1];
    /* 004ec55e */
    f->r[6]=f->r[1];
    /* 004ec560 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004ec563 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec566 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec569 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004ec56b */
    f->r[6]=f->r[0];
    /* 004ec56d */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004ec570 */
    f->r[5]=f->r[0];
    /* 004ec572 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec575 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004ec578 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004ec57a */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004ec57c */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004ec57e */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 004ec580 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004ec582 */
    if((f->flags&MI_ZF))goto mi_004ec58a;
    /* 004ec584 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004ec58a:;
    /* 004ec58a */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004ec58c */
    if((f->flags&MI_ZF))goto mi_004ec593;
    /* 004ec58e */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004ec593:;
    /* 004ec593 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec594 */
    if(!(f->flags&MI_ZF))goto mi_004ec55c;
    /* 004ec596 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 004ec59c */
    f->r[2]=f->r[1];
    /* 004ec59e */
    f->r[6]=f->r[1];
    /* 004ec5a0 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004ec5a3 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 004ec5a9 */
    f->r[1]=f->r[0];
    /* 004ec5ab */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004ec5b1 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 004ec5b6 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 004ec5bc */
    f->r[1]=f->r[0];
    /* 004ec5be */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ec5c1 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec5c7 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 004ec5cc */
    f->r[0]=f->r[2];
    /* 004ec5ce */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec5d1 */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004ec5d4 */
    a=f->r[0];b=0x80000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ec5d9 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 004ec5df */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004ec5e5 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[1]);
    /* 004ec5eb */
    if(!(f->flags&MI_SF))goto mi_004ec5f2;
    /* 004ec5ed */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec5ee */
    a=f->r[0];b=0xfffffff8u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004ec5f1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_004ec5f2:;
    /* 004ec5f2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004ec5f4:;
    /* 004ec5f4 */
    f->r[1]=MI_U32((0x005e4e0cu+f->r[0]*4u));
    /* 004ec5fb */
    f->r[3]=MI_U32((0x00bcd618u+f->r[7]));
    /* 004ec601 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ec603 */
    f->r[1]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 004ec60a */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 004ec60d */
    if((f->flags&MI_ZF))goto mi_004ec617;
    /* 004ec60f */
    a=f->r[1];b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004ec615 */
    if((f->flags&MI_ZF))goto mi_004ec627;
mi_004ec617:;
    /* 004ec617 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec618 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004ec61b */
    if(mi_signed_less(f->flags))goto mi_004ec61f;
    /* 004ec61d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004ec61f:;
    /* 004ec61f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004ec620 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004ec623 */
    if(mi_signed_less(f->flags))goto mi_004ec5f4;
    /* 004ec625 */
    goto mi_004ec653;
mi_004ec627:;
    /* 004ec627 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x005e4e30u+f->r[0]*8u)))&0xffffu)<<0);
    /* 004ec62f */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x005e4e34u+f->r[0]*8u)))&0xffffu)<<0);
    /* 004ec637 */
    a=(f->r[1]&0xffffu);b=MI_U16((0x00bcd60cu+f->r[7]));t=mi_alu(f,a,b,16,0);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ec63e */
    MI_U16((0x00bcd61cu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004ec645 */
    a=(f->r[0]&0xffffu);b=MI_U16((0x00bcd60eu+f->r[7]));t=mi_alu(f,a,b,16,0);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004ec64c */
    MI_U16((0x00bcd61eu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
mi_004ec653:;
    /* 004ec653 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004ec656 */
    if(!(f->flags&MI_ZF))goto mi_004ec6a5;
    /* 004ec658 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004ec65c */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 004ec660 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec661 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec663 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[7]));
    /* 004ec66a */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 004ec66e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec66f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[7]));
    /* 004ec676 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec677 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec679 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec67a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec67b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec67c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ec67d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ec682u;f->boundary(0x004ec67du,0x004eb800u,f);
    /* 004ec682 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ec685 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec687 */
    if(!(f->flags&MI_ZF))goto mi_004ec6b0;
    /* 004ec689 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00bcd60cu+f->r[7])))&0xffffu)<<0);
    /* 004ec690 */
    MI_U16((0x00bcd61cu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004ec697 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00bcd60eu+f->r[7])))&0xffffu)<<0);
    /* 004ec69e */
    MI_U16((0x00bcd61eu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
mi_004ec6a5:;
    /* 004ec6a5 */
    MI_U32((0x00bcd740u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 004ec6ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ec6ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ec6ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ec6ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ec6af */
    f->r[4]+=4;return;
mi_004ec6b0:;
    /* 004ec6b0 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000014u+f->r[4])))&0xffffu)<<0);
    /* 004ec6b5 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000018u+f->r[4])))&0xffffu)<<0);
    /* 004ec6ba */
    MI_U16((0x00bcd61cu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ec6c1 */
    MI_U16((0x00bcd61eu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004ec6c8 */
    MI_U32((0x00bcd740u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 004ec6ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ec6cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ec6d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ec6d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ec6d2 */
    f->r[4]+=4;return;
mi_004ec6d3:;
    /* 004ec6d3 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004ec6d8 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004ec6de */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004ec6e5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ec6e6 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004ec6e9 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004ec6ee */
    if(mi_signed_less(f->flags))goto mi_004ec6fa;
    /* 004ec6f0 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004ec6fa:;
    /* 004ec6fa */
    f->r[2]=MI_U32((0x00d3b384u));
    /* 004ec700 */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004ec706 */
    f->r[3]=0x0000001fu;
mi_004ec70b:;
    /* 004ec70b */
    f->r[0]=f->r[2];
    /* 004ec70d */
    f->r[6]=f->r[2];
    /* 004ec70f */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004ec712 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ec715 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec718 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ec71a */
    f->r[6]=f->r[1];
    /* 004ec71c */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004ec71f */
    f->r[5]=f->r[1];
    /* 004ec721 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec724 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004ec727 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004ec729 */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 004ec72b */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004ec72d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ec72f */
    if((f->flags&MI_ZF))goto mi_004ec737;
    /* 004ec731 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_004ec737:;
    /* 004ec737 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004ec739 */
    if((f->flags&MI_ZF))goto mi_004ec741;
    /* 004ec73b */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004ec741:;
    /* 004ec741 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ec742 */
    if(!(f->flags&MI_ZF))goto mi_004ec70b;
    /* 004ec744 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 004ec74a */
    f->r[0]=f->r[2];
    /* 004ec74c */
    f->r[6]=f->r[2];
    /* 004ec74e */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec751 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004ec757 */
    f->r[2]=f->r[1];
    /* 004ec759 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec75f */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004ec765 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004ec76b */
    f->r[2]=f->r[1];
    /* 004ec76d */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004ec772 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004ec775 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004ec778 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004ec77d */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004ec783 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004ec789 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004ec78a */
    f->r[1]=0x00000064u;
    /* 004ec78f */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004ec792 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004ec794 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004ec79a */
    a=f->r[2];b=0x000000fau;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ec7a0 */
    MI_U16((0x00bcd62eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004ec7a7 */
    MI_U32((0x00bcd73cu+f->r[7]))=(uint32_t)(0x0000000cu);
    /* 004ec7b1 */
    MI_U32((0x00bcd740u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 004ec7bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ec7bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ec7bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ec7be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ec7bf */
    f->r[4]+=4;return;
}

void restore_actor_command_C_004eae20(struct restore_actor_command_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004eae20 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004eae24 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eae25 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eae26 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eae27 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[0]*2u));
    /* 004eae2f */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004eae31 */
    if((f->flags&MI_ZF))goto mi_004eaead;
    /* 004eae33 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
mi_004eae37:;
    /* 004eae37 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004eae39 */
    if((f->flags&MI_ZF))goto mi_004eae99;
    /* 004eae3b */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004eae3e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004eae41 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004eae43 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004eae46 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd606u+f->r[0])))&0xffu)<<0);
    /* 004eae4c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004eae4e */
    if((f->flags&MI_ZF))goto mi_004eae99;
    /* 004eae50 */
    a=MI_U8((0x00bcd630u+f->r[0]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004eae57 */
    if((f->flags&MI_ZF))goto mi_004eae99;
    /* 004eae59 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[0])))&0xffu)<<0);
    /* 004eae5f */
    f->r[1]=f->r[0];
    /* 004eae61 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004eae67 */
    f->r[2]=(0x00000000u+f->r[1]*8u);
    /* 004eae6e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004eae70 */
    f->r[5]=MI_U32((0x007090ccu+f->r[2]*8u));
    /* 004eae77 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004eae79 */
    if((f->flags&MI_ZF))goto mi_004eae99;
    /* 004eae7b */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004eae7e */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004eae81 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004eae83 */
    a=MI_U8((0x00bcd602u+f->r[2]*4u));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004eae8a */
    if((f->flags&MI_ZF))goto mi_004eae99;
    /* 004eae8c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eae8d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004eae92u;f->boundary(0x004eae8du,0x004eada0u,f);
    /* 004eae92 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004eae95 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004eae97 */
    if((f->flags&MI_ZF))goto mi_004eaeb3;
mi_004eae99:;
    /* 004eae99 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004eae9c */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004eae9f */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004eaea1 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[0]*4u));
    /* 004eaea9 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004eaeab */
    if(!(f->flags&MI_ZF))goto mi_004eae37;
mi_004eaead:;
    /* 004eaead */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eaeae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eaeaf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004eaeb1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004eaeb2 */
    f->r[4]+=4;return;
mi_004eaeb3:;
    /* 004eaeb3 */
    f->r[0]=f->r[6];
    /* 004eaeb5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eaeb6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eaeb7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004eaeb8 */
    f->r[4]+=4;return;
}
