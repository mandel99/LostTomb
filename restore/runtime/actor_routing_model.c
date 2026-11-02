/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_routing.h"
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
static __inline uint32_t mi_alu(struct restore_routing_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_routing_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_routing_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_0045be40[]={0xcf,0xbb,0x45,0x00,0xe7,0xbb,0x45,0x00,0xec,0xbb,0x45,0x00};
static const uint8_t mi_rom_0045be4c[]={0x00,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x02,0x02,0x01,0x02,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x01,0x01,0x01};
static const uint8_t mi_rom_00483e00[]={0xa2,0x3a,0x48,0x00,0xad,0x3a,0x48,0x00,0xb4,0x3a,0x48,0x00,0xbb,0x3a,0x48,0x00,0xc2,0x3a,0x48,0x00,0xa6,0x3a,0x48,0x00,0xd0,0x3a,0x48,0x00,0xc9,0x3a,0x48,0x00,0xf3,0x3a,0x48,0x00,0xf3,0x3a,0x48,0x00,0xf3,0x3a,0x48,0x00,0xf3,0x3a,0x48,0x00,0xec,0x3a,0x48,0x00,0xf3,0x3a,0x48,0x00,0xe5,0x3a,0x48,0x00,0xf3,0x3a,0x48,0x00,0xd7,0x3a,0x48,0x00,0xf3,0x3a,0x48,0x00,0xde,0x3a,0x48,0x00};
static const uint8_t mi_rom_00483e4c[]={0x73,0x3b,0x48,0x00,0x5d,0x3b,0x48,0x00,0x5d,0x3b,0x48,0x00,0x5d,0x3b,0x48,0x00,0x5d,0x3b,0x48,0x00,0x5d,0x3b,0x48,0x00,0x73,0x3b,0x48,0x00,0x5d,0x3b,0x48,0x00};
static const uint8_t mi_rom_00483e6c[]={0x87,0x3b,0x48,0x00,0x6d,0x3b,0x48,0x00,0x6d,0x3b,0x48,0x00,0x6d,0x3b,0x48,0x00,0x6d,0x3b,0x48,0x00,0x6d,0x3b,0x48,0x00,0x87,0x3b,0x48,0x00,0x6d,0x3b,0x48,0x00};
static const uint8_t mi_rom_004a610c[]={0xb5,0x5d,0x4a,0x00,0xc6,0x5d,0x4a,0x00,0xf8,0x5d,0x4a,0x00,0x2d,0x5e,0x4a,0x00,0x5f,0x5e,0x4a,0x00,0x95,0x5e,0x4a,0x00,0xcb,0x5e,0x4a,0x00,0xdc,0x5e,0x4a,0x00,0x12,0x5f,0x4a,0x00,0x48,0x5f,0x4a,0x00,0x7d,0x5f,0x4a,0x00,0xbf,0x5f,0x4a,0x00,0xf1,0x5f,0x4a,0x00,0x02,0x60,0x4a,0x00,0x34,0x60,0x4a,0x00,0x76,0x60,0x4a,0x00,0x96,0x60,0x4a,0x00,0xb7,0x60,0x4a,0x00};
static const uint8_t mi_rom_004b3b64[]={0x5c,0x3b,0x4b,0x00,0x97,0x38,0x4b,0x00,0x55,0x3b,0x4b,0x00};
static const uint8_t mi_rom_004b3b70[]={0x00,0x00,0x00,0x01,0x02,0x02,0x02,0x02,0x01,0x02,0x01,0x01,0x01,0x02,0x01,0x01,0x01,0x01,0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x01,0x02,0x02,0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x02,0x01,0x02,0x02,0x02,0x01,0x01,0x01,0x02,0x02,0x02,0x01,0x02,0x02,0x02,0x02,0x01,0x02,0x02,0x02,0x00,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x01,0x02,0x02,0x01};
static const uint8_t mi_rom_004b3bdc[]={0x2e,0x3a,0x4b,0x00,0x5b,0x3a,0x4b,0x00,0x3d,0x3a,0x4b,0x00,0x4c,0x3a,0x4b,0x00,0x5c,0x3b,0x4b,0x00};
static const uint8_t mi_rom_004b3bf0[]={0x00,0x04,0x04,0x04,0x04,0x00,0x04,0x01,0x01,0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x02,0x04,0x04,0x01,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x03,0x04,0x00,0x04,0x04,0x04,0x00,0x00,0x00,0x04,0x04,0x04,0x00,0x04,0x04,0x04,0x04,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00};
static const uint8_t mi_rom_004b3ea4[]={0xaf,0x3c,0x4b,0x00,0xc6,0x3c,0x4b,0x00,0xe4,0x3c,0x4b,0x00,0xf4,0x3c,0x4b,0x00,0x0f,0x3d,0x4b,0x00,0x23,0x3d,0x4b,0x00,0x3e,0x3d,0x4b,0x00,0x4e,0x3d,0x4b,0x00};
static const uint8_t mi_rom_004b3ec4[]={0xd4,0x3d,0x4b,0x00,0xee,0x3d,0x4b,0x00,0xe1,0x3d,0x4b,0x00,0xd4,0x3d,0x4b,0x00,0xfb,0x3d,0x4b,0x00,0xe1,0x3d,0x4b,0x00};
static const uint8_t mi_rom_004b3edc[]={0x91,0x3e,0x4b,0x00,0x79,0x3e,0x4b,0x00};
static const uint8_t mi_rom_004b3ee4[]={0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0045be40u&&width<=12u&&(offset=address-0x0045be40u)<=12u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045be40[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0045be4cu&&width<=166u&&(offset=address-0x0045be4cu)<=166u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045be4c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00483e00u&&width<=76u&&(offset=address-0x00483e00u)<=76u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00483e00[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00483e4cu&&width<=32u&&(offset=address-0x00483e4cu)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00483e4c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00483e6cu&&width<=32u&&(offset=address-0x00483e6cu)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00483e6c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004a610cu&&width<=72u&&(offset=address-0x004a610cu)<=72u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004a610c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3b64u&&width<=12u&&(offset=address-0x004b3b64u)<=12u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3b64[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3b70u&&width<=108u&&(offset=address-0x004b3b70u)<=108u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3b70[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3bdcu&&width<=20u&&(offset=address-0x004b3bdcu)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3bdc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3bf0u&&width<=102u&&(offset=address-0x004b3bf0u)<=102u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3bf0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3ea4u&&width<=32u&&(offset=address-0x004b3ea4u)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3ea4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3ec4u&&width<=24u&&(offset=address-0x004b3ec4u)<=24u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3ec4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3edcu&&width<=8u&&(offset=address-0x004b3edcu)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3edc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b3ee4u&&width<=25u&&(offset=address-0x004b3ee4u)<=25u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b3ee4[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

static __inline void mi_idiv32(struct restore_routing_frame *f,uint32_t divisor_bits)
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

void restore_routing_C_004a5da0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004a5da0 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5da4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5da5 */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 004a5da8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a60f9;
    /* 004a5dae */
    switch(mi_read((0x004a610cu+f->r[0]*4u),4u)){case 0x004a5db5u:goto mi_004a5db5;case 0x004a5dc6u:goto mi_004a5dc6;case 0x004a5df8u:goto mi_004a5df8;case 0x004a5e2du:goto mi_004a5e2d;case 0x004a5e5fu:goto mi_004a5e5f;case 0x004a5e95u:goto mi_004a5e95;case 0x004a5ecbu:goto mi_004a5ecb;case 0x004a5edcu:goto mi_004a5edc;case 0x004a5f12u:goto mi_004a5f12;case 0x004a5f48u:goto mi_004a5f48;case 0x004a5f7du:goto mi_004a5f7d;case 0x004a5fbfu:goto mi_004a5fbf;case 0x004a5ff1u:goto mi_004a5ff1;case 0x004a6002u:goto mi_004a6002;case 0x004a6034u:goto mi_004a6034;case 0x004a6076u:goto mi_004a6076;case 0x004a6096u:goto mi_004a6096;case 0x004a60b7u:goto mi_004a60b7;default:abort();}
mi_004a5db5:;
    /* 004a5db5 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5db9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5dbb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5dbc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5dc1u;f->boundary(0x004a5dbcu,0x004b27e0u,f);
    /* 004a5dc1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5dc4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5dc5 */
    f->r[4]+=4;return;
mi_004a5dc6:;
    /* 004a5dc6 */
    f->r[1]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5dca */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004a5dcd */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004a5dd0 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004a5dd2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004a5dd9 */
    f->r[6]=(0x00bcd758u+f->r[2]*4u);
    /* 004a5de0 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004a5de2 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004a60e3;
    /* 004a5de8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5dea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5deb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5df0u;f->boundary(0x004a5debu,0x004b27e0u,f);
    /* 004a5df0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5df3 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004a5df6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5df7 */
    f->r[4]+=4;return;
mi_004a5df8:;
    /* 004a5df8 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5dfc */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5dff */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a5e02 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a5e04 */
    f->r[2]=(0x00bcd758u+f->r[1]*4u);
    /* 004a5e0b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a5e12 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004a5e15 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a5e1d;
    /* 004a5e17 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5e19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5e1a */
    MI_U8((0x00000000u+f->r[2]))=(uint8_t)((f->r[1]&0xffu));
    /* 004a5e1c */
    f->r[4]+=4;return;
mi_004a5e1d:;
    /* 004a5e1d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5e1f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5e20 */
    MI_U8((0x00000000u+f->r[2]))=(uint8_t)(0x00000000u);
    /* 004a5e23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5e28u;f->boundary(0x004a5e23u,0x004b27e0u,f);
    /* 004a5e28 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5e2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5e2c */
    f->r[4]+=4;return;
mi_004a5e2d:;
    /* 004a5e2d */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5e31 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5e34 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004a5e37 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004a5e39 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004a5e40 */
    f->r[6]=(0x00bcd758u+f->r[2]*4u);
    /* 004a5e47 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004a5e49 */
    if(!(f->flags&MI_ZF))goto mi_004a60f4;
    /* 004a5e4f */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5e51 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5e52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5e57u;f->boundary(0x004a5e52u,0x004b27e0u,f);
    /* 004a5e57 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5e5a */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 004a5e5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5e5e */
    f->r[4]+=4;return;
mi_004a5e5f:;
    /* 004a5e5f */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5e63 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5e66 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a5e69 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a5e6b */
    f->r[6]=(0x00bcd758u+f->r[1]*4u);
    /* 004a5e72 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a5e79 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004a5e7c */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a60f4;
    /* 004a5e82 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5e84 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5e85 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5e8au;f->boundary(0x004a5e85u,0x004b27e0u,f);
    /* 004a5e8a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a5e8c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5e8f */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5e91 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a5e93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5e94 */
    f->r[4]+=4;return;
mi_004a5e95:;
    /* 004a5e95 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5e99 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5e9c */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004a5e9f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004a5ea1 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004a5ea8 */
    f->r[6]=(0x00bcd758u+f->r[2]*4u);
    /* 004a5eaf */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004a5eb2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a60f4;
    /* 004a5eb8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5eba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5ebb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5ec0u;f->boundary(0x004a5ebbu,0x004b27e0u,f);
    /* 004a5ec0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a5ec2 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5ec5 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5ec7 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a5ec9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5eca */
    f->r[4]+=4;return;
mi_004a5ecb:;
    /* 004a5ecb */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5ecf */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5ed1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5ed2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5ed7u;f->boundary(0x004a5ed2u,0x004b27e0u,f);
    /* 004a5ed7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5eda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5edb */
    f->r[4]+=4;return;
mi_004a5edc:;
    /* 004a5edc */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5ee0 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5ee3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a5ee6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a5ee8 */
    f->r[6]=(0x00bcd758u+f->r[1]*4u);
    /* 004a5eef */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a5ef6 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004a5ef9 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a60e9;
    /* 004a5eff */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f01 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f02 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5f07u;f->boundary(0x004a5f02u,0x004b27e0u,f);
    /* 004a5f07 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a5f09 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5f0c */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5f0e */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a5f10 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5f11 */
    f->r[4]+=4;return;
mi_004a5f12:;
    /* 004a5f12 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5f16 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5f19 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004a5f1c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004a5f1e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004a5f25 */
    f->r[6]=(0x00bcd758u+f->r[2]*4u);
    /* 004a5f2c */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004a5f2f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a60e9;
    /* 004a5f35 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f37 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f38 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5f3du;f->boundary(0x004a5f38u,0x004b27e0u,f);
    /* 004a5f3d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a5f3f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5f42 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5f44 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a5f46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5f47 */
    f->r[4]+=4;return;
mi_004a5f48:;
    /* 004a5f48 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5f4c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5f4f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a5f52 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a5f54 */
    f->r[6]=(0x00bcd758u+f->r[1]*4u);
    /* 004a5f5b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a5f62 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004a5f64 */
    if(!(f->flags&MI_ZF))goto mi_004a60e9;
    /* 004a5f6a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f6c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f6d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5f72u;f->boundary(0x004a5f6du,0x004b27e0u,f);
    /* 004a5f72 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a5f74 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5f77 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5f79 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a5f7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5f7c */
    f->r[4]+=4;return;
mi_004a5f7d:;
    /* 004a5f7d */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5f81 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5f84 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004a5f87 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004a5f89 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004a5f90 */
    f->r[6]=(0x00bcd758u+f->r[2]*4u);
    /* 004a5f97 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004a5f9a */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a5faf;
    /* 004a5f9c */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f9e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5f9f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5fa4u;f->boundary(0x004a5f9fu,0x004b27e0u,f);
    /* 004a5fa4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a5fa6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5fa9 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5fab */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a5fad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5fae */
    f->r[4]+=4;return;
mi_004a5faf:;
    /* 004a5faf */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5fb1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5fb2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5fb7u;f->boundary(0x004a5fb2u,0x004b27e0u,f);
    /* 004a5fb7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5fba */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004a5fbd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5fbe */
    f->r[4]+=4;return;
mi_004a5fbf:;
    /* 004a5fbf */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5fc3 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a5fc6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a5fc9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a5fcb */
    f->r[6]=(0x00bcd758u+f->r[1]*4u);
    /* 004a5fd2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a5fd9 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004a5fdc */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a5faf;
    /* 004a5fde */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5fe0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5fe1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5fe6u;f->boundary(0x004a5fe1u,0x004b27e0u,f);
    /* 004a5fe6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a5fe8 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a5feb */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a5fed */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a5fef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a5ff0 */
    f->r[4]+=4;return;
mi_004a5ff1:;
    /* 004a5ff1 */
    f->r[2]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a5ff5 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5ff7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a5ff8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a5ffdu;f->boundary(0x004a5ff8u,0x004b27e0u,f);
    /* 004a5ffd */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a6000 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a6001 */
    f->r[4]+=4;return;
mi_004a6002:;
    /* 004a6002 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a6006 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a6009 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a600c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a600e */
    f->r[6]=(0x00bcd758u+f->r[1]*4u);
    /* 004a6015 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a601c */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004a601f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a6066;
    /* 004a6021 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a6023 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a6024 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a6029u;f->boundary(0x004a6024u,0x004b27e0u,f);
    /* 004a6029 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a602b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a602e */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a6030 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a6032 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a6033 */
    f->r[4]+=4;return;
mi_004a6034:;
    /* 004a6034 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a6038 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a603b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004a603e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004a6040 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004a6047 */
    f->r[6]=(0x00bcd758u+f->r[2]*4u);
    /* 004a604e */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004a6051 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a6066;
    /* 004a6053 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a6055 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a6056 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a605bu;f->boundary(0x004a6056u,0x004b27e0u,f);
    /* 004a605b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a605d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a6060 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a6062 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a6064 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a6065 */
    f->r[4]+=4;return;
mi_004a6066:;
    /* 004a6066 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a6068 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a6069 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a606eu;f->boundary(0x004a6069u,0x004b27e0u,f);
    /* 004a606e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a6071 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004a6074 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a6075 */
    f->r[4]+=4;return;
mi_004a6076:;
    /* 004a6076 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a607a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a607d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a6080 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a6082 */
    f->r[6]=(0x00bcd758u+f->r[1]*4u);
    /* 004a6089 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a6090 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004a6092 */
    if(!(f->flags&MI_ZF))goto mi_004a60e9;
    /* 004a6094 */
    goto mi_004a60d6;
mi_004a6096:;
    /* 004a6096 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a609a */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a609d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004a60a0 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004a60a2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 004a60a9 */
    f->r[6]=(0x00bcd758u+f->r[2]*4u);
    /* 004a60b0 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004a60b3 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a60e9;
    /* 004a60b5 */
    goto mi_004a60d6;
mi_004a60b7:;
    /* 004a60b7 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a60bb */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004a60be */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004a60c1 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004a60c3 */
    f->r[6]=(0x00bcd758u+f->r[1]*4u);
    /* 004a60ca */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd758u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004a60d1 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004a60d4 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004a60e9;
mi_004a60d6:;
    /* 004a60d6 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a60d8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a60d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a60deu;f->boundary(0x004a60d9u,0x004b27e0u,f);
    /* 004a60de */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000000u+f->r[6]),1u))&0xffu)<<0);
    /* 004a60e0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004a60e3:;
    /* 004a60e3 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004a60e5 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004a60e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a60e8 */
    f->r[4]+=4;return;
mi_004a60e9:;
    /* 004a60e9 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a60eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a60ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a60f1u;f->boundary(0x004a60ecu,0x004b27e0u,f);
    /* 004a60f1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004a60f4:;
    /* 004a60f4 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004a60f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a60f8 */
    f->r[4]+=4;return;
mi_004a60f9:;
    /* 004a60f9 */
    f->r[2]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004a60fd */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a60ff */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004a6100 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004a6105u;f->boundary(0x004a6100u,0x004b27e0u,f);
    /* 004a6105 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004a6108 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004a6109 */
    f->r[4]+=4;return;
}

void restore_routing_C_004b27e0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b27e0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b27e1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b27e2 */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004b27e6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b27e7 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004b27e9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b27ea */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b2890;
    /* 004b27f0 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004b27f4 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x0000000fu)&0xffu)<<0);
    /* 004b27f6 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b27f9 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b27fc */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b27fe */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
mi_004b2801:;
    /* 004b2801 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd631u+f->r[6]),1u))&0xffu)<<0);
    /* 004b2807 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004b2808 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b280a */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 004b280c */
    MI_U8((0x00bcd631u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b2812 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b2814 */
    if(mi_signed_less(f->flags))goto mi_004b286d;
    /* 004b2816 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2818 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2819 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b281eu;f->boundary(0x004b2819u,0x004baa10u,f);
    /* 004b281e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2821 */
    MI_U8((0x00bcd631u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b2827 */
    a=mi_read((0x00bcd632u+f->r[6]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004b282f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b283a;
    /* 004b2831 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2832 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2837u;f->boundary(0x004b2832u,0x004b2d20u,f);
    /* 004b2837 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004b283a:;
    /* 004b283a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b283b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2840u;f->boundary(0x004b283bu,0x004b33d0u,f);
    /* 004b2840 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2841 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2846u;f->boundary(0x004b2841u,0x004b3530u,f);
    /* 004b2846 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[6]),1u))&0xffu)<<0);
    /* 004b284c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b284f */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b2851 */
    if(!mi_signed_less(f->flags))goto mi_004b2890;
    /* 004b2853 */
    a=mi_read((0x00bcd634u+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd634u+f->r[6]))=(uint16_t)(t);
    /* 004b285a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[6]),1u))&0xffu)<<0);
    /* 004b2860 */
    MI_U8((0x00bcd60au+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b2866 */
    MI_U8((0x00bcd631u+f->r[6]))=(uint8_t)(0x00000000u);
mi_004b286d:;
    /* 004b286d */
    a=mi_read((0x00bcd631u+f->r[6]),1u);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b2874 */
    if(!(f->flags&MI_ZF))goto mi_004b287f;
    /* 004b2876 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2877 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b287cu;f->boundary(0x004b2877u,0x004b3c60u,f);
    /* 004b287c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004b287f:;
    /* 004b287f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2880 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2885u;f->boundary(0x004b2880u,0x004b3f00u,f);
    /* 004b2885 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2888 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004b288a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b2801;
mi_004b2890:;
    /* 004b2890 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b2891 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b2892 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b2893 */
    MI_U32((0x00bcd5e8u))=(uint32_t)(0x00000000u);
    /* 004b289d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b289e */
    f->r[4]+=4;return;
}

void restore_routing_C_00489970(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00489970 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00489974 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489975 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00489977 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489978 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048997b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048997c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048997f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00489981 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00489982 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00bcd632u+f->r[1]*4u),2u))&0xffffu)<<0);
    /* 0048998a */
    f->r[1]=(0x00bcd632u+f->r[1]*4u);
    /* 00489991 */
    a=(f->r[2]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 00489994 */
    if((f->flags&MI_ZF))goto mi_004899c3;
    /* 00489996 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 00489999 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00d04e58u+f->r[2]*2u),2u);
    /* 004899a1 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004899a3 */
    if(!(f->flags&MI_ZF))goto mi_004899c0;
    /* 004899a5 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004899a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004899aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004899ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004899ac */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004899ae */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004899b0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[1]),2u);
    /* 004899b3 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004899b6 */
    MI_U16((0x00d04e58u+f->r[2]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004899be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004899bf */
    f->r[4]+=4;return;
mi_004899c0:;
    /* 004899c0 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[3]&0xffffu));
mi_004899c3:;
    /* 004899c3 */
    f->r[7]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004899c7 */
    f->r[0]=0x00000001u;
    /* 004899cc */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[0]);
mi_004899ce:;
    /* 004899ce */
    f->r[6]=mi_read((0x00000000u+f->r[7]),4u);
    /* 004899d0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00d04e58u+f->r[6]*2u),2u);
    /* 004899d8 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004899da */
    if((f->flags&MI_ZF))goto mi_00489a15;
    /* 004899dc */
    if(mi_signed_less(f->flags))goto mi_00489a39;
    /* 004899de */
    a=f->r[1];b=0x000007d0u;t=mi_alu(f,a,b,32,1);
    /* 004899e4 */
    if(!mi_signed_less(f->flags))goto mi_00489a39;
    /* 004899e6 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004899e9 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004899ec */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004899ee */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004899f1 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd632u+f->r[2]),2u);
    /* 004899f8 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004899fa */
    if(!(f->flags&MI_ZF))goto mi_00489a2a;
    /* 004899fc */
    a=mi_read((0x00bcd606u+f->r[2]),1u);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00489a02 */
    if(!(f->flags&MI_ZF))goto mi_00489a1a;
    /* 00489a04 */
    f->r[1]=(0x00000001u+f->r[6]);
    /* 00489a07 */
    a=f->r[1];b=0x000003e8u;t=mi_alu(f,a,b,32,1);
    /* 00489a0d */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 00489a0f */
    if(mi_signed_less(f->flags))goto mi_004899ce;
    /* 00489a11 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[3]);
    /* 00489a13 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
mi_00489a15:;
    /* 00489a15 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00489a16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00489a17 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00489a18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00489a19 */
    f->r[4]+=4;return;
mi_00489a1a:;
    /* 00489a1a */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00489a1d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00489a20 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00489a22 */
    MI_U16((0x00bcd632u+f->r[2]*4u))=(uint16_t)((f->r[3]&0xffffu));
mi_00489a2a:;
    /* 00489a2a */
    f->r[1]=mi_read((0x00000000u+f->r[7]),4u);
    /* 00489a2c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00489a2d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00489a2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00489a2f */
    MI_U16((0x00d04e58u+f->r[1]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00489a37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00489a38 */
    f->r[4]+=4;return;
mi_00489a39:;
    /* 00489a39 */
    f->r[2]=mi_read((0x00000000u+f->r[7]),4u);
    /* 00489a3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00489a3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00489a3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00489a3e */
    MI_U16((0x00d04e58u+f->r[2]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00489a46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00489a47 */
    f->r[4]+=4;return;
}

void restore_routing_C_004b2d20(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b2d20 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2d21 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2d22 */
    f->r[5]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004b2d26 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2d27 */
    f->r[0]=(0x00000010u+f->r[4]);
    /* 004b2d2b */
    f->r[3]=0x00000001u;
    /* 004b2d30 */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004b2d34 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2d35 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b2d38 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b2d3a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2d3b */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b2d3e */
    MI_U16((0x00bcd632u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 004b2d47 */
    MI_U16((0x00bcd634u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 004b2d50 */
    MI_U16((0x00bcd636u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 004b2d59 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2d5eu;f->boundary(0x004b2d59u,0x00489970u,f);
    /* 004b2d5e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2d61 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b2d63 */
    if((f->flags&MI_ZF))goto mi_004b323d;
    /* 004b2d69 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd66eu+f->r[6]),1u))&0xffu)<<0);
    /* 004b2d6f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b2d71 */
    if((f->flags&MI_ZF))goto mi_004b2ec4;
    /* 004b2d77 */
    a=mi_read((0x00605a3cu),4u);b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b2d7d */
    if(!(f->flags&MI_ZF))goto mi_004b2d96;
    /* 004b2d7f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00784c8eu),2u);
    /* 004b2d86 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00784c8cu),2u);
    /* 004b2d8d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2d8e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2d8f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2d94u;f->boundary(0x004b2d8fu,0x0051d160u,f);
    /* 004b2d94 */
    goto mi_004b2dc6;
mi_004b2d96:;
    /* 004b2d96 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b2d98 */
    if(!(f->flags&MI_ZF))goto mi_004b2db1;
    /* 004b2d9a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2da1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2da8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2da9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2daa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2dafu;f->boundary(0x004b2daau,0x0051d160u,f);
    /* 004b2daf */
    goto mi_004b2dc6;
mi_004b2db1:;
    /* 004b2db1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2db8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2dbf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2dc0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2dc1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2dc6u;f->boundary(0x004b2dc1u,0x0051ce90u,f);
mi_004b2dc6:;
    /* 004b2dc6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd66eu+f->r[6]),1u))&0xffu)<<0);
    /* 004b2dcc */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2dcf */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b2dd1 */
    if(!(f->flags&MI_ZF))goto mi_004b2e03;
    /* 004b2dd3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2dda */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2de1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2de8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2de9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2dea */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2df1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2df2 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004b2df6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2df7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2df8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2df9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2dfeu;f->boundary(0x004b2df9u,0x0051dc10u,f);
    /* 004b2dfe */
    goto mi_004b321c;
mi_004b2e03:;
    /* 004b2e03 */
    a=mi_read((0x00bcd602u+f->r[6]),1u);b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b2e0a */
    if(!(f->flags&MI_ZF))goto mi_004b2e93;
    /* 004b2e10 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 004b2e16 */
    f->r[0]=0x00000009u;
    /* 004b2e1b */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b2e1d */
    if((f->flags&MI_ZF))goto mi_004b2e27;
    /* 004b2e1f */
    a=mi_read((0x00bcd754u+f->r[6]),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b2e25 */
    if(!(f->flags&MI_ZF))goto mi_004b2e93;
mi_004b2e27:;
    /* 004b2e27 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2e2e */
    f->r[0]=mi_read((0x00bcd738u+f->r[6]),4u);
    /* 004b2e34 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2e3b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e3c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e3d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2e44 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2e4b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e4c */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004b2e50 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e51 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e52 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2e58u;f->boundary(0x004b2e53u,0x0051d980u,f);
    /* 004b2e58 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2e5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b2e5d */
    if(!(f->flags&MI_ZF))goto mi_004b3223;
    /* 004b2e63 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2e6a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2e71 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e72 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2e79 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e7a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e7b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2e82 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004b2e86 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e88 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2e89 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2e8eu;f->boundary(0x004b2e89u,0x0051dc10u,f);
    /* 004b2e8e */
    goto mi_004b321c;
mi_004b2e93:;
    /* 004b2e93 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2e9a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2ea1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2ea8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eaa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eab */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2eb2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eb3 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004b2eb7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eb8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eb9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2ebfu;f->boundary(0x004b2ebau,0x0051dc10u,f);
    /* 004b2ebf */
    goto mi_004b321c;
mi_004b2ec4:;
    /* 004b2ec4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd66au+f->r[6]),1u))&0xffu)<<0);
    /* 004b2eca */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b2ecc */
    if(!(f->flags&MI_ZF))goto mi_004b2f2f;
    /* 004b2ece */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2ed5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2edc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2ee3 */
    a=0x000186a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2ee8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eeb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2eec */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2ef3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2ef4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2ef5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2efau;f->boundary(0x004b2ef5u,0x0051a420u,f);
    /* 004b2efa */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2efd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b2eff */
    if(!(f->flags&MI_ZF))goto mi_004b31f0;
    /* 004b2f05 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2f0c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2f13 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2f1a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f1b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f1c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2f23 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f24 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f25 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2f2au;f->boundary(0x004b2f25u,0x0051b040u,f);
    /* 004b2f2a */
    goto mi_004b3187;
mi_004b2f2f:;
    /* 004b2f2f */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b2f31 */
    if(!(f->flags&MI_ZF))goto mi_004b2f5f;
    /* 004b2f33 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2f3a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2f41 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2f48 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f49 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f4a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2f51 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f52 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f53 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b2f55 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2f5au;f->boundary(0x004b2f55u,0x0051a030u,f);
    /* 004b2f5a */
    goto mi_004b3187;
mi_004b2f5f:;
    /* 004b2f5f */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004b2f61 */
    if(!(f->flags&MI_ZF))goto mi_004b2f8d;
    /* 004b2f63 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2f6a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2f71 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2f78 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f79 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f7a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2f81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f82 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2f83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2f88u;f->boundary(0x004b2f83u,0x005181c0u,f);
    /* 004b2f88 */
    goto mi_004b3187;
mi_004b2f8d:;
    /* 004b2f8d */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004b2f8f */
    if(!(f->flags&MI_ZF))goto mi_004b2feb;
    /* 004b2f91 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2f98 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2f9f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2fa6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fa7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fa8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2faf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fb0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fb1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2fb6u;f->boundary(0x004b2fb1u,0x00519840u,f);
    /* 004b2fb6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2fb9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b2fbb */
    if(!(f->flags&MI_ZF))goto mi_004b3196;
    /* 004b2fc1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2fc8 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2fcf */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b2fd6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fd7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fd8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b2fdf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fe0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2fe1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2fe6u;f->boundary(0x004b2fe1u,0x00518880u,f);
    /* 004b2fe6 */
    goto mi_004b3187;
mi_004b2feb:;
    /* 004b2feb */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004b2fed */
    if(!(f->flags&MI_ZF))goto mi_004b3049;
    /* 004b2fef */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b2ff6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2ffd */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3004 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3005 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3006 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b300d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b300e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b300f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3014u;f->boundary(0x004b300fu,0x00519be0u,f);
    /* 004b3014 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3017 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b3019 */
    if(!(f->flags&MI_ZF))goto mi_004b31f0;
    /* 004b301f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b3026 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b302d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3034 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3035 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3036 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b303d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b303e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b303f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3044u;f->boundary(0x004b303fu,0x00517f20u,f);
    /* 004b3044 */
    goto mi_004b3187;
mi_004b3049:;
    /* 004b3049 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b304b */
    if(!(f->flags&MI_ZF))goto mi_004b3079;
    /* 004b304d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b3054 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b305b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3062 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3063 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3064 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b306b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b306c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b306d */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b306f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3074u;f->boundary(0x004b306fu,0x00519840u,f);
    /* 004b3074 */
    goto mi_004b3187;
mi_004b3079:;
    /* 004b3079 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b307b */
    if(!(f->flags&MI_ZF))goto mi_004b30a7;
    /* 004b307d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b3084 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b308b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3092 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3093 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3094 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b309b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b309c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b309d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b30a2u;f->boundary(0x004b309du,0x00517f20u,f);
    /* 004b30a2 */
    goto mi_004b3187;
mi_004b30a7:;
    /* 004b30a7 */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004b30a9 */
    if(!(f->flags&MI_ZF))goto mi_004b30d5;
    /* 004b30ab */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b30b2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b30b9 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b30c0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30c1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30c2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b30c9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30ca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30cb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b30d0u;f->boundary(0x004b30cbu,0x00518400u,f);
    /* 004b30d0 */
    goto mi_004b3187;
mi_004b30d5:;
    /* 004b30d5 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004b30d7 */
    if(!(f->flags&MI_ZF))goto mi_004b3103;
    /* 004b30d9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b30e0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b30e7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b30ee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30ef */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30f0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b30f7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30f8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b30f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b30feu;f->boundary(0x004b30f9u,0x0051d420u,f);
    /* 004b30fe */
    goto mi_004b3187;
mi_004b3103:;
    /* 004b3103 */
    a=(f->r[0]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004b3105 */
    if(!(f->flags&MI_ZF))goto mi_004b315a;
    /* 004b3107 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b310e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b3115 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b311c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b311d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b311e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b3125 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3126 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3127 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b312cu;f->boundary(0x004b3127u,0x005195e0u,f);
    /* 004b312c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b312f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b3131 */
    if(!(f->flags&MI_ZF))goto mi_004b3196;
    /* 004b3133 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b313a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b3141 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3148 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3149 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b314a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b3151 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3152 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3153 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3158u;f->boundary(0x004b3153u,0x00519390u,f);
    /* 004b3158 */
    goto mi_004b3187;
mi_004b315a:;
    /* 004b315a */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004b315c */
    if(!(f->flags&MI_ZF))goto mi_004b323d;
    /* 004b3162 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b3169 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b3170 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3177 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3178 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3179 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b3180 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3181 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3182 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3187u;f->boundary(0x004b3182u,0x005186d0u,f);
mi_004b3187:;
    /* 004b3187 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b318a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b318c */
    if((f->flags&MI_ZF))goto mi_004b323d;
    /* 004b3192 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b3194 */
    if(!(f->flags&MI_ZF))goto mi_004b31f0;
mi_004b3196:;
    /* 004b3196 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b319d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b31a4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b31ab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31ac */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31ad */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b31b4 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004b31b8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31b9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31ba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31bb */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b31c2u;f->boundary(0x004b31bdu,0x0051b400u,f);
    /* 004b31c2 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b31c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b31c7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b3223;
    /* 004b31c9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b31d0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b31d7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b31de */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31df */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31e0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b31e7 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004b31eb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31ec */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31ed */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b31ee */
    goto mi_004b3215;
mi_004b31f0:;
    /* 004b31f0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b31f7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b31fe */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3205 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3206 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3207 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b320e */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004b3212 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3213 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3214 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004b3215:;
    /* 004b3215 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3217 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b321cu;f->boundary(0x004b3217u,0x0051b400u,f);
mi_004b321c:;
    /* 004b321c */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b321f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b3221 */
    if((f->flags&MI_ZF))goto mi_004b323d;
mi_004b3223:;
    /* 004b3223 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004b3227 */
    MI_U16((0x00bcd632u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b322e */
    MI_U16((0x00bcd636u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b3235 */
    MI_U16((0x00d04e58u+f->r[1]*2u))=(uint16_t)((f->r[5]&0xffffu));
mi_004b323d:;
    /* 004b323d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b323e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b323f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3240 */
    f->r[4]+=4;return;
}

void restore_routing_C_0051d160(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051d160 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d161 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d162 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0051d164 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d165 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d16a */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d16f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d174u;f->boundary(0x0051d16fu,0x0040f8f0u,f);
    /* 0051d174 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051d178 */
    a=(f->r[3]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,4);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051d17b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d17e */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051d181 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051d184 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051d187 */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0051d18c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051d18f */
    f->r[2]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0051d193 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051d195 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051d19a */
    a=mi_read((0x00ed4140u+f->r[0]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d1a0 */
    if(!(f->flags&MI_ZF))goto mi_0051d1a7;
    /* 0051d1a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051d1a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d1a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051d1a6 */
    f->r[4]+=4;return;
mi_0051d1a7:;
    /* 0051d1a7 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051d1b2 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 0051d1b8 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051d1be */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051d1c0 */
    f->r[0]=0x00000001u;
    /* 0051d1c5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d1c6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d1c7 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051d1cd */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d1d2:;
    /* 0051d1d2 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051d1d3 */
    a=f->r[7];b=0x0000c350u;t=mi_alu(f,a,b,32,1);
    /* 0051d1d9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051d40d;
    /* 0051d1df */
    f->r[1]=mi_read((0x00f203a0u+f->r[1]*4u),4u);
    /* 0051d1e6 */
    f->r[6]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 0051d1ed */
    f->r[2]=(0xffffff1cu+f->r[1]);
    /* 0051d1f3 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051d1f4 */
    f->r[5]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051d1fb */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051d1fd */
    if(!(f->flags&MI_ZF))goto mi_0051d22e;
    /* 0051d1ff */
    a=mi_read((0x00ed405cu+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d205 */
    if((f->flags&MI_ZF))goto mi_0051d22e;
    /* 0051d207 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051d20e */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d213 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d21a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d21b */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d220 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d225 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d22e;
    /* 0051d227 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d229 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d22e:;
    /* 0051d22e */
    f->r[2]=(0x00000001u+f->r[1]);
    /* 0051d231 */
    f->r[5]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051d238 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051d23a */
    if(!(f->flags&MI_ZF))goto mi_0051d26b;
    /* 0051d23c */
    a=mi_read((0x00ed4141u+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d242 */
    if((f->flags&MI_ZF))goto mi_0051d26b;
    /* 0051d244 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051d24b */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d250 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d257 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d258 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d25d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d262 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d26b;
    /* 0051d264 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d266 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d26b:;
    /* 0051d26b */
    f->r[2]=(0x000000e4u+f->r[1]);
    /* 0051d271 */
    f->r[5]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051d278 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051d27a */
    if(!(f->flags&MI_ZF))goto mi_0051d2ab;
    /* 0051d27c */
    a=mi_read((0x00ed4224u+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d282 */
    if((f->flags&MI_ZF))goto mi_0051d2ab;
    /* 0051d284 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051d28b */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d290 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d297 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d298 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d29d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d2a2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d2ab;
    /* 0051d2a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d2a6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d2ab:;
    /* 0051d2ab */
    f->r[2]=mi_read((0x00eed75cu+f->r[1]*4u),4u);
    /* 0051d2b2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051d2b4 */
    if(!(f->flags&MI_ZF))goto mi_0051d2e8;
    /* 0051d2b6 */
    a=mi_read((0x00ed413fu+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d2bc */
    if((f->flags&MI_ZF))goto mi_0051d2e8;
    /* 0051d2be */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0051d2c5 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d2ca */
    f->r[2]=(0xffffffffu+f->r[1]);
    /* 0051d2cd */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d2d4 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d2d5 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d2da */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d2df */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d2e8;
    /* 0051d2e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d2e3 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d2e8:;
    /* 0051d2e8 */
    f->r[2]=(0xffffff1du+f->r[1]);
    /* 0051d2ee */
    f->r[5]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051d2f5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051d2f7 */
    if(!(f->flags&MI_ZF))goto mi_0051d328;
    /* 0051d2f9 */
    a=mi_read((0x00ed405du+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d2ff */
    if((f->flags&MI_ZF))goto mi_0051d328;
    /* 0051d301 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051d308 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d30d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d314 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d315 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d31a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d31f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d328;
    /* 0051d321 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d323 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d328:;
    /* 0051d328 */
    f->r[2]=(0x000000e5u+f->r[1]);
    /* 0051d32e */
    f->r[5]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051d335 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051d337 */
    if(!(f->flags&MI_ZF))goto mi_0051d368;
    /* 0051d339 */
    a=mi_read((0x00ed4225u+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d33f */
    if((f->flags&MI_ZF))goto mi_0051d368;
    /* 0051d341 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051d348 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d34d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d354 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d355 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d35a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d35f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d368;
    /* 0051d361 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d363 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d368:;
    /* 0051d368 */
    f->r[2]=(0x000000e3u+f->r[1]);
    /* 0051d36e */
    f->r[5]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051d375 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051d377 */
    if(!(f->flags&MI_ZF))goto mi_0051d3a8;
    /* 0051d379 */
    a=mi_read((0x00ed4223u+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d37f */
    if((f->flags&MI_ZF))goto mi_0051d3a8;
    /* 0051d381 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051d388 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d38d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d394 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d395 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d39a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d39f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d3a8;
    /* 0051d3a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d3a3 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d3a8:;
    /* 0051d3a8 */
    f->r[2]=(0xffffff1bu+f->r[1]);
    /* 0051d3ae */
    f->r[5]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051d3b5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051d3b7 */
    if(!(f->flags&MI_ZF))goto mi_0051d3e8;
    /* 0051d3b9 */
    a=mi_read((0x00ed405bu+f->r[1]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051d3bf */
    if((f->flags&MI_ZF))goto mi_0051d3e8;
    /* 0051d3c1 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051d3c8 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d3cd */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051d3d4 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d3d5 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d3da */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d3df */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d3e8;
    /* 0051d3e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d3e3 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d3e8:;
    /* 0051d3e8 */
    f->r[1]=mi_read((0x00eae00cu),4u);
    /* 0051d3ee */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051d3ef */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d3f5 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051d3fb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d405;
    /* 0051d3fd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051d3ff */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
mi_0051d405:;
    /* 0051d405 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051d407 */
    if(!(f->flags&MI_ZF))goto mi_0051d1d2;
mi_0051d40d:;
    /* 0051d40d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051d40e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051d40f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051d410 */
    f->r[0]=0x00000001u;
    /* 0051d415 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051d416 */
    f->r[4]+=4;return;
}

void restore_routing_C_0051b400(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051b400 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0051b403 */
    f->r[0]=mi_read((0x00000028u+f->r[4]),4u);
    /* 0051b407 */
    f->r[1]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0051b40b */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051b40f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b410 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051b413 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b414 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b415 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b416 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0051b419 */
    f->r[2]=mi_read((0x00bca2d8u),4u);
    /* 0051b41f */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051b423 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051b426 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0051b429 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051b42b */
    f->r[7]=f->r[0];
    /* 0051b42d */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051b432 */
    f->r[0]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 0051b439 */
    f->r[1]=(0x00eed760u+f->r[7]*4u);
    /* 0051b440 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051b442 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051b44e;
mi_0051b444:;
    /* 0051b444 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051b445 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051b446 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051b447 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b449 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051b44a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051b44d */
    f->r[4]+=4;return;
mi_0051b44e:;
    /* 0051b44e */
    f->r[3]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0051b452 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051b454 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0051b457 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 0051b45f */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051b462 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b463 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0051b465 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051b468 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b46c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051b470 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b63f;
    /* 0051b476 */
    f->r[6]=f->r[1];
mi_0051b478:;
    /* 0051b478 */
    f->r[0]=mi_read((0x00000030u+f->r[4]),4u);
    /* 0051b47c */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0051b480 */
    f->r[2]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b484 */
    f->r[5]=mi_read((0x00000000u+f->r[6]),4u);
    /* 0051b486 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b487 */
    f->r[0]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b48b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b48c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b48d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b48e */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051b492 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0051b495 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051b49au;f->boundary(0x0051b495u,0x0051cba0u,f);
    /* 0051b49a */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0051b49f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051b4a2 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051b4a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b4a8 */
    f->r[2]=0x005f80fcu;
mi_0051b4ad:;
    /* 0051b4ad */
    a=f->r[0];b=mi_read((0x00000014u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051b4b1 */
    if((f->flags&MI_ZF))goto mi_0051b4f5;
    /* 0051b4b3 */
    f->r[1]=mi_read((0x00000000u+f->r[2]),4u);
    /* 0051b4b5 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051b4b7 */
    f->r[1]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 0051b4be */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051b4c0 */
    if((f->flags&MI_ZF))goto mi_0051b4f5;
    /* 0051b4c2 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051b4c4 */
    if(!mi_signed_less(f->flags))goto mi_0051b4d0;
    /* 0051b4c6 */
    f->r[5]=f->r[1];
    /* 0051b4c8 */
    f->r[3]=f->r[0];
    /* 0051b4ca */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051b4ce */
    goto mi_0051b4f5;
mi_0051b4d0:;
    /* 0051b4d0 */
    if(!(f->flags&MI_ZF))goto mi_0051b4f5;
    /* 0051b4d2 */
    a=f->r[0];b=mi_read((0x00000018u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051b4d6 */
    if(!(f->flags&MI_ZF))goto mi_0051b4e2;
    /* 0051b4d8 */
    f->r[5]=f->r[1];
    /* 0051b4da */
    f->r[3]=f->r[0];
    /* 0051b4dc */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051b4e0 */
    goto mi_0051b4f5;
mi_0051b4e2:;
    /* 0051b4e2 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0051b4e5 */
    if(!(f->flags&MI_ZF))goto mi_0051b4f1;
    /* 0051b4e7 */
    f->r[5]=f->r[1];
    /* 0051b4e9 */
    f->r[3]=f->r[0];
    /* 0051b4eb */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051b4ef */
    goto mi_0051b4f5;
mi_0051b4f1:;
    /* 0051b4f1 */
    f->r[5]=mi_read((0x00000024u+f->r[4]),4u);
mi_0051b4f5:;
    /* 0051b4f5 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051b4f9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051b4fb */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0051b4fe */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051b500 */
    a=f->r[2];b=0x005f811cu;t=mi_alu(f,a,b,32,1);
    /* 0051b506 */
    if(mi_signed_less(f->flags))goto mi_0051b4ad;
    /* 0051b508 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0051b50b */
    if((f->flags&MI_ZF))goto mi_0051b444;
    /* 0051b511 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051b513 */
    if(!(f->flags&MI_ZF))goto mi_0051b52f;
    /* 0051b515 */
    f->r[2]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b519 */
    a=f->r[7];b=0x000000e4u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0051b51f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b520 */
    a=f->r[6];b=0x00000390u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051b526 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b52a */
    goto mi_0051b60b;
mi_0051b52f:;
    /* 0051b52f */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051b532 */
    if(!(f->flags&MI_ZF))goto mi_0051b557;
    /* 0051b534 */
    f->r[0]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b538 */
    f->r[2]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051b53c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b53d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b53e */
    a=f->r[7];b=0x000000e3u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0051b544 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051b548 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b54c */
    a=f->r[6];b=0x0000038cu;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051b552 */
    goto mi_0051b60b;
mi_0051b557:;
    /* 0051b557 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0051b55a */
    if(!(f->flags&MI_ZF))goto mi_0051b56e;
    /* 0051b55c */
    f->r[2]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051b560 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b561 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051b562 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b566 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051b569 */
    goto mi_0051b60b;
mi_0051b56e:;
    /* 0051b56e */
    a=f->r[3];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0051b571 */
    if(!(f->flags&MI_ZF))goto mi_0051b593;
    /* 0051b573 */
    f->r[0]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051b577 */
    f->r[2]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b57b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b57c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b57d */
    a=f->r[7];b=0x000000e5u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051b583 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051b587 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b58b */
    a=f->r[6];b=0x00000394u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051b591 */
    goto mi_0051b60b;
mi_0051b593:;
    /* 0051b593 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0051b596 */
    if(!(f->flags&MI_ZF))goto mi_0051b5af;
    /* 0051b598 */
    f->r[2]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b59c */
    a=f->r[7];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051b5a2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b5a3 */
    a=f->r[6];b=0x00000390u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051b5a9 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b5ad */
    goto mi_0051b60b;
mi_0051b5af:;
    /* 0051b5af */
    a=f->r[3];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0051b5b2 */
    if(!(f->flags&MI_ZF))goto mi_0051b5d4;
    /* 0051b5b4 */
    f->r[0]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b5b8 */
    f->r[2]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051b5bc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b5bd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b5be */
    a=f->r[7];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051b5c4 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051b5c8 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b5cc */
    a=f->r[6];b=0x0000038cu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051b5d2 */
    goto mi_0051b60b;
mi_0051b5d4:;
    /* 0051b5d4 */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0051b5d7 */
    if(!(f->flags&MI_ZF))goto mi_0051b5e8;
    /* 0051b5d9 */
    f->r[2]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051b5dd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b5de */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051b5df */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b5e3 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051b5e6 */
    goto mi_0051b60b;
mi_0051b5e8:;
    /* 0051b5e8 */
    a=f->r[3];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0051b5eb */
    if(!(f->flags&MI_ZF))goto mi_0051b60b;
    /* 0051b5ed */
    f->r[0]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051b5f1 */
    f->r[2]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051b5f5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b5f6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b5f7 */
    a=f->r[7];b=0x000000e5u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0051b5fd */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051b601 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051b605 */
    a=f->r[6];b=0x00000394u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
mi_0051b60b:;
    /* 0051b60b */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0051b60e */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0051b611 */
    if(mi_signed_less(f->flags))goto mi_0051b616;
    /* 0051b613 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[3]=t;
mi_0051b616:;
    /* 0051b616 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051b61a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0051b61e */
    MI_U8((0x00f533c8u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0051b624 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b625 */
    a=f->r[0];b=0x000001f4u;t=mi_alu(f,a,b,32,1);
    /* 0051b62a */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051b62e */
    if(!mi_signed_less(f->flags))goto mi_0051b444;
    /* 0051b634 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051b637 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051b478;
    /* 0051b63d */
    f->r[5]=f->r[0];
mi_0051b63f:;
    /* 0051b63f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b641 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051b643 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b667;
    /* 0051b645 */
    f->r[2]=mi_read((0x00000028u+f->r[4]),4u);
    /* 0051b649 */
    f->r[1]=(0x00f533c7u+f->r[5]);
    /* 0051b64f */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 0051b652 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 0051b655 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[2]*4u);
mi_0051b658:;
    /* 0051b658 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000000u+f->r[1]),1u))&0xffu)<<0);
    /* 0051b65a */
    MI_U8((0x00c8ad38u+f->r[0]+f->r[6]*4u))=(uint8_t)((f->r[2]&0xffu));
    /* 0051b661 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b662 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051b663 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051b665 */
    if(mi_signed_less(f->flags))goto mi_0051b658;
mi_0051b667:;
    /* 0051b667 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051b668 */
    f->r[0]=f->r[5];
    /* 0051b66a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051b66b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051b66c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051b66d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051b670 */
    f->r[4]+=4;return;
}

void restore_routing_C_0051d980(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051d980 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0051d983 */
    f->r[2]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0051d987 */
    f->r[1]=mi_read((0x00000028u+f->r[4]),4u);
    /* 0051d98b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d98c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d98d */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 0051d990 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d991 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051d997 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051d99b */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 0051d99e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d99f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051d9a3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051d9a6 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 0051d9a9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051d9ab */
    f->r[6]=f->r[0];
    /* 0051d9ad */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051d9b2 */
    f->r[0]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051d9b9 */
    f->r[1]=(0x00eed760u+f->r[6]*4u);
    /* 0051d9c0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d9c2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051d9ce;
mi_0051d9c4:;
    /* 0051d9c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051d9c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051d9c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051d9c7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d9c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051d9ca */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d9cd */
    f->r[4]+=4;return;
mi_0051d9ce:;
    /* 0051d9ce */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0051d9d0 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051d9d3 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051d9d7 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 0051d9df */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051dbd5;
    /* 0051d9e5 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051d9e9 */
    goto mi_0051d9ef;
mi_0051d9eb:;
    /* 0051d9eb */
    f->r[2]=mi_read((0x0000003cu+f->r[4]),4u);
mi_0051d9ef:;
    /* 0051d9ef */
    f->r[0]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051d9f3 */
    f->r[7]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051d9f7 */
    f->r[1]=mi_read((0x00000030u+f->r[4]),4u);
    /* 0051d9fb */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0051d9fe */
    a=f->r[5];b=mi_read((0x00000040u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051da02 */
    f->r[7]=mi_read((0x00000000u+f->r[7]),4u);
    /* 0051da04 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051da05 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051da06 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051da10;
    /* 0051da08 */
    f->r[1]=mi_read((0x00000040u+f->r[4]),4u);
    /* 0051da0c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051da0d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051da0e */
    goto mi_0051da1a;
mi_0051da10:;
    /* 0051da10 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051da14 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051da18 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051da19 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0051da1a:;
    /* 0051da1a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051da1fu;f->boundary(0x0051da1au,0x0051cba0u,f);
    /* 0051da1f */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0051da24 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051da27 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051da2b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051da2d */
    f->r[2]=0x005f80fcu;
mi_0051da32:;
    /* 0051da32 */
    a=f->r[1];b=mi_read((0x00000024u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051da36 */
    if((f->flags&MI_ZF))goto mi_0051da72;
    /* 0051da38 */
    f->r[0]=mi_read((0x00000000u+f->r[2]),4u);
    /* 0051da3a */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051da3c */
    f->r[0]=mi_read((0x00eed760u+f->r[0]*4u),4u);
    /* 0051da43 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051da45 */
    if((f->flags&MI_ZF))goto mi_0051da72;
    /* 0051da47 */
    f->r[5]=mi_read((0x00000040u+f->r[4]),4u);
    /* 0051da4b */
    a=mi_read((0x00000020u+f->r[4]),4u);b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051da4f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051da5d;
    /* 0051da51 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0051da53 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051da72;
    /* 0051da55 */
    a=f->r[1];b=mi_read((0x0000001cu+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051da59 */
    if(!(f->flags&MI_ZF))goto mi_0051da72;
    /* 0051da5b */
    goto mi_0051da6e;
mi_0051da5d:;
    /* 0051da5d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0051da5f */
    if(mi_signed_less(f->flags))goto mi_0051da6e;
    /* 0051da61 */
    if(!(f->flags&MI_ZF))goto mi_0051da72;
    /* 0051da63 */
    a=f->r[1];b=mi_read((0x0000001cu+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051da67 */
    if((f->flags&MI_ZF))goto mi_0051da6e;
    /* 0051da69 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0051da6c */
    if(!(f->flags&MI_ZF))goto mi_0051da72;
mi_0051da6e:;
    /* 0051da6e */
    f->r[7]=f->r[0];
    /* 0051da70 */
    f->r[3]=f->r[1];
mi_0051da72:;
    /* 0051da72 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051da75 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051da76 */
    a=f->r[2];b=0x005f811cu;t=mi_alu(f,a,b,32,1);
    /* 0051da7c */
    if(mi_signed_less(f->flags))goto mi_0051da32;
    /* 0051da7e */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0051da81 */
    if((f->flags&MI_ZF))goto mi_0051d9c4;
    /* 0051da87 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051da89 */
    if(!(f->flags&MI_ZF))goto mi_0051daa8;
    /* 0051da8b */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051da8f */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051da93 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051da94 */
    a=f->r[6];b=0x000000e4u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051da9a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051da9e */
    a=f->r[0];b=0x00000390u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0051daa3 */
    goto mi_0051db9e;
mi_0051daa8:;
    /* 0051daa8 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051daab */
    if(!(f->flags&MI_ZF))goto mi_0051dad3;
    /* 0051daad */
    f->r[5]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051dab1 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051dab5 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051dab9 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051daba */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051dabb */
    a=f->r[6];b=0x000000e3u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051dac1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051dac5 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051dac9 */
    a=f->r[0];b=0x0000038cu;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0051dace */
    goto mi_0051db9e;
mi_0051dad3:;
    /* 0051dad3 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0051dad6 */
    if(!(f->flags&MI_ZF))goto mi_0051daee;
    /* 0051dad8 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051dadc */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051dae0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051dae1 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051dae2 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051dae6 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051dae9 */
    goto mi_0051db9e;
mi_0051daee:;
    /* 0051daee */
    a=f->r[3];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0051daf1 */
    if(!(f->flags&MI_ZF))goto mi_0051db19;
    /* 0051daf3 */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051daf7 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051dafb */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051daff */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051db00 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051db01 */
    a=f->r[6];b=0x000000e5u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051db07 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051db0b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051db0f */
    a=f->r[0];b=0x00000394u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051db14 */
    goto mi_0051db9e;
mi_0051db19:;
    /* 0051db19 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0051db1c */
    if(!(f->flags&MI_ZF))goto mi_0051db38;
    /* 0051db1e */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051db22 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051db26 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051db27 */
    a=f->r[6];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051db2d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051db31 */
    a=f->r[0];b=0x00000390u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051db36 */
    goto mi_0051db9e;
mi_0051db38:;
    /* 0051db38 */
    a=f->r[3];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0051db3b */
    if(!(f->flags&MI_ZF))goto mi_0051db60;
    /* 0051db3d */
    f->r[5]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051db41 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051db45 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051db49 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051db4a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051db4b */
    a=f->r[6];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0051db51 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051db55 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051db59 */
    a=f->r[0];b=0x0000038cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051db5e */
    goto mi_0051db9e;
mi_0051db60:;
    /* 0051db60 */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0051db63 */
    if(!(f->flags&MI_ZF))goto mi_0051db78;
    /* 0051db65 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051db69 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051db6d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051db6e */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051db6f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051db73 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0051db76 */
    goto mi_0051db9e;
mi_0051db78:;
    /* 0051db78 */
    a=f->r[3];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0051db7b */
    if(!(f->flags&MI_ZF))goto mi_0051dba2;
    /* 0051db7d */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051db81 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051db85 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051db89 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051db8a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051db8b */
    a=f->r[6];b=0x000000e5u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051db91 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051db95 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051db99 */
    a=f->r[0];b=0x00000394u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0051db9e:;
    /* 0051db9e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0051dba2:;
    /* 0051dba2 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0051dba5 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0051dba8 */
    if(mi_signed_less(f->flags))goto mi_0051dbad;
    /* 0051dbaa */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[3]=t;
mi_0051dbad:;
    /* 0051dbad */
    f->r[5]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051dbb1 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0051dbb5 */
    MI_U8((0x00f533c8u+f->r[5]))=(uint8_t)((f->r[3]&0xffu));
    /* 0051dbbb */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051dbbc */
    a=f->r[5];b=0x000001f4u;t=mi_alu(f,a,b,32,1);
    /* 0051dbc2 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0051dbc6 */
    if(!mi_signed_less(f->flags))goto mi_0051d9c4;
    /* 0051dbcc */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051dbcf */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051d9eb;
mi_0051dbd5:;
    /* 0051dbd5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051dbd7 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051dbd9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051dbfd;
    /* 0051dbdb */
    f->r[2]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0051dbdf */
    f->r[1]=(0x00f533c7u+f->r[5]);
    /* 0051dbe5 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 0051dbe8 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 0051dbeb */
    f->r[6]=(0x00000000u+f->r[2]+f->r[2]*4u);
mi_0051dbee:;
    /* 0051dbee */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000000u+f->r[1]),1u))&0xffu)<<0);
    /* 0051dbf0 */
    MI_U8((0x00c8ad38u+f->r[0]+f->r[6]*4u))=(uint8_t)((f->r[2]&0xffu));
    /* 0051dbf7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051dbf8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051dbf9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051dbfb */
    if(mi_signed_less(f->flags))goto mi_0051dbee;
mi_0051dbfd:;
    /* 0051dbfd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051dbfe */
    f->r[0]=f->r[5];
    /* 0051dc00 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051dc01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051dc02 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051dc03 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051dc06 */
    f->r[4]+=4;return;
}

void restore_routing_C_0051dc10(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051dc10 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0051dc13 */
    f->r[2]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051dc17 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dc18 */
    f->r[3]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051dc1c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dc1d */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 0051dc20 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dc21 */
    f->r[6]=mi_read((0x00d3b374u),4u);
    /* 0051dc27 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dc28 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 0051dc2b */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0051dc2e */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0051dc32 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051dc35 */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0051dc3a */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 0051dc3d */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051dc3f */
    f->r[5]=f->r[0];
    /* 0051dc41 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051dc46 */
    f->r[0]=mi_read((0x00eed760u+f->r[5]*4u),4u);
    /* 0051dc4d */
    f->r[1]=(0x00eed760u+f->r[5]*4u);
    /* 0051dc54 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051dc56 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051dc62;
mi_0051dc58:;
    /* 0051dc58 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051dc59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051dc5a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051dc5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051dc5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051dc5e */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051dc61 */
    f->r[4]+=4;return;
mi_0051dc62:;
    /* 0051dc62 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0051dc64 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051dc67 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0051dc6b */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 0051dc73 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051de36;
    /* 0051dc79 */
    f->r[0]=(0x00af598cu+f->r[5]*4u);
    /* 0051dc80 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051dc84 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051dc88 */
    goto mi_0051dc92;
mi_0051dc8a:;
    /* 0051dc8a */
    f->r[2]=mi_read((0x00000044u+f->r[4]),4u);
    /* 0051dc8e */
    f->r[6]=mi_read((0x00000028u+f->r[4]),4u);
mi_0051dc92:;
    /* 0051dc92 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051dc96 */
    f->r[0]=mi_read((0x00000000u+f->r[1]),4u);
    /* 0051dc98 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051dc9c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051dca0 */
    f->r[0]=mi_read((0x00000000u+f->r[1]),4u);
    /* 0051dca2 */
    a=f->r[0];t=a>>16u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>15u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0051dca5 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051dca7 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 0051dcab */
    f->r[0]=mi_read((0x00000048u+f->r[4]),4u);
    /* 0051dcaf */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051dcb2 */
    if(!(f->flags&MI_ZF))goto mi_0051dcc3;
    /* 0051dcb4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00acf85cu+f->r[5]),1u))&0xffu)<<0);
    /* 0051dcba */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0051dcbd */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051dcc1 */
    goto mi_0051dcc7;
mi_0051dcc3:;
    /* 0051dcc3 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
mi_0051dcc7:;
    /* 0051dcc7 */
    f->r[1]=mi_read((0x0000003cu+f->r[4]),4u);
    /* 0051dccb */
    f->r[0]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0051dccf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dcd0 */
    f->r[1]=mi_read((0x00000044u+f->r[4]),4u);
    /* 0051dcd4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dcd5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dcd6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051dcd7 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0051dcda */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051dcdfu;f->boundary(0x0051dcdau,0x0051cba0u,f);
    /* 0051dcdf */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051dce2 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0051dce4 */
    f->r[7]=0x005f80fcu;
mi_0051dce9:;
    /* 0051dce9 */
    a=f->r[6];b=mi_read((0x0000002cu+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051dced */
    if((f->flags&MI_ZF))goto mi_0051dd40;
    /* 0051dcef */
    f->r[0]=mi_read((0x00000000u+f->r[7]),4u);
    /* 0051dcf1 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051dcf3 */
    f->r[1]=mi_read((0x00eed760u+f->r[0]*4u),4u);
    /* 0051dcfa */
    f->r[0]=mi_read((0x00af598cu+f->r[0]*4u),4u);
    /* 0051dd01 */
    a=f->r[0];t=a>>16u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>15u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0051dd04 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051dd06 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051dd08 */
    if((f->flags&MI_ZF))goto mi_0051dd40;
    /* 0051dd0a */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051dd0e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0051dd10 */
    if(mi_signed_less(f->flags))goto mi_0051dd36;
    /* 0051dd12 */
    if(!(f->flags&MI_ZF))goto mi_0051dd40;
    /* 0051dd14 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 0051dd18 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051dd1a */
    if((f->flags&MI_ZF))goto mi_0051dd2c;
    /* 0051dd1c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051dd1e */
    if(!(f->flags&MI_ZF))goto mi_0051dd2c;
    /* 0051dd20 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051dd24 */
    f->r[3]=f->r[6];
    /* 0051dd26 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 0051dd2a */
    goto mi_0051dd40;
mi_0051dd2c:;
    /* 0051dd2c */
    f->r[2]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0051dd30 */
    a=mi_read((0x00000028u+f->r[4]),4u);b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0051dd34 */
    if(!(f->flags&MI_ZF))goto mi_0051dd40;
mi_0051dd36:;
    /* 0051dd36 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051dd3a */
    f->r[3]=f->r[6];
    /* 0051dd3c */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
mi_0051dd40:;
    /* 0051dd40 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051dd43 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051dd44 */
    a=f->r[7];b=0x005f811cu;t=mi_alu(f,a,b,32,1);
    /* 0051dd4a */
    if(mi_signed_less(f->flags))goto mi_0051dce9;
    /* 0051dd4c */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0051dd4f */
    if((f->flags&MI_ZF))goto mi_0051dc58;
    /* 0051dd55 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051dd57 */
    if(!(f->flags&MI_ZF))goto mi_0051dd69;
    /* 0051dd59 */
    a=f->r[5];b=0x000000e4u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0051dd5f */
    f->r[0]=0x00000390u;
    /* 0051dd64 */
    goto mi_0051ddea;
mi_0051dd69:;
    /* 0051dd69 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051dd6c */
    if(!(f->flags&MI_ZF))goto mi_0051dd7b;
    /* 0051dd6e */
    a=f->r[5];b=0x000000e3u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0051dd74 */
    f->r[0]=0x0000038cu;
    /* 0051dd79 */
    goto mi_0051ddea;
mi_0051dd7b:;
    /* 0051dd7b */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0051dd7e */
    if(!(f->flags&MI_ZF))goto mi_0051dd88;
    /* 0051dd80 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051dd81 */
    f->r[0]=0x00000004u;
    /* 0051dd86 */
    goto mi_0051ddc0;
mi_0051dd88:;
    /* 0051dd88 */
    a=f->r[3];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0051dd8b */
    if(!(f->flags&MI_ZF))goto mi_0051dd9a;
    /* 0051dd8d */
    a=f->r[5];b=0x000000e5u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0051dd93 */
    f->r[0]=0x00000394u;
    /* 0051dd98 */
    goto mi_0051ddc0;
mi_0051dd9a:;
    /* 0051dd9a */
    f->r[0]=0x00000004u;
    /* 0051dd9f */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051dda1 */
    if(!(f->flags&MI_ZF))goto mi_0051ddb0;
    /* 0051dda3 */
    a=f->r[5];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0051dda9 */
    f->r[0]=0x00000390u;
    /* 0051ddae */
    goto mi_0051ddc0;
mi_0051ddb0:;
    /* 0051ddb0 */
    a=f->r[3];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0051ddb3 */
    if(!(f->flags&MI_ZF))goto mi_0051ddd2;
    /* 0051ddb5 */
    a=f->r[5];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0051ddbb */
    f->r[0]=0x0000038cu;
mi_0051ddc0:;
    /* 0051ddc0 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051ddc4 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051ddc8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0051ddca */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051ddcc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0051ddd0 */
    goto mi_0051ddfa;
mi_0051ddd2:;
    /* 0051ddd2 */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0051ddd5 */
    if(!(f->flags&MI_ZF))goto mi_0051ddda;
    /* 0051ddd7 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051ddd8 */
    goto mi_0051ddea;
mi_0051ddda:;
    /* 0051ddda */
    a=f->r[3];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0051dddd */
    if(!(f->flags&MI_ZF))goto mi_0051ddfe;
    /* 0051dddf */
    a=f->r[5];b=0x000000e5u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0051dde5 */
    f->r[0]=0x00000394u;
mi_0051ddea:;
    /* 0051ddea */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0051ddee */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051ddf2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0051ddf4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0051ddf6 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
mi_0051ddfa:;
    /* 0051ddfa */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051ddfe:;
    /* 0051ddfe */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0051de01 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0051de04 */
    if(mi_signed_less(f->flags))goto mi_0051de09;
    /* 0051de06 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[3]=t;
mi_0051de09:;
    /* 0051de09 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051de0d */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0051de11 */
    MI_U8((0x00f533c8u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0051de17 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051de18 */
    a=f->r[0];b=0x000001f4u;t=mi_alu(f,a,b,32,1);
    /* 0051de1d */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0051de21 */
    if(!mi_signed_less(f->flags))goto mi_0051dc58;
    /* 0051de27 */
    a=mi_read((0x0000001cu+f->r[4]),4u);b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0051de2c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051dc8a;
    /* 0051de32 */
    f->r[7]=mi_read((0x00000020u+f->r[4]),4u);
mi_0051de36:;
    /* 0051de36 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051de38 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0051de3a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051de5e;
    /* 0051de3c */
    f->r[2]=mi_read((0x00000034u+f->r[4]),4u);
    /* 0051de40 */
    f->r[1]=(0x00f533c7u+f->r[7]);
    /* 0051de46 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 0051de49 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 0051de4c */
    f->r[6]=(0x00000000u+f->r[2]+f->r[2]*4u);
mi_0051de4f:;
    /* 0051de4f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000000u+f->r[1]),1u))&0xffu)<<0);
    /* 0051de51 */
    MI_U8((0x00c8ad38u+f->r[0]+f->r[6]*4u))=(uint8_t)((f->r[2]&0xffu));
    /* 0051de58 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051de59 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051de5a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0051de5c */
    if(mi_signed_less(f->flags))goto mi_0051de4f;
mi_0051de5e:;
    /* 0051de5e */
    f->r[0]=f->r[7];
    /* 0051de60 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051de61 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051de62 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051de63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051de64 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051de67 */
    f->r[4]+=4;return;
}

void restore_routing_C_00517f20(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00517f20 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517f21 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517f22 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517f23 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517f24 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517f26 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517f2b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517f30 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00517f35u;f->boundary(0x00517f30u,0x0040f8f0u,f);
    /* 00517f35 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00517f39 */
    f->r[3]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00517f3d */
    f->r[7]=mi_read((0x00eadfd4u),4u);
    /* 00517f43 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00517f46 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00517f49 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00517f4c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00517f4f */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 00517f54 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00517f57 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00517f59 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00517f5b */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00517f5c */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00517f61 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 00517f67 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00517f72 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 00517f78 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 00517f7e */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00517f82 */
    f->r[0]=0x00000001u;
    /* 00517f87 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 00517f8d */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00517f90 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00517f95 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 00517f98 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 00517f9e */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00517fa1 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00517fa4 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00517fa8 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00517faa */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00517fae:;
    /* 00517fae */
    f->r[7]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 00517fb5 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00517fb7 */
    if((f->flags&MI_ZF))goto mi_0051819f;
    /* 00517fbd */
    f->r[3]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 00517fc4 */
    f->r[5]=(0x00eed760u+f->r[7]*4u);
    /* 00517fcb */
    f->r[6]=(0xffffff1cu+f->r[7]);
    /* 00517fd1 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00517fd2 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00517fd9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00517fdb */
    if(!(f->flags&MI_ZF))goto mi_0051803a;
    /* 00517fdd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00517fde */
    f->r[4]-=4;MI_U32(f->r[4])=0x00517fe3u;f->boundary(0x00517fdeu,0x0051aff0u,f);
    /* 00517fe3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00517fe6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00517fe8 */
    if((f->flags&MI_ZF))goto mi_00518035;
    /* 00517fea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[7]),1u))&0xffu)<<0);
    /* 00517ff0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00517ff2 */
    if(!mi_signed_less(f->flags))goto mi_00518008;
    /* 00517ff4 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00517ff6 */
    if((f->flags&MI_ZF))goto mi_00518008;
    /* 00517ff8 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00517ffa */
    if(!(f->flags&MI_ZF))goto mi_00518035;
    /* 00517ffc */
    f->r[1]=mi_read((0x00af598cu+f->r[6]*4u),4u);
    /* 00518003 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518006 */
    if((f->flags&MI_ZF))goto mi_00518035;
mi_00518008:;
    /* 00518008 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051800a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518035;
    /* 0051800c */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518013 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00518018 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051801f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518020 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518025 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051802a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051803a;
    /* 0051802c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051802e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518033 */
    goto mi_0051803a;
mi_00518035:;
    /* 00518035 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051803a:;
    /* 0051803a */
    f->r[6]=(0x00000001u+f->r[7]);
    /* 0051803d */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00518044 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518046 */
    if(!(f->flags&MI_ZF))goto mi_005180a5;
    /* 00518048 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518049 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051804eu;f->boundary(0x00518049u,0x0051aff0u,f);
    /* 0051804e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518051 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518053 */
    if((f->flags&MI_ZF))goto mi_005180a0;
    /* 00518055 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[7]),1u))&0xffu)<<0);
    /* 0051805b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051805d */
    if(!mi_signed_less(f->flags))goto mi_00518073;
    /* 0051805f */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518061 */
    if((f->flags&MI_ZF))goto mi_00518073;
    /* 00518063 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518065 */
    if(!(f->flags&MI_ZF))goto mi_005180a0;
    /* 00518067 */
    f->r[1]=mi_read((0x00af5990u+f->r[7]*4u),4u);
    /* 0051806e */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518071 */
    if((f->flags&MI_ZF))goto mi_005180a0;
mi_00518073:;
    /* 00518073 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518075 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005180a0;
    /* 00518077 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0051807e */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00518083 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051808a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051808b */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518090 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518095 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005180a5;
    /* 00518097 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518099 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051809e */
    goto mi_005180a5;
mi_005180a0:;
    /* 005180a0 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_005180a5:;
    /* 005180a5 */
    f->r[6]=(0x000000e4u+f->r[7]);
    /* 005180ab */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 005180b2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005180b4 */
    if(!(f->flags&MI_ZF))goto mi_00518113;
    /* 005180b6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005180b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005180bcu;f->boundary(0x005180b7u,0x0051aff0u,f);
    /* 005180bc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005180bf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005180c1 */
    if((f->flags&MI_ZF))goto mi_0051810e;
    /* 005180c3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[7]),1u))&0xffu)<<0);
    /* 005180c9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005180cb */
    if(!mi_signed_less(f->flags))goto mi_005180e1;
    /* 005180cd */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 005180cf */
    if((f->flags&MI_ZF))goto mi_005180e1;
    /* 005180d1 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 005180d3 */
    if(!(f->flags&MI_ZF))goto mi_0051810e;
    /* 005180d5 */
    f->r[1]=mi_read((0x00af5d1cu+f->r[7]*4u),4u);
    /* 005180dc */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 005180df */
    if((f->flags&MI_ZF))goto mi_0051810e;
mi_005180e1:;
    /* 005180e1 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 005180e3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051810e;
    /* 005180e5 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 005180ec */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005180f1 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 005180f8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005180f9 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005180fe */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518103 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518113;
    /* 00518105 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518107 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051810c */
    goto mi_00518113;
mi_0051810e:;
    /* 0051810e */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_00518113:;
    /* 00518113 */
    f->r[1]=mi_read((0xfffffffcu+f->r[5]),4u);
    /* 00518116 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518118 */
    if(!(f->flags&MI_ZF))goto mi_00518176;
    /* 0051811a */
    f->r[6]=(0xffffffffu+f->r[7]);
    /* 0051811d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051811e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518123u;f->boundary(0x0051811eu,0x0051aff0u,f);
    /* 00518123 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518126 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518128 */
    if((f->flags&MI_ZF))goto mi_00518171;
    /* 0051812a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[7]),1u))&0xffu)<<0);
    /* 00518130 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518132 */
    if(!mi_signed_less(f->flags))goto mi_00518148;
    /* 00518134 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518136 */
    if((f->flags&MI_ZF))goto mi_00518148;
    /* 00518138 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051813a */
    if(!(f->flags&MI_ZF))goto mi_00518171;
    /* 0051813c */
    f->r[1]=mi_read((0x00af5988u+f->r[7]*4u),4u);
    /* 00518143 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518146 */
    if((f->flags&MI_ZF))goto mi_00518171;
mi_00518148:;
    /* 00518148 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051814a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518171;
    /* 0051814c */
    MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(f->r[3]);
    /* 0051814f */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00518154 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051815b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051815c */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518161 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518166 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518176;
    /* 00518168 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051816a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051816f */
    goto mi_00518176;
mi_00518171:;
    /* 00518171 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_00518176:;
    /* 00518176 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 0051817c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051817d */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518183 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 00518189 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518193;
    /* 0051818b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051818d */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_00518193:;
    /* 00518193 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00518197 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00518199 */
    if(!(f->flags&MI_ZF))goto mi_00517fae;
mi_0051819f:;
    /* 0051819f */
    f->r[2]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 005181a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005181a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005181a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005181aa */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005181ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005181ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005181ae */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 005181b1 */
    f->r[4]+=4;return;
}

void restore_routing_C_005181c0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005181c0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005181c1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005181c2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005181c3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005181c4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005181c6 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005181cb */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005181d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005181d5u;f->boundary(0x005181d0u,0x0040f8f0u,f);
    /* 005181d5 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 005181d9 */
    f->r[3]=mi_read((0x00000020u+f->r[4]),4u);
    /* 005181dd */
    f->r[7]=mi_read((0x00eadfd4u),4u);
    /* 005181e3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005181e6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 005181e9 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 005181ec */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005181ef */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 005181f4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 005181f7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 005181f9 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005181fb */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 005181fc */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00518201 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 00518207 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00518212 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 00518218 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051821e */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00518222 */
    f->r[0]=0x00000001u;
    /* 00518227 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051822d */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00518230 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518235 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 00518238 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051823e */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00518241 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00518244 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00518248 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051824a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051824e:;
    /* 0051824e */
    f->r[6]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 00518255 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00518257 */
    if((f->flags&MI_ZF))goto mi_005183df;
    /* 0051825d */
    f->r[5]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00518264 */
    f->r[2]=(0xffffff1cu+f->r[6]);
    /* 0051826a */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051826b */
    f->r[1]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 00518272 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518274 */
    if(!(f->flags&MI_ZF))goto mi_005182b6;
    /* 00518276 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[6]),1u))&0xffu)<<0);
    /* 0051827c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051827e */
    if(!mi_signed_less(f->flags))goto mi_00518285;
    /* 00518280 */
    a=(f->r[1]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518283 */
    if(!(f->flags&MI_ZF))goto mi_005182b6;
mi_00518285:;
    /* 00518285 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00518288 */
    if((f->flags&MI_ZF))goto mi_005182b6;
    /* 0051828a */
    a=(f->r[1]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051828d */
    if((f->flags&MI_ZF))goto mi_005182b6;
    /* 0051828f */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[5]);
    /* 00518296 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051829b */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 005182a2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005182a3 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005182a8 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005182ad */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005182b6;
    /* 005182af */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005182b1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_005182b6:;
    /* 005182b6 */
    f->r[2]=(0x00000001u+f->r[6]);
    /* 005182b9 */
    f->r[1]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 005182c0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005182c2 */
    if(!(f->flags&MI_ZF))goto mi_00518304;
    /* 005182c4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[6]),1u))&0xffu)<<0);
    /* 005182ca */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005182cc */
    if(!mi_signed_less(f->flags))goto mi_005182d3;
    /* 005182ce */
    a=(f->r[1]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 005182d1 */
    if(!(f->flags&MI_ZF))goto mi_00518304;
mi_005182d3:;
    /* 005182d3 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 005182d6 */
    if((f->flags&MI_ZF))goto mi_00518304;
    /* 005182d8 */
    a=(f->r[1]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 005182db */
    if((f->flags&MI_ZF))goto mi_00518304;
    /* 005182dd */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[5]);
    /* 005182e4 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005182e9 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 005182f0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005182f1 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005182f6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005182fb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518304;
    /* 005182fd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005182ff */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00518304:;
    /* 00518304 */
    f->r[2]=(0x000000e4u+f->r[6]);
    /* 0051830a */
    f->r[1]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 00518311 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518313 */
    if(!(f->flags&MI_ZF))goto mi_00518355;
    /* 00518315 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[6]),1u))&0xffu)<<0);
    /* 0051831b */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051831d */
    if(!mi_signed_less(f->flags))goto mi_00518324;
    /* 0051831f */
    a=(f->r[1]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518322 */
    if(!(f->flags&MI_ZF))goto mi_00518355;
mi_00518324:;
    /* 00518324 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00518327 */
    if((f->flags&MI_ZF))goto mi_00518355;
    /* 00518329 */
    a=(f->r[1]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051832c */
    if((f->flags&MI_ZF))goto mi_00518355;
    /* 0051832e */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[5]);
    /* 00518335 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051833a */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 00518341 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518342 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518347 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051834c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518355;
    /* 0051834e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518350 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00518355:;
    /* 00518355 */
    f->r[1]=mi_read((0x00eed75cu+f->r[6]*4u),4u);
    /* 0051835c */
    f->r[7]=(0x00eed75cu+f->r[6]*4u);
    /* 00518363 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518365 */
    if(!(f->flags&MI_ZF))goto mi_005183b6;
    /* 00518367 */
    f->r[3]=(0xffffffffu+f->r[6]);
    /* 0051836a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051836b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518370u;f->boundary(0x0051836bu,0x0051aff0u,f);
    /* 00518370 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518373 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518375 */
    if((f->flags&MI_ZF))goto mi_005183b1;
    /* 00518377 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[6]),1u))&0xffu)<<0);
    /* 0051837d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051837f */
    if(!mi_signed_less(f->flags))goto mi_00518385;
    /* 00518381 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518383 */
    if(!(f->flags&MI_ZF))goto mi_005183b1;
mi_00518385:;
    /* 00518385 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00518387 */
    if((f->flags&MI_ZF))goto mi_005183b1;
    /* 00518389 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051838b */
    if((f->flags&MI_ZF))goto mi_005183b1;
    /* 0051838d */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 0051838f */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00518394 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 0051839b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051839c */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005183a1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005183a6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005183b6;
    /* 005183a8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005183aa */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005183af */
    goto mi_005183b6;
mi_005183b1:;
    /* 005183b1 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_005183b6:;
    /* 005183b6 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 005183bc */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005183bd */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005183c3 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 005183c9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005183d3;
    /* 005183cb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 005183cd */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_005183d3:;
    /* 005183d3 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 005183d7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 005183d9 */
    if(!(f->flags&MI_ZF))goto mi_0051824e;
mi_005183df:;
    /* 005183df */
    f->r[2]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 005183e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005183e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005183e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005183ea */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005183ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005183ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005183ee */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 005183f1 */
    f->r[4]+=4;return;
}

void restore_routing_C_005186d0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005186d0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005186d1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005186d2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005186d3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005186d5 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005186da */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005186df */
    f->r[4]-=4;MI_U32(f->r[4])=0x005186e4u;f->boundary(0x005186dfu,0x0040f8f0u,f);
    /* 005186e4 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 005186e8 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 005186ec */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005186ef */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 005186f2 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 005186f5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005186f8 */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 005186fd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00518700 */
    f->r[2]=mi_read((0x00eadfd4u),4u);
    /* 00518706 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00518708 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00518709 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051870e */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[2]);
    /* 00518714 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051871f */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 00518725 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051872b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051872f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00518731 */
    f->r[0]=0x00000001u;
    /* 00518736 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00518739 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051873f */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518744 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 00518747 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051874d */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00518750 */
    f->r[7]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00518753 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00518757 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_00518759:;
    /* 00518759 */
    f->r[6]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 00518760 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00518762 */
    if((f->flags&MI_ZF))goto mi_0051886d;
    /* 00518768 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051876f */
    f->r[2]=(0xffffff1cu+f->r[6]);
    /* 00518775 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00518776 */
    f->r[3]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051877d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051877f */
    if(!(f->flags&MI_ZF))goto mi_005187a8;
    /* 00518781 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[1]);
    /* 00518788 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051878d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 00518794 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518795 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051879a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051879f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005187a8;
    /* 005187a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005187a3 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_005187a8:;
    /* 005187a8 */
    f->r[2]=(0x00000001u+f->r[6]);
    /* 005187ab */
    f->r[3]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 005187b2 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 005187b4 */
    if(!(f->flags&MI_ZF))goto mi_005187dd;
    /* 005187b6 */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[1]);
    /* 005187bd */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005187c2 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 005187c9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005187ca */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005187cf */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005187d4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005187dd;
    /* 005187d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005187d8 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_005187dd:;
    /* 005187dd */
    f->r[2]=(0x000000e4u+f->r[6]);
    /* 005187e3 */
    f->r[3]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 005187ea */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 005187ec */
    if(!(f->flags&MI_ZF))goto mi_00518815;
    /* 005187ee */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[1]);
    /* 005187f5 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005187fa */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 00518801 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518802 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518807 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051880c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518815;
    /* 0051880e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518810 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00518815:;
    /* 00518815 */
    f->r[2]=mi_read((0x00eed75cu+f->r[6]*4u),4u);
    /* 0051881c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051881e */
    if(!(f->flags&MI_ZF))goto mi_00518848;
    /* 00518820 */
    MI_U32((0x00eed75cu+f->r[6]*4u))=(uint32_t)(f->r[1]);
    /* 00518827 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051882c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051882d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518834 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518835 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051883a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051883f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518848;
    /* 00518841 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518843 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00518848:;
    /* 00518848 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 0051884e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051884f */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518855 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051885b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518865;
    /* 0051885d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051885f */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_00518865:;
    /* 00518865 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00518867 */
    if(!(f->flags&MI_ZF))goto mi_00518759;
mi_0051886d:;
    /* 0051886d */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 00518874 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518876 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518878 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00518879 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051887a */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051887d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051887e */
    f->r[4]+=4;return;
}

void restore_routing_C_00518880(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00518880 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518881 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518882 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518883 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518884 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518886 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051888b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518890 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518895u;f->boundary(0x00518890u,0x0040f8f0u,f);
    /* 00518895 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00518899 */
    f->r[3]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051889d */
    f->r[7]=mi_read((0x00eadfd4u),4u);
    /* 005188a3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005188a6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 005188a9 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 005188ac */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005188af */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 005188b4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 005188b7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 005188b9 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005188bb */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 005188bc */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 005188c1 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 005188c7 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 005188d2 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 005188d8 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 005188de */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 005188e2 */
    f->r[0]=0x00000001u;
    /* 005188e7 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 005188ed */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 005188f0 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005188f5 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 005188f8 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 005188fe */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00518901 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00518904 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00518908 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051890a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051890e:;
    /* 0051890e */
    f->r[7]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 00518915 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00518917 */
    if((f->flags&MI_ZF))goto mi_00518b0f;
    /* 0051891d */
    f->r[3]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 00518924 */
    f->r[5]=(0x00eed760u+f->r[7]*4u);
    /* 0051892b */
    f->r[6]=(0xffffff1cu+f->r[7]);
    /* 00518931 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00518932 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00518939 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051893b */
    if(!(f->flags&MI_ZF))goto mi_0051899e;
    /* 0051893d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051893e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518943u;f->boundary(0x0051893eu,0x0051aff0u,f);
    /* 00518943 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518946 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518948 */
    if((f->flags&MI_ZF))goto mi_00518999;
    /* 0051894a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[7]),1u))&0xffu)<<0);
    /* 00518950 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518952 */
    if(!mi_signed_less(f->flags))goto mi_00518968;
    /* 00518954 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518956 */
    if((f->flags&MI_ZF))goto mi_00518968;
    /* 00518958 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051895a */
    if(!(f->flags&MI_ZF))goto mi_00518999;
    /* 0051895c */
    f->r[1]=mi_read((0x00af598cu+f->r[6]*4u),4u);
    /* 00518963 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518966 */
    if((f->flags&MI_ZF))goto mi_00518999;
mi_00518968:;
    /* 00518968 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051896a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518999;
    /* 0051896c */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051896e */
    if((f->flags&MI_ZF))goto mi_00518999;
    /* 00518970 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518977 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051897c */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518983 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518984 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518989 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051898e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051899e;
    /* 00518990 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518992 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518997 */
    goto mi_0051899e;
mi_00518999:;
    /* 00518999 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051899e:;
    /* 0051899e */
    f->r[6]=(0x00000001u+f->r[7]);
    /* 005189a1 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 005189a8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005189aa */
    if(!(f->flags&MI_ZF))goto mi_00518a0d;
    /* 005189ac */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005189ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x005189b2u;f->boundary(0x005189adu,0x0051aff0u,f);
    /* 005189b2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005189b5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005189b7 */
    if((f->flags&MI_ZF))goto mi_00518a08;
    /* 005189b9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[7]),1u))&0xffu)<<0);
    /* 005189bf */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005189c1 */
    if(!mi_signed_less(f->flags))goto mi_005189d7;
    /* 005189c3 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 005189c5 */
    if((f->flags&MI_ZF))goto mi_005189d7;
    /* 005189c7 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 005189c9 */
    if(!(f->flags&MI_ZF))goto mi_00518a08;
    /* 005189cb */
    f->r[1]=mi_read((0x00af5990u+f->r[7]*4u),4u);
    /* 005189d2 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 005189d5 */
    if((f->flags&MI_ZF))goto mi_00518a08;
mi_005189d7:;
    /* 005189d7 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 005189d9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518a08;
    /* 005189db */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 005189dd */
    if((f->flags&MI_ZF))goto mi_00518a08;
    /* 005189df */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 005189e6 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005189eb */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 005189f2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005189f3 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005189f8 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005189fd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518a0d;
    /* 005189ff */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518a01 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518a06 */
    goto mi_00518a0d;
mi_00518a08:;
    /* 00518a08 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_00518a0d:;
    /* 00518a0d */
    f->r[6]=(0x000000e4u+f->r[7]);
    /* 00518a13 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00518a1a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518a1c */
    if(!(f->flags&MI_ZF))goto mi_00518a7f;
    /* 00518a1e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518a1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518a24u;f->boundary(0x00518a1fu,0x0051aff0u,f);
    /* 00518a24 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518a27 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518a29 */
    if((f->flags&MI_ZF))goto mi_00518a7a;
    /* 00518a2b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[7]),1u))&0xffu)<<0);
    /* 00518a31 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518a33 */
    if(!mi_signed_less(f->flags))goto mi_00518a49;
    /* 00518a35 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518a37 */
    if((f->flags&MI_ZF))goto mi_00518a49;
    /* 00518a39 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518a3b */
    if(!(f->flags&MI_ZF))goto mi_00518a7a;
    /* 00518a3d */
    f->r[1]=mi_read((0x00af5d1cu+f->r[7]*4u),4u);
    /* 00518a44 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518a47 */
    if((f->flags&MI_ZF))goto mi_00518a7a;
mi_00518a49:;
    /* 00518a49 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518a4b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518a7a;
    /* 00518a4d */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00518a4f */
    if((f->flags&MI_ZF))goto mi_00518a7a;
    /* 00518a51 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518a58 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00518a5d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518a64 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518a65 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518a6a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518a6f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518a7f;
    /* 00518a71 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518a73 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518a78 */
    goto mi_00518a7f;
mi_00518a7a:;
    /* 00518a7a */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_00518a7f:;
    /* 00518a7f */
    f->r[1]=mi_read((0xfffffffcu+f->r[5]),4u);
    /* 00518a82 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518a84 */
    if(!(f->flags&MI_ZF))goto mi_00518ae6;
    /* 00518a86 */
    f->r[6]=(0xffffffffu+f->r[7]);
    /* 00518a89 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518a8a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518a8fu;f->boundary(0x00518a8au,0x0051aff0u,f);
    /* 00518a8f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518a92 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518a94 */
    if((f->flags&MI_ZF))goto mi_00518ae1;
    /* 00518a96 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[7]),1u))&0xffu)<<0);
    /* 00518a9c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518a9e */
    if(!mi_signed_less(f->flags))goto mi_00518ab4;
    /* 00518aa0 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518aa2 */
    if((f->flags&MI_ZF))goto mi_00518ab4;
    /* 00518aa4 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518aa6 */
    if(!(f->flags&MI_ZF))goto mi_00518ae1;
    /* 00518aa8 */
    f->r[1]=mi_read((0x00af5988u+f->r[7]*4u),4u);
    /* 00518aaf */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518ab2 */
    if((f->flags&MI_ZF))goto mi_00518ae1;
mi_00518ab4:;
    /* 00518ab4 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518ab6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00518ae1;
    /* 00518ab8 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00518aba */
    if((f->flags&MI_ZF))goto mi_00518ae1;
    /* 00518abc */
    MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(f->r[3]);
    /* 00518abf */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00518ac4 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518acb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518acc */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518ad1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518ad6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518ae6;
    /* 00518ad8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518ada */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518adf */
    goto mi_00518ae6;
mi_00518ae1:;
    /* 00518ae1 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_00518ae6:;
    /* 00518ae6 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 00518aec */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00518aed */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518af3 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 00518af9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518b03;
    /* 00518afb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00518afd */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_00518b03:;
    /* 00518b03 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00518b07 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00518b09 */
    if(!(f->flags&MI_ZF))goto mi_0051890e;
mi_00518b0f:;
    /* 00518b0f */
    f->r[2]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 00518b16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518b18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00518b19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00518b1a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00518b1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00518b1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00518b1e */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00518b21 */
    f->r[4]+=4;return;
}

void restore_routing_C_00519390(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00519390 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519391 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519392 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519393 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519394 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519396 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051939b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005193a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005193a5u;f->boundary(0x005193a0u,0x0040f8f0u,f);
    /* 005193a5 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 005193a9 */
    f->r[3]=mi_read((0x00000020u+f->r[4]),4u);
    /* 005193ad */
    f->r[7]=mi_read((0x00eadfd4u),4u);
    /* 005193b3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005193b6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 005193b9 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 005193bc */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005193bf */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 005193c4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 005193c7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 005193c9 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005193cb */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 005193cc */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 005193d1 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 005193d7 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 005193e2 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 005193e8 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 005193ee */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 005193f2 */
    f->r[0]=0x00000001u;
    /* 005193f7 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 005193fd */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00519400 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519405 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 00519408 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051940e */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00519411 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00519414 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00519418 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051941a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051941e:;
    /* 0051941e */
    f->r[6]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 00519425 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00519427 */
    if((f->flags&MI_ZF))goto mi_005195c2;
    /* 0051942d */
    f->r[5]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00519434 */
    f->r[7]=(0xffffff1cu+f->r[6]);
    /* 0051943a */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051943b */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 00519442 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00519444 */
    if(!(f->flags&MI_ZF))goto mi_0051948f;
    /* 00519446 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519447 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051944cu;f->boundary(0x00519447u,0x0051aff0u,f);
    /* 0051944c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051944f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00519451 */
    if((f->flags&MI_ZF))goto mi_0051948a;
    /* 00519453 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[6]),1u))&0xffu)<<0);
    /* 00519459 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051945b */
    if(mi_signed_less(f->flags))goto mi_0051948a;
    /* 0051945d */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051945f */
    if((f->flags&MI_ZF))goto mi_0051948a;
    /* 00519461 */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 00519468 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051946d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 00519474 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519475 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051947a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051947f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051948f;
    /* 00519481 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519483 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519488 */
    goto mi_0051948f;
mi_0051948a:;
    /* 0051948a */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051948f:;
    /* 0051948f */
    f->r[7]=(0x00000001u+f->r[6]);
    /* 00519492 */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 00519499 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051949b */
    if(!(f->flags&MI_ZF))goto mi_005194e6;
    /* 0051949d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051949e */
    f->r[4]-=4;MI_U32(f->r[4])=0x005194a3u;f->boundary(0x0051949eu,0x0051aff0u,f);
    /* 005194a3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005194a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005194a8 */
    if((f->flags&MI_ZF))goto mi_005194e1;
    /* 005194aa */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[6]),1u))&0xffu)<<0);
    /* 005194b0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005194b2 */
    if(mi_signed_less(f->flags))goto mi_005194e1;
    /* 005194b4 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 005194b6 */
    if((f->flags&MI_ZF))goto mi_005194e1;
    /* 005194b8 */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 005194bf */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005194c4 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 005194cb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005194cc */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005194d1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005194d6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005194e6;
    /* 005194d8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005194da */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005194df */
    goto mi_005194e6;
mi_005194e1:;
    /* 005194e1 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_005194e6:;
    /* 005194e6 */
    f->r[7]=(0x000000e4u+f->r[6]);
    /* 005194ec */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 005194f3 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005194f5 */
    if(!(f->flags&MI_ZF))goto mi_00519540;
    /* 005194f7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005194f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005194fdu;f->boundary(0x005194f8u,0x0051aff0u,f);
    /* 005194fd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00519500 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00519502 */
    if((f->flags&MI_ZF))goto mi_0051953b;
    /* 00519504 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[6]),1u))&0xffu)<<0);
    /* 0051950a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051950c */
    if(mi_signed_less(f->flags))goto mi_0051953b;
    /* 0051950e */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00519510 */
    if((f->flags&MI_ZF))goto mi_0051953b;
    /* 00519512 */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 00519519 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051951e */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 00519525 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519526 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051952b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519530 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519540;
    /* 00519532 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519534 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519539 */
    goto mi_00519540;
mi_0051953b:;
    /* 0051953b */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_00519540:;
    /* 00519540 */
    f->r[1]=mi_read((0x00eed75cu+f->r[6]*4u),4u);
    /* 00519547 */
    f->r[7]=(0x00eed75cu+f->r[6]*4u);
    /* 0051954e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00519550 */
    if(!(f->flags&MI_ZF))goto mi_00519599;
    /* 00519552 */
    f->r[3]=(0xffffffffu+f->r[6]);
    /* 00519555 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519556 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051955bu;f->boundary(0x00519556u,0x0051aff0u,f);
    /* 0051955b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051955e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00519560 */
    if((f->flags&MI_ZF))goto mi_00519594;
    /* 00519562 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[6]),1u))&0xffu)<<0);
    /* 00519568 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051956a */
    if(mi_signed_less(f->flags))goto mi_00519594;
    /* 0051956c */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051956e */
    if((f->flags&MI_ZF))goto mi_00519594;
    /* 00519570 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 00519572 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00519577 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 0051957e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051957f */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519584 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519589 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519599;
    /* 0051958b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051958d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519592 */
    goto mi_00519599;
mi_00519594:;
    /* 00519594 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_00519599:;
    /* 00519599 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 0051959f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005195a0 */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005195a6 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 005195ac */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005195b6;
    /* 005195ae */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 005195b0 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_005195b6:;
    /* 005195b6 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 005195ba */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 005195bc */
    if(!(f->flags&MI_ZF))goto mi_0051941e;
mi_005195c2:;
    /* 005195c2 */
    f->r[2]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 005195c9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005195cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005195cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005195cd */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005195cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005195d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005195d1 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 005195d4 */
    f->r[4]+=4;return;
}

void restore_routing_C_005195e0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005195e0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005195e1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005195e2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005195e3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005195e4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005195e6 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005195eb */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005195f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005195f5u;f->boundary(0x005195f0u,0x0040f8f0u,f);
    /* 005195f5 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 005195f9 */
    f->r[3]=mi_read((0x00000020u+f->r[4]),4u);
    /* 005195fd */
    f->r[7]=mi_read((0x00eadfd4u),4u);
    /* 00519603 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00519606 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00519609 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051960c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051960f */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 00519614 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00519617 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00519619 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051961b */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051961c */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00519621 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 00519627 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00519632 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 00519638 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051963e */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00519642 */
    f->r[0]=0x00000001u;
    /* 00519647 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051964d */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00519650 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519655 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 00519658 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051965e */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00519661 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00519664 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00519668 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051966a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051966e:;
    /* 0051966e */
    f->r[6]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 00519675 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00519677 */
    if((f->flags&MI_ZF))goto mi_00519822;
    /* 0051967d */
    f->r[5]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00519684 */
    f->r[7]=(0xffffff1cu+f->r[6]);
    /* 0051968a */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051968b */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 00519692 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00519694 */
    if(!(f->flags&MI_ZF))goto mi_005196e3;
    /* 00519696 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519697 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051969cu;f->boundary(0x00519697u,0x0051aff0u,f);
    /* 0051969c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051969f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005196a1 */
    if((f->flags&MI_ZF))goto mi_005196de;
    /* 005196a3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[6]),1u))&0xffu)<<0);
    /* 005196a9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005196ab */
    if(mi_signed_less(f->flags))goto mi_005196de;
    /* 005196ad */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 005196af */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005196b5;
    /* 005196b1 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 005196b3 */
    if(!(f->flags&MI_ZF))goto mi_005196de;
mi_005196b5:;
    /* 005196b5 */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 005196bc */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005196c1 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 005196c8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005196c9 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005196ce */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005196d3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005196e3;
    /* 005196d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005196d7 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005196dc */
    goto mi_005196e3;
mi_005196de:;
    /* 005196de */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_005196e3:;
    /* 005196e3 */
    f->r[7]=(0x00000001u+f->r[6]);
    /* 005196e6 */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 005196ed */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005196ef */
    if(!(f->flags&MI_ZF))goto mi_0051973e;
    /* 005196f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005196f2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005196f7u;f->boundary(0x005196f2u,0x0051aff0u,f);
    /* 005196f7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005196fa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005196fc */
    if((f->flags&MI_ZF))goto mi_00519739;
    /* 005196fe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[6]),1u))&0xffu)<<0);
    /* 00519704 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519706 */
    if(mi_signed_less(f->flags))goto mi_00519739;
    /* 00519708 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051970a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519710;
    /* 0051970c */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051970e */
    if(!(f->flags&MI_ZF))goto mi_00519739;
mi_00519710:;
    /* 00519710 */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 00519717 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051971c */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 00519723 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519724 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519729 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051972e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051973e;
    /* 00519730 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519732 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519737 */
    goto mi_0051973e;
mi_00519739:;
    /* 00519739 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051973e:;
    /* 0051973e */
    f->r[7]=(0x000000e4u+f->r[6]);
    /* 00519744 */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 0051974b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051974d */
    if(!(f->flags&MI_ZF))goto mi_0051979c;
    /* 0051974f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519750 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00519755u;f->boundary(0x00519750u,0x0051aff0u,f);
    /* 00519755 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00519758 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051975a */
    if((f->flags&MI_ZF))goto mi_00519797;
    /* 0051975c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[6]),1u))&0xffu)<<0);
    /* 00519762 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519764 */
    if(mi_signed_less(f->flags))goto mi_00519797;
    /* 00519766 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00519768 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051976e;
    /* 0051976a */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051976c */
    if(!(f->flags&MI_ZF))goto mi_00519797;
mi_0051976e:;
    /* 0051976e */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 00519775 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051977a */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 00519781 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519782 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519787 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051978c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051979c;
    /* 0051978e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519790 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519795 */
    goto mi_0051979c;
mi_00519797:;
    /* 00519797 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051979c:;
    /* 0051979c */
    f->r[1]=mi_read((0x00eed75cu+f->r[6]*4u),4u);
    /* 005197a3 */
    f->r[7]=(0x00eed75cu+f->r[6]*4u);
    /* 005197aa */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005197ac */
    if(!(f->flags&MI_ZF))goto mi_005197f9;
    /* 005197ae */
    f->r[3]=(0xffffffffu+f->r[6]);
    /* 005197b1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005197b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005197b7u;f->boundary(0x005197b2u,0x0051aff0u,f);
    /* 005197b7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005197ba */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005197bc */
    if((f->flags&MI_ZF))goto mi_005197f4;
    /* 005197be */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[6]),1u))&0xffu)<<0);
    /* 005197c4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005197c6 */
    if(mi_signed_less(f->flags))goto mi_005197f4;
    /* 005197c8 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 005197ca */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005197d0;
    /* 005197cc */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 005197ce */
    if(!(f->flags&MI_ZF))goto mi_005197f4;
mi_005197d0:;
    /* 005197d0 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 005197d2 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005197d7 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 005197de */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005197df */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005197e4 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005197e9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005197f9;
    /* 005197eb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005197ed */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005197f2 */
    goto mi_005197f9;
mi_005197f4:;
    /* 005197f4 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_005197f9:;
    /* 005197f9 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 005197ff */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00519800 */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519806 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051980c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519816;
    /* 0051980e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00519810 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_00519816:;
    /* 00519816 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051981a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051981c */
    if(!(f->flags&MI_ZF))goto mi_0051966e;
mi_00519822:;
    /* 00519822 */
    f->r[2]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 00519829 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051982b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051982c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051982d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051982f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00519830 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00519831 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00519834 */
    f->r[4]+=4;return;
}

void restore_routing_C_00519840(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00519840 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519841 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519842 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519843 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519845 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051984a */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051984f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00519854u;f->boundary(0x0051984fu,0x0040f8f0u,f);
    /* 00519854 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00519858 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051985c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051985f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00519862 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00519865 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00519868 */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0051986d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00519870 */
    f->r[2]=mi_read((0x00eadfd4u),4u);
    /* 00519876 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00519878 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00519879 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051987e */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[2]);
    /* 00519884 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051988f */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 00519895 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051989b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051989f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 005198a1 */
    f->r[0]=0x00000001u;
    /* 005198a6 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 005198a9 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 005198af */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005198b4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 005198b7 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 005198bd */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 005198c0 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 005198c3 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 005198c7 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_005198c9:;
    /* 005198c9 */
    f->r[1]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 005198d0 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 005198d2 */
    if((f->flags&MI_ZF))goto mi_00519a19;
    /* 005198d8 */
    f->r[7]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 005198df */
    f->r[6]=(0xffffff1cu+f->r[1]);
    /* 005198e5 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 005198e6 */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 005198ed */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005198ef */
    if(!(f->flags&MI_ZF))goto mi_00519927;
    /* 005198f1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[1]),1u))&0xffu)<<0);
    /* 005198f7 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005198f9 */
    if(mi_signed_less(f->flags))goto mi_00519927;
    /* 005198fb */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 005198fe */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00519927;
    /* 00519900 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519907 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051990c */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519913 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519914 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519919 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051991e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519927;
    /* 00519920 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519922 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519927:;
    /* 00519927 */
    f->r[6]=(0x00000001u+f->r[1]);
    /* 0051992a */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00519931 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519933 */
    if(!(f->flags&MI_ZF))goto mi_0051996b;
    /* 00519935 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[1]),1u))&0xffu)<<0);
    /* 0051993b */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051993d */
    if(mi_signed_less(f->flags))goto mi_0051996b;
    /* 0051993f */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00519942 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051996b;
    /* 00519944 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0051994b */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00519950 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519957 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519958 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051995d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519962 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051996b;
    /* 00519964 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519966 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051996b:;
    /* 0051996b */
    f->r[6]=(0x000000e4u+f->r[1]);
    /* 00519971 */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00519978 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051997a */
    if(!(f->flags&MI_ZF))goto mi_005199b2;
    /* 0051997c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[1]),1u))&0xffu)<<0);
    /* 00519982 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519984 */
    if(mi_signed_less(f->flags))goto mi_005199b2;
    /* 00519986 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00519989 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005199b2;
    /* 0051998b */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519992 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00519997 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051999e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051999f */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005199a4 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005199a9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005199b2;
    /* 005199ab */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005199ad */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_005199b2:;
    /* 005199b2 */
    f->r[2]=mi_read((0x00eed75cu+f->r[1]*4u),4u);
    /* 005199b9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005199bb */
    if(!(f->flags&MI_ZF))goto mi_005199f4;
    /* 005199bd */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[1]),1u))&0xffu)<<0);
    /* 005199c3 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005199c5 */
    if(mi_signed_less(f->flags))goto mi_005199f4;
    /* 005199c7 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 005199ca */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005199f4;
    /* 005199cc */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[7]);
    /* 005199d3 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 005199d8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 005199d9 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 005199e0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005199e1 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005199e6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005199eb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005199f4;
    /* 005199ed */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005199ef */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_005199f4:;
    /* 005199f4 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 005199fa */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005199fb */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519a01 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 00519a07 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519a11;
    /* 00519a09 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00519a0b */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_00519a11:;
    /* 00519a11 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00519a13 */
    if(!(f->flags&MI_ZF))goto mi_005198c9;
mi_00519a19:;
    /* 00519a19 */
    f->r[1]=mi_read((0x00eed760u+f->r[3]*4u),4u);
    /* 00519a20 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519a22 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00519a24 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00519a25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00519a26 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00519a29 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00519a2a */
    f->r[4]+=4;return;
}

void restore_routing_C_00519be0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00519be0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519be1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519be2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519be3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519be4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519be6 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519beb */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519bf0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00519bf5u;f->boundary(0x00519bf0u,0x0040f8f0u,f);
    /* 00519bf5 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00519bf9 */
    f->r[6]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00519bfd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00519c00 */
    f->r[5]=0x00010004u;
    /* 00519c05 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00519c08 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00519c0b */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00519c0e */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 00519c13 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00519c16 */
    f->r[2]=mi_read((0x00eadfd4u),4u);
    /* 00519c1c */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00519c1e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00519c1f */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00519c24 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[2]);
    /* 00519c2a */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00519c35 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 00519c3b */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 00519c41 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00519c45 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00519c47 */
    f->r[0]=0x00000001u;
    /* 00519c4c */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00519c4f */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 00519c55 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519c5a */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 00519c5d */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 00519c63 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00519c66 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00519c69 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00519c6d */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_00519c6f:;
    /* 00519c6f */
    f->r[1]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 00519c76 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00519c78 */
    if((f->flags&MI_ZF))goto mi_00519e17;
    /* 00519c7e */
    f->r[7]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 00519c85 */
    f->r[6]=(0xffffff1cu+f->r[1]);
    /* 00519c8b */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00519c8c */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00519c93 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519c95 */
    if(!(f->flags&MI_ZF))goto mi_00519ce1;
    /* 00519c97 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[1]),1u))&0xffu)<<0);
    /* 00519c9d */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519c9f */
    if(!mi_signed_less(f->flags))goto mi_00519cba;
    /* 00519ca1 */
    a=(f->r[2]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00519ca4 */
    if(!(f->flags&MI_ZF))goto mi_00519ce1;
    /* 00519ca6 */
    a=mi_read((0x00af598cu+f->r[6]*4u),4u);b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00519cad */
    if((f->flags&MI_ZF))goto mi_00519ce1;
    /* 00519caf */
    a=mi_read((0x00b4e6fcu+f->r[6]*2u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00519cb8 */
    if(!(f->flags&MI_ZF))goto mi_00519ce1;
mi_00519cba:;
    /* 00519cba */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519cc1 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00519cc6 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519ccd */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519cce */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519cd3 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519cd8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519ce1;
    /* 00519cda */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519cdc */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519ce1:;
    /* 00519ce1 */
    f->r[6]=(0x00000001u+f->r[1]);
    /* 00519ce4 */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00519ceb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519ced */
    if(!(f->flags&MI_ZF))goto mi_00519d39;
    /* 00519cef */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[1]),1u))&0xffu)<<0);
    /* 00519cf5 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519cf7 */
    if(!mi_signed_less(f->flags))goto mi_00519d12;
    /* 00519cf9 */
    a=(f->r[2]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00519cfc */
    if(!(f->flags&MI_ZF))goto mi_00519d39;
    /* 00519cfe */
    a=mi_read((0x00af5990u+f->r[1]*4u),4u);b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00519d05 */
    if((f->flags&MI_ZF))goto mi_00519d39;
    /* 00519d07 */
    a=mi_read((0x00b4e6feu+f->r[1]*2u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00519d10 */
    if(!(f->flags&MI_ZF))goto mi_00519d39;
mi_00519d12:;
    /* 00519d12 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519d19 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00519d1e */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519d25 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519d26 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519d2b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519d30 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519d39;
    /* 00519d32 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519d34 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519d39:;
    /* 00519d39 */
    f->r[6]=(0x000000e4u+f->r[1]);
    /* 00519d3f */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 00519d46 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519d48 */
    if(!(f->flags&MI_ZF))goto mi_00519d94;
    /* 00519d4a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[1]),1u))&0xffu)<<0);
    /* 00519d50 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519d52 */
    if(!mi_signed_less(f->flags))goto mi_00519d6d;
    /* 00519d54 */
    a=(f->r[2]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00519d57 */
    if(!(f->flags&MI_ZF))goto mi_00519d94;
    /* 00519d59 */
    a=mi_read((0x00af5d1cu+f->r[1]*4u),4u);b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00519d60 */
    if((f->flags&MI_ZF))goto mi_00519d94;
    /* 00519d62 */
    a=mi_read((0x00b4e8c4u+f->r[1]*2u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00519d6b */
    if(!(f->flags&MI_ZF))goto mi_00519d94;
mi_00519d6d:;
    /* 00519d6d */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519d74 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00519d79 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519d80 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519d81 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519d86 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519d8b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519d94;
    /* 00519d8d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519d8f */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519d94:;
    /* 00519d94 */
    f->r[2]=mi_read((0x00eed75cu+f->r[1]*4u),4u);
    /* 00519d9b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519d9d */
    if(!(f->flags&MI_ZF))goto mi_00519dea;
    /* 00519d9f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[1]),1u))&0xffu)<<0);
    /* 00519da5 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519da7 */
    if(!mi_signed_less(f->flags))goto mi_00519dc2;
    /* 00519da9 */
    a=(f->r[2]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00519dac */
    if(!(f->flags&MI_ZF))goto mi_00519dea;
    /* 00519dae */
    a=mi_read((0x00af5988u+f->r[1]*4u),4u);b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00519db5 */
    if((f->flags&MI_ZF))goto mi_00519dea;
    /* 00519db7 */
    a=mi_read((0x00b4e6fau+f->r[1]*2u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00519dc0 */
    if(!(f->flags&MI_ZF))goto mi_00519dea;
mi_00519dc2:;
    /* 00519dc2 */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[7]);
    /* 00519dc9 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 00519dce */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00519dcf */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 00519dd6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519dd7 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519ddc */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519de1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519dea;
    /* 00519de3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519de5 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519dea:;
    /* 00519dea */
    f->r[1]=mi_read((0x00eae00cu),4u);
    /* 00519df0 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00519df1 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519df7 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 00519dfd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519e09;
    /* 00519dff */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
mi_00519e09:;
    /* 00519e09 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 00519e0f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00519e11 */
    if(!(f->flags&MI_ZF))goto mi_00519c6f;
mi_00519e17:;
    /* 00519e17 */
    f->r[1]=mi_read((0x00eed760u+f->r[3]*4u),4u);
    /* 00519e1e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519e20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00519e21 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00519e22 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00519e24 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00519e25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00519e26 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00519e29 */
    f->r[4]+=4;return;
}

void restore_routing_C_0051a030(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051a030 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a031 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a032 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a033 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a035 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a03a */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a03f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a044u;f->boundary(0x0051a03fu,0x0040f8f0u,f);
    /* 0051a044 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051a048 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051a04c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a04f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051a052 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051a055 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051a058 */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0051a05d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051a060 */
    f->r[2]=mi_read((0x00eadfd4u),4u);
    /* 0051a066 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051a068 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051a069 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051a06e */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[2]);
    /* 0051a074 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051a07f */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 0051a085 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051a08b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051a08f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051a091 */
    f->r[0]=0x00000001u;
    /* 0051a096 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0051a099 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051a09f */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a0a4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 0051a0a7 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051a0ad */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0051a0b0 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 0051a0b3 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051a0b7 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0051a0b9:;
    /* 0051a0b9 */
    f->r[1]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 0051a0c0 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0051a0c2 */
    if((f->flags&MI_ZF))goto mi_0051a209;
    /* 0051a0c8 */
    f->r[7]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 0051a0cf */
    f->r[6]=(0xffffff1cu+f->r[1]);
    /* 0051a0d5 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051a0d6 */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a0dd */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a0df */
    if(!(f->flags&MI_ZF))goto mi_0051a117;
    /* 0051a0e1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ebaa3cu+f->r[1]),1u))&0xffu)<<0);
    /* 0051a0e7 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a0e9 */
    if(mi_signed_less(f->flags))goto mi_0051a117;
    /* 0051a0eb */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a0ee */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a117;
    /* 0051a0f0 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0051a0f7 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a0fc */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a103 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a104 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a109 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a10e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a117;
    /* 0051a110 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a112 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a117:;
    /* 0051a117 */
    f->r[6]=(0x00000001u+f->r[1]);
    /* 0051a11a */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a121 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a123 */
    if(!(f->flags&MI_ZF))goto mi_0051a15b;
    /* 0051a125 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ebab21u+f->r[1]),1u))&0xffu)<<0);
    /* 0051a12b */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a12d */
    if(mi_signed_less(f->flags))goto mi_0051a15b;
    /* 0051a12f */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a132 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a15b;
    /* 0051a134 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0051a13b */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a140 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a147 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a148 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a14d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a152 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a15b;
    /* 0051a154 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a156 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a15b:;
    /* 0051a15b */
    f->r[6]=(0x000000e4u+f->r[1]);
    /* 0051a161 */
    f->r[2]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a168 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a16a */
    if(!(f->flags&MI_ZF))goto mi_0051a1a2;
    /* 0051a16c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ebac04u+f->r[1]),1u))&0xffu)<<0);
    /* 0051a172 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a174 */
    if(mi_signed_less(f->flags))goto mi_0051a1a2;
    /* 0051a176 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a179 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a1a2;
    /* 0051a17b */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0051a182 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a187 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a18e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a18f */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a194 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a199 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a1a2;
    /* 0051a19b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a19d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a1a2:;
    /* 0051a1a2 */
    f->r[2]=mi_read((0x00eed75cu+f->r[1]*4u),4u);
    /* 0051a1a9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a1ab */
    if(!(f->flags&MI_ZF))goto mi_0051a1e4;
    /* 0051a1ad */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ebab1fu+f->r[1]),1u))&0xffu)<<0);
    /* 0051a1b3 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a1b5 */
    if(mi_signed_less(f->flags))goto mi_0051a1e4;
    /* 0051a1b7 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a1ba */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a1e4;
    /* 0051a1bc */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[7]);
    /* 0051a1c3 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a1c8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051a1c9 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0051a1d0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a1d1 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a1d6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a1db */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a1e4;
    /* 0051a1dd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a1df */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a1e4:;
    /* 0051a1e4 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 0051a1ea */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051a1eb */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a1f1 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051a1f7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a201;
    /* 0051a1f9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051a1fb */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_0051a201:;
    /* 0051a201 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a203 */
    if(!(f->flags&MI_ZF))goto mi_0051a0b9;
mi_0051a209:;
    /* 0051a209 */
    f->r[1]=mi_read((0x00eed760u+f->r[3]*4u),4u);
    /* 0051a210 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a212 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a214 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051a215 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051a216 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a219 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051a21a */
    f->r[4]+=4;return;
}

void restore_routing_C_0051a420(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051a420 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a421 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a422 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a423 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a424 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0051a426 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a427 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a428 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a42d */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a432 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0051a436 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a43bu;f->boundary(0x0051a436u,0x0040f8f0u,f);
    /* 0051a43b */
    f->r[0]=mi_read((0x00000028u+f->r[4]),4u);
    /* 0051a43f */
    f->r[5]=mi_read((0x00eadfd8u),4u);
    /* 0051a445 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a448 */
    f->r[3]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051a44c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051a44f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051a452 */
    f->r[1]=mi_read((0x00eadfd4u),4u);
    /* 0051a458 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051a45b */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0051a460 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051a463 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051a467 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051a469 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051a46a */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051a46b */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051a470 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 0051a476 */
    MI_U32((0x00eadfd8u))=(uint32_t)(f->r[5]);
    /* 0051a47c */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051a487 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 0051a48d */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051a493 */
    f->r[1]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0051a497 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051a499 */
    f->r[0]=0x00000001u;
    /* 0051a49e */
    f->r[7]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0051a4a1 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051a4a7 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a4ac */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0051a4b0 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[7]*2u);
    /* 0051a4b3 */
    f->r[7]=mi_read((0x00bca2d8u),4u);
    /* 0051a4b9 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0051a4bc */
    f->r[1]=(0x00000000u+f->r[7]+f->r[1]*4u);
    /* 0051a4bf */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051a4c1 */
    f->r[3]=mi_read((0x00000028u+f->r[4]),4u);
    /* 0051a4c5 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0051a4c7 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051a4cb */
    if(!(f->flags&MI_ZF))goto mi_0051a7c9;
mi_0051a4d1:;
    /* 0051a4d1 */
    f->r[6]=mi_read((0x00eae00cu),4u);
    /* 0051a4d7 */
    f->r[3]=f->r[6];
    /* 0051a4d9 */
    f->r[2]=mi_read((0x00f203a0u+f->r[3]*4u),4u);
    /* 0051a4e0 */
    f->r[1]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051a4e7 */
    f->r[2]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051a4eb */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0051a4ed */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a58d;
    /* 0051a4f3 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0051a4f5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a52a;
    /* 0051a4f7 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051a4f8 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a4fa */
    if(!mi_signed_less(f->flags))goto mi_0051a589;
    /* 0051a500 */
    f->r[7]=(0x00f203a0u+f->r[6]*4u);
mi_0051a507:;
    /* 0051a507 */
    f->r[2]=mi_read((0x00000000u+f->r[7]),4u);
    /* 0051a509 */
    f->r[2]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051a510 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051a512 */
    if(!mi_signed_less(f->flags))goto mi_0051a520;
    /* 0051a514 */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051a518 */
    f->r[3]=f->r[6];
    /* 0051a51a */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051a51c */
    f->r[1]=f->r[2];
    /* 0051a51e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a589;
mi_0051a520:;
    /* 0051a520 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051a521 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051a524 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a526 */
    if(mi_signed_less(f->flags))goto mi_0051a507;
    /* 0051a528 */
    goto mi_0051a589;
mi_0051a52a:;
    /* 0051a52a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051a52b */
    a=f->r[6];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a531 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a55f;
    /* 0051a533 */
    f->r[7]=(0x00f203a0u+f->r[6]*4u);
mi_0051a53a:;
    /* 0051a53a */
    f->r[2]=mi_read((0x00000000u+f->r[7]),4u);
    /* 0051a53c */
    f->r[2]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051a543 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051a545 */
    if(!mi_signed_less(f->flags))goto mi_0051a553;
    /* 0051a547 */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0051a54b */
    f->r[3]=f->r[6];
    /* 0051a54d */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051a54f */
    f->r[1]=f->r[2];
    /* 0051a551 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a589;
mi_0051a553:;
    /* 0051a553 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051a556 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051a557 */
    a=f->r[7];b=0x00f52fdcu;t=mi_alu(f,a,b,32,1);
    /* 0051a55d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a53a;
mi_0051a55f:;
    /* 0051a55f */
    a=f->r[1];b=mi_read((0x00000010u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051a563 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a589;
    /* 0051a565 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051a567 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a569 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a589;
    /* 0051a56b */
    f->r[7]=0x00f203a0u;
mi_0051a570:;
    /* 0051a570 */
    f->r[6]=mi_read((0x00000000u+f->r[7]),4u);
    /* 0051a572 */
    f->r[6]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a579 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051a57b */
    if(!mi_signed_less(f->flags))goto mi_0051a581;
    /* 0051a57d */
    f->r[3]=f->r[2];
    /* 0051a57f */
    f->r[1]=f->r[6];
mi_0051a581:;
    /* 0051a581 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051a582 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051a585 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a587 */
    if(mi_signed_less(f->flags))goto mi_0051a570;
mi_0051a589:;
    /* 0051a589 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051a58d:;
    /* 0051a58d */
    f->r[1]=mi_read((0x00eae00cu),4u);
    /* 0051a593 */
    f->r[5]=mi_read((0x00f203a0u+f->r[3]*4u),4u);
    /* 0051a59a */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051a59c */
    if((f->flags&MI_ZF))goto mi_0051a5ac;
    /* 0051a59e */
    f->r[1]=mi_read((0x00f203a0u+f->r[1]*4u),4u);
    /* 0051a5a5 */
    MI_U32((0x00f203a0u+f->r[3]*4u))=(uint32_t)(f->r[1]);
mi_0051a5ac:;
    /* 0051a5ac */
    a=f->r[5];b=mi_read((0x0000001cu+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051a5b0 */
    if((f->flags&MI_ZF))goto mi_0051a9ab;
    /* 0051a5b6 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051a5ba */
    f->r[2]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0051a5be */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051a5bf */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0051a5c1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051a5c5 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a9ab;
    /* 0051a5cb */
    f->r[3]=mi_read((0x00eed760u+f->r[5]*4u),4u);
    /* 0051a5d2 */
    f->r[6]=(0xffffff1cu+f->r[5]);
    /* 0051a5d8 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0051a5d9 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a5e0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a5e2 */
    if(!(f->flags&MI_ZF))goto mi_0051a645;
    /* 0051a5e4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a5e5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a5eau;f->boundary(0x0051a5e5u,0x0051afa0u,f);
    /* 0051a5ea */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a5ed */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a5ef */
    if((f->flags&MI_ZF))goto mi_0051a640;
    /* 0051a5f1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec754cu+f->r[5]),1u))&0xffu)<<0);
    /* 0051a5f7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a5f9 */
    if(mi_signed_less(f->flags))goto mi_0051a640;
    /* 0051a5fb */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051a5fd */
    if(!mi_signed_less(f->flags))goto mi_0051a640;
    /* 0051a5ff */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051a601 */
    if(!(f->flags&MI_ZF))goto mi_0051a60a;
    /* 0051a603 */
    f->r[0]=0x00000028u;
    /* 0051a608 */
    goto mi_0051a615;
mi_0051a60a:;
    /* 0051a60a */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a60c */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a60e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051a610 */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a612 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051a615:;
    /* 0051a615 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051a617 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051a61e */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a623 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a62a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a62b */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a630 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a635 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a645;
    /* 0051a637 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a639 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a63e */
    goto mi_0051a645;
mi_0051a640:;
    /* 0051a640 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a645:;
    /* 0051a645 */
    f->r[6]=(0x00000001u+f->r[5]);
    /* 0051a648 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a64f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a651 */
    if(!(f->flags&MI_ZF))goto mi_0051a6b4;
    /* 0051a653 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a654 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a659u;f->boundary(0x0051a654u,0x0051afa0u,f);
    /* 0051a659 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a65c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a65e */
    if((f->flags&MI_ZF))goto mi_0051a6af;
    /* 0051a660 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec7631u+f->r[5]),1u))&0xffu)<<0);
    /* 0051a666 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a668 */
    if(mi_signed_less(f->flags))goto mi_0051a6af;
    /* 0051a66a */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051a66c */
    if(!mi_signed_less(f->flags))goto mi_0051a6af;
    /* 0051a66e */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051a670 */
    if(!(f->flags&MI_ZF))goto mi_0051a679;
    /* 0051a672 */
    f->r[0]=0x00000028u;
    /* 0051a677 */
    goto mi_0051a684;
mi_0051a679:;
    /* 0051a679 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a67b */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a67d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051a67f */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a681 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051a684:;
    /* 0051a684 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051a686 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051a68d */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a692 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a699 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a69a */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a69f */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a6a4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a6b4;
    /* 0051a6a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a6a8 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a6ad */
    goto mi_0051a6b4;
mi_0051a6af:;
    /* 0051a6af */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a6b4:;
    /* 0051a6b4 */
    f->r[6]=(0x000000e4u+f->r[5]);
    /* 0051a6ba */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a6c1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a6c3 */
    if(!(f->flags&MI_ZF))goto mi_0051a726;
    /* 0051a6c5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a6c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a6cbu;f->boundary(0x0051a6c6u,0x0051afa0u,f);
    /* 0051a6cb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a6ce */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a6d0 */
    if((f->flags&MI_ZF))goto mi_0051a721;
    /* 0051a6d2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec7714u+f->r[5]),1u))&0xffu)<<0);
    /* 0051a6d8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a6da */
    if(mi_signed_less(f->flags))goto mi_0051a721;
    /* 0051a6dc */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051a6de */
    if(!mi_signed_less(f->flags))goto mi_0051a721;
    /* 0051a6e0 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051a6e2 */
    if(!(f->flags&MI_ZF))goto mi_0051a6eb;
    /* 0051a6e4 */
    f->r[0]=0x00000028u;
    /* 0051a6e9 */
    goto mi_0051a6f6;
mi_0051a6eb:;
    /* 0051a6eb */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a6ed */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a6ef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051a6f1 */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a6f3 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051a6f6:;
    /* 0051a6f6 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051a6f8 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051a6ff */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a704 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a70b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a70c */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a711 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a716 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a726;
    /* 0051a718 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a71a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a71f */
    goto mi_0051a726;
mi_0051a721:;
    /* 0051a721 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a726:;
    /* 0051a726 */
    f->r[1]=mi_read((0x00eed75cu+f->r[5]*4u),4u);
    /* 0051a72d */
    f->r[6]=(0x00eed75cu+f->r[5]*4u);
    /* 0051a734 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a736 */
    if(!(f->flags&MI_ZF))goto mi_0051a797;
    /* 0051a738 */
    f->r[7]=(0xffffffffu+f->r[5]);
    /* 0051a73b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a73c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a741u;f->boundary(0x0051a73cu,0x0051afa0u,f);
    /* 0051a741 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a744 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a746 */
    if((f->flags&MI_ZF))goto mi_0051a792;
    /* 0051a748 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec762fu+f->r[5]),1u))&0xffu)<<0);
    /* 0051a74e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a750 */
    if(mi_signed_less(f->flags))goto mi_0051a792;
    /* 0051a752 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051a754 */
    if(!mi_signed_less(f->flags))goto mi_0051a792;
    /* 0051a756 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051a758 */
    if(!(f->flags&MI_ZF))goto mi_0051a761;
    /* 0051a75a */
    f->r[0]=0x00000028u;
    /* 0051a75f */
    goto mi_0051a76c;
mi_0051a761:;
    /* 0051a761 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a763 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a765 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051a767 */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a769 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051a76c:;
    /* 0051a76c */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051a76e */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0051a770 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a775 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051a77c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a77d */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a782 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a787 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a797;
    /* 0051a789 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a78b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a790 */
    goto mi_0051a797;
mi_0051a792:;
    /* 0051a792 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a797:;
    /* 0051a797 */
    f->r[1]=mi_read((0x00eae00cu),4u);
    /* 0051a79d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051a79e */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a7a4 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051a7aa */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a7b4;
    /* 0051a7ac */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051a7ae */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
mi_0051a7b4:;
    /* 0051a7b4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a7b6 */
    if(!(f->flags&MI_ZF))goto mi_0051a4d1;
    /* 0051a7bc */
    goto mi_0051a9ab;
mi_0051a7c1:;
    /* 0051a7c1 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051a7c5 */
    f->r[3]=mi_read((0x00000028u+f->r[4]),4u);
mi_0051a7c9:;
    /* 0051a7c9 */
    f->r[6]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 0051a7d0 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051a7d2 */
    if((f->flags&MI_ZF))goto mi_0051a9ab;
    /* 0051a7d8 */
    f->r[5]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051a7df */
    f->r[7]=(0xffffff1cu+f->r[6]);
    /* 0051a7e5 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051a7e6 */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 0051a7ed */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a7ef */
    if(!(f->flags&MI_ZF))goto mi_0051a84a;
    /* 0051a7f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a7f2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a7f7u;f->boundary(0x0051a7f2u,0x0051afa0u,f);
    /* 0051a7f7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a7fa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a7fc */
    if((f->flags&MI_ZF))goto mi_0051a845;
    /* 0051a7fe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec754cu+f->r[6]),1u))&0xffu)<<0);
    /* 0051a804 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a806 */
    if((f->flags&MI_ZF))goto mi_0051a81c;
    /* 0051a808 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a80a */
    if((f->flags&MI_ZF))goto mi_0051a81c;
    /* 0051a80c */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051a80e */
    if(!(f->flags&MI_ZF))goto mi_0051a845;
    /* 0051a810 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[7]*2u),2u);
    /* 0051a818 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0051a81a */
    if(!(f->flags&MI_ZF))goto mi_0051a845;
mi_0051a81c:;
    /* 0051a81c */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 0051a823 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a828 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051a82f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a830 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a835 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a83a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a84a;
    /* 0051a83c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a83e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a843 */
    goto mi_0051a84a;
mi_0051a845:;
    /* 0051a845 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a84a:;
    /* 0051a84a */
    f->r[7]=(0x00000001u+f->r[6]);
    /* 0051a84d */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 0051a854 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a856 */
    if(!(f->flags&MI_ZF))goto mi_0051a8b1;
    /* 0051a858 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a859 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a85eu;f->boundary(0x0051a859u,0x0051afa0u,f);
    /* 0051a85e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a861 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a863 */
    if((f->flags&MI_ZF))goto mi_0051a8ac;
    /* 0051a865 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec7631u+f->r[6]),1u))&0xffu)<<0);
    /* 0051a86b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a86d */
    if((f->flags&MI_ZF))goto mi_0051a883;
    /* 0051a86f */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a871 */
    if((f->flags&MI_ZF))goto mi_0051a883;
    /* 0051a873 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051a875 */
    if(!(f->flags&MI_ZF))goto mi_0051a8ac;
    /* 0051a877 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6feu+f->r[6]*2u),2u);
    /* 0051a87f */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0051a881 */
    if(!(f->flags&MI_ZF))goto mi_0051a8ac;
mi_0051a883:;
    /* 0051a883 */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 0051a88a */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a88f */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051a896 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a897 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a89c */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a8a1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a8b1;
    /* 0051a8a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a8a5 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a8aa */
    goto mi_0051a8b1;
mi_0051a8ac:;
    /* 0051a8ac */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a8b1:;
    /* 0051a8b1 */
    f->r[7]=(0x000000e4u+f->r[6]);
    /* 0051a8b7 */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 0051a8be */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a8c0 */
    if(!(f->flags&MI_ZF))goto mi_0051a91b;
    /* 0051a8c2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a8c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a8c8u;f->boundary(0x0051a8c3u,0x0051afa0u,f);
    /* 0051a8c8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a8cb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a8cd */
    if((f->flags&MI_ZF))goto mi_0051a916;
    /* 0051a8cf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec7714u+f->r[6]),1u))&0xffu)<<0);
    /* 0051a8d5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a8d7 */
    if((f->flags&MI_ZF))goto mi_0051a8ed;
    /* 0051a8d9 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a8db */
    if((f->flags&MI_ZF))goto mi_0051a8ed;
    /* 0051a8dd */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051a8df */
    if(!(f->flags&MI_ZF))goto mi_0051a916;
    /* 0051a8e1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e8c4u+f->r[6]*2u),2u);
    /* 0051a8e9 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0051a8eb */
    if(!(f->flags&MI_ZF))goto mi_0051a916;
mi_0051a8ed:;
    /* 0051a8ed */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 0051a8f4 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a8f9 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051a900 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a901 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a906 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a90b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a91b;
    /* 0051a90d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a90f */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a914 */
    goto mi_0051a91b;
mi_0051a916:;
    /* 0051a916 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a91b:;
    /* 0051a91b */
    f->r[1]=mi_read((0x00eed75cu+f->r[6]*4u),4u);
    /* 0051a922 */
    f->r[7]=(0x00eed75cu+f->r[6]*4u);
    /* 0051a929 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051a92b */
    if(!(f->flags&MI_ZF))goto mi_0051a986;
    /* 0051a92d */
    f->r[3]=(0xffffffffu+f->r[6]);
    /* 0051a930 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a931 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a936u;f->boundary(0x0051a931u,0x0051afa0u,f);
    /* 0051a936 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a939 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051a93b */
    if((f->flags&MI_ZF))goto mi_0051a981;
    /* 0051a93d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ec762fu+f->r[6]),1u))&0xffu)<<0);
    /* 0051a943 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a945 */
    if((f->flags&MI_ZF))goto mi_0051a95d;
    /* 0051a947 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a949 */
    if((f->flags&MI_ZF))goto mi_0051a95d;
    /* 0051a94b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051a94d */
    if(!(f->flags&MI_ZF))goto mi_0051a981;
    /* 0051a94f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fau+f->r[6]*2u),2u);
    /* 0051a957 */
    a=f->r[2];b=mi_read((0x00000028u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0051a95b */
    if(!(f->flags&MI_ZF))goto mi_0051a981;
mi_0051a95d:;
    /* 0051a95d */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 0051a95f */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051a964 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 0051a96b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a96c */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a971 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a976 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a986;
    /* 0051a978 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a97a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a97f */
    goto mi_0051a986;
mi_0051a981:;
    /* 0051a981 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051a986:;
    /* 0051a986 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 0051a98c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051a98d */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a993 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051a999 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a9a3;
    /* 0051a99b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051a99d */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_0051a9a3:;
    /* 0051a9a3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a9a5 */
    if(!(f->flags&MI_ZF))goto mi_0051a7c1;
mi_0051a9ab:;
    /* 0051a9ab */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051a9af */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a9b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051a9b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051a9b3 */
    f->r[2]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 0051a9ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051a9bb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a9bd */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051a9c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051a9c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0051a9c2 */
    f->r[4]+=4;return;
}

void restore_routing_C_0051b040(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051b040 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b041 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b042 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b043 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b045 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b04a */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b04f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051b054u;f->boundary(0x0051b04fu,0x0040f8f0u,f);
    /* 0051b054 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051b058 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051b05c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051b05f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051b062 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051b065 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051b068 */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0051b06d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051b070 */
    f->r[2]=mi_read((0x00eadfd4u),4u);
    /* 0051b076 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051b078 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b079 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051b07e */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[2]);
    /* 0051b084 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051b08f */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 0051b095 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051b09b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051b09f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051b0a1 */
    f->r[0]=0x00000001u;
    /* 0051b0a6 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0051b0a9 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051b0af */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b0b4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 0051b0b7 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051b0bd */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0051b0c0 */
    f->r[7]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 0051b0c3 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051b0c7 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_0051b0c9:;
    /* 0051b0c9 */
    f->r[1]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 0051b0d0 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0051b0d2 */
    if((f->flags&MI_ZF))goto mi_0051b205;
    /* 0051b0d8 */
    f->r[6]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 0051b0df */
    f->r[2]=(0xffffff1cu+f->r[1]);
    /* 0051b0e5 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051b0e6 */
    f->r[3]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051b0ed */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051b0ef */
    if(!(f->flags&MI_ZF))goto mi_0051b122;
    /* 0051b0f1 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00ec754cu+f->r[1]),1u))&0xffu)<<0);
    /* 0051b0f7 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051b0f9 */
    if(mi_signed_less(f->flags))goto mi_0051b122;
    /* 0051b0fb */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051b102 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051b107 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051b10e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b10f */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b114 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b119 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b122;
    /* 0051b11b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b11d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b122:;
    /* 0051b122 */
    f->r[2]=(0x00000001u+f->r[1]);
    /* 0051b125 */
    f->r[3]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051b12c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051b12e */
    if(!(f->flags&MI_ZF))goto mi_0051b161;
    /* 0051b130 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00ec7631u+f->r[1]),1u))&0xffu)<<0);
    /* 0051b136 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051b138 */
    if(mi_signed_less(f->flags))goto mi_0051b161;
    /* 0051b13a */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051b141 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051b146 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051b14d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b14e */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b153 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b158 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b161;
    /* 0051b15a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b15c */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b161:;
    /* 0051b161 */
    f->r[2]=(0x000000e4u+f->r[1]);
    /* 0051b167 */
    f->r[3]=mi_read((0x00eed760u+f->r[2]*4u),4u);
    /* 0051b16e */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051b170 */
    if(!(f->flags&MI_ZF))goto mi_0051b1a3;
    /* 0051b172 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00ec7714u+f->r[1]),1u))&0xffu)<<0);
    /* 0051b178 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051b17a */
    if(mi_signed_less(f->flags))goto mi_0051b1a3;
    /* 0051b17c */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051b183 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051b188 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051b18f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b190 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b195 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b19a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b1a3;
    /* 0051b19c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b19e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b1a3:;
    /* 0051b1a3 */
    f->r[2]=mi_read((0x00eed75cu+f->r[1]*4u),4u);
    /* 0051b1aa */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051b1ac */
    if(!(f->flags&MI_ZF))goto mi_0051b1e0;
    /* 0051b1ae */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ec762fu+f->r[1]),1u))&0xffu)<<0);
    /* 0051b1b4 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051b1b6 */
    if(mi_signed_less(f->flags))goto mi_0051b1e0;
    /* 0051b1b8 */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0051b1bf */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051b1c4 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051b1c5 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0051b1cc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b1cd */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b1d2 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b1d7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b1e0;
    /* 0051b1d9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b1db */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b1e0:;
    /* 0051b1e0 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 0051b1e6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b1e7 */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b1ed */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051b1f3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b1fd;
    /* 0051b1f5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051b1f7 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_0051b1fd:;
    /* 0051b1fd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051b1ff */
    if(!(f->flags&MI_ZF))goto mi_0051b0c9;
mi_0051b205:;
    /* 0051b205 */
    f->r[1]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 0051b20c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b20e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051b210 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051b211 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051b212 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051b215 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051b216 */
    f->r[4]+=4;return;
}

void restore_routing_C_0051afa0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051afa0 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0051afa4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[0]*2u),2u);
    /* 0051afac */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051afae */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051afda;
    /* 0051afb0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((0x00000005u)&0xffu)<<0);
mi_0051afb2:;
    /* 0051afb2 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0051afb5 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0051afb8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051afba */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0051afbd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd605u+f->r[0]),1u))&0xffu)<<0);
    /* 0051afc3 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051afc5 */
    if(!(f->flags&MI_ZF))goto mi_0051afcf;
    /* 0051afc7 */
    a=mi_read((0x00bcd630u+f->r[0]),1u);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0051afcd */
    if((f->flags&MI_ZF))goto mi_0051afe0;
mi_0051afcf:;
    /* 0051afcf */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[0]),2u);
    /* 0051afd6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051afd8 */
    if(!(f->flags&MI_ZF))goto mi_0051afb2;
mi_0051afda:;
    /* 0051afda */
    f->r[0]=0x00000001u;
    /* 0051afdf */
    f->r[4]+=4;return;
mi_0051afe0:;
    /* 0051afe0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051afe2 */
    f->r[4]+=4;return;
}

void restore_routing_C_004b33d0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b33d0 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004b33d4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b33d5 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b33d8 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b33db */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b33dd */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b33e0 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00bcd632u+f->r[6]),2u))&0xffffu)<<0);
    /* 004b33e7 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004b33ea */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b3427;
    /* 004b33ec */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b33f3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b33fa */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3401 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3402 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3403 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b340a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b340b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b340c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3411u;f->boundary(0x004b340cu,0x0051cba0u,f);
    /* 004b3411 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3414 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b3416 */
    MI_U8((0x00bcd609u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b341c */
    if((f->flags&MI_ZF))goto mi_004b3465;
    /* 004b341e */
    MI_U8((0x00bcd609u+f->r[6]))=(uint8_t)(0x0000000au);
    /* 004b3425 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3426 */
    f->r[4]+=4;return;
mi_004b3427:;
    /* 004b3427 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00bcd634u+f->r[6]),2u))&0xffffu)<<0);
    /* 004b342e */
    a=(f->r[2]&0xffffu);b=mi_read((0x00bcd636u+f->r[6]),2u);t=mi_alu(f,a,b,16,1);
    /* 004b3435 */
    if(mi_signed_less(f->flags))goto mi_004b3449;
    /* 004b3437 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3438 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b343du;f->boundary(0x004b3438u,0x004b3250u,f);
    /* 004b343d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3440 */
    MI_U8((0x00bcd609u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 004b3447 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3448 */
    f->r[4]+=4;return;
mi_004b3449:;
    /* 004b3449 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 004b344c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b344f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*8u);
    /* 004b3452 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 004b3455 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004b3458 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00c8ad38u+f->r[1]+f->r[0]*4u),1u))&0xffu)<<0);
    /* 004b345f */
    MI_U8((0x00bcd609u+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
mi_004b3465:;
    /* 004b3465 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3466 */
    f->r[4]+=4;return;
}

void restore_routing_C_004b3530(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b3530 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004b3533 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3534 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3535 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3536 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3537 */
    f->r[7]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b353b */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b353e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3541 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3543 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3546 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 004b354c */
    a=(f->r[1]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b354f */
    if(!mi_signed_less(f->flags))goto mi_004b3b5c;
    /* 004b3555 */
    a=mi_read((0x00bcd66au+f->r[0]),1u);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004b355c */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3562 */
    f->r[6]=mi_read((0x00bcd618u+f->r[0]),4u);
    /* 004b3568 */
    f->r[2]=(uint32_t)(int32_t)(int8_t)(f->r[1]&0xffu);
    /* 004b356b */
    f->r[3]=mi_read((0x005f80fcu+f->r[2]*4u),4u);
    /* 004b3572 */
    f->r[5]=(0x00000000u+f->r[3]+f->r[6]*1u);
    /* 004b3575 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[5]*2u),2u);
    /* 004b357d */
    f->r[6]=mi_read((0x00af598cu+f->r[5]*4u),4u);
    /* 004b3584 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004b3588 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd66eu+f->r[0]),1u))&0xffu)<<0);
    /* 004b358e */
    f->r[3]=f->r[6];
    /* 004b3590 */
    a=f->r[3];b=0xeefec75fu;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004b3596 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b3598 */
    if((f->flags&MI_ZF))goto mi_004b35c0;
    /* 004b359a */
    a=(f->r[3]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 004b359d */
    if((f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b35a3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00ed4140u+f->r[5]),1u))&0xffu)<<0);
    /* 004b35a9 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b35ab */
    if(!mi_signed_less(f->flags))goto mi_004b3b5c;
    /* 004b35b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b35b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b35b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b35b4 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 004b35bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b35bc */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b35bf */
    f->r[4]+=4;return;
mi_004b35c0:;
    /* 004b35c0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd66au+f->r[0]),1u))&0xffu)<<0);
    /* 004b35c6 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b35c9 */
    if(!(f->flags&MI_ZF))goto mi_004b36b3;
    /* 004b35cf */
    f->r[2]=(uint32_t)(int32_t)(int8_t)mi_read((0x00ec7630u+f->r[5]),1u);
    /* 004b35d6 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004b35d9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b3b55;
    /* 004b35df */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004b35e1 */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b35e7 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004b35ea */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b35f0 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004b35f3 */
    if(!(f->flags&MI_ZF))goto mi_004b3622;
    /* 004b35f5 */
    MI_U8((0x00bcd60bu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b35fb */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 004b3602 */
    a=mi_read((0x0078d7ccu),1u);b=0x00000003u;t=mi_alu(f,a,b,8,3);
    /* 004b3609 */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b360f */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3611 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3612 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3617u;f->boundary(0x004b3612u,0x004690f0u,f);
    /* 004b3617 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b361a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b361b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b361c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b361d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b361e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3621 */
    f->r[4]+=4;return;
mi_004b3622:;
    /* 004b3622 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004b3625 */
    if(!(f->flags&MI_ZF))goto mi_004b3660;
    /* 004b3627 */
    a=f->r[6];b=0x00000400u;t=mi_alu(f,a,b,32,3);
    /* 004b362d */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3633 */
    MI_U8((0x00bcd60bu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b3639 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 004b3640 */
    a=mi_read((0x0078d7ccu),1u);b=0x00000003u;t=mi_alu(f,a,b,8,3);
    /* 004b3647 */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b364d */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b364f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3650 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3655u;f->boundary(0x004b3650u,0x004690f0u,f);
    /* 004b3655 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3658 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3659 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b365a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b365b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b365c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b365f */
    f->r[4]+=4;return;
mi_004b3660:;
    /* 004b3660 */
    MI_U8((0x00bcd60bu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b3666 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004b3669 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 004b3670 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0078d7ccu),1u))&0xffu)<<0);
    /* 004b3675 */
    if(!(f->flags&MI_ZF))goto mi_004b3695;
    /* 004b3677 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,3);
    /* 004b3679 */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b367f */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3684 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3685 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b368au;f->boundary(0x004b3685u,0x004690f0u,f);
    /* 004b368a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b368d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b368e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b368f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3690 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3691 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3694 */
    f->r[4]+=4;return;
mi_004b3695:;
    /* 004b3695 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,3);
    /* 004b3697 */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b369d */
    a=0x00000190u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b36a2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b36a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b36a8u;f->boundary(0x004b36a3u,0x004690f0u,f);
    /* 004b36a8 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b36ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b36ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b36ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b36ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b36af */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b36b2 */
    f->r[4]+=4;return;
mi_004b36b3:;
    /* 004b36b3 */
    a=(f->r[2]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b36b6 */
    if(!(f->flags&MI_ZF))goto mi_004b36d4;
    /* 004b36b8 */
    a=mi_read((0x00ebab20u+f->r[5]),1u);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 004b36bf */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b3b5c;
    /* 004b36c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b36c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b36c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b36c8 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 004b36cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b36d0 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b36d3 */
    f->r[4]+=4;return;
mi_004b36d4:;
    /* 004b36d4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00ee0c50u+f->r[5]),1u))&0xffu)<<0);
    /* 004b36da */
    a=(f->r[2]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004b36dd */
    if(!(f->flags&MI_ZF))goto mi_004b3703;
    /* 004b36df */
    a=(f->r[1]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 004b36e2 */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b36e8 */
    a=f->r[6];b=0x00010004u;t=mi_alu(f,a,b,32,3);
    /* 004b36ee */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b36f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b36f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b36f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b36f7 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 004b36fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b36ff */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3702 */
    f->r[4]+=4;return;
mi_004b3703:;
    /* 004b3703 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b3706 */
    if((f->flags&MI_ZF))goto mi_004b3711;
    /* 004b3708 */
    a=(f->r[1]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004b370b */
    if(!(f->flags&MI_ZF))goto mi_004b37cb;
mi_004b3711:;
    /* 004b3711 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[0]),1u))&0xffu)<<0);
    /* 004b3717 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b3719 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b372d;
    /* 004b371b */
    a=(f->r[1]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004b371e */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b37cb;
    /* 004b3724 */
    a=(f->r[1]&0xffu);b=0x0000005eu;t=mi_alu(f,a,b,8,1);
    /* 004b3727 */
    if((f->flags&MI_ZF))goto mi_004b37cb;
mi_004b372d:;
    /* 004b372d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[0]),2u);
    /* 004b3734 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[0]),2u);
    /* 004b373b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004b373f */
    f->r[0]=(0x00000014u+f->r[4]);
    /* 004b3743 */
    f->r[1]=(0x00000010u+f->r[4]);
    /* 004b3747 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3748 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3749 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004b374d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3752u;f->boundary(0x004b374du,0x004ad880u,f);
    /* 004b3752 */
    f->r[2]=mi_read((0x00000028u+f->r[4]),4u);
    /* 004b3756 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3757 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b375cu;f->boundary(0x004b3757u,0x004b5420u,f);
    /* 004b375c */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 004b3760 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b3764 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004b3768 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3769 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b376c */
    f->r[7]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 004b376f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3772 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3774 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3777 */
    MI_U16((0x00bcd60cu+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 004b377e */
    MI_U16((0x00bcd60eu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b3785 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[7]*2u);
    /* 004b3788 */
    f->r[7]=mi_read((0x00bca2d8u),4u);
    /* 004b378e */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004b3791 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[2]*4u);
    /* 004b3794 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b3796 */
    MI_U32((0x00bcd618u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004b379c */
    MI_U8((0x00bcd631u+f->r[0]))=(uint8_t)(0x0000000fu);
    /* 004b37a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b37a8u;f->boundary(0x004b37a3u,0x004b52f0u,f);
    /* 004b37a8 */
    f->r[0]=mi_read((0x00000030u+f->r[4]),4u);
    /* 004b37ac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b37ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b37b2u;f->boundary(0x004b37adu,0x004b3250u,f);
    /* 004b37b2 */
    f->r[7]=mi_read((0x00000034u+f->r[4]),4u);
    /* 004b37b6 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b37b9 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b37bc */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b37bf */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b37c1 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b37c4 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x00000009u);
mi_004b37cb:;
    /* 004b37cb */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004b37ce */
    if(!(f->flags&MI_ZF))goto mi_004b3b03;
    /* 004b37d4 */
    a=f->r[3];b=0x40000400u;t=mi_alu(f,a,b,32,3);
    /* 004b37da */
    if(!(f->flags&MI_ZF))goto mi_004b3b03;
    /* 004b37e0 */
    a=((f->r[3]>>8)&0xffu);b=0x000000c0u;t=mi_alu(f,a,b,8,3);
    /* 004b37e3 */
    if(!(f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b37e9 */
    a=(f->r[3]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004b37ec */
    if((f->flags&MI_ZF))goto mi_004b3aea;
    /* 004b37f2 */
    a=mi_read((0x00af598cu+f->r[5]*4u),4u);b=0x10000000u;t=mi_alu(f,a,b,32,3);
    /* 004b37fd */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3803 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004b3807 */
    f->r[6]=f->r[1];
    /* 004b3809 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b380c */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b380e */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b3811 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 004b3818 */
    a=(f->r[1]&0xffffu);b=0x00000038u;t=mi_alu(f,a,b,16,1);
    /* 004b381c */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3822 */
    a=(f->r[1]&0xffffu);b=0x00000036u;t=mi_alu(f,a,b,16,1);
    /* 004b3826 */
    if(!(f->flags&MI_ZF))goto mi_004b3836;
    /* 004b3828 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd66au+f->r[0]),1u))&0xffu)<<0);
    /* 004b382e */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b3830 */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
mi_004b3836:;
    /* 004b3836 */
    a=(f->r[1]&0xffffu);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004b383b */
    if(!(f->flags&MI_ZF))goto mi_004b384a;
    /* 004b383d */
    a=mi_read((0x00bcd66au+f->r[0]),1u);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004b3844 */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
mi_004b384a:;
    /* 004b384a */
    a=(f->r[1]&0xffffu);b=0x000000d1u;t=mi_alu(f,a,b,16,1);
    /* 004b384f */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3855 */
    a=(f->r[1]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 004b385a */
    if(!(f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3860 */
    f->r[1]=mi_read((0x00bcd5e8u),4u);
    /* 004b3866 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b3868 */
    if(!(f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b386e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b3870 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004b3872 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[0]),1u))&0xffu)<<0);
    /* 004b3878 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799aa2u+f->r[6]),1u))&0xffu)<<0);
    /* 004b387e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b387f */
    a=f->r[1];b=0x0000006bu;t=mi_alu(f,a,b,32,1);
    /* 004b3882 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b3b55;
    /* 004b3888 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b388a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x004b3b70u+f->r[1]),1u))&0xffu)<<0);
    /* 004b3890 */
    switch(mi_read((0x004b3b64u+f->r[2]*4u),4u)){case 0x004b3897u:goto mi_004b3897;case 0x004b3b55u:goto mi_004b3b55;case 0x004b3b5cu:goto mi_004b3b5c;default:abort();}
mi_004b3897:;
    /* 004b3897 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004b389b */
    f->r[5]=mi_read((0x00799a9cu+f->r[6]),4u);
    /* 004b38a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b38a2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b38a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b38a8u;f->boundary(0x004b38a3u,0x004b3470u,f);
    /* 004b38a8 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b38ab */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004b38ae */
    if(!(f->flags&MI_ZF))goto mi_004b3a6a;
    /* 004b38b4 */
    f->r[1]=(0x00000000u+f->r[3]*8u);
    /* 004b38bb */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004b38bd */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b38c0 */
    f->r[7]=(0x00eaaf68u+f->r[1]);
    /* 004b38c6 */
    f->r[1]=f->r[7];
    /* 004b38c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b38cdu;f->boundary(0x004b38c8u,0x004f6650u,f);
    /* 004b38cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b38cf */
    if(!(f->flags&MI_ZF))goto mi_004b3965;
    /* 004b38d5 */
    a=mi_read((0x00799a18u+f->r[6]),2u);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b38dc */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b3965;
    /* 004b38e2 */
    f->r[2]=f->r[5];
    /* 004b38e4 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b38e7 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b38e9 */
    a=mi_read((0x00799a18u+f->r[2]*8u),2u);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b38f1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b3965;
    /* 004b38f3 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b38f7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b38fa */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b38fd */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b38ff */
    a=mi_read((0x00bcd630u+f->r[1]*4u),1u);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004b3907 */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b390d */
    f->r[2]=(0x00000020u+f->r[4]);
    /* 004b3911 */
    f->r[1]=f->r[7];
    /* 004b3913 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3914 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3919u;f->boundary(0x004b3914u,0x004f65b0u,f);
    /* 004b3919 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b391d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b391e */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b3921 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3924 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3926 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3929 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[0]),1u))&0xffu)<<0);
    /* 004b392f */
    MI_U8((0x00bcd608u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004b3935 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000007u);
    /* 004b393c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3941u;f->boundary(0x004b393cu,0x004b3250u,f);
    /* 004b3941 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a02u+f->r[6]),2u);
    /* 004b3948 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b394b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b394e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b3951 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b3953 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3954 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3955 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3956 */
    MI_U16((0x00bcd62eu+f->r[1]*4u))=(uint16_t)(0x00000000u);
    /* 004b3960 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3961 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3964 */
    f->r[4]+=4;return;
mi_004b3965:;
    /* 004b3965 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b3969 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b396a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b396fu;f->boundary(0x004b396au,0x004b3250u,f);
    /* 004b396f */
    f->r[1]=mi_read((0x00000024u+f->r[4]),4u);
    /* 004b3973 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b3976 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3979 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b397b */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b397e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[0]),2u);
    /* 004b3985 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[0]),2u);
    /* 004b398c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004b3990 */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 004b3994 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 004b3998 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3999 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b399a */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004b399e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b39a3u;f->boundary(0x004b399eu,0x004ad880u,f);
    /* 004b39a3 */
    f->r[2]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 004b39a7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b39a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b39adu;f->boundary(0x004b39a8u,0x004b5420u,f);
    /* 004b39ad */
    f->r[1]=mi_read((0x00000030u+f->r[4]),4u);
    /* 004b39b1 */
    f->r[2]=mi_read((0x00000024u+f->r[4]),4u);
    /* 004b39b5 */
    f->r[6]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b39b9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b39ba */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b39bd */
    f->r[7]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 004b39c0 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b39c3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b39c5 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b39c8 */
    MI_U16((0x00bcd60cu+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 004b39cf */
    MI_U16((0x00bcd60eu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b39d6 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[7]*2u);
    /* 004b39d9 */
    f->r[7]=mi_read((0x00bca2d8u),4u);
    /* 004b39df */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004b39e2 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[2]*4u);
    /* 004b39e5 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b39e7 */
    MI_U32((0x00bcd618u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004b39ed */
    MI_U8((0x00bcd631u+f->r[0]))=(uint8_t)(0x0000000fu);
    /* 004b39f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b39f9u;f->boundary(0x004b39f4u,0x004b52f0u,f);
    /* 004b39f9 */
    f->r[1]=mi_read((0x00000034u+f->r[4]),4u);
    /* 004b39fd */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3a00 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b3a03 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3a06 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3a08 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b3a0a */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3a0d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[0]),1u))&0xffu)<<0);
    /* 004b3a13 */
    a=f->r[1];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b3a16 */
    a=f->r[1];b=0x00000065u;t=mi_alu(f,a,b,32,1);
    /* 004b3a19 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b3b5c;
    /* 004b3a1f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b3a21 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x004b3bf0u+f->r[1]),1u))&0xffu)<<0);
    /* 004b3a27 */
    switch(mi_read((0x004b3bdcu+f->r[2]*4u),4u)){case 0x004b3a2eu:goto mi_004b3a2e;case 0x004b3a3du:goto mi_004b3a3d;case 0x004b3a4cu:goto mi_004b3a4c;case 0x004b3a5bu:goto mi_004b3a5b;case 0x004b3b5cu:goto mi_004b3b5c;default:abort();}
mi_004b3a2e:;
    /* 004b3a2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3a2f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3a30 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3a31 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 004b3a38 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3a39 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3a3c */
    f->r[4]+=4;return;
mi_004b3a3d:;
    /* 004b3a3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3a3e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3a3f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3a40 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 004b3a47 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3a48 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3a4b */
    f->r[4]+=4;return;
mi_004b3a4c:;
    /* 004b3a4c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3a4d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3a4e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3a4f */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000eu);
    /* 004b3a56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3a57 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3a5a */
    f->r[4]+=4;return;
mi_004b3a5b:;
    /* 004b3a5b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3a5c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3a5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3a5e */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004b3a65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3a66 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3a69 */
    f->r[4]+=4;return;
mi_004b3a6a:;
    /* 004b3a6a */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004b3a6d */
    if(!(f->flags&MI_ZF))goto mi_004b3ace;
    /* 004b3a6f */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b3a73 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004b3a75:;
    /* 004b3a75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3a76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3a7bu;f->boundary(0x004b3a76u,0x004b33d0u,f);
    /* 004b3a7b */
    f->r[1]=mi_read((0x00000024u+f->r[4]),4u);
    /* 004b3a7f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3a80 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b3a83 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3a86 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3a88 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3a8b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 004b3a91 */
    MI_U8((0x00bcd60au+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004b3a97 */
    a=mi_read((0x00bcd634u+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd634u+f->r[0]))=(uint16_t)(t);
    /* 004b3a9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3aa3u;f->boundary(0x004b3a9eu,0x004b3c60u,f);
    /* 004b3aa3 */
    f->r[0]=mi_read((0x00000028u+f->r[4]),4u);
    /* 004b3aa7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3aaa */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b3aad */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b3ab0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b3ab2 */
    a=mi_read((0x00bcd609u+f->r[1]*4u),1u);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b3aba */
    if(!mi_signed_less(f->flags))goto mi_004b3b5c;
    /* 004b3ac0 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004b3ac1 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004b3ac4 */
    if(mi_signed_less(f->flags))goto mi_004b3a75;
    /* 004b3ac6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3ac7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3ac8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3ac9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3aca */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3acd */
    f->r[4]+=4;return;
mi_004b3ace:;
    /* 004b3ace */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004b3ad2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3ad3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3ad4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3ad5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b3ad8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3ad9 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b3adc */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b3ade */
    MI_U8((0x00bcd609u+f->r[2]*4u))=(uint8_t)(0x00000009u);
    /* 004b3ae6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3ae9 */
    f->r[4]+=4;return;
mi_004b3aea:;
    /* 004b3aea */
    a=(f->r[3]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 004b3aed */
    if(!(f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3aef */
    a=((f->r[3]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 004b3af2 */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3af4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3af5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3af6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3af7 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 004b3afe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3aff */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3b02 */
    f->r[4]+=4;return;
mi_004b3b03:;
    /* 004b3b03 */
    f->r[1]=mi_read((0x00bcd5e8u),4u);
    /* 004b3b09 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b3b0b */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3b0d */
    a=(f->r[3]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004b3b10 */
    if((f->flags&MI_ZF))goto mi_004b3b5c;
    /* 004b3b12 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004b3b16 */
    f->r[2]=f->r[1];
    /* 004b3b18 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b3b1b */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b3b1d */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[2]*8u),2u))&0xffffu)<<0);
    /* 004b3b25 */
    a=(f->r[1]&0xffffu);b=0x0000003au;t=mi_alu(f,a,b,16,1);
    /* 004b3b29 */
    if((f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3b2b */
    a=(f->r[1]&0xffffu);b=0x000000aau;t=mi_alu(f,a,b,16,1);
    /* 004b3b30 */
    if((f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3b32 */
    a=(f->r[1]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 004b3b37 */
    if((f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3b39 */
    a=(f->r[1]&0xffffu);b=0x000000c8u;t=mi_alu(f,a,b,16,1);
    /* 004b3b3e */
    if((f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3b40 */
    a=(f->r[1]&0xffffu);b=0x000000c9u;t=mi_alu(f,a,b,16,1);
    /* 004b3b45 */
    if((f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3b47 */
    a=(f->r[1]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 004b3b4c */
    if((f->flags&MI_ZF))goto mi_004b3b55;
    /* 004b3b4e */
    a=(f->r[1]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 004b3b53 */
    if(!(f->flags&MI_ZF))goto mi_004b3b5c;
mi_004b3b55:;
    /* 004b3b55 */
    MI_U8((0x00bcd609u+f->r[0]))=(uint8_t)(0x00000009u);
mi_004b3b5c:;
    /* 004b3b5c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3b5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3b5e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3b5f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3b60 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3b63 */
    f->r[4]+=4;return;
}

void restore_routing_C_004b3c60(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b3c60 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3c61 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3c62 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3c63 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3c64 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004b3c68 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004b3c6b */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b3c6e */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b3c70 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b3c73 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[6]),1u))&0xffu)<<0);
    /* 004b3c79 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b3c80 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b3c87 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b3c89 */
    if(!mi_signed_less(f->flags))goto mi_004b3e9f;
    /* 004b3c8f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3c90 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3c95u;f->boundary(0x004b3c90u,0x004b5420u,f);
    /* 004b3c95 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)mi_read((0x00bcd609u+f->r[6]),1u);
    /* 004b3c9c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3c9f */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004b3ca2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b3e9f;
    /* 004b3ca8 */
    switch(mi_read((0x004b3ea4u+f->r[0]*4u),4u)){case 0x004b3cafu:goto mi_004b3caf;case 0x004b3cc6u:goto mi_004b3cc6;case 0x004b3ce4u:goto mi_004b3ce4;case 0x004b3cf4u:goto mi_004b3cf4;case 0x004b3d0fu:goto mi_004b3d0f;case 0x004b3d23u:goto mi_004b3d23;case 0x004b3d3eu:goto mi_004b3d3e;case 0x004b3d4eu:goto mi_004b3d4e;default:abort();}
mi_004b3caf:;
    /* 004b3caf */
    a=mi_read((0x00bcd60eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60eu+f->r[6]))=(uint16_t)(t);
    /* 004b3cb6 */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3cbc */
    a=f->r[0];b=0xffffff1cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3cc1 */
    goto mi_004b3d67;
mi_004b3cc6:;
    /* 004b3cc6 */
    a=mi_read((0x00bcd60eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60eu+f->r[6]))=(uint16_t)(t);
    /* 004b3ccd */
    a=mi_read((0x00bcd60cu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60cu+f->r[6]))=(uint16_t)(t);
    /* 004b3cd4 */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3cda */
    a=f->r[0];b=0xffffff1du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3cdf */
    goto mi_004b3d67;
mi_004b3ce4:;
    /* 004b3ce4 */
    a=mi_read((0x00bcd60cu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60cu+f->r[6]))=(uint16_t)(t);
    /* 004b3ceb */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3cf1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b3cf2 */
    goto mi_004b3d67;
mi_004b3cf4:;
    /* 004b3cf4 */
    a=mi_read((0x00bcd60eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60eu+f->r[6]))=(uint16_t)(t);
    /* 004b3cfb */
    a=mi_read((0x00bcd60cu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60cu+f->r[6]))=(uint16_t)(t);
    /* 004b3d02 */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3d08 */
    a=f->r[0];b=0x000000e5u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3d0d */
    goto mi_004b3d67;
mi_004b3d0f:;
    /* 004b3d0f */
    a=mi_read((0x00bcd60eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60eu+f->r[6]))=(uint16_t)(t);
    /* 004b3d16 */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3d1c */
    a=f->r[0];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3d21 */
    goto mi_004b3d67;
mi_004b3d23:;
    /* 004b3d23 */
    a=mi_read((0x00bcd60eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60eu+f->r[6]))=(uint16_t)(t);
    /* 004b3d2a */
    a=mi_read((0x00bcd60cu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60cu+f->r[6]))=(uint16_t)(t);
    /* 004b3d31 */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3d37 */
    a=f->r[0];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3d3c */
    goto mi_004b3d67;
mi_004b3d3e:;
    /* 004b3d3e */
    a=mi_read((0x00bcd60cu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60cu+f->r[6]))=(uint16_t)(t);
    /* 004b3d45 */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3d4b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b3d4c */
    goto mi_004b3d67;
mi_004b3d4e:;
    /* 004b3d4e */
    a=mi_read((0x00bcd60eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60eu+f->r[6]))=(uint16_t)(t);
    /* 004b3d55 */
    a=mi_read((0x00bcd60cu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd60cu+f->r[6]))=(uint16_t)(t);
    /* 004b3d5c */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3d62 */
    a=f->r[0];b=0xffffff1bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004b3d67:;
    /* 004b3d67 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3d68 */
    MI_U32((0x00bcd618u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004b3d6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3d73u;f->boundary(0x004b3d6eu,0x004b52f0u,f);
    /* 004b3d73 */
    f->r[1]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3d79 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b3d7c */
    f->r[0]=mi_read((0x00af598cu+f->r[1]*4u),4u);
    /* 004b3d83 */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004b3d85 */
    if(!(f->flags&MI_ZF))goto mi_004b3d9a;
    /* 004b3d87 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,3);
    /* 004b3d8c */
    if(!(f->flags&MI_ZF))goto mi_004b3d9a;
    /* 004b3d8e */
    MI_U8((0x00bcd639u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004b3d95 */
    goto mi_004b3e5a;
mi_004b3d9a:;
    /* 004b3d9a */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 004b3d9c */
    MI_U8((0x00bcd639u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 004b3da3 */
    if((f->flags&MI_ZF))goto mi_004b3e5a;
    /* 004b3da9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00adc36cu+f->r[1]),1u))&0xffu)<<0);
    /* 004b3daf */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b3db1 */
    if((f->flags&MI_ZF))goto mi_004b3e5a;
    /* 004b3db7 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004b3db9 */
    MI_U8((0x00bcd66fu+f->r[6]))=(uint8_t)(0x00000012u);
    /* 004b3dc0 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b3e12;
    /* 004b3dc2 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004b3dc7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b3dc8 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004b3dcb */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b3e08;
    /* 004b3dcd */
    switch(mi_read((0x004b3ec4u+f->r[0]*4u),4u)){case 0x004b3dd4u:goto mi_004b3dd4;case 0x004b3de1u:goto mi_004b3de1;case 0x004b3deeu:goto mi_004b3dee;case 0x004b3dfbu:goto mi_004b3dfb;default:abort();}
mi_004b3dd4:;
    /* 004b3dd4 */
    f->r[0]=0x0000000fu;
    /* 004b3dd9 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b3ddf */
    goto mi_004b3e5a;
mi_004b3de1:;
    /* 004b3de1 */
    f->r[0]=0x0000000eu;
    /* 004b3de6 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b3dec */
    goto mi_004b3e5a;
mi_004b3dee:;
    /* 004b3dee */
    f->r[0]=0x0000001bu;
    /* 004b3df3 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b3df9 */
    goto mi_004b3e5a;
mi_004b3dfb:;
    /* 004b3dfb */
    f->r[0]=0x0000001eu;
    /* 004b3e00 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b3e06 */
    goto mi_004b3e5a;
mi_004b3e08:;
    /* 004b3e08 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b3e0a */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b3e10 */
    goto mi_004b3e5a;
mi_004b3e12:;
    /* 004b3e12 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004b3e14 */
    if(!(f->flags&MI_ZF))goto mi_004b3e1f;
    /* 004b3e16 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)(0x0000001eu);
    /* 004b3e1d */
    goto mi_004b3e5a;
mi_004b3e1f:;
    /* 004b3e1f */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004b3e21 */
    if(!(f->flags&MI_ZF))goto mi_004b3e2c;
    /* 004b3e23 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 004b3e2a */
    goto mi_004b3e5a;
mi_004b3e2c:;
    /* 004b3e2c */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b3e2e */
    if(!(f->flags&MI_ZF))goto mi_004b3e39;
    /* 004b3e30 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 004b3e37 */
    goto mi_004b3e5a;
mi_004b3e39:;
    /* 004b3e39 */
    a=(f->r[0]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004b3e3b */
    if(!(f->flags&MI_ZF))goto mi_004b3e46;
    /* 004b3e3d */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 004b3e44 */
    goto mi_004b3e5a;
mi_004b3e46:;
    /* 004b3e46 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004b3e48 */
    if(!(f->flags&MI_ZF))goto mi_004b3e53;
    /* 004b3e4a */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 004b3e51 */
    goto mi_004b3e5a;
mi_004b3e53:;
    /* 004b3e53 */
    MI_U8((0x00bcd671u+f->r[6]))=(uint8_t)(0x00000024u);
mi_004b3e5a:;
    /* 004b3e5a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b3e5c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[6]),1u))&0xffu)<<0);
    /* 004b3e62 */
    a=f->r[0];b=0xffffffb3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3e65 */
    a=f->r[0];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 004b3e68 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b3e79;
    /* 004b3e6a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b3e6c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004b3ee4u+f->r[0]),1u))&0xffu)<<0);
    /* 004b3e72 */
    switch(mi_read((0x004b3edcu+f->r[1]*4u),4u)){case 0x004b3e79u:goto mi_004b3e79;case 0x004b3e91u:goto mi_004b3e91;default:abort();}
mi_004b3e79:;
    /* 004b3e79 */
    f->r[2]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004b3e7f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[2]*2u),2u);
    /* 004b3e87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3e88 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3e89 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b3e8eu;f->boundary(0x004b3e89u,0x004b1210u,f);
    /* 004b3e8e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004b3e91:;
    /* 004b3e91 */
    MI_U16((0x00bcd610u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b3e98 */
    MI_U16((0x00bcd612u+f->r[6]))=(uint16_t)((f->r[5]&0xffffu));
mi_004b3e9f:;
    /* 004b3e9f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3ea0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3ea1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3ea2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3ea3 */
    f->r[4]+=4;return;
}

void restore_routing_C_004b3470(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b3470 */
    f->r[1]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004b3474 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3475 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3476 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b3477 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b347a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b347b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b347e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b3480 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004b3484 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b3487 */
    f->r[6]=f->r[1];
    /* 004b3489 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd634u+f->r[0]),2u);
    /* 004b3490 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd636u+f->r[0]),2u);
    /* 004b3497 */
    f->r[7]=mi_read((0x00bcd618u+f->r[0]),4u);
    /* 004b349d */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b34a0 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b34a2 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b34a5 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b34a7 */
    if(!mi_signed_less(f->flags))goto mi_004b34f5;
mi_004b34a9:;
    /* 004b34a9 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[7]*2u),2u);
    /* 004b34b1 */
    f->r[3]=f->r[1];
    /* 004b34b3 */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004b34b6 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004b34b8 */
    a=mi_read((0x007999e8u+f->r[3]*8u),2u);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 004b34c2 */
    if(!(f->flags&MI_ZF))goto mi_004b34ce;
    /* 004b34c4 */
    f->r[3]=mi_read((0x00799a9cu+f->r[6]),4u);
    /* 004b34ca */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b34cc */
    if((f->flags&MI_ZF))goto mi_004b34ff;
mi_004b34ce:;
    /* 004b34ce */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd632u+f->r[0]),2u);
    /* 004b34d5 */
    f->r[3]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b34d8 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[3]*8u);
    /* 004b34db */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004b34dd */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 004b34e0 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00c8ad38u+f->r[2]+f->r[1]*4u),1u))&0xffu)<<0);
    /* 004b34e7 */
    f->r[1]=mi_read((0x005f80fcu+f->r[3]*4u),4u);
    /* 004b34ee */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004b34f0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004b34f1 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b34f3 */
    if(mi_signed_less(f->flags))goto mi_004b34a9;
mi_004b34f5:;
    /* 004b34f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b34f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b34f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b34f8 */
    f->r[0]=0x00000002u;
    /* 004b34fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b34fe */
    f->r[4]+=4;return;
mi_004b34ff:;
    /* 004b34ff */
    a=mi_read((0x00799a18u+f->r[6]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004b3507 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b3525;
    /* 004b3509 */
    f->r[2]=f->r[3];
    /* 004b350b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b350e */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b3510 */
    a=mi_read((0x00799a18u+f->r[2]*8u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004b3519 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b3525;
    /* 004b351b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b351c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b351d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b351e */
    f->r[0]=0x00000001u;
    /* 004b3523 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b3524 */
    f->r[4]+=4;return;
mi_004b3525:;
    /* 004b3525 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b3526 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b3527 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b3528 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b352a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b352b */
    f->r[4]+=4;return;
}

void restore_routing_C_004690f0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004690f0 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004690f4 */
    f->r[2]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004690f8 */
    a=mi_read((0x00a9cc1cu+f->r[0]*2u),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00a9cc1cu+f->r[0]*2u))=(uint16_t)(t);
    /* 00469100 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00a9cc1cu+f->r[0]*2u),2u);
    /* 00469108 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0046910a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046913e;
    /* 0046910c */
    f->r[2]=mi_read((0x00bca2d8u),4u);
    /* 00469112 */
    f->r[1]=f->r[0];
    /* 00469114 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469115 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00469117 */
    f->r[0]=0x8fb823efu;
    /* 0046911c */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046911e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00469120 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00469123 */
    f->r[0]=f->r[2];
    /* 00469125 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00469128 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046912a */
    f->r[0]=f->r[1];
    /* 0046912c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046912d */
    f->r[1]=0x000000e4u;
    /* 00469132 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00469133 */
    a=f->r[1];mi_idiv32(f,a);
    /* 00469135 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469136 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046913bu;f->boundary(0x00469136u,0x00469140u,f);
    /* 0046913b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0046913e:;
    /* 0046913e */
    f->r[4]+=4;return;
}

void restore_routing_C_0051d420(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051d420 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d421 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d422 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d423 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d424 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d426 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d42b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d430 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d435u;f->boundary(0x0051d430u,0x0040f8f0u,f);
    /* 0051d435 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0051d439 */
    f->r[3]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051d43d */
    f->r[7]=mi_read((0x00eadfd4u),4u);
    /* 0051d443 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d446 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051d449 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051d44c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051d44f */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0051d454 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051d457 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051d459 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051d45b */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051d45c */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051d461 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 0051d467 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051d472 */
    f->r[1]=mi_read((0x00eae004u),4u);
    /* 0051d478 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051d47e */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0051d482 */
    f->r[0]=0x00000001u;
    /* 0051d487 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051d48d */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0051d490 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d495 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 0051d498 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 0051d49e */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0051d4a1 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 0051d4a4 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0051d4a8 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0051d4aa */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051d4ae:;
    /* 0051d4ae */
    f->r[7]=mi_read((0x00f203a0u+f->r[2]*4u),4u);
    /* 0051d4b5 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051d4b7 */
    if((f->flags&MI_ZF))goto mi_0051d6af;
    /* 0051d4bd */
    f->r[3]=mi_read((0x00eed760u+f->r[7]*4u),4u);
    /* 0051d4c4 */
    f->r[5]=(0x00eed760u+f->r[7]*4u);
    /* 0051d4cb */
    f->r[6]=(0xffffff1cu+f->r[7]);
    /* 0051d4d1 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0051d4d2 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051d4d9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d4db */
    if(!(f->flags&MI_ZF))goto mi_0051d53e;
    /* 0051d4dd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d4de */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d4e3u;f->boundary(0x0051d4deu,0x0051aff0u,f);
    /* 0051d4e3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d4e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d4e8 */
    if((f->flags&MI_ZF))goto mi_0051d539;
    /* 0051d4ea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0b6cu+f->r[7]),1u))&0xffu)<<0);
    /* 0051d4f0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d4f2 */
    if(!mi_signed_less(f->flags))goto mi_0051d508;
    /* 0051d4f4 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d4f6 */
    if((f->flags&MI_ZF))goto mi_0051d508;
    /* 0051d4f8 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d4fa */
    if(!(f->flags&MI_ZF))goto mi_0051d539;
    /* 0051d4fc */
    f->r[1]=mi_read((0x00af598cu+f->r[6]*4u),4u);
    /* 0051d503 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d506 */
    if((f->flags&MI_ZF))goto mi_0051d539;
mi_0051d508:;
    /* 0051d508 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d50a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d510;
    /* 0051d50c */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d50e */
    if(!(f->flags&MI_ZF))goto mi_0051d539;
mi_0051d510:;
    /* 0051d510 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0051d517 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d51c */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d523 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d524 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d529 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d52e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d53e;
    /* 0051d530 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d532 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d537 */
    goto mi_0051d53e;
mi_0051d539:;
    /* 0051d539 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051d53e:;
    /* 0051d53e */
    f->r[6]=(0x00000001u+f->r[7]);
    /* 0051d541 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051d548 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d54a */
    if(!(f->flags&MI_ZF))goto mi_0051d5ad;
    /* 0051d54c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d54d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d552u;f->boundary(0x0051d54du,0x0051aff0u,f);
    /* 0051d552 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d555 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d557 */
    if((f->flags&MI_ZF))goto mi_0051d5a8;
    /* 0051d559 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c51u+f->r[7]),1u))&0xffu)<<0);
    /* 0051d55f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d561 */
    if(!mi_signed_less(f->flags))goto mi_0051d577;
    /* 0051d563 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d565 */
    if((f->flags&MI_ZF))goto mi_0051d577;
    /* 0051d567 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d569 */
    if(!(f->flags&MI_ZF))goto mi_0051d5a8;
    /* 0051d56b */
    f->r[1]=mi_read((0x00af5990u+f->r[7]*4u),4u);
    /* 0051d572 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d575 */
    if((f->flags&MI_ZF))goto mi_0051d5a8;
mi_0051d577:;
    /* 0051d577 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d579 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d57f;
    /* 0051d57b */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d57d */
    if(!(f->flags&MI_ZF))goto mi_0051d5a8;
mi_0051d57f:;
    /* 0051d57f */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0051d586 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d58b */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d592 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d593 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d598 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d59d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d5ad;
    /* 0051d59f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d5a1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d5a6 */
    goto mi_0051d5ad;
mi_0051d5a8:;
    /* 0051d5a8 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051d5ad:;
    /* 0051d5ad */
    f->r[6]=(0x000000e4u+f->r[7]);
    /* 0051d5b3 */
    f->r[1]=mi_read((0x00eed760u+f->r[6]*4u),4u);
    /* 0051d5ba */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d5bc */
    if(!(f->flags&MI_ZF))goto mi_0051d61f;
    /* 0051d5be */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d5bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d5c4u;f->boundary(0x0051d5bfu,0x0051aff0u,f);
    /* 0051d5c4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d5c7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d5c9 */
    if((f->flags&MI_ZF))goto mi_0051d61a;
    /* 0051d5cb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0d34u+f->r[7]),1u))&0xffu)<<0);
    /* 0051d5d1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d5d3 */
    if(!mi_signed_less(f->flags))goto mi_0051d5e9;
    /* 0051d5d5 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d5d7 */
    if((f->flags&MI_ZF))goto mi_0051d5e9;
    /* 0051d5d9 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d5db */
    if(!(f->flags&MI_ZF))goto mi_0051d61a;
    /* 0051d5dd */
    f->r[1]=mi_read((0x00af5d1cu+f->r[7]*4u),4u);
    /* 0051d5e4 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d5e7 */
    if((f->flags&MI_ZF))goto mi_0051d61a;
mi_0051d5e9:;
    /* 0051d5e9 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d5eb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d5f1;
    /* 0051d5ed */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d5ef */
    if(!(f->flags&MI_ZF))goto mi_0051d61a;
mi_0051d5f1:;
    /* 0051d5f1 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0051d5f8 */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d5fd */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d604 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d605 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d60a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d60f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d61f;
    /* 0051d611 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d613 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d618 */
    goto mi_0051d61f;
mi_0051d61a:;
    /* 0051d61a */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051d61f:;
    /* 0051d61f */
    f->r[1]=mi_read((0xfffffffcu+f->r[5]),4u);
    /* 0051d622 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d624 */
    if(!(f->flags&MI_ZF))goto mi_0051d686;
    /* 0051d626 */
    f->r[6]=(0xffffffffu+f->r[7]);
    /* 0051d629 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d62a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d62fu;f->boundary(0x0051d62au,0x0051aff0u,f);
    /* 0051d62f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d632 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d634 */
    if((f->flags&MI_ZF))goto mi_0051d681;
    /* 0051d636 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00ee0c4fu+f->r[7]),1u))&0xffu)<<0);
    /* 0051d63c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d63e */
    if(!mi_signed_less(f->flags))goto mi_0051d654;
    /* 0051d640 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d642 */
    if((f->flags&MI_ZF))goto mi_0051d654;
    /* 0051d644 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d646 */
    if(!(f->flags&MI_ZF))goto mi_0051d681;
    /* 0051d648 */
    f->r[1]=mi_read((0x00af5988u+f->r[7]*4u),4u);
    /* 0051d64f */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d652 */
    if((f->flags&MI_ZF))goto mi_0051d681;
mi_0051d654:;
    /* 0051d654 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d656 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d65c;
    /* 0051d658 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d65a */
    if(!(f->flags&MI_ZF))goto mi_0051d681;
mi_0051d65c:;
    /* 0051d65c */
    MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(f->r[3]);
    /* 0051d65f */
    f->r[0]=mi_read((0x00eae008u),4u);
    /* 0051d664 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d66b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d66c */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d671 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d676 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d686;
    /* 0051d678 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d67a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d67f */
    goto mi_0051d686;
mi_0051d681:;
    /* 0051d681 */
    f->r[0]=mi_read((0x00eae008u),4u);
mi_0051d686:;
    /* 0051d686 */
    f->r[2]=mi_read((0x00eae00cu),4u);
    /* 0051d68c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051d68d */
    a=f->r[2];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d693 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
    /* 0051d699 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d6a3;
    /* 0051d69b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051d69d */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[2]);
mi_0051d6a3:;
    /* 0051d6a3 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0051d6a7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051d6a9 */
    if(!(f->flags&MI_ZF))goto mi_0051d4ae;
mi_0051d6af:;
    /* 0051d6af */
    f->r[2]=mi_read((0x00eed760u+f->r[1]*4u),4u);
    /* 0051d6b6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d6b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051d6b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051d6ba */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051d6bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051d6bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051d6be */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051d6c1 */
    f->r[4]+=4;return;
}

void restore_routing_C_00484990(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00484990 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484991 */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00484995 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484996 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484997 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484998 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484999 */
    f->r[7]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0048499d */
    f->r[2]=(0x00000002u+f->r[1]);
    /* 004849a0 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004849a3 */
    f->r[5]=(0x00000002u+f->r[7]);
    /* 004849a6 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004849a9 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004849ab */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004849b3 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004849b7 */
    if(!mi_signed_less(f->flags))goto mi_004849bb;
    /* 004849b9 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_004849bb:;
    /* 004849bb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004849bd */
    if(!mi_signed_less(f->flags))goto mi_004849c1;
    /* 004849bf */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004849c1:;
    /* 004849c1 */
    f->r[0]=mi_read((0x00bca2d0u),4u);
    /* 004849c6 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004849c8 */
    if(mi_signed_less(f->flags))goto mi_004849d1;
    /* 004849ca */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004849cb */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004849cf */
    f->r[5]=f->r[0];
mi_004849d1:;
    /* 004849d1 */
    f->r[0]=mi_read((0x00bca2d4u),4u);
    /* 004849d6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004849d8 */
    if(mi_signed_less(f->flags))goto mi_004849dd;
    /* 004849da */
    f->r[2]=(0xffffffffu+f->r[0]);
mi_004849dd:;
    /* 004849dd */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004849e0 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
    /* 004849e6 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004849e9 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004849ec */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 004849f2 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 004849f5 */
    f->r[0]=f->r[7];
    /* 004849f7 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004849f9 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004849fb */
    a=f->r[0];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00484a00 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00484a02 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 00484a08 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484a82;
    /* 00484a0a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00484a0c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00484a0d */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00484a11 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00484a13 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
mi_00484a19:;
    /* 00484a19 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00484a1b */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 00484a21 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484a6f;
    /* 00484a23 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00484a27 */
    f->r[5]=(0x00b4e6fcu+f->r[3]*2u);
    /* 00484a2e */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00484a30 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00484a31 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00484a33 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00484a36 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
mi_00484a3c:;
    /* 00484a3c */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[5]),2u);
    /* 00484a40 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00484a42 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484a65;
    /* 00484a44 */
    f->r[1]=f->r[6];
    /* 00484a46 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00484a49 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00484a4b */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00484a4e */
    a=mi_read((0x007999ddu+f->r[1]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00484a55 */
    if((f->flags&MI_ZF))goto mi_00484a65;
    /* 00484a57 */
    a=mi_read((0x007999f4u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00484a5f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484a65;
    /* 00484a61 */
    a=mi_read((0x00000010u+f->r[4]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000010u+f->r[4]))=(uint32_t)(t);
mi_00484a65:;
    /* 00484a65 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00484a68 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00484a69 */
    if(!(f->flags&MI_ZF))goto mi_00484a3c;
    /* 00484a6b */
    f->r[5]=mi_read((0x00000018u+f->r[4]),4u);
mi_00484a6f:;
    /* 00484a6f */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00484a73 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00484a75 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00484a76 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 00484a7c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00484a80 */
    if(!(f->flags&MI_ZF))goto mi_00484a19;
mi_00484a82:;
    /* 00484a82 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484a86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00484a87 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00484a88 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00484a89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00484a8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00484a8b */
    f->r[4]+=4;return;
}
void restore_routing_C_00484a90(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00484a90 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00484a93 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484a97 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484a98 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484a99 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484a9a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484a9b */
    f->r[7]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00484a9f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00484aa1 */
    f->r[2]=(0x00000002u+f->r[1]);
    /* 00484aa4 */
    f->r[5]=(0x00000002u+f->r[7]);
    /* 00484aa7 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00484aaa */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00484aad */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00484aaf */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00484ab3 */
    MI_U32((0x009ab9e8u))=(uint32_t)(f->r[0]);
    /* 00484ab8 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00484abc */
    if(!mi_signed_less(f->flags))goto mi_00484ac0;
    /* 00484abe */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_00484ac0:;
    /* 00484ac0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00484ac2 */
    if(!mi_signed_less(f->flags))goto mi_00484ac6;
    /* 00484ac4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00484ac6:;
    /* 00484ac6 */
    f->r[6]=mi_read((0x00bca2d0u),4u);
    /* 00484acc */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00484ace */
    if(mi_signed_less(f->flags))goto mi_00484ad7;
    /* 00484ad0 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00484ad1 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00484ad5 */
    f->r[5]=f->r[6];
mi_00484ad7:;
    /* 00484ad7 */
    f->r[6]=mi_read((0x00bca2d4u),4u);
    /* 00484add */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00484adf */
    if(mi_signed_less(f->flags))goto mi_00484ae4;
    /* 00484ae1 */
    f->r[2]=(0xffffffffu+f->r[6]);
mi_00484ae4:;
    /* 00484ae4 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00484ae7 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
    /* 00484aed */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00484af0 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 00484af3 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 00484af9 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 00484afc */
    f->r[0]=f->r[7];
    /* 00484afe */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00484b00 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00484b02 */
    a=f->r[0];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00484b07 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00484b09 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 00484b0f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00484b13 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484bcd;
    /* 00484b19 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00484b1b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00484b1c */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00484b20 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00484b22 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
mi_00484b28:;
    /* 00484b28 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00484b2a */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 00484b30 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484bb6;
    /* 00484b36 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00484b3a */
    f->r[5]=(0x00b4e6fcu+f->r[3]*2u);
    /* 00484b41 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00484b43 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00484b44 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00484b46 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[7]*1u);
    /* 00484b49 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
mi_00484b4f:;
    /* 00484b4f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[5]),2u);
    /* 00484b53 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00484b55 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484bac;
    /* 00484b57 */
    f->r[1]=f->r[2];
    /* 00484b59 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00484b5c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00484b5e */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00484b61 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[1]),1u))&0xffu)<<0);
    /* 00484b67 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00484b69 */
    if((f->flags&MI_ZF))goto mi_00484bac;
    /* 00484b6b */
    a=mi_read((0x007999f4u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00484b73 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484bac;
    /* 00484b75 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[1]),2u);
    /* 00484b7c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 00484b7f */
    f->r[2]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 00484b82 */
    f->r[0]=mi_read((0x009ab9e8u),4u);
    /* 00484b87 */
    f->r[2]=mi_read((0x0070ce18u+f->r[2]*8u),4u);
    /* 00484b8e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00484b90 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484b98;
    /* 00484b92 */
    MI_U32((0x009ab9e8u))=(uint32_t)(f->r[2]);
mi_00484b98:;
    /* 00484b98 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484b9c */
    MI_U8((0x00799a26u+f->r[1]))=(uint8_t)(0x00000032u);
    /* 00484ba3 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00484ba4 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00484ba8 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
mi_00484bac:;
    /* 00484bac */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00484baf */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00484bb0 */
    if(!(f->flags&MI_ZF))goto mi_00484b4f;
    /* 00484bb2 */
    f->r[5]=mi_read((0x0000001cu+f->r[4]),4u);
mi_00484bb6:;
    /* 00484bb6 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00484bba */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00484bbc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00484bbd */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 00484bc3 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00484bc7 */
    if(!(f->flags&MI_ZF))goto mi_00484b28;
mi_00484bcd:;
    /* 00484bcd */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484bd1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00484bd2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00484bd3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00484bd4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00484bd5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00484bd8 */
    f->r[4]+=4;return;
}
void restore_routing_C_00483910(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00483910 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00483913 */
    f->r[2]=mi_read((0x0000003cu+f->r[4]),4u);
    /* 00483917 */
    f->r[0]=mi_read((0x00000040u+f->r[4]),4u);
    /* 0048391b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048391c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048391d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048391e */
    f->r[6]=(0x00000002u+f->r[2]);
    /* 00483921 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00483923 */
    f->r[1]=(0x00000002u+f->r[0]);
    /* 00483926 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00483929 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048392c */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0048392e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048392f */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00483933 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00483937 */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048393b */
    if(!mi_signed_less(f->flags))goto mi_00483941;
    /* 0048393d */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(f->r[5]);
mi_00483941:;
    /* 00483941 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00483943 */
    if(!mi_signed_less(f->flags))goto mi_00483947;
    /* 00483945 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00483947:;
    /* 00483947 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 0048394d */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048394f */
    if(mi_signed_less(f->flags))goto mi_00483956;
    /* 00483951 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00483952 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
mi_00483956:;
    /* 00483956 */
    f->r[2]=mi_read((0x00bca2d4u),4u);
    /* 0048395c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048395e */
    if(mi_signed_less(f->flags))goto mi_00483963;
    /* 00483960 */
    f->r[1]=(0xffffffffu+f->r[2]);
mi_00483963:;
    /* 00483963 */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 00483969 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0048396c */
    f->r[3]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00483970 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00483975 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00483978 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048397b */
    f->r[2]=(0x00000000u+f->r[6]+f->r[2]*4u);
    /* 0048397e */
    f->r[6]=mi_read((0x0000004cu+f->r[4]),4u);
    /* 00483982 */
    f->r[7]=f->r[6];
    /* 00483984 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483986 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00483988 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 0048398e */
    a=f->r[7];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00483994 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00483996 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0048399a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483df4;
    /* 004839a0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004839a2 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004839a3 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004839a7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004839a9 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
mi_004839af:;
    /* 004839af */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004839b1 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[6]);
    /* 004839b7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483dd1;
    /* 004839bd */
    f->r[0]=(0x00b4e6fcu+f->r[2]*2u);
    /* 004839c4 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004839c8 */
    f->r[0]=f->r[3];
    /* 004839ca */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004839cc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004839cd */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004839d1 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004839d3 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004839d5 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004839db */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
mi_004839e0:;
    /* 004839e0 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004839e4 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[1]),2u);
    /* 004839e7 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004839e9 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004839ed */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483da5;
    /* 004839f3 */
    f->r[0]=f->r[7];
    /* 004839f5 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004839f8 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004839fa */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004839fd */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00483a01 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[0]),1u))&0xffu)<<0);
    /* 00483a07 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00483a09 */
    if((f->flags&MI_ZF))goto mi_00483da5;
    /* 00483a0f */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999f4u+f->r[0]),2u))&0xffffu)<<0);
    /* 00483a16 */
    a=(f->r[2]&0xffffu);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00483a19 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483da5;
    /* 00483a1f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[0]),2u);
    /* 00483a26 */
    f->r[3]=mi_read((0x00000028u+f->r[4]),4u);
    /* 00483a2a */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00483a2b */
    a=f->r[1];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 00483a2e */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00483a32 */
    if(!mi_signed_less(f->flags))goto mi_00483a35;
    /* 00483a34 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_00483a35:;
    /* 00483a35 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 00483a38 */
    f->r[3]=(0x00000000u+f->r[1]+f->r[6]*2u);
    /* 00483a3b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 00483a3e */
    a=f->r[3];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 00483a41 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*1u);
    /* 00483a44 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00483a47 */
    f->r[6]=mi_read((0x0070cdf4u+f->r[3]),4u);
    /* 00483a4d */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00483a51 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483a53 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00483a55 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00483a59 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00483a5d */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00483a61 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483a6d;
    /* 00483a63 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00483a66 */
    if(!mi_signed_less(f->flags))goto mi_00483a6d;
    /* 00483a68 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00483a69 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[6]);
mi_00483a6d:;
    /* 00483a6d */
    f->r[1]=mi_read((0x00f53dd8u),4u);
    /* 00483a73 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00483a75 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483bc5;
    /* 00483a7b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00f53ddcu);
    /* 00483a83 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_00483a87:;
    /* 00483a87 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00483a8b */
    f->r[1]=mi_read((0x00000000u+f->r[1]),4u);
    /* 00483a8d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00483a8f */
    if((f->flags&MI_ZF))goto mi_00483bab;
    /* 00483a95 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00483a96 */
    a=f->r[1];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 00483a99 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00483af3;
    /* 00483a9b */
    switch(mi_read((0x00483e00u+f->r[1]*4u),4u)){case 0x00483aa2u:goto mi_00483aa2;case 0x00483aa6u:goto mi_00483aa6;case 0x00483aadu:goto mi_00483aad;case 0x00483ab4u:goto mi_00483ab4;case 0x00483abbu:goto mi_00483abb;case 0x00483ac2u:goto mi_00483ac2;case 0x00483ac9u:goto mi_00483ac9;case 0x00483ad0u:goto mi_00483ad0;case 0x00483ad7u:goto mi_00483ad7;case 0x00483adeu:goto mi_00483ade;case 0x00483ae5u:goto mi_00483ae5;case 0x00483aecu:goto mi_00483aec;case 0x00483af3u:goto mi_00483af3;default:abort();}
mi_00483aa2:;
    /* 00483aa2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00483aa4 */
    goto mi_00483af6;
mi_00483aa6:;
    /* 00483aa6 */
    f->r[2]=0x00000005u;
    /* 00483aab */
    goto mi_00483af6;
mi_00483aad:;
    /* 00483aad */
    f->r[2]=0x00000001u;
    /* 00483ab2 */
    goto mi_00483af6;
mi_00483ab4:;
    /* 00483ab4 */
    f->r[2]=0x00000002u;
    /* 00483ab9 */
    goto mi_00483af6;
mi_00483abb:;
    /* 00483abb */
    f->r[2]=0x00000003u;
    /* 00483ac0 */
    goto mi_00483af6;
mi_00483ac2:;
    /* 00483ac2 */
    f->r[2]=0x00000004u;
    /* 00483ac7 */
    goto mi_00483af6;
mi_00483ac9:;
    /* 00483ac9 */
    f->r[2]=0x00000007u;
    /* 00483ace */
    goto mi_00483af6;
mi_00483ad0:;
    /* 00483ad0 */
    f->r[2]=0x00000006u;
    /* 00483ad5 */
    goto mi_00483af6;
mi_00483ad7:;
    /* 00483ad7 */
    f->r[2]=0x0000000au;
    /* 00483adc */
    goto mi_00483af6;
mi_00483ade:;
    /* 00483ade */
    f->r[2]=0x0000000bu;
    /* 00483ae3 */
    goto mi_00483af6;
mi_00483ae5:;
    /* 00483ae5 */
    f->r[2]=0x00000008u;
    /* 00483aea */
    goto mi_00483af6;
mi_00483aec:;
    /* 00483aec */
    f->r[2]=0x00000009u;
    /* 00483af1 */
    goto mi_00483af6;
mi_00483af3:;
    /* 00483af3 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_00483af6:;
    /* 00483af6 */
    f->r[1]=mi_read((0x00000048u+f->r[4]),4u);
    /* 00483afa */
    f->r[6]=f->r[1];
    /* 00483afc */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00483aff */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483b01 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[6]*4u);
    /* 00483b04 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((mi_read((0x00799a2eu+f->r[6]*2u),2u))&0xffffu)<<0);
    /* 00483b0c */
    a=(f->r[5]&0xffffu);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00483b0f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483bab;
    /* 00483b15 */
    f->r[1]=mi_read((0x00000050u+f->r[4]),4u);
    /* 00483b19 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00483b1b */
    if((f->flags&MI_ZF))goto mi_00483bab;
    /* 00483b21 */
    f->r[0]=f->r[7];
    /* 00483b23 */
    f->r[1]=mi_read((0x00000038u+f->r[4]),4u);
    /* 00483b27 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00483b2a */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00483b2c */
    f->r[7]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 00483b2f */
    f->r[3]=(f->r[3]&0xffff0000u)|(((mi_read((0x00799a2cu+f->r[7]*2u),2u))&0xffffu)<<0);
    /* 00483b37 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[3]&0xffffu);
    /* 00483b3a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00483b3c */
    f->r[0]=mi_read((0x0000003cu+f->r[4]),4u);
    /* 00483b40 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00483b42 */
    if(!mi_signed_less(f->flags))goto mi_00483b46;
    /* 00483b44 */
    f->r[1]=f->r[0];
mi_00483b46:;
    /* 00483b46 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00483b48 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483b9f;
    /* 00483b4a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[5]&0xffffu);
    /* 00483b4d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00483b4f */
    if(mi_signed_less(f->flags))goto mi_00483b61;
    /* 00483b51 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 00483b54 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00483b9b;
    /* 00483b56 */
    switch(mi_read((0x00483e4cu+f->r[2]*4u),4u)){case 0x00483b5du:goto mi_00483b5d;case 0x00483b73u:goto mi_00483b73;default:abort();}
mi_00483b5d:;
    /* 00483b5d */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 00483b5f */
    goto mi_00483b73;
mi_00483b61:;
    /* 00483b61 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 00483b64 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00483b9b;
    /* 00483b66 */
    switch(mi_read((0x00483e6cu+f->r[2]*4u),4u)){case 0x00483b6du:goto mi_00483b6d;case 0x00483b87u:goto mi_00483b87;default:abort();}
mi_00483b6d:;
    /* 00483b6d */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 00483b6f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00483b71 */
    if(mi_signed_less(f->flags))goto mi_00483b87;
mi_00483b73:;
    /* 00483b73 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00483b75 */
    MI_U16((0x00799a2cu+f->r[7]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00483b7d */
    a=mi_read((0x00799a2eu+f->r[6]*2u),2u);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799a2eu+f->r[6]*2u))=(uint16_t)(t);
    /* 00483b85 */
    goto mi_00483b9b;
mi_00483b87:;
    /* 00483b87 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00483b89 */
    MI_U16((0x00799a2cu+f->r[7]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 00483b91 */
    MI_U16((0x00799a2eu+f->r[6]*2u))=(uint16_t)(0x00000000u);
mi_00483b9b:;
    /* 00483b9b */
    a=mi_read((0x00000050u+f->r[4]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000050u+f->r[4]))=(uint32_t)(t);
mi_00483b9f:;
    /* 00483b9f */
    f->r[3]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00483ba3 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00483ba7 */
    f->r[7]=mi_read((0x00000018u+f->r[4]),4u);
mi_00483bab:;
    /* 00483bab */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00483baf */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 00483bb3 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483bb6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00483bb7 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00483bbb */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00483bbf */
    if(!(f->flags&MI_ZF))goto mi_00483a87;
mi_00483bc5:;
    /* 00483bc5 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[0]),1u))&0xffu)<<0);
    /* 00483bcb */
    f->r[2]=mi_read((0x0070ce04u+f->r[3]),4u);
    /* 00483bd1 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00483bd4 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00483bd9;
    /* 00483bd6 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
mi_00483bd9:;
    /* 00483bd9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483bdb */
    if((f->flags&MI_ZF))goto mi_00483c3d;
    /* 00483bdd */
    f->r[6]=mi_read((0x00000048u+f->r[4]),4u);
    /* 00483be1 */
    f->r[1]=f->r[6];
    /* 00483be3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483be6 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00483be8 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483beb */
    MI_U16((0x00799a7cu+f->r[1]))=(uint16_t)(0x0000000au);
    /* 00483bf4 */
    f->r[7]=(f->r[7]&0xffff0000u)|(((mi_read((0x00799a3eu+f->r[1]),2u))&0xffffu)<<0);
    /* 00483bfb */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00483bfe */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483c3d;
    /* 00483c00 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((mi_read((0x00799a3cu+f->r[0]),2u))&0xffffu)<<0);
    /* 00483c07 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483c09 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 00483c0c */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00483c0e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483c10 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483c3d;
    /* 00483c12 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[7]&0xffffu);
    /* 00483c15 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00483c17 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483c2d;
    /* 00483c19 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483c1b */
    MI_U16((0x00799a3cu+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483c22 */
    MI_U16((0x00799a3eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 00483c2b */
    goto mi_00483c3d;
mi_00483c2d:;
    /* 00483c2d */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483c2f */
    MI_U16((0x00799a3cu+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483c36 */
    a=mi_read((0x00799a3eu+f->r[1]),2u);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799a3eu+f->r[1]))=(uint16_t)(t);
mi_00483c3d:;
    /* 00483c3d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[0]),1u))&0xffu)<<0);
    /* 00483c43 */
    f->r[2]=mi_read((0x0070cdf8u+f->r[3]),4u);
    /* 00483c49 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00483c4c */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00483c51;
    /* 00483c4e */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
mi_00483c51:;
    /* 00483c51 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483c53 */
    if((f->flags&MI_ZF))goto mi_00483cb5;
    /* 00483c55 */
    f->r[6]=mi_read((0x00000048u+f->r[4]),4u);
    /* 00483c59 */
    f->r[1]=f->r[6];
    /* 00483c5b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483c5e */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00483c60 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483c63 */
    MI_U16((0x00799a76u+f->r[1]))=(uint16_t)(0x0000000au);
    /* 00483c6c */
    f->r[7]=(f->r[7]&0xffff0000u)|(((mi_read((0x00799a40u+f->r[1]),2u))&0xffffu)<<0);
    /* 00483c73 */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00483c76 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483cb5;
    /* 00483c78 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((mi_read((0x00799a3eu+f->r[0]),2u))&0xffffu)<<0);
    /* 00483c7f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483c81 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 00483c84 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00483c86 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483c88 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483cb5;
    /* 00483c8a */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[7]&0xffffu);
    /* 00483c8d */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00483c8f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483ca5;
    /* 00483c91 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483c93 */
    MI_U16((0x00799a3eu+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483c9a */
    MI_U16((0x00799a40u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 00483ca3 */
    goto mi_00483cb5;
mi_00483ca5:;
    /* 00483ca5 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483ca7 */
    MI_U16((0x00799a3eu+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483cae */
    a=mi_read((0x00799a40u+f->r[1]),2u);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799a40u+f->r[1]))=(uint16_t)(t);
mi_00483cb5:;
    /* 00483cb5 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[0]),1u))&0xffu)<<0);
    /* 00483cbb */
    f->r[2]=mi_read((0x0070cdfcu+f->r[3]),4u);
    /* 00483cc1 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00483cc4 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00483cc9;
    /* 00483cc6 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
mi_00483cc9:;
    /* 00483cc9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483ccb */
    if((f->flags&MI_ZF))goto mi_00483d2d;
    /* 00483ccd */
    f->r[6]=mi_read((0x00000048u+f->r[4]),4u);
    /* 00483cd1 */
    f->r[1]=f->r[6];
    /* 00483cd3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483cd6 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00483cd8 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483cdb */
    MI_U16((0x00799a7au+f->r[1]))=(uint16_t)(0x0000000au);
    /* 00483ce4 */
    f->r[7]=(f->r[7]&0xffff0000u)|(((mi_read((0x00799a42u+f->r[1]),2u))&0xffffu)<<0);
    /* 00483ceb */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00483cee */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483d2d;
    /* 00483cf0 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((mi_read((0x00799a40u+f->r[0]),2u))&0xffffu)<<0);
    /* 00483cf7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483cf9 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 00483cfc */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00483cfe */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483d00 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483d2d;
    /* 00483d02 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[7]&0xffffu);
    /* 00483d05 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00483d07 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483d1d;
    /* 00483d09 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483d0b */
    MI_U16((0x00799a40u+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483d12 */
    MI_U16((0x00799a42u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 00483d1b */
    goto mi_00483d2d;
mi_00483d1d:;
    /* 00483d1d */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483d1f */
    MI_U16((0x00799a40u+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483d26 */
    a=mi_read((0x00799a42u+f->r[1]),2u);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799a42u+f->r[1]))=(uint16_t)(t);
mi_00483d2d:;
    /* 00483d2d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[0]),1u))&0xffu)<<0);
    /* 00483d33 */
    f->r[2]=mi_read((0x0070ce00u+f->r[3]),4u);
    /* 00483d39 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00483d3c */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00483d41;
    /* 00483d3e */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
mi_00483d41:;
    /* 00483d41 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483d43 */
    if((f->flags&MI_ZF))goto mi_00483da5;
    /* 00483d45 */
    f->r[6]=mi_read((0x00000048u+f->r[4]),4u);
    /* 00483d49 */
    f->r[1]=f->r[6];
    /* 00483d4b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483d4e */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00483d50 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483d53 */
    MI_U16((0x00799a78u+f->r[1]))=(uint16_t)(0x0000000au);
    /* 00483d5c */
    f->r[7]=(f->r[7]&0xffff0000u)|(((mi_read((0x00799a44u+f->r[1]),2u))&0xffffu)<<0);
    /* 00483d63 */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00483d66 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483da5;
    /* 00483d68 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((mi_read((0x00799a42u+f->r[0]),2u))&0xffffu)<<0);
    /* 00483d6f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483d71 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 00483d74 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00483d76 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00483d78 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483da5;
    /* 00483d7a */
    f->r[3]=(uint32_t)(int32_t)(int16_t)(f->r[7]&0xffffu);
    /* 00483d7d */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00483d7f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483d95;
    /* 00483d81 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483d83 */
    MI_U16((0x00799a42u+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483d8a */
    MI_U16((0x00799a44u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 00483d93 */
    goto mi_00483da5;
mi_00483d95:;
    /* 00483d95 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00483d97 */
    MI_U16((0x00799a42u+f->r[0]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00483d9e */
    a=mi_read((0x00799a44u+f->r[1]),2u);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799a44u+f->r[1]))=(uint16_t)(t);
mi_00483da5:;
    /* 00483da5 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00483da9 */
    f->r[0]=mi_read((0x00000030u+f->r[4]),4u);
    /* 00483dad */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00483db0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00483db1 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00483db5 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00483db9 */
    if(!(f->flags&MI_ZF))goto mi_004839e0;
    /* 00483dbf */
    f->r[7]=mi_read((0x00000040u+f->r[4]),4u);
    /* 00483dc3 */
    f->r[2]=mi_read((0x00bc9768u),4u);
    /* 00483dc9 */
    f->r[3]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00483dcd */
    f->r[6]=mi_read((0x0000004cu+f->r[4]),4u);
mi_00483dd1:;
    /* 00483dd1 */
    f->r[0]=mi_read((0x00000034u+f->r[4]),4u);
    /* 00483dd5 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483dd7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00483dd8 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 00483dde */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00483de2 */
    if(!(f->flags&MI_ZF))goto mi_004839af;
    /* 00483de8 */
    f->r[0]=mi_read((0x00000028u+f->r[4]),4u);
    /* 00483dec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00483ded */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00483dee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00483def */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00483df0 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00483df3 */
    f->r[4]+=4;return;
mi_00483df4:;
    /* 00483df4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00483df5 */
    f->r[0]=f->r[5];
    /* 00483df7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00483df8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00483df9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00483dfa */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00483dfd */
    f->r[4]+=4;return;
}
void restore_routing_C_00483e90(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00483e90 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483e91 */
    f->r[0]=mi_read((0x0078d7b0u),4u);
    /* 00483e96 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00483e98 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483e9e;
    /* 00483e9a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00483e9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00483e9d */
    f->r[4]+=4;return;
mi_00483e9e:;
    /* 00483e9e */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00483ea2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483ea3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483ea4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483ea5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483ea6 */
    f->r[7]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00483eaa */
    f->r[2]=(0x00000002u+f->r[1]);
    /* 00483ead */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00483eb0 */
    f->r[5]=(0x00000002u+f->r[7]);
    /* 00483eb3 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00483eb6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00483eb8 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00483ec0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00483ec4 */
    if(!mi_signed_less(f->flags))goto mi_00483ec8;
    /* 00483ec6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_00483ec8:;
    /* 00483ec8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00483eca */
    if(!mi_signed_less(f->flags))goto mi_00483ece;
    /* 00483ecc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00483ece:;
    /* 00483ece */
    f->r[6]=mi_read((0x00bca2d0u),4u);
    /* 00483ed4 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00483ed6 */
    if(mi_signed_less(f->flags))goto mi_00483edf;
    /* 00483ed8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00483ed9 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00483edd */
    f->r[5]=f->r[6];
mi_00483edf:;
    /* 00483edf */
    f->r[6]=mi_read((0x00bca2d4u),4u);
    /* 00483ee5 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00483ee7 */
    if(mi_signed_less(f->flags))goto mi_00483eec;
    /* 00483ee9 */
    f->r[2]=(0xffffffffu+f->r[6]);
mi_00483eec:;
    /* 00483eec */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00483eef */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
    /* 00483ef5 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00483ef8 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 00483efb */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 00483f01 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 00483f04 */
    f->r[0]=f->r[7];
    /* 00483f06 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00483f08 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00483f0a */
    a=f->r[0];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00483f0f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00483f11 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 00483f17 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483fa1;
    /* 00483f1d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00483f1f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00483f20 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00483f24 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00483f26 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
mi_00483f2c:;
    /* 00483f2c */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00483f2e */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 00483f34 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483f8e;
    /* 00483f36 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00483f3a */
    f->r[5]=(0x00b4e6fcu+f->r[3]*2u);
    /* 00483f41 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00483f43 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00483f44 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00483f46 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 00483f49 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
mi_00483f4f:;
    /* 00483f4f */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[5]),2u);
    /* 00483f53 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00483f55 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483f84;
    /* 00483f57 */
    f->r[1]=f->r[6];
    /* 00483f59 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483f5c */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00483f5e */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00483f61 */
    a=mi_read((0x007999ddu+f->r[1]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00483f68 */
    if((f->flags&MI_ZF))goto mi_00483f84;
    /* 00483f6a */
    a=mi_read((0x007999f4u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00483f72 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483f84;
    /* 00483f74 */
    MI_U8((0x00799a4fu+f->r[1]))=(uint8_t)(0x00000060u);
    /* 00483f7b */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00483f7f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00483f80 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00483f84:;
    /* 00483f84 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00483f87 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00483f88 */
    if(!(f->flags&MI_ZF))goto mi_00483f4f;
    /* 00483f8a */
    f->r[5]=mi_read((0x00000018u+f->r[4]),4u);
mi_00483f8e:;
    /* 00483f8e */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00483f92 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00483f94 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00483f95 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00483f99 */
    if(!(f->flags&MI_ZF))goto mi_00483f2c;
    /* 00483f9b */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
mi_00483fa1:;
    /* 00483fa1 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00483fa5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00483fa6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00483fa7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00483fa8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00483fa9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00483faa */
    f->r[4]+=4;return;
}
static void services_house_visit(struct restore_routing_frame *f,uint32_t service_field)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00484450 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484451 */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00484455 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484456 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484457 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484458 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484459 */
    f->r[7]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0048445d */
    f->r[2]=(0x00000002u+f->r[1]);
    /* 00484460 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00484463 */
    f->r[5]=(0x00000002u+f->r[7]);
    /* 00484466 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00484469 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0048446b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00484473 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00484477 */
    if(!mi_signed_less(f->flags))goto mi_0048447b;
    /* 00484479 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0048447b:;
    /* 0048447b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0048447d */
    if(!mi_signed_less(f->flags))goto mi_00484481;
    /* 0048447f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00484481:;
    /* 00484481 */
    f->r[6]=mi_read((0x00bca2d0u),4u);
    /* 00484487 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00484489 */
    if(mi_signed_less(f->flags))goto mi_00484492;
    /* 0048448b */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0048448c */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00484490 */
    f->r[5]=f->r[6];
mi_00484492:;
    /* 00484492 */
    f->r[6]=mi_read((0x00bca2d4u),4u);
    /* 00484498 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048449a */
    if(mi_signed_less(f->flags))goto mi_0048449f;
    /* 0048449c */
    f->r[2]=(0xffffffffu+f->r[6]);
mi_0048449f:;
    /* 0048449f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004844a2 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
    /* 004844a8 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004844ab */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004844ae */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 004844b4 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 004844b7 */
    f->r[0]=f->r[7];
    /* 004844b9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004844bb */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004844bd */
    a=f->r[0];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004844c2 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004844c4 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 004844ca */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484554;
    /* 004844d0 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004844d2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004844d3 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004844d7 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004844d9 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
mi_004844df:;
    /* 004844df */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004844e1 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 004844e7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484541;
    /* 004844e9 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004844ed */
    f->r[5]=(0x00b4e6fcu+f->r[3]*2u);
    /* 004844f4 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004844f6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004844f7 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004844f9 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[7]*1u);
    /* 004844fc */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
mi_00484502:;
    /* 00484502 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[5]),2u);
    /* 00484506 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00484508 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484537;
    /* 0048450a */
    f->r[1]=f->r[6];
    /* 0048450c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048450f */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00484511 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00484514 */
    a=mi_read((0x007999ddu+f->r[1]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0048451b */
    if((f->flags&MI_ZF))goto mi_00484537;
    /* 0048451d */
    a=mi_read((0x007999f4u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00484525 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484537;
    /* 00484527 */
    MI_U8((service_field+f->r[1]))=(uint8_t)(0x00000060u);
    /* 0048452e */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484532 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00484533 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00484537:;
    /* 00484537 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0048453a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0048453b */
    if(!(f->flags&MI_ZF))goto mi_00484502;
    /* 0048453d */
    f->r[5]=mi_read((0x00000018u+f->r[4]),4u);
mi_00484541:;
    /* 00484541 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00484545 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00484547 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00484548 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[3]);
    /* 0048454e */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00484552 */
    if(!(f->flags&MI_ZF))goto mi_004844df;
mi_00484554:;
    /* 00484554 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484558 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00484559 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048455a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048455b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048455c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0048455d */
    f->r[4]+=4;return;
}
void restore_routing_C_00482ef0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00482ef0 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00482ef4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482ef5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482ef6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482ef7 */
    f->r[6]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00482efb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482efc */
    f->r[1]=(0x00000002u+f->r[0]);
    /* 00482eff */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00482f02 */
    f->r[7]=(0x00000002u+f->r[6]);
    /* 00482f05 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00482f08 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00482f0a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00482f0e */
    if(!mi_signed_less(f->flags))goto mi_00482f12;
    /* 00482f10 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_00482f12:;
    /* 00482f12 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00482f14 */
    if(!mi_signed_less(f->flags))goto mi_00482f18;
    /* 00482f16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00482f18:;
    /* 00482f18 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 00482f1e */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00482f20 */
    if(mi_signed_less(f->flags))goto mi_00482f29;
    /* 00482f22 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00482f23 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00482f27 */
    f->r[7]=f->r[2];
mi_00482f29:;
    /* 00482f29 */
    f->r[2]=mi_read((0x00bca2d4u),4u);
    /* 00482f2f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00482f31 */
    if(mi_signed_less(f->flags))goto mi_00482f36;
    /* 00482f33 */
    f->r[1]=(0xffffffffu+f->r[2]);
mi_00482f36:;
    /* 00482f36 */
    f->r[3]=mi_read((0x00bca2d8u),4u);
    /* 00482f3c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00482f3f */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00482f44 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00482f47 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00482f4a */
    f->r[5]=(0x00000000u+f->r[3]+f->r[2]*4u);
    /* 00482f4d */
    f->r[3]=f->r[6];
    /* 00482f4f */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00482f51 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00482f53 */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00482f59 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00482f5b */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 00482f61 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482ff7;
    /* 00482f67 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00482f69 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00482f6a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00482f6e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482f70 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
mi_00482f76:;
    /* 00482f76 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00482f78 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[6]);
    /* 00482f7e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482fe4;
    /* 00482f80 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00482f84 */
    f->r[7]=(0x00b4e6fcu+f->r[5]*2u);
    /* 00482f8b */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00482f8d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00482f8e */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00482f90 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 00482f93 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
mi_00482f98:;
    /* 00482f98 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[7]),2u);
    /* 00482f9b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00482f9d */
    if((f->flags&MI_ZF))goto mi_00482fda;
    /* 00482f9f */
    f->r[0]=f->r[1];
    /* 00482fa1 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00482fa4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00482fa6 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00482fa9 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[0]),2u);
    /* 00482fb0 */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00482fb3 */
    if(mi_signed_less(f->flags))goto mi_00482fda;
    /* 00482fb5 */
    a=f->r[1];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 00482fb8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482fda;
    /* 00482fba */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999d8u+f->r[0]),1u))&0xffu)<<0);
    /* 00482fc0 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00482fc2 */
    if((f->flags&MI_ZF))goto mi_00482fda;
    /* 00482fc4 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00482fc8 */
    a=mi_read((0x00799ad4u+f->r[0]),4u);b=f->r[1];t=mi_alu(f,a,b,32,1);MI_U32((0x00799ad4u+f->r[0]))=(uint32_t)(t);
    /* 00482fce */
    if(!(f->flags&MI_SF))goto mi_00482fda;
    /* 00482fd0 */
    MI_U32((0x00799ad4u+f->r[0]))=(uint32_t)(0x00000000u);
mi_00482fda:;
    /* 00482fda */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00482fdd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00482fde */
    if(!(f->flags&MI_ZF))goto mi_00482f98;
    /* 00482fe0 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
mi_00482fe4:;
    /* 00482fe4 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00482fe8 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00482fea */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00482feb */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00482fef */
    if(!(f->flags&MI_ZF))goto mi_00482f76;
    /* 00482ff1 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
mi_00482ff7:;
    /* 00482ff7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00482ff8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00482ff9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00482ffa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00482ffb */
    f->r[4]+=4;return;
}
void restore_routing_C_00482dd0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00482dd0 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00482dd4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482dd5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482dd6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482dd7 */
    f->r[6]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00482ddb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482ddc */
    f->r[1]=(0x00000002u+f->r[0]);
    /* 00482ddf */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00482de2 */
    f->r[7]=(0x00000002u+f->r[6]);
    /* 00482de5 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00482de8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00482dea */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00482dee */
    if(!mi_signed_less(f->flags))goto mi_00482df2;
    /* 00482df0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_00482df2:;
    /* 00482df2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00482df4 */
    if(!mi_signed_less(f->flags))goto mi_00482df8;
    /* 00482df6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00482df8:;
    /* 00482df8 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 00482dfe */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00482e00 */
    if(mi_signed_less(f->flags))goto mi_00482e09;
    /* 00482e02 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00482e03 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00482e07 */
    f->r[7]=f->r[2];
mi_00482e09:;
    /* 00482e09 */
    f->r[2]=mi_read((0x00bca2d4u),4u);
    /* 00482e0f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00482e11 */
    if(mi_signed_less(f->flags))goto mi_00482e16;
    /* 00482e13 */
    f->r[1]=(0xffffffffu+f->r[2]);
mi_00482e16:;
    /* 00482e16 */
    f->r[3]=mi_read((0x00bca2d8u),4u);
    /* 00482e1c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00482e1f */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00482e24 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00482e27 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00482e2a */
    f->r[5]=(0x00000000u+f->r[3]+f->r[2]*4u);
    /* 00482e2d */
    f->r[3]=f->r[6];
    /* 00482e2f */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00482e31 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00482e33 */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00482e39 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00482e3b */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 00482e41 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482edb;
    /* 00482e47 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00482e49 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00482e4a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00482e4e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482e50 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
mi_00482e56:;
    /* 00482e56 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00482e58 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[6]);
    /* 00482e5e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482ec4;
    /* 00482e60 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00482e64 */
    f->r[7]=(0x00b4e6fcu+f->r[5]*2u);
    /* 00482e6b */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00482e6d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00482e6e */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00482e70 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 00482e73 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
mi_00482e78:;
    /* 00482e78 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[7]),2u);
    /* 00482e7b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00482e7d */
    if((f->flags&MI_ZF))goto mi_00482eba;
    /* 00482e7f */
    f->r[0]=f->r[1];
    /* 00482e81 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00482e84 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00482e86 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00482e89 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[0]),2u);
    /* 00482e90 */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00482e93 */
    if(mi_signed_less(f->flags))goto mi_00482eba;
    /* 00482e95 */
    a=f->r[1];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 00482e98 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482eba;
    /* 00482e9a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999d8u+f->r[0]),1u))&0xffu)<<0);
    /* 00482ea0 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00482ea2 */
    if((f->flags&MI_ZF))goto mi_00482eba;
    /* 00482ea4 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00482ea8 */
    a=mi_read((0x00799ac0u+f->r[0]),4u);b=f->r[1];t=mi_alu(f,a,b,32,1);MI_U32((0x00799ac0u+f->r[0]))=(uint32_t)(t);
    /* 00482eae */
    if(!(f->flags&MI_SF))goto mi_00482eba;
    /* 00482eb0 */
    MI_U32((0x00799ac0u+f->r[0]))=(uint32_t)(0x00000000u);
mi_00482eba:;
    /* 00482eba */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00482ebd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00482ebe */
    if(!(f->flags&MI_ZF))goto mi_00482e78;
    /* 00482ec0 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
mi_00482ec4:;
    /* 00482ec4 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00482ec8 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00482eca */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00482ecb */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 00482ed1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00482ed5 */
    if(!(f->flags&MI_ZF))goto mi_00482e56;
mi_00482edb:;
    /* 00482edb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00482edc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00482edd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00482ede */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00482ee0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00482ee1 */
    f->r[4]+=4;return;
}
void restore_routing_C_0042ff20(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0042ff20 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0042ff24 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042ff25 */
    f->r[3]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0042ff29 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042ff2a */
    f->r[2]=(0x00000002u+f->r[0]);
    /* 0042ff2d */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0042ff30 */
    f->r[5]=(0x00000002u+f->r[3]);
    /* 0042ff33 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0042ff36 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042ff37 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042ff38 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0042ff3a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0042ff3e */
    if(!mi_signed_less(f->flags))goto mi_0042ff42;
    /* 0042ff40 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_0042ff42:;
    /* 0042ff42 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0042ff44 */
    if(!mi_signed_less(f->flags))goto mi_0042ff48;
    /* 0042ff46 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0042ff48:;
    /* 0042ff48 */
    f->r[1]=mi_read((0x00bca2d0u),4u);
    /* 0042ff4e */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0042ff50 */
    if(mi_signed_less(f->flags))goto mi_0042ff55;
    /* 0042ff52 */
    f->r[5]=(0xffffffffu+f->r[1]);
mi_0042ff55:;
    /* 0042ff55 */
    f->r[1]=mi_read((0x00bca2d4u),4u);
    /* 0042ff5b */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0042ff5d */
    if(mi_signed_less(f->flags))goto mi_0042ff64;
    /* 0042ff5f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0042ff60 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0042ff64:;
    /* 0042ff64 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0042ff67 */
    f->r[7]=f->r[3];
    /* 0042ff69 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0042ff6b */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 0042ff70 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0042ff73 */
    a=f->r[7];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0042ff79 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0042ff7c */
    f->r[1]=mi_read((0x00bca2d8u),4u);
    /* 0042ff82 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 0042ff85 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0042ff89 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0042ff8b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0042ff8d */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[6]);
    /* 0042ff93 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0043002c;
mi_0042ff99:;
    /* 0042ff99 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0042ff9b */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[3]);
    /* 0042ffa1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0043000d;
mi_0042ffa3:;
    /* 0042ffa3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[6]*2u),2u);
    /* 0042ffab */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0042ffad */
    if((f->flags&MI_ZF))goto mi_0042fff7;
    /* 0042ffaf */
    f->r[0]=f->r[1];
    /* 0042ffb1 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0042ffb4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0042ffb6 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0042ffb9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999d8u+f->r[0]),1u))&0xffu)<<0);
    /* 0042ffbf */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0042ffc1 */
    if((f->flags&MI_ZF))goto mi_0042fff7;
    /* 0042ffc3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[0]),2u);
    /* 0042ffca */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0042ffcd */
    if(mi_signed_less(f->flags))goto mi_0042fff7;
    /* 0042ffcf */
    a=f->r[2];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 0042ffd2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0042fff7;
    /* 0042ffd4 */
    a=mi_read((0x007999f4u+f->r[0]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0042ffdc */
    if((f->flags&MI_ZF))goto mi_0042fff7;
    /* 0042ffde */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a5fu+f->r[0]),1u))&0xffu)<<0);
    /* 0042ffe4 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0042ffe6 */
    if(!(f->flags&MI_ZF))goto mi_0042fff7;
    /* 0042ffe8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0042ffe9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0042ffeeu;f->boundary(0x0042ffe9u,0x0042fb30u,f);
    /* 0042ffee */
    f->r[6]=mi_read((0x00bc9768u),4u);
    /* 0042fff4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0042fff7:;
    /* 0042fff7 */
    f->r[0]=mi_read((0x00d3b4e4u),4u);
    /* 0042fffc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0042fffd */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0042fffe */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00430000 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
    /* 00430005 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[6]);
    /* 0043000b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0042ffa3;
mi_0043000d:;
    /* 0043000d */
    f->r[0]=mi_read((0x00d3b4e0u),4u);
    /* 00430012 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00430016 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00430017 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00430019 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043001b */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00430020 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[6]);
    /* 00430026 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0042ff99;
mi_0043002c:;
    /* 0043002c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043002d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043002e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043002f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00430030 */
    f->r[4]+=4;return;
}
void restore_routing_C_00484890(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00484890 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00484894 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484895 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484896 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484897 */
    f->r[6]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0048489b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048489c */
    f->r[1]=(0x00000004u+f->r[0]);
    /* 0048489f */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004848a2 */
    f->r[7]=(0x00000004u+f->r[6]);
    /* 004848a5 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004848a8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004848aa */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004848ae */
    if(!mi_signed_less(f->flags))goto mi_004848b2;
    /* 004848b0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004848b2:;
    /* 004848b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004848b4 */
    if(!mi_signed_less(f->flags))goto mi_004848b8;
    /* 004848b6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004848b8:;
    /* 004848b8 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 004848be */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004848c0 */
    if(mi_signed_less(f->flags))goto mi_004848c9;
    /* 004848c2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004848c3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004848c7 */
    f->r[7]=f->r[2];
mi_004848c9:;
    /* 004848c9 */
    f->r[2]=mi_read((0x00bca2d4u),4u);
    /* 004848cf */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004848d1 */
    if(mi_signed_less(f->flags))goto mi_004848d6;
    /* 004848d3 */
    f->r[1]=(0xffffffffu+f->r[2]);
mi_004848d6:;
    /* 004848d6 */
    f->r[3]=mi_read((0x00bca2d8u),4u);
    /* 004848dc */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004848df */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 004848e4 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004848e7 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004848ea */
    f->r[5]=(0x00000000u+f->r[3]+f->r[2]*4u);
    /* 004848ed */
    f->r[3]=f->r[6];
    /* 004848ef */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004848f1 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004848f3 */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004848f9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004848fb */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 00484901 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0048497c;
    /* 00484903 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00484905 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00484906 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0048490a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048490c */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
mi_00484912:;
    /* 00484912 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00484914 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[6]);
    /* 0048491a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484969;
    /* 0048491c */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00484920 */
    f->r[7]=(0x00b4e6fcu+f->r[5]*2u);
    /* 00484927 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00484929 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0048492a */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0048492c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0048492f */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
mi_00484934:;
    /* 00484934 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[7]),2u);
    /* 00484937 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00484939 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048495f;
    /* 0048493b */
    f->r[0]=f->r[1];
    /* 0048493d */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00484940 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00484942 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00484945 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[0]),2u))&0xffffu)<<0);
    /* 0048494c */
    a=(f->r[1]&0xffffu);b=0x00000058u;t=mi_alu(f,a,b,16,1);
    /* 00484950 */
    if((f->flags&MI_ZF))goto mi_00484958;
    /* 00484952 */
    a=(f->r[1]&0xffffu);b=0x00000059u;t=mi_alu(f,a,b,16,1);
    /* 00484956 */
    if(!(f->flags&MI_ZF))goto mi_0048495f;
mi_00484958:;
    /* 00484958 */
    MI_U8((0x00799a9au+f->r[0]))=(uint8_t)(0x00000000u);
mi_0048495f:;
    /* 0048495f */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00484962 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00484963 */
    if(!(f->flags&MI_ZF))goto mi_00484934;
    /* 00484965 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
mi_00484969:;
    /* 00484969 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0048496d */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0048496f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00484970 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 00484976 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048497a */
    if(!(f->flags&MI_ZF))goto mi_00484912;
mi_0048497c:;
    /* 0048497c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048497d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048497e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048497f */
    f->r[0]=0x00000001u;
    /* 00484984 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00484985 */
    f->r[4]+=4;return;
}
void restore_routing_C_00483fb0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00483fb0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00483fb3 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00483fb7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483fb8 */
    f->r[3]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00483fbc */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483fbd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483fbe */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483fbf */
    f->r[5]=(0x00000002u+f->r[3]);
    /* 00483fc2 */
    f->r[2]=(0x00000002u+f->r[1]);
    /* 00483fc5 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00483fc8 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00483fca */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00483fcd */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00483fd1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00483fd3 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00483fd7 */
    if(!mi_signed_less(f->flags))goto mi_00483fdb;
    /* 00483fd9 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_00483fdb:;
    /* 00483fdb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00483fdd */
    if(!mi_signed_less(f->flags))goto mi_00483fe1;
    /* 00483fdf */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00483fe1:;
    /* 00483fe1 */
    f->r[6]=mi_read((0x00bca2d0u),4u);
    /* 00483fe7 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00483fe9 */
    if(mi_signed_less(f->flags))goto mi_00483ff2;
    /* 00483feb */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00483fec */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00483ff0 */
    f->r[5]=f->r[6];
mi_00483ff2:;
    /* 00483ff2 */
    f->r[6]=mi_read((0x00bca2d4u),4u);
    /* 00483ff8 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00483ffa */
    if(mi_signed_less(f->flags))goto mi_00483fff;
    /* 00483ffc */
    f->r[2]=(0xffffffffu+f->r[6]);
mi_00483fff:;
    /* 00483fff */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00484002 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
    /* 00484008 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0048400b */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 0048400e */
    f->r[6]=mi_read((0x00bca2d8u),4u);
    /* 00484014 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 00484017 */
    f->r[6]=f->r[3];
    /* 00484019 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0048401b */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048401d */
    a=f->r[6];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00484023 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00484025 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0048402a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0048402e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484110;
    /* 00484034 */
    f->r[7]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00484038 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0048403a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0048403b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048403f */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00484041 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[2]);
mi_00484047:;
    /* 00484047 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00484049 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[3]);
    /* 0048404f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004840ee;
    /* 00484055 */
    f->r[6]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00484059 */
    f->r[5]=(0x00b4e6fcu+f->r[0]*2u);
    /* 00484060 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00484062 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00484063 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00484065 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00484068 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[1]);
mi_0048406e:;
    /* 0048406e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[5]),2u);
    /* 00484072 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00484074 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004840e0;
    /* 00484076 */
    f->r[1]=f->r[2];
    /* 00484078 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048407b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048407d */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00484080 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[1]),1u))&0xffu)<<0);
    /* 00484086 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00484088 */
    if((f->flags&MI_ZF))goto mi_004840e0;
    /* 0048408a */
    a=mi_read((0x007999f4u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00484092 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004840e0;
    /* 00484094 */
    f->r[2]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484098 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00484099 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0048409b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048409f */
    if(!(f->flags&MI_ZF))goto mi_004840aa;
    /* 004840a1 */
    MI_U8((0x00799a51u+f->r[1]))=(uint8_t)(0x00000060u);
    /* 004840a8 */
    goto mi_004840e0;
mi_004840aa:;
    /* 004840aa */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004840ad */
    if(!(f->flags&MI_ZF))goto mi_004840b8;
    /* 004840af */
    MI_U8((0x00799a52u+f->r[1]))=(uint8_t)(0x00000060u);
    /* 004840b6 */
    goto mi_004840e0;
mi_004840b8:;
    /* 004840b8 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004840bb */
    if(!(f->flags&MI_ZF))goto mi_004840c6;
    /* 004840bd */
    MI_U8((0x00799a53u+f->r[1]))=(uint8_t)(0x00000060u);
    /* 004840c4 */
    goto mi_004840e0;
mi_004840c6:;
    /* 004840c6 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004840c9 */
    if(!(f->flags&MI_ZF))goto mi_004840d4;
    /* 004840cb */
    MI_U8((0x00799a54u+f->r[1]))=(uint8_t)(0x00000060u);
    /* 004840d2 */
    goto mi_004840e0;
mi_004840d4:;
    /* 004840d4 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004840d7 */
    if(!(f->flags&MI_ZF))goto mi_004840e0;
    /* 004840d9 */
    MI_U8((0x00799a55u+f->r[1]))=(uint8_t)(0x00000060u);
mi_004840e0:;
    /* 004840e0 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004840e3 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004840e4 */
    if(!(f->flags&MI_ZF))goto mi_0048406e;
    /* 004840e6 */
    f->r[5]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004840ea */
    f->r[6]=mi_read((0x00000014u+f->r[4]),4u);
mi_004840ee:;
    /* 004840ee */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004840f2 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004840f4 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004840f5 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 004840fa */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004840fe */
    if(!(f->flags&MI_ZF))goto mi_00484047;
    /* 00484104 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484108 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00484109 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048410a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048410b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048410c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048410f */
    f->r[4]+=4;return;
mi_00484110:;
    /* 00484110 */
    f->r[0]=f->r[7];
    /* 00484112 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00484113 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00484114 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00484115 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00484116 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00484119 */
    f->r[4]+=4;return;
}
void restore_routing_C_00483160(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00483160 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00483164 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483165 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483166 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483167 */
    f->r[6]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0048316b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048316c */
    f->r[1]=(0x00000002u+f->r[0]);
    /* 0048316f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00483172 */
    f->r[7]=(0x00000002u+f->r[6]);
    /* 00483175 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00483178 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0048317a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0048317e */
    if(!mi_signed_less(f->flags))goto mi_00483182;
    /* 00483180 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_00483182:;
    /* 00483182 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00483184 */
    if(!mi_signed_less(f->flags))goto mi_00483188;
    /* 00483186 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00483188:;
    /* 00483188 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 0048318e */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00483190 */
    if(mi_signed_less(f->flags))goto mi_00483199;
    /* 00483192 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00483193 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00483197 */
    f->r[7]=f->r[2];
mi_00483199:;
    /* 00483199 */
    f->r[2]=mi_read((0x00bca2d4u),4u);
    /* 0048319f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004831a1 */
    if(mi_signed_less(f->flags))goto mi_004831a6;
    /* 004831a3 */
    f->r[1]=(0xffffffffu+f->r[2]);
mi_004831a6:;
    /* 004831a6 */
    f->r[3]=mi_read((0x00bca2d8u),4u);
    /* 004831ac */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004831af */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 004831b4 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004831b7 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004831ba */
    f->r[5]=(0x00000000u+f->r[3]+f->r[2]*4u);
    /* 004831bd */
    f->r[3]=f->r[6];
    /* 004831bf */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004831c1 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004831c3 */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004831c9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004831cb */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 004831d1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483258;
    /* 004831d7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004831d9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004831da */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004831de */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004831e0 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
mi_004831e6:;
    /* 004831e6 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004831e8 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[6]);
    /* 004831ee */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483245;
    /* 004831f0 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004831f4 */
    f->r[7]=(0x00b4e6fcu+f->r[5]*2u);
    /* 004831fb */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004831fd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004831fe */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00483200 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 00483203 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
mi_00483208:;
    /* 00483208 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[7]),2u);
    /* 0048320b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0048320d */
    if((f->flags&MI_ZF))goto mi_0048323b;
    /* 0048320f */
    f->r[0]=f->r[1];
    /* 00483211 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00483214 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00483216 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00483219 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[0]),2u);
    /* 00483220 */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00483223 */
    if(mi_signed_less(f->flags))goto mi_0048323b;
    /* 00483225 */
    a=f->r[1];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 00483228 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0048323b;
    /* 0048322a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999d8u+f->r[0]),1u))&0xffu)<<0);
    /* 00483230 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00483232 */
    if((f->flags&MI_ZF))goto mi_0048323b;
    /* 00483234 */
    MI_U8((0x00799a5fu+f->r[0]))=(uint8_t)(0x00000000u);
mi_0048323b:;
    /* 0048323b */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0048323e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0048323f */
    if(!(f->flags&MI_ZF))goto mi_00483208;
    /* 00483241 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
mi_00483245:;
    /* 00483245 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00483249 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0048324b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048324c */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 00483252 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00483256 */
    if(!(f->flags&MI_ZF))goto mi_004831e6;
mi_00483258:;
    /* 00483258 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00483259 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048325a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048325b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048325d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048325e */
    f->r[4]+=4;return;
}
void restore_routing_C_00483260(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00483260 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483261 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00483265 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483266 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483267 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483268 */
    f->r[7]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0048326c */
    f->r[1]=(0x00000002u+f->r[0]);
    /* 0048326f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00483272 */
    MI_U32((0x009ab9dcu))=(uint32_t)(0x00000000u);
    /* 0048327c */
    f->r[6]=(0x00000002u+f->r[7]);
    /* 0048327f */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00483282 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00483284 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00483288 */
    if(!mi_signed_less(f->flags))goto mi_0048328c;
    /* 0048328a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0048328c:;
    /* 0048328c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048328e */
    if(!mi_signed_less(f->flags))goto mi_00483292;
    /* 00483290 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00483292:;
    /* 00483292 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 00483298 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048329a */
    if(mi_signed_less(f->flags))goto mi_004832a3;
    /* 0048329c */
    f->r[6]=(0xffffffffu+f->r[2]);
    /* 0048329f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004832a3:;
    /* 004832a3 */
    f->r[2]=mi_read((0x00bca2d4u),4u);
    /* 004832a9 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004832ab */
    if(mi_signed_less(f->flags))goto mi_004832b0;
    /* 004832ad */
    f->r[1]=(0xffffffffu+f->r[2]);
mi_004832b0:;
    /* 004832b0 */
    f->r[3]=mi_read((0x00bca2d8u),4u);
    /* 004832b6 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004832b9 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 004832be */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004832c1 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004832c4 */
    f->r[2]=(0x00000000u+f->r[3]+f->r[2]*4u);
    /* 004832c7 */
    f->r[3]=f->r[7];
    /* 004832c9 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004832cb */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004832cd */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004832d3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004832d5 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004832db */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004832df */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004833ad;
    /* 004832e5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004832e7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004832e8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004832e9 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004832ed */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004832ef */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
mi_004832f5:;
    /* 004832f5 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004832f7 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 004832fd */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483395;
    /* 00483303 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00483305 */
    f->r[3]=(0x00b4e6fcu+f->r[2]*2u);
    /* 0048330c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0048330d */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048330f */
    f->r[0]=(0x00000000u+f->r[6]+f->r[7]*1u);
    /* 00483312 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 00483318 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
mi_0048331d:;
    /* 0048331d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[3]),2u);
    /* 00483320 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00483322 */
    if((f->flags&MI_ZF))goto mi_00483381;
    /* 00483324 */
    f->r[0]=f->r[1];
    /* 00483326 */
    f->r[2]=mi_read((0x009ab9dcu),4u);
    /* 0048332c */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048332f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00483331 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00483334 */
    f->r[1]=mi_read((0x00799abcu+f->r[0]),4u);
    /* 0048333a */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048333c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483344;
    /* 0048333e */
    MI_U32((0x009ab9dcu))=(uint32_t)(f->r[1]);
mi_00483344:;
    /* 00483344 */
    a=f->r[1];b=mi_read((0x00000020u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00483348 */
    MI_U32((0x00799abcu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0048334e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[0]),2u);
    /* 00483355 */
    f->r[5]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 00483358 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[5]*2u);
    /* 0048335b */
    f->r[2]=mi_read((0x0070ce0cu+f->r[2]*8u),4u);
    /* 00483362 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00483364 */
    if(!mi_signed_less(f->flags))goto mi_0048336c;
    /* 00483366 */
    MI_U32((0x00799abcu+f->r[0]))=(uint32_t)(f->r[2]);
mi_0048336c:;
    /* 0048336c */
    f->r[2]=mi_read((0x00799abcu+f->r[0]),4u);
    /* 00483372 */
    f->r[1]=0x000003e8u;
    /* 00483377 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00483379 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483381;
    /* 0048337b */
    MI_U32((0x00799abcu+f->r[0]))=(uint32_t)(f->r[1]);
mi_00483381:;
    /* 00483381 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00483384 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00483385 */
    if(!(f->flags&MI_ZF))goto mi_0048331d;
    /* 00483387 */
    f->r[2]=mi_read((0x00bc9768u),4u);
    /* 0048338d */
    f->r[3]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00483391 */
    f->r[6]=mi_read((0x00000018u+f->r[4]),4u);
mi_00483395:;
    /* 00483395 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00483399 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048339b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048339c */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 004833a2 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004833a6 */
    if(!(f->flags&MI_ZF))goto mi_004832f5;
    /* 004833ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_004833ad:;
    /* 004833ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004833ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004833af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004833b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004833b1 */
    f->r[4]+=4;return;
}
void restore_routing_C_00482c10(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00482c10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482c11 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00482c15 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482c16 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482c17 */
    f->r[5]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00482c1b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482c1c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482c1d */
    f->r[2]=(0x00000002u+f->r[5]);
    /* 00482c20 */
    f->r[3]=(0x00000002u+f->r[0]);
    /* 00482c23 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00482c26 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00482c28 */
    f->r[6]=0x000003e8u;
    /* 00482c2d */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00482c30 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00482c32 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00482c36 */
    MI_U32((0x009ab9e0u))=(uint32_t)(f->r[6]);
    /* 00482c3c */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00482c40 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00482c44 */
    if(!mi_signed_less(f->flags))goto mi_00482c48;
    /* 00482c46 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_00482c48:;
    /* 00482c48 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00482c4a */
    if(!mi_signed_less(f->flags))goto mi_00482c4e;
    /* 00482c4c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00482c4e:;
    /* 00482c4e */
    f->r[1]=mi_read((0x00bca2d0u),4u);
    /* 00482c54 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00482c56 */
    if(mi_signed_less(f->flags))goto mi_00482c5d;
    /* 00482c58 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00482c59 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00482c5d:;
    /* 00482c5d */
    f->r[1]=mi_read((0x00bca2d4u),4u);
    /* 00482c63 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00482c65 */
    if(mi_signed_less(f->flags))goto mi_00482c6c;
    /* 00482c67 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00482c68 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_00482c6c:;
    /* 00482c6c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00482c6f */
    f->r[3]=f->r[5];
    /* 00482c71 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00482c76 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00482c79 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00482c7c */
    f->r[1]=mi_read((0x00bca2d8u),4u);
    /* 00482c82 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 00482c85 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00482c89 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00482c8b */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482c8d */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00482c93 */
    a=f->r[0];b=mi_read((0x0000001cu+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 00482c97 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 00482c9d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482dbd;
    /* 00482ca3 */
    f->r[7]=mi_read((0x00706b70u),4u);
mi_00482ca9:;
    /* 00482ca9 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00482cab */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[5]);
    /* 00482cb1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00482d4f;
mi_00482cb7:;
    /* 00482cb7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[1]*2u),2u);
    /* 00482cbf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00482cc1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482d7c;
    /* 00482cc7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00482cc8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00482ccdu;f->boundary(0x00482cc8u,0x00475b50u,f);
    /* 00482ccd */
    f->r[1]=f->r[0];
    /* 00482ccf */
    f->r[6]=mi_read((0x009ab9e0u),4u);
    /* 00482cd5 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00482cd8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482cda */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00482cdd */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00482ce0 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00799a1eu+f->r[1]),2u))&0xffffu)<<0);
    /* 00482ce7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00482cea */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00482cec */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482cf6;
    /* 00482cee */
    f->r[6]=f->r[2];
    /* 00482cf0 */
    MI_U32((0x009ab9e0u))=(uint32_t)(f->r[6]);
mi_00482cf6:;
    /* 00482cf6 */
    f->r[7]=mi_read((0x00706b70u),4u);
    /* 00482cfc */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00482cfe */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00482d01 */
    MI_U16((0x00799a1eu+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00482d08 */
    if(!mi_signed_less(f->flags))goto mi_00482d13;
    /* 00482d0a */
    MI_U16((0x00799a1eu+f->r[1]))=(uint16_t)(0x00000000u);
mi_00482d13:;
    /* 00482d13 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[1]),1u))&0xffu)<<0);
    /* 00482d19 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00482d1b */
    if((f->flags&MI_ZF))goto mi_00482d2b;
    /* 00482d1d */
    a=mi_read((0x007999f4u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00482d25 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482d2b;
    /* 00482d27 */
    a=mi_read((0x00000010u+f->r[4]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000010u+f->r[4]))=(uint32_t)(t);
mi_00482d2b:;
    /* 00482d2b */
    f->r[1]=mi_read((0x00bc9768u),4u);
mi_00482d31:;
    /* 00482d31 */
    f->r[0]=mi_read((0x00d3b4e4u),4u);
    /* 00482d36 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00482d3a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00482d3b */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00482d3c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00482d3e */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
    /* 00482d43 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 00482d49 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482cb7;
mi_00482d4f:;
    /* 00482d4f */
    f->r[0]=mi_read((0x00d3b4e0u),4u);
    /* 00482d54 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00482d56 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00482d57 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 00482d5d */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00482d62 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00482d66 */
    a=mi_read((0x00d3b4e0u),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00482d6c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482ca9;
    /* 00482d72 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00482d76 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00482d77 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00482d78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00482d79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00482d7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00482d7b */
    f->r[4]+=4;return;
mi_00482d7c:;
    /* 00482d7c */
    f->r[0]=mi_read((0x00af598cu+f->r[1]*4u),4u);
    /* 00482d83 */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00482d86 */
    if((f->flags&MI_ZF))goto mi_00482d31;
    /* 00482d88 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00a9cc1cu+f->r[1]*2u),2u))&0xffffu)<<0);
    /* 00482d90 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00482d93 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00482d95 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00482d9f;
    /* 00482d97 */
    f->r[6]=f->r[2];
    /* 00482d99 */
    MI_U32((0x009ab9e0u))=(uint32_t)(f->r[6]);
mi_00482d9f:;
    /* 00482d9f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00482da1 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00482da4 */
    MI_U16((0x00a9cc1cu+f->r[1]*2u))=(uint16_t)((f->r[0]&0xffffu));
    /* 00482dac */
    if(!mi_signed_less(f->flags))goto mi_00482d31;
    /* 00482dae */
    MI_U16((0x00a9cc1cu+f->r[1]*2u))=(uint16_t)(0x00000000u);
    /* 00482db8 */
    goto mi_00482d31;
mi_00482dbd:;
    /* 00482dbd */
    f->r[0]=f->r[7];
    /* 00482dbf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00482dc0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00482dc1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00482dc2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00482dc3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00482dc4 */
    f->r[4]+=4;return;
}
void restore_routing_C_00483000(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00483000 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483001 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00483005 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483006 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483007 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483008 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00483009 */
    f->r[7]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0048300d */
    f->r[2]=(0x00000002u+f->r[0]);
    /* 00483010 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00483012 */
    f->r[5]=(0x00000002u+f->r[7]);
    /* 00483015 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00483018 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048301b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0048301f */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00483021 */
    MI_U32((0x009ab9e4u))=(uint32_t)(0x000003e8u);
    /* 0048302b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0048302f */
    if(!mi_signed_less(f->flags))goto mi_00483033;
    /* 00483031 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_00483033:;
    /* 00483033 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00483035 */
    if(!mi_signed_less(f->flags))goto mi_00483039;
    /* 00483037 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00483039:;
    /* 00483039 */
    f->r[1]=mi_read((0x00bca2d0u),4u);
    /* 0048303f */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00483041 */
    if(mi_signed_less(f->flags))goto mi_00483046;
    /* 00483043 */
    f->r[5]=(0xffffffffu+f->r[1]);
mi_00483046:;
    /* 00483046 */
    f->r[1]=mi_read((0x00bca2d4u),4u);
    /* 0048304c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048304e */
    if(mi_signed_less(f->flags))goto mi_00483055;
    /* 00483050 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00483051 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00483055:;
    /* 00483055 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00483058 */
    f->r[3]=f->r[7];
    /* 0048305a */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0048305c */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00483061 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00483064 */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0048306a */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048306d */
    f->r[1]=mi_read((0x00bca2d8u),4u);
    /* 00483073 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 00483076 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0048307a */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048307c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048307e */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 00483084 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483150;
mi_0048308a:;
    /* 0048308a */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0048308c */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 00483092 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00483131;
mi_00483098:;
    /* 00483098 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[1]*2u),2u);
    /* 004830a0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004830a2 */
    if((f->flags&MI_ZF))goto mi_0048310d;
    /* 004830a4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004830a5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004830aau;f->boundary(0x004830a5u,0x00475b50u,f);
    /* 004830aa */
    f->r[2]=f->r[0];
    /* 004830ac */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004830af */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004830b2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004830b4 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00799a20u+f->r[2]*8u),2u))&0xffffu)<<0);
    /* 004830bc */
    f->r[2]=(0x00799a20u+f->r[2]*8u);
    /* 004830c3 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 004830c6 */
    a=f->r[6];b=mi_read((0x009ab9e4u),4u);t=mi_alu(f,a,b,32,1);
    /* 004830cc */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004830d4;
    /* 004830ce */
    MI_U32((0x009ab9e4u))=(uint32_t)(f->r[6]);
mi_004830d4:;
    /* 004830d4 */
    a=f->r[1];b=mi_read((0x007078e0u),4u);t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004830da */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004830dd */
    MI_U16((0x00000000u+f->r[2]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004830e0 */
    if(!mi_signed_less(f->flags))goto mi_004830e7;
    /* 004830e2 */
    MI_U16((0x00000000u+f->r[2]))=(uint16_t)(0x00000000u);
mi_004830e7:;
    /* 004830e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004830e9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048310d;
    /* 004830eb */
    f->r[1]=f->r[0];
    /* 004830ed */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004830f0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004830f2 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004830f5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[1]),1u))&0xffu)<<0);
    /* 004830fb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004830fd */
    if((f->flags&MI_ZF))goto mi_0048310d;
    /* 004830ff */
    a=mi_read((0x007999f4u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00483107 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048310d;
    /* 00483109 */
    a=mi_read((0x00000010u+f->r[4]),4u);b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000010u+f->r[4]))=(uint32_t)(t);
mi_0048310d:;
    /* 0048310d */
    f->r[0]=mi_read((0x00d3b4e4u),4u);
    /* 00483112 */
    f->r[1]=mi_read((0x00bc9768u),4u);
    /* 00483118 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00483119 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0048311a */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0048311c */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
    /* 00483121 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 00483127 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00483098;
    /* 0048312d */
    f->r[6]=mi_read((0x00000010u+f->r[4]),4u);
mi_00483131:;
    /* 00483131 */
    f->r[0]=mi_read((0x00d3b4e0u),4u);
    /* 00483136 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0048313a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048313b */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048313d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048313f */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 00483144 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 0048314a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048308a;
mi_00483150:;
    /* 00483150 */
    f->r[0]=f->r[6];
    /* 00483152 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00483153 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00483154 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00483155 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00483156 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00483157 */
    f->r[4]+=4;return;
}
void restore_routing_C_00484670(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00484670 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484671 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00484675 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484676 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484677 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484678 */
    f->r[6]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0048467c */
    f->r[1]=(0x00000002u+f->r[0]);
    /* 0048467f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00484682 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00484683 */
    f->r[5]=(0x00000002u+f->r[6]);
    /* 00484686 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00484689 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0048468b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00484693 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00484697 */
    if(!mi_signed_less(f->flags))goto mi_0048469b;
    /* 00484699 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0048469b:;
    /* 0048469b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048469d */
    if(!mi_signed_less(f->flags))goto mi_004846a1;
    /* 0048469f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004846a1:;
    /* 004846a1 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 004846a7 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004846a9 */
    if(mi_signed_less(f->flags))goto mi_004846b2;
    /* 004846ab */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004846ac */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004846b0 */
    f->r[5]=f->r[2];
mi_004846b2:;
    /* 004846b2 */
    f->r[2]=mi_read((0x00bca2d4u),4u);
    /* 004846b8 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004846ba */
    if(mi_signed_less(f->flags))goto mi_004846bf;
    /* 004846bc */
    f->r[1]=(0xffffffffu+f->r[2]);
mi_004846bf:;
    /* 004846bf */
    f->r[7]=mi_read((0x00bca2d8u),4u);
    /* 004846c5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004846c8 */
    f->r[3]=f->r[6];
    /* 004846ca */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 004846cf */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004846d2 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004846d4 */
    a=f->r[3];b=0x000000e3u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004846da */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004846dd */
    f->r[7]=(0x00000000u+f->r[7]+f->r[2]*4u);
    /* 004846e0 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004846e2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004846e4 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[7]);
    /* 004846ea */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484774;
    /* 004846f0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004846f2 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004846f3 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004846f7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004846f9 */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[1]);
mi_004846ff:;
    /* 004846ff */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00484701 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[6]);
    /* 00484707 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00484761;
    /* 00484709 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0048470d */
    f->r[5]=(0x00b4e6fcu+f->r[7]*2u);
    /* 00484714 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00484716 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00484717 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00484719 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0048471c */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[0]);
mi_00484721:;
    /* 00484721 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[5]),2u);
    /* 00484725 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00484727 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484757;
    /* 00484729 */
    f->r[0]=f->r[2];
    /* 0048472b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048472e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00484730 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00484733 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[0]),1u))&0xffu)<<0);
    /* 00484739 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048473b */
    if((f->flags&MI_ZF))goto mi_00484757;
    /* 0048473d */
    a=mi_read((0x007999f4u+f->r[0]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00484745 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00484757;
    /* 00484747 */
    MI_U8((0x00799a48u+f->r[0]))=(uint8_t)(0x00000060u);
    /* 0048474e */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00484752 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00484753 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00484757:;
    /* 00484757 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0048475a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0048475b */
    if(!(f->flags&MI_ZF))goto mi_00484721;
    /* 0048475d */
    f->r[5]=mi_read((0x00000018u+f->r[4]),4u);
mi_00484761:;
    /* 00484761 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00484765 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00484767 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00484768 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048476c */
    if(!(f->flags&MI_ZF))goto mi_004846ff;
    /* 0048476e */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[7]);
mi_00484774:;
    /* 00484774 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00484775 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00484776 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00484777 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00484778 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00484779 */
    f->r[4]+=4;return;
}
void restore_routing_C_0047e250(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047e250 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0047e254 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e255 */
    f->r[6]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0047e259 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047e25c */
    f->r[2]=mi_read((0x00978d9cu+f->r[6]*4u),4u);
    /* 0047e263 */
    f->r[0]=f->r[2];
    /* 0047e265 */
    a=f->r[0];b=((f->r[1]&0xffu))&31u;if(b){t=a>>b;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>(b-1u))&1u)|mi_szp(t,32);if(b==1u)f->flags=(f->flags&~MI_OF)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;} /* AF and OF for count>1 are architecturally unspecified; retained bits are not a status-equivalence claim. */
    /* 0047e267 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047e26a */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0047e26d */
    if(!(f->flags&MI_CF))goto mi_0047e28c;
    /* 0047e26f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e270 */
    f->r[7]=0x00000007u;
    /* 0047e275 */
    a=f->r[7];b=(f->r[1]&0xffu);t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0047e277 */
    a=f->r[7];b=0x0000ffffu;t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0047e27d */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0047e27f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047e280 */
    a=f->r[0];b=(f->r[1]&0xffu);t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0047e282 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0047e284 */
    MI_U32((0x00978d9cu+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0047e28b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_0047e28c:;
    /* 0047e28c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047e28d */
    f->r[4]+=4;return;
}
void restore_routing_C_004baa10(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004baa10 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004baa14 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa15 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa16 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004baa18 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004baa1b */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004baa1e */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004baa20 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004baa23 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[6]),1u))&0xffu)<<0);
    /* 004baa29 */
    a=(f->r[0]&0xffu);b=0x00000028u;t=mi_alu(f,a,b,8,1);
    /* 004baa2b */
    if((f->flags&MI_ZF))goto mi_004bb07d;
    /* 004baa31 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa32 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004baa34 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa35 */
    if(!(f->flags&MI_ZF))goto mi_004baa51;
    /* 004baa37 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baa3e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baa45 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa46 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baa4cu;f->boundary(0x004baa47u,0x00484990u,f);
    /* 004baa4c */
    goto mi_004bb01b;
mi_004baa51:;
    /* 004baa51 */
    a=(f->r[0]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 004baa53 */
    if(!(f->flags&MI_ZF))goto mi_004baa80;
    /* 004baa55 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baa5c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baa63 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa64 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa65 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baa6au;f->boundary(0x004baa65u,0x00484a90u,f);
    /* 004baa6a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x009ab9e8u),1u))&0xffu)<<0);
    /* 004baa70 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004baa73 */
    f->r[3]=f->r[0];
    /* 004baa75 */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004baa7b */
    goto mi_004bb020;
mi_004baa80:;
    /* 004baa80 */
    a=(f->r[0]&0xffu);b=0x0000001au;t=mi_alu(f,a,b,8,1);
    /* 004baa82 */
    if(!(f->flags&MI_ZF))goto mi_004baaa9;
    /* 004baa84 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baa8b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baa92 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[6]),2u);
    /* 004baa99 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa9a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa9b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baa9c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baaa1u;f->boundary(0x004baa9cu,0x00483910u,f);
    /* 004baaa1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004baaa4 */
    goto mi_004bb01e;
mi_004baaa9:;
    /* 004baaa9 */
    a=(f->r[0]&0xffu);b=0x00000057u;t=mi_alu(f,a,b,8,1);
    /* 004baaab */
    if(!(f->flags&MI_ZF))goto mi_004baac7;
    /* 004baaad */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baab4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baabb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baabc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baabd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baac2u;f->boundary(0x004baabdu,0x00483e90u,f);
    /* 004baac2 */
    goto mi_004bb01b;
mi_004baac7:;
    /* 004baac7 */
    a=(f->r[0]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,1);
    /* 004baac9 */
    if(!(f->flags&MI_ZF))goto mi_004bab05;
    /* 004baacb */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baad2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baad9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baada */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baadb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baae0u;f->boundary(0x004baadbu,0x00484450u,f);
    /* 004baae0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baae7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baaee */
    f->r[3]=f->r[0];
    /* 004baaf0 */
    f->r[0]=mi_read((0x00707ef8u),4u);
    /* 004baaf5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baaf6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baaf7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baaf8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baafdu;f->boundary(0x004baaf8u,0x00482ef0u,f);
    /* 004baafd */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bab00 */
    goto mi_004bb020;
mi_004bab05:;
    /* 004bab05 */
    a=(f->r[0]&0xffu);b=0x0000001du;t=mi_alu(f,a,b,8,1);
    /* 004bab07 */
    if(!(f->flags&MI_ZF))goto mi_004bab23;
    /* 004bab09 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bab10 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bab17 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab18 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bab1eu;f->boundary(0x004bab19u,0x00484120u,f);
    /* 004bab1e */
    goto mi_004bb01b;
mi_004bab23:;
    /* 004bab23 */
    a=(f->r[0]&0xffu);b=0x0000001eu;t=mi_alu(f,a,b,8,1);
    /* 004bab25 */
    if(!(f->flags&MI_ZF))goto mi_004bab41;
    /* 004bab27 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bab2e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bab35 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab36 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab37 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bab3cu;f->boundary(0x004bab37u,0x00484230u,f);
    /* 004bab3c */
    goto mi_004bb01b;
mi_004bab41:;
    /* 004bab41 */
    a=(f->r[0]&0xffu);b=0x0000001fu;t=mi_alu(f,a,b,8,1);
    /* 004bab43 */
    if(!(f->flags&MI_ZF))goto mi_004bab5f;
    /* 004bab45 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bab4c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bab53 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab54 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab55 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bab5au;f->boundary(0x004bab55u,0x00484340u,f);
    /* 004bab5a */
    goto mi_004bb01b;
mi_004bab5f:;
    /* 004bab5f */
    a=(f->r[0]&0xffu);b=0x00000021u;t=mi_alu(f,a,b,8,1);
    /* 004bab61 */
    if(!(f->flags&MI_ZF))goto mi_004babd5;
    /* 004bab63 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bab6a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bab71 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab72 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bab78u;f->boundary(0x004bab73u,0x00484560u,f);
    /* 004bab78 */
    f->r[2]=mi_read((0x007065f8u),4u);
    /* 004bab7e */
    f->r[3]=f->r[0];
    /* 004bab80 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bab87 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bab8e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab8f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab90 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bab91 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bab96u;f->boundary(0x004bab91u,0x00482dd0u,f);
    /* 004bab96 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 004bab9c */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bab9f */
    f->r[2]=f->r[1];
    /* 004baba1 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004baba7 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004baba9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004babb6;
    /* 004babab */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004babb1 */
    goto mi_004bb020;
mi_004babb6:;
    /* 004babb6 */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004babb9 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004babc9;
    /* 004babbb */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004babbe */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004babc4 */
    goto mi_004bb020;
mi_004babc9:;
    /* 004babc9 */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004babd0 */
    goto mi_004bb020;
mi_004babd5:;
    /* 004babd5 */
    a=(f->r[0]&0xffu);b=0x00000062u;t=mi_alu(f,a,b,8,1);
    /* 004babd7 */
    if(!(f->flags&MI_ZF))goto mi_004babf6;
    /* 004babd9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004babe0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004babe7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004babe8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004babe9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004babeeu;f->boundary(0x004babe9u,0x0042ff20u,f);
    /* 004babee */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004babf1 */
    goto mi_004bb020;
mi_004babf6:;
    /* 004babf6 */
    a=(f->r[0]&0xffu);b=0x00000022u;t=mi_alu(f,a,b,8,1);
    /* 004babf8 */
    if(!(f->flags&MI_ZF))goto mi_004bac14;
    /* 004babfa */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bac01 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bac08 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac09 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac0a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bac0fu;f->boundary(0x004bac0au,0x00484780u,f);
    /* 004bac0f */
    goto mi_004bb01b;
mi_004bac14:;
    /* 004bac14 */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,1);
    /* 004bac16 */
    if(!(f->flags&MI_ZF))goto mi_004bac32;
    /* 004bac18 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bac1f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bac26 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac27 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bac2du;f->boundary(0x004bac28u,0x00484890u,f);
    /* 004bac2d */
    goto mi_004bb01b;
mi_004bac32:;
    /* 004bac32 */
    a=(f->r[0]&0xffu);b=0x0000001bu;t=mi_alu(f,a,b,8,1);
    /* 004bac34 */
    if(!(f->flags&MI_ZF))goto mi_004bae15;
    /* 004bac3a */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004bac3f */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[6]),2u);
    /* 004bac46 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004bac4c */
    f->r[5]=mi_read((0x00ea5f9cu+f->r[0]),4u);
    /* 004bac52 */
    f->r[1]=f->r[7];
    /* 004bac54 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bac57 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bac59 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 004bac61 */
    a=(f->r[0]&0xffffu);b=0x0000003cu;t=mi_alu(f,a,b,16,1);
    /* 004bac65 */
    if(!(f->flags&MI_ZF))goto mi_004bac6b;
    /* 004bac67 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac69 */
    goto mi_004bace3;
mi_004bac6b:;
    /* 004bac6b */
    a=(f->r[0]&0xffffu);b=0x00000041u;t=mi_alu(f,a,b,16,1);
    /* 004bac6f */
    if(!(f->flags&MI_ZF))goto mi_004bac75;
    /* 004bac71 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac73 */
    goto mi_004bacaf;
mi_004bac75:;
    /* 004bac75 */
    a=(f->r[0]&0xffffu);b=0x0000003du;t=mi_alu(f,a,b,16,1);
    /* 004bac79 */
    if(!(f->flags&MI_ZF))goto mi_004bac7f;
    /* 004bac7b */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac7d */
    goto mi_004bacc9;
mi_004bac7f:;
    /* 004bac7f */
    a=(f->r[0]&0xffffu);b=0x00000042u;t=mi_alu(f,a,b,16,1);
    /* 004bac83 */
    if(!(f->flags&MI_ZF))goto mi_004bac89;
    /* 004bac85 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac87 */
    goto mi_004bace3;
mi_004bac89:;
    /* 004bac89 */
    a=(f->r[0]&0xffffu);b=0x0000003eu;t=mi_alu(f,a,b,16,1);
    /* 004bac8d */
    if(!(f->flags&MI_ZF))goto mi_004bac93;
    /* 004bac8f */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac91 */
    goto mi_004bacaf;
mi_004bac93:;
    /* 004bac93 */
    a=(f->r[0]&0xffffu);b=0x00000043u;t=mi_alu(f,a,b,16,1);
    /* 004bac97 */
    if(!(f->flags&MI_ZF))goto mi_004bac9d;
    /* 004bac99 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bac9b */
    goto mi_004bacc9;
mi_004bac9d:;
    /* 004bac9d */
    a=(f->r[0]&0xffffu);b=0x0000003fu;t=mi_alu(f,a,b,16,1);
    /* 004baca1 */
    if(!(f->flags&MI_ZF))goto mi_004baca7;
    /* 004baca3 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baca5 */
    goto mi_004bace3;
mi_004baca7:;
    /* 004baca7 */
    a=(f->r[0]&0xffffu);b=0x00000044u;t=mi_alu(f,a,b,16,1);
    /* 004bacab */
    if(!(f->flags&MI_ZF))goto mi_004bacc1;
    /* 004bacad */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004bacaf:;
    /* 004bacaf */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bacb6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bacbd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bacbe */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bacbf */
    goto mi_004bacf3;
mi_004bacc1:;
    /* 004bacc1 */
    a=(f->r[0]&0xffffu);b=0x00000040u;t=mi_alu(f,a,b,16,1);
    /* 004bacc5 */
    if(!(f->flags&MI_ZF))goto mi_004bacdb;
    /* 004bacc7 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004bacc9:;
    /* 004bacc9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bacd0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bacd7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bacd8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bacd9 */
    goto mi_004bacf3;
mi_004bacdb:;
    /* 004bacdb */
    a=(f->r[0]&0xffffu);b=0x00000045u;t=mi_alu(f,a,b,16,1);
    /* 004bacdf */
    if(!(f->flags&MI_ZF))goto mi_004bacfd;
    /* 004bace1 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004bace3:;
    /* 004bace3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bacea */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bacf1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bacf2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004bacf3:;
    /* 004bacf3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bacf8u;f->boundary(0x004bacf3u,0x00483fb0u,f);
    /* 004bacf8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bacfb */
    f->r[3]=f->r[0];
mi_004bacfd:;
    /* 004bacfd */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004bad02 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004bad08 */
    a=mi_read((0x00ea5ce8u+f->r[0]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004bad10 */
    if((f->flags&MI_ZF))goto mi_004bb020;
    /* 004bad16 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad17 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bad1cu;f->boundary(0x004bad17u,0x004c5120u,f);
    /* 004bad1c */
    f->r[2]=f->r[0];
    /* 004bad1e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad1f */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004bad23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bad28u;f->boundary(0x004bad23u,0x004c5120u,f);
    /* 004bad28 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004bad2c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bad2f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004bad31 */
    if(!(f->flags&MI_ZF))goto mi_004bb020;
    /* 004bad37 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad38 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bad3du;f->boundary(0x004bad38u,0x004c5120u,f);
    /* 004bad3d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bad40 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bad41 */
    if((f->flags&MI_ZF))goto mi_004bade9;
    /* 004bad47 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004bad4a */
    if((f->flags&MI_ZF))goto mi_004bad95;
    /* 004bad4c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bad4d */
    if(!(f->flags&MI_ZF))goto mi_004bb020;
    /* 004bad53 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004bad59 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad5b */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004bad61 */
    f->r[0]=mi_read((0x00ea5f9cu+f->r[2]),4u);
    /* 004bad67 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad68 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bad6du;f->boundary(0x004bad68u,0x004c52d0u,f);
    /* 004bad6d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bad70 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bad72 */
    if((f->flags&MI_ZF))goto mi_004bb020;
    /* 004bad78 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bad7f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bad86 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad87 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad88 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bad8du;f->boundary(0x004bad88u,0x00483160u,f);
    /* 004bad8d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bad90 */
    goto mi_004bb020;
mi_004bad95:;
    /* 004bad95 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004bad9a */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bad9c */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004bada2 */
    f->r[1]=mi_read((0x00ea5f9cu+f->r[0]),4u);
    /* 004bada8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bada9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004badaeu;f->boundary(0x004bada9u,0x004c52d0u,f);
    /* 004badae */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004badb1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004badb3 */
    if((f->flags&MI_ZF))goto mi_004bb020;
    /* 004badb9 */
    f->r[2]=mi_read((0x00706968u),4u);
mi_004badbf:;
    /* 004badbf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004badc6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004badcd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004badce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004badcf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004badd0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004badd5u;f->boundary(0x004badd0u,0x00483260u,f);
    /* 004badd5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x009ab9dcu),1u))&0xffu)<<0);
    /* 004baddb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004badde */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 004bade4 */
    goto mi_004bb020;
mi_004bade9:;
    /* 004bade9 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 004badee */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004badf0 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004badf6 */
    f->r[1]=mi_read((0x00ea5f9cu+f->r[0]),4u);
    /* 004badfc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004badfd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bae02u;f->boundary(0x004badfdu,0x004c52d0u,f);
    /* 004bae02 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bae05 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bae07 */
    if((f->flags&MI_ZF))goto mi_004bb020;
    /* 004bae0d */
    f->r[2]=mi_read((0x00706918u),4u);
    /* 004bae13 */
    goto mi_004badbf;
mi_004bae15:;
    /* 004bae15 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004bae17 */
    if(!(f->flags&MI_ZF))goto mi_004bae6b;
    /* 004bae19 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bae20 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bae27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bae28 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bae29 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bae2eu;f->boundary(0x004bae29u,0x00482c10u,f);
    /* 004bae2e */
    f->r[1]=mi_read((0x009ab9e0u),4u);
    /* 004bae34 */
    f->r[3]=f->r[0];
    /* 004bae36 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd65eu+f->r[6]),1u))&0xffu)<<0);
    /* 004bae3c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bae3f */
    f->r[2]=f->r[0];
    /* 004bae41 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bae47 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004bae49 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bae56;
    /* 004bae4b */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004bae51 */
    goto mi_004bb020;
mi_004bae56:;
    /* 004bae56 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004bae58 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004babc9;
    /* 004bae5e */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004bae60 */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004bae66 */
    goto mi_004bb020;
mi_004bae6b:;
    /* 004bae6b */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004bae6d */
    if(!(f->flags&MI_ZF))goto mi_004bae94;
    /* 004bae6f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bae76 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bae7d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bae7e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bae7f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bae84u;f->boundary(0x004bae7fu,0x00483000u,f);
    /* 004bae84 */
    f->r[3]=f->r[0];
    /* 004bae86 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bae89 */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004bae8f */
    goto mi_004bb020;
mi_004bae94:;
    /* 004bae94 */
    a=(f->r[0]&0xffu);b=0x00000058u;t=mi_alu(f,a,b,8,1);
    /* 004bae96 */
    if(!(f->flags&MI_ZF))goto mi_004baea3;
    /* 004bae98 */
    f->r[2]=mi_read((0x00706760u),4u);
    /* 004bae9e */
    goto mi_004badbf;
mi_004baea3:;
    /* 004baea3 */
    a=(f->r[0]&0xffu);b=0x00000059u;t=mi_alu(f,a,b,8,1);
    /* 004baea5 */
    if(!(f->flags&MI_ZF))goto mi_004baeeb;
    /* 004baea7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baeae */
    f->r[0]=mi_read((0x00707b88u),4u);
    /* 004baeb3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baeba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baebb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baebc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baebd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baec2u;f->boundary(0x004baebdu,0x00483260u,f);
    /* 004baec2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baec9 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baed0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baed1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baed2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baed7u;f->boundary(0x004baed2u,0x00484670u,f);
    /* 004baed7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x009ab9dcu),1u))&0xffu)<<0);
    /* 004baedd */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004baee0 */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 004baee6 */
    goto mi_004bb020;
mi_004baeeb:;
    /* 004baeeb */
    a=(f->r[0]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 004baeed */
    if(!(f->flags&MI_ZF))goto mi_004baf09;
    /* 004baeef */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baef6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baefd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baefe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baeff */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baf04u;f->boundary(0x004baeffu,0x004833c0u,f);
    /* 004baf04 */
    goto mi_004bb01b;
mi_004baf09:;
    /* 004baf09 */
    a=(f->r[0]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 004baf0b */
    if(!(f->flags&MI_ZF))goto mi_004baf7b;
    /* 004baf0d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 004baf13 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004baf15 */
    if((f->flags&MI_ZF))goto mi_004baf24;
    /* 004baf17 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004baf19 */
    if((f->flags&MI_ZF))goto mi_004baf24;
    /* 004baf1b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[6]),2u);
    /* 004baf22 */
    goto mi_004baf2b;
mi_004baf24:;
    /* 004baf24 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[6]),2u);
mi_004baf2b:;
    /* 004baf2b */
    f->r[2]=f->r[0];
    /* 004baf2d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004baf30 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004baf32 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a80u+f->r[2]*8u),1u))&0xffu)<<0);
    /* 004baf39 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004baf3b */
    if((f->flags&MI_ZF))goto mi_004baf5c;
    /* 004baf3d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baf44 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baf4b */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baf4d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baf4e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baf4f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baf54u;f->boundary(0x004baf4fu,0x004834d0u,f);
    /* 004baf54 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004baf57 */
    goto mi_004bb01e;
mi_004baf5c:;
    /* 004baf5c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baf63 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baf6a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baf6c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baf6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baf6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004baf73u;f->boundary(0x004baf6eu,0x004834d0u,f);
    /* 004baf73 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004baf76 */
    goto mi_004bb01e;
mi_004baf7b:;
    /* 004baf7b */
    a=(f->r[0]&0xffu);b=0x00000011u;t=mi_alu(f,a,b,8,1);
    /* 004baf7d */
    if(!(f->flags&MI_ZF))goto mi_004bafe7;
    /* 004baf7f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 004baf85 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004baf87 */
    if((f->flags&MI_ZF))goto mi_004baf96;
    /* 004baf89 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004baf8b */
    if((f->flags&MI_ZF))goto mi_004baf96;
    /* 004baf8d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[6]),2u);
    /* 004baf94 */
    goto mi_004baf9d;
mi_004baf96:;
    /* 004baf96 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[6]),2u);
mi_004baf9d:;
    /* 004baf9d */
    f->r[1]=f->r[0];
    /* 004baf9f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bafa2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bafa4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a80u+f->r[1]*8u),1u))&0xffu)<<0);
    /* 004bafab */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bafad */
    if((f->flags&MI_ZF))goto mi_004bafcb;
    /* 004bafaf */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bafb6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bafbd */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bafbf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bafc0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bafc1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bafc6u;f->boundary(0x004bafc1u,0x004835e0u,f);
    /* 004bafc6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bafc9 */
    goto mi_004bb01e;
mi_004bafcb:;
    /* 004bafcb */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bafd2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bafd9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bafdb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bafdc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bafdd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bafe2u;f->boundary(0x004bafddu,0x004835e0u,f);
    /* 004bafe2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bafe5 */
    goto mi_004bb01e;
mi_004bafe7:;
    /* 004bafe7 */
    a=(f->r[0]&0xffu);b=0x00000012u;t=mi_alu(f,a,b,8,1);
    /* 004bafe9 */
    if(!(f->flags&MI_ZF))goto mi_004bb002;
    /* 004bafeb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004baff2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004baff9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baffa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004baffb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb000u;f->boundary(0x004baffbu,0x004836f0u,f);
    /* 004bb000 */
    goto mi_004bb01b;
mi_004bb002:;
    /* 004bb002 */
    a=(f->r[0]&0xffu);b=0x00000069u;t=mi_alu(f,a,b,8,1);
    /* 004bb004 */
    if(!(f->flags&MI_ZF))goto mi_004bb020;
    /* 004bb006 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004bb00d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004bb014 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb015 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb016 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb01bu;f->boundary(0x004bb016u,0x00483800u,f);
mi_004bb01b:;
    /* 004bb01b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004bb01e:;
    /* 004bb01e */
    f->r[3]=f->r[0];
mi_004bb020:;
    /* 004bb020 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[6]),2u);
    /* 004bb027 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[6]),1u))&0xffu)<<0);
    /* 004bb02d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bb02e */
    a=(f->r[2]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004bb031 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004bb032 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bb034 */
    if((f->flags&MI_ZF))goto mi_004bb060;
    /* 004bb036 */
    f->r[1]=f->r[0];
    /* 004bb038 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb03b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb03d */
    a=mi_read((0x007999f0u+f->r[1]*8u),2u);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x007999f0u+f->r[1]*8u))=(uint16_t)(t);
    /* 004bb045 */
    f->r[0]=(0x007999f0u+f->r[1]*8u);
    /* 004bb04c */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999f0u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 004bb054 */
    a=(f->r[1]&0xffffu);b=0x0000012cu;t=mi_alu(f,a,b,16,1);
    /* 004bb059 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bb060;
    /* 004bb05b */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)(0x0000012cu);
mi_004bb060:;
    /* 004bb060 */
    f->r[0]=mi_read((0x00bcd618u+f->r[6]),4u);
    /* 004bb066 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb06c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb06d */
    f->r[1]=mi_read((0x005dbf70u+f->r[2]*4u),4u);
    /* 004bb074 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb075 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb07au;f->boundary(0x004bb075u,0x0047e250u,f);
    /* 004bb07a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004bb07d:;
    /* 004bb07d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bb07e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004bb080 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004bb081 */
    f->r[4]+=4;return;
}
void restore_routing_C_004b21d0(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b21d0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b21d1 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004b21d5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b21d6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b21d7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b21d8 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b21db */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b21dc */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b21df */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b21e1 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b21e4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[6]),2u);
    /* 004b21eb */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[6]),2u);
    /* 004b21f2 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004b21f5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004b21f8 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004b21fb */
    f->r[7]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 004b21fe */
    f->r[2]=mi_read((0x00bca2d8u),4u);
    /* 004b2204 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004b2206 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61eu+f->r[6]),2u);
    /* 004b220d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b220e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd61cu+f->r[6]),2u);
    /* 004b2215 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2216 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2217 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2218 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b221du;f->boundary(0x004b2218u,0x0051cba0u,f);
    /* 004b221d */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004b2222 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2225 */
    f->r[1]=f->r[0];
    /* 004b2227 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004b222a */
    if(mi_signed_less(f->flags))goto mi_004b222e;
    /* 004b222c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004b222e:;
    /* 004b222e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b2230 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 004b2238 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004b223c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004b223e */
    f->r[0]=f->r[1];
mi_004b2240:;
    /* 004b2240 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 004b2242 */
    if(!(f->flags&MI_ZF))goto mi_004b2257;
    /* 004b2244 */
    f->r[3]=mi_read((0x005f80fcu+f->r[0]*4u),4u);
    /* 004b224b */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004b224d */
    a=mi_read((0x00af598cu+f->r[3]*4u),1u);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004b2255 */
    if(!(f->flags&MI_ZF))goto mi_004b226c;
mi_004b2257:;
    /* 004b2257 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b2258 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004b225b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b225f;
    /* 004b225d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004b225f:;
    /* 004b225f */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004b2260 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004b2261 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004b2264 */
    if(mi_signed_less(f->flags))goto mi_004b2240;
    /* 004b2266 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00000018u+f->r[4]),1u))&0xffu)<<0);
    /* 004b226a */
    goto mi_004b226e;
mi_004b226c:;
    /* 004b226c */
    f->r[3]=f->r[0];
mi_004b226e:;
    /* 004b226e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b2270 */
    f->r[0]=f->r[1];
mi_004b2272:;
    /* 004b2272 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 004b2274 */
    if(!(f->flags&MI_ZF))goto mi_004b2289;
    /* 004b2276 */
    f->r[1]=mi_read((0x005f80fcu+f->r[0]*4u),4u);
    /* 004b227d */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b227f */
    a=mi_read((0x00af598cu+f->r[1]*4u),1u);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004b2287 */
    if(!(f->flags&MI_ZF))goto mi_004b229b;
mi_004b2289:;
    /* 004b2289 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b228a */
    if(!(f->flags&MI_SF))goto mi_004b2291;
    /* 004b228c */
    f->r[0]=0x00000007u;
mi_004b2291:;
    /* 004b2291 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004b2292 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004b2295 */
    if(mi_signed_less(f->flags))goto mi_004b2272;
    /* 004b2297 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000010u+f->r[4]),1u))&0xffu)<<0);
mi_004b229b:;
    /* 004b229b */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004b229d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b22b9;
    /* 004b229f */
    MI_U8((0x00bcd609u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 004b22a5 */
    MI_U8((0x00bcd640u+f->r[6]))=(uint8_t)(0x000000feu);
    /* 004b22ac */
    MI_U8((0x00bcd641u+f->r[6]))=(uint8_t)(0x00000005u);
    /* 004b22b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b22b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b22b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b22b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b22b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b22b8 */
    f->r[4]+=4;return;
mi_004b22b9:;
    /* 004b22b9 */
    MI_U8((0x00bcd609u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b22bf */
    MI_U8((0x00bcd640u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 004b22c6 */
    MI_U8((0x00bcd641u+f->r[6]))=(uint8_t)(0x00000005u);
    /* 004b22cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b22ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b22cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b22d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b22d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b22d2 */
    f->r[4]+=4;return;
}
void restore_routing_C_0045bb50(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045bb50 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0045bb53 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bb54 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bb55 */
    f->r[6]=mi_read((0x00000030u+f->r[4]),4u);
    /* 0045bb59 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00002710u);
    /* 0045bb61 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0045bb63 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0045bb6b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045bb77;
    /* 0045bb6d */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0045bb70 */
    MI_U8((0x0000000bu+f->r[4]))=(uint8_t)(0x00000001u);
    /* 0045bb75 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045bb7c;
mi_0045bb77:;
    /* 0045bb77 */
    MI_U8((0x0000000bu+f->r[4]))=(uint8_t)(0x00000000u);
mi_0045bb7c:;
    /* 0045bb7c */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0045bb81 */
    a=f->r[0];b=0x00001276u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045bb87 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045bb89 */
    f->r[1]=mi_read((0x00ea42f8u+f->r[0]*4u),4u);
    /* 0045bb90 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0045bb92 */
    if((f->flags&MI_ZF))goto mi_0045bb9c;
    /* 0045bb94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045bb95 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045bb97 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045bb98 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045bb9b */
    f->r[4]+=4;return;
mi_0045bb9c:;
    /* 0045bb9c */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0045bba0 */
    MI_U8((0x0000000au+f->r[4]))=(uint8_t)(0x00000001u);
    /* 0045bba5 */
    f->r[5]=f->r[1];
    /* 0045bba7 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0045bbaa */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0045bbac */
    a=f->r[5];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0045bbaf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[5]),2u);
    /* 0045bbb6 */
    a=f->r[0];b=0xffffffb9u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045bbb9 */
    a=f->r[0];b=0x000000a5u;t=mi_alu(f,a,b,32,1);
    /* 0045bbbe */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0045bbec;
    /* 0045bbc0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0045bbc2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0045be4cu+f->r[0]),1u))&0xffu)<<0);
    /* 0045bbc8 */
    switch(mi_read((0x0045be40u+f->r[2]*4u),4u)){case 0x0045bbcfu:goto mi_0045bbcf;case 0x0045bbe7u:goto mi_0045bbe7;case 0x0045bbecu:goto mi_0045bbec;default:abort();}
mi_0045bbcf:;
    /* 0045bbcf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045bbd1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a99u+f->r[5]),1u))&0xffu)<<0);
    /* 0045bbd7 */
    a=f->r[0];b=0x000000c4u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045bbdd */
    a=mi_read((0x007900c2u+f->r[0]+f->r[6]*1u),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0045bbe5 */
    if(!(f->flags&MI_ZF))goto mi_0045bbec;
mi_0045bbe7:;
    /* 0045bbe7 */
    MI_U8((0x0000000au+f->r[4]))=(uint8_t)(0x00000000u);
mi_0045bbec:;
    /* 0045bbec */
    f->r[2]=mi_read((0x0078d788u),4u);
    /* 0045bbf2 */
    f->r[0]=0x00000001u;
    /* 0045bbf7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045bbf9 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045bbfd */
    if(mi_signed_less(f->flags))goto mi_0045be0c;
    /* 0045bc03 */
    f->r[2]=0xfffffffeu;
    /* 0045bc08 */
    f->r[0]=0xfffffffau;
    /* 0045bc0d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bc0e */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045bc10 */
    f->r[3]=(0x00790186u+f->r[6]);
    /* 0045bc16 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045bc18 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bc19 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0045bc1d */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0045bc21 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045bc25 */
    goto mi_0045bc33;
mi_0045bc27:;
    /* 0045bc27 */
    f->r[0]=mi_read((0x00000028u+f->r[4]),4u);
    /* 0045bc2b */
    f->r[2]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0045bc2f */
    f->r[1]=mi_read((0x00000034u+f->r[4]),4u);
mi_0045bc33:;
    /* 0045bc33 */
    a=mi_read((0x00000000u+f->r[2]+f->r[3]*1u),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0045bc37 */
    if((f->flags&MI_ZF))goto mi_0045bde1;
    /* 0045bc3d */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[0]+f->r[3]*1u),2u);
    /* 0045bc41 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0045bc43 */
    if((f->flags&MI_ZF))goto mi_0045bde1;
    /* 0045bc49 */
    f->r[2]=f->r[7];
    /* 0045bc4b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045bc4e */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045bc50 */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045bc53 */
    a=mi_read((0x007999d8u+f->r[2]),1u);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045bc5a */
    if(!(f->flags&MI_ZF))goto mi_0045bde1;
    /* 0045bc60 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045bc62 */
    if((f->flags&MI_ZF))goto mi_0045bde1;
    /* 0045bc68 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000012u+f->r[4]),1u))&0xffu)<<0);
    /* 0045bc6c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045bc6e */
    if((f->flags&MI_ZF))goto mi_0045bc84;
    /* 0045bc70 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999ecu+f->r[2]),1u))&0xffu)<<0);
    /* 0045bc76 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999ecu+f->r[5]),1u))&0xffu)<<0);
    /* 0045bc7c */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0045bc7e */
    if(!(f->flags&MI_ZF))goto mi_0045bde1;
mi_0045bc84:;
    /* 0045bc84 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[2]),2u))&0xffffu)<<0);
    /* 0045bc8b */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0045bc8f */
    if(!(f->flags&MI_ZF))goto mi_0045bd66;
    /* 0045bc95 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0045bc97 */
    f->r[1]=f->r[7];
    /* 0045bc99 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000008u);
mi_0045bca1:;
    /* 0045bca1 */
    f->r[0]=f->r[1];
    /* 0045bca3 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045bca6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045bca8 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a12u+f->r[0]*8u),2u);
    /* 0045bcb0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0045bcb2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045bcdc;
    /* 0045bcb4 */
    f->r[0]=f->r[1];
    /* 0045bcb6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045bcb9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045bcbb */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045bcbe */
    f->r[6]=(f->r[6]&0xffff0000u)|(((mi_read((0x00799a14u+f->r[0]),2u))&0xffffu)<<0);
    /* 0045bcc5 */
    a=(f->r[6]&0xffffu);b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0045bcc8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045bcdc;
    /* 0045bcca */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999eau+f->r[0]),2u);
    /* 0045bcd1 */
    a=f->r[0];b=mi_read((0x00000038u+f->r[4]),4u);t=mi_alu(f,a,b,32,1);
    /* 0045bcd5 */
    if(!(f->flags&MI_ZF))goto mi_0045bcdc;
    /* 0045bcd7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 0045bcda */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0045bcdc:;
    /* 0045bcdc */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0045bce0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045bce1 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045bce5 */
    if(!(f->flags&MI_ZF))goto mi_0045bca1;
    /* 0045bce7 */
    a=f->r[3];b=0x00000190u;t=mi_alu(f,a,b,32,1);
    /* 0045bced */
    if(!mi_signed_less(f->flags))goto mi_0045bcfb;
    /* 0045bcef */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0000003cu+f->r[4]),1u))&0xffu)<<0);
    /* 0045bcf3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045bcf5 */
    if(!(f->flags&MI_ZF))goto mi_0045bdd9;
mi_0045bcfb:;
    /* 0045bcfb */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0045bcfd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045bdd9;
    /* 0045bd03 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0045bd07 */
    a=mi_read((0x00000000u+f->r[3]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0045bd0a */
    if(!(f->flags&MI_ZF))goto mi_0045bd18;
    /* 0045bd0c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0000003cu+f->r[4]),1u))&0xffu)<<0);
    /* 0045bd10 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045bd12 */
    if((f->flags&MI_ZF))goto mi_0045bddd;
mi_0045bd18:;
    /* 0045bd18 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999f8u+f->r[2]),2u);
    /* 0045bd1f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999f8u+f->r[5]),2u);
    /* 0045bd26 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd28 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[5]),2u);
    /* 0045bd2f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[5]),2u);
    /* 0045bd36 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd37 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd38 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[2]),2u);
    /* 0045bd3f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[2]),2u);
    /* 0045bd46 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd47 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd48 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045bd4du;f->boundary(0x0045bd48u,0x0045dc50u,f);
    /* 0045bd4d */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0045bd51 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045bd54 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045bd56 */
    if(!mi_signed_less(f->flags))goto mi_0045bddd;
    /* 0045bd5c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045bd60 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0045bd64 */
    goto mi_0045bddd;
mi_0045bd66:;
    /* 0045bd66 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 0045bd6a */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045bd6c */
    if((f->flags&MI_ZF))goto mi_0045bde1;
    /* 0045bd6e */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0045bd72 */
    if(!(f->flags&MI_ZF))goto mi_0045bde1;
    /* 0045bd74 */
    f->r[0]=f->r[7];
    /* 0045bd76 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045bd79 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045bd7b */
    f->r[1]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 0045bd7e */
    a=mi_read((0x00799a2eu+f->r[1]*2u),2u);b=0x00000190u;t=mi_alu(f,a,b,16,1);
    /* 0045bd88 */
    if(mi_signed_less(f->flags))goto mi_0045bde1;
    /* 0045bd8a */
    a=mi_read((0x00000000u+f->r[3]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0045bd8d */
    if((f->flags&MI_ZF))goto mi_0045bde1;
    /* 0045bd8f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999f8u+f->r[2]),2u);
    /* 0045bd96 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999f8u+f->r[5]),2u);
    /* 0045bd9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd9e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bd9f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[5]),2u);
    /* 0045bda6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[5]),2u);
    /* 0045bdad */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bdae */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bdaf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[2]),2u);
    /* 0045bdb6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[2]),2u);
    /* 0045bdbd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bdbe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045bdbf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045bdc4u;f->boundary(0x0045bdbfu,0x0045dc50u,f);
    /* 0045bdc4 */
    f->r[1]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0045bdc8 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045bdcb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045bdcd */
    if(!mi_signed_less(f->flags))goto mi_0045bde1;
    /* 0045bdcf */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045bdd3 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0045bdd7 */
    goto mi_0045bde1;
mi_0045bdd9:;
    /* 0045bdd9 */
    f->r[3]=mi_read((0x00000018u+f->r[4]),4u);
mi_0045bddd:;
    /* 0045bddd */
    f->r[6]=mi_read((0x00000038u+f->r[4]),4u);
mi_0045bde1:;
    /* 0045bde1 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0045bde5 */
    f->r[1]=mi_read((0x0078d788u),4u);
    /* 0045bdeb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045bdec */
    a=f->r[3];b=0x000000c4u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0045bdf2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045bdf4 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045bdf8 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0045bdfc */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045bc27;
    /* 0045be02 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0045be06 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045be07 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0045be09 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045be0a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0045be14;
mi_0045be0c:;
    /* 0045be0c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045be0d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045be0f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045be10 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045be13 */
    f->r[4]+=4;return;
mi_0045be14:;
    /* 0045be14 */
    f->r[0]=f->r[1];
    /* 0045be16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045be17 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045be1a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045be1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0045be1d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045be20 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999feu+f->r[0]),2u);
    /* 0045be27 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a00u+f->r[0]),2u);
    /* 0045be2e */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[0]);
    /* 0045be33 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[2]);
    /* 0045be39 */
    f->r[0]=f->r[1];
    /* 0045be3b */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045be3e */
    f->r[4]+=4;return;
}
void restore_routing_C_0047e340(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047e340 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0047e343 */
    f->r[0]=mi_read((0x00000030u+f->r[4]),4u);
    /* 0047e347 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0047e349 */
    MI_U8((0x00000003u+f->r[4]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047e34d */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047e350 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0047e353 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047e355 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 0047e35c */
    a=(f->r[0]&0xffu);b=0x00000062u;t=mi_alu(f,a,b,8,1);
    /* 0047e35e */
    MI_U8((0x00000004u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 0047e362 */
    if(!(f->flags&MI_ZF))goto mi_0047e369;
    /* 0047e364 */
    MI_U8((0x00000003u+f->r[4]))=(uint8_t)(0x00000001u);
mi_0047e369:;
    /* 0047e369 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e36a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e36b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e36c */
    f->r[6]=mi_read((0x00000038u+f->r[4]),4u);
    /* 0047e370 */
    MI_U32((0x00bc97dcu))=(uint32_t)(f->r[1]);
    /* 0047e376 */
    MI_U32((0x00bc97d4u))=(uint32_t)(f->r[1]);
    /* 0047e37c */
    f->r[2]=mi_read((0x00af5d1cu+f->r[6]*4u),4u);
    /* 0047e383 */
    f->r[0]=mi_read((0x00af55fcu+f->r[6]*4u),4u);
    /* 0047e38a */
    f->r[3]=f->r[2];
    /* 0047e38c */
    MI_U32((0x00bc97ccu))=(uint32_t)(f->r[1]);
    /* 0047e392 */
    a=f->r[3];b=0x00000440u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 0047e398 */
    MI_U32((0x00bc97c4u))=(uint32_t)(f->r[1]);
    /* 0047e39e */
    f->r[1]=mi_read((0x00af5990u+f->r[6]*4u),4u);
    /* 0047e3a5 */
    MI_U32((0x00bc97d0u))=(uint32_t)(f->r[3]);
    /* 0047e3ab */
    f->r[3]=mi_read((0x00af5988u+f->r[6]*4u),4u);
    /* 0047e3b2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e3b3 */
    f->r[7]=f->r[0];
    /* 0047e3b5 */
    f->r[5]=f->r[1];
    /* 0047e3b7 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0047e3bb */
    a=f->r[7];b=0x00000440u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0047e3c1 */
    a=f->r[5];b=0x00000440u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0047e3c7 */
    a=f->r[3];b=0x00000440u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 0047e3cd */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0047e3cf */
    MI_U32((0x00bc97c0u))=(uint32_t)(f->r[7]);
    /* 0047e3d5 */
    MI_U32((0x00bc97c8u))=(uint32_t)(f->r[5]);
    /* 0047e3db */
    MI_U32((0x00bc97d8u))=(uint32_t)(f->r[3]);
    /* 0047e3e1 */
    if((f->flags&MI_ZF))goto mi_0047e428;
    /* 0047e3e3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e534u+f->r[6]*2u),2u);
    /* 0047e3eb */
    f->r[3]=f->r[0];
    /* 0047e3ed */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 0047e3f0 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0047e3f2 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[3]*8u),2u))&0xffffu)<<0);
    /* 0047e3fa */
    a=(f->r[0]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 0047e3fe */
    if((f->flags&MI_ZF))goto mi_0047e41a;
    /* 0047e400 */
    a=(f->r[0]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 0047e404 */
    if((f->flags&MI_ZF))goto mi_0047e41a;
    /* 0047e406 */
    a=(f->r[0]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0047e40a */
    if(!(f->flags&MI_ZF))goto mi_0047e414;
    /* 0047e40c */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 0047e410 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047e412 */
    if((f->flags&MI_ZF))goto mi_0047e41a;
mi_0047e414:;
    /* 0047e414 */
    a=(f->r[0]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 0047e418 */
    if(!(f->flags&MI_ZF))goto mi_0047e422;
mi_0047e41a:;
    /* 0047e41a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0047e41c */
    MI_U32((0x00bc97c0u))=(uint32_t)(f->r[7]);
mi_0047e422:;
    /* 0047e422 */
    f->r[3]=mi_read((0x00bc97d8u),4u);
mi_0047e428:;
    /* 0047e428 */
    a=(f->r[1]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0047e42b */
    if((f->flags&MI_ZF))goto mi_0047e46c;
    /* 0047e42d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6feu+f->r[6]*2u),2u);
    /* 0047e435 */
    f->r[1]=f->r[0];
    /* 0047e437 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0047e43a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047e43c */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 0047e444 */
    a=(f->r[0]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 0047e448 */
    if((f->flags&MI_ZF))goto mi_0047e464;
    /* 0047e44a */
    a=(f->r[0]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 0047e44e */
    if((f->flags&MI_ZF))goto mi_0047e464;
    /* 0047e450 */
    a=(f->r[0]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0047e454 */
    if(!(f->flags&MI_ZF))goto mi_0047e45e;
    /* 0047e456 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 0047e45a */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047e45c */
    if((f->flags&MI_ZF))goto mi_0047e464;
mi_0047e45e:;
    /* 0047e45e */
    a=(f->r[0]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 0047e462 */
    if(!(f->flags&MI_ZF))goto mi_0047e46c;
mi_0047e464:;
    /* 0047e464 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0047e466 */
    MI_U32((0x00bc97c8u))=(uint32_t)(f->r[5]);
mi_0047e46c:;
    /* 0047e46c */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0047e46f */
    if((f->flags&MI_ZF))goto mi_0047e4b2;
    /* 0047e471 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e8c4u+f->r[6]*2u),2u);
    /* 0047e479 */
    f->r[2]=f->r[0];
    /* 0047e47b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0047e47e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047e480 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[2]*8u),2u))&0xffffu)<<0);
    /* 0047e488 */
    a=(f->r[0]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 0047e48c */
    if((f->flags&MI_ZF))goto mi_0047e4a8;
    /* 0047e48e */
    a=(f->r[0]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 0047e492 */
    if((f->flags&MI_ZF))goto mi_0047e4a8;
    /* 0047e494 */
    a=(f->r[0]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0047e498 */
    if(!(f->flags&MI_ZF))goto mi_0047e4a2;
    /* 0047e49a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 0047e49e */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047e4a0 */
    if((f->flags&MI_ZF))goto mi_0047e4a8;
mi_0047e4a2:;
    /* 0047e4a2 */
    a=(f->r[0]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 0047e4a6 */
    if(!(f->flags&MI_ZF))goto mi_0047e4b2;
mi_0047e4a8:;
    /* 0047e4a8 */
    MI_U32((0x00bc97d0u))=(uint32_t)(0x00000000u);
mi_0047e4b2:;
    /* 0047e4b2 */
    a=mi_read((0x00000040u+f->r[4]),1u);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0047e4b7 */
    if((f->flags&MI_ZF))goto mi_0047e4f8;
    /* 0047e4b9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fau+f->r[6]*2u),2u);
    /* 0047e4c1 */
    f->r[1]=f->r[0];
    /* 0047e4c3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0047e4c6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047e4c8 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 0047e4d0 */
    a=(f->r[0]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 0047e4d4 */
    if((f->flags&MI_ZF))goto mi_0047e4f0;
    /* 0047e4d6 */
    a=(f->r[0]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 0047e4da */
    if((f->flags&MI_ZF))goto mi_0047e4f0;
    /* 0047e4dc */
    a=(f->r[0]&0xffffu);b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 0047e4e0 */
    if(!(f->flags&MI_ZF))goto mi_0047e4ea;
    /* 0047e4e2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000013u+f->r[4]),1u))&0xffu)<<0);
    /* 0047e4e6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047e4e8 */
    if((f->flags&MI_ZF))goto mi_0047e4f0;
mi_0047e4ea:;
    /* 0047e4ea */
    a=(f->r[0]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 0047e4ee */
    if(!(f->flags&MI_ZF))goto mi_0047e4f8;
mi_0047e4f0:;
    /* 0047e4f0 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0047e4f2 */
    MI_U32((0x00bc97d8u))=(uint32_t)(f->r[3]);
mi_0047e4f8:;
    /* 0047e4f8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0047e4fa */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047e4fc */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0047e500 */
    if((f->flags&MI_ZF))goto mi_0047e50e;
    /* 0047e502 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0047e50a */
    f->r[1]=mi_read((0x00000040u+f->r[4]),4u);
mi_0047e50e:;
    /* 0047e50e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0047e510 */
    if((f->flags&MI_ZF))goto mi_0047e517;
    /* 0047e512 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047e513 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0047e517:;
    /* 0047e517 */
    f->r[0]=mi_read((0x00bc97d0u),4u);
    /* 0047e51c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047e51e */
    if((f->flags&MI_ZF))goto mi_0047e525;
    /* 0047e520 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047e521 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0047e525:;
    /* 0047e525 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0047e527 */
    if((f->flags&MI_ZF))goto mi_0047e52e;
    /* 0047e529 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047e52a */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0047e52e:;
    /* 0047e52e */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0047e531 */
    if(mi_signed_less(f->flags))goto mi_0047e5af;
    /* 0047e533 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0047e537 */
    f->r[3]=0x00000007u;
    /* 0047e53c */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047e541 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0047e543 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0047e547 */
    f->r[5]=mi_read((0x005dbf70u+f->r[0]*4u),4u);
mi_0047e54e:;
    /* 0047e54e */
    f->r[0]=mi_read((0x00bc97c0u+f->r[7]),4u);
    /* 0047e554 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047e556 */
    if((f->flags&MI_ZF))goto mi_0047e582;
    /* 0047e558 */
    f->r[2]=mi_read((0x005dc128u+f->r[7]),4u);
    /* 0047e55e */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047e560 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e561 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047e562 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047e567u;f->boundary(0x0047e562u,0x0047e230u,f);
    /* 0047e567 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047e56a */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047e56c */
    MI_U32((0x00000018u+f->r[4]+f->r[7]*1u))=(uint32_t)(f->r[0]);
    /* 0047e570 */
    if(mi_signed_less(f->flags))goto mi_0047e574;
    /* 0047e572 */
    f->r[3]=f->r[0];
mi_0047e574:;
    /* 0047e574 */
    a=mi_read((0x0000003cu+f->r[4]),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047e578 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047e57e;
    /* 0047e57a */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_0047e57e:;
    /* 0047e57e */
    f->r[1]=mi_read((0x00000040u+f->r[4]),4u);
mi_0047e582:;
    /* 0047e582 */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0047e585 */
    a=f->r[7];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 0047e588 */
    if(mi_signed_less(f->flags))goto mi_0047e54e;
    /* 0047e58a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0047e58c:;
    /* 0047e58c */
    f->r[2]=mi_read((0x00bc97c0u+f->r[0]),4u);
    /* 0047e592 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0047e594 */
    if((f->flags&MI_ZF))goto mi_0047e5a7;
    /* 0047e596 */
    a=mi_read((0x00000018u+f->r[4]+f->r[0]*1u),4u);b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047e59a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047e5a7;
    /* 0047e59c */
    MI_U32((0x00bc97c0u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0047e5a6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_0047e5a7:;
    /* 0047e5a7 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047e5aa */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 0047e5ad */
    if(mi_signed_less(f->flags))goto mi_0047e58c;
mi_0047e5af:;
    /* 0047e5af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047e5b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047e5b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047e5b2 */
    f->r[0]=f->r[1];
    /* 0047e5b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047e5b5 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047e5b8 */
    f->r[4]+=4;return;
}
void restore_routing_C_0045dc50(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045dc50 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0045dc54 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0045dc58 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045dc5a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045dc5b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045dc63;
    /* 0045dc5d */
    f->r[6]=f->r[1];
    /* 0045dc5f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0045dc61 */
    goto mi_0045dc67;
mi_0045dc63:;
    /* 0045dc63 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045dc65 */
    f->r[6]=f->r[0];
mi_0045dc67:;
    /* 0045dc67 */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0045dc6a */
    if(!(f->flags&MI_ZF))goto mi_0045dc6e;
    /* 0045dc6c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0045dc6e:;
    /* 0045dc6e */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0045dc72 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0045dc76 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045dc78 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045dc80;
    /* 0045dc7a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045dc7c */
    f->r[2]=f->r[0];
    /* 0045dc7e */
    goto mi_0045dc8a;
mi_0045dc80:;
    /* 0045dc80 */
    if(!mi_signed_less(f->flags))goto mi_0045dc88;
    /* 0045dc82 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045dc84 */
    f->r[2]=f->r[1];
    /* 0045dc86 */
    goto mi_0045dc8a;
mi_0045dc88:;
    /* 0045dc88 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0045dc8a:;
    /* 0045dc8a */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0045dc8e */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0045dc92 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045dc94 */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 0045dc9a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045dca0;
    /* 0045dc9c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0045dc9e */
    goto mi_0045dcaa;
mi_0045dca0:;
    /* 0045dca0 */
    if(!mi_signed_less(f->flags))goto mi_0045dca8;
    /* 0045dca2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0045dca4 */
    f->r[0]=f->r[1];
    /* 0045dca6 */
    goto mi_0045dcaa;
mi_0045dca8:;
    /* 0045dca8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0045dcaa:;
    /* 0045dcaa */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045dcac */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[0]);
    /* 0045dcb1 */
    if(!mi_signed_less(f->flags))goto mi_0045dcb7;
    /* 0045dcb3 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045dcb5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045dcb6 */
    f->r[4]+=4;return;
mi_0045dcb7:;
    /* 0045dcb7 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 0045dcba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0045dcbb */
    f->r[4]+=4;return;
}
void restore_routing_C_0047e230(struct restore_routing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047e230 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0047e234 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047e237 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0047e23b */
    f->r[0]=mi_read((0x00978d9cu+f->r[0]*4u),4u);
    /* 0047e242 */
    a=f->r[0];b=((f->r[1]&0xffu))&31u;if(b){t=a>>b;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>(b-1u))&1u)|mi_szp(t,32);if(b==1u)f->flags=(f->flags&~MI_OF)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;} /* AF and OF for count>1 are architecturally unspecified; retained bits are not a status-equivalence claim. */
    /* 0047e244 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047e247 */
    f->r[4]+=4;return;
}

/* Eleven byte-identical native service loops differ only in this field.
   Sharing the whole frame body also retains native caller-slot writes. */
void restore_routing_C_00484450(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a46u);}
void restore_routing_C_00484120(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a4au);}
void restore_routing_C_00484230(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a4bu);}
void restore_routing_C_00484340(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a4du);}
void restore_routing_C_00484560(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a4eu);}
void restore_routing_C_00484780(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a50u);}
void restore_routing_C_004833c0(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a44u);}
void restore_routing_C_004834d0(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a45u);}
void restore_routing_C_004835e0(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a47u);}
void restore_routing_C_004836f0(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a49u);}
void restore_routing_C_00483800(struct restore_routing_frame *f)
{services_house_visit(f,0x00799a5eu);}
