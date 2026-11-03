#include "storage_delivery.h"
#include <stdlib.h>
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"
#include <stdlib.h>

#define MI_U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MI_U16(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
enum {MI_CF=1,MI_PF=4,MI_AF=16,MI_ZF=64,MI_SF=128,MI_OF=2048};
static __inline uint32_t gb_common_mask(uint32_t width){return width==32?UINT32_MAX:((1u<<width)-1u);}
static __inline uint32_t gb_common_szp(uint32_t value,uint32_t width)
{
 uint32_t parity=value&255u;parity^=parity>>4;parity&=15;
 return (value==0?MI_ZF:0)|((value>>(width-1u))&1u?MI_SF:0)|
        (((0x6996u>>parity)&1u)?0:MI_PF);
}
static __inline uint32_t gb_common_alu(struct restore_god_building_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
{
 uint32_t mask=gb_common_mask(width),sign=1u<<(width-1u),result,flags,carry;
 uint64_t wide;a&=mask;b&=mask;carry=operation==2?(f->flags&MI_CF):0;
 if(operation==0){wide=(uint64_t)a+b;result=(uint32_t)wide&mask;
  flags=(wide>mask?MI_CF:0)|((~(a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);
 }else if(operation<=2){wide=(uint64_t)b+carry;result=(a-b-carry)&mask;
  flags=((uint64_t)a<wide?MI_CF:0)|(((a^b)&(a^result)&sign)?MI_OF:0)|((a^b^result)&MI_AF);
 }else{result=(operation==3?(a&b):operation==4?(a|b):(a^b))&mask;flags=f->flags&MI_AF;}
 f->flags=(f->flags&~(MI_CF|MI_PF|MI_AF|MI_ZF|MI_SF|MI_OF))|flags|gb_common_szp(result,width);
 return result;
}
static __inline uint32_t gb_common_shift(struct restore_god_building_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
{
 uint32_t mask=gb_common_mask(width),sign=1u<<(width-1u),result,carry,flags;
 count&=31u;a&=mask;if(!count)return a;
 if(arithmetic){carry=count>=width?(a&sign)!=0:((a>>(count-1u))&1u);
  result=count>=width?((a&sign)?mask:0):((a>>count)|((a&sign)?(mask^(mask>>count)):0));
 }else{carry=count>width?0:((a>>(width-count))&1u);result=(uint32_t)((uint64_t)a<<count)&mask;}
 flags=carry|gb_common_szp(result,width);
 if(count==1){if(!arithmetic&&(((result&sign)!=0)^carry))flags|=MI_OF;}
 else flags|=f->flags&MI_OF;
 f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|flags;return result;
}
static __inline uint32_t gb_common_imul(struct restore_god_building_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int gb_common_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t gb_0043fd00_rom_0043ff94[]={0x5d,0xfd,0x43,0x00,0x53,0xfd,0x43,0x00,0x67,0xfd,0x43,0x00,0x71,0xfd,0x43,0x00,0x7b,0xfd,0x43,0x00};
static const uint8_t gb_0043fd00_rom_0043ffa8[]={0x00,0x01,0x04,0x02,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x03};
static __inline uint32_t gb_0043fd00_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0043ff94u&&width<=20u&&(offset=address-0x0043ff94u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0043fd00_rom_0043ff94[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0043ffa8u&&width<=180u&&(offset=address-0x0043ffa8u)<=180u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0043fd00_rom_0043ffa8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

static __inline void gb_0043fd00_idiv32(struct restore_god_building_frame *f,uint32_t divisor_bits)
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

void restore_god_C_0043fd00(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0043fd00 */
    f->r[1]=gb_0043fd00_read((0x00000004u+f->r[4]),4u);
    /* 0043fd04 */
    a=f->r[4];b=0x00000020u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 0043fd07 */
    f->r[0]=f->r[1];
    /* 0043fd09 */
    a=f->r[0];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043fd0c */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043fd0e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fd0f */
    a=f->r[0];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043fd12 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fd13 */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,5);f->r[3]=t;
    /* 0043fd15 */
    a=gb_0043fd00_read((0x007999e8u+f->r[0]),2u);b=0x000000d1u;t=gb_common_alu(f,a,b,16,1);
    /* 0043fd1e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fd1f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fd20 */
    if(!(f->flags&MI_ZF))goto gb_0043fd00_0043fd35;
    /* 0043fd22 */
    f->r[1]=gb_0043fd00_read((0x00eaac28u),4u);
    /* 0043fd28 */
    a=f->r[1];b=0x000049d8u;t=gb_common_imul(f,a,b);f->r[1]=t;
    /* 0043fd2e */
    MI_U16((0x00ea6084u+f->r[1]))=(uint16_t)((f->r[3]&0xffffu));
gb_0043fd00_0043fd35:;
    /* 0043fd35 */
    f->r[2]=gb_0043fd00_read((0x00000038u+f->r[4]),4u);
    /* 0043fd39 */
    f->r[6]=(0xffffffe2u+f->r[2]);
    /* 0043fd3c */
    a=f->r[6];b=0x000000b3u;t=gb_common_alu(f,a,b,32,1);
    /* 0043fd42 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0043fd00_0043fd7b;
    /* 0043fd44 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 0043fd46 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((gb_0043fd00_read((0x0043ffa8u+f->r[6]),1u))&0xffu)<<0);
    /* 0043fd4c */
    switch(gb_0043fd00_read((0x0043ff94u+f->r[1]*4u),4u)){case 0x0043fd53u:goto gb_0043fd00_0043fd53;case 0x0043fd5du:goto gb_0043fd00_0043fd5d;case 0x0043fd67u:goto gb_0043fd00_0043fd67;case 0x0043fd71u:goto gb_0043fd00_0043fd71;case 0x0043fd7bu:goto gb_0043fd00_0043fd7b;default:abort();}
gb_0043fd00_0043fd53:;
    /* 0043fd53 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000002u);
    /* 0043fd5b */
    goto gb_0043fd00_0043fd7f;
gb_0043fd00_0043fd5d:;
    /* 0043fd5d */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000003u);
    /* 0043fd65 */
    goto gb_0043fd00_0043fd7f;
gb_0043fd00_0043fd67:;
    /* 0043fd67 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 0043fd6f */
    goto gb_0043fd00_0043fd7f;
gb_0043fd00_0043fd71:;
    /* 0043fd71 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000005u);
    /* 0043fd79 */
    goto gb_0043fd00_0043fd7f;
gb_0043fd00_0043fd7b:;
    /* 0043fd7b */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[3]);
gb_0043fd00_0043fd7f:;
    /* 0043fd7f */
    f->r[6]=gb_0043fd00_read((0x00799aa8u+f->r[0]),4u);
    /* 0043fd85 */
    f->r[2]=gb_0043fd00_read((0x00bca2d8u),4u);
    /* 0043fd8b */
    f->r[1]=f->r[6];
    /* 0043fd8d */
    f->r[7]=0x000000e4u;
    /* 0043fd92 */
    a=f->r[1];b=f->r[2];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043fd94 */
    f->r[0]=f->r[1];
    /* 0043fd96 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0043fd97 */
    a=f->r[7];gb_0043fd00_idiv32(f,a);
    /* 0043fd99 */
    f->r[0]=0x8fb823efu;
    /* 0043fd9e */
    f->r[7]=f->r[2];
    /* 0043fda0 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0043fda2 */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043fda4 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0043fda8 */
    a=f->r[2];b=0x00000007u;t=gb_common_shift(f,a,b,32,1);f->r[2]=t;
    /* 0043fdab */
    f->r[0]=f->r[2];
    /* 0043fdad */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|gb_common_szp(t,32);f->r[0]=t;
    /* 0043fdb0 */
    a=f->r[2];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043fdb2 */
    f->r[0]=gb_0043fd00_read((0x00000034u+f->r[4]),4u);
    /* 0043fdb6 */
    f->r[5]=f->r[2];
    /* 0043fdb8 */
    a=f->r[0];b=f->r[3];t=gb_common_alu(f,a,b,32,1);
    /* 0043fdba */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043fdbe */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0043fd00_0043fefc;
    /* 0043fdc4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043fdc8 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
gb_0043fd00_0043fdcc:;
    /* 0043fdcc */
    f->r[2]=gb_0043fd00_read((0x00000034u+f->r[4]),4u);
    /* 0043fdd0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0043fdd4 */
    f->r[7]=0x00b285ccu;
    /* 0043fdd9 */
    f->r[5]=0x00ae8e7cu;
    /* 0043fdde */
    f->r[0]=0x00adc36cu;
    /* 0043fde3 */
    f->r[1]=0x00a9010cu;
    /* 0043fde8 */
    a=f->r[7];b=0x00b67d1cu;t=gb_common_alu(f,a,b,32,1);f->r[7]=t;
    /* 0043fdee */
    a=f->r[5];b=0x00b67d1cu;t=gb_common_alu(f,a,b,32,1);f->r[5]=t;
    /* 0043fdf4 */
    a=f->r[0];b=0x00b67d1cu;t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043fdf9 */
    a=f->r[1];b=0x00b67d1cu;t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043fdff */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043fe03 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043fe07 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043fe0b */
    goto gb_0043fd00_0043fe15;
gb_0043fd00_0043fe0d:;
    /* 0043fe0d */
    f->r[0]=gb_0043fd00_read((0x00000024u+f->r[4]),4u);
    /* 0043fe11 */
    f->r[1]=gb_0043fd00_read((0x00000028u+f->r[4]),4u);
gb_0043fd00_0043fe15:;
    /* 0043fe15 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((gb_0043fd00_read((0x00b67d1cu+f->r[7]+f->r[6]*1u),1u))&0xffu)<<0);
    /* 0043fe1c */
    MI_U8((0x00b67d1cu+f->r[0]+f->r[6]*1u))=(uint8_t)((f->r[3]&0xffu));
    /* 0043fe23 */
    a=(f->r[2]&0xffu);b=0x000000e0u;t=gb_common_alu(f,a,b,8,3);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0043fe26 */
    MI_U8((0x00b67d1cu+f->r[6]+f->r[5]*1u))=(uint8_t)((f->r[3]&0xffu));
    /* 0043fe2d */
    MI_U8((0x00b67d1cu+f->r[7]+f->r[6]*1u))=(uint8_t)((f->r[2]&0xffu));
    /* 0043fe34 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((gb_0043fd00_read((0x00b67d1cu+f->r[6]),1u))&0xffu)<<0);
    /* 0043fe3a */
    a=(f->r[2]&0xffu);b=0x000000c0u;t=gb_common_alu(f,a,b,8,3);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0043fe3d */
    MI_U16((0x00b4e6fcu+f->r[6]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 0043fe45 */
    a=(f->r[2]&0xffu);b=0x00000040u;t=gb_common_alu(f,a,b,8,4);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0043fe48 */
    MI_U16((0x00a9cc1cu+f->r[6]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 0043fe50 */
    MI_U8((0x00b67d1cu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0043fe56 */
    f->r[2]=gb_0043fd00_read((0x00af598cu+f->r[6]*4u),4u);
    /* 0043fe5d */
    a=f->r[2];b=0x03052ec4u;t=gb_common_alu(f,a,b,32,3);f->r[2]=t;
    /* 0043fe63 */
    MI_U32((0x00b7482cu+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0043fe6a */
    MI_U32((0x00af598cu+f->r[6]*4u))=(uint32_t)(f->r[2]);
    /* 0043fe71 */
    f->r[0]=f->r[2];
    /* 0043fe73 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((gb_0043fd00_read((0x00000038u+f->r[4]),1u))&0xffu)<<0);
    /* 0043fe77 */
    a=(f->r[0]&0xffu);b=0x00000040u;t=gb_common_alu(f,a,b,8,3);
    /* 0043fe79 */
    MI_U8((0x00b67d1cu+f->r[1]+f->r[6]*1u))=(uint8_t)((f->r[2]&0xffu));
    /* 0043fe80 */
    if((f->flags&MI_ZF))goto gb_0043fd00_0043fe98;
    /* 0043fe82 */
    f->r[0]=gb_0043fd00_read((0x00000014u+f->r[4]),4u);
    /* 0043fe86 */
    f->r[1]=gb_0043fd00_read((0x00000010u+f->r[4]),4u);
    /* 0043fe8a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fe8c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fe8d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fe8e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043fe93u;f->boundary(0x0043fe8eu,0x0047b310u,f);
    /* 0043fe93 */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043fe96 */
    goto gb_0043fd00_0043feb2;
gb_0043fd00_0043fe98:;
    /* 0043fe98 */
    a=gb_0043fd00_read((0x0077567cu),4u);b=f->r[3];t=gb_common_alu(f,a,b,32,1);
    /* 0043fe9e */
    if((f->flags&MI_ZF))goto gb_0043fd00_0043feb2;
    /* 0043fea0 */
    f->r[2]=gb_0043fd00_read((0x00000014u+f->r[4]),4u);
    /* 0043fea4 */
    f->r[0]=gb_0043fd00_read((0x00000010u+f->r[4]),4u);
    /* 0043fea8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043fea9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043feaa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043feafu;f->boundary(0x0043feaau,0x0047a120u,f);
    /* 0043feaf */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_0043fd00_0043feb2:;
    /* 0043feb2 */
    f->r[2]=gb_0043fd00_read((0x00000010u+f->r[4]),4u);
    /* 0043feb6 */
    f->r[0]=gb_0043fd00_read((0x00000018u+f->r[4]),4u);
    /* 0043feba */
    a=f->r[2];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0043febb */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043febc */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043febd */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043fec1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043fec5 */
    if(!(f->flags&MI_ZF))goto gb_0043fd00_0043fe0d;
    /* 0043fecb */
    f->r[7]=gb_0043fd00_read((0x00000034u+f->r[4]),4u);
    /* 0043fecf */
    f->r[5]=gb_0043fd00_read((0x00000014u+f->r[4]),4u);
    /* 0043fed3 */
    f->r[0]=gb_0043fd00_read((0x0000001cu+f->r[4]),4u);
    /* 0043fed7 */
    f->r[1]=0x000000e4u;
    /* 0043fedc */
    a=f->r[1];b=f->r[7];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043fede */
    f->r[7]=gb_0043fd00_read((0x00000020u+f->r[4]),4u);
    /* 0043fee2 */
    a=f->r[5];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0043fee3 */
    a=f->r[6];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043fee5 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043fee6 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043feea */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043feee */
    if(!(f->flags&MI_ZF))goto gb_0043fd00_0043fdcc;
    /* 0043fef4 */
    f->r[5]=gb_0043fd00_read((0x0000002cu+f->r[4]),4u);
    /* 0043fef8 */
    f->r[0]=gb_0043fd00_read((0x00000034u+f->r[4]),4u);
gb_0043fd00_0043fefc:;
    /* 0043fefc */
    f->r[6]=(0x00000000u+f->r[0]+f->r[5]*1u);
    /* 0043feff */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff00 */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043ff02 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff03 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff04 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff05 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff06 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff07 */
    MI_U32((0x0077567cu))=(uint32_t)(f->r[3]);
    /* 0043ff0d */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043ff11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff16u;f->boundary(0x0043ff11u,0x004767d0u,f);
    /* 0043ff16 */
    f->r[0]=gb_0043fd00_read((0x00000050u+f->r[4]),4u);
    /* 0043ff1a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff1b */
    f->r[2]=(0x00000005u+f->r[6]);
    /* 0043ff1e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff20 */
    a=f->r[0];b=0x00000005u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043ff23 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff24 */
    f->r[1]=(0xfffffffbu+f->r[5]);
    /* 0043ff27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff28 */
    f->r[2]=(0xfffffffbu+f->r[7]);
    /* 0043ff2b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff2c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff2d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff32u;f->boundary(0x0043ff2du,0x004767d0u,f);
    /* 0043ff32 */
    f->r[0]=gb_0043fd00_read((0x00000068u+f->r[4]),4u);
    /* 0043ff36 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff37 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff38 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff39 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff3fu;f->boundary(0x0043ff3au,0x00477500u,f);
    /* 0043ff3f */
    f->r[1]=gb_0043fd00_read((0x00000078u+f->r[4]),4u);
    /* 0043ff43 */
    a=f->r[4];b=0x00000040u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043ff46 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff47 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff48 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff49 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff4fu;f->boundary(0x0043ff4au,0x004777c0u,f);
    /* 0043ff4f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff50 */
    f->r[3]=gb_0043fd00_read((0x0000004cu+f->r[4]),4u);
    /* 0043ff54 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff55 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff56 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff57 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff5du;f->boundary(0x0043ff58u,0x00475cc0u,f);
    /* 0043ff5d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff5e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff5f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff60 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff61 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff66u;f->boundary(0x0043ff61u,0x004776f0u,f);
    /* 0043ff66 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff67 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff68 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff69 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff6a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff6fu;f->boundary(0x0043ff6au,0x00477890u,f);
    /* 0043ff6f */
    a=f->r[4];b=0x00000044u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043ff72 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff74 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff75 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff76 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff77 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff78 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff7du;f->boundary(0x0043ff78u,0x00475720u,f);
    /* 0043ff7d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff7f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff80 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff81 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff82 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ff83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ff88u;f->boundary(0x0043ff83u,0x00516f90u,f);
    /* 0043ff88 */
    a=f->r[4];b=0x00000028u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043ff8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043ff8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043ff8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043ff8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043ff8f */
    a=f->r[4];b=0x00000020u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043ff92 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_0045b2a0(struct restore_god_building_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_deliver((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));f->r[4]=sp+4u;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_0045b700(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045b700 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b701 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 0045b705 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b706 */
    f->r[1]=f->r[0];
    /* 0045b708 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b709 */
    a=f->r[1];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045b70c */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b70e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b70f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b710 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[1]*8u)))&0xffffu)<<0);
    /* 0045b718 */
    a=(f->r[1]&0xffffu);b=0x00000048u;t=gb_common_alu(f,a,b,16,1);
    /* 0045b71c */
    if((f->flags&MI_ZF))goto gb_0045b700_0045b729;
    /* 0045b71e */
    a=(f->r[1]&0xffffu);b=0x000000c1u;t=gb_common_alu(f,a,b,16,1);
    /* 0045b723 */
    if(!(f->flags&MI_ZF))goto gb_0045b700_0045b870;
gb_0045b700_0045b729:;
    /* 0045b729 */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045b72d */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 0045b733 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0045b73b */
    goto gb_0045b700_0045b741;
gb_0045b700_0045b73d:;
    /* 0045b73d */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
gb_0045b700_0045b741:;
    /* 0045b741 */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,3);
    /* 0045b743 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0045b700_0045b870;
    /* 0045b749 */
    f->r[2]=f->r[0];
    /* 0045b74b */
    a=f->r[2];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045b74e */
    a=f->r[2];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045b750 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[2]*8u));
    /* 0045b758 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 0045b75a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045b75e */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0045b700_0045b85e;
    /* 0045b764 */
    f->r[6]=f->r[0];
    /* 0045b766 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045b769 */
    a=f->r[6];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045b76b */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 0045b76e */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00799a14u+f->r[6])))&0xffffu)<<0);
    /* 0045b775 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=gb_common_alu(f,a,b,16,3);
    /* 0045b778 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0045b700_0045b85e;
    /* 0045b77e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0045b785 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0045b788 */
    a=f->r[7];b=f->r[3];t=gb_common_alu(f,a,b,32,1);
    /* 0045b78a */
    f->r[0]=f->r[5];
    /* 0045b78c */
    if(!(f->flags&MI_ZF)&&!gb_common_signed_less(f->flags))goto gb_0045b700_0045b7c9;
    /* 0045b78e */
    a=f->r[0];b=0x00001276u;t=gb_common_imul(f,a,b);f->r[0]=t;
    /* 0045b794 */
    a=f->r[0];b=f->r[2];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045b796 */
    a=f->r[3];b=f->r[7];t=gb_common_alu(f,a,b,32,1);f->r[3]=t;
    /* 0045b798 */
    a=f->r[0];b=0x00000002u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045b79b */
    f->r[1]=MI_U32((0x00ea60d0u+f->r[0]));
    /* 0045b7a1 */
    a=f->r[1];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045b7a3 */
    MI_U32((0x00ea60d0u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0045b7a9 */
    f->r[1]=MI_U32((0x00ea6160u+f->r[0]));
    /* 0045b7af */
    a=f->r[1];b=f->r[7];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045b7b1 */
    MI_U32((0x00ea6160u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0045b7b7 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045b7b9 */
    MI_U16((0x00799a14u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0045b7c0 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0045b7c7 */
    goto gb_0045b700_0045b7fb;
gb_0045b700_0045b7c9:;
    /* 0045b7c9 */
    a=f->r[0];b=0x00001276u;t=gb_common_imul(f,a,b);f->r[0]=t;
    /* 0045b7cf */
    a=f->r[0];b=f->r[2];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045b7d1 */
    a=f->r[1];b=f->r[3];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045b7d3 */
    a=f->r[0];b=0x00000002u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045b7d6 */
    MI_U16((0x00799a14u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0045b7dd */
    f->r[7]=MI_U32((0x00ea60d0u+f->r[0]));
    /* 0045b7e3 */
    a=f->r[7];b=f->r[3];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045b7e5 */
    MI_U32((0x00ea60d0u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 0045b7eb */
    f->r[7]=MI_U32((0x00ea6160u+f->r[0]));
    /* 0045b7f1 */
    a=f->r[7];b=f->r[3];t=gb_common_alu(f,a,b,32,1);f->r[7]=t;
    /* 0045b7f3 */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,5);f->r[3]=t;
    /* 0045b7f5 */
    MI_U32((0x00ea6160u+f->r[0]))=(uint32_t)(f->r[7]);
gb_0045b700_0045b7fb:;
    /* 0045b7fb */
    f->r[0]=MI_U32((0x007999e4u+f->r[6]));
    /* 0045b801 */
    a=MI_U16((0x00799a14u+f->r[6]));b=0x00000000u;t=gb_common_alu(f,a,b,16,1);
    /* 0045b809 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045b80d */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0045b700_0045b84d;
    /* 0045b80f */
    a=f->r[2];b=0x0000001fu;t=gb_common_alu(f,a,b,32,1);
    /* 0045b812 */
    if(!gb_common_signed_less(f->flags))goto gb_0045b700_0045b820;
    /* 0045b814 */
    f->r[1]=MI_U32((0x00f566bcu));
    /* 0045b81a */
    f->r[7]=(0xfffffffcu+f->r[1]+f->r[2]*4u);
    /* 0045b81e */
    goto gb_0045b700_0045b829;
gb_0045b700_0045b820:;
    /* 0045b820 */
    f->r[0]=MI_U32((0x00f57204u));
    /* 0045b825 */
    f->r[7]=(0xffffff84u+f->r[0]+f->r[2]*4u);
gb_0045b700_0045b829:;
    /* 0045b829 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b82b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b82c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045b831u;f->boundary(0x0045b82cu,0x0047d350u,f);
    /* 0045b831 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a14u+f->r[6]));
    /* 0045b838 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045b839 */
    a=f->r[7];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045b83b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045b840u;f->boundary(0x0045b83bu,0x0045b490u,f);
    /* 0045b840 */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 0045b846 */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045b849 */
    a=f->r[7];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045b84b */
    goto gb_0045b700_0045b853;
gb_0045b700_0045b84d:;
    /* 0045b84d */
    f->r[7]=MI_U32((0x00f566b4u));
gb_0045b700_0045b853:;
    /* 0045b853 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0045b857 */
    MI_U32((0x00b7482cu+f->r[2]*4u))=(uint32_t)(f->r[7]);
gb_0045b700_0045b85e:;
    /* 0045b85e */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0045b862 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045b863 */
    a=f->r[0];b=0x00000008u;t=gb_common_alu(f,a,b,32,1);
    /* 0045b866 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045b86a */
    if(gb_common_signed_less(f->flags))goto gb_0045b700_0045b73d;
gb_0045b700_0045b870:;
    /* 0045b870 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045b871 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045b872 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045b873 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045b874 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0045b875 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"
#include <stdlib.h>








static const uint8_t gb_0045fe20_rom_00460294[]={0x76,0xfe,0x45,0x00,0x8b,0x02,0x46,0x00};
static const uint8_t gb_0045fe20_rom_0046029c[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
static const uint8_t gb_0045fe20_rom_00460368[]={0xbb,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0xc2,0xff,0x45,0x00,0x67,0xff,0x45,0x00,0x6e,0xff,0x45,0x00,0x49,0xff,0x45,0x00,0x5d,0xff,0x45,0x00,0xd0,0xff,0x45,0x00,0x75,0xff,0x45,0x00,0xb4,0xff,0x45,0x00,0x21,0xff,0x45,0x00,0x98,0xff,0x45,0x00,0x7c,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0x3f,0xff,0x45,0x00,0x91,0xff,0x45,0x00,0x83,0xff,0x45,0x00,0x8a,0xff,0x45,0x00,0x35,0xff,0x45,0x00,0x53,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0xe5,0xff,0x45,0x00,0xad,0xff,0x45,0x00,0x9f,0xff,0x45,0x00,0xa6,0xff,0x45,0x00,0xc9,0xff,0x45,0x00,0x2b,0xff,0x45,0x00,0xd7,0xff,0x45,0x00,0xde,0xff,0x45,0x00};
static const uint8_t gb_0045fe20_rom_004603ec[]={0x14,0x00,0x46,0x00,0x1b,0x00,0x46,0x00,0x22,0x00,0x46,0x00,0x29,0x00,0x46,0x00};
static const uint8_t gb_0045fe20_rom_004603fc[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t gb_0045fe20_rom_0046049c[]={0x57,0x00,0x46,0x00,0x5e,0x00,0x46,0x00,0x65,0x00,0x46,0x00,0x6c,0x00,0x46,0x00};
static const uint8_t gb_0045fe20_rom_004604ac[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t gb_0045fe20_rom_0046054c[]={0x93,0x00,0x46,0x00,0x9a,0x00,0x46,0x00,0xa1,0x00,0x46,0x00,0xa8,0x00,0x46,0x00};
static const uint8_t gb_0045fe20_rom_0046055c[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t gb_0045fe20_rom_004605fc[]={0x78,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0x7f,0x01,0x46,0x00,0x24,0x01,0x46,0x00,0x2b,0x01,0x46,0x00,0x06,0x01,0x46,0x00,0x1a,0x01,0x46,0x00,0x8d,0x01,0x46,0x00,0x32,0x01,0x46,0x00,0x71,0x01,0x46,0x00,0xde,0x00,0x46,0x00,0x55,0x01,0x46,0x00,0x39,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0xfc,0x00,0x46,0x00,0x4e,0x01,0x46,0x00,0x40,0x01,0x46,0x00,0x47,0x01,0x46,0x00,0xf2,0x00,0x46,0x00,0x10,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0xa2,0x01,0x46,0x00,0x6a,0x01,0x46,0x00,0x5c,0x01,0x46,0x00,0x63,0x01,0x46,0x00,0x86,0x01,0x46,0x00,0xe8,0x00,0x46,0x00,0x94,0x01,0x46,0x00,0x9b,0x01,0x46,0x00};
static const uint8_t gb_0045fe20_rom_00460680[]={0xd1,0x01,0x46,0x00,0xd8,0x01,0x46,0x00,0xdf,0x01,0x46,0x00,0xe6,0x01,0x46,0x00};
static const uint8_t gb_0045fe20_rom_00460690[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t gb_0045fe20_rom_00460730[]={0x14,0x02,0x46,0x00,0x1b,0x02,0x46,0x00,0x22,0x02,0x46,0x00,0x29,0x02,0x46,0x00};
static const uint8_t gb_0045fe20_rom_00460740[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t gb_0045fe20_rom_004607e0[]={0x50,0x02,0x46,0x00,0x57,0x02,0x46,0x00,0x5e,0x02,0x46,0x00,0x65,0x02,0x46,0x00};
static const uint8_t gb_0045fe20_rom_004607f0[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static __inline uint32_t gb_0045fe20_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00460294u&&width<=8u&&(offset=address-0x00460294u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_00460294[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0046029cu&&width<=202u&&(offset=address-0x0046029cu)<=202u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_0046029c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00460368u&&width<=132u&&(offset=address-0x00460368u)<=132u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_00460368[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004603ecu&&width<=16u&&(offset=address-0x004603ecu)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_004603ec[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004603fcu&&width<=158u&&(offset=address-0x004603fcu)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_004603fc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0046049cu&&width<=16u&&(offset=address-0x0046049cu)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_0046049c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004604acu&&width<=158u&&(offset=address-0x004604acu)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_004604ac[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0046054cu&&width<=16u&&(offset=address-0x0046054cu)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_0046054c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0046055cu&&width<=158u&&(offset=address-0x0046055cu)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_0046055c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004605fcu&&width<=132u&&(offset=address-0x004605fcu)<=132u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_004605fc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00460680u&&width<=16u&&(offset=address-0x00460680u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_00460680[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00460690u&&width<=158u&&(offset=address-0x00460690u)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_00460690[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00460730u&&width<=16u&&(offset=address-0x00460730u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_00460730[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00460740u&&width<=158u&&(offset=address-0x00460740u)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_00460740[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004607e0u&&width<=16u&&(offset=address-0x004607e0u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_004607e0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004607f0u&&width<=158u&&(offset=address-0x004607f0u)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_0045fe20_rom_004607f0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_god_C_0045fe20(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045fe20 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045fe21 */
    f->r[1]=gb_0045fe20_read((0x00000008u+f->r[4]),4u);
    /* 0045fe25 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045fe26 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045fe27 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045fe28 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,3);
    /* 0045fe2a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045fe2b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0045fe33 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0045fe20_0046028b;
    /* 0045fe39 */
    f->r[3]=f->r[1];
    /* 0045fe3b */
    a=f->r[3];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[3]=t;
    /* 0045fe3e */
    a=f->r[3];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[3]=t;
    /* 0045fe40 */
    a=f->r[3];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[3]=t;
    /* 0045fe43 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((gb_0045fe20_read((0x007999e8u+f->r[3]),2u))&0xffffu)<<0);
    /* 0045fe4a */
    a=(f->r[6]&0xffffu);b=0x0000006eu;t=gb_common_alu(f,a,b,16,1);
    /* 0045fe4e */
    if(gb_common_signed_less(f->flags))goto gb_0045fe20_0045fe56;
    /* 0045fe50 */
    a=(f->r[6]&0xffffu);b=0x00000072u;t=gb_common_alu(f,a,b,16,1);
    /* 0045fe54 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0045fe20_0045fe76;
gb_0045fe20_0045fe56:;
    /* 0045fe56 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 0045fe59 */
    a=f->r[0];b=0xffffffe0u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045fe5c */
    a=f->r[0];b=0x000000c9u;t=gb_common_alu(f,a,b,32,1);
    /* 0045fe61 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_0046028b;
    /* 0045fe67 */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
    /* 0045fe69 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((gb_0045fe20_read((0x0046029cu+f->r[0]),1u))&0xffu)<<0);
    /* 0045fe6f */
    switch(gb_0045fe20_read((0x00460294u+f->r[2]*4u),4u)){case 0x0045fe76u:goto gb_0045fe20_0045fe76;case 0x0046028bu:goto gb_0045fe20_0046028b;default:abort();}
gb_0045fe20_0045fe76:;
    /* 0045fe76 */
    f->r[0]=gb_0045fe20_read((0x00000020u+f->r[4]),4u);
    /* 0045fe7a */
    a=f->r[0];b=0x00000015u;t=gb_common_alu(f,a,b,32,1);
    /* 0045fe7d */
    if(!(f->flags&MI_ZF))goto gb_0045fe20_0045ff0e;
    /* 0045fe83 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045fe84 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045fe89u;f->boundary(0x0045fe84u,0x00459ef0u,f);
    /* 0045fe89 */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045fe8c */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 0045fe8e */
    if((f->flags&MI_ZF))goto gb_0045fe20_0046028b;
    /* 0045fe94 */
    f->r[0]=gb_0045fe20_read((0x00eaac28u),4u);
    /* 0045fe99 */
    f->r[6]=gb_0045fe20_read((0x0000001cu+f->r[4]),4u);
    /* 0045fe9d */
    a=f->r[0];b=0x000049d8u;t=gb_common_imul(f,a,b);f->r[0]=t;
    /* 0045fea3 */
    f->r[1]=gb_0045fe20_read((0x00e38ebcu),4u);
    /* 0045fea9 */
    f->r[3]=gb_0045fe20_read((0x00ea187cu+f->r[0]),4u);
    /* 0045feaf */
    a=f->r[3];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[3]=t;
    /* 0045feb1 */
    MI_U32((0x00ea187cu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0045feb7 */
    f->r[7]=gb_0045fe20_read((0x00ea5b50u+f->r[0]),4u);
    /* 0045febd */
    a=f->r[7];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045febf */
    MI_U32((0x00ea5b50u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 0045fec5 */
    f->r[2]=gb_0045fe20_read((0x00ea6020u+f->r[0]),4u);
    /* 0045fecb */
    a=f->r[2];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045fecd */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,3);
    /* 0045fecf */
    MI_U32((0x00ea6020u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0045fed5 */
    if(!(f->flags&MI_ZF))goto gb_0045fe20_0045ff06;
    /* 0045fed7 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)gb_0045fe20_read((0x00e38ea8u),1u);
    /* 0045fede */
    a=gb_0045fe20_read((0x00579520u),4u);b=f->r[1];t=gb_common_alu(f,a,b,32,1);
    /* 0045fee4 */
    if(!(f->flags&MI_ZF))goto gb_0045fe20_0045ff06;
    /* 0045fee6 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((gb_0045fe20_read((0x007027bcu),1u))&0xffu)<<0);
    /* 0045feec */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=gb_common_alu(f,a,b,8,3);
    /* 0045feee */
    if(!(f->flags&MI_ZF))goto gb_0045fe20_0045ff06;
    /* 0045fef0 */
    a=gb_0045fe20_read((0x00ea5b50u+f->r[0]),4u);b=0x000001f4u;t=gb_common_alu(f,a,b,32,1);
    /* 0045fefa */
    if(gb_common_signed_less(f->flags))goto gb_0045fe20_0045ff06;
    /* 0045fefc */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045fefe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045ff03u;f->boundary(0x0045fefeu,0x00417ae0u,f);
    /* 0045ff03 */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_0045fe20_0045ff06:;
    /* 0045ff06 */
    f->r[0]=f->r[6];
    /* 0045ff08 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045ff09 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045ff0a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045ff0b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045ff0c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0045ff0d */
    f->r[4]+=4;return;
gb_0045fe20_0045ff0e:;
    /* 0045ff0e */
    f->r[1]=(0xfffffffeu+f->r[0]);
    /* 0045ff11 */
    a=f->r[1];b=0x00000020u;t=gb_common_alu(f,a,b,32,1);
    /* 0045ff14 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_0045ffe5;
    /* 0045ff1a */
    switch(gb_0045fe20_read((0x00460368u+f->r[1]*4u),4u)){case 0x0045ff21u:goto gb_0045fe20_0045ff21;case 0x0045ff2bu:goto gb_0045fe20_0045ff2b;case 0x0045ff35u:goto gb_0045fe20_0045ff35;case 0x0045ff3fu:goto gb_0045fe20_0045ff3f;case 0x0045ff49u:goto gb_0045fe20_0045ff49;case 0x0045ff53u:goto gb_0045fe20_0045ff53;case 0x0045ff5du:goto gb_0045fe20_0045ff5d;case 0x0045ff67u:goto gb_0045fe20_0045ff67;case 0x0045ff6eu:goto gb_0045fe20_0045ff6e;case 0x0045ff75u:goto gb_0045fe20_0045ff75;case 0x0045ff7cu:goto gb_0045fe20_0045ff7c;case 0x0045ff83u:goto gb_0045fe20_0045ff83;case 0x0045ff8au:goto gb_0045fe20_0045ff8a;case 0x0045ff91u:goto gb_0045fe20_0045ff91;case 0x0045ff98u:goto gb_0045fe20_0045ff98;case 0x0045ff9fu:goto gb_0045fe20_0045ff9f;case 0x0045ffa6u:goto gb_0045fe20_0045ffa6;case 0x0045ffadu:goto gb_0045fe20_0045ffad;case 0x0045ffb4u:goto gb_0045fe20_0045ffb4;case 0x0045ffbbu:goto gb_0045fe20_0045ffbb;case 0x0045ffc2u:goto gb_0045fe20_0045ffc2;case 0x0045ffc9u:goto gb_0045fe20_0045ffc9;case 0x0045ffd0u:goto gb_0045fe20_0045ffd0;case 0x0045ffd7u:goto gb_0045fe20_0045ffd7;case 0x0045ffdeu:goto gb_0045fe20_0045ffde;case 0x0045ffe5u:goto gb_0045fe20_0045ffe5;default:abort();}
gb_0045fe20_0045ff21:;
    /* 0045ff21 */
    f->r[0]=0x00000001u;
    /* 0045ff26 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff2b:;
    /* 0045ff2b */
    f->r[0]=0x00000014u;
    /* 0045ff30 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff35:;
    /* 0045ff35 */
    f->r[0]=0x00000003u;
    /* 0045ff3a */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff3f:;
    /* 0045ff3f */
    f->r[0]=0x00000004u;
    /* 0045ff44 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff49:;
    /* 0045ff49 */
    f->r[0]=0x00000005u;
    /* 0045ff4e */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff53:;
    /* 0045ff53 */
    f->r[0]=0x00000006u;
    /* 0045ff58 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff5d:;
    /* 0045ff5d */
    f->r[0]=0x00000007u;
    /* 0045ff62 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff67:;
    /* 0045ff67 */
    f->r[0]=0x0000000du;
    /* 0045ff6c */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff6e:;
    /* 0045ff6e */
    f->r[0]=0x00000008u;
    /* 0045ff73 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff75:;
    /* 0045ff75 */
    f->r[0]=0x00000009u;
    /* 0045ff7a */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff7c:;
    /* 0045ff7c */
    f->r[0]=0x0000000au;
    /* 0045ff81 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff83:;
    /* 0045ff83 */
    f->r[0]=0x0000000bu;
    /* 0045ff88 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff8a:;
    /* 0045ff8a */
    f->r[0]=0x0000000cu;
    /* 0045ff8f */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff91:;
    /* 0045ff91 */
    f->r[0]=0x0000000fu;
    /* 0045ff96 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff98:;
    /* 0045ff98 */
    f->r[0]=0x00000010u;
    /* 0045ff9d */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ff9f:;
    /* 0045ff9f */
    f->r[0]=0x00000011u;
    /* 0045ffa4 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffa6:;
    /* 0045ffa6 */
    f->r[0]=0x00000012u;
    /* 0045ffab */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffad:;
    /* 0045ffad */
    f->r[0]=0x0000000eu;
    /* 0045ffb2 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffb4:;
    /* 0045ffb4 */
    f->r[0]=0x00000013u;
    /* 0045ffb9 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffbb:;
    /* 0045ffbb */
    f->r[0]=0x00000015u;
    /* 0045ffc0 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffc2:;
    /* 0045ffc2 */
    f->r[0]=0x00000016u;
    /* 0045ffc7 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffc9:;
    /* 0045ffc9 */
    f->r[0]=0x00000017u;
    /* 0045ffce */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffd0:;
    /* 0045ffd0 */
    f->r[0]=0x00000018u;
    /* 0045ffd5 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffd7:;
    /* 0045ffd7 */
    f->r[0]=0x00000019u;
    /* 0045ffdc */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffde:;
    /* 0045ffde */
    f->r[0]=0x0000001au;
    /* 0045ffe3 */
    goto gb_0045fe20_0045ffe8;
gb_0045fe20_0045ffe5:;
    /* 0045ffe5 */
    a=f->r[0];b=0xffffffffu;t=gb_common_alu(f,a,b,32,4);f->r[0]=t;
gb_0045fe20_0045ffe8:;
    /* 0045ffe8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)gb_0045fe20_read((0x007999eau+f->r[3]),2u);
    /* 0045ffef */
    a=f->r[2];b=f->r[0];t=gb_common_alu(f,a,b,32,1);
    /* 0045fff1 */
    if(!(f->flags&MI_ZF))goto gb_0045fe20_004600ce;
    /* 0045fff7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 0045fffa */
    f->r[2]=(0xffffffe0u+f->r[0]);
    /* 0045fffd */
    a=f->r[2];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 00460003 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_00460029;
    /* 00460005 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 00460007 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((gb_0045fe20_read((0x004603fcu+f->r[2]),1u))&0xffu)<<0);
    /* 0046000d */
    switch(gb_0045fe20_read((0x004603ecu+f->r[0]*4u),4u)){case 0x00460014u:goto gb_0045fe20_00460014;case 0x0046001bu:goto gb_0045fe20_0046001b;case 0x00460022u:goto gb_0045fe20_00460022;case 0x00460029u:goto gb_0045fe20_00460029;default:abort();}
gb_0045fe20_00460014:;
    /* 00460014 */
    f->r[0]=0x0000012cu;
    /* 00460019 */
    goto gb_0045fe20_0046002e;
gb_0045fe20_0046001b:;
    /* 0046001b */
    f->r[0]=0x000001f4u;
    /* 00460020 */
    goto gb_0045fe20_0046002e;
gb_0045fe20_00460022:;
    /* 00460022 */
    f->r[0]=0x000186a0u;
    /* 00460027 */
    goto gb_0045fe20_0046002e;
gb_0045fe20_00460029:;
    /* 00460029 */
    f->r[0]=0x000000c8u;
gb_0045fe20_0046002e:;
    /* 0046002e */
    f->r[5]=(f->r[5]&0xffff0000u)|(((gb_0045fe20_read((0x00799a14u+f->r[3]),2u))&0xffffu)<<0);
    /* 00460035 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 00460038 */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,1);
    /* 0046003a */
    if(!gb_common_signed_less(f->flags))goto gb_0045fe20_0046028b;
    /* 00460040 */
    a=f->r[2];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 00460046 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_0046006c;
    /* 00460048 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046004a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((gb_0045fe20_read((0x004604acu+f->r[2]),1u))&0xffu)<<0);
    /* 00460050 */
    switch(gb_0045fe20_read((0x0046049cu+f->r[0]*4u),4u)){case 0x00460057u:goto gb_0045fe20_00460057;case 0x0046005eu:goto gb_0045fe20_0046005e;case 0x00460065u:goto gb_0045fe20_00460065;case 0x0046006cu:goto gb_0045fe20_0046006c;default:abort();}
gb_0045fe20_00460057:;
    /* 00460057 */
    f->r[6]=0x0000012cu;
    /* 0046005c */
    goto gb_0045fe20_00460071;
gb_0045fe20_0046005e:;
    /* 0046005e */
    f->r[6]=0x000001f4u;
    /* 00460063 */
    goto gb_0045fe20_00460071;
gb_0045fe20_00460065:;
    /* 00460065 */
    f->r[6]=0x000186a0u;
    /* 0046006a */
    goto gb_0045fe20_00460071;
gb_0045fe20_0046006c:;
    /* 0046006c */
    f->r[6]=0x000000c8u;
gb_0045fe20_00460071:;
    /* 00460071 */
    f->r[7]=gb_0045fe20_read((0x0000001cu+f->r[4]),4u);
    /* 00460075 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 00460078 */
    a=f->r[0];b=f->r[6];t=gb_common_alu(f,a,b,32,1);
    /* 0046007a */
    if(gb_common_signed_less(f->flags))goto gb_0045fe20_004600b5;
    /* 0046007c */
    a=f->r[2];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 00460082 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_004600a8;
    /* 00460084 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 00460086 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((gb_0045fe20_read((0x0046055cu+f->r[2]),1u))&0xffu)<<0);
    /* 0046008c */
    switch(gb_0045fe20_read((0x0046054cu+f->r[0]*4u),4u)){case 0x00460093u:goto gb_0045fe20_00460093;case 0x0046009au:goto gb_0045fe20_0046009a;case 0x004600a1u:goto gb_0045fe20_004600a1;case 0x004600a8u:goto gb_0045fe20_004600a8;default:abort();}
gb_0045fe20_00460093:;
    /* 00460093 */
    f->r[2]=0x0000012cu;
    /* 00460098 */
    goto gb_0045fe20_004600ad;
gb_0045fe20_0046009a:;
    /* 0046009a */
    f->r[2]=0x000001f4u;
    /* 0046009f */
    goto gb_0045fe20_004600ad;
gb_0045fe20_004600a1:;
    /* 004600a1 */
    f->r[2]=0x000186a0u;
    /* 004600a6 */
    goto gb_0045fe20_004600ad;
gb_0045fe20_004600a8:;
    /* 004600a8 */
    f->r[2]=0x000000c8u;
gb_0045fe20_004600ad:;
    /* 004600ad */
    a=f->r[1];b=f->r[2];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 004600af */
    a=f->r[1];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004600b1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
gb_0045fe20_004600b5:;
    /* 004600b5 */
    f->r[1]=gb_0045fe20_read((0x00000010u+f->r[4]),4u);
    /* 004600b9 */
    f->r[0]=f->r[7];
    /* 004600bb */
    a=f->r[5];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[5]=t;
    /* 004600bd */
    a=f->r[5];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 004600bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004600c0 */
    MI_U16((0x00799a14u+f->r[3]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004600c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004600c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004600c9 */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004600cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004600cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004600cd */
    f->r[4]+=4;return;
gb_0045fe20_004600ce:;
    /* 004600ce */
    a=f->r[1];b=0x00000020u;t=gb_common_alu(f,a,b,32,1);
    /* 004600d1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_004601a2;
    /* 004600d7 */
    switch(gb_0045fe20_read((0x004605fcu+f->r[1]*4u),4u)){case 0x004600deu:goto gb_0045fe20_004600de;case 0x004600e8u:goto gb_0045fe20_004600e8;case 0x004600f2u:goto gb_0045fe20_004600f2;case 0x004600fcu:goto gb_0045fe20_004600fc;case 0x00460106u:goto gb_0045fe20_00460106;case 0x00460110u:goto gb_0045fe20_00460110;case 0x0046011au:goto gb_0045fe20_0046011a;case 0x00460124u:goto gb_0045fe20_00460124;case 0x0046012bu:goto gb_0045fe20_0046012b;case 0x00460132u:goto gb_0045fe20_00460132;case 0x00460139u:goto gb_0045fe20_00460139;case 0x00460140u:goto gb_0045fe20_00460140;case 0x00460147u:goto gb_0045fe20_00460147;case 0x0046014eu:goto gb_0045fe20_0046014e;case 0x00460155u:goto gb_0045fe20_00460155;case 0x0046015cu:goto gb_0045fe20_0046015c;case 0x00460163u:goto gb_0045fe20_00460163;case 0x0046016au:goto gb_0045fe20_0046016a;case 0x00460171u:goto gb_0045fe20_00460171;case 0x00460178u:goto gb_0045fe20_00460178;case 0x0046017fu:goto gb_0045fe20_0046017f;case 0x00460186u:goto gb_0045fe20_00460186;case 0x0046018du:goto gb_0045fe20_0046018d;case 0x00460194u:goto gb_0045fe20_00460194;case 0x0046019bu:goto gb_0045fe20_0046019b;case 0x004601a2u:goto gb_0045fe20_004601a2;default:abort();}
gb_0045fe20_004600de:;
    /* 004600de */
    f->r[0]=0x00000001u;
    /* 004600e3 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_004600e8:;
    /* 004600e8 */
    f->r[0]=0x00000014u;
    /* 004600ed */
    goto gb_0045fe20_004601a5;
gb_0045fe20_004600f2:;
    /* 004600f2 */
    f->r[0]=0x00000003u;
    /* 004600f7 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_004600fc:;
    /* 004600fc */
    f->r[0]=0x00000004u;
    /* 00460101 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460106:;
    /* 00460106 */
    f->r[0]=0x00000005u;
    /* 0046010b */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460110:;
    /* 00460110 */
    f->r[0]=0x00000006u;
    /* 00460115 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046011a:;
    /* 0046011a */
    f->r[0]=0x00000007u;
    /* 0046011f */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460124:;
    /* 00460124 */
    f->r[0]=0x0000000du;
    /* 00460129 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046012b:;
    /* 0046012b */
    f->r[0]=0x00000008u;
    /* 00460130 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460132:;
    /* 00460132 */
    f->r[0]=0x00000009u;
    /* 00460137 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460139:;
    /* 00460139 */
    f->r[0]=0x0000000au;
    /* 0046013e */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460140:;
    /* 00460140 */
    f->r[0]=0x0000000bu;
    /* 00460145 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460147:;
    /* 00460147 */
    f->r[0]=0x0000000cu;
    /* 0046014c */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046014e:;
    /* 0046014e */
    f->r[0]=0x0000000fu;
    /* 00460153 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460155:;
    /* 00460155 */
    f->r[0]=0x00000010u;
    /* 0046015a */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046015c:;
    /* 0046015c */
    f->r[0]=0x00000011u;
    /* 00460161 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460163:;
    /* 00460163 */
    f->r[0]=0x00000012u;
    /* 00460168 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046016a:;
    /* 0046016a */
    f->r[0]=0x0000000eu;
    /* 0046016f */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460171:;
    /* 00460171 */
    f->r[0]=0x00000013u;
    /* 00460176 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460178:;
    /* 00460178 */
    f->r[0]=0x00000015u;
    /* 0046017d */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046017f:;
    /* 0046017f */
    f->r[0]=0x00000016u;
    /* 00460184 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460186:;
    /* 00460186 */
    f->r[0]=0x00000017u;
    /* 0046018b */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046018d:;
    /* 0046018d */
    f->r[0]=0x00000018u;
    /* 00460192 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_00460194:;
    /* 00460194 */
    f->r[0]=0x00000019u;
    /* 00460199 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_0046019b:;
    /* 0046019b */
    f->r[0]=0x0000001au;
    /* 004601a0 */
    goto gb_0045fe20_004601a5;
gb_0045fe20_004601a2:;
    /* 004601a2 */
    a=f->r[0];b=0xffffffffu;t=gb_common_alu(f,a,b,32,4);f->r[0]=t;
gb_0045fe20_004601a5:;
    /* 004601a5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)gb_0045fe20_read((0x00799aa6u+f->r[3]),2u);
    /* 004601ac */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,1);
    /* 004601ae */
    if(!(f->flags&MI_ZF))goto gb_0045fe20_0046028b;
    /* 004601b4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 004601b7 */
    f->r[2]=(0xffffffe0u+f->r[0]);
    /* 004601ba */
    a=f->r[2];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 004601c0 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_004601e6;
    /* 004601c2 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004601c4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((gb_0045fe20_read((0x00460690u+f->r[2]),1u))&0xffu)<<0);
    /* 004601ca */
    switch(gb_0045fe20_read((0x00460680u+f->r[0]*4u),4u)){case 0x004601d1u:goto gb_0045fe20_004601d1;case 0x004601d8u:goto gb_0045fe20_004601d8;case 0x004601dfu:goto gb_0045fe20_004601df;case 0x004601e6u:goto gb_0045fe20_004601e6;default:abort();}
gb_0045fe20_004601d1:;
    /* 004601d1 */
    f->r[0]=0x0000012cu;
    /* 004601d6 */
    goto gb_0045fe20_004601eb;
gb_0045fe20_004601d8:;
    /* 004601d8 */
    f->r[0]=0x000001f4u;
    /* 004601dd */
    goto gb_0045fe20_004601eb;
gb_0045fe20_004601df:;
    /* 004601df */
    f->r[0]=0x000186a0u;
    /* 004601e4 */
    goto gb_0045fe20_004601eb;
gb_0045fe20_004601e6:;
    /* 004601e6 */
    f->r[0]=0x000000c8u;
gb_0045fe20_004601eb:;
    /* 004601eb */
    f->r[5]=(f->r[5]&0xffff0000u)|(((gb_0045fe20_read((0x00799aa4u+f->r[3]),2u))&0xffffu)<<0);
    /* 004601f2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 004601f5 */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,1);
    /* 004601f7 */
    if(!gb_common_signed_less(f->flags))goto gb_0045fe20_0046028b;
    /* 004601fd */
    a=f->r[2];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 00460203 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_00460229;
    /* 00460205 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 00460207 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((gb_0045fe20_read((0x00460740u+f->r[2]),1u))&0xffu)<<0);
    /* 0046020d */
    switch(gb_0045fe20_read((0x00460730u+f->r[0]*4u),4u)){case 0x00460214u:goto gb_0045fe20_00460214;case 0x0046021bu:goto gb_0045fe20_0046021b;case 0x00460222u:goto gb_0045fe20_00460222;case 0x00460229u:goto gb_0045fe20_00460229;default:abort();}
gb_0045fe20_00460214:;
    /* 00460214 */
    f->r[6]=0x0000012cu;
    /* 00460219 */
    goto gb_0045fe20_0046022e;
gb_0045fe20_0046021b:;
    /* 0046021b */
    f->r[6]=0x000001f4u;
    /* 00460220 */
    goto gb_0045fe20_0046022e;
gb_0045fe20_00460222:;
    /* 00460222 */
    f->r[6]=0x000186a0u;
    /* 00460227 */
    goto gb_0045fe20_0046022e;
gb_0045fe20_00460229:;
    /* 00460229 */
    f->r[6]=0x000000c8u;
gb_0045fe20_0046022e:;
    /* 0046022e */
    f->r[7]=gb_0045fe20_read((0x0000001cu+f->r[4]),4u);
    /* 00460232 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[7]*1u);
    /* 00460235 */
    a=f->r[0];b=f->r[6];t=gb_common_alu(f,a,b,32,1);
    /* 00460237 */
    if(gb_common_signed_less(f->flags))goto gb_0045fe20_00460272;
    /* 00460239 */
    a=f->r[2];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 0046023f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_0045fe20_00460265;
    /* 00460241 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 00460243 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((gb_0045fe20_read((0x004607f0u+f->r[2]),1u))&0xffu)<<0);
    /* 00460249 */
    switch(gb_0045fe20_read((0x004607e0u+f->r[0]*4u),4u)){case 0x00460250u:goto gb_0045fe20_00460250;case 0x00460257u:goto gb_0045fe20_00460257;case 0x0046025eu:goto gb_0045fe20_0046025e;case 0x00460265u:goto gb_0045fe20_00460265;default:abort();}
gb_0045fe20_00460250:;
    /* 00460250 */
    f->r[2]=0x0000012cu;
    /* 00460255 */
    goto gb_0045fe20_0046026a;
gb_0045fe20_00460257:;
    /* 00460257 */
    f->r[2]=0x000001f4u;
    /* 0046025c */
    goto gb_0045fe20_0046026a;
gb_0045fe20_0046025e:;
    /* 0046025e */
    f->r[2]=0x000186a0u;
    /* 00460263 */
    goto gb_0045fe20_0046026a;
gb_0045fe20_00460265:;
    /* 00460265 */
    f->r[2]=0x000000c8u;
gb_0045fe20_0046026a:;
    /* 0046026a */
    a=f->r[1];b=f->r[2];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046026c */
    a=f->r[1];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046026e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
gb_0045fe20_00460272:;
    /* 00460272 */
    f->r[1]=gb_0045fe20_read((0x00000010u+f->r[4]),4u);
    /* 00460276 */
    f->r[0]=f->r[7];
    /* 00460278 */
    a=f->r[5];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[5]=t;
    /* 0046027a */
    a=f->r[5];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046027c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046027d */
    MI_U16((0x00799aa4u+f->r[3]))=(uint16_t)((f->r[5]&0xffffu));
    /* 00460284 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00460285 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00460286 */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 00460288 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00460289 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0046028a */
    f->r[4]+=4;return;
gb_0045fe20_0046028b:;
    /* 0046028b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046028c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046028d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046028e */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 00460290 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00460291 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00460292 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004695d0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004695d0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004695d1 */
    f->r[3]=MI_U32((0x00000008u+f->r[4]));
    /* 004695d5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004695d6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004695d7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004695d8 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,5);f->r[7]=t;
    /* 004695da */
    f->r[5]=f->r[3];
gb_004695d0_004695dc:;
    /* 004695dc */
    f->r[0]=f->r[5];
    /* 004695de */
    a=f->r[0];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 004695e1 */
    a=f->r[0];b=f->r[5];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004695e3 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a10u+f->r[0]*8u));
    /* 004695eb */
    a=f->r[5];b=f->r[5];t=gb_common_alu(f,a,b,32,3);
    /* 004695ed */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004695d0_00469641;
    /* 004695ef */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004695f3 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 004695f5 */
    if((f->flags&MI_ZF))goto gb_004695d0_00469604;
    /* 004695f7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004695f9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004695fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004695ffu;f->boundary(0x004695fau,0x0046d100u,f);
    /* 004695ff */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 00469602 */
    goto gb_004695d0_00469638;
gb_004695d0_00469604:;
    /* 00469604 */
    f->r[6]=f->r[5];
    /* 00469606 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 00469608 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046960b */
    a=f->r[6];b=f->r[5];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046960d */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 00469610 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 00469616 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046961d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 00469624 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469625 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469626 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469627 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469628 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469629 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046962eu;f->boundary(0x00469629u,0x0047c2b0u,f);
    /* 0046962e */
    a=f->r[4];b=0x00000014u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 00469631 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)(0x00000004u);
gb_004695d0_00469638:;
    /* 00469638 */
    a=f->r[7];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00469639 */
    a=f->r[7];b=0x0000020du;t=gb_common_alu(f,a,b,32,1);
    /* 0046963f */
    if(gb_common_signed_less(f->flags))goto gb_004695d0_004695dc;
gb_004695d0_00469641:;
    /* 00469641 */
    a=f->r[5];b=f->r[5];t=gb_common_alu(f,a,b,32,5);f->r[5]=t;
    /* 00469643 */
    f->r[7]=f->r[3];
gb_004695d0_00469645:;
    /* 00469645 */
    f->r[0]=f->r[7];
    /* 00469647 */
    a=f->r[0];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046964a */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046964c */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[0]*8u));
    /* 00469654 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,3);
    /* 00469656 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004695d0_004696aa;
    /* 00469658 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0046965c */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 0046965e */
    if((f->flags&MI_ZF))goto gb_004695d0_0046966d;
    /* 00469660 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469662 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469663 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469668u;f->boundary(0x00469663u,0x0046d100u,f);
    /* 00469668 */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046966b */
    goto gb_004695d0_004696a1;
gb_004695d0_0046966d:;
    /* 0046966d */
    f->r[6]=f->r[7];
    /* 0046966f */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 00469671 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 00469674 */
    a=f->r[6];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 00469676 */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 00469679 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 0046967f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 00469686 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046968d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046968e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046968f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469690 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469691 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469692 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469697u;f->boundary(0x00469692u,0x0047c2b0u,f);
    /* 00469697 */
    a=f->r[4];b=0x00000014u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046969a */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)(0x00000004u);
gb_004695d0_004696a1:;
    /* 004696a1 */
    a=f->r[5];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004696a2 */
    a=f->r[5];b=0x0000020du;t=gb_common_alu(f,a,b,32,1);
    /* 004696a8 */
    if(gb_common_signed_less(f->flags))goto gb_004695d0_00469645;
gb_004695d0_004696aa:;
    /* 004696aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004696ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004696ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004696ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004696ae */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_0046d0f0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046d0f0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0046d0f4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d0f5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d0fau;f->boundary(0x0046d0f5u,0x00475b50u,f);
    /* 0046d0fa */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d0fd */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_0046d100(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046d100 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d101 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d102 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d103 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046d107 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d108 */
    f->r[7]=f->r[6];
    /* 0046d10a */
    a=f->r[5];b=f->r[5];t=gb_common_alu(f,a,b,32,5);f->r[5]=t;
    /* 0046d10c */
    a=f->r[7];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[7]=t;
    /* 0046d10f */
    a=f->r[7];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046d111 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0046d115 */
    a=f->r[7];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[7]=t;
    /* 0046d118 */
    a=MI_U16((0x007999e8u+f->r[7]));b=0x000000cau;t=gb_common_alu(f,a,b,16,1);
    /* 0046d121 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d131;
    /* 0046d123 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d124 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d129u;f->boundary(0x0046d124u,0x004c04a0u,f);
    /* 0046d129 */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d12c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046d12d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046d12e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046d12f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0046d130 */
    f->r[4]+=4;return;
gb_0046d100_0046d131:;
    /* 0046d131 */
    MI_U16((0x00799a20u+f->r[7]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046d138 */
    MI_U16((0x00799a1eu+f->r[7]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046d13f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[7])))&0xffu)<<0);
    /* 0046d145 */
    MI_U32((0x00bc9694u))=(uint32_t)(f->r[5]);
    /* 0046d14b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=gb_common_alu(f,a,b,8,3);
    /* 0046d14d */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d16e;
    /* 0046d14f */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999f4u+f->r[7])))&0xffffu)<<0);
    /* 0046d156 */
    a=(f->r[0]&0xffffu);b=(f->r[5]&0xffffu);t=gb_common_alu(f,a,b,16,1);
    /* 0046d159 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d16e;
    /* 0046d15b */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0046d161 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0046d164 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d165 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d166 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d16bu;f->boundary(0x0046d166u,0x004e9170u,f);
    /* 0046d16b */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_0046d100_0046d16e:;
    /* 0046d16e */
    MI_U16((0x007999f4u+f->r[7]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046d175 */
    MI_U8((0x007999ddu+f->r[7]))=(uint8_t)(0x00000000u);
    /* 0046d17c */
    MI_U8((0x00799a1bu+f->r[7]))=(uint8_t)(0x00000000u);
    /* 0046d183 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d184 */
    MI_U16((0x007999f8u+f->r[7]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046d18b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d190u;f->boundary(0x0046d18bu,0x0046ac50u,f);
    /* 0046d190 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[7])))&0xffffu)<<0);
    /* 0046d197 */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d19a */
    a=(f->r[0]&0xffffu);b=0x0000001fu;t=gb_common_alu(f,a,b,16,1);
    /* 0046d19e */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d1b2;
    /* 0046d1a0 */
    a=(f->r[0]&0xffffu);b=0x0000001eu;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1a4 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d1b2;
    /* 0046d1a6 */
    a=(f->r[0]&0xffffu);b=0x00000021u;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1aa */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d1b2;
    /* 0046d1ac */
    a=(f->r[0]&0xffffu);b=0x000000d1u;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1b0 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d1dc;
gb_0046d100_0046d1b2:;
    /* 0046d1b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d1b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d1b8u;f->boundary(0x0046d1b3u,0x0046d0f0u,f);
    /* 0046d1b8 */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d1bb */
    a=f->r[0];b=f->r[5];t=gb_common_alu(f,a,b,32,1);
    /* 0046d1bd */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0046d100_0046d1dc;
    /* 0046d1bf */
    a=f->r[0];b=f->r[6];t=gb_common_alu(f,a,b,32,1);
    /* 0046d1c1 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d1dc;
    /* 0046d1c3 */
    f->r[2]=f->r[0];
    /* 0046d1c5 */
    a=f->r[2];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046d1c8 */
    a=f->r[2];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046d1ca */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[2]*8u));
    /* 0046d1d2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d1d3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d1d4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d1d9u;f->boundary(0x0046d1d4u,0x0043fd00u,f);
    /* 0046d1d9 */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_0046d100_0046d1dc:;
    /* 0046d1dc */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[7])))&0xffffu)<<0);
    /* 0046d1e3 */
    a=(f->r[0]&0xffffu);b=0x0000004bu;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1e7 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d207;
    /* 0046d1e9 */
    a=(f->r[0]&0xffffu);b=0x0000004cu;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1ed */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d207;
    /* 0046d1ef */
    a=(f->r[0]&0xffffu);b=0x0000004au;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1f3 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d207;
    /* 0046d1f5 */
    a=(f->r[0]&0xffffu);b=0x000000b5u;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1f9 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d207;
    /* 0046d1fb */
    a=(f->r[0]&0xffffu);b=0x000000b6u;t=gb_common_alu(f,a,b,16,1);
    /* 0046d1ff */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d207;
    /* 0046d201 */
    a=(f->r[0]&0xffffu);b=0x000000c0u;t=gb_common_alu(f,a,b,16,1);
    /* 0046d205 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d212;
gb_0046d100_0046d207:;
    /* 0046d207 */
    f->r[0]=0x00000001u;
    /* 0046d20c */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046d210 */
    goto gb_0046d100_0046d217;
gb_0046d100_0046d212:;
    /* 0046d212 */
    f->r[0]=0x00000001u;
gb_0046d100_0046d217:;
    /* 0046d217 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[7])))&0xffu)<<0);
    /* 0046d21d */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=gb_common_alu(f,a,b,8,3);
    /* 0046d21f */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d22e;
    /* 0046d221 */
    a=MI_U16((0x007999eau+f->r[7]));b=(f->r[0]&0xffffu);t=gb_common_alu(f,a,b,16,1);
    /* 0046d228 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046d22c */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0046d100_0046d232;
gb_0046d100_0046d22e:;
    /* 0046d22e */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
gb_0046d100_0046d232:;
    /* 0046d232 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[7])))&0xffu)<<0);
    /* 0046d238 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d239 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=gb_common_alu(f,a,b,8,1);
    /* 0046d23b */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d244;
    /* 0046d23d */
    f->r[3]=0x00000004u;
    /* 0046d242 */
    goto gb_0046d100_0046d270;
gb_0046d100_0046d244:;
    /* 0046d244 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=gb_common_alu(f,a,b,8,1);
    /* 0046d246 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d24f;
    /* 0046d248 */
    f->r[3]=0x00000009u;
    /* 0046d24d */
    goto gb_0046d100_0046d270;
gb_0046d100_0046d24f:;
    /* 0046d24f */
    a=(f->r[0]&0xffu);b=0x00000004u;t=gb_common_alu(f,a,b,8,1);
    /* 0046d251 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d25a;
    /* 0046d253 */
    f->r[3]=0x00000010u;
    /* 0046d258 */
    goto gb_0046d100_0046d270;
gb_0046d100_0046d25a:;
    /* 0046d25a */
    a=(f->r[0]&0xffu);b=0x00000005u;t=gb_common_alu(f,a,b,8,1);
    /* 0046d25c */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d265;
    /* 0046d25e */
    f->r[3]=0x00000019u;
    /* 0046d263 */
    goto gb_0046d100_0046d270;
gb_0046d100_0046d265:;
    /* 0046d265 */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046d267 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=gb_common_alu(f,a,b,8,1);
    /* 0046d269 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0046d26c */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046d26d */
    a=f->r[3];b=0x00000024u;t=gb_common_alu(f,a,b,32,3);f->r[3]=t;
gb_0046d100_0046d270:;
    /* 0046d270 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[7]));
    /* 0046d277 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[7]));
    /* 0046d27e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d27f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d280 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d281 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d286u;f->boundary(0x0046d281u,0x0047c480u,f);
    /* 0046d286 */
    f->r[1]=MI_U32((0x007999e4u+f->r[7]));
    /* 0046d28c */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d28f */
    a=MI_U8((0x00af598cu+f->r[1]*4u));b=0x00000004u;t=gb_common_alu(f,a,b,8,3);
    /* 0046d297 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d2a5;
    /* 0046d299 */
    MI_U8((0x007999d8u+f->r[7]))=(uint8_t)(0x00000005u);
    /* 0046d2a0 */
    goto gb_0046d100_0046d354;
gb_0046d100_0046d2a5:;
    /* 0046d2a5 */
    MI_U16((0x007999e8u+f->r[7]))=(uint16_t)(0x00000063u);
    /* 0046d2ae */
    MI_U16((0x00799a08u+f->r[7]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046d2b5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a25u+f->r[7])))&0xffu)<<0);
    /* 0046d2bb */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0000001cu+f->r[4])))&0xffu)<<0);
    /* 0046d2bf */
    a=(f->r[2]&0xffu);b=0x00000007u;t=gb_common_alu(f,a,b,8,3);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0046d2c2 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=gb_common_alu(f,a,b,16,5);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0046d2c5 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[2]&0xffu))&0xffu)<<0);
    /* 0046d2c7 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046d2c8 */
    MI_U16((0x00799a22u+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0046d2cf */
    MI_U8((0x00799a24u+f->r[7]))=(uint8_t)(0x00000001u);
    /* 0046d2d6 */
    MI_U8((0x007999dbu+f->r[7]))=(uint8_t)(0x00000001u);
    /* 0046d2dd */
    MI_U8((0x00799a95u+f->r[7]))=(uint8_t)((f->r[1]&0xffu));
    /* 0046d2e3 */
    f->r[2]=MI_U32((0x007999e4u+f->r[7]));
    /* 0046d2e9 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0046d2ed */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046d2f3 */
    a=f->r[0];b=0x00000003u;t=gb_common_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046d2f6 */
    a=f->r[1];b=f->r[5];t=gb_common_alu(f,a,b,32,1);
    /* 0046d2f8 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d301;
    /* 0046d2fa */
    f->r[0]=MI_U32((0x00f56c54u));
    /* 0046d2ff */
    goto gb_0046d100_0046d334;
gb_0046d100_0046d301:;
    /* 0046d301 */
    a=f->r[0];b=f->r[5];t=gb_common_alu(f,a,b,32,1);
    /* 0046d303 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d30c;
    /* 0046d305 */
    f->r[0]=MI_U32((0x00f56c5cu));
    /* 0046d30a */
    goto gb_0046d100_0046d334;
gb_0046d100_0046d30c:;
    /* 0046d30c */
    a=f->r[0];b=0x00000001u;t=gb_common_alu(f,a,b,32,1);
    /* 0046d30f */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d31b;
    /* 0046d311 */
    f->r[0]=MI_U32((0x00f56c5cu));
    /* 0046d316 */
    a=f->r[0];b=0x00000009u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046d319 */
    goto gb_0046d100_0046d334;
gb_0046d100_0046d31b:;
    /* 0046d31b */
    a=f->r[0];b=0x00000002u;t=gb_common_alu(f,a,b,32,1);
    /* 0046d31e */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d32b;
    /* 0046d320 */
    f->r[1]=MI_U32((0x00f56c5cu));
    /* 0046d326 */
    f->r[0]=(0x00000012u+f->r[1]);
    /* 0046d329 */
    goto gb_0046d100_0046d334;
gb_0046d100_0046d32b:;
    /* 0046d32b */
    f->r[2]=MI_U32((0x00f56c5cu));
    /* 0046d331 */
    f->r[0]=(0x0000001bu+f->r[2]);
gb_0046d100_0046d334:;
    /* 0046d334 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[7]));
    /* 0046d33b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d33d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d33e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[7]));
    /* 0046d345 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d347 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d349 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d34a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d34b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d34c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d351u;f->boundary(0x0046d34cu,0x0047bf50u,f);
    /* 0046d351 */
    a=f->r[4];b=0x0000001cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_0046d100_0046d354:;
    /* 0046d354 */
    a=f->r[3];b=0x00000001u;t=gb_common_alu(f,a,b,32,1);
    /* 0046d357 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_0046d100_0046d46a;
    /* 0046d35d */
    f->r[5]=0x005ccd1cu;
    /* 0046d362 */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
gb_0046d100_0046d363:;
    /* 0046d363 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[7]));
    /* 0046d36a */
    f->r[1]=MI_U32((0xfffffffcu+f->r[5]));
    /* 0046d36d */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 0046d373 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[7]));
    /* 0046d37a */
    a=f->r[1];b=f->r[2];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046d37c */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 0046d37f */
    a=f->r[0];b=f->r[2];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046d381 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0046d384 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0046d387 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0046d38a */
    f->r[2]=(0x00000000u+f->r[6]+f->r[2]*4u);
    /* 0046d38d */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046d38f */
    a=MI_U8((0x00af598cu+f->r[2]*4u));b=0x00000004u;t=gb_common_alu(f,a,b,8,3);
    /* 0046d397 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d460;
    /* 0046d39d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d39f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d3a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d3a2 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046d3a7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d3a8 */
    a=0x00000063u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d3aa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d3ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d3b0u;f->boundary(0x0046d3abu,0x0046a400u,f);
    /* 0046d3b0 */
    f->r[6]=f->r[0];
    /* 0046d3b2 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 0046d3b6 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046d3b9 */
    a=f->r[6];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046d3bb */
    a=f->r[4];b=0x00000018u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d3be */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046d3c1 */
    f->r[1]=MI_U32((0x007999e4u+f->r[6]));
    /* 0046d3c7 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[1])))&0xffu)<<0);
    /* 0046d3cd */
    a=f->r[1];b=0x00000003u;t=gb_common_alu(f,a,b,32,3);f->r[1]=t;
    /* 0046d3d0 */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,3);
    /* 0046d3d2 */
    if((f->flags&MI_ZF))goto gb_0046d100_0046d3dc;
    /* 0046d3d4 */
    f->r[1]=MI_U32((0x00f56c54u));
    /* 0046d3da */
    goto gb_0046d100_0046d411;
gb_0046d100_0046d3dc:;
    /* 0046d3dc */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,3);
    /* 0046d3de */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d3e8;
    /* 0046d3e0 */
    f->r[1]=MI_U32((0x00f56c5cu));
    /* 0046d3e6 */
    goto gb_0046d100_0046d411;
gb_0046d100_0046d3e8:;
    /* 0046d3e8 */
    a=f->r[1];b=0x00000001u;t=gb_common_alu(f,a,b,32,1);
    /* 0046d3eb */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d3f8;
    /* 0046d3ed */
    f->r[2]=MI_U32((0x00f56c5cu));
    /* 0046d3f3 */
    f->r[1]=(0x00000009u+f->r[2]);
    /* 0046d3f6 */
    goto gb_0046d100_0046d411;
gb_0046d100_0046d3f8:;
    /* 0046d3f8 */
    a=f->r[1];b=0x00000002u;t=gb_common_alu(f,a,b,32,1);
    /* 0046d3fb */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d408;
    /* 0046d3fd */
    f->r[1]=MI_U32((0x00f56c5cu));
    /* 0046d403 */
    a=f->r[1];b=0x00000012u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046d406 */
    goto gb_0046d100_0046d411;
gb_0046d100_0046d408:;
    /* 0046d408 */
    f->r[2]=MI_U32((0x00f56c5cu));
    /* 0046d40e */
    f->r[1]=(0x0000001bu+f->r[2]);
gb_0046d100_0046d411:;
    /* 0046d411 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046d418 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d41a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d41b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046d422 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d424 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d426 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d427 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d428 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d429 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d42eu;f->boundary(0x0046d429u,0x0047bf50u,f);
    /* 0046d42e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a25u+f->r[6])))&0xffu)<<0);
    /* 0046d434 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000038u+f->r[4])))&0xffu)<<0);
    /* 0046d438 */
    a=(f->r[0]&0xffu);b=0x00000007u;t=gb_common_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0046d43a */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=gb_common_alu(f,a,b,16,5);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0046d43d */
    f->r[1]=(f->r[1]&0xffffff00u)|((((f->r[0]&0xffu))&0xffu)<<0);
    /* 0046d43f */
    a=f->r[4];b=0x0000001cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d442 */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0046d443 */
    MI_U16((0x00799a22u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0046d44a */
    MI_U16((0x00799a08u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0046d453 */
    MI_U8((0x00799a24u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0046d45a */
    MI_U8((0x00799a95u+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
gb_0046d100_0046d460:;
    /* 0046d460 */
    a=f->r[5];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046d463 */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046d464 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d363;
gb_0046d100_0046d46a:;
    /* 0046d46a */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0046d46e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046d46f */
    a=f->r[0];b=0x00000001u;t=gb_common_alu(f,a,b,32,1);
    /* 0046d472 */
    if(!(f->flags&MI_ZF))goto gb_0046d100_0046d479;
    /* 0046d474 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d479u;f->boundary(0x0046d474u,0x005178b0u,f);
gb_0046d100_0046d479:;
    /* 0046d479 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046d47a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046d47b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046d47c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0046d47d */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004bfee0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bfee0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004bfee4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfee5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfee6 */
    f->r[5]=f->r[0];
    /* 004bfee8 */
    a=f->r[5];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[5]=t;
    /* 004bfeeb */
    a=f->r[5];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 004bfeed */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfeee */
    a=f->r[5];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[5]=t;
    /* 004bfef1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfef2 */
    f->r[6]=0x005df818u;
    /* 004bfef7 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[5]));
    /* 004bfefe */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[5]));
gb_004bfee0_004bff05:;
    /* 004bff05 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004bff07 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a82u+f->r[5])))&0xffu)<<0);
    /* 004bff0d */
    a=f->r[0];b=0x00000000u;t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004bff10 */
    if((f->flags&MI_ZF))goto gb_004bfee0_004bff24;
    /* 004bff12 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bff13 */
    if((f->flags&MI_ZF))goto gb_004bfee0_004bff1b;
    /* 004bff15 */
    f->r[0]=f->r[7];
    /* 004bff17 */
    f->r[1]=f->r[3];
    /* 004bff19 */
    goto gb_004bfee0_004bff2d;
gb_004bfee0_004bff1b:;
    /* 004bff1b */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 004bff1d */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004bff20 */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bff22 */
    goto gb_004bfee0_004bff2b;
gb_004bfee0_004bff24:;
    /* 004bff24 */
    f->r[0]=MI_U32((0x00000004u+f->r[6]));
    /* 004bff27 */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 004bff29 */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
gb_004bfee0_004bff2b:;
    /* 004bff2b */
    a=f->r[1];b=f->r[3];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
gb_004bfee0_004bff2d:;
    /* 004bff2d */
    f->r[2]=MI_U32((0xfffffffcu+f->r[6]));
    /* 004bff30 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bff31 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bff32 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004bff36 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bff37 */
    MI_U32((0x00bc9738u))=(uint32_t)(f->r[2]);
    /* 004bff3d */
    MI_U32((0x00bc973cu))=(uint32_t)(f->r[2]);
    /* 004bff43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bff48u;f->boundary(0x004bff43u,0x0047c480u,f);
    /* 004bff48 */
    a=f->r[6];b=0x00000010u;t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004bff4b */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bff4e */
    a=f->r[6];b=0x005df858u;t=gb_common_alu(f,a,b,32,1);
    /* 004bff54 */
    if(gb_common_signed_less(f->flags))goto gb_004bfee0_004bff05;
    /* 004bff56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bff57 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bff58 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004bff59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004bff5a */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004c04a0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c04a0 */
    a=f->r[4];b=0x00000014u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c04a3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c04a4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c04a5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c04a6 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004c04aa */
    f->r[5]=f->r[6];
    /* 004c04ac */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,5);f->r[3]=t;
    /* 004c04ae */
    a=f->r[5];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[5]=t;
    /* 004c04b1 */
    a=f->r[5];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 004c04b3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c04b4 */
    a=f->r[5];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[5]=t;
    /* 004c04b7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c04b8 */
    MI_U32((0x00bc9694u))=(uint32_t)(f->r[3]);
    /* 004c04be */
    MI_U16((0x00799a20u+f->r[5]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004c04c5 */
    MI_U16((0x00799a1eu+f->r[5]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004c04cc */
    MI_U8((0x00799a1bu+f->r[5]))=(uint8_t)((f->r[3]&0xffu));
    /* 004c04d2 */
    MI_U16((0x007999f8u+f->r[5]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004c04d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c04deu;f->boundary(0x004c04d9u,0x0046ac50u,f);
    /* 004c04de */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c04df */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c04e4u;f->boundary(0x004c04dfu,0x004bfee0u,f);
    /* 004c04e4 */
    MI_U16((0x007999e8u+f->r[5]))=(uint16_t)(0x00000063u);
    /* 004c04ed */
    MI_U16((0x00799a08u+f->r[5]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004c04f4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a25u+f->r[5])))&0xffu)<<0);
    /* 004c04fa */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=gb_common_alu(f,a,b,16,5);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004c04fd */
    a=(f->r[0]&0xffu);b=0x00000007u;t=gb_common_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c04ff */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0502 */
    f->r[1]=(f->r[1]&0xffffff00u)|((((f->r[0]&0xffu))&0xffu)<<0);
    /* 004c0504 */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
    /* 004c0506 */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004c0507 */
    MI_U16((0x00799a22u+f->r[5]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004c050e */
    f->r[1]=MI_U32((0x00bca2e0u));
    /* 004c0514 */
    MI_U8((0x00799a24u+f->r[5]))=(uint8_t)(0x00000001u);
    /* 004c051b */
    MI_U8((0x007999dbu+f->r[5]))=(uint8_t)(0x00000001u);
    /* 004c0522 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a82u+f->r[5])))&0xffu)<<0);
    /* 004c0528 */
    f->r[6]=f->r[2];
    /* 004c052a */
    a=f->r[6];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c052c */
    a=f->r[6];b=0x00000008u;t=gb_common_alu(f,a,b,32,1);
    /* 004c052f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c0533 */
    if(gb_common_signed_less(f->flags))goto gb_004c04a0_004c053c;
    /* 004c0535 */
    a=f->r[6];b=0x00000008u;t=gb_common_alu(f,a,b,32,1);f->r[6]=t;
    /* 004c0538 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
gb_004c04a0_004c053c:;
    /* 004c053c */
    f->r[7]=f->r[6];
    /* 004c053e */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000004u);
    /* 004c0546 */
    a=f->r[7];b=0x00000004u;t=gb_common_shift(f,a,b,32,0);f->r[7]=t;
    /* 004c0549 */
    a=f->r[7];b=0x005df78cu;t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c054f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0553 */
    goto gb_004c04a0_004c0557;
gb_004c04a0_004c0555:;
    /* 004c0555 */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,5);f->r[3]=t;
gb_004c04a0_004c0557:;
    /* 004c0557 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 004c0559 */
    f->r[0]=f->r[6];
    /* 004c055b */
    a=f->r[0];b=f->r[3];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c055d */
    if((f->flags&MI_ZF))goto gb_004c04a0_004c0597;
    /* 004c055f */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0560 */
    if((f->flags&MI_ZF))goto gb_004c04a0_004c0576;
    /* 004c0562 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[5]));
    /* 004c0569 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[5]));
    /* 004c0570 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0574 */
    goto gb_004c04a0_004c05ba;
gb_004c04a0_004c0576:;
    /* 004c0576 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[5]));
    /* 004c057d */
    f->r[0]=f->r[1];
    /* 004c057f */
    a=f->r[0];b=0x00000004u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c0582 */
    a=f->r[3];b=MI_U32((0x005df818u+f->r[0]));t=gb_common_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c0588 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[5]));
    /* 004c058f */
    a=f->r[2];b=MI_U32((0x005df81cu+f->r[0]));t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c0595 */
    goto gb_004c04a0_004c05b6;
gb_004c04a0_004c0597:;
    /* 004c0597 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[5]));
    /* 004c059e */
    f->r[0]=f->r[1];
    /* 004c05a0 */
    a=f->r[0];b=0x00000004u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c05a3 */
    a=f->r[3];b=MI_U32((0x005df81cu+f->r[0]));t=gb_common_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c05a9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[5]));
    /* 004c05b0 */
    a=f->r[2];b=MI_U32((0x005df818u+f->r[0]));t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
gb_004c04a0_004c05b6:;
    /* 004c05b6 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
gb_004c04a0_004c05ba:;
    /* 004c05ba */
    a=f->r[1];b=0x00000004u;t=gb_common_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c05bd */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004c05c5 */
    f->r[0]=MI_U32((0x005df814u+f->r[1]));
    /* 004c05cb */
    f->r[1]=(0x005df814u+f->r[1]);
    /* 004c05d1 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 004c05d3 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c05d7 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c04a0_004c06ba;
    /* 004c05dd */
    f->r[7]=0x005ccd14u;
gb_004c04a0_004c05e2:;
    /* 004c05e2 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004c05e6 */
    f->r[6]=MI_U32((0x00000000u+f->r[7]));
    /* 004c05e8 */
    f->r[0]=MI_U32((0xfffffffcu+f->r[7]));
    /* 004c05eb */
    a=f->r[1];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c05ed */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c05ef */
    a=f->r[0];b=f->r[3];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c05f1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c05f3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c05f4 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004c05fa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c05fb */
    a=0x00000063u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c05fd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c05fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0603u;f->boundary(0x004c05feu,0x0046a400u,f);
    /* 004c0603 */
    f->r[6]=f->r[0];
    /* 004c0605 */
    a=f->r[4];b=0x00000018u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0608 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c060b */
    a=f->r[6];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c060d */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c0610 */
    f->r[2]=MI_U32((0x007999e4u+f->r[6]));
    /* 004c0616 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 004c061c */
    a=f->r[1];b=0x00000003u;t=gb_common_alu(f,a,b,32,3);f->r[1]=t;
    /* 004c061f */
    if(!(f->flags&MI_ZF))goto gb_004c04a0_004c0629;
    /* 004c0621 */
    f->r[1]=MI_U32((0x00f56c5cu));
    /* 004c0627 */
    goto gb_004c04a0_004c0652;
gb_004c04a0_004c0629:;
    /* 004c0629 */
    a=f->r[1];b=0x00000001u;t=gb_common_alu(f,a,b,32,1);
    /* 004c062c */
    if(!(f->flags&MI_ZF))goto gb_004c04a0_004c0639;
    /* 004c062e */
    f->r[1]=MI_U32((0x00f56c5cu));
    /* 004c0634 */
    a=f->r[1];b=0x00000009u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c0637 */
    goto gb_004c04a0_004c0652;
gb_004c04a0_004c0639:;
    /* 004c0639 */
    a=f->r[1];b=0x00000002u;t=gb_common_alu(f,a,b,32,1);
    /* 004c063c */
    if(!(f->flags&MI_ZF))goto gb_004c04a0_004c0649;
    /* 004c063e */
    f->r[2]=MI_U32((0x00f56c5cu));
    /* 004c0644 */
    f->r[1]=(0x00000012u+f->r[2]);
    /* 004c0647 */
    goto gb_004c04a0_004c0652;
gb_004c04a0_004c0649:;
    /* 004c0649 */
    f->r[1]=MI_U32((0x00f56c5cu));
    /* 004c064f */
    a=f->r[1];b=0x0000001bu;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
gb_004c04a0_004c0652:;
    /* 004c0652 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c0659 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c065b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c065c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c0663 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0665 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0667 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0668 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0669 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c066a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c066fu;f->boundary(0x004c066au,0x0047bf50u,f);
    /* 004c066f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a25u+f->r[6])))&0xffu)<<0);
    /* 004c0675 */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 004c0679 */
    a=(f->r[2]&0xffu);b=0x00000007u;t=gb_common_alu(f,a,b,8,3);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c067c */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=gb_common_alu(f,a,b,16,5);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004c067f */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[2]&0xffu))&0xffu)<<0);
    /* 004c0681 */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 004c0683 */
    a=f->r[4];b=0x0000001cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0686 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0687 */
    MI_U16((0x00799a22u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004c068e */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0692 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0693 */
    a=f->r[7];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c0696 */
    MI_U16((0x00799a08u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 004c069f */
    a=f->r[0];b=f->r[2];t=gb_common_alu(f,a,b,32,1);
    /* 004c06a1 */
    MI_U8((0x00799a24u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 004c06a8 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c06ac */
    if(gb_common_signed_less(f->flags))goto gb_004c04a0_004c05e2;
    /* 004c06b2 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004c06b6 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
gb_004c04a0_004c06ba:;
    /* 004c06ba */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004c06be */
    a=f->r[7];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c06c1 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c06c2 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c06c6 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c06ca */
    if(!(f->flags&MI_ZF))goto gb_004c04a0_004c0555;
    /* 004c06d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c06d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c06d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c06d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c06d4 */
    a=f->r[4];b=0x00000014u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c06d7 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004c3950(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c3950 */
    a=f->r[4];b=0x00000024u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c3953 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3954 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3955 */
    f->r[1]=0x00799af0u;
    /* 004c395a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c395b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c395c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000012u);
    /* 004c3964 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x0000000bu);
    /* 004c396c */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x0000000du);
    /* 004c3974 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000010u);
    /* 004c397c */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0x00000011u);
    /* 004c3984 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x00000013u);
    /* 004c398c */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00002710u);
    /* 004c3994 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004c399c */
    f->r[3]=0x00000001u;
    /* 004c39a1 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
gb_004c3950_004c39a5:;
    /* 004c39a5 */
    a=MI_U8((0xfffffff0u+f->r[1]));b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c39a9 */
    if(!(f->flags&MI_ZF))goto gb_004c3950_004c39ef;
    /* 004c39ab */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 004c39ae */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,5);f->r[7]=t;
    /* 004c39b0 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=gb_common_alu(f,a,b,16,1);
    /* 004c39b4 */
    if((f->flags&MI_ZF))goto gb_004c3950_004c39bc;
    /* 004c39b6 */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=gb_common_alu(f,a,b,16,1);
    /* 004c39ba */
    if(!(f->flags&MI_ZF))goto gb_004c3950_004c39ef;
gb_004c3950_004c39bc:;
    /* 004c39bc */
    f->r[6]=(0x0000001cu+f->r[4]);
    /* 004c39c0 */
    f->r[5]=0x00000006u;
gb_004c3950_004c39c5:;
    /* 004c39c5 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 004c39c7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c39c8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c39c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c39ceu;f->boundary(0x004c39c9u,0x0045d510u,f);
    /* 004c39ce */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c39d1 */
    a=f->r[7];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c39d3 */
    a=f->r[6];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c39d6 */
    a=f->r[5];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004c39d7 */
    if(!(f->flags&MI_ZF))goto gb_004c3950_004c39c5;
    /* 004c39d9 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,3);
    /* 004c39db */
    if((f->flags&MI_ZF))goto gb_004c3950_004c39eb;
    /* 004c39dd */
    a=f->r[7];b=MI_U32((0x00000018u+f->r[4]));t=gb_common_alu(f,a,b,32,1);
    /* 004c39e1 */
    if(!gb_common_signed_less(f->flags))goto gb_004c3950_004c39eb;
    /* 004c39e3 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c39e7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
gb_004c3950_004c39eb:;
    /* 004c39eb */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
gb_004c3950_004c39ef:;
    /* 004c39ef */
    a=f->r[1];b=0x00000108u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c39f5 */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004c39f6 */
    a=f->r[1];b=0x0089b6e8u;t=gb_common_alu(f,a,b,32,1);
    /* 004c39fc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c3a00 */
    if(gb_common_signed_less(f->flags))goto gb_004c3950_004c39a5;
    /* 004c3a02 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c3a06 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,5);f->r[7]=t;
    /* 004c3a08 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 004c3a0a */
    f->r[5]=0x00002710u;
    /* 004c3a0f */
    if((f->flags&MI_ZF))goto gb_004c3950_004c3aaf;
    /* 004c3a15 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004c3a19 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,3);
    /* 004c3a1b */
    if((f->flags&MI_ZF))goto gb_004c3950_004c3aaf;
    /* 004c3a21 */
    f->r[3]=(0x0000001cu+f->r[4]);
    /* 004c3a25 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000006u);
gb_004c3950_004c3a2d:;
    /* 004c3a2d */
    f->r[6]=MI_U32((0x00000000u+f->r[3]));
    /* 004c3a2f */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004c3a33 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3a34 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3a35 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3a3au;f->boundary(0x004c3a35u,0x0045d510u,f);
    /* 004c3a3a */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3a3d */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 004c3a3f */
    if((f->flags&MI_ZF))goto gb_004c3950_004c3a49;
    /* 004c3a41 */
    a=f->r[0];b=f->r[5];t=gb_common_alu(f,a,b,32,1);
    /* 004c3a43 */
    if(!gb_common_signed_less(f->flags))goto gb_004c3950_004c3a49;
    /* 004c3a45 */
    f->r[7]=f->r[6];
    /* 004c3a47 */
    f->r[5]=f->r[0];
gb_004c3950_004c3a49:;
    /* 004c3a49 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004c3a4d */
    a=f->r[3];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c3a50 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c3a51 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c3a55 */
    if(!(f->flags&MI_ZF))goto gb_004c3950_004c3a2d;
    /* 004c3a57 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,3);
    /* 004c3a59 */
    if((f->flags&MI_ZF))goto gb_004c3950_004c3aab;
    /* 004c3a5b */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 004c3a5f */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3a61 */
    f->r[2]=f->r[6];
    /* 004c3a63 */
    a=f->r[2];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c3a66 */
    a=f->r[2];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c3a68 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a99u+f->r[2]*8u)))&0xffu)<<0);
    /* 004c3a6f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004c3a72 */
    a=f->r[1];b=0x00000004u;t=gb_common_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c3a75 */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c3a77 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*2u);
    /* 004c3a7a */
    f->r[1]=0x00000000u;
    /* 004c3a7f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007900ecu+f->r[2]*2u));
    /* 004c3a87 */
    a=f->r[0];b=f->r[5];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3a89 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004c3a8f */
    t=((f->flags&MI_SF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c3a92 */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004c3a93 */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,3);f->r[0]=t;
    /* 004c3a95 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3a96 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3a97 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3a98 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3a99 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3a9eu;f->boundary(0x004c3a99u,0x0045b2a0u,f);
    /* 004c3a9e */
    a=f->r[4];b=0x00000010u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3aa1 */
    f->r[0]=f->r[6];
    /* 004c3aa3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3aa4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3aa5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c3aa6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c3aa7 */
    a=f->r[4];b=0x00000024u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3aaa */
    f->r[4]+=4;return;
gb_004c3950_004c3aab:;
    /* 004c3aab */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
gb_004c3950_004c3aaf:;
    /* 004c3aaf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3ab0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3ab1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c3ab2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c3ab3 */
    a=f->r[4];b=0x00000024u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3ab6 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"
#include <stdlib.h>








static const uint8_t gb_004c3ac0_rom_004c3d04[]={0xad,0x3b,0x4c,0x00,0xb4,0x3b,0x4c,0x00,0xbb,0x3b,0x4c,0x00,0xc2,0x3b,0x4c,0x00};
static const uint8_t gb_004c3ac0_rom_004c3d14[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t gb_004c3ac0_rom_004c3db4[]={0xea,0x3b,0x4c,0x00,0xf1,0x3b,0x4c,0x00,0xf8,0x3b,0x4c,0x00,0xff,0x3b,0x4c,0x00};
static const uint8_t gb_004c3ac0_rom_004c3dc4[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t gb_004c3ac0_rom_004c3e64[]={0x1d,0x3c,0x4c,0x00,0xd7,0x3c,0x4c,0x00,0x31,0x3c,0x4c,0x00,0x3b,0x3c,0x4c,0x00,0x45,0x3c,0x4c,0x00,0x4f,0x3c,0x4c,0x00,0x59,0x3c,0x4c,0x00,0x67,0x3c,0x4c,0x00,0x6e,0x3c,0x4c,0x00,0x75,0x3c,0x4c,0x00,0x7c,0x3c,0x4c,0x00,0x83,0x3c,0x4c,0x00,0x60,0x3c,0x4c,0x00,0xa6,0x3c,0x4c,0x00,0x8a,0x3c,0x4c,0x00,0x91,0x3c,0x4c,0x00,0x98,0x3c,0x4c,0x00,0x9f,0x3c,0x4c,0x00,0xd7,0x3c,0x4c,0x00,0x27,0x3c,0x4c,0x00,0xad,0x3c,0x4c,0x00,0xb4,0x3c,0x4c,0x00,0xbb,0x3c,0x4c,0x00,0xc2,0x3c,0x4c,0x00,0xc9,0x3c,0x4c,0x00,0xd0,0x3c,0x4c,0x00};
static __inline uint32_t gb_004c3ac0_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004c3d04u&&width<=16u&&(offset=address-0x004c3d04u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c3ac0_rom_004c3d04[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c3d14u&&width<=158u&&(offset=address-0x004c3d14u)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c3ac0_rom_004c3d14[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c3db4u&&width<=16u&&(offset=address-0x004c3db4u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c3ac0_rom_004c3db4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c3dc4u&&width<=158u&&(offset=address-0x004c3dc4u)<=158u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c3ac0_rom_004c3dc4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c3e64u&&width<=104u&&(offset=address-0x004c3e64u)<=104u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c3ac0_rom_004c3e64[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

static __inline void gb_004c3ac0_idiv32(struct restore_god_building_frame *f,uint32_t divisor_bits)
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

void restore_god_C_004c3ac0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c3ac0 */
    a=f->r[4];b=0x00000018u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c3ac3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3ac4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3ac5 */
    a=f->r[6];b=f->r[6];t=gb_common_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c3ac7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3ac8 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x0000004au);
    /* 004c3ad0 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000006fu);
    /* 004c3ad8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000071u);
    /* 004c3ae0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 004c3ae8 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000011u);
    /* 004c3af0 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000013u);
    /* 004c3af8 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
gb_004c3ac0_004c3afa:;
    /* 004c3afa */
    f->r[1]=gb_004c3ac0_read((0x0000000cu+f->r[4]+f->r[0]*1u),4u);
    /* 004c3afe */
    f->r[2]=gb_004c3ac0_read((0x00705af0u+f->r[1]*4u),4u);
    /* 004c3b05 */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,3);
    /* 004c3b07 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c3ac0_004c3b0c;
    /* 004c3b09 */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c3b0a */
    goto gb_004c3ac0_004c3b2a;
gb_004c3ac0_004c3b0c:;
    /* 004c3b0c */
    f->r[1]=gb_004c3ac0_read((0x00000018u+f->r[4]+f->r[0]*1u),4u);
    /* 004c3b10 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,3);
    /* 004c3b12 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c3ac0_004c3b22;
    /* 004c3b14 */
    f->r[2]=gb_004c3ac0_read((0x0078d64cu+f->r[1]*4u),4u);
    /* 004c3b1b */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,3);
    /* 004c3b1d */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c3ac0_004c3b22;
    /* 004c3b1f */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c3b20 */
    goto gb_004c3ac0_004c3b2a;
gb_004c3ac0_004c3b22:;
    /* 004c3b22 */
    MI_U32((0x0000000cu+f->r[4]+f->r[0]*1u))=(uint32_t)(0x00000000u);
gb_004c3ac0_004c3b2a:;
    /* 004c3b2a */
    a=f->r[0];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c3b2d */
    a=f->r[0];b=0x0000000cu;t=gb_common_alu(f,a,b,32,1);
    /* 004c3b30 */
    if(gb_common_signed_less(f->flags))goto gb_004c3ac0_004c3afa;
    /* 004c3b32 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,5);f->r[7]=t;
    /* 004c3b34 */
    a=f->r[6];b=f->r[6];t=gb_common_alu(f,a,b,32,3);
    /* 004c3b36 */
    if((f->flags&MI_ZF))goto gb_004c3ac0_004c3b6b;
    /* 004c3b38 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3b3du;f->boundary(0x004c3b38u,0x00562348u,f);
    /* 004c3b3d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c3b3e */
    a=f->r[6];gb_004c3ac0_idiv32(f,a);
    /* 004c3b40 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3b42 */
    f->r[1]=(0x0000000cu+f->r[4]);
gb_004c3ac0_004c3b46:;
    /* 004c3b46 */
    a=gb_004c3ac0_read((0x00000000u+f->r[1]),4u);b=0x00000000u;t=gb_common_alu(f,a,b,32,1);
    /* 004c3b49 */
    if((f->flags&MI_ZF))goto gb_004c3ac0_004c3b50;
    /* 004c3b4b */
    a=f->r[7];b=f->r[2];t=gb_common_alu(f,a,b,32,1);
    /* 004c3b4d */
    if((f->flags&MI_ZF))goto gb_004c3ac0_004c3b62;
    /* 004c3b4f */
    a=f->r[7];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
gb_004c3ac0_004c3b50:;
    /* 004c3b50 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c3b51 */
    a=f->r[1];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c3b54 */
    a=f->r[0];b=0x00000003u;t=gb_common_alu(f,a,b,32,1);
    /* 004c3b57 */
    if(gb_common_signed_less(f->flags))goto gb_004c3ac0_004c3b46;
    /* 004c3b59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3b5a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3b5b */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3b5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c3b5e */
    a=f->r[4];b=0x00000018u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3b61 */
    f->r[4]+=4;return;
gb_004c3ac0_004c3b62:;
    /* 004c3b62 */
    f->r[5]=gb_004c3ac0_read((0x0000000cu+f->r[4]+f->r[0]*4u),4u);
    /* 004c3b66 */
    a=f->r[5];b=0xffffffffu;t=gb_common_alu(f,a,b,32,1);
    /* 004c3b69 */
    if(!(f->flags&MI_ZF))goto gb_004c3ac0_004c3b74;
gb_004c3ac0_004c3b6b:;
    /* 004c3b6b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3b6c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3b6d */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3b6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c3b70 */
    a=f->r[4];b=0x00000018u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3b73 */
    f->r[4]+=4;return;
gb_004c3ac0_004c3b74:;
    /* 004c3b74 */
    f->r[7]=0x00000001u;
    /* 004c3b79 */
    f->r[6]=0x00799af0u;
gb_004c3ac0_004c3b7e:;
    /* 004c3b7e */
    a=gb_004c3ac0_read((0xfffffff0u+f->r[6]),1u);b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c3b82 */
    if(!(f->flags&MI_ZF))goto gb_004c3ac0_004c3ce5;
    /* 004c3b88 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)gb_004c3ac0_read((0x00000000u+f->r[6]),2u);
    /* 004c3b8b */
    a=f->r[0];b=f->r[5];t=gb_common_alu(f,a,b,32,1);
    /* 004c3b8d */
    if(!(f->flags&MI_ZF))goto gb_004c3ac0_004c3ce5;
    /* 004c3b93 */
    f->r[1]=(0xffffffe0u+f->r[0]);
    /* 004c3b96 */
    a=f->r[1];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 004c3b9c */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_004c3ac0_004c3bc2;
    /* 004c3b9e */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
    /* 004c3ba0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((gb_004c3ac0_read((0x004c3d14u+f->r[1]),1u))&0xffu)<<0);
    /* 004c3ba6 */
    switch(gb_004c3ac0_read((0x004c3d04u+f->r[2]*4u),4u)){case 0x004c3badu:goto gb_004c3ac0_004c3bad;case 0x004c3bb4u:goto gb_004c3ac0_004c3bb4;case 0x004c3bbbu:goto gb_004c3ac0_004c3bbb;case 0x004c3bc2u:goto gb_004c3ac0_004c3bc2;default:abort();}
gb_004c3ac0_004c3bad:;
    /* 004c3bad */
    f->r[0]=0x0000012cu;
    /* 004c3bb2 */
    goto gb_004c3ac0_004c3bc7;
gb_004c3ac0_004c3bb4:;
    /* 004c3bb4 */
    f->r[0]=0x000001f4u;
    /* 004c3bb9 */
    goto gb_004c3ac0_004c3bc7;
gb_004c3ac0_004c3bbb:;
    /* 004c3bbb */
    f->r[0]=0x000186a0u;
    /* 004c3bc0 */
    goto gb_004c3ac0_004c3bc7;
gb_004c3ac0_004c3bc2:;
    /* 004c3bc2 */
    f->r[0]=0x000000c8u;
gb_004c3ac0_004c3bc7:;
    /* 004c3bc7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)gb_004c3ac0_read((0x0000002cu+f->r[6]),2u);
    /* 004c3bcb */
    a=f->r[0];b=f->r[2];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3bcd */
    if(!(f->flags&MI_SF))goto gb_004c3ac0_004c3bd3;
    /* 004c3bcf */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c3bd1 */
    goto gb_004c3ac0_004c3c08;
gb_004c3ac0_004c3bd3:;
    /* 004c3bd3 */
    a=f->r[1];b=0x0000009du;t=gb_common_alu(f,a,b,32,1);
    /* 004c3bd9 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_004c3ac0_004c3bff;
    /* 004c3bdb */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3bdd */
    f->r[0]=(f->r[0]&0xffffff00u)|(((gb_004c3ac0_read((0x004c3dc4u+f->r[1]),1u))&0xffu)<<0);
    /* 004c3be3 */
    switch(gb_004c3ac0_read((0x004c3db4u+f->r[0]*4u),4u)){case 0x004c3beau:goto gb_004c3ac0_004c3bea;case 0x004c3bf1u:goto gb_004c3ac0_004c3bf1;case 0x004c3bf8u:goto gb_004c3ac0_004c3bf8;case 0x004c3bffu:goto gb_004c3ac0_004c3bff;default:abort();}
gb_004c3ac0_004c3bea:;
    /* 004c3bea */
    f->r[0]=0x0000012cu;
    /* 004c3bef */
    goto gb_004c3ac0_004c3c04;
gb_004c3ac0_004c3bf1:;
    /* 004c3bf1 */
    f->r[0]=0x000001f4u;
    /* 004c3bf6 */
    goto gb_004c3ac0_004c3c04;
gb_004c3ac0_004c3bf8:;
    /* 004c3bf8 */
    f->r[0]=0x000186a0u;
    /* 004c3bfd */
    goto gb_004c3ac0_004c3c04;
gb_004c3ac0_004c3bff:;
    /* 004c3bff */
    f->r[0]=0x000000c8u;
gb_004c3ac0_004c3c04:;
    /* 004c3c04 */
    a=f->r[0];b=f->r[2];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3c06 */
    f->r[1]=f->r[0];
gb_004c3ac0_004c3c08:;
    /* 004c3c08 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)gb_004c3ac0_read((0x00000002u+f->r[6]),2u);
    /* 004c3c0c */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c3c0d */
    a=f->r[0];b=0x00000019u;t=gb_common_alu(f,a,b,32,1);
    /* 004c3c10 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_004c3ac0_004c3cd7;
    /* 004c3c16 */
    switch(gb_004c3ac0_read((0x004c3e64u+f->r[0]*4u),4u)){case 0x004c3c1du:goto gb_004c3ac0_004c3c1d;case 0x004c3c27u:goto gb_004c3ac0_004c3c27;case 0x004c3c31u:goto gb_004c3ac0_004c3c31;case 0x004c3c3bu:goto gb_004c3ac0_004c3c3b;case 0x004c3c45u:goto gb_004c3ac0_004c3c45;case 0x004c3c4fu:goto gb_004c3ac0_004c3c4f;case 0x004c3c59u:goto gb_004c3ac0_004c3c59;case 0x004c3c60u:goto gb_004c3ac0_004c3c60;case 0x004c3c67u:goto gb_004c3ac0_004c3c67;case 0x004c3c6eu:goto gb_004c3ac0_004c3c6e;case 0x004c3c75u:goto gb_004c3ac0_004c3c75;case 0x004c3c7cu:goto gb_004c3ac0_004c3c7c;case 0x004c3c83u:goto gb_004c3ac0_004c3c83;case 0x004c3c8au:goto gb_004c3ac0_004c3c8a;case 0x004c3c91u:goto gb_004c3ac0_004c3c91;case 0x004c3c98u:goto gb_004c3ac0_004c3c98;case 0x004c3c9fu:goto gb_004c3ac0_004c3c9f;case 0x004c3ca6u:goto gb_004c3ac0_004c3ca6;case 0x004c3cadu:goto gb_004c3ac0_004c3cad;case 0x004c3cb4u:goto gb_004c3ac0_004c3cb4;case 0x004c3cbbu:goto gb_004c3ac0_004c3cbb;case 0x004c3cc2u:goto gb_004c3ac0_004c3cc2;case 0x004c3cc9u:goto gb_004c3ac0_004c3cc9;case 0x004c3cd0u:goto gb_004c3ac0_004c3cd0;case 0x004c3cd7u:goto gb_004c3ac0_004c3cd7;default:abort();}
gb_004c3ac0_004c3c1d:;
    /* 004c3c1d */
    f->r[0]=0x00000010u;
    /* 004c3c22 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c27:;
    /* 004c3c27 */
    f->r[0]=0x00000020u;
    /* 004c3c2c */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c31:;
    /* 004c3c31 */
    f->r[0]=0x00000018u;
    /* 004c3c36 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c3b:;
    /* 004c3c3b */
    f->r[0]=0x00000014u;
    /* 004c3c40 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c45:;
    /* 004c3c45 */
    f->r[0]=0x0000000bu;
    /* 004c3c4a */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c4f:;
    /* 004c3c4f */
    f->r[0]=0x00000019u;
    /* 004c3c54 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c59:;
    /* 004c3c59 */
    f->r[0]=0x0000000cu;
    /* 004c3c5e */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c60:;
    /* 004c3c60 */
    f->r[0]=0x00000009u;
    /* 004c3c65 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c67:;
    /* 004c3c67 */
    f->r[0]=0x0000000au;
    /* 004c3c6c */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c6e:;
    /* 004c3c6e */
    f->r[0]=0x0000000eu;
    /* 004c3c73 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c75:;
    /* 004c3c75 */
    f->r[0]=0x00000012u;
    /* 004c3c7a */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c7c:;
    /* 004c3c7c */
    f->r[0]=0x00000016u;
    /* 004c3c81 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c83:;
    /* 004c3c83 */
    f->r[0]=0x00000017u;
    /* 004c3c88 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c8a:;
    /* 004c3c8a */
    f->r[0]=0x00000015u;
    /* 004c3c8f */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c91:;
    /* 004c3c91 */
    f->r[0]=0x00000011u;
    /* 004c3c96 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c98:;
    /* 004c3c98 */
    f->r[0]=0x0000001du;
    /* 004c3c9d */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3c9f:;
    /* 004c3c9f */
    f->r[0]=0x0000001eu;
    /* 004c3ca4 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3ca6:;
    /* 004c3ca6 */
    f->r[0]=0x0000001cu;
    /* 004c3cab */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3cad:;
    /* 004c3cad */
    f->r[0]=0x00000002u;
    /* 004c3cb2 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3cb4:;
    /* 004c3cb4 */
    f->r[0]=0x00000008u;
    /* 004c3cb9 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3cbb:;
    /* 004c3cbb */
    f->r[0]=0x0000001fu;
    /* 004c3cc0 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3cc2:;
    /* 004c3cc2 */
    f->r[0]=0x0000000du;
    /* 004c3cc7 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3cc9:;
    /* 004c3cc9 */
    f->r[0]=0x00000021u;
    /* 004c3cce */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3cd0:;
    /* 004c3cd0 */
    f->r[0]=0x00000022u;
    /* 004c3cd5 */
    goto gb_004c3ac0_004c3cda;
gb_004c3ac0_004c3cd7:;
    /* 004c3cd7 */
    a=f->r[0];b=0xffffffffu;t=gb_common_alu(f,a,b,32,4);f->r[0]=t;
gb_004c3ac0_004c3cda:;
    /* 004c3cda */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3cdb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3cdc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3cdd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3ce2u;f->boundary(0x004c3cddu,0x0045fe20u,f);
    /* 004c3ce2 */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_004c3ac0_004c3ce5:;
    /* 004c3ce5 */
    a=f->r[6];b=0x00000108u;t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c3ceb */
    a=f->r[7];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004c3cec */
    a=f->r[6];b=0x0089b6e8u;t=gb_common_alu(f,a,b,32,1);
    /* 004c3cf2 */
    if(gb_common_signed_less(f->flags))goto gb_004c3ac0_004c3b7e;
    /* 004c3cf8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3cf9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3cfa */
    f->r[0]=0x00000001u;
    /* 004c3cff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c3d00 */
    a=f->r[4];b=0x00000018u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3d03 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"
#include <stdlib.h>








static __inline void gb_004c3ed0_idiv32(struct restore_god_building_frame *f,uint32_t divisor_bits)
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

void restore_god_C_004c3ed0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c3ed0 */
    a=f->r[4];b=0x00000030u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c3ed3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3ed4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3ed5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3ed6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3ed7 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,5);f->r[7]=t;
    /* 004c3ed9 */
    a=f->r[6];b=f->r[6];t=gb_common_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c3edb */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,5);f->r[3]=t;
    /* 004c3edd */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x000000a1u);
    /* 004c3ee5 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x000000a2u);
    /* 004c3eed */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000006du);
    /* 004c3ef5 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x0000004au);
    /* 004c3efd */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x0000006fu);
    /* 004c3f05 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000071u);
    /* 004c3f0d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000015u);
    /* 004c3f15 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0x00000012u);
    /* 004c3f1d */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x0000000bu);
    /* 004c3f25 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 004c3f2d */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(0x00000011u);
    /* 004c3f35 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000013u);
    /* 004c3f3d */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
gb_004c3ed0_004c3f3f:;
    /* 004c3f3f */
    f->r[1]=MI_U32((0x00000010u+f->r[4]+f->r[0]*1u));
    /* 004c3f43 */
    a=MI_U32((0x00705af0u+f->r[1]*4u));b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 004c3f4a */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c3ed0_004c3f4f;
    /* 004c3f4c */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c3f4d */
    goto gb_004c3ed0_004c3f67;
gb_004c3ed0_004c3f4f:;
    /* 004c3f4f */
    f->r[1]=MI_U32((0x00000028u+f->r[4]+f->r[0]*1u));
    /* 004c3f53 */
    a=f->r[1];b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 004c3f55 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c3ed0_004c3f63;
    /* 004c3f57 */
    a=MI_U32((0x0078d64cu+f->r[1]*4u));b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 004c3f5e */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c3ed0_004c3f63;
    /* 004c3f60 */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c3f61 */
    goto gb_004c3ed0_004c3f67;
gb_004c3ed0_004c3f63:;
    /* 004c3f63 */
    MI_U32((0x00000010u+f->r[4]+f->r[0]*1u))=(uint32_t)(f->r[7]);
gb_004c3ed0_004c3f67:;
    /* 004c3f67 */
    a=f->r[0];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c3f6a */
    a=f->r[0];b=0x00000018u;t=gb_common_alu(f,a,b,32,1);
    /* 004c3f6d */
    if(gb_common_signed_less(f->flags))goto gb_004c3ed0_004c3f3f;
    /* 004c3f6f */
    a=f->r[6];b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 004c3f71 */
    if((f->flags&MI_ZF))goto gb_004c3ed0_004c3fa6;
    /* 004c3f73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3f78u;f->boundary(0x004c3f73u,0x00562348u,f);
    /* 004c3f78 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c3f79 */
    a=f->r[6];gb_004c3ed0_idiv32(f,a);
    /* 004c3f7b */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3f7d */
    f->r[1]=(0x00000010u+f->r[4]);
gb_004c3ed0_004c3f81:;
    /* 004c3f81 */
    a=MI_U32((0x00000000u+f->r[1]));b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 004c3f83 */
    if((f->flags&MI_ZF))goto gb_004c3ed0_004c3f8a;
    /* 004c3f85 */
    a=f->r[3];b=f->r[2];t=gb_common_alu(f,a,b,32,1);
    /* 004c3f87 */
    if((f->flags&MI_ZF))goto gb_004c3ed0_004c3f9d;
    /* 004c3f89 */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
gb_004c3ed0_004c3f8a:;
    /* 004c3f8a */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c3f8b */
    a=f->r[1];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c3f8e */
    a=f->r[0];b=0x00000006u;t=gb_common_alu(f,a,b,32,1);
    /* 004c3f91 */
    if(gb_common_signed_less(f->flags))goto gb_004c3ed0_004c3f81;
    /* 004c3f93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3f94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3f95 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c3f96 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3f98 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c3f99 */
    a=f->r[4];b=0x00000030u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3f9c */
    f->r[4]+=4;return;
gb_004c3ed0_004c3f9d:;
    /* 004c3f9d */
    f->r[5]=MI_U32((0x00000010u+f->r[4]+f->r[0]*4u));
    /* 004c3fa1 */
    a=f->r[5];b=0xffffffffu;t=gb_common_alu(f,a,b,32,1);
    /* 004c3fa4 */
    if(!(f->flags&MI_ZF))goto gb_004c3ed0_004c3fb0;
gb_004c3ed0_004c3fa6:;
    /* 004c3fa6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3fa7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3fa8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c3fa9 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c3fab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c3fac */
    a=f->r[4];b=0x00000030u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3faf */
    f->r[4]+=4;return;
gb_004c3ed0_004c3fb0:;
    /* 004c3fb0 */
    f->r[3]=0x00000001u;
    /* 004c3fb5 */
    f->r[6]=0x00799af0u;
    /* 004c3fba */
    f->r[7]=f->r[3];
gb_004c3ed0_004c3fbc:;
    /* 004c3fbc */
    a=MI_U8((0xfffffff0u+f->r[6]));b=(f->r[3]&0xffu);t=gb_common_alu(f,a,b,8,1);
    /* 004c3fbf */
    if(!(f->flags&MI_ZF))goto gb_004c3ed0_004c3fda;
    /* 004c3fc1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 004c3fc4 */
    a=f->r[2];b=f->r[5];t=gb_common_alu(f,a,b,32,1);
    /* 004c3fc6 */
    if(!(f->flags&MI_ZF))goto gb_004c3ed0_004c3fda;
    /* 004c3fc8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3fca */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3fcb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3fd0u;f->boundary(0x004c3fcbu,0x0046d100u,f);
    /* 004c3fd0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3fd1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3fd2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3fd7u;f->boundary(0x004c3fd2u,0x004695d0u,f);
    /* 004c3fd7 */
    a=f->r[4];b=0x00000010u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_004c3ed0_004c3fda:;
    /* 004c3fda */
    a=f->r[6];b=0x00000108u;t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c3fe0 */
    a=f->r[7];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004c3fe1 */
    a=f->r[6];b=0x0089b6e8u;t=gb_common_alu(f,a,b,32,1);
    /* 004c3fe7 */
    if(gb_common_signed_less(f->flags))goto gb_004c3ed0_004c3fbc;
    /* 004c3fe9 */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3feb */
    MI_U32((0x00e950e0u))=(uint32_t)(0x00000000u);
    /* 004c3ff5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3ffau;f->boundary(0x004c3ff5u,0x0053b090u,f);
    /* 004c3ffa */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3ffd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4002u;f->boundary(0x004c3ffdu,0x00517370u,f);
    /* 004c4002 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4007u;f->boundary(0x004c4002u,0x00517660u,f);
    /* 004c4007 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c4008 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c4009 */
    f->r[0]=f->r[3];
    /* 004c400b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c400c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c400d */
    a=f->r[4];b=0x00000030u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4010 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004c4020(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c4020 */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c4023 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4024 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4025 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4026 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4027 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,5);f->r[7]=t;
    /* 004c4029 */
    f->r[1]=0x00799af0u;
    /* 004c402e */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c4032 */
    f->r[3]=0x00000001u;
    /* 004c4037 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
gb_004c4020_004c403b:;
    /* 004c403b */
    a=MI_U8((0xfffffff0u+f->r[1]));b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c403f */
    if(!(f->flags&MI_ZF))goto gb_004c4020_004c4079;
    /* 004c4041 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 004c4044 */
    a=f->r[5];b=f->r[5];t=gb_common_alu(f,a,b,32,5);f->r[5]=t;
    /* 004c4046 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=gb_common_alu(f,a,b,16,1);
    /* 004c404a */
    if((f->flags&MI_ZF))goto gb_004c4020_004c4052;
    /* 004c404c */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=gb_common_alu(f,a,b,16,1);
    /* 004c4050 */
    if(!(f->flags&MI_ZF))goto gb_004c4020_004c4079;
gb_004c4020_004c4052:;
    /* 004c4052 */
    f->r[6]=0x00000001u;
gb_004c4020_004c4057:;
    /* 004c4057 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4058 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4059 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c405eu;f->boundary(0x004c4059u,0x0045d510u,f);
    /* 004c405e */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4061 */
    a=f->r[5];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 004c4063 */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c4064 */
    a=f->r[6];b=0x00000024u;t=gb_common_alu(f,a,b,32,1);
    /* 004c4067 */
    if(gb_common_signed_less(f->flags))goto gb_004c4020_004c4057;
    /* 004c4069 */
    a=f->r[5];b=MI_U32((0x00000014u+f->r[4]));t=gb_common_alu(f,a,b,32,1);
    /* 004c406d */
    if(gb_common_signed_less(f->flags))goto gb_004c4020_004c4075;
    /* 004c406f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004c4073 */
    f->r[7]=f->r[3];
gb_004c4020_004c4075:;
    /* 004c4075 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
gb_004c4020_004c4079:;
    /* 004c4079 */
    a=f->r[1];b=0x00000108u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c407f */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004c4080 */
    a=f->r[1];b=0x0089b6e8u;t=gb_common_alu(f,a,b,32,1);
    /* 004c4086 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c408a */
    if(gb_common_signed_less(f->flags))goto gb_004c4020_004c403b;
    /* 004c408c */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,3);
    /* 004c408e */
    if((f->flags&MI_ZF))goto gb_004c4020_004c414f;
    /* 004c4094 */
    a=MI_U32((0x0000001cu+f->r[4]));b=0x00000001u;t=gb_common_alu(f,a,b,32,1);
    /* 004c4099 */
    if(!(f->flags&MI_ZF))goto gb_004c4020_004c4129;
    /* 004c409f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40a1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40a2 */
    MI_U32((0x00e950e0u))=(uint32_t)(0x00000000u);
    /* 004c40ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c40b1u;f->boundary(0x004c40acu,0x0046d100u,f);
    /* 004c40b1 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40b3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40b4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c40b9u;f->boundary(0x004c40b4u,0x004695d0u,f);
    /* 004c40b9 */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40bb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c40c0u;f->boundary(0x004c40bbu,0x0053b090u,f);
    /* 004c40c0 */
    f->r[6]=f->r[7];
    /* 004c40c2 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c40c4 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c40c7 */
    a=f->r[6];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c40c9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40cb */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c40ce */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c40d5 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004c40db */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c40e2 */
    f->r[7]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 004c40e5 */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c40e7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40e8 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004c40eb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40ec */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[2]=t;
    /* 004c40ee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40ef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c40f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c40f5u;f->boundary(0x004c40f0u,0x00516f90u,f);
    /* 004c40f5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c40fc */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c4103 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c4105 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004c410b */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 004c410e */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c4110 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4111 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004c4114 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4115 */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[2]=t;
    /* 004c4117 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4118 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4119 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c411eu;f->boundary(0x004c4119u,0x005173a0u,f);
    /* 004c411e */
    a=f->r[4];b=0x00000038u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4121 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c4122 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c4123 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c4124 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c4125 */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4128 */
    f->r[4]+=4;return;
gb_004c4020_004c4129:;
    /* 004c4129 */
    f->r[0]=f->r[7];
    /* 004c412b */
    a=f->r[0];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c412e */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c4130 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[0]*8u)))&0xffffu)<<0);
    /* 004c4138 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=gb_common_alu(f,a,b,16,1);
    /* 004c413c */
    if((f->flags&MI_ZF))goto gb_004c4020_004c4144;
    /* 004c413e */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=gb_common_alu(f,a,b,16,1);
    /* 004c4142 */
    if(!(f->flags&MI_ZF))goto gb_004c4020_004c414f;
gb_004c4020_004c4144:;
    /* 004c4144 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4146 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4147 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c414cu;f->boundary(0x004c4147u,0x0045b700u,f);
    /* 004c414c */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_004c4020_004c414f:;
    /* 004c414f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c4150 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c4151 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c4152 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c4153 */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4156 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004c41b0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c41b0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c41b1 */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
    /* 004c41b3 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,5);f->r[7]=t;
    /* 004c41b5 */
    f->r[0]=0x00bcbad2u;
gb_004c41b0_004c41ba:;
    /* 004c41ba */
    a=MI_U8((0xffffff86u+f->r[0]));b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c41be */
    if(!(f->flags&MI_ZF))goto gb_004c41b0_004c41d4;
    /* 004c41c0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffffc8u+f->r[0])))&0xffu)<<0);
    /* 004c41c3 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=gb_common_alu(f,a,b,8,3);
    /* 004c41c5 */
    if((f->flags&MI_ZF))goto gb_004c41b0_004c41d4;
    /* 004c41c7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 004c41ca */
    a=f->r[1];b=f->r[2];t=gb_common_alu(f,a,b,32,1);
    /* 004c41cc */
    if(gb_common_signed_less(f->flags))goto gb_004c41b0_004c41d4;
    /* 004c41ce */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffff8cu+f->r[0]));
    /* 004c41d2 */
    f->r[2]=f->r[1];
gb_004c41b0_004c41d4:;
    /* 004c41d4 */
    a=f->r[0];b=0x00000090u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c41d9 */
    a=f->r[0];b=0x00bcd662u;t=gb_common_alu(f,a,b,32,1);
    /* 004c41de */
    if(gb_common_signed_less(f->flags))goto gb_004c41b0_004c41ba;
    /* 004c41e0 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,3);
    /* 004c41e2 */
    if((f->flags&MI_ZF))goto gb_004c41b0_004c42a5;
    /* 004c41e8 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004c41eb */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
    /* 004c41ed */
    a=f->r[1];b=0x00000004u;t=gb_common_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c41f0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c41f1 */
    MI_U32((0x00e950e0u))=(uint32_t)(0x00000000u);
    /* 004c41fb */
    a=f->r[1];b=0x00bcb9d0u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
gb_004c41b0_004c4201:;
    /* 004c4201 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 004c4204 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=gb_common_alu(f,a,b,16,3);
    /* 004c4207 */
    if((f->flags&MI_ZF))goto gb_004c41b0_004c4225;
    /* 004c4209 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004c420c */
    a=f->r[1];b=0x00000002u;t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c420f */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004c4212 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c4215 */
    a=f->r[6];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c4217 */
    a=f->r[2];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004c4218 */
    a=f->r[2];b=0x00000010u;t=gb_common_alu(f,a,b,32,1);
    /* 004c421b */
    MI_U8((0x00bcd606u+f->r[6]*4u))=(uint8_t)(0x00000002u);
    /* 004c4223 */
    if(gb_common_signed_less(f->flags))goto gb_004c41b0_004c4201;
gb_004c41b0_004c4225:;
    /* 004c4225 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4227 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4228 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c422du;f->boundary(0x004c4228u,0x0046d100u,f);
    /* 004c422d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c422f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4230 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4235u;f->boundary(0x004c4230u,0x004695d0u,f);
    /* 004c4235 */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4237 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c423cu;f->boundary(0x004c4237u,0x0053b090u,f);
    /* 004c423c */
    f->r[6]=f->r[7];
    /* 004c423e */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c4240 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c4243 */
    a=f->r[6];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c4245 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4247 */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c424a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c4251 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004c4257 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c425e */
    f->r[7]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 004c4261 */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c4263 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4264 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004c4267 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4268 */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[2]=t;
    /* 004c426a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c426b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c426c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4271u;f->boundary(0x004c426cu,0x00516f90u,f);
    /* 004c4271 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c4278 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c427f */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c4281 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004c4287 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 004c428a */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c428c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c428d */
    f->r[6]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004c4290 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4291 */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[2]=t;
    /* 004c4293 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4294 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4295 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c429au;f->boundary(0x004c4295u,0x005173a0u,f);
    /* 004c429a */
    a=f->r[4];b=0x00000038u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c429d */
    f->r[0]=0x00000001u;
    /* 004c42a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c42a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c42a4 */
    f->r[4]+=4;return;
gb_004c41b0_004c42a5:;
    /* 004c42a5 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c42a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c42a8 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"
#include <stdlib.h>








static const uint8_t gb_004c42b0_rom_004c4500[]={0x6c,0x43,0x4c,0x00,0x6c,0x43,0x4c,0x00,0x6c,0x43,0x4c,0x00,0x6c,0x43,0x4c,0x00,0x6c,0x43,0x4c,0x00,0x6c,0x43,0x4c,0x00,0x6c,0x43,0x4c,0x00,0x6c,0x43,0x4c,0x00,0x59,0x43,0x4c,0x00,0x60,0x43,0x4c,0x00,0x1f,0x43,0x4c,0x00,0x52,0x43,0x4c,0x00};
static const uint8_t gb_004c42b0_rom_004c4530[]={0x76,0x43,0x4c,0x00,0xd2,0x43,0x4c,0x00,0x37,0x43,0x4c,0x00,0xd2,0x43,0x4c,0x00,0x91,0x43,0x4c,0x00,0xd2,0x43,0x4c,0x00,0xac,0x43,0x4c,0x00};
static const uint8_t gb_004c42b0_rom_004c454c[]={0x07,0x44,0x4c,0x00,0x12,0x44,0x4c,0x00,0x19,0x44,0x4c,0x00,0x20,0x44,0x4c,0x00,0x27,0x44,0x4c,0x00,0x0b,0x44,0x4c,0x00,0x35,0x44,0x4c,0x00,0x2e,0x44,0x4c,0x00,0x58,0x44,0x4c,0x00,0x58,0x44,0x4c,0x00,0x58,0x44,0x4c,0x00,0x58,0x44,0x4c,0x00,0x51,0x44,0x4c,0x00,0x58,0x44,0x4c,0x00,0x4a,0x44,0x4c,0x00,0x58,0x44,0x4c,0x00,0x3c,0x44,0x4c,0x00,0x58,0x44,0x4c,0x00,0x43,0x44,0x4c,0x00};
static __inline uint32_t gb_004c42b0_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004c4500u&&width<=48u&&(offset=address-0x004c4500u)<=48u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c42b0_rom_004c4500[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c4530u&&width<=28u&&(offset=address-0x004c4530u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c42b0_rom_004c4530[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004c454cu&&width<=76u&&(offset=address-0x004c454cu)<=76u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)gb_004c42b0_rom_004c454c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_god_C_004c42b0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c42b0 */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c42b3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c42b4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c42b5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c42b6 */
    a=f->r[6];b=f->r[6];t=gb_common_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c42b8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c42b9 */
    f->r[7]=gb_004c42b0_read((0x00f53e6cu),4u);
    /* 004c42bf */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c42c3 */
    f->r[5]=0x007999e8u;
gb_004c42b0_004c42c8:;
    /* 004c42c8 */
    a=gb_004c42b0_read((0xfffffff0u+f->r[5]),1u);b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c42cc */
    if(!(f->flags&MI_ZF))goto gb_004c42b0_004c44da;
    /* 004c42d2 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((gb_004c42b0_read((0x00000000u+f->r[5]),2u))&0xffffu)<<0);
    /* 004c42d6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 004c42d9 */
    a=f->r[0];b=0x0000000au;t=gb_common_alu(f,a,b,32,1);
    /* 004c42dc */
    if(gb_common_signed_less(f->flags))goto gb_004c42b0_004c42e3;
    /* 004c42de */
    a=f->r[0];b=0x0000001du;t=gb_common_alu(f,a,b,32,1);
    /* 004c42e1 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c42b0_004c42ed;
gb_004c42b0_004c42e3:;
    /* 004c42e3 */
    a=(f->r[1]&0xffffu);b=0x00000046u;t=gb_common_alu(f,a,b,16,1);
    /* 004c42e7 */
    if(!(f->flags&MI_ZF))goto gb_004c42b0_004c44da;
gb_004c42b0_004c42ed:;
    /* 004c42ed */
    a=f->r[0];b=0x0000000au;t=gb_common_alu(f,a,b,32,1);
    /* 004c42f0 */
    if(gb_common_signed_less(f->flags))goto gb_004c42b0_004c43e1;
    /* 004c42f6 */
    a=f->r[0];b=0x0000001du;t=gb_common_alu(f,a,b,32,1);
    /* 004c42f9 */
    if(!(f->flags&MI_ZF)&&!gb_common_signed_less(f->flags))goto gb_004c42b0_004c43e1;
    /* 004c42ff */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
gb_004c42b0_004c4301:;
    /* 004c4301 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[2]*1u);
    /* 004c4304 */
    a=gb_004c42b0_read((0x00799a2cu+f->r[1]*2u),2u);b=0x00000000u;t=gb_common_alu(f,a,b,16,1);
    /* 004c430d */
    if((f->flags&MI_ZF))goto gb_004c42b0_004c43d2;
    /* 004c4313 */
    a=f->r[2];b=0x0000000bu;t=gb_common_alu(f,a,b,32,1);
    /* 004c4316 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_004c42b0_004c4367;
    /* 004c4318 */
    switch(gb_004c42b0_read((0x004c4500u+f->r[2]*4u),4u)){case 0x004c431fu:goto gb_004c42b0_004c431f;case 0x004c4352u:goto gb_004c42b0_004c4352;case 0x004c4359u:goto gb_004c42b0_004c4359;case 0x004c4360u:goto gb_004c42b0_004c4360;case 0x004c436cu:goto gb_004c42b0_004c436c;default:abort();}
gb_004c42b0_004c431f:;
    /* 004c431f */
    f->r[0]=0x00000011u;
gb_004c42b0_004c4324:;
    /* 004c4324 */
    a=f->r[0];b=0xfffffff3u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c4327 */
    a=f->r[0];b=0x00000006u;t=gb_common_alu(f,a,b,32,1);
    /* 004c432a */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_004c42b0_004c43d2;
    /* 004c4330 */
    switch(gb_004c42b0_read((0x004c4530u+f->r[0]*4u),4u)){case 0x004c4337u:goto gb_004c42b0_004c4337;case 0x004c4376u:goto gb_004c42b0_004c4376;case 0x004c4391u:goto gb_004c42b0_004c4391;case 0x004c43acu:goto gb_004c42b0_004c43ac;case 0x004c43d2u:goto gb_004c42b0_004c43d2;default:abort();}
gb_004c42b0_004c4337:;
    /* 004c4337 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)gb_004c42b0_read((0x00000002u+f->r[5]),2u);
    /* 004c433b */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c433e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 004c4341 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((gb_004c42b0_read((0xfffffff3u+f->r[5]),1u))&0xffu)<<0);
    /* 004c4344 */
    a=(f->r[3]&0xffu);b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c4347 */
    f->r[0]=gb_004c42b0_read((0x0070ce04u+f->r[0]*8u),4u);
    /* 004c434e */
    if((f->flags&(MI_CF|MI_ZF)))goto gb_004c42b0_004c43c8;
    /* 004c4350 */
    goto gb_004c42b0_004c43c5;
gb_004c42b0_004c4352:;
    /* 004c4352 */
    f->r[0]=0x00000013u;
    /* 004c4357 */
    goto gb_004c42b0_004c4324;
gb_004c42b0_004c4359:;
    /* 004c4359 */
    f->r[0]=0x0000000fu;
    /* 004c435e */
    goto gb_004c42b0_004c4324;
gb_004c42b0_004c4360:;
    /* 004c4360 */
    f->r[0]=0x0000000du;
    /* 004c4365 */
    goto gb_004c42b0_004c4324;
gb_004c42b0_004c4367:;
    /* 004c4367 */
    a=f->r[0];b=0xffffffffu;t=gb_common_alu(f,a,b,32,4);f->r[0]=t;
    /* 004c436a */
    goto gb_004c42b0_004c4324;
gb_004c42b0_004c436c:;
    /* 004c436c */
    f->r[0]=(f->r[0]&0xffff0000u)|(((gb_004c42b0_read((0x0000000cu+f->r[5]),2u))&0xffffu)<<0);
    /* 004c4370 */
    a=(f->r[0]&0xffffu);b=0x00000006u;{int32_t product=(int32_t)(int16_t)a*(int32_t)(int16_t)b;t=(uint16_t)product;f->flags&=~(MI_CF|MI_OF);if(product!=(int32_t)(int16_t)t)f->flags|=MI_CF|MI_OF;}f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004c4374 */
    goto gb_004c42b0_004c43ca;
gb_004c42b0_004c4376:;
    /* 004c4376 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)gb_004c42b0_read((0x00000002u+f->r[5]),2u);
    /* 004c437a */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c437d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 004c4380 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((gb_004c42b0_read((0xfffffff3u+f->r[5]),1u))&0xffu)<<0);
    /* 004c4383 */
    a=(f->r[3]&0xffu);b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c4386 */
    f->r[0]=gb_004c42b0_read((0x0070cdf8u+f->r[0]*8u),4u);
    /* 004c438d */
    if((f->flags&(MI_CF|MI_ZF)))goto gb_004c42b0_004c43c8;
    /* 004c438f */
    goto gb_004c42b0_004c43c5;
gb_004c42b0_004c4391:;
    /* 004c4391 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)gb_004c42b0_read((0x00000002u+f->r[5]),2u);
    /* 004c4395 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c4398 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 004c439b */
    f->r[3]=(f->r[3]&0xffffff00u)|(((gb_004c42b0_read((0xfffffff3u+f->r[5]),1u))&0xffu)<<0);
    /* 004c439e */
    a=(f->r[3]&0xffu);b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c43a1 */
    f->r[0]=gb_004c42b0_read((0x0070cdfcu+f->r[0]*8u),4u);
    /* 004c43a8 */
    if((f->flags&(MI_CF|MI_ZF)))goto gb_004c42b0_004c43c8;
    /* 004c43aa */
    goto gb_004c42b0_004c43c5;
gb_004c42b0_004c43ac:;
    /* 004c43ac */
    f->r[0]=(uint32_t)(int32_t)(int16_t)gb_004c42b0_read((0x00000002u+f->r[5]),2u);
    /* 004c43b0 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004c43b3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 004c43b6 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((gb_004c42b0_read((0xfffffff3u+f->r[5]),1u))&0xffu)<<0);
    /* 004c43b9 */
    a=(f->r[3]&0xffu);b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c43bc */
    f->r[0]=gb_004c42b0_read((0x0070ce00u+f->r[0]*8u),4u);
    /* 004c43c3 */
    if((f->flags&(MI_CF|MI_ZF)))goto gb_004c42b0_004c43c8;
gb_004c42b0_004c43c5:;
    /* 004c43c5 */
    a=f->r[0];b=0x00000002u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
gb_004c42b0_004c43c8:;
    /* 004c43c8 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
gb_004c42b0_004c43ca:;
    /* 004c43ca */
    MI_U16((0x00799a2cu+f->r[1]*2u))=(uint16_t)((f->r[0]&0xffffu));
gb_004c42b0_004c43d2:;
    /* 004c43d2 */
    a=f->r[2];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004c43d3 */
    a=f->r[2];b=0x0000000cu;t=gb_common_alu(f,a,b,32,1);
    /* 004c43d6 */
    if(gb_common_signed_less(f->flags))goto gb_004c42b0_004c4301;
    /* 004c43dc */
    goto gb_004c42b0_004c44da;
gb_004c42b0_004c43e1:;
    /* 004c43e1 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,3);
    /* 004c43e3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004c43eb */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c42b0_004c44da;
    /* 004c43f1 */
    f->r[3]=0x00f53e70u;
gb_004c42b0_004c43f6:;
    /* 004c43f6 */
    f->r[7]=gb_004c42b0_read((0x00000000u+f->r[3]),4u);
    /* 004c43f8 */
    f->r[0]=(0xffffffffu+f->r[7]);
    /* 004c43fb */
    a=f->r[0];b=0x00000012u;t=gb_common_alu(f,a,b,32,1);
    /* 004c43fe */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_004c42b0_004c4458;
    /* 004c4400 */
    switch(gb_004c42b0_read((0x004c454cu+f->r[0]*4u),4u)){case 0x004c4407u:goto gb_004c42b0_004c4407;case 0x004c440bu:goto gb_004c42b0_004c440b;case 0x004c4412u:goto gb_004c42b0_004c4412;case 0x004c4419u:goto gb_004c42b0_004c4419;case 0x004c4420u:goto gb_004c42b0_004c4420;case 0x004c4427u:goto gb_004c42b0_004c4427;case 0x004c442eu:goto gb_004c42b0_004c442e;case 0x004c4435u:goto gb_004c42b0_004c4435;case 0x004c443cu:goto gb_004c42b0_004c443c;case 0x004c4443u:goto gb_004c42b0_004c4443;case 0x004c444au:goto gb_004c42b0_004c444a;case 0x004c4451u:goto gb_004c42b0_004c4451;case 0x004c4458u:goto gb_004c42b0_004c4458;default:abort();}
gb_004c42b0_004c4407:;
    /* 004c4407 */
    a=f->r[6];b=f->r[6];t=gb_common_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c4409 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c440b:;
    /* 004c440b */
    f->r[6]=0x00000005u;
    /* 004c4410 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4412:;
    /* 004c4412 */
    f->r[6]=0x00000001u;
    /* 004c4417 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4419:;
    /* 004c4419 */
    f->r[6]=0x00000002u;
    /* 004c441e */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4420:;
    /* 004c4420 */
    f->r[6]=0x00000003u;
    /* 004c4425 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4427:;
    /* 004c4427 */
    f->r[6]=0x00000004u;
    /* 004c442c */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c442e:;
    /* 004c442e */
    f->r[6]=0x00000007u;
    /* 004c4433 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4435:;
    /* 004c4435 */
    f->r[6]=0x00000006u;
    /* 004c443a */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c443c:;
    /* 004c443c */
    f->r[6]=0x0000000au;
    /* 004c4441 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4443:;
    /* 004c4443 */
    f->r[6]=0x0000000bu;
    /* 004c4448 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c444a:;
    /* 004c444a */
    f->r[6]=0x00000008u;
    /* 004c444f */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4451:;
    /* 004c4451 */
    f->r[6]=0x00000009u;
    /* 004c4456 */
    goto gb_004c42b0_004c445b;
gb_004c42b0_004c4458:;
    /* 004c4458 */
    a=f->r[6];b=0xffffffffu;t=gb_common_alu(f,a,b,32,4);f->r[6]=t;
gb_004c42b0_004c445b:;
    /* 004c445b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c445c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4461u;f->boundary(0x004c445cu,0x005013a0u,f);
    /* 004c4461 */
    a=f->r[4];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4464 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 004c4466 */
    if((f->flags&MI_ZF))goto gb_004c42b0_004c44bc;
    /* 004c4468 */
    f->r[1]=gb_004c42b0_read((0x00000010u+f->r[4]),4u);
    /* 004c446c */
    a=f->r[1];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c446e */
    f->r[0]=(f->r[0]&0xffff0000u)|(((gb_004c42b0_read((0x00799a2eu+f->r[1]*2u),2u))&0xffffu)<<0);
    /* 004c4476 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=gb_common_alu(f,a,b,16,3);
    /* 004c4479 */
    if((f->flags&MI_ZF))goto gb_004c42b0_004c44bc;
    /* 004c447b */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,3);
    /* 004c447d */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c42b0_004c4497;
    /* 004c447f */
    a=f->r[7];b=0x00000008u;t=gb_common_alu(f,a,b,32,1);
    /* 004c4482 */
    if(!(f->flags&MI_ZF)&&!gb_common_signed_less(f->flags))goto gb_004c42b0_004c4497;
    /* 004c4484 */
    a=f->r[7];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004c4485 */
    a=f->r[7];b=f->flags&MI_CF;t=gb_common_alu(f,0,a,32,1);f->r[7]=t;
    /* 004c4487 */
    a=f->r[7];b=f->r[7];t=gb_common_alu(f,a,b,32,2);f->r[7]=t;
    /* 004c4489 */
    a=f->r[7];b=0xffffff38u;t=gb_common_alu(f,a,b,32,3);f->r[7]=t;
    /* 004c448f */
    a=f->r[7];b=0x00000320u;t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c4495 */
    goto gb_004c42b0_004c449c;
gb_004c42b0_004c4497:;
    /* 004c4497 */
    f->r[7]=0x00000190u;
gb_004c42b0_004c449c:;
    /* 004c449c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004c449f */
    a=f->r[0];b=0x00000001u;t=gb_common_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c44a1 */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 004c44a3 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c42b0_004c44a7;
    /* 004c44a5 */
    f->r[7]=f->r[0];
gb_004c42b0_004c44a7:;
    /* 004c44a7 */
    a=f->r[7];b=0x00004e20u;t=gb_common_alu(f,a,b,32,1);
    /* 004c44ad */
    if(gb_common_signed_less(f->flags))goto gb_004c42b0_004c44b4;
    /* 004c44af */
    f->r[7]=0x00004e20u;
gb_004c42b0_004c44b4:;
    /* 004c44b4 */
    MI_U16((0x00799a2eu+f->r[1]*2u))=(uint16_t)((f->r[7]&0xffffu));
gb_004c42b0_004c44bc:;
    /* 004c44bc */
    f->r[0]=gb_004c42b0_read((0x00000014u+f->r[4]),4u);
    /* 004c44c0 */
    f->r[7]=gb_004c42b0_read((0x00f53e6cu),4u);
    /* 004c44c6 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c44c7 */
    a=f->r[3];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c44ca */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 004c44cc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c44d0 */
    if(gb_common_signed_less(f->flags))goto gb_004c42b0_004c43f6;
    /* 004c44d6 */
    f->r[6]=gb_004c42b0_read((0x00000010u+f->r[4]),4u);
gb_004c42b0_004c44da:;
    /* 004c44da */
    a=f->r[5];b=0x00000108u;t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 004c44e0 */
    a=f->r[6];b=0x00000084u;t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c44e6 */
    a=f->r[5];b=0x0089b6e8u;t=gb_common_alu(f,a,b,32,1);
    /* 004c44ec */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c44f0 */
    if(gb_common_signed_less(f->flags))goto gb_004c42b0_004c42c8;
    /* 004c44f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c44f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c44f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c44f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c44fa */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c44fd */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004c45a0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c45a0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004c45a4 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 004c45a8 */
    f->r[0]=MI_U32((0x00000000u+f->r[0]));
    /* 004c45aa */
    f->r[1]=f->r[0];
    /* 004c45ac */
    a=f->r[1];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c45af */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c45b1 */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 004c45b3 */
    f->r[2]=f->r[0];
    /* 004c45b5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]*8u));
    /* 004c45bd */
    a=f->r[2];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c45c0 */
    a=f->r[2];b=f->r[0];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c45c2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[2]*8u));
    /* 004c45ca */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,1);
    /* 004c45cc */
    if(!gb_common_signed_less(f->flags))goto gb_004c45a0_004c45d2;
    /* 004c45ce */
    a=f->r[0];b=0xffffffffu;t=gb_common_alu(f,a,b,32,4);f->r[0]=t;
    /* 004c45d1 */
    f->r[4]+=4;return;
gb_004c45a0_004c45d2:;
    /* 004c45d2 */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
    /* 004c45d4 */
    a=f->r[1];b=f->r[0];t=gb_common_alu(f,a,b,32,1);
    /* 004c45d6 */
    t=(!(f->flags&MI_ZF)&&!gb_common_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c45d9 */
    f->r[0]=f->r[2];
    /* 004c45db */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004c45e0(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c45e0 */
    a=f->r[4];b=0x00000058u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c45e3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c45e4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c45e5 */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,5);f->r[3]=t;
    /* 004c45e7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c45e8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c45e9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004c45ed */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000013u);
    /* 004c45f5 */
    f->r[1]=0x00000001u;
    /* 004c45fa */
    f->r[6]=(0x00000018u+f->r[4]);
    /* 004c45fe */
    f->r[0]=0x00799ae0u;
gb_004c45e0_004c4603:;
    /* 004c4603 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000010u+f->r[0]));
    /* 004c4607 */
    a=f->r[2];b=0x0000000au;t=gb_common_alu(f,a,b,32,1);
    /* 004c460a */
    if(gb_common_signed_less(f->flags))goto gb_004c45e0_004c4621;
    /* 004c460c */
    a=f->r[2];b=0x0000001du;t=gb_common_alu(f,a,b,32,1);
    /* 004c460f */
    if(!(f->flags&MI_ZF)&&!gb_common_signed_less(f->flags))goto gb_004c45e0_004c4621;
    /* 004c4611 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c4614 */
    if(!(f->flags&MI_ZF))goto gb_004c45e0_004c4621;
    /* 004c4616 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004c4618 */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004c4619 */
    a=f->r[6];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c461c */
    a=f->r[3];b=0x00000014u;t=gb_common_alu(f,a,b,32,1);
    /* 004c461f */
    if(!gb_common_signed_less(f->flags))goto gb_004c45e0_004c463d;
gb_004c45e0_004c4621:;
    /* 004c4621 */
    a=f->r[0];b=0x00000108u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c4626 */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004c4627 */
    a=f->r[0];b=0x0089b6d8u;t=gb_common_alu(f,a,b,32,1);
    /* 004c462c */
    if(gb_common_signed_less(f->flags))goto gb_004c45e0_004c4603;
    /* 004c462e */
    a=f->r[3];b=0x00000014u;t=gb_common_alu(f,a,b,32,1);
    /* 004c4631 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004c4635 */
    if(gb_common_signed_less(f->flags))goto gb_004c45e0_004c46f5;
    /* 004c463b */
    goto gb_004c45e0_004c4641;
gb_004c45e0_004c463d:;
    /* 004c463d */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
gb_004c45e0_004c4641:;
    /* 004c4641 */
    a=0x004c45a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4646 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4648 */
    f->r[0]=(0x00000020u+f->r[4]);
    /* 004c464c */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c464e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c464f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4654u;f->boundary(0x004c464fu,0x00562069u,f);
    /* 004c4654 */
    f->r[1]=MI_U32((0x00000074u+f->r[4]));
    /* 004c4658 */
    a=f->r[4];b=0x00000010u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c465b */
    f->r[5]=0x00000015u;
    /* 004c4660 */
    f->r[7]=0x0079af92u;
gb_004c45e0_004c4665:;
    /* 004c4665 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[7]));
    /* 004c4669 */
    a=f->r[0];b=0x0000000au;t=gb_common_alu(f,a,b,32,1);
    /* 004c466c */
    if(gb_common_signed_less(f->flags))goto gb_004c45e0_004c46e2;
    /* 004c466e */
    a=f->r[0];b=0x0000001du;t=gb_common_alu(f,a,b,32,1);
    /* 004c4671 */
    if(!(f->flags&MI_ZF)&&!gb_common_signed_less(f->flags))goto gb_004c45e0_004c46e2;
    /* 004c4673 */
    a=MI_U8((0xffffffeeu+f->r[7]));b=0x00000001u;t=gb_common_alu(f,a,b,8,1);
    /* 004c4677 */
    if(!(f->flags&MI_ZF))goto gb_004c45e0_004c46e2;
    /* 004c4679 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[7])))&0xffffu)<<0);
    /* 004c467c */
    f->r[2]=f->r[1];
    /* 004c467e */
    a=f->r[2];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c4681 */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c4683 */
    a=(f->r[6]&0xffffu);b=MI_U16((0x007999eau+f->r[2]*8u));t=gb_common_alu(f,a,b,16,1);
    /* 004c468b */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c45e0_004c46e2;
    /* 004c468d */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c468f */
    f->r[2]=(0x00000018u+f->r[4]);
gb_004c45e0_004c4693:;
    /* 004c4693 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 004c4695 */
    f->r[3]=f->r[1];
    /* 004c4697 */
    a=f->r[3];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[3]=t;
    /* 004c469a */
    a=f->r[3];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c469c */
    a=MI_U16((0x007999eau+f->r[3]*8u));b=(f->r[6]&0xffffu);t=gb_common_alu(f,a,b,16,1);
    /* 004c46a4 */
    if(gb_common_signed_less(f->flags))goto gb_004c45e0_004c46b5;
    /* 004c46a6 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c46a7 */
    a=f->r[2];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c46aa */
    a=f->r[0];b=0x00000014u;t=gb_common_alu(f,a,b,32,1);
    /* 004c46ad */
    if(gb_common_signed_less(f->flags))goto gb_004c45e0_004c4693;
    /* 004c46af */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004c46b3 */
    goto gb_004c45e0_004c46bb;
gb_004c45e0_004c46b5:;
    /* 004c46b5 */
    f->r[2]=f->r[0];
    /* 004c46b7 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
gb_004c45e0_004c46bb:;
    /* 004c46bb */
    a=f->r[2];b=0x00000013u;t=gb_common_alu(f,a,b,32,1);
    /* 004c46be */
    if(!gb_common_signed_less(f->flags))goto gb_004c45e0_004c46d6;
    /* 004c46c0 */
    f->r[1]=0x00000013u;
    /* 004c46c5 */
    f->r[0]=(0x00000064u+f->r[4]);
    /* 004c46c9 */
    a=f->r[1];b=f->r[2];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
gb_004c45e0_004c46cb:;
    /* 004c46cb */
    f->r[6]=MI_U32((0xfffffffcu+f->r[0]));
    /* 004c46ce */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[6]);
    /* 004c46d0 */
    a=f->r[0];b=0xfffffffcu;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c46d3 */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004c46d4 */
    if(!(f->flags&MI_ZF))goto gb_004c45e0_004c46cb;
gb_004c45e0_004c46d6:;
    /* 004c46d6 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 004c46da */
    MI_U32((0x00000018u+f->r[4]+f->r[2]*4u))=(uint32_t)(f->r[5]);
    /* 004c46de */
    f->r[1]=MI_U32((0x00000064u+f->r[4]));
gb_004c45e0_004c46e2:;
    /* 004c46e2 */
    a=f->r[7];b=0x00000108u;t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c46e8 */
    a=f->r[5];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004c46e9 */
    a=f->r[7];b=0x0089b6eau;t=gb_common_alu(f,a,b,32,1);
    /* 004c46ef */
    if(gb_common_signed_less(f->flags))goto gb_004c45e0_004c4665;
gb_004c45e0_004c46f5:;
    /* 004c46f5 */
    a=f->r[3];b=f->r[3];t=gb_common_alu(f,a,b,32,3);
    /* 004c46f7 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004c45e0_004c4790;
    /* 004c46fd */
    f->r[5]=(0x00000018u+f->r[4]);
gb_004c45e0_004c4701:;
    /* 004c4701 */
    f->r[7]=MI_U32((0x00000000u+f->r[5]));
    /* 004c4704 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4706 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4707 */
    MI_U32((0x00e950e0u))=(uint32_t)(0x00000000u);
    /* 004c4711 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4716u;f->boundary(0x004c4711u,0x0046d100u,f);
    /* 004c4716 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4718 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4719 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c471eu;f->boundary(0x004c4719u,0x004695d0u,f);
    /* 004c471e */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4720 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4725u;f->boundary(0x004c4720u,0x0053b090u,f);
    /* 004c4725 */
    f->r[6]=f->r[7];
    /* 004c4727 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c4729 */
    a=f->r[6];b=0x00000005u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c472c */
    a=f->r[6];b=f->r[7];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c472e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4730 */
    a=f->r[6];b=0x00000003u;t=gb_common_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c4733 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c473a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004c4740 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c4747 */
    f->r[7]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 004c474a */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c474c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c474d */
    f->r[7]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004c4750 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4751 */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[2]=t;
    /* 004c4753 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4754 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4755 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c475au;f->boundary(0x004c4755u,0x00516f90u,f);
    /* 004c475a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c4761 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c4768 */
    a=f->r[1];b=f->r[1];t=gb_common_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c476a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 004c4770 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 004c4773 */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c4775 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c4776 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004c4779 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c477a */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[2]=t;
    /* 004c477c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c477d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c477e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c4783u;f->boundary(0x004c477eu,0x005173a0u,f);
    /* 004c4783 */
    a=f->r[4];b=0x00000038u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4786 */
    a=f->r[5];b=0x00000004u;t=gb_common_alu(f,a,b,32,0);f->r[5]=t;
    /* 004c4789 */
    a=f->r[3];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004c478a */
    if(!(f->flags&MI_ZF))goto gb_004c45e0_004c4701;
gb_004c45e0_004c4790:;
    /* 004c4790 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c4791 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c4792 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c4793 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c4794 */
    a=f->r[4];b=0x00000058u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c4797 */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_004e9170(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e9170 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004e9174 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e9175 */
    f->r[6]=f->r[0];
    /* 004e9177 */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 004e917b */
    a=f->r[6];b=0x000049d8u;t=gb_common_imul(f,a,b);f->r[6]=t;
    /* 004e9181 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e9182 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e9183 */
    f->r[2]=MI_U32((0x00ea3f54u+f->r[6]));
    /* 004e9189 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e918a */
    a=f->r[2];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e918c */
    MI_U32((0x00ea3f54u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004e9192 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e9197u;f->boundary(0x004e9192u,0x004e9350u,f);
    /* 004e9197 */
    a=f->r[4];b=0x00000008u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e919a */
    MI_U32((0x00ea1894u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 004e91a4 */
    f->r[0]=(0x00ea3e34u+f->r[6]);
    /* 004e91aa */
    f->r[1]=0x00000064u;
gb_004e9170_004e91af:;
    /* 004e91af */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 004e91b2 */
    f->r[7]=MI_U32((0x00ea1894u+f->r[6]));
    /* 004e91b8 */
    a=f->r[0];b=0x00000002u;t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e91bb */
    a=f->r[7];b=f->r[2];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
    /* 004e91bd */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004e91be */
    MI_U32((0x00ea1894u+f->r[6]))=(uint32_t)(f->r[7]);
    /* 004e91c4 */
    if(!(f->flags&MI_ZF))goto gb_004e9170_004e91af;
    /* 004e91c6 */
    f->r[1]=MI_U32((0x00ea5f0cu+f->r[6]));
    /* 004e91cc */
    f->r[0]=f->r[7];
    /* 004e91ce */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);
    /* 004e91d0 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_004e9170_004e91d8;
    /* 004e91d2 */
    MI_U32((0x00ea5f0cu+f->r[6]))=(uint32_t)(f->r[0]);
gb_004e9170_004e91d8:;
    /* 004e91d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e91d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e91da */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








static __inline void gb_00562069_udiv32(struct restore_god_building_frame *f,uint32_t divisor)
{
 uint64_t dividend=((uint64_t)f->r[2]<<32)|f->r[0],quotient;
 if(!divisor)abort();quotient=dividend/divisor;if(quotient>UINT32_MAX)abort();
 f->r[0]=(uint32_t)quotient;f->r[2]=(uint32_t)(dividend%divisor);
}
void restore_god_C_00562069(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00562069 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0056206a */
    f->r[5]=f->r[4];
    /* 0056206c */
    a=f->r[4];b=0x000000f8u;t=gb_common_alu(f,a,b,32,1);f->r[4]=t;
    /* 00562072 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562073 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562074 */
    f->r[6]=MI_U32((0x0000000cu+f->r[5]));
    /* 00562077 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562078 */
    a=f->r[6];b=0x00000002u;t=gb_common_alu(f,a,b,32,1);
    /* 0056207b */
    if((f->flags&MI_CF))goto gb_00562069_005621b8;
    /* 00562081 */
    a=MI_U32((0x00000010u+f->r[5]));b=0x00000000u;t=gb_common_alu(f,a,b,32,1);
    /* 00562085 */
    if((f->flags&MI_ZF))goto gb_00562069_005621b8;
    /* 0056208b */
    a=MI_U32((0xfffffffcu+f->r[5]));b=0x00000000u;t=gb_common_alu(f,a,b,32,3);MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(t);
    /* 0056208f */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00562090 */
    a=f->r[6];b=MI_U32((0x00000010u+f->r[5]));t=gb_common_imul(f,a,b);f->r[6]=t;
    /* 00562094 */
    f->r[3]=MI_U32((0x00000008u+f->r[5]));
    /* 00562097 */
    f->r[0]=(0xffffff80u+f->r[5]);
    /* 0056209a */
    MI_U32((0x00000008u+f->r[5]))=(uint32_t)(f->r[0]);
    /* 0056209d */
    f->r[0]=(0xffffff08u+f->r[5]);
    /* 005620a3 */
    a=f->r[6];b=f->r[3];t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 005620a5 */
    MI_U32((0x0000000cu+f->r[5]))=(uint32_t)(f->r[0]);
gb_00562069_005620a8:;
    /* 005620a8 */
    f->r[7]=MI_U32((0x00000010u+f->r[5]));
    /* 005620ab */
    f->r[0]=f->r[6];
    /* 005620ad */
    a=f->r[0];b=f->r[3];t=gb_common_alu(f,a,b,32,1);f->r[0]=t;
    /* 005620af */
    a=f->r[2];b=f->r[2];t=gb_common_alu(f,a,b,32,5);f->r[2]=t;
    /* 005620b1 */
    a=f->r[7];gb_00562069_udiv32(f,a);
    /* 005620b3 */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005620b4 */
    a=f->r[0];b=0x00000008u;t=gb_common_alu(f,a,b,32,1);
    /* 005620b7 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_00562069_005620e8;
    /* 005620b9 */
    a=MI_U32((0x00000014u+f->r[5]));f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005620bc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005620bd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005620be */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005620bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x005620c4u;f->boundary(0x005620bfu,0x005621bdu,f);
    /* 005620c4 */
    a=f->r[4];b=0x00000010u;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
gb_00562069_005620c7:;
    /* 005620c7 */
    a=MI_U32((0xfffffffcu+f->r[5]));b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(t);
    /* 005620ca */
    a=MI_U32((0x0000000cu+f->r[5]));b=0x00000004u;t=gb_common_alu(f,a,b,32,1);MI_U32((0x0000000cu+f->r[5]))=(uint32_t)(t);
    /* 005620ce */
    a=MI_U32((0x00000008u+f->r[5]));b=0x00000004u;t=gb_common_alu(f,a,b,32,1);MI_U32((0x00000008u+f->r[5]))=(uint32_t)(t);
    /* 005620d2 */
    a=MI_U32((0xfffffffcu+f->r[5]));b=0x00000000u;t=gb_common_alu(f,a,b,32,1);
    /* 005620d6 */
    if(gb_common_signed_less(f->flags))goto gb_00562069_005621b8;
    /* 005620dc */
    f->r[0]=MI_U32((0x0000000cu+f->r[5]));
    /* 005620df */
    f->r[3]=MI_U32((0x00000000u+f->r[0]));
    /* 005620e1 */
    f->r[0]=MI_U32((0x00000008u+f->r[5]));
    /* 005620e4 */
    f->r[6]=MI_U32((0x00000000u+f->r[0]));
    /* 005620e6 */
    goto gb_00562069_005620a8;
gb_00562069_005620e8:;
    /* 005620e8 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|gb_common_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 005620ea */
    a=f->r[0];b=f->r[7];t=gb_common_imul(f,a,b);f->r[0]=t;
    /* 005620ed */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005620ee */
    a=f->r[0];b=f->r[3];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 005620f0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005620f1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005620f2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005620f7u;f->boundary(0x005620f2u,0x0056220bu,f);
    /* 005620f7 */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 005620fa */
    MI_U32((0xfffffff8u+f->r[5]))=(uint32_t)(f->r[3]);
    /* 005620fd */
    a=f->r[7];b=f->r[6];t=gb_common_alu(f,a,b,32,0);f->r[7]=t;
gb_00562069_005620ff:;
    /* 005620ff */
    f->r[0]=MI_U32((0xfffffff8u+f->r[5]));
    /* 00562102 */
    a=f->r[0];b=MI_U32((0x00000010u+f->r[5]));t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 00562105 */
    a=f->r[0];b=f->r[6];t=gb_common_alu(f,a,b,32,1);
    /* 00562107 */
    MI_U32((0xfffffff8u+f->r[5]))=(uint32_t)(f->r[0]);
    /* 0056210a */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_00562069_00562117;
    /* 0056210c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0056210d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0056210e */
    p=MI_U32((0x00000014u+f->r[5]));f->r[4]-=4;MI_U32(f->r[4])=0x00562111u;f->boundary(0x0056210eu,p,f);
    /* 00562111 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00562112 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 00562114 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00562115 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_00562069_005620ff;
gb_00562069_00562117:;
    /* 00562117 */
    a=f->r[7];b=MI_U32((0x00000010u+f->r[5]));t=gb_common_alu(f,a,b,32,1);f->r[7]=t;
    /* 0056211a */
    a=f->r[7];b=f->r[3];t=gb_common_alu(f,a,b,32,1);
    /* 0056211c */
    if((f->flags&(MI_CF|MI_ZF)))goto gb_00562069_00562129;
    /* 0056211e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0056211f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562120 */
    p=MI_U32((0x00000014u+f->r[5]));f->r[4]-=4;MI_U32(f->r[4])=0x00562123u;f->boundary(0x00562120u,p,f);
    /* 00562123 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00562124 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 00562126 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00562127 */
    if(!gb_common_signed_less(f->flags))goto gb_00562069_00562117;
gb_00562069_00562129:;
    /* 00562129 */
    a=MI_U32((0x00000010u+f->r[5]));f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0056212c */
    a=f->r[7];b=MI_U32((0xfffffff8u+f->r[5]));t=gb_common_alu(f,a,b,32,1);
    /* 0056212f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562130 */
    if((f->flags&MI_CF))goto gb_00562069_0056213f;
    /* 00562132 */
    a=MI_U32((0xfffffff8u+f->r[5]));f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562135 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0056213au;f->boundary(0x00562135u,0x0056220bu,f);
    /* 0056213a */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 0056213d */
    goto gb_00562069_005620ff;
gb_00562069_0056213f:;
    /* 0056213f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562140 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00562145u;f->boundary(0x00562140u,0x0056220bu,f);
    /* 00562145 */
    f->r[0]=MI_U32((0xfffffff8u+f->r[5]));
    /* 00562148 */
    f->r[1]=f->r[7];
    /* 0056214a */
    a=f->r[1];b=f->r[3];t=gb_common_alu(f,a,b,32,1);f->r[1]=t;
    /* 0056214c */
    f->r[2]=f->r[6];
    /* 0056214e */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 00562151 */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00562152 */
    a=f->r[2];b=f->r[0];t=gb_common_alu(f,a,b,32,1);f->r[2]=t;
    /* 00562154 */
    a=f->r[1];b=f->r[2];t=gb_common_alu(f,a,b,32,1);
    /* 00562156 */
    if(gb_common_signed_less(f->flags))goto gb_00562069_00562188;
    /* 00562158 */
    f->r[1]=MI_U32((0x00000010u+f->r[5]));
    /* 0056215b */
    f->r[2]=(0x00000000u+f->r[3]+f->r[1]*1u);
    /* 0056215e */
    a=f->r[2];b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 00562160 */
    if(!(f->flags&MI_CF))goto gb_00562069_00562179;
    /* 00562162 */
    f->r[2]=MI_U32((0x0000000cu+f->r[5]));
    /* 00562165 */
    a=MI_U32((0x0000000cu+f->r[5]));b=0x00000004u;t=gb_common_alu(f,a,b,32,0);MI_U32((0x0000000cu+f->r[5]))=(uint32_t)(t);
    /* 00562169 */
    a=f->r[7];b=f->r[1];t=gb_common_alu(f,a,b,32,1);f->r[7]=t;
    /* 0056216b */
    f->r[1]=MI_U32((0x00000008u+f->r[5]));
    /* 0056216e */
    a=MI_U32((0xfffffffcu+f->r[5]));b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(t);
    /* 00562171 */
    a=MI_U32((0x00000008u+f->r[5]));b=0x00000004u;t=gb_common_alu(f,a,b,32,0);MI_U32((0x00000008u+f->r[5]))=(uint32_t)(t);
    /* 00562175 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[3]);
    /* 00562177 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[7]);
gb_00562069_00562179:;
    /* 00562179 */
    a=f->r[0];b=f->r[6];t=gb_common_alu(f,a,b,32,1);
    /* 0056217b */
    if(!(f->flags&MI_CF))goto gb_00562069_005620c7;
    /* 00562181 */
    f->r[3]=f->r[0];
    /* 00562183 */
    goto gb_00562069_005620a8;
gb_00562069_00562188:;
    /* 00562188 */
    a=f->r[0];b=f->r[6];t=gb_common_alu(f,a,b,32,1);
    /* 0056218a */
    if(!(f->flags&MI_CF))goto gb_00562069_005621a1;
    /* 0056218c */
    f->r[1]=MI_U32((0x0000000cu+f->r[5]));
    /* 0056218f */
    a=MI_U32((0xfffffffcu+f->r[5]));b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(t);
    /* 00562192 */
    a=MI_U32((0x0000000cu+f->r[5]));b=0x00000004u;t=gb_common_alu(f,a,b,32,0);MI_U32((0x0000000cu+f->r[5]))=(uint32_t)(t);
    /* 00562196 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00562198 */
    f->r[0]=MI_U32((0x00000008u+f->r[5]));
    /* 0056219b */
    a=MI_U32((0x00000008u+f->r[5]));b=0x00000004u;t=gb_common_alu(f,a,b,32,0);MI_U32((0x00000008u+f->r[5]))=(uint32_t)(t);
    /* 0056219f */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[6]);
gb_00562069_005621a1:;
    /* 005621a1 */
    f->r[0]=MI_U32((0x00000010u+f->r[5]));
    /* 005621a4 */
    a=f->r[0];b=f->r[3];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 005621a6 */
    a=f->r[0];b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 005621a8 */
    if(!(f->flags&MI_CF))goto gb_00562069_005620c7;
    /* 005621ae */
    a=f->r[7];b=MI_U32((0x00000010u+f->r[5]));t=gb_common_alu(f,a,b,32,1);f->r[7]=t;
    /* 005621b1 */
    f->r[6]=f->r[7];
    /* 005621b3 */
    goto gb_00562069_005620a8;
gb_00562069_005621b8:;
    /* 005621b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005621b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005621ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005621bb */
    f->r[4]=f->r[5];f->r[5]=MI_U32(f->r[4]);f->r[4]+=4;
    /* 005621bc */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_005621bd(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005621bd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621be */
    f->r[5]=f->r[4];
    /* 005621c0 */
    f->r[0]=MI_U32((0x00000008u+f->r[5]));
    /* 005621c3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621c4 */
    f->r[7]=MI_U32((0x0000000cu+f->r[5]));
    /* 005621c7 */
    a=f->r[7];b=f->r[0];t=gb_common_alu(f,a,b,32,1);
    /* 005621c9 */
    if((f->flags&(MI_CF|MI_ZF)))goto gb_005621bd_00562208;
    /* 005621cb */
    f->r[1]=MI_U32((0x00000010u+f->r[5]));
    /* 005621ce */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621cf */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,0);f->r[0]=t;
    /* 005621d1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621d2 */
    MI_U32((0x0000000cu+f->r[5]))=(uint32_t)(f->r[0]);
gb_005621bd_005621d5:;
    /* 005621d5 */
    f->r[3]=MI_U32((0x00000008u+f->r[5]));
    /* 005621d8 */
    f->r[6]=MI_U32((0x0000000cu+f->r[5]));
gb_005621bd_005621db:;
    /* 005621db */
    a=f->r[6];b=f->r[7];t=gb_common_alu(f,a,b,32,1);
    /* 005621dd */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_005621bd_005621f1;
    /* 005621df */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621e0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621e1 */
    p=MI_U32((0x00000014u+f->r[5]));f->r[4]-=4;MI_U32(f->r[4])=0x005621e4u;f->boundary(0x005621e1u,p,f);
    /* 005621e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 005621e5 */
    a=f->r[0];b=f->r[0];t=gb_common_alu(f,a,b,32,3);
    /* 005621e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 005621e8 */
    if((f->flags&MI_ZF)||gb_common_signed_less(f->flags))goto gb_005621bd_005621ec;
    /* 005621ea */
    f->r[3]=f->r[6];
gb_005621bd_005621ec:;
    /* 005621ec */
    a=f->r[6];b=MI_U32((0x00000010u+f->r[5]));t=gb_common_alu(f,a,b,32,0);f->r[6]=t;
    /* 005621ef */
    goto gb_005621bd_005621db;
gb_005621bd_005621f1:;
    /* 005621f1 */
    a=MI_U32((0x00000010u+f->r[5]));f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621f4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621f5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005621f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005621fbu;f->boundary(0x005621f6u,0x0056220bu,f);
    /* 005621fb */
    a=f->r[7];b=MI_U32((0x00000010u+f->r[5]));t=gb_common_alu(f,a,b,32,1);f->r[7]=t;
    /* 005621fe */
    a=f->r[4];b=0x0000000cu;t=gb_common_alu(f,a,b,32,0);f->r[4]=t;
    /* 00562201 */
    a=f->r[7];b=MI_U32((0x00000008u+f->r[5]));t=gb_common_alu(f,a,b,32,1);
    /* 00562204 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto gb_005621bd_005621d5;
    /* 00562206 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00562207 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
gb_005621bd_00562208:;
    /* 00562208 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00562209 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0056220a */
    f->r[4]+=4;return;
}

/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "god_building_effects.h"








void restore_god_C_0056220b(struct restore_god_building_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0056220b */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0056220f */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00562213 */
    a=f->r[0];b=f->r[1];t=gb_common_alu(f,a,b,32,1);
    /* 00562215 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562216 */
    if((f->flags&MI_ZF))goto gb_0056220b_00562235;
    /* 00562218 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0056221c */
    f->r[6]=f->r[2];
    /* 0056221e */
    a=f->r[2];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0056221f */
    a=f->r[6];b=f->r[6];t=gb_common_alu(f,a,b,32,3);
    /* 00562221 */
    if((f->flags&MI_ZF))goto gb_0056220b_00562235;
    /* 00562223 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00562224 */
    f->r[6]=(0x00000001u+f->r[2]);
gb_0056220b_00562227:;
    /* 00562227 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 00562229 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 0056222b */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0056222d */
    a=f->r[0];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0056222e */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 00562230 */
    a=f->r[1];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00562231 */
    a=f->r[6];b=f->flags&MI_CF;t=gb_common_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00562232 */
    if(!(f->flags&MI_ZF))goto gb_0056220b_00562227;
    /* 00562234 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
gb_0056220b_00562235:;
    /* 00562235 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00562236 */
    f->r[4]+=4;return;
}
