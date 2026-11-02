/* Exact bounded instruction-to-C bodies; integer helpers shared once. */
#include "pyramid_summary.h"
#include <stdio.h>
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
static __inline uint32_t mi_alu(struct restore_pyramid_summary_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_pyramid_summary_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_pyramid_summary_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}
static const uint8_t mi_rom_004eeb44[]={0xce,0xea,0x4e,0x00,0x1d,0xeb,0x4e,0x00,0x80,0xea,0x4e,0x00,0xc7,0xea,0x4e,0x00,0xc7,0xea,0x4e,0x00,0xc7,0xea,0x4e,0x00,0xe4,0xea,0x4e,0x00};
static const uint8_t mi_rom_004f1c4c[]={0xbf,0x13,0x4f,0x00,0xb6,0x13,0x4f,0x00,0xad,0x13,0x4f,0x00,0xb6,0x13,0x4f,0x00,0xc8,0x13,0x4f,0x00};
static const uint8_t mi_rom_004f1c60[]={0x10,0x14,0x4f,0x00,0xdf,0x13,0x4f,0x00,0x21,0x14,0x4f,0x00,0xdf,0x13,0x4f,0x00,0x21,0x14,0x4f,0x00};
static const uint8_t mi_rom_004f1c74[]={0x36,0x15,0x4f,0x00,0x52,0x15,0x4f,0x00,0x67,0x15,0x4f,0x00,0x86,0x15,0x4f,0x00,0xa5,0x15,0x4f,0x00,0xc4,0x15,0x4f,0x00,0xe3,0x15,0x4f,0x00};
static const uint8_t mi_rom_004f1c90[]={0x43,0x1b,0x4f,0x00,0xd0,0x1a,0x4f,0x00,0x92,0x1a,0x4f,0x00,0x6d,0x1b,0x4f,0x00,0x6d,0x1b,0x4f,0x00,0x6d,0x1b,0x4f,0x00,0x59,0x1b,0x4f,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004eeb44u&&width<=28u&&(offset=address-0x004eeb44u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004eeb44[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004f1c4cu&&width<=20u&&(offset=address-0x004f1c4cu)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004f1c4c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004f1c60u&&width<=20u&&(offset=address-0x004f1c60u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004f1c60[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004f1c74u&&width<=28u&&(offset=address-0x004f1c74u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004f1c74[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004f1c90u&&width<=28u&&(offset=address-0x004f1c90u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004f1c90[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

static __inline void mi_idiv32(struct restore_pyramid_summary_frame *f,uint32_t divisor_bits)
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

void restore_pyramid_C_004f1310(struct restore_pyramid_summary_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f1310 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004f1313 */
    f->r[1]=0x0000001cu;
    /* 004f1318 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f131a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f131b */
    f->r[6]=mi_read((0x00000058u+f->r[4]),4u);
    /* 004f131f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1320 */
    f->r[7]=f->r[6];
    /* 004f1322 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 004f1324 */
    f->r[1]=mi_read((0x00000058u+f->r[4]),4u);
    /* 004f1328 */
    f->r[0]=f->r[1];
    /* 004f132a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f132d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f132f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f1332 */
    a=mi_read((0x007999e8u+f->r[0]),2u);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004f133b */
    if(!(f->flags&MI_ZF))goto mi_004f1c44;
    /* 004f1341 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[0]),2u);
    /* 004f1348 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004f134a */
    f->r[0]=mi_read((0x005e4ef4u),4u);
    /* 004f134f */
    f->r[7]=f->r[1];
    /* 004f1351 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004f1355 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004f1358 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f135a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f135c */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004f135f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1360 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1361 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a2cu+f->r[7]),1u))&0xffu)<<0);
    /* 004f1367 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f1369 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004f136d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f138b;
    /* 004f136f */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004f1372 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f1382;
    /* 004f1374 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f1377 */
    if(!(f->flags&MI_ZF))goto mi_004f138b;
    /* 004f1379 */
    f->r[5]=mi_read((0x005f5c9cu+f->r[1]*4u),4u);
    /* 004f1380 */
    goto mi_004f138d;
mi_004f1382:;
    /* 004f1382 */
    f->r[5]=mi_read((0x005f5c88u+f->r[1]*4u),4u);
    /* 004f1389 */
    goto mi_004f138d;
mi_004f138b:;
    /* 004f138b */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_004f138d:;
    /* 004f138d */
    MI_U32((0x00000044u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004f1390 */
    f->r[2]=mi_read((0x005e4ef4u),4u);
    /* 004f1396 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f1398 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a2cu+f->r[7]),1u))&0xffu)<<0);
    /* 004f139e */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 004f13a1 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004f13a4 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004f13d1;
    /* 004f13a6 */
    switch(mi_read((0x004f1c4cu+f->r[1]*4u),4u)){case 0x004f13adu:goto mi_004f13ad;case 0x004f13b6u:goto mi_004f13b6;case 0x004f13bfu:goto mi_004f13bf;case 0x004f13c8u:goto mi_004f13c8;default:abort();}
mi_004f13ad:;
    /* 004f13ad */
    f->r[3]=mi_read((0x005f5c54u+f->r[0]*4u),4u);
    /* 004f13b4 */
    goto mi_004f13d3;
mi_004f13b6:;
    /* 004f13b6 */
    f->r[3]=mi_read((0x005f5c40u+f->r[0]*4u),4u);
    /* 004f13bd */
    goto mi_004f13d3;
mi_004f13bf:;
    /* 004f13bf */
    f->r[3]=mi_read((0x005f5c68u+f->r[0]*4u),4u);
    /* 004f13c6 */
    goto mi_004f13d3;
mi_004f13c8:;
    /* 004f13c8 */
    f->r[3]=mi_read((0x005f5c7cu+f->r[0]*4u),4u);
    /* 004f13cf */
    goto mi_004f13d3;
mi_004f13d1:;
    /* 004f13d1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_004f13d3:;
    /* 004f13d3 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004f13d6 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004f1421;
    /* 004f13d8 */
    switch(mi_read((0x004f1c60u+f->r[1]*4u),4u)){case 0x004f13dfu:goto mi_004f13df;case 0x004f1410u:goto mi_004f1410;case 0x004f1421u:goto mi_004f1421;default:abort();}
mi_004f13df:;
    /* 004f13df */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f13e2 */
    if(!(f->flags&MI_ZF))goto mi_004f13fa;
    /* 004f13e4 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f13e8 */
    f->r[0]=mi_read((0x0056f7ecu+f->r[1]*4u),4u);
    /* 004f13ef */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f13f2 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f13f4 */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f13f8 */
    goto mi_004f1429;
mi_004f13fa:;
    /* 004f13fa */
    f->r[2]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f13fe */
    f->r[0]=mi_read((0x0056f7d8u+f->r[2]*4u),4u);
    /* 004f1405 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f1408 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f140a */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f140e */
    goto mi_004f1429;
mi_004f1410:;
    /* 004f1410 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f1414 */
    f->r[1]=mi_read((0x0056f82cu+f->r[0]*4u),4u);
    /* 004f141b */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004f141f */
    goto mi_004f1429;
mi_004f1421:;
    /* 004f1421 */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(0x00000000u);
mi_004f1429:;
    /* 004f1429 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a2du+f->r[7]),1u))&0xffu)<<0);
    /* 004f142f */
    f->r[1]=0x00000001u;
    /* 004f1434 */
    a=(f->r[0]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004f1436 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004f171a;
    /* 004f143c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004f143e */
    f->r[0]=0x00000002u;
    /* 004f1443 */
    f->r[2]=0x00000003u;
    /* 004f1448 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f144a */
    MI_U32((0x0000003cu+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004f144d */
    MI_U32((0x00000038u+f->r[6]))=(uint32_t)(0x000003e7u);
    /* 004f1454 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004f1457 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004f145b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004f145f */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004f1463 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f1467 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004f146b */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f146f */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f1473 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004f1477 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004f147b */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f147f */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f1483 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004f1487 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f148b */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004f148f */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004f1493 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 004f149b */
    f->r[7]=(0x00799a33u+f->r[7]);
    /* 004f14a1 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000055u)&0xffu)<<0);
mi_004f14a3:;
    /* 004f14a3 */
    a=mi_read((0x005e4ef4u),4u);b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f14aa */
    if(!(f->flags&MI_ZF))goto mi_004f14b9;
    /* 004f14ac */
    f->r[2]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f14b0 */
    f->r[0]=mi_read((0x0056f7ecu+f->r[2]*4u),4u);
    /* 004f14b7 */
    goto mi_004f14c4;
mi_004f14b9:;
    /* 004f14b9 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f14bd */
    f->r[0]=mi_read((0x0056f7d8u+f->r[0]*4u),4u);
mi_004f14c4:;
    /* 004f14c4 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004f14c6 */
    if(!mi_signed_less(f->flags))goto mi_004f1602;
    /* 004f14cc */
    f->r[1]=mi_read((0x0000003cu+f->r[6]),4u);
    /* 004f14cf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f14d1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0xfffffffau+f->r[7]),1u))&0xffu)<<0);
    /* 004f14d4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004f14d6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004f14da;
    /* 004f14d8 */
    f->r[0]=f->r[1];
mi_004f14da:;
    /* 004f14da */
    f->r[1]=mi_read((0x00000038u+f->r[6]),4u);
    /* 004f14dd */
    MI_U32((0x0000003cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f14e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f14e2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0xfffffffau+f->r[7]),1u))&0xffu)<<0);
    /* 004f14e5 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004f14e7 */
    if(mi_signed_less(f->flags))goto mi_004f14eb;
    /* 004f14e9 */
    f->r[0]=f->r[1];
mi_004f14eb:;
    /* 004f14eb */
    MI_U32((0x00000038u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f14ee */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000009u+f->r[7]),2u);
    /* 004f14f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f14f4 */
    if((f->flags&MI_ZF))goto mi_004f151e;
    /* 004f14f6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f14f9 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f14fc */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f14fe */
    a=mi_read((0x00bcd602u+f->r[1]*4u),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004f1505 */
    if(!(f->flags&MI_ZF))goto mi_004f151e;
    /* 004f1507 */
    f->r[2]=mi_read((0x00000060u+f->r[4]),4u);
    /* 004f150b */
    f->r[0]=(0x00000000u+f->r[2]+f->r[5]*1u);
    /* 004f150e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f150f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1514u;f->boundary(0x004f150fu,0x004eea10u,f);
    /* 004f1514 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1517 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1519 */
    if((f->flags&MI_ZF))goto mi_004f151e;
    /* 004f151b */
    a=mi_read((0x00000064u+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000064u+f->r[6]))=(uint32_t)(t);
mi_004f151e:;
    /* 004f151e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f1520 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0xfffffffau+f->r[7]),1u))&0xffu)<<0);
    /* 004f1523 */
    a=f->r[0];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1526 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004f1529 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004f15f6;
    /* 004f152f */
    switch(mi_read((0x004f1c74u+f->r[0]*4u),4u)){case 0x004f1536u:goto mi_004f1536;case 0x004f1552u:goto mi_004f1552;case 0x004f1567u:goto mi_004f1567;case 0x004f1586u:goto mi_004f1586;case 0x004f15a5u:goto mi_004f15a5;case 0x004f15c4u:goto mi_004f15c4;case 0x004f15e3u:goto mi_004f15e3;default:abort();}
mi_004f1536:;
    /* 004f1536 */
    f->r[0]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f1539 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f153b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000000u+f->r[7]),1u))&0xffu)<<0);
    /* 004f153d */
    f->r[2]=mi_read((0x0000001cu+f->r[4]+f->r[1]*4u),4u);
    /* 004f1541 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1543 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f1544 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1547 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f154d */
    goto mi_004f14a3;
mi_004f1552:;
    /* 004f1552 */
    f->r[0]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f1555 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1558 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f1559 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f155c */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f1562 */
    goto mi_004f14a3;
mi_004f1567:;
    /* 004f1567 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f1569 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[7]),1u))&0xffu)<<0);
    /* 004f156b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]+f->r[0]*4u),4u);
    /* 004f156f */
    f->r[0]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f1572 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f1575 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1577 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f1578 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f157b */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f1581 */
    goto mi_004f14a3;
mi_004f1586:;
    /* 004f1586 */
    f->r[1]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f1589 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f158b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000000u+f->r[7]),1u))&0xffu)<<0);
    /* 004f158d */
    f->r[0]=mi_read((0x0000001cu+f->r[4]+f->r[2]*4u),4u);
    /* 004f1591 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1594 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f1596 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f1597 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f159a */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f15a0 */
    goto mi_004f14a3;
mi_004f15a5:;
    /* 004f15a5 */
    f->r[0]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f15a8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f15aa */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000000u+f->r[7]),1u))&0xffu)<<0);
    /* 004f15ac */
    f->r[2]=mi_read((0x0000001cu+f->r[4]+f->r[1]*4u),4u);
    /* 004f15b0 */
    a=f->r[2];b=0x0000000du;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f15b3 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f15b5 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f15b6 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f15b9 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f15bf */
    goto mi_004f14a3;
mi_004f15c4:;
    /* 004f15c4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f15c6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[7]),1u))&0xffu)<<0);
    /* 004f15c8 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]+f->r[0]*4u),4u);
    /* 004f15cc */
    f->r[0]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f15cf */
    a=f->r[1];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f15d2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f15d4 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f15d5 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f15d8 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f15de */
    goto mi_004f14a3;
mi_004f15e3:;
    /* 004f15e3 */
    f->r[1]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f15e6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f15e8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000000u+f->r[7]),1u))&0xffu)<<0);
    /* 004f15ea */
    f->r[0]=mi_read((0x0000001cu+f->r[4]+f->r[2]*4u),4u);
    /* 004f15ee */
    a=f->r[0];b=0x00000015u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f15f1 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f15f3 */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[1]);
mi_004f15f6:;
    /* 004f15f6 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004f15f7 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f15fd */
    goto mi_004f14a3;
mi_004f1602:;
    /* 004f1602 */
    a=mi_read((0x0000003cu+f->r[6]),4u);b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004f1606 */
    if(!(f->flags&MI_ZF))goto mi_004f160b;
    /* 004f1608 */
    a=mi_read((0x00000040u+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000040u+f->r[6]))=(uint32_t)(t);
mi_004f160b:;
    /* 004f160b */
    f->r[0]=mi_read((0x00000040u+f->r[6]),4u);
    /* 004f160e */
    f->r[7]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f1612 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f1615 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*8u);
    /* 004f1618 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f161b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f161c */
    a=mi_read((0x00000044u+f->r[6]),4u);mi_idiv32(f,a);
    /* 004f161f */
    MI_U32((0x00000030u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1622 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004f1625 */
    f->r[0]=0x51eb851fu;
    /* 004f162a */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004f162c */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004f162f */
    f->r[0]=f->r[2];
    /* 004f1631 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004f1634 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f1636 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004f1638 */
    MI_U32((0x00000034u+f->r[6]))=(uint32_t)(f->r[2]);
mi_004f163b:;
    /* 004f163b */
    f->r[1]=mi_read((0x005e4ef4u),4u);
    /* 004f1641 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f1644 */
    if(!(f->flags&MI_ZF))goto mi_004f164f;
    /* 004f1646 */
    f->r[0]=mi_read((0x0056f7ecu+f->r[7]*4u),4u);
    /* 004f164d */
    goto mi_004f1656;
mi_004f164f:;
    /* 004f164f */
    f->r[0]=mi_read((0x0056f7d8u+f->r[7]*4u),4u);
mi_004f1656:;
    /* 004f1656 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004f1658 */
    if(!mi_signed_less(f->flags))goto mi_004f1bc2;
    /* 004f165e */
    f->r[2]=mi_read((0x00000060u+f->r[4]),4u);
    /* 004f1662 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f1665 */
    f->r[5]=(0x00000000u+f->r[3]+f->r[2]*1u);
    /* 004f1668 */
    if(!(f->flags&MI_ZF))goto mi_004f1685;
    /* 004f166a */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004f166d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004f166f */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 004f1672 */
    f->r[0]=mi_read((0x005e657cu+f->r[1]*8u),4u);
    /* 004f1679 */
    if(!(f->flags&MI_ZF))goto mi_004f16e8;
    /* 004f167b */
    f->r[0]=mi_read((0x00000028u+f->r[6]),4u);
    /* 004f167e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f167f */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004f1680 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1683 */
    goto mi_004f163b;
mi_004f1685:;
    /* 004f1685 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004f1688 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f168b */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f168d */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004f1690 */
    f->r[0]=mi_read((0x005e4efcu+f->r[2]*8u),4u);
    /* 004f1697 */
    if(!(f->flags&MI_ZF))goto mi_004f16e8;
    /* 004f1699 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f169b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f169d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f16bb;
    /* 004f169f */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
mi_004f16a2:;
    /* 004f16a2 */
    a=mi_read((0x005e68d8u+f->r[1]+f->r[2]*2u),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004f16aa */
    if((f->flags&MI_ZF))goto mi_004f16b2;
    /* 004f16ac */
    a=mi_read((0x00000028u+f->r[6]),4u);b=0x00000003u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000028u+f->r[6]))=(uint32_t)(t);
    /* 004f16b0 */
    goto mi_004f16b6;
mi_004f16b2:;
    /* 004f16b2 */
    a=mi_read((0x00000028u+f->r[6]),4u);b=0x00000006u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000028u+f->r[6]))=(uint32_t)(t);
mi_004f16b6:;
    /* 004f16b6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004f16b7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004f16b9 */
    if(mi_signed_less(f->flags))goto mi_004f16a2;
mi_004f16bb:;
    /* 004f16bb */
    f->r[0]=f->r[5];
    /* 004f16bd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f16bf */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f16c2 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f16c4 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004f16c7 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a30u+f->r[0]*8u),1u))&0xffu)<<0);
    /* 004f16ce */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x005e68d8u+f->r[1]+f->r[2]*2u),1u))&0xffu)<<0);
    /* 004f16d5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f16d7 */
    f->r[0]=mi_read((0x0000002cu+f->r[6]),4u);
    /* 004f16da */
    if((f->flags&MI_ZF))goto mi_004f1702;
    /* 004f16dc */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f16df */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004f16e0 */
    MI_U32((0x0000002cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f16e3 */
    goto mi_004f163b;
mi_004f16e8:;
    /* 004f16e8 */
    f->r[2]=mi_read((0x00000028u+f->r[6]),4u);
    /* 004f16eb */
    f->r[1]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f16ef */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f16f2 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f16f4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f16f6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f16f8 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004f16fb */
    f->r[0]=f->r[2];
    /* 004f16fd */
    if((f->flags&MI_ZF))goto mi_004f170e;
    /* 004f16ff */
    f->r[0]=mi_read((0x0000002cu+f->r[6]),4u);
mi_004f1702:;
    /* 004f1702 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1705 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004f1706 */
    MI_U32((0x0000002cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1709 */
    goto mi_004f163b;
mi_004f170e:;
    /* 004f170e */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1711 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004f1712 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1715 */
    goto mi_004f163b;
mi_004f171a:;
    /* 004f171a */
    f->r[1]=mi_read((0x00000060u+f->r[4]),4u);
    /* 004f171e */
    MI_U32((0x00000040u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004f1721 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1722 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1727u;f->boundary(0x004f1722u,0x004ef450u,f);
    /* 004f1727 */
    f->r[0]=mi_read((0x00eaaf58u),4u);
    /* 004f172c */
    f->r[1]=mi_read((0x00eaac44u),4u);
    /* 004f1732 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1735 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f1737 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004f173d;
    /* 004f1739 */
    f->r[1]=mi_read((0x00000060u+f->r[4]),4u);
mi_004f173d:;
    /* 004f173d */
    f->r[0]=f->r[1];
    /* 004f173f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f1742 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1744 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f1746 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f1749 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a2fu+f->r[0]),1u))&0xffu)<<0);
    /* 004f174f */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004f1752 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f1754 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a2eu+f->r[0]),1u))&0xffu)<<0);
    /* 004f175a */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 004f175d */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f1760 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004f1761 */
    MI_U32((0x00000008u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f1764 */
    f->r[1]=mi_read((0x005e4ef4u),4u);
    /* 004f176a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004f176b */
    if((f->flags&MI_ZF))goto mi_004f178b;
    /* 004f176d */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f1771 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004f1774 */
    if((f->flags&MI_ZF))goto mi_004f177f;
    /* 004f1776 */
    f->r[2]=mi_read((0x0056f7f8u+f->r[5]*4u),4u);
    /* 004f177d */
    goto mi_004f1796;
mi_004f177f:;
    /* 004f177f */
    f->r[1]=mi_read((0x0056f820u+f->r[5]*4u),4u);
    /* 004f1786 */
    MI_U32((0x0000000cu+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f1789 */
    goto mi_004f1799;
mi_004f178b:;
    /* 004f178b */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f178f */
    f->r[2]=mi_read((0x0056f80cu+f->r[5]*4u),4u);
mi_004f1796:;
    /* 004f1796 */
    MI_U32((0x0000000cu+f->r[6]))=(uint32_t)(f->r[2]);
mi_004f1799:;
    /* 004f1799 */
    a=mi_read((0x00eaaf58u),4u);b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004f17a0 */
    if(!mi_signed_less(f->flags))goto mi_004f17b5;
    /* 004f17a2 */
    f->r[0]=0x00000064u;
    /* 004f17a7 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(0x00000004u);
    /* 004f17ad */
    MI_U32((0x00000030u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f17b0 */
    MI_U32((0x00000034u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f17b3 */
    goto mi_004f181d;
mi_004f17b5:;
    /* 004f17b5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799aa0u+f->r[7]),2u);
    /* 004f17bc */
    f->r[5]=f->r[3];
    /* 004f17be */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004f17c0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a2du+f->r[0]),1u))&0xffu)<<0);
    /* 004f17c6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f17c8 */
    if(!(f->flags&MI_ZF))goto mi_004f17e3;
    /* 004f17ca */
    f->r[0]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f17ce */
    f->r[1]=f->r[3];
    /* 004f17d0 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(0x00000002u);
    /* 004f17d6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004f17d8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799aa0u+f->r[7]),2u);
    /* 004f17df */
    f->r[0]=f->r[3];
    /* 004f17e1 */
    goto mi_004f17f6;
mi_004f17e3:;
    /* 004f17e3 */
    f->r[0]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f17e7 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(0x00000003u);
    /* 004f17ed */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799aa0u+f->r[7]),2u);
    /* 004f17f4 */
    f->r[1]=f->r[0];
mi_004f17f6:;
    /* 004f17f6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f17f8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f17fb */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*8u);
    /* 004f17fe */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f1801 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1802 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004f1804 */
    MI_U32((0x00000030u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1807 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004f180b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f180e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1810 */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f1814 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1815 */
    a=f->r[3];mi_idiv32(f,a);
    /* 004f1817 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f181a */
    MI_U32((0x00000034u+f->r[6]))=(uint32_t)(f->r[0]);
mi_004f181d:;
    /* 004f181d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f181f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004f1823:;
    /* 004f1823 */
    f->r[2]=mi_read((0x005e4ef4u),4u);
    /* 004f1829 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f182c */
    if(!(f->flags&MI_ZF))goto mi_004f1837;
    /* 004f182e */
    f->r[0]=mi_read((0x0056f7ecu+f->r[5]*4u),4u);
    /* 004f1835 */
    goto mi_004f183e;
mi_004f1837:;
    /* 004f1837 */
    f->r[0]=mi_read((0x0056f7d8u+f->r[5]*4u),4u);
mi_004f183e:;
    /* 004f183e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004f1840 */
    if(!mi_signed_less(f->flags))goto mi_004f1b81;
    /* 004f1846 */
    f->r[0]=mi_read((0x00000060u+f->r[4]),4u);
    /* 004f184a */
    f->r[7]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 004f184e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1850 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f1853 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f1857 */
    if(!(f->flags&MI_ZF))goto mi_004f1865;
    /* 004f1859 */
    f->r[7]=(0x00000000u+f->r[1]+f->r[7]*4u);
    /* 004f185c */
    f->r[7]=mi_read((0x005e657cu+f->r[7]*8u),4u);
    /* 004f1863 */
    goto mi_004f1871;
mi_004f1865:;
    /* 004f1865 */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004f1868 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f186a */
    f->r[7]=mi_read((0x005e4efcu+f->r[7]*8u),4u);
mi_004f1871:;
    /* 004f1871 */
    f->r[3]=f->r[0];
    /* 004f1873 */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004f1876 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004f1878 */
    a=f->r[3];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004f187b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a2du+f->r[3]),1u))&0xffu)<<0);
    /* 004f1881 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1883 */
    if(!(f->flags&MI_ZF))goto mi_004f1937;
    /* 004f1889 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f188b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a2fu+f->r[3]),1u))&0xffu)<<0);
    /* 004f1891 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004f1893 */
    f->r[1]=f->r[0];
    /* 004f1895 */
    if(mi_signed_less(f->flags))goto mi_004f1899;
    /* 004f1897 */
    f->r[1]=f->r[7];
mi_004f1899:;
    /* 004f1899 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f189b */
    if(!mi_signed_less(f->flags))goto mi_004f18a1;
    /* 004f189d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f189f */
    goto mi_004f18a9;
mi_004f18a1:;
    /* 004f18a1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004f18a3 */
    f->r[1]=f->r[0];
    /* 004f18a5 */
    if(mi_signed_less(f->flags))goto mi_004f18a9;
    /* 004f18a7 */
    f->r[1]=f->r[7];
mi_004f18a9:;
    /* 004f18a9 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004f18ac */
    if(!(f->flags&MI_ZF))goto mi_004f18d3;
    /* 004f18ae */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f18b0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f18b2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f18ed;
    /* 004f18b4 */
    f->r[2]=(0x00000000u+f->r[5]+f->r[5]*2u);
mi_004f18b8:;
    /* 004f18b8 */
    a=mi_read((0x005e68d8u+f->r[0]+f->r[2]*2u),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004f18c0 */
    if((f->flags&MI_ZF))goto mi_004f18c8;
    /* 004f18c2 */
    a=mi_read((0x00000010u+f->r[6]),4u);b=0x00000003u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000010u+f->r[6]))=(uint32_t)(t);
    /* 004f18c6 */
    goto mi_004f18cc;
mi_004f18c8:;
    /* 004f18c8 */
    a=mi_read((0x00000010u+f->r[6]),4u);b=0x00000006u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000010u+f->r[6]))=(uint32_t)(t);
mi_004f18cc:;
    /* 004f18cc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f18cd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004f18cf */
    if(mi_signed_less(f->flags))goto mi_004f18b8;
    /* 004f18d1 */
    goto mi_004f18ed;
mi_004f18d3:;
    /* 004f18d3 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f18d6 */
    if(!(f->flags&MI_ZF))goto mi_004f18e0;
    /* 004f18d8 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004f18dc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f18de */
    if((f->flags&MI_ZF))goto mi_004f18ed;
mi_004f18e0:;
    /* 004f18e0 */
    f->r[0]=mi_read((0x00000010u+f->r[6]),4u);
    /* 004f18e3 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004f18e6 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f18e8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f18ea */
    MI_U32((0x00000010u+f->r[6]))=(uint32_t)(f->r[0]);
mi_004f18ed:;
    /* 004f18ed */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a2fu+f->r[3]),1u))&0xffu)<<0);
    /* 004f18f3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a30u+f->r[3]),1u))&0xffu)<<0);
    /* 004f18f9 */
    a=(f->r[2]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004f18fb */
    if((f->flags&MI_CF))goto mi_004f191e;
    /* 004f18fd */
    f->r[0]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f1901 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f1903 */
    if((f->flags&MI_ZF))goto mi_004f191e;
    /* 004f1905 */
    f->r[1]=mi_read((0x00000014u+f->r[6]),4u);
    /* 004f1908 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f190a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a2eu+f->r[3]),1u))&0xffu)<<0);
    /* 004f1910 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f1912 */
    MI_U32((0x00000014u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f1915 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004f1919 */
    goto mi_004f19d1;
mi_004f191e:;
    /* 004f191e */
    f->r[0]=mi_read((0x00000010u+f->r[6]),4u);
    /* 004f1921 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004f1923 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a2eu+f->r[3]),1u))&0xffu)<<0);
    /* 004f1929 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f192b */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004f192f */
    MI_U32((0x00000010u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1932 */
    goto mi_004f19d1;
mi_004f1937:;
    /* 004f1937 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004f193a */
    if(!(f->flags&MI_ZF))goto mi_004f1996;
    /* 004f193c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f193e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004f1940 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f1964;
    /* 004f1942 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*2u);
mi_004f1946:;
    /* 004f1946 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x005e68d8u+f->r[0]+f->r[1]*2u),1u))&0xffu)<<0);
    /* 004f194d */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f194f */
    f->r[2]=mi_read((0x00000010u+f->r[6]),4u);
    /* 004f1952 */
    if((f->flags&MI_ZF))goto mi_004f1959;
    /* 004f1954 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f1957 */
    goto mi_004f195c;
mi_004f1959:;
    /* 004f1959 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_004f195c:;
    /* 004f195c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f195d */
    MI_U32((0x00000010u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004f1960 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004f1962 */
    if(mi_signed_less(f->flags))goto mi_004f1946;
mi_004f1964:;
    /* 004f1964 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f1966 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004f196a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a2fu+f->r[3]),1u))&0xffu)<<0);
    /* 004f1970 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x005e68d8u+f->r[2]+f->r[0]*2u),1u))&0xffu)<<0);
    /* 004f1977 */
    f->r[0]=mi_read((0x00000014u+f->r[6]),4u);
    /* 004f197a */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f197c */
    if((f->flags&MI_ZF))goto mi_004f198a;
    /* 004f197e */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004f1982 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1985 */
    MI_U32((0x00000014u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1988 */
    goto mi_004f19d1;
mi_004f198a:;
    /* 004f198a */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004f198e */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1991 */
    MI_U32((0x00000014u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1994 */
    goto mi_004f19d1;
mi_004f1996:;
    /* 004f1996 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f1999 */
    if(!(f->flags&MI_ZF))goto mi_004f19a5;
    /* 004f199b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f199d */
    if(!(f->flags&MI_ZF))goto mi_004f19a5;
    /* 004f199f */
    f->r[0]=mi_read((0x00000010u+f->r[6]),4u);
    /* 004f19a2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f19a3 */
    goto mi_004f19af;
mi_004f19a5:;
    /* 004f19a5 */
    f->r[0]=mi_read((0x00000010u+f->r[6]),4u);
    /* 004f19a8 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004f19ab */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f19ad */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004f19af:;
    /* 004f19af */
    MI_U32((0x00000010u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f19b2 */
    f->r[0]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f19b6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f19b8 */
    if((f->flags&MI_ZF))goto mi_004f19c0;
    /* 004f19ba */
    a=mi_read((0x00000014u+f->r[6]),4u);b=0x00000006u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000014u+f->r[6]))=(uint32_t)(t);
    /* 004f19be */
    goto mi_004f19d1;
mi_004f19c0:;
    /* 004f19c0 */
    a=mi_read((0x005e4ef4u),4u);b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f19c7 */
    if(!(f->flags&MI_ZF))goto mi_004f19cd;
    /* 004f19c9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f19cb */
    if((f->flags&MI_ZF))goto mi_004f19df;
mi_004f19cd:;
    /* 004f19cd */
    a=mi_read((0x00000010u+f->r[6]),4u);b=0x00000006u;t=mi_alu(f,a,b,32,0);MI_U32((0x00000010u+f->r[6]))=(uint32_t)(t);
mi_004f19d1:;
    /* 004f19d1 */
    f->r[0]=mi_read((0x005e4ef4u),4u);
    /* 004f19d6 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004f19d9 */
    if(!(f->flags&MI_ZF))goto mi_004f19e5;
    /* 004f19db */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f19dd */
    if(!(f->flags&MI_ZF))goto mi_004f1a34;
mi_004f19df:;
    /* 004f19df */
    f->r[0]=mi_read((0x00000028u+f->r[6]),4u);
    /* 004f19e2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f19e3 */
    goto mi_004f1a59;
mi_004f19e5:;
    /* 004f19e5 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004f19e8 */
    if(!(f->flags&MI_ZF))goto mi_004f1a34;
    /* 004f19ea */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f19ec */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004f19ee */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f1a12;
    /* 004f19f0 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*2u);
mi_004f19f4:;
    /* 004f19f4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x005e68d8u+f->r[0]+f->r[1]*2u),1u))&0xffu)<<0);
    /* 004f19fb */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f19fd */
    f->r[2]=mi_read((0x00000028u+f->r[6]),4u);
    /* 004f1a00 */
    if((f->flags&MI_ZF))goto mi_004f1a07;
    /* 004f1a02 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f1a05 */
    goto mi_004f1a0a;
mi_004f1a07:;
    /* 004f1a07 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_004f1a0a:;
    /* 004f1a0a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f1a0b */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004f1a0e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004f1a10 */
    if(mi_signed_less(f->flags))goto mi_004f19f4;
mi_004f1a12:;
    /* 004f1a12 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f1a14 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004f1a18 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a30u+f->r[3]),1u))&0xffu)<<0);
    /* 004f1a1e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x005e68d8u+f->r[0]+f->r[1]*2u),1u))&0xffu)<<0);
    /* 004f1a25 */
    f->r[0]=mi_read((0x0000002cu+f->r[6]),4u);
    /* 004f1a28 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1a2a */
    if((f->flags&MI_ZF))goto mi_004f1a4e;
    /* 004f1a2c */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1a2f */
    MI_U32((0x0000002cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1a32 */
    goto mi_004f1a5c;
mi_004f1a34:;
    /* 004f1a34 */
    f->r[1]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f1a38 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004f1a3b */
    f->r[7]=mi_read((0x00000028u+f->r[6]),4u);
    /* 004f1a3e */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f1a40 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f1a42 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f1a44 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[7]);
    /* 004f1a47 */
    f->r[0]=f->r[7];
    /* 004f1a49 */
    if((f->flags&MI_ZF))goto mi_004f1a56;
    /* 004f1a4b */
    f->r[0]=mi_read((0x0000002cu+f->r[6]),4u);
mi_004f1a4e:;
    /* 004f1a4e */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1a51 */
    MI_U32((0x0000002cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1a54 */
    goto mi_004f1a5c;
mi_004f1a56:;
    /* 004f1a56 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004f1a59:;
    /* 004f1a59 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
mi_004f1a5c:;
    /* 004f1a5c */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a3cu+f->r[3]),2u);
    /* 004f1a63 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004f1a65 */
    if((f->flags&MI_ZF))goto mi_004f1b6d;
    /* 004f1a6b */
    f->r[7]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004f1a6f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f1a71 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004f1a74 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004f1a76 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004f1a79 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[7]),1u))&0xffu)<<0);
    /* 004f1a7f */
    a=f->r[0];b=0xffffffb1u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f1a82 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004f1a85 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004f1b6d;
    /* 004f1a8b */
    switch(mi_read((0x004f1c90u+f->r[0]*4u),4u)){case 0x004f1a92u:goto mi_004f1a92;case 0x004f1ad0u:goto mi_004f1ad0;case 0x004f1b43u:goto mi_004f1b43;case 0x004f1b59u:goto mi_004f1b59;case 0x004f1b6du:goto mi_004f1b6d;default:abort();}
mi_004f1a92:;
    /* 004f1a92 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004f1a96 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1a97 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1a9cu;f->boundary(0x004f1a97u,0x004eea10u,f);
    /* 004f1a9c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1a9f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1aa1 */
    if((f->flags&MI_ZF))goto mi_004f1b6d;
    /* 004f1aa7 */
    f->r[2]=mi_read((0x00000048u+f->r[6]),4u);
    /* 004f1aaa */
    f->r[1]=mi_read((0x00000060u+f->r[6]),4u);
    /* 004f1aad */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004f1aae */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004f1aaf */
    MI_U32((0x00000048u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004f1ab2 */
    MI_U32((0x00000060u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f1ab5 */
    a=mi_read((0x00bcd630u+f->r[7]),1u);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004f1abc */
    if(!(f->flags&MI_ZF))goto mi_004f1b6d;
    /* 004f1ac2 */
    f->r[7]=(f->r[7]&0xffff0000u)|(((mi_read((0x00bcd660u+f->r[7]),2u))&0xffffu)<<0);
    /* 004f1ac9 */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004f1acc */
    if((f->flags&MI_ZF))goto mi_004f1b19;
    /* 004f1ace */
    goto mi_004f1b08;
mi_004f1ad0:;
    /* 004f1ad0 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004f1ad4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1ad5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1adau;f->boundary(0x004f1ad5u,0x004eea10u,f);
    /* 004f1ada */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1add */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1adf */
    if((f->flags&MI_ZF))goto mi_004f1b6d;
    /* 004f1ae5 */
    f->r[2]=mi_read((0x0000004cu+f->r[6]),4u);
    /* 004f1ae8 */
    f->r[1]=mi_read((0x00000060u+f->r[6]),4u);
    /* 004f1aeb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004f1aec */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004f1aed */
    MI_U32((0x0000004cu+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004f1af0 */
    MI_U32((0x00000060u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 004f1af3 */
    a=mi_read((0x00bcd630u+f->r[7]),1u);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004f1afa */
    if(!(f->flags&MI_ZF))goto mi_004f1b6d;
    /* 004f1afc */
    f->r[7]=(f->r[7]&0xffff0000u)|(((mi_read((0x00bcd660u+f->r[7]),2u))&0xffffu)<<0);
    /* 004f1b03 */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004f1b06 */
    if((f->flags&MI_ZF))goto mi_004f1b19;
mi_004f1b08:;
    /* 004f1b08 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[7]&0xffffu);
    /* 004f1b0b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1b0c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1b0d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1b12u;f->boundary(0x004f1b0du,0x004f26a0u,f);
    /* 004f1b12 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1b15 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1b17 */
    if(!(f->flags&MI_ZF))goto mi_004f1b6d;
mi_004f1b19:;
    /* 004f1b19 */
    f->r[3]=mi_read((0x0000005cu+f->r[6]),4u);
    /* 004f1b1c */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004f1b20 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004f1b21 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1b22 */
    MI_U32((0x0000005cu+f->r[6]))=(uint32_t)(f->r[3]);
    /* 004f1b25 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1b2au;f->boundary(0x004f1b25u,0x004efd90u,f);
    /* 004f1b2a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1b2d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1b2f */
    if(!(f->flags&MI_ZF))goto mi_004f1b3e;
    /* 004f1b31 */
    f->r[0]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f1b35 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f1b37 */
    if((f->flags&MI_ZF))goto mi_004f1b3e;
    /* 004f1b39 */
    a=mi_read((0x00000054u+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000054u+f->r[6]))=(uint32_t)(t);
    /* 004f1b3c */
    goto mi_004f1b6d;
mi_004f1b3e:;
    /* 004f1b3e */
    a=mi_read((0x00000050u+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000050u+f->r[6]))=(uint32_t)(t);
    /* 004f1b41 */
    goto mi_004f1b6d;
mi_004f1b43:;
    /* 004f1b43 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004f1b47 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1b48 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1b4du;f->boundary(0x004f1b48u,0x004eea10u,f);
    /* 004f1b4d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1b50 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1b52 */
    if((f->flags&MI_ZF))goto mi_004f1b6d;
    /* 004f1b54 */
    a=mi_read((0x00000058u+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000058u+f->r[6]))=(uint32_t)(t);
    /* 004f1b57 */
    goto mi_004f1b6d;
mi_004f1b59:;
    /* 004f1b59 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004f1b5d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1b5e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1b63u;f->boundary(0x004f1b5eu,0x004eea10u,f);
    /* 004f1b63 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1b66 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1b68 */
    if((f->flags&MI_ZF))goto mi_004f1b6d;
    /* 004f1b6a */
    a=mi_read((0x00000064u+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000064u+f->r[6]))=(uint32_t)(t);
mi_004f1b6d:;
    /* 004f1b6d */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004f1b71 */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f1b75 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f1b76 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004f1b7a */
    f->r[1]=f->r[0];
    /* 004f1b7c */
    goto mi_004f1823;
mi_004f1b81:;
    /* 004f1b81 */
    a=mi_read((0x00000000u+f->r[6]),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004f1b84 */
    if(!(f->flags&MI_ZF))goto mi_004f1bc2;
    /* 004f1b86 */
    f->r[0]=mi_read((0x00eaaf58u),4u);
    /* 004f1b8b */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004f1b8d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f1b8f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f1bc2;
    /* 004f1b91 */
    f->r[5]=mi_read((0x00000064u+f->r[4]),4u);
    /* 004f1b95 */
    f->r[3]=0x00eaac44u;
mi_004f1b9a:;
    /* 004f1b9a */
    f->r[0]=mi_read((0x00000000u+f->r[3]),4u);
    /* 004f1b9c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f1b9d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f1ba2u;f->boundary(0x004f1b9du,0x004efd90u,f);
    /* 004f1ba2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1ba5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f1ba7 */
    if(!(f->flags&MI_ZF))goto mi_004f1bb2;
    /* 004f1ba9 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004f1bab */
    if((f->flags&MI_ZF))goto mi_004f1bb2;
    /* 004f1bad */
    a=mi_read((0x0000001cu+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x0000001cu+f->r[6]))=(uint32_t)(t);
    /* 004f1bb0 */
    goto mi_004f1bb5;
mi_004f1bb2:;
    /* 004f1bb2 */
    a=mi_read((0x00000018u+f->r[6]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000018u+f->r[6]))=(uint32_t)(t);
mi_004f1bb5:;
    /* 004f1bb5 */
    f->r[0]=mi_read((0x00eaaf58u),4u);
    /* 004f1bba */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004f1bbb */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004f1bbe */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004f1bc0 */
    if(mi_signed_less(f->flags))goto mi_004f1b9a;
mi_004f1bc2:;
    /* 004f1bc2 */
    f->r[1]=mi_read((0x00000028u+f->r[6]),4u);
    /* 004f1bc5 */
    f->r[7]=mi_read((0x00000010u+f->r[6]),4u);
    /* 004f1bc8 */
    f->r[0]=f->r[1];
    /* 004f1bca */
    f->r[3]=mi_read((0x0000002cu+f->r[6]),4u);
    /* 004f1bcd */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1bcf */
    f->r[5]=mi_read((0x00000014u+f->r[6]),4u);
    /* 004f1bd2 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1bd3 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1bd5 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1bd7 */
    MI_U32((0x00000020u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1bda */
    f->r[0]=f->r[3];
    /* 004f1bdc */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1bde */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1bdf */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1be1 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1be3 */
    MI_U32((0x00000024u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1be6 */
    f->r[0]=f->r[1];
    /* 004f1be8 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1be9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1beb */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1bed */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1bf0 */
    f->r[0]=f->r[3];
    /* 004f1bf2 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1bf3 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1bf5 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1bf7 */
    MI_U32((0x0000002cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1bfa */
    f->r[0]=f->r[7];
    /* 004f1bfc */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1bfd */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1bff */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1c01 */
    MI_U32((0x00000010u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1c04 */
    f->r[0]=f->r[5];
    /* 004f1c06 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1c07 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1c09 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f1c0a */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1c0c */
    MI_U32((0x00000014u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1c0f */
    f->r[0]=mi_read((0x00000018u+f->r[6]),4u);
    /* 004f1c12 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1c13 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1c15 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f1c16 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1c18 */
    MI_U32((0x00000018u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1c1b */
    f->r[0]=mi_read((0x0000001cu+f->r[6]),4u);
    /* 004f1c1e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004f1c1f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f1c21 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004f1c23 */
    MI_U32((0x0000001cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1c26 */
    f->r[0]=mi_read((0x00000050u+f->r[6]),4u);
    /* 004f1c29 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f1c2c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*8u);
    /* 004f1c2f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f1c32 */
    MI_U32((0x00000068u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004f1c35 */
    f->r[0]=mi_read((0x00000054u+f->r[6]),4u);
    /* 004f1c38 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f1c3b */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*8u);
    /* 004f1c3e */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f1c41 */
    MI_U32((0x0000006cu+f->r[6]))=(uint32_t)(f->r[2]);
mi_004f1c44:;
    /* 004f1c44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f1c45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f1c46 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f1c49 */
    f->r[4]+=4;return;
}


void restore_pyramid_C_004eea10(struct restore_pyramid_summary_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004eea10 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eea11 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eea12 */
    f->r[5]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004eea16 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eea17 */
    f->r[0]=f->r[5];
    /* 004eea19 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eea1a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004eea1d */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004eea1f */
    f->r[3]=(0x00799a3cu+f->r[0]*8u);
    /* 004eea26 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a3cu+f->r[0]*8u),2u);
    /* 004eea2e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eea30 */
    if((f->flags&MI_ZF))goto mi_004eeac7;
    /* 004eea36 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004eea39 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004eea3c */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004eea3e */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004eea41 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd606u+f->r[7]),1u))&0xffu)<<0);
    /* 004eea47 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004eea49 */
    if((f->flags&MI_ZF))goto mi_004eeac2;
    /* 004eea4b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004eea4d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[7]),1u))&0xffu)<<0);
    /* 004eea53 */
    f->r[6]=f->r[1];
    /* 004eea55 */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004eea58 */
    if((f->flags&MI_ZF))goto mi_004eea5f;
    /* 004eea5a */
    a=f->r[6];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004eea5d */
    if(!(f->flags&MI_ZF))goto mi_004eea69;
mi_004eea5f:;
    /* 004eea5f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004eea61 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd608u+f->r[7]),1u))&0xffu)<<0);
    /* 004eea67 */
    f->r[6]=f->r[2];
mi_004eea69:;
    /* 004eea69 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004eea6b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[7]),1u))&0xffu)<<0);
    /* 004eea71 */
    a=f->r[0];b=0xffffffb1u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004eea74 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004eea77 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004eeac7;
    /* 004eea79 */
    switch(mi_read((0x004eeb44u+f->r[0]*4u),4u)){case 0x004eea80u:goto mi_004eea80;case 0x004eeac7u:goto mi_004eeac7;case 0x004eeaceu:goto mi_004eeace;case 0x004eeae4u:goto mi_004eeae4;case 0x004eeb1du:goto mi_004eeb1d;default:abort();}
mi_004eea80:;
    /* 004eea80 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eea81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004eea86u;f->boundary(0x004eea81u,0x004ef100u,f);
    /* 004eea86 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004eea89 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eea8b */
    if(!(f->flags&MI_ZF))goto mi_004eeac2;
    /* 004eea8d */
    a=f->r[6];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004eea90 */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eea92 */
    a=f->r[6];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004eea95 */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eea97 */
    a=f->r[6];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004eea9a */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eea9c */
    a=f->r[6];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004eea9f */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eeaa1 */
    a=f->r[6];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 004eeaa4 */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eeaa6 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004eeaa9 */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eeaab */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004eeaae */
    if(!(f->flags&MI_ZF))goto mi_004eeac7;
    /* 004eeab0 */
    f->r[0]=mi_read((0x00bcd620u+f->r[7]),4u);
    /* 004eeab6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[0]*2u),2u);
    /* 004eeabe */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004eeac0 */
    if((f->flags&MI_ZF))goto mi_004eeb3b;
mi_004eeac2:;
    /* 004eeac2 */
    MI_U16((0x00000000u+f->r[3]))=(uint16_t)(0x00000000u);
mi_004eeac7:;
    /* 004eeac7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eeac8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eeac9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004eeaca */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004eeacc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004eeacd */
    f->r[4]+=4;return;
mi_004eeace:;
    /* 004eeace */
    a=f->r[6];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004eead1 */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eead3 */
    a=f->r[6];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004eead6 */
    if((f->flags&MI_ZF))goto mi_004eeadd;
    /* 004eead8 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004eeadb */
    if(!(f->flags&MI_ZF))goto mi_004eeac7;
mi_004eeadd:;
    /* 004eeadd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eeade */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eeadf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004eeae0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004eeae2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004eeae3 */
    f->r[4]+=4;return;
mi_004eeae4:;
    /* 004eeae4 */
    a=f->r[6];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004eeae7 */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeae9 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004eeaec */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeaee */
    a=f->r[6];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 004eeaf1 */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeaf3 */
    a=f->r[6];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004eeaf6 */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeaf8 */
    a=f->r[6];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 004eeafb */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeafd */
    a=f->r[6];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 004eeb00 */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeb02 */
    a=f->r[6];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004eeb05 */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeb07 */
    a=f->r[6];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 004eeb0a */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeb0c */
    a=f->r[6];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 004eeb0f */
    if((f->flags&MI_ZF))goto mi_004eeb16;
    /* 004eeb11 */
    a=f->r[6];b=0x00000016u;t=mi_alu(f,a,b,32,1);
    /* 004eeb14 */
    if(!(f->flags&MI_ZF))goto mi_004eeac7;
mi_004eeb16:;
    /* 004eeb16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eeb17 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eeb18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004eeb19 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004eeb1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004eeb1c */
    f->r[4]+=4;return;
mi_004eeb1d:;
    /* 004eeb1d */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004eeb20 */
    if((f->flags&MI_ZF))goto mi_004eeb3b;
    /* 004eeb22 */
    a=f->r[6];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004eeb25 */
    if((f->flags&MI_ZF))goto mi_004eeb3b;
    /* 004eeb27 */
    a=f->r[6];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004eeb2a */
    if((f->flags&MI_ZF))goto mi_004eeb3b;
    /* 004eeb2c */
    a=f->r[6];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004eeb2f */
    if((f->flags&MI_ZF))goto mi_004eeb3b;
    /* 004eeb31 */
    a=f->r[6];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 004eeb34 */
    if((f->flags&MI_ZF))goto mi_004eeb3b;
    /* 004eeb36 */
    a=f->r[6];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004eeb39 */
    if(!(f->flags&MI_ZF))goto mi_004eeac7;
mi_004eeb3b:;
    /* 004eeb3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004eeb3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004eeb3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004eeb3e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004eeb40 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004eeb41 */
    f->r[4]+=4;return;
}


void restore_pyramid_C_004ef100(struct restore_pyramid_summary_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ef100 */
    f->r[0]=MI_U32((0x005e4ef4u));
    /* 004ef105 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef106 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef107 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef108 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ef10b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef10c */
    if((f->flags&MI_ZF))goto mi_004ef17d;
    /* 004ef10e */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004ef112 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef114 */
    f->r[1]=f->r[2];
    /* 004ef116 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ef119 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ef11b */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ef11e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a2cu+f->r[1])))&0xffu)<<0);
    /* 004ef124 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00799a2du+f->r[1])))&0xffu)<<0);
    /* 004ef12a */
    f->r[6]=f->r[0];
    /* 004ef12c */
    f->r[0]=0x00000001u;
    /* 004ef131 */
    a=(f->r[3]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004ef133 */
    if(!(f->flags&MI_ZF))goto mi_004ef14e;
    /* 004ef135 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]));
    /* 004ef13c */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004ef13e */
    f->r[7]=f->r[2];
    /* 004ef140 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004ef143 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ef145 */
    a=MI_U8((0x00799a2du+f->r[7]*8u));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004ef14c */
    if((f->flags&MI_ZF))goto mi_004ef17d;
mi_004ef14e:;
    /* 004ef14e */
    f->r[3]=MI_U32((0x00570070u+f->r[6]*4u));
    /* 004ef155 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004ef157 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004ef159 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ef17d;
    /* 004ef15b */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]));
    /* 004ef162 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004ef165 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004ef168 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ef16b */
    a=f->r[2];b=0x005f3878u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_004ef171:;
    /* 004ef171 */
    a=f->r[5];b=MI_U32((0x00000000u+f->r[2]));t=mi_alu(f,a,b,32,1);
    /* 004ef173 */
    if((f->flags&MI_ZF))goto mi_004ef184;
    /* 004ef175 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004ef176 */
    a=f->r[2];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ef179 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004ef17b */
    if(mi_signed_less(f->flags))goto mi_004ef171;
mi_004ef17d:;
    /* 004ef17d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004ef17f:;
    /* 004ef17f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ef180 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ef181 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ef182 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ef183 */
    f->r[4]+=4;return;
mi_004ef184:;
    /* 004ef184 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004ef187 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ef18a */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ef18c */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004ef18f */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ef192 */
    f->r[6]=MI_U32((0x005f387cu+f->r[2]));
    /* 004ef198 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004ef19a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ef1c2;
    /* 004ef19c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ef19e */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00799a2fu+f->r[1])))&0xffu)<<0);
    /* 004ef1a4 */
    f->r[7]=f->r[3];
    /* 004ef1a6 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ef1a8 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[1])))&0xffu)<<0);
    /* 004ef1ae */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004ef1b1 */
    f->r[7]=(0x00000000u+f->r[3]+f->r[7]*2u);
    /* 004ef1b4 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004ef1b6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004ef1c2;
    /* 004ef1b8 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00799a31u+f->r[1])))&0xffu)<<0);
    /* 004ef1be */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef1c0 */
    if((f->flags&MI_ZF))goto mi_004ef17f;
mi_004ef1c2:;
    /* 004ef1c2 */
    f->r[2]=MI_U32((0x005f3880u+f->r[2]));
    /* 004ef1c8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004ef1ca */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ef17d;
    /* 004ef1cc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ef1ce */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ef1d0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a2fu+f->r[1])))&0xffu)<<0);
    /* 004ef1d6 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[1])))&0xffu)<<0);
    /* 004ef1dc */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ef1df */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*2u);
    /* 004ef1e2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004ef1e4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004ef17d;
    /* 004ef1e6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a32u+f->r[1])))&0xffu)<<0);
    /* 004ef1ec */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef1ee */
    if(!(f->flags&MI_ZF))goto mi_004ef17d;
    /* 004ef1f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ef1f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ef1f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ef1f3 */
    f->r[0]=0x00000002u;
    /* 004ef1f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ef1f9 */
    f->r[4]+=4;return;
}


void restore_pyramid_C_004ef450(struct restore_pyramid_summary_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ef450 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef451 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef452 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef453 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ef454 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004ef458 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004ef45a */
    f->r[0]=f->r[7];
    /* 004ef45c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004ef45e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef461 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef463 */
    MI_U32((0x00eaaf58u))=(uint32_t)(f->r[5]);
    /* 004ef469 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef46c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a2cu+f->r[0])))&0xffu)<<0);
    /* 004ef472 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[0]));
    /* 004ef479 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004ef47b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004ef47f */
    f->r[2]=f->r[7];
    /* 004ef481 */
    f->r[3]=f->r[7];
    /* 004ef483 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004ef486 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004ef488 */
    f->r[6]=(0x00799a2eu+f->r[2]*8u);
mi_004ef48f:;
    /* 004ef48f */
    f->r[2]=MI_U32((0x005e4ef4u));
    /* 004ef495 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ef498 */
    if(!(f->flags&MI_ZF))goto mi_004ef4a3;
    /* 004ef49a */
    f->r[0]=MI_U32((0x0056f7ecu+f->r[1]*4u));
    /* 004ef4a1 */
    goto mi_004ef4aa;
mi_004ef4a3:;
    /* 004ef4a3 */
    f->r[0]=MI_U32((0x0056f7d8u+f->r[1]*4u));
mi_004ef4aa:;
    /* 004ef4aa */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef4ac */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004ef4ae */
    if(!mi_signed_less(f->flags))goto mi_004ef54b;
    /* 004ef4b4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffffffu+f->r[6])))&0xffu)<<0);
    /* 004ef4b7 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef4b9 */
    if((f->flags&MI_ZF))goto mi_004ef4ca;
    /* 004ef4bb */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004ef4be */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004ef53b;
    /* 004ef4c0 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004ef4c3 */
    if((f->flags&MI_ZF))goto mi_004ef53b;
    /* 004ef4c5 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ef4c8 */
    if((f->flags&MI_ZF))goto mi_004ef53b;
mi_004ef4ca:;
    /* 004ef4ca */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004ef4cc */
    if((f->flags&MI_ZF))goto mi_004ef51d;
    /* 004ef4ce */
    f->r[0]=f->r[5];
    /* 004ef4d0 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef4d3 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004ef4d5 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004ef4d8 */
    a=(f->r[1]&0xffu);b=MI_U8((0x00799a2du+f->r[0]));t=mi_alu(f,a,b,8,1);
    /* 004ef4de */
    if(!(f->flags&MI_ZF))goto mi_004ef519;
    /* 004ef4e0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2fu+f->r[0])))&0xffu)<<0);
    /* 004ef4e6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef4e8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000001u+f->r[6])))&0xffu)<<0);
    /* 004ef4eb */
    if(!(f->flags&MI_ZF))goto mi_004ef503;
    /* 004ef4ed */
    a=(f->r[1]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004ef4ef */
    if((f->flags&MI_CF))goto mi_004ef51d;
    /* 004ef4f1 */
    if(!(f->flags&MI_ZF))goto mi_004ef53b;
    /* 004ef4f3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004ef4f5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[0])))&0xffu)<<0);
    /* 004ef4fb */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004ef4fd */
    if((f->flags&MI_CF))goto mi_004ef51d;
    /* 004ef4ff */
    if(!(f->flags&MI_ZF))goto mi_004ef53b;
    /* 004ef501 */
    goto mi_004ef529;
mi_004ef503:;
    /* 004ef503 */
    a=(f->r[1]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004ef505 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ef51d;
    /* 004ef507 */
    if(!(f->flags&MI_ZF))goto mi_004ef53b;
    /* 004ef509 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004ef50b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[0])))&0xffu)<<0);
    /* 004ef511 */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004ef513 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004ef51d;
    /* 004ef515 */
    if(!(f->flags&MI_ZF))goto mi_004ef53b;
    /* 004ef517 */
    goto mi_004ef529;
mi_004ef519:;
    /* 004ef519 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ef51b */
    if(!(f->flags&MI_ZF))goto mi_004ef53b;
mi_004ef51d:;
    /* 004ef51d */
    MI_U32((0x00eaaf58u))=(uint32_t)(0x00000000u);
    /* 004ef527 */
    f->r[5]=f->r[3];
mi_004ef529:;
    /* 004ef529 */
    f->r[0]=MI_U32((0x00eaaf58u));
    /* 004ef52e */
    MI_U32((0x00eaac44u+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 004ef535 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ef536 */
    MI_U32((0x00eaaf58u))=(uint32_t)(f->r[0]);
mi_004ef53b:;
    /* 004ef53b */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004ef53f */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004ef540 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004ef546 */
    goto mi_004ef48f;
mi_004ef54b:;
    /* 004ef54b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ef54c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ef54d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ef54e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ef54f */
    f->r[4]+=4;return;
}


void restore_pyramid_C_004f26a0(struct restore_pyramid_summary_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f26a0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004f26a4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f26a7 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f26aa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f26ac */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f26af */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd606u+f->r[1])))&0xffu)<<0);
    /* 004f26b5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f26b7 */
    if((f->flags&MI_ZF))goto mi_004f2714;
    /* 004f26b9 */
    a=MI_U8((0x00bcd602u+f->r[1]));b=0x00000055u;t=mi_alu(f,a,b,8,1);
    /* 004f26c0 */
    if(!(f->flags&MI_ZF))goto mi_004f2714;
    /* 004f26c2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f26c4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[1])))&0xffu)<<0);
    /* 004f26ca */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004f26cd */
    if((f->flags&MI_ZF))goto mi_004f26d4;
    /* 004f26cf */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004f26d2 */
    if(!(f->flags&MI_ZF))goto mi_004f26dc;
mi_004f26d4:;
    /* 004f26d4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f26d6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd608u+f->r[1])))&0xffu)<<0);
mi_004f26dc:;
    /* 004f26dc */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004f26df */
    if((f->flags&MI_ZF))goto mi_004f2704;
    /* 004f26e1 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004f26e4 */
    if((f->flags&MI_ZF))goto mi_004f2704;
    /* 004f26e6 */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 004f26e9 */
    if((f->flags&MI_ZF))goto mi_004f2704;
    /* 004f26eb */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004f26ee */
    if((f->flags&MI_ZF))goto mi_004f2704;
    /* 004f26f0 */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 004f26f3 */
    if((f->flags&MI_ZF))goto mi_004f2704;
    /* 004f26f5 */
    a=f->r[0];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 004f26f8 */
    if((f->flags&MI_ZF))goto mi_004f2704;
    /* 004f26fa */
    a=f->r[0];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 004f26fd */
    if((f->flags&MI_ZF))goto mi_004f2704;
    /* 004f26ff */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004f2702 */
    if(!(f->flags&MI_ZF))goto mi_004f2714;
mi_004f2704:;
    /* 004f2704 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd660u+f->r[1]));
    /* 004f270b */
    a=f->r[0];b=MI_U32((0x00000008u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004f270f */
    if(!(f->flags&MI_ZF))goto mi_004f2714;
    /* 004f2711 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004f2713 */
    f->r[4]+=4;return;
mi_004f2714:;
    /* 004f2714 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004f2716 */
    f->r[4]+=4;return;
}


void restore_pyramid_C_004efd90(struct restore_pyramid_summary_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004efd90 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004efd94 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efd95 */
    f->r[0]=f->r[1];
    /* 004efd97 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004efd9a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004efd9c */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004efd9f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a2fu+f->r[0])))&0xffu)<<0);
    /* 004efda5 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00799a30u+f->r[0])))&0xffu)<<0);
    /* 004efdab */
    a=(f->r[1]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004efdad */
    if(!(f->flags&MI_CF))goto mi_004efe1e;
    /* 004efdaf */
    f->r[2]=MI_U32((0x005e4ef4u));
    /* 004efdb5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004efdb6 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004efdb9 */
    if(!(f->flags&MI_ZF))goto mi_004efde3;
    /* 004efdbb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004efdbd */
    f->r[6]=f->r[1];
    /* 004efdbf */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2cu+f->r[0])))&0xffu)<<0);
    /* 004efdc5 */
    a=f->r[6];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004efdcb */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004efdce */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x005e68d8u+f->r[6]+f->r[2]*2u)))&0xffu)<<0);
    /* 004efdd5 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004efdd7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 004efdd9 */
    a=f->r[2];b=0xfffffffdu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004efddc */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004efddf */
    f->r[6]=f->r[2];
    /* 004efde1 */
    goto mi_004efdfc;
mi_004efde3:;
    /* 004efde3 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004efde6 */
    if(!(f->flags&MI_ZF))goto mi_004efdf7;
    /* 004efde8 */
    a=MI_U16((0x007999eau+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004efdf0 */
    f->r[6]=0x00000001u;
    /* 004efdf5 */
    if((f->flags&MI_ZF))goto mi_004efdfc;
mi_004efdf7:;
    /* 004efdf7 */
    f->r[6]=0x00000006u;
mi_004efdfc:;
    /* 004efdfc */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004efdfe */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a2eu+f->r[0])))&0xffu)<<0);
    /* 004efe04 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004efe06 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004efe07 */
    if(!(f->flags&MI_ZF))goto mi_004efe1a;
    /* 004efe09 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004efe0f */
    a=f->r[3];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004efe15 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004efe16 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004efe18 */
    if((f->flags&MI_ZF))goto mi_004efe1e;
mi_004efe1a:;
    /* 004efe1a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004efe1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004efe1d */
    f->r[4]+=4;return;
mi_004efe1e:;
    /* 004efe1e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004efe20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004efe21 */
    f->r[4]+=4;return;
}


__declspec(noinline) __declspec(noreturn) void restore_pyramid_summary_failure(uint32_t target)
{fprintf(stderr,"Unsupported pyramid summary boundary %08lx\n",(unsigned long)target);abort();}
void restore_pyramid_summary_dispatch(uint32_t site,uint32_t target,struct restore_pyramid_summary_frame *frame)
{
 (void)site;
 switch(target){
 case 0x4f1310:restore_pyramid_C_004f1310(frame);return;
 case 0x4eea10:restore_pyramid_C_004eea10(frame);return;
 case 0x4ef100:restore_pyramid_C_004ef100(frame);return;
 case 0x4ef450:restore_pyramid_C_004ef450(frame);return;
 case 0x4f26a0:restore_pyramid_C_004f26a0(frame);return;
 case 0x4efd90:restore_pyramid_C_004efd90(frame);return;
 default:restore_pyramid_summary_failure(target);
 }
}
uint32_t FUN_004f1310(uint32_t building,int32_t output[28])
{
 uint32_t stack[1024],i,entry;struct restore_pyramid_summary_frame frame={{0},2,restore_pyramid_summary_dispatch};
 typedef char pointer_is_32[sizeof(void*)==4?1:-1];(void)sizeof(pointer_is_32);
 for(i=0;i<1024;++i)stack[i]=0xa5a55a5au;
 entry=(uint32_t)(uintptr_t)&stack[1000];frame.r[4]=entry;
 stack[1000]=0;stack[1001]=building;stack[1002]=(uint32_t)(uintptr_t)output;
 restore_pyramid_C_004f1310(&frame);
 if(frame.r[4]!=entry+4u)restore_pyramid_summary_failure(frame.r[4]);
 for(i=0;i<16;++i)if(stack[i]!=0xa5a55a5au)restore_pyramid_summary_failure(0x4f1310u);
 for(i=1003;i<1024;++i)if(stack[i]!=0xa5a55a5au)restore_pyramid_summary_failure(0x4f1310u);
 return frame.r[0];
}
