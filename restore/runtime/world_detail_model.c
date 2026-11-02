/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_detail_model.h"
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

static const uint8_t mi_rom_0043b4fc[]={0x81,0xa4,0x43,0x00,0x91,0xa4,0x43,0x00,0xb7,0xa4,0x43,0x00,0xdd,0xa4,0x43,0x00,0x02,0xa5,0x43,0x00};
static const uint8_t mi_rom_0043b510[]={0x8e,0xa5,0x43,0x00,0x9e,0xa5,0x43,0x00,0xc4,0xa5,0x43,0x00,0xea,0xa5,0x43,0x00,0x0f,0xa6,0x43,0x00};
static const uint8_t mi_rom_0043b524[]={0xaa,0xb1,0x43,0x00,0x1e,0xb2,0x43,0x00,0x3c,0xb2,0x43,0x00,0x00,0xb2,0x43,0x00,0x59,0xb2,0x43,0x00,0x17,0xad,0x43,0x00,0x62,0xad,0x43,0x00,0x63,0xaf,0x43,0x00,0x5b,0xae,0x43,0x00,0x44,0xb0,0x43,0x00,0xd2,0xad,0x43,0x00,0x76,0xb1,0x43,0x00,0x59,0xb1,0x43,0x00,0x77,0xb2,0x43,0x00};
static const uint8_t mi_rom_0043b55c[]={0x00,0x01,0x0d,0x02,0x03,0x04,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x05,0x06,0x0d,0x07,0x08,0x09,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0a,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0b,0x0b,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0c,0x0c,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x05,0x08,0x06,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0d,0x0b,0x0c,0x0d,0x0d,0x0d,0x0b};
static const uint8_t mi_rom_0043b61c[]={0xf0,0xad,0x43,0x00,0xfe,0xad,0x43,0x00,0x0f,0xae,0x43,0x00,0x1d,0xae,0x43,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0043b4fcu&&width<=20u&&(offset=address-0x0043b4fcu)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0043b4fc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0043b510u&&width<=20u&&(offset=address-0x0043b510u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0043b510[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0043b524u&&width<=56u&&(offset=address-0x0043b524u)<=56u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0043b524[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0043b55cu&&width<=192u&&(offset=address-0x0043b55cu)<=192u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0043b55c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0043b61cu&&width<=16u&&(offset=address-0x0043b61cu)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0043b61c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_building_world_00439f10(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00439f10 */
    a=f->r[4];b=0x000000b8u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00439f16 */
    f->r[0]=mi_read((0x0070e44cu),4u);
    /* 00439f1b */
    f->r[2]=mi_read((0x0070e460u),4u);
    /* 00439f21 */
    f->r[1]=mi_read((0x0070e474u),4u);
    /* 00439f27 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00439f28 */
    f->r[7]=mi_read((0x0070e48cu),4u);
    /* 00439f2e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00439f31 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00439f33 */
    a=f->r[2];b=0x00000012u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00439f36 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00439f38 */
    a=f->r[1];b=0xfffffff8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00439f3b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00439f3d */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00439f41 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00439f45 */
    MI_U32((0x0070e47cu))=(uint32_t)(f->r[1]);
    /* 00439f4b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043b4f1;
    /* 00439f51 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00439f52 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00439f53 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00439f54:;
    /* 00439f54 */
    f->r[0]=mi_read((0x0070e47cu),4u);
    /* 00439f59 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00439f5b */
    if(mi_signed_less(f->flags))goto mi_0043b4b0;
    /* 00439f61 */
    a=f->r[0];b=0x000001c9u;t=mi_alu(f,a,b,32,1);
    /* 00439f66 */
    if(!mi_signed_less(f->flags))goto mi_0043b4b0;
    /* 00439f6c */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 00439f70 */
    f->r[2]=mi_read((0x0070e490u),4u);
    /* 00439f76 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00439f78 */
    f->r[6]=f->r[2];
    /* 00439f7a */
    if((f->flags&MI_ZF))goto mi_00439f83;
    /* 00439f7c */
    a=f->r[6];b=mi_read((0x0070e45cu),4u);t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00439f82 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
mi_00439f83:;
    /* 00439f83 */
    f->r[0]=mi_read((0x0070e458u),4u);
    /* 00439f88 */
    f->r[5]=mi_read((0x0070e464u),4u);
    /* 00439f8e */
    f->r[1]=0xfffffffeu;
    /* 00439f93 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00439f96 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00439f98 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00439f9a */
    f->r[6]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00439f9d */
    f->r[1]=mi_read((0x0070e478u),4u);
    /* 00439fa3 */
    a=f->r[1];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00439fa6 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00439fa8 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[1]);
    /* 00439fae */
    if(mi_signed_less(f->flags))goto mi_0043a060;
    /* 00439fb4 */
    f->r[2]=mi_read((0x0070e47cu),4u);
mi_00439fba:;
    /* 00439fba */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00439fbc */
    if(mi_signed_less(f->flags))goto mi_0043a038;
    /* 00439fbe */
    a=f->r[1];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 00439fc4 */
    if(!mi_signed_less(f->flags))goto mi_0043a038;
    /* 00439fc6 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 00439fc9 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 00439fcc */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 00439fcf */
    f->r[5]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00439fd2 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00439fd4 */
    f->r[0]=mi_read((0x0070e494u+f->r[0]*4u),4u);
    /* 00439fdb */
    a=f->r[0];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00439fe0 */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[0]);
    /* 00439fe5 */
    if(!mi_signed_less(f->flags))goto mi_0043a038;
    /* 00439fe7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[0]*2u),2u);
    /* 00439fef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00439ff1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043a038;
mi_00439ff3:;
    /* 00439ff3 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00439ff6 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00439ff9 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00439ffb */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00439ffe */
    a=mi_read((0x00bcd65du+f->r[5]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0043a005 */
    if(!(f->flags&MI_ZF))goto mi_0043a02d;
    /* 0043a007 */
    a=mi_read((0x00bcd718u+f->r[5]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0043a00f */
    if(!(f->flags&MI_ZF))goto mi_0043a02d;
    /* 0043a011 */
    a=0x0000270fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a016 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a017 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a018 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a019 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a01eu;f->boundary(0x0043a019u,0x00438e50u,f);
    /* 0043a01e */
    f->r[2]=mi_read((0x0070e47cu),4u);
    /* 0043a024 */
    f->r[1]=mi_read((0x0070e480u),4u);
    /* 0043a02a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043a02d:;
    /* 0043a02d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[5]),2u);
    /* 0043a034 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043a036 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00439ff3;
mi_0043a038:;
    /* 0043a038 */
    f->r[5]=mi_read((0x0070e464u),4u);
    /* 0043a03e */
    f->r[0]=mi_read((0x0070e458u),4u);
    /* 0043a043 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0043a044 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0043a047 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0043a048 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0043a04a */
    f->r[6]=(0x00000002u+f->r[6]+f->r[0]*1u);
    /* 0043a04e */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[1]);
    /* 0043a054 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00439fba;
    /* 0043a05a */
    f->r[2]=mi_read((0x0070e490u),4u);
mi_0043a060:;
    /* 0043a060 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0043a064 */
    f->r[6]=f->r[2];
    /* 0043a066 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0043a068 */
    if((f->flags&MI_ZF))goto mi_0043a071;
    /* 0043a06a */
    a=f->r[6];b=mi_read((0x0070e45cu),4u);t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043a070 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
mi_0043a071:;
    /* 0043a071 */
    f->r[2]=mi_read((0x0070e478u),4u);
    /* 0043a077 */
    f->r[1]=0xfffffffeu;
    /* 0043a07c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043a07e */
    f->r[0]=mi_read((0x0070e464u),4u);
    /* 0043a083 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043a086 */
    a=f->r[2];b=0xfffffffcu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043a089 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043a08b */
    f->r[6]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 0043a08e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0043a096 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[2]);
    /* 0043a09c */
    if(mi_signed_less(f->flags))goto mi_0043ab13;
mi_0043a0a2:;
    /* 0043a0a2 */
    f->r[0]=mi_read((0x0070e480u),4u);
    /* 0043a0a7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043a0a9 */
    if(mi_signed_less(f->flags))goto mi_0043aae3;
    /* 0043a0af */
    a=f->r[0];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 0043a0b4 */
    if(!mi_signed_less(f->flags))goto mi_0043aae3;
    /* 0043a0ba */
    f->r[0]=mi_read((0x0070e47cu),4u);
    /* 0043a0bf */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0043a0c2 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0043a0c5 */
    f->r[1]=mi_read((0x0070e480u),4u);
    /* 0043a0cb */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0043a0ce */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0043a0d1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043a0d3 */
    f->r[0]=mi_read((0x0070e494u+f->r[0]*4u),4u);
    /* 0043a0da */
    a=f->r[0];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 0043a0df */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[0]);
    /* 0043a0e4 */
    if(!mi_signed_less(f->flags))goto mi_0043aae3;
    /* 0043a0ea */
    f->r[2]=f->r[0];
    /* 0043a0ec */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[2]*2u),2u);
    /* 0043a0f4 */
    f->r[1]=f->r[0];
    /* 0043a0f6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043a0f9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043a0fb */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 0043a103 */
    a=(f->r[0]&0xffffu);b=0x000000e5u;t=mi_alu(f,a,b,16,1);
    /* 0043a107 */
    if((f->flags&MI_ZF))goto mi_0043a125;
    /* 0043a109 */
    a=(f->r[0]&0xffffu);b=0x000000eau;t=mi_alu(f,a,b,16,1);
    /* 0043a10d */
    if((f->flags&MI_ZF))goto mi_0043a125;
    /* 0043a10f */
    a=(f->r[0]&0xffffu);b=0x000000ebu;t=mi_alu(f,a,b,16,1);
    /* 0043a113 */
    if((f->flags&MI_ZF))goto mi_0043a125;
    /* 0043a115 */
    a=(f->r[0]&0xffffu);b=0x000000ecu;t=mi_alu(f,a,b,16,1);
    /* 0043a119 */
    if((f->flags&MI_ZF))goto mi_0043a125;
    /* 0043a11b */
    a=(f->r[0]&0xffffu);b=0x000000e6u;t=mi_alu(f,a,b,16,1);
    /* 0043a11f */
    if((f->flags&MI_ZF))goto mi_0043a125;
    /* 0043a121 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,5);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0043a123 */
    goto mi_0043a127;
mi_0043a125:;
    /* 0043a125 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
mi_0043a127:;
    /* 0043a127 */
    f->r[0]=mi_read((0x00af598cu+f->r[2]*4u),4u);
    /* 0043a12e */
    a=f->r[0];b=0x00000044u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043a131 */
    a=(f->r[0]&0xffu);b=0x00000044u;t=mi_alu(f,a,b,8,1);
    /* 0043a133 */
    if(!(f->flags&MI_ZF))goto mi_0043a16a;
    /* 0043a135 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00adc36cu+f->r[2]),1u))&0xffu)<<0);
    /* 0043a13b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a13d */
    if((f->flags&MI_ZF))goto mi_0043a16a;
    /* 0043a13f */
    f->r[2]=mi_read((0x0070e448u),4u);
    /* 0043a145 */
    f->r[1]=f->r[7];
    /* 0043a147 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043a149 */
    f->r[2]=mi_read((0x00f56d34u),4u);
    /* 0043a14f */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043a154 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a155 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a156 */
    f->r[1]=0x00f55bb0u;
    /* 0043a15b */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[2]*1u);
    /* 0043a15f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a160 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a165u;f->boundary(0x0043a160u,0x0053e590u,f);
    /* 0043a165 */
    goto mi_0043aae3;
mi_0043a16a:;
    /* 0043a16a */
    f->r[0]=mi_read((0x00e94e0cu),4u);
    /* 0043a16f */
    a=f->r[0];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 0043a172 */
    if(!(f->flags&MI_ZF))goto mi_0043a1de;
    /* 0043a174 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a176 */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a17c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a17d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a17e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a183u;f->boundary(0x0043a17eu,0x004339c0u,f);
    /* 0043a183 */
    f->r[1]=mi_read((0x0070e488u),4u);
    /* 0043a189 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a18c */
    f->r[0]=mi_read((0x00b7482cu+f->r[1]*4u),4u);
    /* 0043a193 */
    f->r[1]=mi_read((0x00f572ecu),4u);
    /* 0043a199 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0043a19e */
    f->r[2]=(0x00000030u+f->r[1]);
    /* 0043a1a1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043a1a3 */
    if(mi_signed_less(f->flags))goto mi_0043aae3;
    /* 0043a1a9 */
    a=f->r[1];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043a1ac */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043a1ae */
    if(!mi_signed_less(f->flags))goto mi_0043aae3;
    /* 0043a1b4 */
    f->r[5]=(0xffffff6au+f->r[7]);
    /* 0043a1ba */
    f->r[1]=0x00f55bb0u;
    /* 0043a1bf */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1c0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1c1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a1c7u;f->boundary(0x0043a1c2u,0x0053e590u,f);
    /* 0043a1c7 */
    f->r[0]=mi_read((0x00d3b5c0u),4u);
    /* 0043a1cc */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1cd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1cf */
    f->r[1]=0x00f55bb0u;
    /* 0043a1d4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a1d9u;f->boundary(0x0043a1d4u,0x0053f250u,f);
    /* 0043a1d9 */
    goto mi_0043aae3;
mi_0043a1de:;
    /* 0043a1de */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0043a1e1 */
    if(!(f->flags&MI_ZF))goto mi_0043a1f7;
    /* 0043a1e3 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a1e5 */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a1eb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1ec */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a1ed */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a1f2u;f->boundary(0x0043a1edu,0x00435050u,f);
    /* 0043a1f2 */
    goto mi_0043aae0;
mi_0043a1f7:;
    /* 0043a1f7 */
    a=f->r[0];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 0043a1fa */
    if(!(f->flags&MI_ZF))goto mi_0043a217;
    /* 0043a1fc */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a1fe */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a204 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a206 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a208 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a209 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a20a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a20fu;f->boundary(0x0043a20au,0x00434aa0u,f);
    /* 0043a20f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a212 */
    goto mi_0043aae3;
mi_0043a217:;
    /* 0043a217 */
    a=f->r[0];b=0x00000022u;t=mi_alu(f,a,b,32,1);
    /* 0043a21a */
    if(!(f->flags&MI_ZF))goto mi_0043a237;
    /* 0043a21c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a21e */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a224 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a226 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a228 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a229 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a22a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a22fu;f->boundary(0x0043a22au,0x00434aa0u,f);
    /* 0043a22f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a232 */
    goto mi_0043aae3;
mi_0043a237:;
    /* 0043a237 */
    a=f->r[0];b=0x00000023u;t=mi_alu(f,a,b,32,1);
    /* 0043a23a */
    if(!(f->flags&MI_ZF))goto mi_0043a257;
    /* 0043a23c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a23e */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a244 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a246 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a248 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a249 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a24a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a24fu;f->boundary(0x0043a24au,0x00434aa0u,f);
    /* 0043a24f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a252 */
    goto mi_0043aae3;
mi_0043a257:;
    /* 0043a257 */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0043a25a */
    if(!(f->flags&MI_ZF))goto mi_0043a277;
    /* 0043a25c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a25e */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a264 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a266 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a268 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a269 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a26a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a26fu;f->boundary(0x0043a26au,0x00434aa0u,f);
    /* 0043a26f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a272 */
    goto mi_0043aae3;
mi_0043a277:;
    /* 0043a277 */
    a=f->r[0];b=0x00000025u;t=mi_alu(f,a,b,32,1);
    /* 0043a27a */
    if(!(f->flags&MI_ZF))goto mi_0043a297;
    /* 0043a27c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a27e */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a284 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a286 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a288 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a289 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a28a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a28fu;f->boundary(0x0043a28au,0x00434aa0u,f);
    /* 0043a28f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a292 */
    goto mi_0043aae3;
mi_0043a297:;
    /* 0043a297 */
    a=f->r[0];b=0x00000026u;t=mi_alu(f,a,b,32,1);
    /* 0043a29a */
    if(!(f->flags&MI_ZF))goto mi_0043a2b7;
    /* 0043a29c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a29e */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a2a4 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2a6 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2a8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2a9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a2afu;f->boundary(0x0043a2aau,0x00434aa0u,f);
    /* 0043a2af */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a2b2 */
    goto mi_0043aae3;
mi_0043a2b7:;
    /* 0043a2b7 */
    a=f->r[0];b=0x00000027u;t=mi_alu(f,a,b,32,1);
    /* 0043a2ba */
    if(!(f->flags&MI_ZF))goto mi_0043a2d3;
    /* 0043a2bc */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a2be */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a2c0 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2c2 */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2c4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2c5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a2cbu;f->boundary(0x0043a2c6u,0x00434aa0u,f);
    /* 0043a2cb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a2ce */
    goto mi_0043aae3;
mi_0043a2d3:;
    /* 0043a2d3 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 0043a2d6 */
    if(!(f->flags&MI_ZF))goto mi_0043a2ef;
    /* 0043a2d8 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a2da */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a2dc */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2de */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2e0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2e1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a2e7u;f->boundary(0x0043a2e2u,0x00434aa0u,f);
    /* 0043a2e7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a2ea */
    goto mi_0043aae3;
mi_0043a2ef:;
    /* 0043a2ef */
    a=f->r[0];b=0x00000029u;t=mi_alu(f,a,b,32,1);
    /* 0043a2f2 */
    if(!(f->flags&MI_ZF))goto mi_0043a30b;
    /* 0043a2f4 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a2f6 */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a2f8 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2fa */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2fc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2fd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a2fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a303u;f->boundary(0x0043a2feu,0x00434aa0u,f);
    /* 0043a303 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a306 */
    goto mi_0043aae3;
mi_0043a30b:;
    /* 0043a30b */
    a=f->r[0];b=0x0000002au;t=mi_alu(f,a,b,32,1);
    /* 0043a30e */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a310 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a312 */
    if(!(f->flags&MI_ZF))goto mi_0043a327;
    /* 0043a314 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a316 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a318 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a319 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a31a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a31fu;f->boundary(0x0043a31au,0x00434aa0u,f);
    /* 0043a31f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a322 */
    goto mi_0043aae3;
mi_0043a327:;
    /* 0043a327 */
    a=mi_read((0x00b67d1cu+f->r[2]),1u);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0043a32e */
    if((f->flags&MI_ZF))goto mi_0043aae3;
    /* 0043a334 */
    f->r[2]=mi_read((0x00e94e0cu),4u);
    /* 0043a33a */
    a=f->r[2];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 0043a33d */
    if(!(f->flags&MI_ZF))goto mi_0043a353;
    /* 0043a33f */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a341 */
    if(!(f->flags&MI_ZF))goto mi_0043a353;
    /* 0043a343 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a344 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a345 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a34au;f->boundary(0x0043a345u,0x00432320u,f);
    /* 0043a34a */
    f->r[2]=mi_read((0x00e94e0cu),4u);
    /* 0043a350 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043a353:;
    /* 0043a353 */
    f->r[1]=mi_read((0x0070e488u),4u);
    /* 0043a359 */
    f->r[0]=mi_read((0x00b7482cu+f->r[1]*4u),4u);
    /* 0043a360 */
    f->r[1]=mi_read((0x00f572ecu),4u);
    /* 0043a366 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0043a36b */
    f->r[3]=(0x00000030u+f->r[1]);
    /* 0043a36e */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0043a370 */
    if(mi_signed_less(f->flags))goto mi_0043a3a4;
    /* 0043a372 */
    a=f->r[1];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043a375 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043a377 */
    if(!mi_signed_less(f->flags))goto mi_0043a3a4;
    /* 0043a379 */
    f->r[5]=(0xffffff6au+f->r[7]);
    /* 0043a37f */
    f->r[1]=0x00f55bb0u;
    /* 0043a384 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a385 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a386 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a387 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a38cu;f->boundary(0x0043a387u,0x0053e590u,f);
    /* 0043a38c */
    f->r[2]=mi_read((0x00d3b5c0u),4u);
    /* 0043a392 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a393 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a394 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a395 */
    f->r[1]=0x00f55bb0u;
    /* 0043a39a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a39fu;f->boundary(0x0043a39au,0x0053f250u,f);
    /* 0043a39f */
    goto mi_0043aae3;
mi_0043a3a4:;
    /* 0043a3a4 */
    a=f->r[2];b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 0043a3a7 */
    if((f->flags&MI_ZF))goto mi_0043aad9;
    /* 0043a3ad */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x006de515u),1u))&0xffu)<<0);
    /* 0043a3b3 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a3b5 */
    if(!(f->flags&MI_ZF))goto mi_0043aad9;
    /* 0043a3bb */
    a=f->r[2];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0043a3be */
    if(!(f->flags&MI_ZF))goto mi_0043a3cc;
    /* 0043a3c0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a3c1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a3c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a3c7u;f->boundary(0x0043a3c2u,0x0051f780u,f);
    /* 0043a3c7 */
    goto mi_0043aae0;
mi_0043a3cc:;
    /* 0043a3cc */
    f->r[1]=mi_read((0x0070e488u),4u);
    /* 0043a3d2 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[1]*2u),2u);
    /* 0043a3da */
    f->r[2]=mi_read((0x00af598cu+f->r[1]*4u),4u);
    /* 0043a3e1 */
    f->r[1]=f->r[5];
    /* 0043a3e3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043a3e6 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043a3e8 */
    f->r[3]=(0x007999e8u+f->r[1]*8u);
    /* 0043a3ef */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 0043a3f7 */
    a=(f->r[1]&0xffffu);b=0x000000e5u;t=mi_alu(f,a,b,16,1);
    /* 0043a3fc */
    if((f->flags&MI_ZF))goto mi_0043a9d3;
    /* 0043a402 */
    a=(f->r[1]&0xffffu);b=0x000000eau;t=mi_alu(f,a,b,16,1);
    /* 0043a407 */
    if((f->flags&MI_ZF))goto mi_0043a9d3;
    /* 0043a40d */
    a=(f->r[1]&0xffffu);b=0x000000ebu;t=mi_alu(f,a,b,16,1);
    /* 0043a412 */
    if((f->flags&MI_ZF))goto mi_0043a9d3;
    /* 0043a418 */
    a=(f->r[1]&0xffffu);b=0x000000ecu;t=mi_alu(f,a,b,16,1);
    /* 0043a41d */
    if((f->flags&MI_ZF))goto mi_0043a9d3;
    /* 0043a423 */
    a=(f->r[1]&0xffffu);b=0x000000e6u;t=mi_alu(f,a,b,16,1);
    /* 0043a428 */
    if((f->flags&MI_ZF))goto mi_0043a9d3;
    /* 0043a42e */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0043a431 */
    if((f->flags&MI_ZF))goto mi_0043a88d;
    /* 0043a437 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0043a439 */
    if((f->flags&MI_ZF))goto mi_0043aae3;
    /* 0043a43f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a440 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a445u;f->boundary(0x0043a440u,0x00431690u,f);
    /* 0043a445 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a448 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043a44a */
    if((f->flags&MI_ZF))goto mi_0043a536;
    /* 0043a450 */
    f->r[5]=mi_read((0x0070e488u),4u);
    /* 0043a456 */
    f->r[1]=f->r[6];
    /* 0043a458 */
    f->r[0]=f->r[7];
    /* 0043a45a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00b285ccu+f->r[5]),1u))&0xffu)<<0);
    /* 0043a460 */
    f->r[3]=mi_read((0x00b7482cu+f->r[5]*4u),4u);
    /* 0043a467 */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0043a46a */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[3]);
    /* 0043a470 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0043a471 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043a474 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0043a524;
    /* 0043a47a */
    switch(mi_read((0x0043b4fcu+f->r[2]*4u),4u)){case 0x0043a481u:goto mi_0043a481;case 0x0043a491u:goto mi_0043a491;case 0x0043a4b7u:goto mi_0043a4b7;case 0x0043a4ddu:goto mi_0043a4dd;case 0x0043a502u:goto mi_0043a502;default:abort();}
mi_0043a481:;
    /* 0043a481 */
    f->r[0]=mi_read((0x0070e45cu),4u);
    /* 0043a486 */
    f->r[1]=(0x00000001u+f->r[0]+f->r[6]*1u);
    /* 0043a48a */
    f->r[0]=f->r[7];
    /* 0043a48c */
    goto mi_0043a51e;
mi_0043a491:;
    /* 0043a491 */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 0043a497 */
    f->r[2]=mi_read((0x0070e448u),4u);
    /* 0043a49d */
    f->r[0]=f->r[7];
    /* 0043a49f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043a4a1 */
    f->r[1]=(0x00000002u+f->r[1]+f->r[6]*1u);
    /* 0043a4a5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4a6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4a7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4a8 */
    f->r[1]=0x00f55bb0u;
    /* 0043a4ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a4b2u;f->boundary(0x0043a4adu,0x0053f250u,f);
    /* 0043a4b2 */
    goto mi_0043aae3;
mi_0043a4b7:;
    /* 0043a4b7 */
    f->r[2]=mi_read((0x0070e454u),4u);
    /* 0043a4bd */
    f->r[0]=f->r[7];
    /* 0043a4bf */
    f->r[1]=(0x00000003u+f->r[2]+f->r[6]*1u);
    /* 0043a4c3 */
    f->r[2]=mi_read((0x0070e444u),4u);
    /* 0043a4c9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043a4cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4cc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4cd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4ce */
    f->r[1]=0x00f55bb0u;
    /* 0043a4d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a4d8u;f->boundary(0x0043a4d3u,0x0053f250u,f);
    /* 0043a4d8 */
    goto mi_0043aae3;
mi_0043a4dd:;
    /* 0043a4dd */
    f->r[0]=mi_read((0x0070e450u),4u);
    /* 0043a4e2 */
    f->r[2]=mi_read((0x0070e440u),4u);
    /* 0043a4e8 */
    f->r[1]=(0x00000004u+f->r[0]+f->r[6]*1u);
    /* 0043a4ec */
    f->r[0]=f->r[7];
    /* 0043a4ee */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043a4f0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4f1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4f2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a4f3 */
    f->r[1]=0x00f55bb0u;
    /* 0043a4f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a4fdu;f->boundary(0x0043a4f8u,0x0053f250u,f);
    /* 0043a4fd */
    goto mi_0043aae3;
mi_0043a502:;
    /* 0043a502 */
    f->r[1]=mi_read((0x0070e45cu),4u);
    /* 0043a508 */
    f->r[2]=mi_read((0x0070e450u),4u);
    /* 0043a50e */
    f->r[5]=mi_read((0x0070e440u),4u);
    /* 0043a514 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043a516 */
    f->r[0]=f->r[7];
    /* 0043a518 */
    f->r[1]=(0x00000005u+f->r[2]+f->r[6]*1u);
    /* 0043a51c */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0043a51e:;
    /* 0043a51e */
    a=f->r[0];b=mi_read((0x0070e44cu),4u);t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0043a524:;
    /* 0043a524 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a525 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a526 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a527 */
    f->r[1]=0x00f55bb0u;
    /* 0043a52c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a531u;f->boundary(0x0043a52cu,0x0053f250u,f);
    /* 0043a531 */
    goto mi_0043aae3;
mi_0043a536:;
    /* 0043a536 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00000000u+f->r[3]),2u))&0xffffu)<<0);
    /* 0043a539 */
    a=(f->r[0]&0xffffu);b=0x00000021u;t=mi_alu(f,a,b,16,1);
    /* 0043a53d */
    if((f->flags&MI_ZF))goto mi_0043a54f;
    /* 0043a53f */
    a=(f->r[0]&0xffffu);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 0043a543 */
    if((f->flags&MI_ZF))goto mi_0043a54f;
    /* 0043a545 */
    a=(f->r[0]&0xffffu);b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 0043a549 */
    if(!(f->flags&MI_ZF))goto mi_0043a643;
mi_0043a54f:;
    /* 0043a54f */
    f->r[2]=mi_read((0x0070e488u),4u);
    /* 0043a555 */
    a=mi_read((0x00af598cu+f->r[2]*4u),1u);b=0x00000060u;t=mi_alu(f,a,b,8,3);
    /* 0043a55d */
    if((f->flags&MI_ZF))goto mi_0043a643;
    /* 0043a563 */
    f->r[5]=mi_read((0x00b7482cu+f->r[2]*4u),4u);
    /* 0043a56a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00b285ccu+f->r[2]),1u))&0xffu)<<0);
    /* 0043a570 */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0043a573 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[5]);
    /* 0043a579 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0043a57a */
    f->r[1]=f->r[6];
    /* 0043a57c */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043a57f */
    f->r[0]=f->r[7];
    /* 0043a581 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0043a631;
    /* 0043a587 */
    switch(mi_read((0x0043b510u+f->r[2]*4u),4u)){case 0x0043a58eu:goto mi_0043a58e;case 0x0043a59eu:goto mi_0043a59e;case 0x0043a5c4u:goto mi_0043a5c4;case 0x0043a5eau:goto mi_0043a5ea;case 0x0043a60fu:goto mi_0043a60f;default:abort();}
mi_0043a58e:;
    /* 0043a58e */
    f->r[0]=mi_read((0x0070e45cu),4u);
    /* 0043a593 */
    f->r[1]=(0x00000001u+f->r[0]+f->r[6]*1u);
    /* 0043a597 */
    f->r[0]=f->r[7];
    /* 0043a599 */
    goto mi_0043a62b;
mi_0043a59e:;
    /* 0043a59e */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 0043a5a4 */
    f->r[2]=mi_read((0x0070e448u),4u);
    /* 0043a5aa */
    f->r[0]=f->r[7];
    /* 0043a5ac */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043a5ae */
    f->r[1]=(0x00000002u+f->r[1]+f->r[6]*1u);
    /* 0043a5b2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5b3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5b4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5b5 */
    f->r[1]=0x00f55bb0u;
    /* 0043a5ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a5bfu;f->boundary(0x0043a5bau,0x0053f250u,f);
    /* 0043a5bf */
    goto mi_0043aae3;
mi_0043a5c4:;
    /* 0043a5c4 */
    f->r[2]=mi_read((0x0070e454u),4u);
    /* 0043a5ca */
    f->r[0]=f->r[7];
    /* 0043a5cc */
    f->r[1]=(0x00000003u+f->r[2]+f->r[6]*1u);
    /* 0043a5d0 */
    f->r[2]=mi_read((0x0070e444u),4u);
    /* 0043a5d6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043a5d8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5d9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5da */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5db */
    f->r[1]=0x00f55bb0u;
    /* 0043a5e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a5e5u;f->boundary(0x0043a5e0u,0x0053f250u,f);
    /* 0043a5e5 */
    goto mi_0043aae3;
mi_0043a5ea:;
    /* 0043a5ea */
    f->r[0]=mi_read((0x0070e450u),4u);
    /* 0043a5ef */
    f->r[2]=mi_read((0x0070e440u),4u);
    /* 0043a5f5 */
    f->r[1]=(0x00000004u+f->r[0]+f->r[6]*1u);
    /* 0043a5f9 */
    f->r[0]=f->r[7];
    /* 0043a5fb */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043a5fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5fe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a5ff */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a600 */
    f->r[1]=0x00f55bb0u;
    /* 0043a605 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a60au;f->boundary(0x0043a605u,0x0053f250u,f);
    /* 0043a60a */
    goto mi_0043aae3;
mi_0043a60f:;
    /* 0043a60f */
    f->r[1]=mi_read((0x0070e45cu),4u);
    /* 0043a615 */
    f->r[2]=mi_read((0x0070e450u),4u);
    /* 0043a61b */
    f->r[3]=mi_read((0x0070e440u),4u);
    /* 0043a621 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043a623 */
    f->r[0]=f->r[7];
    /* 0043a625 */
    f->r[1]=(0x00000005u+f->r[2]+f->r[6]*1u);
    /* 0043a629 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0043a62b:;
    /* 0043a62b */
    a=f->r[0];b=mi_read((0x0070e44cu),4u);t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0043a631:;
    /* 0043a631 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a632 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a633 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a634 */
    f->r[1]=0x00f55bb0u;
    /* 0043a639 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a63eu;f->boundary(0x0043a639u,0x0053f250u,f);
    /* 0043a63e */
    goto mi_0043aae3;
mi_0043a643:;
    /* 0043a643 */
    a=(f->r[0]&0xffffu);b=0x000000d1u;t=mi_alu(f,a,b,16,1);
    /* 0043a647 */
    if((f->flags&MI_ZF))goto mi_0043aae3;
    /* 0043a64d */
    f->r[0]=mi_read((0x00e94e0cu),4u);
    /* 0043a652 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0043a655 */
    if(!(f->flags&MI_ZF))goto mi_0043a667;
    /* 0043a657 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a658 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a659 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a65a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a65fu;f->boundary(0x0043a65au,0x00435290u,f);
    /* 0043a65f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a662 */
    goto mi_0043aae3;
mi_0043a667:;
    /* 0043a667 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0043a66a */
    if(!(f->flags&MI_ZF))goto mi_0043a67c;
    /* 0043a66c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a66d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a66e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a66f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a674u;f->boundary(0x0043a66fu,0x004353a0u,f);
    /* 0043a674 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a677 */
    goto mi_0043aae3;
mi_0043a67c:;
    /* 0043a67c */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0043a67f */
    if(!(f->flags&MI_ZF))goto mi_0043a691;
    /* 0043a681 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a682 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a683 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a684 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a689u;f->boundary(0x0043a684u,0x00435570u,f);
    /* 0043a689 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a68c */
    goto mi_0043aae3;
mi_0043a691:;
    /* 0043a691 */
    a=f->r[0];b=0x0000002bu;t=mi_alu(f,a,b,32,1);
    /* 0043a694 */
    if(!(f->flags&MI_ZF))goto mi_0043a6a6;
    /* 0043a696 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a697 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a698 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a699 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a69eu;f->boundary(0x0043a699u,0x004354b0u,f);
    /* 0043a69e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a6a1 */
    goto mi_0043aae3;
mi_0043a6a6:;
    /* 0043a6a6 */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0043a6a9 */
    if(!(f->flags&MI_ZF))goto mi_0043a6bb;
    /* 0043a6ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6ac */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6ad */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6ae */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a6b3u;f->boundary(0x0043a6aeu,0x00435510u,f);
    /* 0043a6b3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a6b6 */
    goto mi_0043aae3;
mi_0043a6bb:;
    /* 0043a6bb */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0043a6be */
    if(!(f->flags&MI_ZF))goto mi_0043a6d0;
    /* 0043a6c0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6c1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6c2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a6c8u;f->boundary(0x0043a6c3u,0x004355d0u,f);
    /* 0043a6c8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a6cb */
    goto mi_0043aae3;
mi_0043a6d0:;
    /* 0043a6d0 */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 0043a6d3 */
    if(!(f->flags&MI_ZF))goto mi_0043a6e5;
    /* 0043a6d5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6d6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6d7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a6ddu;f->boundary(0x0043a6d8u,0x00435630u,f);
    /* 0043a6dd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a6e0 */
    goto mi_0043aae3;
mi_0043a6e5:;
    /* 0043a6e5 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0043a6e8 */
    if(!(f->flags&MI_ZF))goto mi_0043a6fa;
    /* 0043a6ea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6eb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6ec */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a6ed */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a6f2u;f->boundary(0x0043a6edu,0x004356a0u,f);
    /* 0043a6f2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a6f5 */
    goto mi_0043aae3;
mi_0043a6fa:;
    /* 0043a6fa */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 0043a6fd */
    if(!(f->flags&MI_ZF))goto mi_0043a70f;
    /* 0043a6ff */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a700 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a701 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a702 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a707u;f->boundary(0x0043a702u,0x00435700u,f);
    /* 0043a707 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a70a */
    goto mi_0043aae3;
mi_0043a70f:;
    /* 0043a70f */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 0043a712 */
    if(!(f->flags&MI_ZF))goto mi_0043a724;
    /* 0043a714 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a715 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a716 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a717 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a71cu;f->boundary(0x0043a717u,0x00435760u,f);
    /* 0043a71c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a71f */
    goto mi_0043aae3;
mi_0043a724:;
    /* 0043a724 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0043a727 */
    if(!(f->flags&MI_ZF))goto mi_0043a739;
    /* 0043a729 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a72a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a72b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a72c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a731u;f->boundary(0x0043a72cu,0x004357c0u,f);
    /* 0043a731 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a734 */
    goto mi_0043aae3;
mi_0043a739:;
    /* 0043a739 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0043a73c */
    if(!(f->flags&MI_ZF))goto mi_0043a74e;
    /* 0043a73e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a73f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a740 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a741 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a746u;f->boundary(0x0043a741u,0x00435820u,f);
    /* 0043a746 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a749 */
    goto mi_0043aae3;
mi_0043a74e:;
    /* 0043a74e */
    a=f->r[0];b=0x0000001au;t=mi_alu(f,a,b,32,1);
    /* 0043a751 */
    if(!(f->flags&MI_ZF))goto mi_0043a763;
    /* 0043a753 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a754 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a755 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a756 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a75bu;f->boundary(0x0043a756u,0x00435880u,f);
    /* 0043a75b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a75e */
    goto mi_0043aae3;
mi_0043a763:;
    /* 0043a763 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043a766 */
    if(!(f->flags&MI_ZF))goto mi_0043a778;
    /* 0043a768 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a769 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a76a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a76b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a770u;f->boundary(0x0043a76bu,0x00435a70u,f);
    /* 0043a770 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a773 */
    goto mi_0043aae3;
mi_0043a778:;
    /* 0043a778 */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 0043a77b */
    if(!(f->flags&MI_ZF))goto mi_0043a78d;
    /* 0043a77d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a77e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a77f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a780 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a785u;f->boundary(0x0043a780u,0x00435ac0u,f);
    /* 0043a785 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a788 */
    goto mi_0043aae3;
mi_0043a78d:;
    /* 0043a78d */
    a=f->r[0];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 0043a790 */
    if(!(f->flags&MI_ZF))goto mi_0043a7a2;
    /* 0043a792 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a793 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a794 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a795 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a79au;f->boundary(0x0043a795u,0x00435b20u,f);
    /* 0043a79a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a79d */
    goto mi_0043aae3;
mi_0043a7a2:;
    /* 0043a7a2 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0043a7a5 */
    if(!(f->flags&MI_ZF))goto mi_0043a7b7;
    /* 0043a7a7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7a8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7a9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a7afu;f->boundary(0x0043a7aau,0x0051f780u,f);
    /* 0043a7af */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a7b2 */
    goto mi_0043aae3;
mi_0043a7b7:;
    /* 0043a7b7 */
    a=f->r[0];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0043a7ba */
    if(!(f->flags&MI_ZF))goto mi_0043a7cc;
    /* 0043a7bc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7bd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7be */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a7c4u;f->boundary(0x0043a7bfu,0x00435b80u,f);
    /* 0043a7c4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a7c7 */
    goto mi_0043aae3;
mi_0043a7cc:;
    /* 0043a7cc */
    a=f->r[0];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 0043a7cf */
    if(!(f->flags&MI_ZF))goto mi_0043a7e1;
    /* 0043a7d1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7d2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7d3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7d4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a7d9u;f->boundary(0x0043a7d4u,0x00435a10u,f);
    /* 0043a7d9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a7dc */
    goto mi_0043aae3;
mi_0043a7e1:;
    /* 0043a7e1 */
    a=f->r[0];b=0x00000017u;t=mi_alu(f,a,b,32,1);
    /* 0043a7e4 */
    if(!(f->flags&MI_ZF))goto mi_0043a7f6;
    /* 0043a7e6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7e7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7e8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a7eeu;f->boundary(0x0043a7e9u,0x00435c40u,f);
    /* 0043a7ee */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a7f1 */
    goto mi_0043aae3;
mi_0043a7f6:;
    /* 0043a7f6 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);
    /* 0043a7f9 */
    if(!(f->flags&MI_ZF))goto mi_0043a80b;
    /* 0043a7fb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7fc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7fd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a7fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a803u;f->boundary(0x0043a7feu,0x00435be0u,f);
    /* 0043a803 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a806 */
    goto mi_0043aae3;
mi_0043a80b:;
    /* 0043a80b */
    a=f->r[0];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 0043a80e */
    if(!(f->flags&MI_ZF))goto mi_0043a820;
    /* 0043a810 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a811 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a812 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a813 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a818u;f->boundary(0x0043a813u,0x00435ca0u,f);
    /* 0043a818 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a81b */
    goto mi_0043aae3;
mi_0043a820:;
    /* 0043a820 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0043a823 */
    if(!(f->flags&MI_ZF))goto mi_0043a835;
    /* 0043a825 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a826 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a827 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a828 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a82du;f->boundary(0x0043a828u,0x00435d60u,f);
    /* 0043a82d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a830 */
    goto mi_0043aae3;
mi_0043a835:;
    /* 0043a835 */
    a=f->r[0];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 0043a838 */
    if(!(f->flags&MI_ZF))goto mi_0043a84a;
    /* 0043a83a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a83b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a83c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a83d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a842u;f->boundary(0x0043a83du,0x00436140u,f);
    /* 0043a842 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a845 */
    goto mi_0043aae3;
mi_0043a84a:;
    /* 0043a84a */
    a=f->r[0];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 0043a84d */
    if(!(f->flags&MI_ZF))goto mi_0043a85f;
    /* 0043a84f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a850 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a851 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a852 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a857u;f->boundary(0x0043a852u,0x00435e70u,f);
    /* 0043a857 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a85a */
    goto mi_0043aae3;
mi_0043a85f:;
    /* 0043a85f */
    a=f->r[0];b=0x0000002cu;t=mi_alu(f,a,b,32,1);
    /* 0043a862 */
    if(!(f->flags&MI_ZF))goto mi_0043a874;
    /* 0043a864 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a865 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a866 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a867 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a86cu;f->boundary(0x0043a867u,0x004360a0u,f);
    /* 0043a86c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a86f */
    goto mi_0043aae3;
mi_0043a874:;
    /* 0043a874 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 0043a877 */
    if(!(f->flags&MI_ZF))goto mi_0043aae3;
    /* 0043a87d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a87e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a87f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a880 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a885u;f->boundary(0x0043a880u,0x00435d00u,f);
    /* 0043a885 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043a888 */
    goto mi_0043aae3;
mi_0043a88d:;
    /* 0043a88d */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,3);
    /* 0043a893 */
    if(!(f->flags&MI_ZF))goto mi_0043aae3;
    /* 0043a899 */
    a=(f->r[2]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 0043a89c */
    if((f->flags&MI_ZF))goto mi_0043a8ab;
    /* 0043a89e */
    a=mi_read((0x00e94e0cu),4u);b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 0043a8a5 */
    if((f->flags&MI_ZF))goto mi_0043aae3;
mi_0043a8ab:;
    /* 0043a8ab */
    a=f->r[2];b=0x00c04000u;t=mi_alu(f,a,b,32,3);
    /* 0043a8b1 */
    if((f->flags&MI_ZF))goto mi_0043a8c0;
    /* 0043a8b3 */
    a=mi_read((0x00e94e0cu),4u);b=0x00000031u;t=mi_alu(f,a,b,32,1);
    /* 0043a8ba */
    if(!(f->flags&MI_ZF))goto mi_0043aae3;
mi_0043a8c0:;
    /* 0043a8c0 */
    f->r[1]=mi_read((0x0070e488u),4u);
    /* 0043a8c6 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0043a8cb */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00b285ccu+f->r[1]),1u))&0xffu)<<0);
    /* 0043a8d1 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0043a8d4 */
    if(!(f->flags&MI_ZF))goto mi_0043a8e8;
    /* 0043a8d6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a8d7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a8d8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a8d9 */
    f->r[1]=0x00f55bb0u;
    /* 0043a8de */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a8e3u;f->boundary(0x0043a8deu,0x0053f250u,f);
    /* 0043a8e3 */
    goto mi_0043aae3;
mi_0043a8e8:;
    /* 0043a8e8 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043a8eb */
    if(!(f->flags&MI_ZF))goto mi_0043a913;
    /* 0043a8ed */
    f->r[1]=mi_read((0x0070e44cu),4u);
    /* 0043a8f3 */
    f->r[2]=f->r[7];
    /* 0043a8f5 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043a8f7 */
    f->r[1]=mi_read((0x0070e45cu),4u);
    /* 0043a8fd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a8fe */
    f->r[2]=(0x00000001u+f->r[1]+f->r[6]*1u);
    /* 0043a902 */
    f->r[1]=0x00f55bb0u;
    /* 0043a907 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a908 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a909 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a90eu;f->boundary(0x0043a909u,0x0053f250u,f);
    /* 0043a90e */
    goto mi_0043aae3;
mi_0043a913:;
    /* 0043a913 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043a916 */
    if(!(f->flags&MI_ZF))goto mi_0043a93e;
    /* 0043a918 */
    f->r[5]=mi_read((0x0070e448u),4u);
    /* 0043a91e */
    f->r[2]=mi_read((0x0070e458u),4u);
    /* 0043a924 */
    f->r[1]=f->r[7];
    /* 0043a926 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043a928 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a929 */
    f->r[1]=(0x00000002u+f->r[2]+f->r[6]*1u);
    /* 0043a92d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a92e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a92f */
    f->r[1]=0x00f55bb0u;
    /* 0043a934 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a939u;f->boundary(0x0043a934u,0x0053f250u,f);
    /* 0043a939 */
    goto mi_0043aae3;
mi_0043a93e:;
    /* 0043a93e */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043a941 */
    if(!(f->flags&MI_ZF))goto mi_0043a969;
    /* 0043a943 */
    f->r[1]=mi_read((0x0070e444u),4u);
    /* 0043a949 */
    f->r[2]=f->r[7];
    /* 0043a94b */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043a94d */
    f->r[1]=mi_read((0x0070e454u),4u);
    /* 0043a953 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a954 */
    f->r[2]=(0x00000003u+f->r[1]+f->r[6]*1u);
    /* 0043a958 */
    f->r[1]=0x00f55bb0u;
    /* 0043a95d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a95e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a95f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a964u;f->boundary(0x0043a95fu,0x0053f250u,f);
    /* 0043a964 */
    goto mi_0043aae3;
mi_0043a969:;
    /* 0043a969 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043a96c */
    if(!(f->flags&MI_ZF))goto mi_0043a994;
    /* 0043a96e */
    f->r[5]=mi_read((0x0070e440u),4u);
    /* 0043a974 */
    f->r[2]=mi_read((0x0070e450u),4u);
    /* 0043a97a */
    f->r[1]=f->r[7];
    /* 0043a97c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043a97e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a97f */
    f->r[1]=(0x00000004u+f->r[2]+f->r[6]*1u);
    /* 0043a983 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a984 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a985 */
    f->r[1]=0x00f55bb0u;
    /* 0043a98a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a98fu;f->boundary(0x0043a98au,0x0053f250u,f);
    /* 0043a98f */
    goto mi_0043aae3;
mi_0043a994:;
    /* 0043a994 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0043a997 */
    if(!(f->flags&MI_ZF))goto mi_0043aae3;
    /* 0043a99d */
    f->r[1]=mi_read((0x0070e440u),4u);
    /* 0043a9a3 */
    f->r[5]=mi_read((0x0070e44cu),4u);
    /* 0043a9a9 */
    f->r[2]=f->r[7];
    /* 0043a9ab */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043a9ad */
    f->r[1]=mi_read((0x0070e450u),4u);
    /* 0043a9b3 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043a9b5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a9b6 */
    f->r[2]=mi_read((0x0070e45cu),4u);
    /* 0043a9bc */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043a9be */
    f->r[1]=(0x00000005u+f->r[1]+f->r[6]*1u);
    /* 0043a9c2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a9c3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a9c4 */
    f->r[1]=0x00f55bb0u;
    /* 0043a9c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a9ceu;f->boundary(0x0043a9c9u,0x0053f250u,f);
    /* 0043a9ce */
    goto mi_0043aae3;
mi_0043a9d3:;
    /* 0043a9d3 */
    f->r[2]=mi_read((0x0070e488u),4u);
    /* 0043a9d9 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0043a9de */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00b285ccu+f->r[2]),1u))&0xffu)<<0);
    /* 0043a9e4 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0043a9e7 */
    if(!(f->flags&MI_ZF))goto mi_0043a9fb;
    /* 0043a9e9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a9ea */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a9eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043a9ec */
    f->r[1]=0x00f55bb0u;
    /* 0043a9f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043a9f6u;f->boundary(0x0043a9f1u,0x0053f250u,f);
    /* 0043a9f6 */
    goto mi_0043aae3;
mi_0043a9fb:;
    /* 0043a9fb */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043a9fe */
    if(!(f->flags&MI_ZF))goto mi_0043aa26;
    /* 0043aa00 */
    f->r[5]=mi_read((0x0070e44cu),4u);
    /* 0043aa06 */
    f->r[2]=mi_read((0x0070e45cu),4u);
    /* 0043aa0c */
    f->r[1]=f->r[7];
    /* 0043aa0e */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043aa10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa11 */
    f->r[1]=(0x00000001u+f->r[2]+f->r[6]*1u);
    /* 0043aa15 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa16 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa17 */
    f->r[1]=0x00f55bb0u;
    /* 0043aa1c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043aa21u;f->boundary(0x0043aa1cu,0x0053f250u,f);
    /* 0043aa21 */
    goto mi_0043aae3;
mi_0043aa26:;
    /* 0043aa26 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043aa29 */
    if(!(f->flags&MI_ZF))goto mi_0043aa51;
    /* 0043aa2b */
    f->r[1]=mi_read((0x0070e448u),4u);
    /* 0043aa31 */
    f->r[2]=f->r[7];
    /* 0043aa33 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043aa35 */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 0043aa3b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa3c */
    f->r[2]=(0x00000002u+f->r[1]+f->r[6]*1u);
    /* 0043aa40 */
    f->r[1]=0x00f55bb0u;
    /* 0043aa45 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa46 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043aa4cu;f->boundary(0x0043aa47u,0x0053f250u,f);
    /* 0043aa4c */
    goto mi_0043aae3;
mi_0043aa51:;
    /* 0043aa51 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043aa54 */
    if(!(f->flags&MI_ZF))goto mi_0043aa79;
    /* 0043aa56 */
    f->r[5]=mi_read((0x0070e444u),4u);
    /* 0043aa5c */
    f->r[2]=mi_read((0x0070e454u),4u);
    /* 0043aa62 */
    f->r[1]=f->r[7];
    /* 0043aa64 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043aa66 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa67 */
    f->r[1]=(0x00000003u+f->r[2]+f->r[6]*1u);
    /* 0043aa6b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa6c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa6d */
    f->r[1]=0x00f55bb0u;
    /* 0043aa72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043aa77u;f->boundary(0x0043aa72u,0x0053f250u,f);
    /* 0043aa77 */
    goto mi_0043aae3;
mi_0043aa79:;
    /* 0043aa79 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043aa7c */
    if(!(f->flags&MI_ZF))goto mi_0043aaa1;
    /* 0043aa7e */
    f->r[1]=mi_read((0x0070e440u),4u);
    /* 0043aa84 */
    f->r[2]=f->r[7];
    /* 0043aa86 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043aa88 */
    f->r[1]=mi_read((0x0070e450u),4u);
    /* 0043aa8e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa8f */
    f->r[2]=(0x00000004u+f->r[1]+f->r[6]*1u);
    /* 0043aa93 */
    f->r[1]=0x00f55bb0u;
    /* 0043aa98 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa99 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aa9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043aa9fu;f->boundary(0x0043aa9au,0x0053f250u,f);
    /* 0043aa9f */
    goto mi_0043aae3;
mi_0043aaa1:;
    /* 0043aaa1 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0043aaa4 */
    if(!(f->flags&MI_ZF))goto mi_0043aae3;
    /* 0043aaa6 */
    f->r[3]=mi_read((0x0070e440u),4u);
    /* 0043aaac */
    f->r[2]=mi_read((0x0070e44cu),4u);
    /* 0043aab2 */
    f->r[1]=f->r[7];
    /* 0043aab4 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043aab6 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043aab8 */
    f->r[2]=mi_read((0x0070e450u),4u);
    /* 0043aabe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aabf */
    f->r[1]=mi_read((0x0070e45cu),4u);
    /* 0043aac5 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043aac7 */
    f->r[1]=0x00f55bb0u;
    /* 0043aacc */
    f->r[2]=(0x00000005u+f->r[2]+f->r[6]*1u);
    /* 0043aad0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aad1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aad2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043aad7u;f->boundary(0x0043aad2u,0x0053f250u,f);
    /* 0043aad7 */
    goto mi_0043aae3;
mi_0043aad9:;
    /* 0043aad9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aada */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043aadb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043aae0u;f->boundary(0x0043aadbu,0x00432820u,f);
mi_0043aae0:;
    /* 0043aae0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043aae3:;
    /* 0043aae3 */
    f->r[0]=mi_read((0x0070e458u),4u);
    /* 0043aae8 */
    f->r[1]=mi_read((0x0070e464u),4u);
    /* 0043aaee */
    f->r[3]=mi_read((0x0070e480u),4u);
    /* 0043aaf4 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043aaf7 */
    f->r[6]=(0x00000002u+f->r[6]+f->r[0]*1u);
    /* 0043aafb */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0043aaff */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043ab00 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0043ab01 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043ab03 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043ab07 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[3]);
    /* 0043ab0d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043a0a2;
mi_0043ab13:;
    /* 0043ab13 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0043ab17 */
    f->r[3]=mi_read((0x0070e490u),4u);
    /* 0043ab1d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043ab1f */
    if((f->flags&MI_ZF))goto mi_0043ab28;
    /* 0043ab21 */
    a=f->r[3];b=mi_read((0x0070e45cu),4u);t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0043ab27 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
mi_0043ab28:;
    /* 0043ab28 */
    f->r[1]=mi_read((0x0070e458u),4u);
    /* 0043ab2e */
    f->r[0]=mi_read((0x0070e478u),4u);
    /* 0043ab33 */
    f->r[2]=0xfffffffeu;
    /* 0043ab38 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0043ab40 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043ab42 */
    f->r[1]=(0xfffffffcu+f->r[0]);
    /* 0043ab45 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[1]);
    /* 0043ab4b */
    f->r[3]=(0x00000000u+f->r[3]+f->r[2]*4u);
    /* 0043ab4e */
    f->r[2]=mi_read((0x0070e464u),4u);
    /* 0043ab54 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043ab57 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0043ab59 */
    if(mi_signed_less(f->flags))goto mi_0043b4b0;
mi_0043ab5f:;
    /* 0043ab5f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0043ab61 */
    if(mi_signed_less(f->flags))goto mi_0043b47f;
    /* 0043ab67 */
    a=f->r[1];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 0043ab6d */
    if(!mi_signed_less(f->flags))goto mi_0043b47f;
    /* 0043ab73 */
    f->r[0]=mi_read((0x0070e47cu),4u);
    /* 0043ab78 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0043ab7b */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0043ab7e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0043ab81 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0043ab84 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043ab86 */
    f->r[0]=mi_read((0x0070e494u+f->r[0]*4u),4u);
    /* 0043ab8d */
    a=f->r[0];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 0043ab92 */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[0]);
    /* 0043ab97 */
    if(!mi_signed_less(f->flags))goto mi_0043b47f;
    /* 0043ab9d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[0]*2u),2u);
    /* 0043aba5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0043aba7 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043abab */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0043abad */
    if((f->flags&MI_ZF))goto mi_0043abc6;
    /* 0043abaf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043abb0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043abb5u;f->boundary(0x0043abb0u,0x00431690u,f);
    /* 0043abb5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043abb8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043abba */
    if((f->flags&MI_ZF))goto mi_0043abc1;
    /* 0043abbc */
    f->r[5]=0x00000001u;
mi_0043abc1:;
    /* 0043abc1 */
    f->r[0]=mi_read((0x0070e488u),4u);
mi_0043abc6:;
    /* 0043abc6 */
    f->r[6]=mi_read((0x00b7482cu+f->r[0]*4u),4u);
    /* 0043abcd */
    f->r[2]=mi_read((0x0126517cu),4u);
    /* 0043abd3 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043abd5 */
    f->r[1]=f->r[6];
    /* 0043abd7 */
    if(mi_signed_less(f->flags))goto mi_0043abdb;
    /* 0043abd9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043abdb:;
    /* 0043abdb */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043abde */
    a=mi_read((0x010d6a16u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0043abe6 */
    if((f->flags&MI_ZF))goto mi_0043b47f;
    /* 0043abec */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0043abee */
    if((f->flags&MI_ZF))goto mi_0043b47f;
    /* 0043abf4 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0043abf8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0043abfa */
    f->r[5]=f->r[2];
    /* 0043abfc */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00b67d1cu+f->r[0]),1u))&0xffu)<<0);
    /* 0043ac02 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0043ac05 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0043ac07 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043ac0b */
    a=f->r[5];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0043ac0e */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043ac12 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[5]),2u))&0xffffu)<<0);
    /* 0043ac19 */
    a=(f->r[2]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0043ac1d */
    if((f->flags&MI_ZF))goto mi_0043ac26;
    /* 0043ac1f */
    a=(f->r[2]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 0043ac24 */
    if(!(f->flags&MI_ZF))goto mi_0043ac4b;
mi_0043ac26:;
    /* 0043ac26 */
    f->r[1]=mi_read((0x00f566acu),4u);
    /* 0043ac2c */
    f->r[2]=(0xffffffd6u+f->r[7]);
    /* 0043ac2f */
    f->r[0]=(0xfffffffcu+f->r[3]);
    /* 0043ac32 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ac33 */
    a=f->r[1];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043ac36 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ac37 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ac38 */
    f->r[1]=0x00f55bb0u;
    /* 0043ac3d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ac42u;f->boundary(0x0043ac3du,0x0053e590u,f);
    /* 0043ac42 */
    f->r[0]=mi_read((0x0070e488u),4u);
    /* 0043ac47 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
mi_0043ac4b:;
    /* 0043ac4b */
    a=(f->r[1]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0043ac4e */
    if((f->flags&MI_ZF))goto mi_0043b47f;
    /* 0043ac54 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[5]),2u))&0xffffu)<<0);
    /* 0043ac5b */
    a=(f->r[5]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0043ac5f */
    if((f->flags&MI_ZF))goto mi_0043b460;
    /* 0043ac65 */
    a=(f->r[5]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 0043ac6a */
    if((f->flags&MI_ZF))goto mi_0043b460;
    /* 0043ac70 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ac71 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ac72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ac77u;f->boundary(0x0043ac72u,0x0040f2c0u,f);
    /* 0043ac77 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043ac7a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043ac7c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043b470;
    /* 0043ac82 */
    f->r[2]=mi_read((0x0126517cu),4u);
    /* 0043ac88 */
    f->r[5]=f->r[0];
    /* 0043ac8a */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043ac8c */
    f->r[1]=f->r[6];
    /* 0043ac8e */
    if(mi_signed_less(f->flags))goto mi_0043ac92;
    /* 0043ac90 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043ac92:;
    /* 0043ac92 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043ac95 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a16u+f->r[1]),2u);
    /* 0043ac9c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043ac9e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043acb2;
    /* 0043aca0 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043aca2 */
    f->r[0]=f->r[6];
    /* 0043aca4 */
    if(mi_signed_less(f->flags))goto mi_0043aca8;
    /* 0043aca6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043aca8:;
    /* 0043aca8 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043acab */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a16u+f->r[0]),2u);
mi_0043acb2:;
    /* 0043acb2 */
    f->r[2]=mi_read((0x0070e488u),4u);
    /* 0043acb8 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0043acbc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043acbd */
    f->r[1]=0x00bca350u;
    /* 0043acc2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00b285ccu+f->r[2]),1u))&0xffu)<<0);
    /* 0043acc8 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043accb */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043accf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043acd4u;f->boundary(0x0043accfu,0x00488050u,f);
    /* 0043acd4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043acd6 */
    if((f->flags&MI_ZF))goto mi_0043acef;
    /* 0043acd8 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0043acdc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043acdd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043acde */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043acdf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ace0 */
    f->r[1]=0x00bca350u;
    /* 0043ace5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043aceau;f->boundary(0x0043ace5u,0x00489500u,f);
    /* 0043acea */
    goto mi_0043b470;
mi_0043acef:;
    /* 0043acef */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0043acf3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[1]),2u);
    /* 0043acfa */
    a=f->r[0];b=0xffffffe2u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043acfd */
    a=f->r[0];b=0x000000bfu;t=mi_alu(f,a,b,32,1);
    /* 0043ad02 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0043b277;
    /* 0043ad08 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0043ad0a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0043b55cu+f->r[0]),1u))&0xffu)<<0);
    /* 0043ad10 */
    switch(mi_read((0x0043b524u+f->r[2]*4u),4u)){case 0x0043ad17u:goto mi_0043ad17;case 0x0043ad62u:goto mi_0043ad62;case 0x0043add2u:goto mi_0043add2;case 0x0043ae5bu:goto mi_0043ae5b;case 0x0043af63u:goto mi_0043af63;case 0x0043b044u:goto mi_0043b044;case 0x0043b159u:goto mi_0043b159;case 0x0043b176u:goto mi_0043b176;case 0x0043b1aau:goto mi_0043b1aa;case 0x0043b200u:goto mi_0043b200;case 0x0043b21eu:goto mi_0043b21e;case 0x0043b23cu:goto mi_0043b23c;case 0x0043b259u:goto mi_0043b259;case 0x0043b277u:goto mi_0043b277;default:abort();}
mi_0043ad17:;
    /* 0043ad17 */
    f->r[0]=mi_read((0x00f56f34u),4u);
    /* 0043ad1c */
    f->r[1]=0x00f55bb0u;
    /* 0043ad21 */
    f->r[5]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 0043ad25 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ad26 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ad2bu;f->boundary(0x0043ad26u,0x00438180u,f);
    /* 0043ad2b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000022u+f->r[0]),2u);
    /* 0043ad2f */
    f->r[6]=0x00000071u;
    /* 0043ad34 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ad35 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043ad37 */
    f->r[1]=0x00f55bb0u;
    /* 0043ad3c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ad41u;f->boundary(0x0043ad3cu,0x00438180u,f);
    /* 0043ad41 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 0043ad45 */
    f->r[0]=0x00000003u;
    /* 0043ad4a */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043ad4c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043ad4e */
    f->r[1]=0x00f55bb0u;
    /* 0043ad53 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043ad55 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ad56 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ad57 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ad58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ad5du;f->boundary(0x0043ad58u,0x0053e590u,f);
    /* 0043ad5d */
    goto mi_0043b470;
mi_0043ad62:;
    /* 0043ad62 */
    f->r[0]=mi_read((0x00f56f54u),4u);
    /* 0043ad67 */
    f->r[1]=0x00f55bb0u;
    /* 0043ad6c */
    f->r[5]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 0043ad70 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ad71 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ad76u;f->boundary(0x0043ad71u,0x00438180u,f);
    /* 0043ad76 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000022u+f->r[0]),2u);
    /* 0043ad7a */
    f->r[6]=0x00000016u;
    /* 0043ad7f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ad80 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043ad82 */
    f->r[1]=0x00f55bb0u;
    /* 0043ad87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ad8cu;f->boundary(0x0043ad87u,0x00438180u,f);
    /* 0043ad8c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[0]),2u);
    /* 0043ad90 */
    f->r[0]=0x00000018u;
    /* 0043ad95 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043ad97 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043ad99 */
    f->r[1]=0x00f55bb0u;
    /* 0043ad9e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043ada0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ada1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ada2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ada3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ada8u;f->boundary(0x0043ada3u,0x0053e590u,f);
    /* 0043ada8 */
    a=0x0000015fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043adad */
    f->r[1]=0x00f55bb0u;
    /* 0043adb2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043adb7u;f->boundary(0x0043adb2u,0x004381a0u,f);
    /* 0043adb7 */
    f->r[1]=(0xffffffd6u+f->r[7]);
    /* 0043adba */
    f->r[2]=(0xfffffffcu+f->r[3]);
    /* 0043adbd */
    a=f->r[0];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043adc0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043adc1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043adc2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043adc3 */
    f->r[1]=0x00f55bb0u;
    /* 0043adc8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043adcdu;f->boundary(0x0043adc8u,0x0053e590u,f);
    /* 0043adcd */
    goto mi_0043b470;
mi_0043add2:;
    /* 0043add2 */
    a=f->r[6];b=mi_read((0x00f56cccu),4u);t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043add8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0043adda */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0043addc */
    f->r[0]=0x00000242u;
    /* 0043ade1 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0043ade4 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043ade7 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0043ae2c;
    /* 0043ade9 */
    switch(mi_read((0x0043b61cu+f->r[6]*4u),4u)){case 0x0043adf0u:goto mi_0043adf0;case 0x0043adfeu:goto mi_0043adfe;case 0x0043ae0fu:goto mi_0043ae0f;case 0x0043ae1du:goto mi_0043ae1d;default:abort();}
mi_0043adf0:;
    /* 0043adf0 */
    f->r[0]=0x00000242u;
    /* 0043adf5 */
    f->r[1]=0x00000004u;
    /* 0043adfa */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0043adfc */
    goto mi_0043ae2c;
mi_0043adfe:;
    /* 0043adfe */
    f->r[0]=0x00000243u;
    /* 0043ae03 */
    f->r[1]=0x00000003u;
    /* 0043ae08 */
    f->r[2]=0xfffffffeu;
    /* 0043ae0d */
    goto mi_0043ae2c;
mi_0043ae0f:;
    /* 0043ae0f */
    f->r[0]=0x00000244u;
    /* 0043ae14 */
    f->r[1]=0x0000000cu;
    /* 0043ae19 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0043ae1b */
    goto mi_0043ae2c;
mi_0043ae1d:;
    /* 0043ae1d */
    f->r[0]=0x00000245u;
    /* 0043ae22 */
    f->r[1]=0x0000000fu;
    /* 0043ae27 */
    f->r[2]=0x00000006u;
mi_0043ae2c:;
    /* 0043ae2c */
    f->r[6]=mi_read((0x0070e44cu),4u);
    /* 0043ae32 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043ae34 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ae35 */
    f->r[2]=mi_read((0x0070e45cu),4u);
    /* 0043ae3b */
    f->r[1]=(0xfffffff8u+f->r[1]+f->r[2]*2u);
    /* 0043ae3f */
    f->r[2]=mi_read((0x00f55bb4u+f->r[0]*8u),4u);
    /* 0043ae46 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ae47 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ae48 */
    f->r[0]=(0xffffffffu+f->r[2]+f->r[5]*1u);
    /* 0043ae4c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ae4d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043ae4e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043ae53u;f->boundary(0x0043ae4eu,0x004381b0u,f);
    /* 0043ae53 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043ae56 */
    goto mi_0043b470;
mi_0043ae5b:;
    /* 0043ae5b */
    f->r[2]=mi_read((0x00f56cc4u),4u);
    /* 0043ae61 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043ae65 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8cu+f->r[1]),2u);
    /* 0043ae6c */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043ae6e */
    f->r[2]=0x00000278u;
    /* 0043ae73 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0043ae76 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(0x00000084u);
    /* 0043ae7e */
    MI_U32((0x0000004cu+f->r[4]))=(uint32_t)(0xfffffff9u);
    /* 0043ae86 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(0x00000073u);
    /* 0043ae8e */
    f->r[0]=(0xffffffffu+f->r[6]);
    /* 0043ae91 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(0x00000019u);
    /* 0043ae99 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043ae9c */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(0x0000003cu);
    /* 0043aea4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0043aea6 */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(0x00000014u);
    /* 0043aeae */
    MI_U32((0x00000060u+f->r[4]))=(uint32_t)(0x00000031u);
    /* 0043aeb6 */
    MI_U32((0x00000064u+f->r[4]))=(uint32_t)(0xfffffffcu);
    /* 0043aebe */
    MI_U32((0x00000068u+f->r[4]))=(uint32_t)(0x00000098u);
    /* 0043aec6 */
    MI_U32((0x0000006cu+f->r[4]))=(uint32_t)(0xfffffff3u);
    /* 0043aece */
    MI_U32((0x00000070u+f->r[4]))=(uint32_t)(0x0000007bu);
    /* 0043aed6 */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(0x00000010u);
    /* 0043aede */
    MI_U32((0x00000078u+f->r[4]))=(uint32_t)(0x00000041u);
    /* 0043aee6 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(0x0000000eu);
    /* 0043aeee */
    MI_U32((0x00000080u+f->r[4]))=(uint32_t)(0x00000026u);
    /* 0043aef9 */
    MI_U32((0x00000084u+f->r[4]))=(uint32_t)(0xfffffff4u);
    /* 0043af04 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043af08 */
    if((f->flags&MI_ZF))goto mi_0043af37;
    /* 0043af0a */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0043af0d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043af10 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043af12 */
    a=mi_read((0x00bcd630u+f->r[2]*4u),1u);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0043af1a */
    if(!(f->flags&MI_ZF))goto mi_0043af33;
    /* 0043af1c */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043af1f */
    a=f->r[5];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0043af22 */
    f->r[2]=0x00000279u;
    /* 0043af27 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043af37;
    /* 0043af29 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000014u);
    /* 0043af31 */
    goto mi_0043af37;
mi_0043af33:;
    /* 0043af33 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
mi_0043af37:;
    /* 0043af37 */
    f->r[1]=mi_read((0x0000004cu+f->r[4]+f->r[6]*8u),4u);
    /* 0043af3b */
    f->r[2]=mi_read((0x00f55bb4u+f->r[2]*8u),4u);
    /* 0043af42 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043af43 */
    f->r[1]=mi_read((0x0000004cu+f->r[4]+f->r[6]*8u),4u);
    /* 0043af47 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043af48 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0043af4c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043af4d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043af4e */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0043af51 */
    f->r[0]=(0xfffffffcu+f->r[2]+f->r[0]*1u);
    /* 0043af55 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043af56 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043af5bu;f->boundary(0x0043af56u,0x004381b0u,f);
    /* 0043af5b */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043af5e */
    goto mi_0043b470;
mi_0043af63:;
    /* 0043af63 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043af65 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00799a8cu+f->r[1]),2u))&0xffffu)<<0);
    /* 0043af6c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043af6e */
    if((f->flags&MI_ZF))goto mi_0043b470;
    /* 0043af74 */
    f->r[1]=f->r[0];
    /* 0043af76 */
    a=f->r[0];b=0x00000fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043af7b */
    a=f->r[1];b=0x0000f000u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0043af81 */
    a=f->r[1];b=0x00001000u;t=mi_alu(f,a,b,32,1);
    /* 0043af87 */
    if(!(f->flags&MI_ZF))goto mi_0043af9a;
    /* 0043af89 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043af8c */
    if((f->flags&MI_ZF))goto mi_0043af93;
    /* 0043af8e */
    a=f->r[0];b=0x00000034u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043af91 */
    goto mi_0043afd2;
mi_0043af93:;
    /* 0043af93 */
    f->r[0]=0x00000277u;
    /* 0043af98 */
    goto mi_0043afe1;
mi_0043af9a:;
    /* 0043af9a */
    a=f->r[1];b=0x00002000u;t=mi_alu(f,a,b,32,1);
    /* 0043afa0 */
    if(!(f->flags&MI_ZF))goto mi_0043afdc;
    /* 0043afa2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0043afa5 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043afa8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043afaa */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043afad */
    a=mi_read((0x00bcd606u+f->r[1]),1u);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0043afb4 */
    if(!(f->flags&MI_ZF))goto mi_0043b470;
    /* 0043afba */
    a=mi_read((0x00bcd630u+f->r[1]),1u);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0043afc1 */
    if((f->flags&MI_ZF))goto mi_0043b470;
    /* 0043afc7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043afc9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[1]),1u))&0xffu)<<0);
    /* 0043afcf */
    a=f->r[0];b=0x0000004du;t=mi_alu(f,a,b,32,1);f->r[0]=t;
mi_0043afd2:;
    /* 0043afd2 */
    if(!(f->flags&MI_ZF))goto mi_0043afdb;
    /* 0043afd4 */
    f->r[0]=0x00000275u;
    /* 0043afd9 */
    goto mi_0043afe1;
mi_0043afdb:;
    /* 0043afdb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0043afdc:;
    /* 0043afdc */
    f->r[0]=0x0000026eu;
mi_0043afe1:;
    /* 0043afe1 */
    f->r[1]=mi_read((0x00f56c0cu),4u);
    /* 0043afe7 */
    f->r[0]=mi_read((0x00f55bb4u+f->r[0]*8u),4u);
    /* 0043afee */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043aff0 */
    f->r[1]=0x00000014u;
    /* 0043aff5 */
    f->r[2]=0x00000055u;
    /* 0043affa */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x0000006eu);
    /* 0043b002 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043b006 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043b00a */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043b00e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043b012 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043b016 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(0x00000064u);
    /* 0043b01e */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(0x0000000au);
    /* 0043b026 */
    f->r[1]=mi_read((0x0000002cu+f->r[4]+f->r[6]*8u),4u);
    /* 0043b02a */
    f->r[2]=mi_read((0x00000028u+f->r[4]+f->r[6]*8u),4u);
    /* 0043b02e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b02f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b030 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b031 */
    f->r[1]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 0043b035 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b036 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b037 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b03cu;f->boundary(0x0043b037u,0x004381b0u,f);
    /* 0043b03c */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b03f */
    goto mi_0043b470;
mi_0043b044:;
    /* 0043b044 */
    f->r[0]=mi_read((0x00f56bccu),4u);
    /* 0043b049 */
    f->r[2]=0x0000000au;
    /* 0043b04e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8cu+f->r[1]),2u);
    /* 0043b055 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043b057 */
    f->r[0]=0x0000004bu;
    /* 0043b05c */
    MI_U32((0x00000088u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043b063 */
    MI_U32((0x000000a8u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043b06a */
    f->r[0]=(0xffffffffu+f->r[6]);
    /* 0043b06d */
    MI_U32((0x0000008cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043b074 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0043b077 */
    MI_U32((0x000000acu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043b07e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0043b080 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043b084 */
    MI_U32((0x00000090u+f->r[4]))=(uint32_t)(0x0000004eu);
    /* 0043b08f */
    MI_U32((0x00000094u+f->r[4]))=(uint32_t)(0x00000007u);
    /* 0043b09a */
    MI_U32((0x00000098u+f->r[4]))=(uint32_t)(0x0000003eu);
    /* 0043b0a5 */
    MI_U32((0x0000009cu+f->r[4]))=(uint32_t)(0x00000012u);
    /* 0043b0b0 */
    MI_U32((0x000000a0u+f->r[4]))=(uint32_t)(0x00000030u);
    /* 0043b0bb */
    MI_U32((0x000000a4u+f->r[4]))=(uint32_t)(0x00000008u);
    /* 0043b0c6 */
    MI_U32((0x000000b0u+f->r[4]))=(uint32_t)(0x0000004eu);
    /* 0043b0d1 */
    MI_U32((0x000000b4u+f->r[4]))=(uint32_t)(0x00000007u);
    /* 0043b0dc */
    MI_U32((0x000000b8u+f->r[4]))=(uint32_t)(0x0000003eu);
    /* 0043b0e7 */
    MI_U32((0x000000bcu+f->r[4]))=(uint32_t)(0x00000012u);
    /* 0043b0f2 */
    MI_U32((0x000000c0u+f->r[4]))=(uint32_t)(0x00000030u);
    /* 0043b0fd */
    MI_U32((0x000000c4u+f->r[4]))=(uint32_t)(0x00000008u);
    /* 0043b108 */
    f->r[2]=0x00000278u;
    /* 0043b10d */
    if((f->flags&MI_ZF))goto mi_0043b12b;
    /* 0043b10f */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0043b112 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043b115 */
    a=f->r[1];b=mi_read((0x0000001cu+f->r[4]),4u);t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043b119 */
    a=mi_read((0x00bcd630u+f->r[1]*4u),1u);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0043b121 */
    if(!(f->flags&MI_ZF))goto mi_0043b12b;
    /* 0043b123 */
    f->r[2]=0x0000026fu;
    /* 0043b128 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_0043b12b:;
    /* 0043b12b */
    f->r[1]=mi_read((0x0000008cu+f->r[4]+f->r[6]*8u),4u);
    /* 0043b132 */
    f->r[2]=mi_read((0x00f55bb4u+f->r[2]*8u),4u);
    /* 0043b139 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b13a */
    f->r[1]=mi_read((0x0000008cu+f->r[4]+f->r[6]*8u),4u);
    /* 0043b141 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b142 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[5]*4u);
    /* 0043b145 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b146 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b147 */
    f->r[2]=(0xfffffffcu+f->r[1]+f->r[0]*1u);
    /* 0043b14b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b14c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b151u;f->boundary(0x0043b14cu,0x004381b0u,f);
    /* 0043b151 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b154 */
    goto mi_0043b470;
mi_0043b159:;
    /* 0043b159 */
    f->r[0]=mi_read((0x00f56f3cu),4u);
    /* 0043b15e */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b160 */
    a=0x00000036u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b162 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b163 */
    f->r[1]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 0043b167 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b168 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b169 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b16eu;f->boundary(0x0043b169u,0x004381b0u,f);
    /* 0043b16e */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b171 */
    goto mi_0043b470;
mi_0043b176:;
    /* 0043b176 */
    f->r[2]=mi_read((0x00f56f44u),4u);
    /* 0043b17c */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b17e */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b180 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b181 */
    f->r[0]=(0xffffffffu+f->r[2]+f->r[5]*1u);
    /* 0043b185 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b186 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b187 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b18cu;f->boundary(0x0043b187u,0x004381b0u,f);
    /* 0043b18c */
    f->r[1]=mi_read((0x00f56f4cu),4u);
    /* 0043b192 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b194 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b196 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b197 */
    f->r[2]=(0xffffffffu+f->r[1]+f->r[5]*1u);
    /* 0043b19b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b19c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b19d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b1a2u;f->boundary(0x0043b19du,0x004381b0u,f);
    /* 0043b1a2 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b1a5 */
    goto mi_0043b470;
mi_0043b1aa:;
    /* 0043b1aa */
    a=f->r[6];b=mi_read((0x00f566fcu),4u);t=mi_alu(f,a,b,32,1);
    /* 0043b1b0 */
    if((f->flags&MI_ZF))goto mi_0043b1d9;
    /* 0043b1b2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1b4 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1b6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1b7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1b8 */
    a=0x0000024bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1bd */
    f->r[1]=0x00f55bb0u;
    /* 0043b1c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b1c7u;f->boundary(0x0043b1c2u,0x004381a0u,f);
    /* 0043b1c7 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 0043b1cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b1d1u;f->boundary(0x0043b1ccu,0x004381b0u,f);
    /* 0043b1d1 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b1d4 */
    goto mi_0043b470;
mi_0043b1d9:;
    /* 0043b1d9 */
    a=0xfffffffdu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1db */
    a=0x00000034u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1dd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1de */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1df */
    a=0x0000024au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1e4 */
    f->r[1]=0x00f55bb0u;
    /* 0043b1e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b1eeu;f->boundary(0x0043b1e9u,0x004381a0u,f);
    /* 0043b1ee */
    f->r[1]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 0043b1f2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b1f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b1f8u;f->boundary(0x0043b1f3u,0x004381b0u,f);
    /* 0043b1f8 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b1fb */
    goto mi_0043b470;
mi_0043b200:;
    /* 0043b200 */
    f->r[2]=mi_read((0x00f56e0cu),4u);
    /* 0043b206 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b208 */
    a=0x00000052u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b20a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b20b */
    f->r[0]=(0xffffffffu+f->r[2]+f->r[5]*1u);
    /* 0043b20f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b210 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b211 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b216u;f->boundary(0x0043b211u,0x004381b0u,f);
    /* 0043b216 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b219 */
    goto mi_0043b470;
mi_0043b21e:;
    /* 0043b21e */
    f->r[1]=mi_read((0x00f56df4u),4u);
    /* 0043b224 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b226 */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b228 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b229 */
    f->r[2]=(0xffffffffu+f->r[1]+f->r[5]*1u);
    /* 0043b22d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b22e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b22f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b234u;f->boundary(0x0043b22fu,0x004381b0u,f);
    /* 0043b234 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b237 */
    goto mi_0043b470;
mi_0043b23c:;
    /* 0043b23c */
    f->r[0]=mi_read((0x00f56decu),4u);
    /* 0043b241 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b243 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b245 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b246 */
    f->r[1]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 0043b24a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b24b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b24c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b251u;f->boundary(0x0043b24cu,0x004381b0u,f);
    /* 0043b251 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b254 */
    goto mi_0043b470;
mi_0043b259:;
    /* 0043b259 */
    f->r[2]=mi_read((0x00f56decu),4u);
    /* 0043b25f */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b261 */
    a=0x0000006cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b263 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b264 */
    f->r[0]=(0xffffffffu+f->r[2]+f->r[5]*1u);
    /* 0043b268 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b269 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b26a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b26fu;f->boundary(0x0043b26au,0x004381b0u,f);
    /* 0043b26f */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b272 */
    goto mi_0043b470;
mi_0043b277:;
    /* 0043b277 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0043b27b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043b27d */
    if(!(f->flags&MI_ZF))goto mi_0043b2bf;
    /* 0043b27f */
    f->r[0]=mi_read((0x0126517cu),4u);
    /* 0043b284 */
    f->r[2]=f->r[6];
    /* 0043b286 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b288 */
    if(mi_signed_less(f->flags))goto mi_0043b28c;
    /* 0043b28a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0043b28c:;
    /* 0043b28c */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b28e */
    f->r[1]=f->r[6];
    /* 0043b290 */
    if(mi_signed_less(f->flags))goto mi_0043b294;
    /* 0043b292 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043b294:;
    /* 0043b294 */
    a=f->r[6];b=mi_read((0x0126517cu),4u);t=mi_alu(f,a,b,32,1);
    /* 0043b29a */
    f->r[0]=f->r[6];
    /* 0043b29c */
    if(mi_signed_less(f->flags))goto mi_0043b2a0;
    /* 0043b29e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043b2a0:;
    /* 0043b2a0 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043b2a3 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043b2a6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a1cu+f->r[1]),2u);
    /* 0043b2ad */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a0eu+f->r[2]),2u);
    /* 0043b2b4 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b2b6 */
    f->r[1]=(0x0000001eu+f->r[1]+f->r[7]*1u);
    /* 0043b2ba */
    goto mi_0043b3ee;
mi_0043b2bf:;
    /* 0043b2bf */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043b2c2 */
    if(!(f->flags&MI_ZF))goto mi_0043b30c;
    /* 0043b2c4 */
    f->r[0]=mi_read((0x0126517cu),4u);
    /* 0043b2c9 */
    f->r[2]=f->r[6];
    /* 0043b2cb */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b2cd */
    if(mi_signed_less(f->flags))goto mi_0043b2d1;
    /* 0043b2cf */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0043b2d1:;
    /* 0043b2d1 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b2d3 */
    f->r[1]=f->r[6];
    /* 0043b2d5 */
    if(mi_signed_less(f->flags))goto mi_0043b2d9;
    /* 0043b2d7 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043b2d9:;
    /* 0043b2d9 */
    a=f->r[6];b=mi_read((0x0126517cu),4u);t=mi_alu(f,a,b,32,1);
    /* 0043b2df */
    f->r[0]=f->r[6];
    /* 0043b2e1 */
    if(mi_signed_less(f->flags))goto mi_0043b2e5;
    /* 0043b2e3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043b2e5:;
    /* 0043b2e5 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043b2e8 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043b2eb */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a1cu+f->r[1]),2u);
    /* 0043b2f2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a0eu+f->r[2]),2u);
    /* 0043b2f9 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b2fb */
    f->r[2]=mi_read((0x0070e44cu),4u);
    /* 0043b301 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b303 */
    f->r[1]=(0x0000003cu+f->r[1]+f->r[7]*1u);
    /* 0043b307 */
    goto mi_0043b3ee;
mi_0043b30c:;
    /* 0043b30c */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043b30f */
    if(!(f->flags&MI_ZF))goto mi_0043b359;
    /* 0043b311 */
    f->r[0]=mi_read((0x0126517cu),4u);
    /* 0043b316 */
    f->r[2]=f->r[6];
    /* 0043b318 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b31a */
    if(mi_signed_less(f->flags))goto mi_0043b31e;
    /* 0043b31c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0043b31e:;
    /* 0043b31e */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b320 */
    f->r[1]=f->r[6];
    /* 0043b322 */
    if(mi_signed_less(f->flags))goto mi_0043b326;
    /* 0043b324 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043b326:;
    /* 0043b326 */
    a=f->r[6];b=mi_read((0x0126517cu),4u);t=mi_alu(f,a,b,32,1);
    /* 0043b32c */
    f->r[0]=f->r[6];
    /* 0043b32e */
    if(mi_signed_less(f->flags))goto mi_0043b332;
    /* 0043b330 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043b332:;
    /* 0043b332 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043b335 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043b338 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a1cu+f->r[1]),2u);
    /* 0043b33f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a0eu+f->r[2]),2u);
    /* 0043b346 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b348 */
    f->r[2]=mi_read((0x0070e448u),4u);
    /* 0043b34e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b350 */
    f->r[1]=(0x0000005au+f->r[1]+f->r[7]*1u);
    /* 0043b354 */
    goto mi_0043b3ee;
mi_0043b359:;
    /* 0043b359 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043b35c */
    if(!(f->flags&MI_ZF))goto mi_0043b3a3;
    /* 0043b35e */
    f->r[0]=mi_read((0x0126517cu),4u);
    /* 0043b363 */
    f->r[2]=f->r[6];
    /* 0043b365 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b367 */
    if(mi_signed_less(f->flags))goto mi_0043b36b;
    /* 0043b369 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0043b36b:;
    /* 0043b36b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b36d */
    f->r[1]=f->r[6];
    /* 0043b36f */
    if(mi_signed_less(f->flags))goto mi_0043b373;
    /* 0043b371 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043b373:;
    /* 0043b373 */
    a=f->r[6];b=mi_read((0x0126517cu),4u);t=mi_alu(f,a,b,32,1);
    /* 0043b379 */
    f->r[0]=f->r[6];
    /* 0043b37b */
    if(mi_signed_less(f->flags))goto mi_0043b37f;
    /* 0043b37d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043b37f:;
    /* 0043b37f */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043b382 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043b385 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a1cu+f->r[1]),2u);
    /* 0043b38c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a0eu+f->r[2]),2u);
    /* 0043b393 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b395 */
    f->r[2]=mi_read((0x0070e444u),4u);
    /* 0043b39b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b39d */
    f->r[1]=(0x00000078u+f->r[1]+f->r[7]*1u);
    /* 0043b3a1 */
    goto mi_0043b3ee;
mi_0043b3a3:;
    /* 0043b3a3 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043b3a6 */
    if(!(f->flags&MI_ZF))goto mi_0043b40b;
    /* 0043b3a8 */
    f->r[0]=mi_read((0x0126517cu),4u);
    /* 0043b3ad */
    f->r[2]=f->r[6];
    /* 0043b3af */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b3b1 */
    if(mi_signed_less(f->flags))goto mi_0043b3b5;
    /* 0043b3b3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0043b3b5:;
    /* 0043b3b5 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b3b7 */
    f->r[1]=f->r[6];
    /* 0043b3b9 */
    if(mi_signed_less(f->flags))goto mi_0043b3bd;
    /* 0043b3bb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043b3bd:;
    /* 0043b3bd */
    a=f->r[6];b=mi_read((0x0126517cu),4u);t=mi_alu(f,a,b,32,1);
    /* 0043b3c3 */
    f->r[0]=f->r[6];
    /* 0043b3c5 */
    if(mi_signed_less(f->flags))goto mi_0043b3c9;
    /* 0043b3c7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043b3c9:;
    /* 0043b3c9 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043b3cc */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043b3cf */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a1cu+f->r[1]),2u);
    /* 0043b3d6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a0eu+f->r[2]),2u);
    /* 0043b3dd */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b3df */
    f->r[2]=mi_read((0x0070e440u),4u);
    /* 0043b3e5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b3e7 */
    f->r[1]=(0x00000096u+f->r[1]+f->r[7]*1u);
mi_0043b3ee:;
    /* 0043b3ee */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043b3f1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b3f2 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0043b3f4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a1au+f->r[0]),2u);
    /* 0043b3fb */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043b3fd */
    f->r[1]=0x00f55bb0u;
    /* 0043b402 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b403 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b404 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b409u;f->boundary(0x0043b404u,0x0053e590u,f);
    /* 0043b409 */
    goto mi_0043b470;
mi_0043b40b:;
    /* 0043b40b */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0043b40e */
    if(!(f->flags&MI_ZF))goto mi_0043b470;
    /* 0043b410 */
    f->r[0]=mi_read((0x0126517cu),4u);
    /* 0043b415 */
    f->r[2]=f->r[6];
    /* 0043b417 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b419 */
    if(mi_signed_less(f->flags))goto mi_0043b41d;
    /* 0043b41b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0043b41d:;
    /* 0043b41d */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043b41f */
    f->r[1]=f->r[6];
    /* 0043b421 */
    if(mi_signed_less(f->flags))goto mi_0043b425;
    /* 0043b423 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043b425:;
    /* 0043b425 */
    a=f->r[6];b=mi_read((0x0126517cu),4u);t=mi_alu(f,a,b,32,1);
    /* 0043b42b */
    f->r[0]=f->r[6];
    /* 0043b42d */
    if(mi_signed_less(f->flags))goto mi_0043b431;
    /* 0043b42f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043b431:;
    /* 0043b431 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043b434 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043b437 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a1cu+f->r[1]),2u);
    /* 0043b43e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x010d6a0eu+f->r[2]),2u);
    /* 0043b445 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b447 */
    f->r[2]=mi_read((0x0070e440u),4u);
    /* 0043b44d */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b44f */
    f->r[2]=mi_read((0x0070e44cu),4u);
    /* 0043b455 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043b457 */
    f->r[1]=(0x000000b4u+f->r[1]+f->r[7]*1u);
    /* 0043b45e */
    goto mi_0043b3ee;
mi_0043b460:;
    /* 0043b460 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b461 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b462 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b463 */
    f->r[0]=mi_read((0x0000002cu+f->r[4]),4u);
    /* 0043b467 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b468 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b46du;f->boundary(0x0043b468u,0x00442280u,f);
    /* 0043b46d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043b470:;
    /* 0043b470 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0043b474 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b475 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b476 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043b477 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043b47cu;f->boundary(0x0043b477u,0x00436310u,f);
    /* 0043b47c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043b47f:;
    /* 0043b47f */
    f->r[2]=mi_read((0x0070e458u),4u);
    /* 0043b485 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0043b489 */
    f->r[1]=mi_read((0x0070e480u),4u);
    /* 0043b48f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043b490 */
    f->r[3]=(0x00000002u+f->r[3]+f->r[2]*1u);
    /* 0043b494 */
    f->r[2]=mi_read((0x0070e464u),4u);
    /* 0043b49a */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043b49d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0043b49e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043b4a0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043b4a4 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[1]);
    /* 0043b4aa */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043ab5f;
mi_0043b4b0:;
    /* 0043b4b0 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0043b4b4 */
    f->r[5]=mi_read((0x0070e44cu),4u);
    /* 0043b4ba */
    f->r[6]=mi_read((0x0070e47cu),4u);
    /* 0043b4c0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043b4c2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0043b4c4 */
    f->r[1]=mi_read((0x0070e460u),4u);
    /* 0043b4ca */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0043b4cd */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043b4d1 */
    f->r[0]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0043b4d5 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0043b4d7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043b4d8 */
    a=f->r[1];b=0x00000012u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043b4db */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043b4dc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043b4de */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043b4e2 */
    MI_U32((0x0070e47cu))=(uint32_t)(f->r[6]);
    /* 0043b4e8 */
    if(mi_signed_less(f->flags))goto mi_00439f54;
    /* 0043b4ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043b4ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043b4f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0043b4f1:;
    /* 0043b4f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043b4f2 */
    a=f->r[4];b=0x000000b8u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043b4f8 */
    f->r[4]+=4;return;
}
