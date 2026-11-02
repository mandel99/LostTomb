/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_base_model.h"
#include <stdlib.h>
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
static __inline uint32_t mi_alu(struct restore_building_world_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_building_world_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_building_world_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_00437f98[]={0x43,0x6f,0x43,0x00,0xef,0x6d,0x43,0x00};
static const uint8_t mi_rom_00437fa0[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00};
static const uint8_t mi_rom_00437fd0[]={0x43,0x6f,0x43,0x00,0x86,0x6e,0x43,0x00};
static const uint8_t mi_rom_00437fd8[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00};
static const uint8_t mi_rom_00438008[]={0x4b,0x72,0x43,0x00,0xb1,0x70,0x43,0x00};
static const uint8_t mi_rom_00438010[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00};
static const uint8_t mi_rom_00438040[]={0x4b,0x72,0x43,0x00,0x48,0x71,0x43,0x00};
static const uint8_t mi_rom_00438048[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00};
static const uint8_t mi_rom_00438078[]={0xcb,0x7a,0x43,0x00,0xff,0x7a,0x43,0x00,0x19,0x7b,0x43,0x00,0xe5,0x7a,0x43,0x00,0x33,0x7b,0x43,0x00,0xf4,0x77,0x43,0x00,0x24,0x78,0x43,0x00,0x49,0x7a,0x43,0x00,0x2f,0x79,0x43,0x00,0x71,0x7a,0x43,0x00,0x66,0x78,0x43,0x00,0xb1,0x7a,0x43,0x00,0x97,0x7a,0x43,0x00,0x4d,0x7b,0x43,0x00};
static const uint8_t mi_rom_004380b0[]={0x00,0x01,0x0d,0x02,0x03,0x04,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x05,0x06,0x0d,0x07,0x08,0x09,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0a,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0b,0x0b,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0c,0x0c,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x05,0x08,0x06,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0b,0x0c,0x0d,0x0d,0x0d,0x0b};
static const uint8_t mi_rom_00438170[]={0x98,0x78,0x43,0x00,0xac,0x78,0x43,0x00,0xc0,0x78,0x43,0x00,0xd4,0x78,0x43,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00437f98u&&width<=8u&&(offset=address-0x00437f98u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00437f98[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00437fa0u&&width<=45u&&(offset=address-0x00437fa0u)<=45u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00437fa0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00437fd0u&&width<=8u&&(offset=address-0x00437fd0u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00437fd0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00437fd8u&&width<=45u&&(offset=address-0x00437fd8u)<=45u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00437fd8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00438008u&&width<=8u&&(offset=address-0x00438008u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00438008[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00438010u&&width<=45u&&(offset=address-0x00438010u)<=45u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00438010[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00438040u&&width<=8u&&(offset=address-0x00438040u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00438040[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00438048u&&width<=45u&&(offset=address-0x00438048u)<=45u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00438048[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00438078u&&width<=56u&&(offset=address-0x00438078u)<=56u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00438078[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004380b0u&&width<=192u&&(offset=address-0x004380b0u)<=192u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004380b0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00438170u&&width<=16u&&(offset=address-0x00438170u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00438170[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_00436b60(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00436b60 */
    a=f->r[4];b=0x00000060u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00436b63 */
    f->r[0]=mi_read((0x0070e44cu),4u);
    /* 00436b68 */
    f->r[2]=mi_read((0x0070e460u),4u);
    /* 00436b6e */
    f->r[1]=mi_read((0x0070e474u),4u);
    /* 00436b74 */
    a=mi_read((0x00775688u),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00775688u))=(uint16_t)(t);
    /* 00436b7b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436b7c */
    f->r[6]=mi_read((0x0070e48cu),4u);
    /* 00436b82 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00436b85 */
    a=f->r[2];b=0x00000012u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436b88 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00436b8a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00436b8c */
    a=f->r[1];b=0xfffffff8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436b8f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000004u);
    /* 00436b97 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00436b99 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00436b9d */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00436ba1 */
    MI_U32((0x0070e47cu))=(uint32_t)(f->r[1]);
    /* 00436ba7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00437f91;
    /* 00436bad */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436bae */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436baf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00436bb0:;
    /* 00436bb0 */
    f->r[0]=mi_read((0x0070e47cu),4u);
    /* 00436bb5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00436bb7 */
    if(mi_signed_less(f->flags))goto mi_00437f4b;
    /* 00436bbd */
    a=f->r[0];b=0x000001c9u;t=mi_alu(f,a,b,32,1);
    /* 00436bc2 */
    if(!mi_signed_less(f->flags))goto mi_00437f4b;
    /* 00436bc8 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00436bcc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00436bce */
    f->r[0]=mi_read((0x0070e490u),4u);
    /* 00436bd3 */
    f->r[7]=f->r[0];
    /* 00436bd5 */
    if((f->flags&MI_ZF))goto mi_00436bde;
    /* 00436bd7 */
    a=f->r[7];b=mi_read((0x0070e45cu),4u);t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00436bdd */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_00436bde:;
    /* 00436bde */
    f->r[5]=mi_read((0x0070e458u),4u);
    /* 00436be4 */
    f->r[2]=mi_read((0x0070e478u),4u);
    /* 00436bea */
    f->r[1]=0xfffffffeu;
    /* 00436bef */
    a=f->r[2];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436bf2 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00436bf4 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00436bfc */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[2]);
    /* 00436c02 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[1]*4u);
    /* 00436c05 */
    f->r[1]=mi_read((0x0070e464u),4u);
    /* 00436c0b */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436c0e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00436c10 */
    f->r[1]=mi_read((0x0070e47cu),4u);
    /* 00436c16 */
    if(mi_signed_less(f->flags))goto mi_004374a1;
mi_00436c1c:;
    /* 00436c1c */
    f->r[0]=mi_read((0x0070e480u),4u);
    /* 00436c21 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00436c23 */
    if(mi_signed_less(f->flags))goto mi_00437471;
    /* 00436c29 */
    a=f->r[0];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 00436c2e */
    if(!mi_signed_less(f->flags))goto mi_00437471;
    /* 00436c34 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00436c37 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 00436c3a */
    f->r[2]=mi_read((0x0070e480u),4u);
    /* 00436c40 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 00436c43 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 00436c46 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[2]*1u);
    /* 00436c49 */
    f->r[2]=mi_read((0x0070e494u+f->r[3]*4u),4u);
    /* 00436c50 */
    a=f->r[2];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00436c56 */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[2]);
    /* 00436c5c */
    if(!mi_signed_less(f->flags))goto mi_00437471;
    /* 00436c62 */
    a=mi_read((0x00e94e0cu),4u);b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 00436c69 */
    if((f->flags&MI_ZF))goto mi_00436c74;
    /* 00436c6b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x006de515u),1u))&0xffu)<<0);
    /* 00436c70 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00436c72 */
    if((f->flags&MI_ZF))goto mi_00436c8b;
mi_00436c74:;
    /* 00436c74 */
    f->r[0]=mi_read((0x00af598cu+f->r[2]*4u),4u);
    /* 00436c7b */
    a=f->r[0];b=0x20000002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00436c80 */
    a=f->r[0];b=0x20000002u;t=mi_alu(f,a,b,32,1);
    /* 00436c85 */
    if((f->flags&MI_ZF))goto mi_00437471;
mi_00436c8b:;
    /* 00436c8b */
    a=mi_read((0x00b67d1cu+f->r[2]),1u);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00436c92 */
    if((f->flags&MI_ZF))goto mi_00437471;
    /* 00436c98 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[2]*2u),2u);
    /* 00436ca0 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00436ca2 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00436ca6 */
    if((f->flags&MI_ZF))goto mi_00436cc3;
    /* 00436ca8 */
    f->r[1]=f->r[5];
    /* 00436caa */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436cad */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436caf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00799a97u+f->r[1]*8u),1u))&0xffu)<<0);
    /* 00436cb6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00436cb8 */
    if((f->flags&MI_ZF))goto mi_00436cc3;
    /* 00436cba */
    MI_U16((0x0070e120u))=(uint16_t)(0x0000f863u);
mi_00436cc3:;
    /* 00436cc3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00b285ccu+f->r[2]),1u))&0xffu)<<0);
    /* 00436cc9 */
    f->r[0]=mi_read((0x00b7482cu+f->r[2]*4u),4u);
    /* 00436cd0 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00436cd3 */
    a=f->r[5];b=mi_read((0x00000014u+f->r[4]),4u);t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00436cd7 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00436cda */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00436cdf */
    a=f->r[5];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00436ce2 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00436ce6 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[5]),2u))&0xffffu)<<0);
    /* 00436ced */
    a=(f->r[5]&0xffffu);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 00436cf2 */
    if(!(f->flags&MI_ZF))goto mi_00436d11;
    /* 00436cf4 */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00436cf8 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000020u);
    /* 00436d00 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436d01 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436d02 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436d03 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436d04 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436d09u;f->boundary(0x00436d04u,0x004ef550u,f);
    /* 00436d09 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436d0c */
    goto mi_004372a7;
mi_00436d11:;
    /* 00436d11 */
    a=(f->r[5]&0xffffu);b=0x000000e6u;t=mi_alu(f,a,b,16,1);
    /* 00436d16 */
    if(!(f->flags&MI_ZF))goto mi_00436fa1;
    /* 00436d1c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00436d1e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000050u);
    /* 00436d26 */
    if(!(f->flags&MI_ZF))goto mi_00436e98;
    /* 00436d2c */
    a=mi_read((0x00af598cu+f->r[2]*4u),4u);b=0xeefed77fu;t=mi_alu(f,a,b,32,3);
    /* 00436d37 */
    if(!(f->flags&MI_ZF))goto mi_00436e91;
    /* 00436d3d */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00436d41 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00436d43 */
    if(!(f->flags&MI_ZF))goto mi_00436d55;
    /* 00436d45 */
    f->r[5]=mi_read((0x0070e0f8u+f->r[3]*4u),4u);
    /* 00436d4c */
    f->r[3]=mi_read((0x0070e0fcu+f->r[3]*4u),4u);
    /* 00436d53 */
    goto mi_00436d63;
mi_00436d55:;
    /* 00436d55 */
    f->r[5]=mi_read((0x0070e0f4u+f->r[3]*4u),4u);
    /* 00436d5c */
    f->r[3]=mi_read((0x0070e0f8u+f->r[3]*4u),4u);
mi_00436d63:;
    /* 00436d63 */
    a=f->r[5];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00436d69 */
    if(!mi_signed_less(f->flags))goto mi_00436dfa;
    /* 00436d6f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[5]*2u),2u);
    /* 00436d77 */
    f->r[1]=f->r[2];
    /* 00436d79 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436d7c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436d7e */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436d81 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 00436d8a */
    if(!(f->flags&MI_ZF))goto mi_00436dad;
    /* 00436d8c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a2eu+f->r[1]),1u))&0xffu)<<0);
    /* 00436d92 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00436d94 */
    if((f->flags&MI_ZF))goto mi_00436dad;
    /* 00436d96 */
    f->r[2]=mi_read((0x00799a40u+f->r[1]),4u);
    /* 00436d9c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00436d9e */
    if((f->flags&MI_ZF))goto mi_00436dad;
    /* 00436da0 */
    a=mi_read((0x00799a31u+f->r[1]),1u);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00436da7 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00436f43;
mi_00436dad:;
    /* 00436dad */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[5]*2u),2u);
    /* 00436db5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00436db7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436dfa;
mi_00436db9:;
    /* 00436db9 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00436dbc */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436dbf */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436dc1 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436dc4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd65du+f->r[1]),1u))&0xffu)<<0);
    /* 00436dca */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00436dcc */
    if(!(f->flags&MI_ZF))goto mi_00436def;
    /* 00436dce */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00436dd0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[1]),1u))&0xffu)<<0);
    /* 00436dd6 */
    f->r[5]=f->r[2];
    /* 00436dd8 */
    a=f->r[5];b=0xfffffff3u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00436ddb */
    a=f->r[5];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 00436dde */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00436def;
    /* 00436de0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00436de2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00437fa0u+f->r[5]),1u))&0xffu)<<0);
    /* 00436de8 */
    switch(mi_read((0x00437f98u+f->r[2]*4u),4u)){case 0x00436defu:goto mi_00436def;case 0x00436f43u:goto mi_00436f43;default:abort();}
mi_00436def:;
    /* 00436def */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[1]),2u);
    /* 00436df6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00436df8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00436db9;
mi_00436dfa:;
    /* 00436dfa */
    a=f->r[3];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00436e00 */
    if(!mi_signed_less(f->flags))goto mi_00436e91;
    /* 00436e06 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[3]*2u),2u);
    /* 00436e0e */
    f->r[1]=f->r[2];
    /* 00436e10 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436e13 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436e15 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436e18 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 00436e21 */
    if(!(f->flags&MI_ZF))goto mi_00436e44;
    /* 00436e23 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a2eu+f->r[1]),1u))&0xffu)<<0);
    /* 00436e29 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00436e2b */
    if((f->flags&MI_ZF))goto mi_00436e44;
    /* 00436e2d */
    f->r[2]=mi_read((0x00799a40u+f->r[1]),4u);
    /* 00436e33 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00436e35 */
    if((f->flags&MI_ZF))goto mi_00436e44;
    /* 00436e37 */
    a=mi_read((0x00799a31u+f->r[1]),1u);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00436e3e */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00436f43;
mi_00436e44:;
    /* 00436e44 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[3]*2u),2u);
    /* 00436e4c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00436e4e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436e91;
mi_00436e50:;
    /* 00436e50 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00436e53 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436e56 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00436e58 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00436e5b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd65du+f->r[1]),1u))&0xffu)<<0);
    /* 00436e61 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00436e63 */
    if(!(f->flags&MI_ZF))goto mi_00436e86;
    /* 00436e65 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00436e67 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[1]),1u))&0xffu)<<0);
    /* 00436e6d */
    f->r[5]=f->r[2];
    /* 00436e6f */
    a=f->r[5];b=0xfffffff3u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00436e72 */
    a=f->r[5];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 00436e75 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00436e86;
    /* 00436e77 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00436e79 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00437fd8u+f->r[5]),1u))&0xffu)<<0);
    /* 00436e7f */
    switch(mi_read((0x00437fd0u+f->r[2]*4u),4u)){case 0x00436e86u:goto mi_00436e86;case 0x00436f43u:goto mi_00436f43;default:abort();}
mi_00436e86:;
    /* 00436e86 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[1]),2u);
    /* 00436e8d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00436e8f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00436e50;
mi_00436e91:;
    /* 00436e91 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436e92 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436e93 */
    goto mi_00436f38;
mi_00436e98:;
    /* 00436e98 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00436e9b */
    if(!(f->flags&MI_ZF))goto mi_00436eb8;
    /* 00436e9d */
    f->r[5]=mi_read((0x0070e44cu),4u);
    /* 00436ea3 */
    f->r[2]=mi_read((0x0070e45cu),4u);
    /* 00436ea9 */
    f->r[1]=f->r[6];
    /* 00436eab */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00436ead */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436eae */
    f->r[1]=(0x00000001u+f->r[2]+f->r[7]*1u);
    /* 00436eb2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436eb3 */
    goto mi_00436f38;
mi_00436eb8:;
    /* 00436eb8 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00436ebb */
    if(!(f->flags&MI_ZF))goto mi_00436ed4;
    /* 00436ebd */
    f->r[1]=mi_read((0x0070e448u),4u);
    /* 00436ec3 */
    f->r[2]=f->r[6];
    /* 00436ec5 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00436ec7 */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 00436ecd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436ece */
    f->r[2]=(0x00000002u+f->r[1]+f->r[7]*1u);
    /* 00436ed2 */
    goto mi_00436f37;
mi_00436ed4:;
    /* 00436ed4 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00436ed7 */
    if(!(f->flags&MI_ZF))goto mi_00436ef1;
    /* 00436ed9 */
    f->r[5]=mi_read((0x0070e444u),4u);
    /* 00436edf */
    f->r[2]=mi_read((0x0070e454u),4u);
    /* 00436ee5 */
    f->r[1]=f->r[6];
    /* 00436ee7 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00436ee9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436eea */
    f->r[1]=(0x00000003u+f->r[2]+f->r[7]*1u);
    /* 00436eee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436eef */
    goto mi_00436f38;
mi_00436ef1:;
    /* 00436ef1 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00436ef4 */
    if(!(f->flags&MI_ZF))goto mi_00436f0d;
    /* 00436ef6 */
    f->r[1]=mi_read((0x0070e440u),4u);
    /* 00436efc */
    f->r[2]=f->r[6];
    /* 00436efe */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00436f00 */
    f->r[1]=mi_read((0x0070e450u),4u);
    /* 00436f06 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f07 */
    f->r[2]=(0x00000004u+f->r[1]+f->r[7]*1u);
    /* 00436f0b */
    goto mi_00436f37;
mi_00436f0d:;
    /* 00436f0d */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00436f10 */
    if(!(f->flags&MI_ZF))goto mi_00436f43;
    /* 00436f12 */
    f->r[3]=mi_read((0x0070e440u),4u);
    /* 00436f18 */
    f->r[2]=mi_read((0x0070e44cu),4u);
    /* 00436f1e */
    f->r[1]=f->r[6];
    /* 00436f20 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00436f22 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00436f24 */
    f->r[2]=mi_read((0x0070e450u),4u);
    /* 00436f2a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f2b */
    f->r[1]=mi_read((0x0070e45cu),4u);
    /* 00436f31 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00436f33 */
    f->r[2]=(0x00000005u+f->r[2]+f->r[7]*1u);
mi_00436f37:;
    /* 00436f37 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00436f38:;
    /* 00436f38 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f39 */
    f->r[1]=0x00f55bb0u;
    /* 00436f3e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436f43u;f->boundary(0x00436f3eu,0x0053f250u,f);
mi_00436f43:;
    /* 00436f43 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 00436f48 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00436f4a */
    if((f->flags&MI_ZF))goto mi_00436f51;
    /* 00436f4c */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00436f4f */
    if(!(f->flags&MI_ZF))goto mi_00436f61;
mi_00436f51:;
    /* 00436f51 */
    f->r[3]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00436f55 */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00436f5d */
    if((f->flags&MI_ZF))goto mi_00436f81;
    /* 00436f5f */
    goto mi_00436f65;
mi_00436f61:;
    /* 00436f61 */
    f->r[3]=mi_read((0x00000010u+f->r[4]),4u);
mi_00436f65:;
    /* 00436f65 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00436f68 */
    if((f->flags&MI_ZF))goto mi_00436f73;
    /* 00436f6a */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00436f6d */
    if(!(f->flags&MI_ZF))goto mi_004372ab;
mi_00436f73:;
    /* 00436f73 */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 00436f7b */
    if(!(f->flags&MI_ZF))goto mi_004372ab;
mi_00436f81:;
    /* 00436f81 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00436f85 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f86 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f88 */
    f->r[1]=0x006de650u;
    /* 00436f8d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436f92u;f->boundary(0x00436f8du,0x00401f70u,f);
    /* 00436f92 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f93 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436f94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436f99u;f->boundary(0x00436f94u,0x00420a30u,f);
    /* 00436f99 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00436f9c */
    goto mi_004372ab;
mi_00436fa1:;
    /* 00436fa1 */
    f->r[5]=mi_read((0x00f572ecu),4u);
    /* 00436fa7 */
    a=f->r[5];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00436faa */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00436fac */
    if(mi_signed_less(f->flags))goto mi_00436fe6;
    /* 00436fae */
    f->r[5]=mi_read((0x00f572ecu),4u);
    /* 00436fb4 */
    a=f->r[5];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00436fb7 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00436fb9 */
    if(!mi_signed_less(f->flags))goto mi_00436fe6;
    /* 00436fbb */
    f->r[5]=(0xffffff6au+f->r[6]);
    /* 00436fc1 */
    f->r[1]=0x00f55bb0u;
    /* 00436fc6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436fc7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436fc8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436fc9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436fceu;f->boundary(0x00436fc9u,0x0053e590u,f);
    /* 00436fce */
    f->r[1]=mi_read((0x00d3b5c0u),4u);
    /* 00436fd4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436fd5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436fd6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00436fd7 */
    f->r[1]=0x00f55bb0u;
    /* 00436fdc */
    f->r[4]-=4;MI_U32(f->r[4])=0x00436fe1u;f->boundary(0x00436fdcu,0x0053f250u,f);
    /* 00436fe1 */
    goto mi_004372a7;
mi_00436fe6:;
    /* 00436fe6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00436fe8 */
    if(!(f->flags&MI_ZF))goto mi_0043715a;
    /* 00436fee */
    a=mi_read((0x00af598cu+f->r[2]*4u),4u);b=0xeefed77fu;t=mi_alu(f,a,b,32,3);
    /* 00436ff9 */
    if(!(f->flags&MI_ZF))goto mi_00437153;
    /* 00436fff */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00437003 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00437005 */
    if(!(f->flags&MI_ZF))goto mi_00437017;
    /* 00437007 */
    f->r[5]=mi_read((0x0070e0f8u+f->r[3]*4u),4u);
    /* 0043700e */
    f->r[3]=mi_read((0x0070e0fcu+f->r[3]*4u),4u);
    /* 00437015 */
    goto mi_00437025;
mi_00437017:;
    /* 00437017 */
    f->r[5]=mi_read((0x0070e0f4u+f->r[3]*4u),4u);
    /* 0043701e */
    f->r[3]=mi_read((0x0070e0f8u+f->r[3]*4u),4u);
mi_00437025:;
    /* 00437025 */
    a=f->r[5];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 0043702b */
    if(!mi_signed_less(f->flags))goto mi_004370bc;
    /* 00437031 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[5]*2u),2u);
    /* 00437039 */
    f->r[1]=f->r[2];
    /* 0043703b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043703e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00437040 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00437043 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 0043704c */
    if(!(f->flags&MI_ZF))goto mi_0043706f;
    /* 0043704e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a2eu+f->r[1]),1u))&0xffu)<<0);
    /* 00437054 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00437056 */
    if((f->flags&MI_ZF))goto mi_0043706f;
    /* 00437058 */
    f->r[2]=mi_read((0x00799a40u+f->r[1]),4u);
    /* 0043705e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00437060 */
    if((f->flags&MI_ZF))goto mi_0043706f;
    /* 00437062 */
    a=mi_read((0x00799a31u+f->r[1]),1u);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00437069 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_0043724b;
mi_0043706f:;
    /* 0043706f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[5]*2u),2u);
    /* 00437077 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00437079 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004370bc;
mi_0043707b:;
    /* 0043707b */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0043707e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00437081 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00437083 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00437086 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd65du+f->r[1]),1u))&0xffu)<<0);
    /* 0043708c */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043708e */
    if(!(f->flags&MI_ZF))goto mi_004370b1;
    /* 00437090 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00437092 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[1]),1u))&0xffu)<<0);
    /* 00437098 */
    f->r[5]=f->r[2];
    /* 0043709a */
    a=f->r[5];b=0xfffffff3u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0043709d */
    a=f->r[5];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 004370a0 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004370b1;
    /* 004370a2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004370a4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00438010u+f->r[5]),1u))&0xffu)<<0);
    /* 004370aa */
    switch(mi_read((0x00438008u+f->r[2]*4u),4u)){case 0x004370b1u:goto mi_004370b1;case 0x0043724bu:goto mi_0043724b;default:abort();}
mi_004370b1:;
    /* 004370b1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[1]),2u);
    /* 004370b8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004370ba */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0043707b;
mi_004370bc:;
    /* 004370bc */
    a=f->r[3];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 004370c2 */
    if(!mi_signed_less(f->flags))goto mi_00437153;
    /* 004370c8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[3]*2u),2u);
    /* 004370d0 */
    f->r[1]=f->r[2];
    /* 004370d2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004370d5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004370d7 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004370da */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004370e3 */
    if(!(f->flags&MI_ZF))goto mi_00437106;
    /* 004370e5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a2eu+f->r[1]),1u))&0xffu)<<0);
    /* 004370eb */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004370ed */
    if((f->flags&MI_ZF))goto mi_00437106;
    /* 004370ef */
    f->r[2]=mi_read((0x00799a40u+f->r[1]),4u);
    /* 004370f5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004370f7 */
    if((f->flags&MI_ZF))goto mi_00437106;
    /* 004370f9 */
    a=mi_read((0x00799a31u+f->r[1]),1u);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00437100 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_0043724b;
mi_00437106:;
    /* 00437106 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[3]*2u),2u);
    /* 0043710e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00437110 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00437153;
mi_00437112:;
    /* 00437112 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00437115 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00437118 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043711a */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043711d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd65du+f->r[1]),1u))&0xffu)<<0);
    /* 00437123 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00437125 */
    if(!(f->flags&MI_ZF))goto mi_00437148;
    /* 00437127 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00437129 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[1]),1u))&0xffu)<<0);
    /* 0043712f */
    f->r[5]=f->r[2];
    /* 00437131 */
    a=f->r[5];b=0xfffffff3u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00437134 */
    a=f->r[5];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 00437137 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00437148;
    /* 00437139 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0043713b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00438048u+f->r[5]),1u))&0xffu)<<0);
    /* 00437141 */
    switch(mi_read((0x00438040u+f->r[2]*4u),4u)){case 0x00437148u:goto mi_00437148;case 0x0043724bu:goto mi_0043724b;default:abort();}
mi_00437148:;
    /* 00437148 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[1]),2u);
    /* 0043714f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00437151 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00437112;
mi_00437153:;
    /* 00437153 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437154 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437155 */
    goto mi_00437240;
mi_0043715a:;
    /* 0043715a */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043715d */
    if(!(f->flags&MI_ZF))goto mi_0043717a;
    /* 0043715f */
    f->r[5]=mi_read((0x0070e44cu),4u);
    /* 00437165 */
    f->r[2]=mi_read((0x0070e45cu),4u);
    /* 0043716b */
    f->r[1]=f->r[6];
    /* 0043716d */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043716f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437170 */
    f->r[1]=(0x00000001u+f->r[2]+f->r[7]*1u);
    /* 00437174 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437175 */
    goto mi_00437240;
mi_0043717a:;
    /* 0043717a */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043717d */
    if(!(f->flags&MI_ZF))goto mi_004371dc;
    /* 0043717f */
    f->r[3]=mi_read((0x0070e448u),4u);
    /* 00437185 */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 0043718b */
    f->r[2]=f->r[6];
    /* 0043718d */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043718f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437190 */
    f->r[2]=(0x00000002u+f->r[1]+f->r[7]*1u);
    /* 00437194 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437195 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437196 */
    f->r[1]=0x00f55bb0u;
    /* 0043719b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004371a0u;f->boundary(0x0043719bu,0x0053f250u,f);
    /* 004371a0 */
    f->r[3]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004371a4 */
    f->r[1]=0x00bca350u;
    /* 004371a9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004371aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004371afu;f->boundary(0x004371aau,0x00488030u,f);
    /* 004371af */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004371b1 */
    if((f->flags&MI_ZF))goto mi_0043724b;
    /* 004371b7 */
    f->r[5]=mi_read((0x0070e448u),4u);
    /* 004371bd */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 004371c3 */
    f->r[0]=f->r[6];
    /* 004371c5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004371c7 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004371c9 */
    f->r[2]=(0x00000002u+f->r[1]+f->r[7]*1u);
    /* 004371cd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004371ce */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004371cf */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004371d0 */
    f->r[1]=0x00bca350u;
    /* 004371d5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004371dau;f->boundary(0x004371d5u,0x00488c20u,f);
    /* 004371da */
    goto mi_0043724b;
mi_004371dc:;
    /* 004371dc */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004371df */
    if(!(f->flags&MI_ZF))goto mi_004371f9;
    /* 004371e1 */
    f->r[5]=mi_read((0x0070e444u),4u);
    /* 004371e7 */
    f->r[2]=mi_read((0x0070e454u),4u);
    /* 004371ed */
    f->r[1]=f->r[6];
    /* 004371ef */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004371f1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004371f2 */
    f->r[1]=(0x00000003u+f->r[2]+f->r[7]*1u);
    /* 004371f6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004371f7 */
    goto mi_00437240;
mi_004371f9:;
    /* 004371f9 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004371fc */
    if(!(f->flags&MI_ZF))goto mi_00437215;
    /* 004371fe */
    f->r[1]=mi_read((0x0070e440u),4u);
    /* 00437204 */
    f->r[2]=f->r[6];
    /* 00437206 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00437208 */
    f->r[1]=mi_read((0x0070e450u),4u);
    /* 0043720e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043720f */
    f->r[2]=(0x00000004u+f->r[1]+f->r[7]*1u);
    /* 00437213 */
    goto mi_0043723f;
mi_00437215:;
    /* 00437215 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00437218 */
    if(!(f->flags&MI_ZF))goto mi_0043724b;
    /* 0043721a */
    f->r[3]=mi_read((0x0070e440u),4u);
    /* 00437220 */
    f->r[2]=mi_read((0x0070e44cu),4u);
    /* 00437226 */
    f->r[1]=f->r[6];
    /* 00437228 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043722a */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043722c */
    f->r[2]=mi_read((0x0070e450u),4u);
    /* 00437232 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437233 */
    f->r[1]=mi_read((0x0070e45cu),4u);
    /* 00437239 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043723b */
    f->r[2]=(0x00000005u+f->r[2]+f->r[7]*1u);
mi_0043723f:;
    /* 0043723f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00437240:;
    /* 00437240 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437241 */
    f->r[1]=0x00f55bb0u;
    /* 00437246 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043724bu;f->boundary(0x00437246u,0x0053f250u,f);
mi_0043724b:;
    /* 0043724b */
    f->r[3]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0043724f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[3]),1u))&0xffu)<<0);
    /* 00437255 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00437257 */
    if((f->flags&MI_ZF))goto mi_004372a7;
    /* 00437259 */
    a=mi_read((0x00799a5fu+f->r[3]),1u);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00437260 */
    if(!(f->flags&MI_ZF))goto mi_0043727b;
    /* 00437262 */
    f->r[2]=mi_read((0x00f56724u),4u);
    /* 00437268 */
    f->r[0]=(0xffffffe0u+f->r[6]);
    /* 0043726b */
    f->r[1]=(0x00000012u+f->r[7]);
    /* 0043726e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043726f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437270 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437271 */
    f->r[1]=0x00f55bb0u;
    /* 00437276 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043727bu;f->boundary(0x00437276u,0x0053e590u,f);
mi_0043727b:;
    /* 0043727b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[3]),1u))&0xffu)<<0);
    /* 00437281 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00437283 */
    if((f->flags&MI_ZF))goto mi_004372a7;
    /* 00437285 */
    a=mi_read((0x00799a5fu+f->r[3]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0043728c */
    if(!(f->flags&MI_ZF))goto mi_004372a7;
    /* 0043728e */
    f->r[2]=mi_read((0x00f572e4u),4u);
    /* 00437294 */
    f->r[0]=(0xffffffe0u+f->r[6]);
    /* 00437297 */
    f->r[1]=(0x00000012u+f->r[7]);
    /* 0043729a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043729b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043729c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043729d */
    f->r[1]=0x00f55bb0u;
    /* 004372a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004372a7u;f->boundary(0x004372a2u,0x0053e590u,f);
mi_004372a7:;
    /* 004372a7 */
    f->r[3]=mi_read((0x00000010u+f->r[4]),4u);
mi_004372ab:;
    /* 004372ab */
    a=mi_read((0x007999e8u+f->r[3]),2u);b=0x000000d2u;t=mi_alu(f,a,b,16,1);
    /* 004372b4 */
    if(!(f->flags&MI_ZF))goto mi_00437328;
    /* 004372b6 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004372bb */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004372be */
    if(!(f->flags&MI_ZF))goto mi_004372d4;
    /* 004372c0 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[3]),2u))&0xffffu)<<0);
    /* 004372c7 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004372ca */
    if((f->flags&MI_ZF))goto mi_0043731e;
    /* 004372cc */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004372d0 */
    if((f->flags&MI_ZF))goto mi_0043731e;
    /* 004372d2 */
    goto mi_00437328;
mi_004372d4:;
    /* 004372d4 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004372d7 */
    if(!(f->flags&MI_ZF))goto mi_004372ed;
    /* 004372d9 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[3]),2u))&0xffffu)<<0);
    /* 004372e0 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004372e3 */
    if((f->flags&MI_ZF))goto mi_0043731e;
    /* 004372e5 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 004372e9 */
    if((f->flags&MI_ZF))goto mi_0043731e;
    /* 004372eb */
    goto mi_00437328;
mi_004372ed:;
    /* 004372ed */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004372ef */
    if(!(f->flags&MI_ZF))goto mi_00437306;
    /* 004372f1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[3]),2u))&0xffffu)<<0);
    /* 004372f8 */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 004372fc */
    if((f->flags&MI_ZF))goto mi_0043731e;
    /* 004372fe */
    a=(f->r[0]&0xffffu);b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 00437302 */
    if((f->flags&MI_ZF))goto mi_0043731e;
    /* 00437304 */
    goto mi_00437328;
mi_00437306:;
    /* 00437306 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00437309 */
    if(!(f->flags&MI_ZF))goto mi_00437328;
    /* 0043730b */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[3]),2u))&0xffffu)<<0);
    /* 00437312 */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 00437316 */
    if((f->flags&MI_ZF))goto mi_0043731e;
    /* 00437318 */
    a=(f->r[0]&0xffffu);b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0043731c */
    if(!(f->flags&MI_ZF))goto mi_00437328;
mi_0043731e:;
    /* 0043731e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043731f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437320 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437325u;f->boundary(0x00437320u,0x0041fff0u,f);
    /* 00437325 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00437328:;
    /* 00437328 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[3]),2u))&0xffffu)<<0);
    /* 0043732f */
    a=(f->r[0]&0xffffu);b=0x000000cfu;t=mi_alu(f,a,b,16,1);
    /* 00437333 */
    if((f->flags&MI_ZF))goto mi_0043733b;
    /* 00437335 */
    a=(f->r[0]&0xffffu);b=0x000000d0u;t=mi_alu(f,a,b,16,1);
    /* 00437339 */
    if(!(f->flags&MI_ZF))goto mi_0043734c;
mi_0043733b:;
    /* 0043733b */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0043733f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437340 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437341 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437342 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437347u;f->boundary(0x00437342u,0x004e5e90u,f);
    /* 00437347 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043734a */
    goto mi_00437350;
mi_0043734c:;
    /* 0043734c */
    f->r[5]=mi_read((0x00000014u+f->r[4]),4u);
mi_00437350:;
    /* 00437350 */
    a=mi_read((0x007999e8u+f->r[3]),2u);b=0x000000d5u;t=mi_alu(f,a,b,16,1);
    /* 00437359 */
    if(!(f->flags&MI_ZF))goto mi_00437377;
    /* 0043735b */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00437363 */
    if(!(f->flags&MI_ZF))goto mi_00437377;
    /* 00437365 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437366 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437367 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437368 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043736du;f->boundary(0x00437368u,0x00542e10u,f);
    /* 0043736d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043736e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043736f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437374u;f->boundary(0x0043736fu,0x00420050u,f);
    /* 00437374 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00437377:;
    /* 00437377 */
    a=mi_read((0x007999e8u+f->r[3]),2u);b=0x000000e1u;t=mi_alu(f,a,b,16,1);
    /* 00437380 */
    if(!(f->flags&MI_ZF))goto mi_004373d2;
    /* 00437382 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 00437387 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437389 */
    if(!(f->flags&MI_ZF))goto mi_00437397;
    /* 0043738b */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000006u;t=mi_alu(f,a,b,16,1);
    /* 00437393 */
    if((f->flags&MI_ZF))goto mi_004373c8;
    /* 00437395 */
    goto mi_004373d2;
mi_00437397:;
    /* 00437397 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043739a */
    if(!(f->flags&MI_ZF))goto mi_004373a8;
    /* 0043739c */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 004373a4 */
    if((f->flags&MI_ZF))goto mi_004373c8;
    /* 004373a6 */
    goto mi_004373d2;
mi_004373a8:;
    /* 004373a8 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004373ab */
    if(!(f->flags&MI_ZF))goto mi_004373b9;
    /* 004373ad */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000007u;t=mi_alu(f,a,b,16,1);
    /* 004373b5 */
    if((f->flags&MI_ZF))goto mi_004373c8;
    /* 004373b7 */
    goto mi_004373d2;
mi_004373b9:;
    /* 004373b9 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004373bc */
    if(!(f->flags&MI_ZF))goto mi_004373d2;
    /* 004373be */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000008u;t=mi_alu(f,a,b,16,1);
    /* 004373c6 */
    if(!(f->flags&MI_ZF))goto mi_004373d2;
mi_004373c8:;
    /* 004373c8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004373c9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004373ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004373cfu;f->boundary(0x004373cau,0x004206f0u,f);
    /* 004373cf */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004373d2:;
    /* 004373d2 */
    a=mi_read((0x007999e8u+f->r[3]),2u);b=0x000000e3u;t=mi_alu(f,a,b,16,1);
    /* 004373db */
    if(!(f->flags&MI_ZF))goto mi_004373f1;
    /* 004373dd */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004373e5 */
    if(!(f->flags&MI_ZF))goto mi_004373f1;
    /* 004373e7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004373e8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004373e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004373eeu;f->boundary(0x004373e9u,0x00420d60u,f);
    /* 004373ee */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004373f1:;
    /* 004373f1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[3]),2u))&0xffffu)<<0);
    /* 004373f8 */
    a=(f->r[0]&0xffffu);b=0x000000e5u;t=mi_alu(f,a,b,16,1);
    /* 004373fc */
    if((f->flags&MI_ZF))goto mi_00437410;
    /* 004373fe */
    a=(f->r[0]&0xffffu);b=0x000000eau;t=mi_alu(f,a,b,16,1);
    /* 00437402 */
    if((f->flags&MI_ZF))goto mi_00437410;
    /* 00437404 */
    a=(f->r[0]&0xffffu);b=0x000000ecu;t=mi_alu(f,a,b,16,1);
    /* 00437408 */
    if((f->flags&MI_ZF))goto mi_00437410;
    /* 0043740a */
    a=(f->r[0]&0xffffu);b=0x000000ebu;t=mi_alu(f,a,b,16,1);
    /* 0043740e */
    if(!(f->flags&MI_ZF))goto mi_0043743d;
mi_00437410:;
    /* 00437410 */
    f->r[0]=mi_read((0x00799a3cu+f->r[3]),4u);
    /* 00437416 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437418 */
    if((f->flags&MI_ZF))goto mi_0043743d;
    /* 0043741a */
    f->r[0]=mi_read((0x00799a34u+f->r[3]),4u);
    /* 00437420 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00437423 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00437426 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00437428 */
    a=mi_read((0x00bcd609u+f->r[1]*4u),1u);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 00437430 */
    if(!(f->flags&MI_ZF))goto mi_0043743d;
    /* 00437432 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437433 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437434 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437435 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043743au;f->boundary(0x00437435u,0x00421920u,f);
    /* 0043743a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043743d:;
    /* 0043743d */
    a=mi_read((0x007999e8u+f->r[3]),2u);b=0x000000e4u;t=mi_alu(f,a,b,16,1);
    /* 00437446 */
    if(!(f->flags&MI_ZF))goto mi_0043745c;
    /* 00437448 */
    a=mi_read((0x007999eau+f->r[3]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00437450 */
    if(!(f->flags&MI_ZF))goto mi_0043745c;
    /* 00437452 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437453 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437454 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437459u;f->boundary(0x00437454u,0x00421060u,f);
    /* 00437459 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043745c:;
    /* 0043745c */
    f->r[1]=mi_read((0x0070e47cu),4u);
    /* 00437462 */
    f->r[5]=mi_read((0x0070e458u),4u);
    /* 00437468 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
mi_00437471:;
    /* 00437471 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00437475 */
    f->r[2]=mi_read((0x0070e480u),4u);
    /* 0043747b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043747c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0043747d */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[2]);
    /* 00437483 */
    f->r[2]=mi_read((0x0070e464u),4u);
    /* 00437489 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043748c */
    f->r[7]=(0x00000002u+f->r[7]+f->r[5]*1u);
    /* 00437490 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00437492 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00437496 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00436c1c;
    /* 0043749c */
    f->r[0]=mi_read((0x0070e490u),4u);
mi_004374a1:;
    /* 004374a1 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004374a5 */
    f->r[7]=f->r[0];
    /* 004374a7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004374a9 */
    if((f->flags&MI_ZF))goto mi_004374b2;
    /* 004374ab */
    a=f->r[7];b=mi_read((0x0070e45cu),4u);t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004374b1 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_004374b2:;
    /* 004374b2 */
    f->r[2]=mi_read((0x0070e478u),4u);
    /* 004374b8 */
    f->r[0]=0xfffffffeu;
    /* 004374bd */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004374bf */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004374c1 */
    a=f->r[2];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004374c4 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 004374c7 */
    f->r[0]=mi_read((0x0070e464u),4u);
    /* 004374cc */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004374cf */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[2]);
    /* 004374d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004374d7 */
    if(mi_signed_less(f->flags))goto mi_0043758d;
mi_004374dd:;
    /* 004374dd */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004374df */
    if(mi_signed_less(f->flags))goto mi_0043756c;
    /* 004374e5 */
    a=f->r[2];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 004374eb */
    if(!mi_signed_less(f->flags))goto mi_0043756c;
    /* 004374ed */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004374f0 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004374f3 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004374f6 */
    f->r[5]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 004374f9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004374fb */
    f->r[0]=mi_read((0x0070e494u+f->r[0]*4u),4u);
    /* 00437502 */
    a=f->r[0];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00437507 */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[0]);
    /* 0043750c */
    if(!mi_signed_less(f->flags))goto mi_0043756c;
    /* 0043750e */
    a=mi_read((0x00af598cu+f->r[0]*4u),4u);b=0xeefed77fu;t=mi_alu(f,a,b,32,3);
    /* 00437519 */
    if((f->flags&MI_ZF))goto mi_0043756c;
    /* 0043751b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[0]*2u),2u);
    /* 00437523 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437525 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043756c;
mi_00437527:;
    /* 00437527 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0043752a */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0043752d */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0043752f */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00437532 */
    a=mi_read((0x00bcd65du+f->r[5]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00437539 */
    if(!(f->flags&MI_ZF))goto mi_00437561;
    /* 0043753b */
    a=mi_read((0x00bcd718u+f->r[5]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00437543 */
    if(!(f->flags&MI_ZF))goto mi_00437561;
    /* 00437545 */
    f->r[1]=mi_read((0x00000074u+f->r[4]),4u);
    /* 00437549 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043754a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043754b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043754c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043754d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437552u;f->boundary(0x0043754du,0x00438e50u,f);
    /* 00437552 */
    f->r[2]=mi_read((0x0070e480u),4u);
    /* 00437558 */
    f->r[1]=mi_read((0x0070e47cu),4u);
    /* 0043755e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00437561:;
    /* 00437561 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[5]),2u);
    /* 00437568 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043756a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00437527;
mi_0043756c:;
    /* 0043756c */
    f->r[0]=mi_read((0x0070e458u),4u);
    /* 00437571 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00437572 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00437573 */
    f->r[7]=(0x00000002u+f->r[7]+f->r[0]*1u);
    /* 00437577 */
    f->r[0]=mi_read((0x0070e464u),4u);
    /* 0043757c */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043757f */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[2]);
    /* 00437585 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00437587 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004374dd;
mi_0043758d:;
    /* 0043758d */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00437591 */
    f->r[5]=mi_read((0x0070e490u),4u);
    /* 00437597 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437599 */
    if((f->flags&MI_ZF))goto mi_004375a2;
    /* 0043759b */
    a=f->r[5];b=mi_read((0x0070e45cu),4u);t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004375a1 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
mi_004375a2:;
    /* 004375a2 */
    f->r[2]=mi_read((0x0070e458u),4u);
    /* 004375a8 */
    f->r[0]=mi_read((0x0070e464u),4u);
    /* 004375ad */
    f->r[1]=0xfffffffeu;
    /* 004375b2 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004375b5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004375b7 */
    f->r[2]=mi_read((0x0070e478u),4u);
    /* 004375bd */
    a=f->r[2];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004375c0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004375c8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004375ca */
    f->r[5]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 004375ce */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[2]);
    /* 004375d4 */
    if(mi_signed_less(f->flags))goto mi_00437e40;
mi_004375da:;
    /* 004375da */
    f->r[0]=mi_read((0x0070e480u),4u);
    /* 004375df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004375e1 */
    if(mi_signed_less(f->flags))goto mi_00437e0f;
    /* 004375e7 */
    a=f->r[0];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 004375ec */
    if(!mi_signed_less(f->flags))goto mi_00437e0f;
    /* 004375f2 */
    f->r[0]=mi_read((0x0070e47cu),4u);
    /* 004375f7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004375fa */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004375fd */
    f->r[1]=mi_read((0x0070e480u),4u);
    /* 00437603 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00437606 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00437609 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043760b */
    f->r[1]=mi_read((0x0070e494u+f->r[0]*4u),4u);
    /* 00437612 */
    a=f->r[1];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00437618 */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[1]);
    /* 0043761e */
    if(!mi_signed_less(f->flags))goto mi_00437e0f;
    /* 00437624 */
    f->r[0]=mi_read((0x00af598cu+f->r[1]*4u),4u);
    /* 0043762b */
    a=f->r[0];b=0x00000044u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043762e */
    a=(f->r[0]&0xffu);b=0x00000044u;t=mi_alu(f,a,b,8,1);
    /* 00437630 */
    if(!(f->flags&MI_ZF))goto mi_00437667;
    /* 00437632 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00adc36cu+f->r[1]),1u))&0xffu)<<0);
    /* 00437638 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043763a */
    if((f->flags&MI_ZF))goto mi_00437667;
    /* 0043763c */
    f->r[2]=mi_read((0x0070e448u),4u);
    /* 00437642 */
    f->r[1]=f->r[6];
    /* 00437644 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00437646 */
    f->r[2]=mi_read((0x00f56d34u),4u);
    /* 0043764c */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00437651 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437652 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437653 */
    f->r[1]=0x00f55bb0u;
    /* 00437658 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[2]*1u);
    /* 0043765c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043765d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437662u;f->boundary(0x0043765du,0x0053e590u,f);
    /* 00437662 */
    goto mi_00437e0f;
mi_00437667:;
    /* 00437667 */
    f->r[7]=mi_read((0x00b7482cu+f->r[1]*4u),4u);
    /* 0043766e */
    f->r[2]=mi_read((0x0126517cu),4u);
    /* 00437674 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00437676 */
    f->r[0]=f->r[7];
    /* 00437678 */
    if(mi_signed_less(f->flags))goto mi_0043767c;
    /* 0043767a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043767c:;
    /* 0043767c */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043767f */
    a=mi_read((0x010d6a16u+f->r[0]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00437687 */
    if((f->flags&MI_ZF))goto mi_00437cf3;
    /* 0043768d */
    a=mi_read((0x00b67d1cu+f->r[1]),1u);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00437694 */
    if((f->flags&MI_ZF))goto mi_00437e0f;
    /* 0043769a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[1]*2u),2u);
    /* 004376a2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004376a4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004376a8 */
    if((f->flags&MI_ZF))goto mi_004376c5;
    /* 004376aa */
    f->r[2]=f->r[0];
    /* 004376ac */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004376af */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004376b1 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a97u+f->r[2]*8u),1u))&0xffu)<<0);
    /* 004376b8 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004376ba */
    if((f->flags&MI_ZF))goto mi_004376c5;
    /* 004376bc */
    MI_U16((0x0070e120u))=(uint16_t)(0x0000f863u);
mi_004376c5:;
    /* 004376c5 */
    f->r[3]=f->r[0];
    /* 004376c7 */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004376ca */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004376cc */
    a=f->r[3];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004376cf */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004376d3 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[3]),2u))&0xffffu)<<0);
    /* 004376da */
    a=(f->r[2]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 004376de */
    if((f->flags&MI_ZF))goto mi_004376e7;
    /* 004376e0 */
    a=(f->r[2]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 004376e5 */
    if(!(f->flags&MI_ZF))goto mi_004376f9;
mi_004376e7:;
    /* 004376e7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004376e8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004376e9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004376ea */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004376eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004376f0u;f->boundary(0x004376ebu,0x00442280u,f);
    /* 004376f0 */
    f->r[1]=mi_read((0x0070e488u),4u);
    /* 004376f6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004376f9:;
    /* 004376f9 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[3]),2u))&0xffffu)<<0);
    /* 00437700 */
    a=(f->r[3]&0xffffu);b=0x0000004bu;t=mi_alu(f,a,b,16,1);
    /* 00437704 */
    if(!(f->flags&MI_ZF))goto mi_0043770d;
    /* 00437706 */
    f->r[7]=mi_read((0x00b7482cu+f->r[1]*4u),4u);
mi_0043770d:;
    /* 0043770d */
    a=(f->r[3]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 00437711 */
    if((f->flags&MI_ZF))goto mi_0043771a;
    /* 00437713 */
    a=(f->r[3]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 00437718 */
    if(!(f->flags&MI_ZF))goto mi_0043773c;
mi_0043771a:;
    /* 0043771a */
    f->r[2]=mi_read((0x00f566acu),4u);
    /* 00437720 */
    f->r[0]=(0xffffffd6u+f->r[6]);
    /* 00437723 */
    f->r[1]=(0xfffffffcu+f->r[5]);
    /* 00437726 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437727 */
    a=f->r[2];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043772a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043772b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043772c */
    f->r[1]=0x00f55bb0u;
    /* 00437731 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437736u;f->boundary(0x00437731u,0x0053e590u,f);
    /* 00437736 */
    f->r[1]=mi_read((0x0070e488u),4u);
mi_0043773c:;
    /* 0043773c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043773d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043773e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437743u;f->boundary(0x0043773eu,0x0040f2c0u,f);
    /* 00437743 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437746 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437748 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00437ce5;
    /* 0043774e */
    f->r[2]=mi_read((0x0126517cu),4u);
    /* 00437754 */
    f->r[3]=f->r[0];
    /* 00437756 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00437758 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0043775c */
    f->r[1]=f->r[7];
    /* 0043775e */
    if(mi_signed_less(f->flags))goto mi_00437762;
    /* 00437760 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00437762:;
    /* 00437762 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00437765 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a16u+f->r[1]),2u);
    /* 0043776c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043776e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00437786;
    /* 00437770 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00437772 */
    f->r[0]=f->r[7];
    /* 00437774 */
    if(mi_signed_less(f->flags))goto mi_00437778;
    /* 00437776 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00437778:;
    /* 00437778 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043777b */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a16u+f->r[0]),2u);
    /* 00437782 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[3]);
mi_00437786:;
    /* 00437786 */
    f->r[2]=mi_read((0x0070e488u),4u);
    /* 0043778c */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00437790 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437791 */
    f->r[1]=0x00bca350u;
    /* 00437796 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00b285ccu+f->r[2]),1u))&0xffu)<<0);
    /* 0043779c */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043779f */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004377a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004377a8u;f->boundary(0x004377a3u,0x00488050u,f);
    /* 004377a8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004377aa */
    if((f->flags&MI_ZF))goto mi_004377cc;
    /* 004377ac */
    f->r[2]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004377b0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377b1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377b2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377b3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377b4 */
    f->r[1]=0x00bca350u;
    /* 004377b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004377beu;f->boundary(0x004377b9u,0x00489500u,f);
    /* 004377be */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 004377c7 */
    goto mi_00437ded;
mi_004377cc:;
    /* 004377cc */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004377d0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[0]),2u);
    /* 004377d7 */
    a=f->r[0];b=0xffffffe2u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004377da */
    a=f->r[0];b=0x000000bfu;t=mi_alu(f,a,b,32,1);
    /* 004377df */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00437b4d;
    /* 004377e5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004377e7 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004380b0u+f->r[0]),1u))&0xffu)<<0);
    /* 004377ed */
    switch(mi_read((0x00438078u+f->r[1]*4u),4u)){case 0x004377f4u:goto mi_004377f4;case 0x00437824u:goto mi_00437824;case 0x00437866u:goto mi_00437866;case 0x0043792fu:goto mi_0043792f;case 0x00437a49u:goto mi_00437a49;case 0x00437a71u:goto mi_00437a71;case 0x00437a97u:goto mi_00437a97;case 0x00437ab1u:goto mi_00437ab1;case 0x00437acbu:goto mi_00437acb;case 0x00437ae5u:goto mi_00437ae5;case 0x00437affu:goto mi_00437aff;case 0x00437b19u:goto mi_00437b19;case 0x00437b33u:goto mi_00437b33;case 0x00437b4du:goto mi_00437b4d;default:abort();}
mi_004377f4:;
    /* 004377f4 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377f6 */
    a=0x00000071u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377f8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377f9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377fa */
    a=0x00000270u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004377ff */
    f->r[1]=0x00f55bb0u;
    /* 00437804 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437809u;f->boundary(0x00437804u,0x004381a0u,f);
    /* 00437809 */
    f->r[2]=(0xffffffffu+f->r[0]+f->r[3]*1u);
    /* 0043780d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043780e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437813u;f->boundary(0x0043780eu,0x004381b0u,f);
    /* 00437813 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437816 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 0043781f */
    goto mi_00437ded;
mi_00437824:;
    /* 00437824 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437826 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437828 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437829 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043782a */
    a=0x00000274u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043782f */
    f->r[1]=0x00f55bb0u;
    /* 00437834 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437839u;f->boundary(0x00437834u,0x004381a0u,f);
    /* 00437839 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[3]*1u);
    /* 0043783d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043783e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437843u;f->boundary(0x0043783eu,0x004381b0u,f);
    /* 00437843 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437846 */
    f->r[1]=0x00f55bb0u;
    /* 0043784b */
    a=0x0000015fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437850 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437855u;f->boundary(0x00437850u,0x004381a0u,f);
    /* 00437855 */
    f->r[1]=(0xffffffd6u+f->r[6]);
    /* 00437858 */
    f->r[2]=(0xfffffffcu+f->r[5]);
    /* 0043785b */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043785e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043785f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437860 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437861 */
    goto mi_00437cdb;
mi_00437866:;
    /* 00437866 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00437868 */
    a=0x00000223u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043786d */
    f->r[1]=0x00f55bb0u;
    /* 00437872 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00437876 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043787a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000242u);
    /* 00437882 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437887u;f->boundary(0x00437882u,0x004381a0u,f);
    /* 00437887 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00437889 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0043788c */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043788f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004378e8;
    /* 00437891 */
    switch(mi_read((0x00438170u+f->r[7]*4u),4u)){case 0x00437898u:goto mi_00437898;case 0x004378acu:goto mi_004378ac;case 0x004378c0u:goto mi_004378c0;case 0x004378d4u:goto mi_004378d4;default:abort();}
mi_00437898:;
    /* 00437898 */
    f->r[0]=0x00000242u;
    /* 0043789d */
    f->r[7]=0x00000004u;
    /* 004378a2 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004378aa */
    goto mi_004378f0;
mi_004378ac:;
    /* 004378ac */
    f->r[0]=0x00000243u;
    /* 004378b1 */
    f->r[7]=0x00000003u;
    /* 004378b6 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0xfffffffeu);
    /* 004378be */
    goto mi_004378f0;
mi_004378c0:;
    /* 004378c0 */
    f->r[0]=0x00000244u;
    /* 004378c5 */
    f->r[7]=0x0000000cu;
    /* 004378ca */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004378d2 */
    goto mi_004378f0;
mi_004378d4:;
    /* 004378d4 */
    f->r[0]=0x00000245u;
    /* 004378d9 */
    f->r[7]=0x0000000fu;
    /* 004378de */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000006u);
    /* 004378e6 */
    goto mi_004378f0;
mi_004378e8:;
    /* 004378e8 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 004378ec */
    f->r[7]=mi_read((0x00000010u+f->r[4]),4u);
mi_004378f0:;
    /* 004378f0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004378f1 */
    f->r[1]=0x00f55bb0u;
    /* 004378f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004378fbu;f->boundary(0x004378f6u,0x004381a0u,f);
    /* 004378fb */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004378ff */
    f->r[2]=mi_read((0x0070e45cu),4u);
    /* 00437905 */
    f->r[3]=(0xffffffffu+f->r[0]+f->r[3]*1u);
    /* 00437909 */
    f->r[0]=mi_read((0x0070e44cu),4u);
    /* 0043790e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00437910 */
    f->r[0]=(0xfffffff8u+f->r[7]+f->r[2]*2u);
    /* 00437914 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437915 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437916 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437917 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437918 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437919 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043791eu;f->boundary(0x00437919u,0x004381b0u,f);
    /* 0043791e */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437921 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 0043792a */
    goto mi_00437ded;
mi_0043792f:;
    /* 0043792f */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00437933 */
    a=0x00000222u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437938 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0043793c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8cu+f->r[1]),2u);
    /* 00437943 */
    f->r[1]=0x00f55bb0u;
    /* 00437948 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043794c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437951u;f->boundary(0x0043794cu,0x004381a0u,f);
    /* 00437951 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00437953 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00437957 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0043795a */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x00000084u);
    /* 00437962 */
    f->r[0]=f->r[7];
    /* 00437964 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0xfffffff9u);
    /* 0043796c */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(0x00000073u);
    /* 00437974 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000019u);
    /* 0043797c */
    f->r[7]=(0xffffffffu+f->r[0]);
    /* 0043797f */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(0x0000003cu);
    /* 00437987 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0043798a */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(0x00000014u);
    /* 00437992 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00437994 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(0x00000031u);
    /* 0043799c */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(0xfffffffcu);
    /* 004379a4 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(0x00000098u);
    /* 004379ac */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(0xfffffff3u);
    /* 004379b4 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(0x0000007bu);
    /* 004379bc */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(0x00000010u);
    /* 004379c4 */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(0x00000041u);
    /* 004379cc */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(0x0000000eu);
    /* 004379d4 */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(0x00000026u);
    /* 004379dc */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(0xfffffff4u);
    /* 004379e4 */
    f->r[1]=0x00000278u;
    /* 004379e9 */
    if((f->flags&MI_ZF))goto mi_00437a11;
    /* 004379eb */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004379ee */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004379f1 */
    a=f->r[2];b=mi_read((0x00000020u+f->r[4]),4u);t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004379f5 */
    a=mi_read((0x00bcd630u+f->r[2]*4u),1u);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004379fd */
    if(!(f->flags&MI_ZF))goto mi_00437a11;
    /* 004379ff */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437a02 */
    a=f->r[3];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 00437a05 */
    f->r[1]=0x00000279u;
    /* 00437a0a */
    f->r[3]=0x00000014u;
    /* 00437a0f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00437a15;
mi_00437a11:;
    /* 00437a11 */
    f->r[3]=mi_read((0x00000014u+f->r[4]),4u);
mi_00437a15:;
    /* 00437a15 */
    f->r[2]=mi_read((0x00000034u+f->r[4]+f->r[0]*8u),4u);
    /* 00437a19 */
    f->r[0]=mi_read((0x00000030u+f->r[4]+f->r[0]*8u),4u);
    /* 00437a1d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a1e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a1f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a20 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a21 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a22 */
    f->r[1]=0x00f55bb0u;
    /* 00437a27 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437a2cu;f->boundary(0x00437a27u,0x004381a0u,f);
    /* 00437a2c */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437a2e */
    f->r[1]=(0xfffffffcu+f->r[0]+f->r[3]*4u);
    /* 00437a32 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a33 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437a38u;f->boundary(0x00437a33u,0x004381b0u,f);
    /* 00437a38 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437a3b */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437a44 */
    goto mi_00437ded;
mi_00437a49:;
    /* 00437a49 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00437a4d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00437a4f */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00799a8cu+f->r[0]),2u))&0xffffu)<<0);
    /* 00437a56 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a57 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a58 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a59 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a5a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437a60u;f->boundary(0x00437a5bu,0x00438210u,f);
    /* 00437a60 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437a63 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437a6c */
    goto mi_00437ded;
mi_00437a71:;
    /* 00437a71 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 00437a75 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8cu+f->r[1]),2u);
    /* 00437a7c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a7d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a7e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a7f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a80 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437a86u;f->boundary(0x00437a81u,0x00438350u,f);
    /* 00437a86 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437a89 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437a92 */
    goto mi_00437ded;
mi_00437a97:;
    /* 00437a97 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a98 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a99 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a9a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437a9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437aa0u;f->boundary(0x00437a9bu,0x00438470u,f);
    /* 00437aa0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437aa3 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437aac */
    goto mi_00437ded;
mi_00437ab1:;
    /* 00437ab1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ab2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ab3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ab4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ab5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437abau;f->boundary(0x00437ab5u,0x004384d0u,f);
    /* 00437aba */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437abd */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437ac6 */
    goto mi_00437ded;
mi_00437acb:;
    /* 00437acb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437acc */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437acd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ace */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437acf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437ad4u;f->boundary(0x00437acfu,0x00438590u,f);
    /* 00437ad4 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437ad7 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437ae0 */
    goto mi_00437ded;
mi_00437ae5:;
    /* 00437ae5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ae6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ae7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ae8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437ae9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437aeeu;f->boundary(0x00437ae9u,0x00438650u,f);
    /* 00437aee */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437af1 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437afa */
    goto mi_00437ded;
mi_00437aff:;
    /* 00437aff */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b00 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b01 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b02 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437b08u;f->boundary(0x00437b03u,0x004386b0u,f);
    /* 00437b08 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437b0b */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437b14 */
    goto mi_00437ded;
mi_00437b19:;
    /* 00437b19 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b1a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b1b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b1c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b1d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437b22u;f->boundary(0x00437b1du,0x00438710u,f);
    /* 00437b22 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437b25 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437b2e */
    goto mi_00437ded;
mi_00437b33:;
    /* 00437b33 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b34 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b35 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b36 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b37 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437b3cu;f->boundary(0x00437b37u,0x00438770u,f);
    /* 00437b3c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437b3f */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437b48 */
    goto mi_00437ded;
mi_00437b4d:;
    /* 00437b4d */
    f->r[0]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 00437b51 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437b53 */
    if(!(f->flags&MI_ZF))goto mi_00437b7e;
    /* 00437b55 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b56 */
    f->r[1]=0x00f55bb0u;
    /* 00437b5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437b60u;f->boundary(0x00437b5bu,0x00438180u,f);
    /* 00437b60 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 00437b64 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b65 */
    f->r[1]=0x00f55bb0u;
    /* 00437b6a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437b6fu;f->boundary(0x00437b6au,0x00438180u,f);
    /* 00437b6f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000016u+f->r[0]),2u);
    /* 00437b73 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437b75 */
    f->r[3]=(0x0000001eu+f->r[3]+f->r[6]*1u);
    /* 00437b79 */
    goto mi_00437c19;
mi_00437b7e:;
    /* 00437b7e */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00437b81 */
    if(!(f->flags&MI_ZF))goto mi_00437bb4;
    /* 00437b83 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b84 */
    f->r[1]=0x00f55bb0u;
    /* 00437b89 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437b8eu;f->boundary(0x00437b89u,0x00438180u,f);
    /* 00437b8e */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 00437b92 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437b93 */
    f->r[1]=0x00f55bb0u;
    /* 00437b98 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437b9du;f->boundary(0x00437b98u,0x00438180u,f);
    /* 00437b9d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000016u+f->r[0]),2u);
    /* 00437ba1 */
    f->r[1]=mi_read((0x0070e44cu),4u);
    /* 00437ba7 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437ba9 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437bab */
    f->r[3]=(0x0000003cu+f->r[3]+f->r[6]*1u);
    /* 00437baf */
    goto mi_00437c6c;
mi_00437bb4:;
    /* 00437bb4 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00437bb7 */
    if(!(f->flags&MI_ZF))goto mi_00437be9;
    /* 00437bb9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437bba */
    f->r[1]=0x00f55bb0u;
    /* 00437bbf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437bc4u;f->boundary(0x00437bbfu,0x00438180u,f);
    /* 00437bc4 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 00437bc8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437bc9 */
    f->r[1]=0x00f55bb0u;
    /* 00437bce */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437bd3u;f->boundary(0x00437bceu,0x00438180u,f);
    /* 00437bd3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000016u+f->r[0]),2u);
    /* 00437bd7 */
    f->r[0]=mi_read((0x0070e448u),4u);
    /* 00437bdc */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437bde */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437be0 */
    f->r[3]=(0x0000005au+f->r[3]+f->r[6]*1u);
    /* 00437be4 */
    goto mi_00437cc1;
mi_00437be9:;
    /* 00437be9 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00437bec */
    if(!(f->flags&MI_ZF))goto mi_00437c38;
    /* 00437bee */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437bef */
    f->r[1]=0x00f55bb0u;
    /* 00437bf4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437bf9u;f->boundary(0x00437bf4u,0x00438180u,f);
    /* 00437bf9 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 00437bfd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437bfe */
    f->r[1]=0x00f55bb0u;
    /* 00437c03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437c08u;f->boundary(0x00437c03u,0x00438180u,f);
    /* 00437c08 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000016u+f->r[0]),2u);
    /* 00437c0c */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437c0e */
    f->r[0]=mi_read((0x0070e444u),4u);
    /* 00437c13 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437c15 */
    f->r[3]=(0x00000078u+f->r[3]+f->r[6]*1u);
mi_00437c19:;
    /* 00437c19 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c1a */
    f->r[1]=0x00f55bb0u;
    /* 00437c1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437c24u;f->boundary(0x00437c1fu,0x00438180u,f);
    /* 00437c24 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000022u+f->r[0]),2u);
    /* 00437c28 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00437c2c */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437c2e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c2f */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00437c31 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c32 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c33 */
    goto mi_00437cdb;
mi_00437c38:;
    /* 00437c38 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00437c3b */
    if(!(f->flags&MI_ZF))goto mi_00437c88;
    /* 00437c3d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c3e */
    f->r[1]=0x00f55bb0u;
    /* 00437c43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437c48u;f->boundary(0x00437c43u,0x00438180u,f);
    /* 00437c48 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 00437c4c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c4d */
    f->r[1]=0x00f55bb0u;
    /* 00437c52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437c57u;f->boundary(0x00437c52u,0x00438180u,f);
    /* 00437c57 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000016u+f->r[0]),2u);
    /* 00437c5b */
    f->r[1]=mi_read((0x0070e440u),4u);
    /* 00437c61 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437c63 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437c65 */
    f->r[3]=(0x00000096u+f->r[3]+f->r[6]*1u);
mi_00437c6c:;
    /* 00437c6c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c6d */
    f->r[1]=0x00f55bb0u;
    /* 00437c72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437c77u;f->boundary(0x00437c72u,0x00438180u,f);
    /* 00437c77 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000022u+f->r[0]),2u);
    /* 00437c7b */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437c7d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c7e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c7f */
    f->r[0]=mi_read((0x00000028u+f->r[4]),4u);
    /* 00437c83 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437c85 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c86 */
    goto mi_00437cdb;
mi_00437c88:;
    /* 00437c88 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00437c8b */
    if(!(f->flags&MI_ZF))goto mi_00437ce5;
    /* 00437c8d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c8e */
    f->r[1]=0x00f55bb0u;
    /* 00437c93 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437c98u;f->boundary(0x00437c93u,0x00438180u,f);
    /* 00437c98 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 00437c9c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437c9d */
    f->r[1]=0x00f55bb0u;
    /* 00437ca2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437ca7u;f->boundary(0x00437ca2u,0x00438180u,f);
    /* 00437ca7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000016u+f->r[0]),2u);
    /* 00437cab */
    f->r[0]=mi_read((0x0070e44cu),4u);
    /* 00437cb0 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437cb2 */
    a=f->r[3];b=mi_read((0x0070e440u),4u);t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437cb8 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00437cba */
    f->r[3]=(0x000000b4u+f->r[3]+f->r[6]*1u);
mi_00437cc1:;
    /* 00437cc1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437cc2 */
    f->r[1]=0x00f55bb0u;
    /* 00437cc7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437cccu;f->boundary(0x00437cc7u,0x00438180u,f);
    /* 00437ccc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000022u+f->r[0]),2u);
    /* 00437cd0 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00437cd4 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437cd6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437cd7 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00437cd9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437cda */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00437cdb:;
    /* 00437cdb */
    f->r[1]=0x00f55bb0u;
    /* 00437ce0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437ce5u;f->boundary(0x00437ce0u,0x0053e590u,f);
mi_00437ce5:;
    /* 00437ce5 */
    MI_U16((0x0070e120u))=(uint16_t)(0x00000000u);
    /* 00437cee */
    goto mi_00437ded;
mi_00437cf3:;
    /* 00437cf3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[1]*2u),2u);
    /* 00437cfb */
    f->r[2]=f->r[0];
    /* 00437cfd */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00437cff */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00437d02 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00b67d1cu+f->r[1]),1u))&0xffu)<<0);
    /* 00437d08 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00437d0a */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00437d0d */
    f->r[7]=(f->r[7]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[2]),2u))&0xffffu)<<0);
    /* 00437d14 */
    a=(f->r[7]&0xffffu);b=0x00000039u;t=mi_alu(f,a,b,16,1);
    /* 00437d18 */
    if(!(f->flags&MI_ZF))goto mi_00437d99;
    /* 00437d1a */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00437d1d */
    if((f->flags&MI_ZF))goto mi_00437df3;
    /* 00437d23 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999eau+f->r[2]),2u))&0xffffu)<<0);
    /* 00437d2a */
    a=(f->r[2]&0xffffu);b=0x0000000du;t=mi_alu(f,a,b,16,1);
    /* 00437d2e */
    if(!(f->flags&MI_ZF))goto mi_00437d51;
    /* 00437d30 */
    f->r[2]=mi_read((0x00f5662cu),4u);
    /* 00437d36 */
    f->r[0]=(0xffffffecu+f->r[6]);
    /* 00437d39 */
    f->r[1]=(0x0000005du+f->r[5]);
    /* 00437d3c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d3d */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00437d40 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d41 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d42 */
    f->r[1]=0x00f55bb0u;
    /* 00437d47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437d4cu;f->boundary(0x00437d47u,0x0053e590u,f);
    /* 00437d4c */
    goto mi_00437ded;
mi_00437d51:;
    /* 00437d51 */
    a=(f->r[2]&0xffffu);b=0x0000000cu;t=mi_alu(f,a,b,16,1);
    /* 00437d55 */
    if(!(f->flags&MI_ZF))goto mi_00437d75;
    /* 00437d57 */
    f->r[2]=mi_read((0x00f5662cu),4u);
    /* 00437d5d */
    f->r[0]=(0xffffffecu+f->r[6]);
    /* 00437d60 */
    f->r[1]=(0x0000005du+f->r[5]);
    /* 00437d63 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d64 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00437d67 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d68 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d69 */
    f->r[1]=0x00f55bb0u;
    /* 00437d6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437d73u;f->boundary(0x00437d6eu,0x0053e590u,f);
    /* 00437d73 */
    goto mi_00437ded;
mi_00437d75:;
    /* 00437d75 */
    a=(f->r[2]&0xffffu);b=0x0000000bu;t=mi_alu(f,a,b,16,1);
    /* 00437d79 */
    if(!(f->flags&MI_ZF))goto mi_00437df3;
    /* 00437d7b */
    f->r[2]=mi_read((0x00f5662cu),4u);
    /* 00437d81 */
    f->r[0]=(0xffffffecu+f->r[6]);
    /* 00437d84 */
    f->r[1]=(0x0000005du+f->r[5]);
    /* 00437d87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d88 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00437d8b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d8c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437d8d */
    f->r[1]=0x00f55bb0u;
    /* 00437d92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437d97u;f->boundary(0x00437d92u,0x0053e590u,f);
    /* 00437d97 */
    goto mi_00437ded;
mi_00437d99:;
    /* 00437d99 */
    a=(f->r[7]&0xffffu);b=0x0000003au;t=mi_alu(f,a,b,16,1);
    /* 00437d9d */
    if((f->flags&MI_ZF))goto mi_00437df3;
    /* 00437d9f */
    a=(f->r[7]&0xffffu);b=0x000000aau;t=mi_alu(f,a,b,16,1);
    /* 00437da4 */
    if((f->flags&MI_ZF))goto mi_00437df3;
    /* 00437da6 */
    a=(f->r[7]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 00437dab */
    if((f->flags&MI_ZF))goto mi_00437df3;
    /* 00437dad */
    a=(f->r[7]&0xffffu);b=0x000000c8u;t=mi_alu(f,a,b,16,1);
    /* 00437db2 */
    if((f->flags&MI_ZF))goto mi_00437df3;
    /* 00437db4 */
    a=(f->r[7]&0xffffu);b=0x000000c9u;t=mi_alu(f,a,b,16,1);
    /* 00437db9 */
    if((f->flags&MI_ZF))goto mi_00437df3;
    /* 00437dbb */
    a=(f->r[7]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 00437dc0 */
    if(!(f->flags&MI_ZF))goto mi_00437dd4;
    /* 00437dc2 */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00437dc5 */
    if((f->flags&MI_ZF))goto mi_00437df3;
    /* 00437dc7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437dc8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437dc9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437dca */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437dcfu;f->boundary(0x00437dcau,0x004c0160u,f);
    /* 00437dcf */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437dd2 */
    goto mi_00437ded;
mi_00437dd4:;
    /* 00437dd4 */
    a=(f->r[7]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 00437dd8 */
    if((f->flags&MI_ZF))goto mi_00437de1;
    /* 00437dda */
    a=(f->r[7]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 00437ddf */
    if(!(f->flags&MI_ZF))goto mi_00437df3;
mi_00437de1:;
    /* 00437de1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437de2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437de3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437de4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437de5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437deau;f->boundary(0x00437de5u,0x00442280u,f);
    /* 00437dea */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00437ded:;
    /* 00437ded */
    f->r[1]=mi_read((0x0070e488u),4u);
mi_00437df3:;
    /* 00437df3 */
    a=mi_read((0x00b67d1cu+f->r[1]),1u);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00437dfa */
    if((f->flags&MI_ZF))goto mi_00437e0f;
    /* 00437dfc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[1]*2u),2u);
    /* 00437e04 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437e05 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437e06 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437e07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437e0cu;f->boundary(0x00437e07u,0x00436310u,f);
    /* 00437e0c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00437e0f:;
    /* 00437e0f */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00437e13 */
    f->r[2]=mi_read((0x0070e480u),4u);
    /* 00437e19 */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 00437e1f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00437e20 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00437e21 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00437e25 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[2]);
    /* 00437e2b */
    f->r[2]=mi_read((0x0070e464u),4u);
    /* 00437e31 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00437e34 */
    f->r[5]=(0x00000002u+f->r[1]+f->r[5]*1u);
    /* 00437e38 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00437e3a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004375da;
mi_00437e40:;
    /* 00437e40 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00437e44 */
    f->r[2]=mi_read((0x0070e47cu),4u);
    /* 00437e4a */
    f->r[7]=mi_read((0x0070e490u),4u);
    /* 00437e50 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437e52 */
    if((f->flags&MI_ZF))goto mi_00437e5b;
    /* 00437e54 */
    a=f->r[7];b=mi_read((0x0070e45cu),4u);t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00437e5a */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_00437e5b:;
    /* 00437e5b */
    f->r[5]=mi_read((0x0070e458u),4u);
    /* 00437e61 */
    f->r[1]=mi_read((0x0070e478u),4u);
    /* 00437e67 */
    f->r[0]=0xfffffffeu;
    /* 00437e6c */
    a=f->r[1];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00437e6f */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00437e71 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00437e79 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[1]);
    /* 00437e7f */
    f->r[7]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 00437e82 */
    f->r[0]=mi_read((0x0070e464u),4u);
    /* 00437e87 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437e8a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437e8c */
    if(mi_signed_less(f->flags))goto mi_00437f4b;
    /* 00437e92 */
    f->r[3]=mi_read((0x00000074u+f->r[4]),4u);
mi_00437e96:;
    /* 00437e96 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00437e98 */
    if(mi_signed_less(f->flags))goto mi_00437f21;
    /* 00437e9e */
    a=f->r[1];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 00437ea4 */
    if(!mi_signed_less(f->flags))goto mi_00437f21;
    /* 00437ea6 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 00437ea9 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 00437eac */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 00437eaf */
    f->r[5]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00437eb2 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00437eb4 */
    f->r[0]=mi_read((0x0070e494u+f->r[0]*4u),4u);
    /* 00437ebb */
    a=f->r[0];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00437ec0 */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[0]);
    /* 00437ec5 */
    if(!mi_signed_less(f->flags))goto mi_00437f21;
    /* 00437ec7 */
    a=mi_read((0x00af598cu+f->r[0]*4u),4u);b=0xeefed77fu;t=mi_alu(f,a,b,32,3);
    /* 00437ed2 */
    if(!(f->flags&MI_ZF))goto mi_00437f21;
    /* 00437ed4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[0]*2u),2u);
    /* 00437edc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437ede */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00437f21;
mi_00437ee0:;
    /* 00437ee0 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00437ee3 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00437ee6 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00437ee8 */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00437eeb */
    a=mi_read((0x00bcd65du+f->r[5]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 00437ef2 */
    if(!(f->flags&MI_ZF))goto mi_00437f16;
    /* 00437ef4 */
    a=mi_read((0x00bcd718u+f->r[5]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00437efc */
    if(!(f->flags&MI_ZF))goto mi_00437f16;
    /* 00437efe */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437eff */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437f00 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437f01 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00437f02 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00437f07u;f->boundary(0x00437f02u,0x00438e50u,f);
    /* 00437f07 */
    f->r[1]=mi_read((0x0070e480u),4u);
    /* 00437f0d */
    f->r[2]=mi_read((0x0070e47cu),4u);
    /* 00437f13 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00437f16:;
    /* 00437f16 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[5]),2u);
    /* 00437f1d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00437f1f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00437ee0;
mi_00437f21:;
    /* 00437f21 */
    f->r[0]=mi_read((0x0070e458u),4u);
    /* 00437f26 */
    f->r[5]=mi_read((0x0070e464u),4u);
    /* 00437f2c */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00437f2f */
    f->r[7]=(0x00000002u+f->r[7]+f->r[0]*1u);
    /* 00437f33 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00437f37 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00437f38 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00437f39 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00437f3b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00437f3f */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[1]);
    /* 00437f45 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00437e96;
mi_00437f4b:;
    /* 00437f4b */
    f->r[5]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00437f4f */
    f->r[3]=mi_read((0x0070e44cu),4u);
    /* 00437f55 */
    f->r[0]=mi_read((0x00000028u+f->r[4]),4u);
    /* 00437f59 */
    f->r[2]=mi_read((0x0070e47cu),4u);
    /* 00437f5f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00437f61 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00437f63 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00437f66 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00437f68 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00437f69 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00437f6a */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00437f6e */
    f->r[1]=mi_read((0x0070e460u),4u);
    /* 00437f74 */
    MI_U32((0x0070e47cu))=(uint32_t)(f->r[2]);
    /* 00437f7a */
    f->r[2]=mi_read((0x00000024u+f->r[4]),4u);
    /* 00437f7e */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00437f82 */
    f->r[2]=(0x0000000eu+f->r[1]+f->r[2]*1u);
    /* 00437f86 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00437f88 */
    if(mi_signed_less(f->flags))goto mi_00436bb0;
    /* 00437f8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00437f8f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00437f90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_00437f91:;
    /* 00437f91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00437f92 */
    a=f->r[4];b=0x00000060u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00437f95 */
    f->r[4]+=4;return;
}
