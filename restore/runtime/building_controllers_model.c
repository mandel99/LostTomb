/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "building_controllers.h"
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
static __inline uint32_t mi_alu(struct restore_building_control_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_building_control_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_building_control_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_0046b28c[]={0xff,0xb1,0x46,0x00,0x3f,0xb2,0x46,0x00,0xa6,0xb1,0x46,0x00,0x82,0xb2,0x46,0x00};
static const uint8_t mi_rom_0046b29c[]={0x00,0x01,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00};
static __inline uint32_t mi_read_0046b100(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0046b28cu&&width<=16u&&(offset=address-0x0046b28cu)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0046b28c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0046b29cu&&width<=121u&&(offset=address-0x0046b29cu)<=121u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0046b29c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


static const uint8_t mi_rom_004bfcac[]={0xdc,0xfb,0x4b,0x00,0xe3,0xfb,0x4b,0x00,0xe3,0xfb,0x4b,0x00,0xdc,0xfb,0x4b,0x00,0xdc,0xfb,0x4b,0x00,0xe3,0xfb,0x4b,0x00,0xe3,0xfb,0x4b,0x00,0xdc,0xfb,0x4b,0x00};
static __inline uint32_t mi_read_004bfb30(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004bfcacu&&width<=32u&&(offset=address-0x004bfcacu)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004bfcac[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


static const uint8_t mi_rom_004c3708[]={0x32,0x36,0x4c,0x00,0x45,0x36,0x4c,0x00,0x3b,0x36,0x4c,0x00,0x45,0x36,0x4c,0x00,0x32,0x36,0x4c,0x00,0x45,0x36,0x4c,0x00,0x3b,0x36,0x4c,0x00};
static __inline uint32_t mi_read_004c3420(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004c3708u&&width<=28u&&(offset=address-0x004c3708u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004c3708[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


void restore_building_control_C_0046b100(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046b100 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b101 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b102 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b103 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b108u;f->boundary(0x0046b103u,0x005596d0u,f);
    /* 0046b108 */
    f->r[3]=0x00000001u;
    /* 0046b10d */
    f->r[0]=0x00790184u;
    /* 0046b112 */
    f->r[7]=f->r[3];
    /* 0046b114 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[7]);
mi_0046b118:;
    /* 0046b118 */
    a=mi_read_0046b100((0x00000000u+f->r[0]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0046b11b */
    if((f->flags&MI_ZF))goto mi_0046b134;
    /* 0046b11d */
    a=f->r[0];b=0x000000c4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b122 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046b123 */
    a=f->r[0];b=0x007999e0u;t=mi_alu(f,a,b,32,1);
    /* 0046b128 */
    if(mi_signed_less(f->flags))goto mi_0046b118;
    /* 0046b12a */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0046b12e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b12f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b131 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b132 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0046b133 */
    f->r[4]+=4;return;
mi_0046b134:;
    /* 0046b134 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b135 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b136 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 0046b139 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b13b */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046b13e */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b140 */
    a=0x000000c4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b145 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046b148 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0046b14c */
    f->r[0]=(0x007900b8u+f->r[6]);
    /* 0046b152 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b153 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b158u;f->boundary(0x0046b153u,0x0040f8f0u,f);
    /* 0046b158 */
    f->r[1]=mi_read_0046b100((0x0078d788u),4u);
    /* 0046b15e */
    f->r[0]=mi_read_0046b100((0x00000024u+f->r[4]),4u);
    /* 0046b162 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b165 */
    MI_U8((0x007900c0u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0046b16b */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046b16d */
    MI_U16((0x007900bcu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0046b174 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046b17c;
    /* 0046b176 */
    MI_U32((0x0078d788u))=(uint32_t)(f->r[7]);
mi_0046b17c:;
    /* 0046b17c */
    f->r[1]=f->r[0];
    /* 0046b17e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b181 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b183 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_0046b100((0x007999e8u+f->r[1]*8u),2u);
    /* 0046b18b */
    a=f->r[0];b=0xffffffbau;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b18e */
    a=f->r[0];b=0x00000078u;t=mi_alu(f,a,b,32,1);
    /* 0046b191 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0046b282;
    /* 0046b197 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0046b199 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_0046b100((0x0046b29cu+f->r[0]),1u))&0xffu)<<0);
    /* 0046b19f */
    switch(mi_read_0046b100((0x0046b28cu+f->r[2]*4u),4u)){case 0x0046b1a6u:goto mi_0046b1a6;case 0x0046b1ffu:goto mi_0046b1ff;case 0x0046b23fu:goto mi_0046b23f;case 0x0046b282u:goto mi_0046b282;default:abort();}
mi_0046b1a6:;
    /* 0046b1a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b1a8 */
    f->r[1]=(0x00790134u+f->r[6]);
    /* 0046b1ae */
    f->r[2]=0x00000c80u;
mi_0046b1b3:;
    /* 0046b1b3 */
    MI_U8((0x007900c2u+f->r[6]+f->r[0]*1u))=(uint8_t)((f->r[3]&0xffu));
    /* 0046b1ba */
    MI_U16((0xffffffb8u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046b1be */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046b1c1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046b1c2 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b1c5 */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0046b1c8 */
    if(mi_signed_less(f->flags))goto mi_0046b1b3;
    /* 0046b1ca */
    f->r[0]=mi_read_0046b100((0x00f53e6cu),4u);
    /* 0046b1cf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046b1d1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b282;
    /* 0046b1d7 */
    f->r[1]=0x00f53e70u;
    /* 0046b1dc */
    f->r[2]=f->r[0];
mi_0046b1de:;
    /* 0046b1de */
    f->r[0]=mi_read_0046b100((0x00000000u+f->r[1]),4u);
    /* 0046b1e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046b1e2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b1e9;
    /* 0046b1e4 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0046b1e7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b1f1;
mi_0046b1e9:;
    /* 0046b1e9 */
    MI_U8((0x007900c2u+f->r[6]+f->r[0]*1u))=(uint8_t)(0x00000000u);
mi_0046b1f1:;
    /* 0046b1f1 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b1f4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046b1f5 */
    if(!(f->flags&MI_ZF))goto mi_0046b1de;
    /* 0046b1f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b1f8 */
    f->r[0]=f->r[7];
    /* 0046b1fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b1fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b1fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b1fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0046b1fe */
    f->r[4]+=4;return;
mi_0046b1ff:;
    /* 0046b1ff */
    f->r[0]=mi_read_0046b100((0x00f53e6cu),4u);
    /* 0046b204 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0046b206 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046b208 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b282;
    /* 0046b20a */
    f->r[3]=0x00f53e70u;
mi_0046b20f:;
    /* 0046b20f */
    f->r[7]=mi_read_0046b100((0x00000000u+f->r[3]),4u);
    /* 0046b211 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b212 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b217u;f->boundary(0x0046b212u,0x005013a0u,f);
    /* 0046b217 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b21a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046b21c */
    if((f->flags&MI_ZF))goto mi_0046b226;
    /* 0046b21e */
    MI_U8((0x007900c2u+f->r[6]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_0046b226:;
    /* 0046b226 */
    f->r[0]=mi_read_0046b100((0x00f53e6cu),4u);
    /* 0046b22b */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0046b22c */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046b22f */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046b231 */
    if(mi_signed_less(f->flags))goto mi_0046b20f;
    /* 0046b233 */
    f->r[7]=mi_read_0046b100((0x00000010u+f->r[4]),4u);
    /* 0046b237 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b238 */
    f->r[0]=f->r[7];
    /* 0046b23a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b23b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b23c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b23d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0046b23e */
    f->r[4]+=4;return;
mi_0046b23f:;
    /* 0046b23f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b241 */
    f->r[1]=(0x00790134u+f->r[6]);
    /* 0046b247 */
    f->r[2]=0x00000c80u;
mi_0046b24c:;
    /* 0046b24c */
    MI_U8((0x007900c2u+f->r[6]+f->r[0]*1u))=(uint8_t)((f->r[3]&0xffu));
    /* 0046b253 */
    MI_U16((0xffffffb8u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046b257 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046b25a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046b25b */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b25e */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0046b261 */
    if(mi_signed_less(f->flags))goto mi_0046b24c;
    /* 0046b263 */
    f->r[1]=mi_read_0046b100((0x00f53dd8u),4u);
    /* 0046b269 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046b26b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b282;
    /* 0046b26d */
    f->r[0]=0x00f53ddcu;
mi_0046b272:;
    /* 0046b272 */
    f->r[2]=mi_read_0046b100((0x00000000u+f->r[0]),4u);
    /* 0046b274 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b277 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0046b278 */
    MI_U8((0x007900c2u+f->r[2]+f->r[6]*1u))=(uint8_t)(0x00000000u);
    /* 0046b280 */
    if(!(f->flags&MI_ZF))goto mi_0046b272;
mi_0046b282:;
    /* 0046b282 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b283 */
    f->r[0]=f->r[7];
    /* 0046b285 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b286 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b287 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b288 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0046b289 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00472790(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00472790 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 00472794 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472795 */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 00472799 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047279a */
    f->r[6]=(0x00000000u+f->r[3]+f->r[3]*8u);
    /* 0047279d */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004727a0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004727a2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcb9ceu+f->r[6]));
    /* 004727a9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047288b;
    /* 004727af */
    f->r[2]=f->r[0];
    /* 004727b1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004727b2 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004727b5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004727b7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004727b8 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004727bc */
    f->r[0]=(0x007999d9u+f->r[2]*8u);
    /* 004727c3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004727c7:;
    /* 004727c7 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004727cc */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004727d2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcb9f8u+f->r[6]));
    /* 004727d9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcb9f6u+f->r[6]));
    /* 004727e0 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00ea3feau+f->r[0]));
    /* 004727e7 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00ea3fe8u+f->r[0]));
    /* 004727ee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004727ef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004727f0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004727f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004727f2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004727f7u;f->boundary(0x004727f2u,0x00518400u,f);
    /* 004727f7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004727fa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004727fc */
    if(!(f->flags&MI_ZF))goto mi_0047283d;
    /* 004727fe */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00472803 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00472809 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcb9f6u+f->r[6]));
    /* 00472810 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00ea3ff0u+f->r[0]));
    /* 00472817 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00ea3ff2u+f->r[0]));
    /* 0047281e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcb9f8u+f->r[6]));
    /* 00472825 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472826 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472827 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472828 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472829 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047282eu;f->boundary(0x00472829u,0x00518400u,f);
    /* 0047282e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00472831 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00472833 */
    if(!(f->flags&MI_ZF))goto mi_0047283d;
    /* 00472835 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 00472839 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
mi_0047283d:;
    /* 0047283d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcb9ccu+f->r[6]));
    /* 00472844 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00472848 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047284a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0047284c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047284d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 0047284f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472850 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472851 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00472852 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472857u;f->boundary(0x00472852u,0x004b4800u,f);
    /* 00472857 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047285a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047285d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00472860 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00472862 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00472866 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00472869 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047286a */
    MI_U16((0x00bcd658u+f->r[1]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00472871 */
    MI_U8((0x00bcd65bu+f->r[1]))=(uint8_t)(0x00000001u);
    /* 00472878 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000010u);
    /* 0047287f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00472883 */
    if(!(f->flags&MI_ZF))goto mi_004727c7;
    /* 00472889 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047288a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_0047288b:;
    /* 0047288b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00472890u;f->boundary(0x0047288bu,0x004b7c80u,f);
    /* 00472890 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00472891 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00472892 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004b6e10(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b6e10 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004b6e13 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6e14 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6e15 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6e16 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004b6e1e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b6e23u;f->boundary(0x004b6e1eu,0x004b77d0u,f);
    /* 004b6e23 */
    f->r[1]=MI_U32((0x006fdce4u));
    /* 004b6e29 */
    f->r[2]=MI_U32((0x006fdcf0u));
    /* 004b6e2f */
    f->r[6]=MI_U32((0x006fdcfcu));
    /* 004b6e35 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b6e37 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004b6e3b */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004b6e41 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004b6e47 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004b6e4b */
    a=f->r[6];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004b6e51 */
    f->r[3]=0x00000001u;
mi_004b6e56:;
    /* 004b6e56 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b6e58 */
    if(!(f->flags&MI_ZF))goto mi_004b6e5e;
    /* 004b6e5a */
    MI_U8((0x00000010u+f->r[4]+f->r[0]*1u))=(uint8_t)((f->r[3]&0xffu));
mi_004b6e5e:;
    /* 004b6e5e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b6e60 */
    if(!(f->flags&MI_ZF))goto mi_004b6e66;
    /* 004b6e62 */
    MI_U8((0x00000010u+f->r[4]+f->r[0]*1u))=(uint8_t)((f->r[3]&0xffu));
mi_004b6e66:;
    /* 004b6e66 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b6e68 */
    if(!(f->flags&MI_ZF))goto mi_004b6e6e;
    /* 004b6e6a */
    MI_U8((0x00000010u+f->r[4]+f->r[0]*1u))=(uint8_t)((f->r[3]&0xffu));
mi_004b6e6e:;
    /* 004b6e6e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b6e6f */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004b6e72 */
    if(mi_signed_less(f->flags))goto mi_004b6e56;
    /* 004b6e74 */
    f->r[0]=0x00bcba0au;
mi_004b6e79:;
    /* 004b6e79 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffffbeu+f->r[0])))&0xffu)<<0);
    /* 004b6e7c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b6e7e */
    if((f->flags&MI_ZF))goto mi_004b6e8d;
    /* 004b6e80 */
    a=MI_U8((0x00000000u+f->r[0]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b6e82 */
    if(!(f->flags&MI_ZF))goto mi_004b6e8d;
    /* 004b6e84 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b6e86 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffffc0u+f->r[0])))&0xffu)<<0);
    /* 004b6e89 */
    MI_U8((0x00000010u+f->r[4]+f->r[1]*1u))=(uint8_t)((f->r[3]&0xffu));
mi_004b6e8d:;
    /* 004b6e8d */
    a=f->r[0];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b6e92 */
    a=f->r[0];b=0x00bcd62au;t=mi_alu(f,a,b,32,1);
    /* 004b6e97 */
    if(mi_signed_less(f->flags))goto mi_004b6e79;
    /* 004b6e99 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004b6e9b:;
    /* 004b6e9b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000010u+f->r[4]+f->r[0]*1u)))&0xffu)<<0);
    /* 004b6e9f */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b6ea1 */
    if((f->flags&MI_ZF))goto mi_004b6eaf;
    /* 004b6ea3 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b6ea4 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004b6ea7 */
    if(mi_signed_less(f->flags))goto mi_004b6e9b;
    /* 004b6ea9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0000000cu+f->r[4])))&0xffu)<<0);
    /* 004b6ead */
    goto mi_004b6eb1;
mi_004b6eaf:;
    /* 004b6eaf */
    f->r[1]=f->r[0];
mi_004b6eb1:;
    /* 004b6eb1 */
    f->r[7]=f->r[3];
    /* 004b6eb3 */
    f->r[0]=0x00bcba58u;
mi_004b6eb8:;
    /* 004b6eb8 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004b6ebb */
    if((f->flags&MI_ZF))goto mi_004b6ed3;
    /* 004b6ebd */
    a=f->r[0];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b6ec2 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004b6ec3 */
    a=f->r[0];b=0x00bcd5e8u;t=mi_alu(f,a,b,32,1);
    /* 004b6ec8 */
    if(mi_signed_less(f->flags))goto mi_004b6eb8;
    /* 004b6eca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b6ecb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b6ecc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b6ece */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b6ecf */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b6ed2 */
    f->r[4]+=4;return;
mi_004b6ed3:;
    /* 004b6ed3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6ed4 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004b6ed8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00eaac28u)))&0xffu)<<0);
    /* 004b6ede */
    f->r[0]=f->r[5];
    /* 004b6ee0 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004b6ee3 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b6ee6 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b6ee9 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b6eeb */
    MI_U8((0x00bcb9c8u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b6ef1 */
    MI_U8((0x00bcba0au+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b6ef7 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b6efa */
    MI_U8((0x00bcb9c9u+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 004b6f00 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[0])))&0xffffu)<<0);
    /* 004b6f07 */
    MI_U16((0x00bcb9ccu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b6f0e */
    MI_U16((0x00bcb9ceu+f->r[6]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004b6f15 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e0u+f->r[0])))&0xffffu)<<0);
    /* 004b6f1c */
    MI_U16((0x00bcb9f2u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b6f23 */
    MI_U16((0x00bcb9f4u+f->r[6]))=(uint16_t)(0x00000032u);
    /* 004b6f2c */
    MI_U8((0x00bcb9cbu+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b6f32 */
    MI_U16((0x00bcba4au+f->r[6]))=(uint16_t)(0x0000ffffu);
    /* 004b6f3b */
    MI_U8((0x00bcba3fu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004b6f42 */
    MI_U8((0x00bcb9cau+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b6f48 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999deu+f->r[0])))&0xffffu)<<0);
    /* 004b6f4f */
    a=(f->r[1]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,0);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004b6f53 */
    a=(f->r[2]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004b6f55 */
    MI_U16((0x00bcb9f6u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6f5c */
    MI_U16((0x00bcb9f8u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b6f63 */
    MI_U16((0x00bcb9fau+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6f6a */
    MI_U16((0x00bcb9fcu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b6f71 */
    MI_U16((0x00bcb9feu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6f78 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b6f7a */
    MI_U16((0x00bcba00u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b6f81 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6f82 */
    MI_U16((0x00bcba48u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6f89 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6f8a */
    MI_U16((0x00bcba46u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b6f91 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6f92 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999d9u+f->r[0])))&0xffu)<<0);
    /* 004b6f98 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6f9a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b6f9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b6fa0u;f->boundary(0x004b6f9bu,0x004b4800u,f);
    /* 004b6fa0 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b6fa3 */
    MI_U16((0x00bcba08u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b6faa */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b6fad */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b6faf */
    f->r[0]=MI_U32((0x00bcb9bcu));
    /* 004b6fb4 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b6fb7 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b6fba */
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004b6fc1 */
    MI_U16((0x00bcd658u+f->r[1]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004b6fc8 */
    f->r[1]=MI_U32((0x00bcb9b4u));
    /* 004b6fce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b6fcf */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b6fd0 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b6fd2 */
    MI_U32((0x00bcb9b4u))=(uint32_t)(f->r[1]);
    /* 004b6fd8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b6fe0;
    /* 004b6fda */
    MI_U32((0x00bcb9bcu))=(uint32_t)(f->r[7]);
mi_004b6fe0:;
    /* 004b6fe0 */
    f->r[0]=f->r[7];
    /* 004b6fe2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b6fe3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b6fe4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b6fe5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b6fe8 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004bfb30(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bfb30 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004bfb33 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfb34 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfb35 */
    f->r[5]=mi_read_004bfb30((0x00000020u+f->r[4]),4u);
    /* 004bfb39 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfb3a */
    f->r[3]=f->r[5];
    /* 004bfb3c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfb3d */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004bfb40 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004bfb42 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfb44 */
    a=f->r[3];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004bfb47 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004bfb30((0x007999deu+f->r[3]),2u);
    /* 004bfb4e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004bfb30((0x007999e0u+f->r[3]),2u);
    /* 004bfb55 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bfb59 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004bfb5d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bfb62u;f->boundary(0x004bfb5du,0x004c53d0u,f);
    /* 004bfb62 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bfb65 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004bfb68 */
    if(!(f->flags&MI_ZF))goto mi_004bfb7b;
    /* 004bfb6a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfb6c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bfb71u;f->boundary(0x004bfb6cu,0x004c53d0u,f);
    /* 004bfb71 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bfb74 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004bfb77 */
    if(!(f->flags&MI_ZF))goto mi_004bfb7b;
    /* 004bfb79 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004bfb7b:;
    /* 004bfb7b */
    f->r[2]=mi_read_004bfb30((0x005df778u+f->r[0]*4u),4u);
    /* 004bfb82 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004bfb84 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_004bfb30((0x00799a82u+f->r[3]),1u))&0xffu)<<0);
    /* 004bfb8a */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004bfb8e */
    a=f->r[1];b=mi_read_004bfb30((0x00bca2e0u),4u);t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bfb94 */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004bfb97 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004bfb9b */
    if(mi_signed_less(f->flags))goto mi_004bfba4;
    /* 004bfb9d */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004bfba0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004bfba4:;
    /* 004bfba4 */
    f->r[0]=f->r[1];
    /* 004bfba6 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x005df768u);
    /* 004bfbae */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bfbb1 */
    a=f->r[0];b=0x005df78cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bfbb6 */
    a=f->r[0];b=0x005df768u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004bfbbb */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bfbbf */
    goto mi_004bfbc9;
mi_004bfbc1:;
    /* 004bfbc1 */
    f->r[0]=mi_read_004bfb30((0x0000001cu+f->r[4]),4u);
    /* 004bfbc5 */
    f->r[1]=mi_read_004bfb30((0x00000018u+f->r[4]),4u);
mi_004bfbc9:;
    /* 004bfbc9 */
    f->r[2]=mi_read_004bfb30((0x00000028u+f->r[4]),4u);
    /* 004bfbcd */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004bfbd0 */
    f->r[6]=mi_read_004bfb30((0x00000000u+f->r[0]+f->r[2]*1u),4u);
    /* 004bfbd3 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004bfbe3;
    /* 004bfbd5 */
    switch(mi_read_004bfb30((0x004bfcacu+f->r[1]*4u),4u)){case 0x004bfbdcu:goto mi_004bfbdc;case 0x004bfbe3u:goto mi_004bfbe3;default:abort();}
mi_004bfbdc:;
    /* 004bfbdc */
    f->r[7]=0x00000001u;
    /* 004bfbe1 */
    goto mi_004bfbe5;
mi_004bfbe3:;
    /* 004bfbe3 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_004bfbe5:;
    /* 004bfbe5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004bfbe7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read_004bfb30((0x00799a82u+f->r[3]),1u))&0xffu)<<0);
    /* 004bfbed */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004bfbf0 */
    if((f->flags&MI_ZF))goto mi_004bfc18;
    /* 004bfbf2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bfbf3 */
    if((f->flags&MI_ZF))goto mi_004bfbff;
    /* 004bfbf5 */
    f->r[0]=mi_read_004bfb30((0x00000014u+f->r[4]),4u);
    /* 004bfbf9 */
    f->r[1]=mi_read_004bfb30((0x00000010u+f->r[4]),4u);
    /* 004bfbfd */
    goto mi_004bfc33;
mi_004bfbff:;
    /* 004bfbff */
    f->r[2]=mi_read_004bfb30((0x00000014u+f->r[4]),4u);
    /* 004bfc03 */
    f->r[1]=f->r[6];
    /* 004bfc05 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bfc08 */
    f->r[0]=mi_read_004bfb30((0x005df818u+f->r[1]),4u);
    /* 004bfc0e */
    f->r[1]=mi_read_004bfb30((0x005df81cu+f->r[1]),4u);
    /* 004bfc14 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bfc16 */
    goto mi_004bfc2f;
mi_004bfc18:;
    /* 004bfc18 */
    f->r[2]=mi_read_004bfb30((0x00000014u+f->r[4]),4u);
    /* 004bfc1c */
    f->r[1]=f->r[6];
    /* 004bfc1e */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bfc21 */
    f->r[0]=mi_read_004bfb30((0x005df81cu+f->r[1]),4u);
    /* 004bfc27 */
    f->r[1]=mi_read_004bfb30((0x005df818u+f->r[1]),4u);
    /* 004bfc2d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004bfc2f:;
    /* 004bfc2f */
    a=f->r[1];b=mi_read_004bfb30((0x00000010u+f->r[4]),4u);t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_004bfc33:;
    /* 004bfc33 */
    f->r[2]=mi_read_004bfb30((0x00000028u+f->r[4]),4u);
    /* 004bfc37 */
    f->r[2]=mi_read_004bfb30((0x00000000u+f->r[2]),4u);
    /* 004bfc39 */
    a=f->r[2];b=0x000001aeu;t=mi_alu(f,a,b,32,1);
    /* 004bfc3f */
    if(!(f->flags&MI_ZF))goto mi_004bfc45;
    /* 004bfc41 */
    f->r[2]=mi_read_004bfb30((0x00000020u+f->r[4]),4u);
mi_004bfc45:;
    /* 004bfc45 */
    f->r[2]=mi_read_004bfb30((0x00f55bb4u+f->r[2]*8u),4u);
    /* 004bfc4c */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004bfc4f */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bfc51 */
    a=mi_read_004bfb30((0x005df810u+f->r[6]),1u);b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004bfc58 */
    f->r[6]=mi_read_004bfb30((0x005df814u+f->r[6]),4u);
    /* 004bfc5e */
    if((f->flags&MI_ZF))goto mi_004bfc67;
    /* 004bfc60 */
    a=0x00008048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc65 */
    goto mi_004bfc6c;
mi_004bfc67:;
    /* 004bfc67 */
    a=0x00008008u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004bfc6c:;
    /* 004bfc6c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc6d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc6e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc6f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc70 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc71 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bfc77u;f->boundary(0x004bfc72u,0x0047bf50u,f);
    /* 004bfc77 */
    f->r[0]=mi_read_004bfb30((0x00000044u+f->r[4]),4u);
    /* 004bfc7b */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bfc7e */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bfc81 */
    a=f->r[0];b=0x005df778u;t=mi_alu(f,a,b,32,1);
    /* 004bfc86 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bfc8a */
    if(mi_signed_less(f->flags))goto mi_004bfbc1;
    /* 004bfc90 */
    f->r[0]=mi_read_004bfb30((0x00000010u+f->r[4]),4u);
    /* 004bfc94 */
    f->r[1]=mi_read_004bfb30((0x00000014u+f->r[4]),4u);
    /* 004bfc98 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc9a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc9b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bfc9c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bfca1u;f->boundary(0x004bfc9cu,0x0047afd0u,f);
    /* 004bfca1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bfca4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bfca5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bfca6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004bfca7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004bfca8 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bfcab */
    f->r[4]+=4;return;
}

void restore_building_control_C_004c0d50(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c0d50 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c0d53 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0d54 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0d58 */
    f->r[0]=f->r[7];
    /* 004c0d5a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c0d5d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c0d5f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c0d62 */
    a=MI_U16((0x00799a10u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004c0d6a */
    if(!(f->flags&MI_ZF))goto mi_004c0ebd;
    /* 004c0d70 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c0d72 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0d73 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a82u+f->r[0])))&0xffu)<<0);
    /* 004c0d79 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004c0d7b */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004c0d7d */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0d81 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0d85 */
    if((f->flags&MI_ZF))goto mi_004c0eaf;
    /* 004c0d8b */
    f->r[0]=f->r[1];
    /* 004c0d8d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0d8e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c0d8f */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004c0d93 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0d95 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004c0d97 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c0d9a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0d9b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0d9f */
    goto mi_004c0da5;
mi_004c0da1:;
    /* 004c0da1 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
mi_004c0da5:;
    /* 004c0da5 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004c0daa */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004c0dac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c0dae */
    if((f->flags&MI_ZF))goto mi_004c0dc1;
    /* 004c0db0 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c0db3 */
    if((f->flags&MI_ZF))goto mi_004c0dc1;
    /* 004c0db5 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004c0db8 */
    if((f->flags&MI_ZF))goto mi_004c0dd0;
    /* 004c0dba */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004c0dbd */
    if((f->flags&MI_ZF))goto mi_004c0dd0;
    /* 004c0dbf */
    goto mi_004c0dcb;
mi_004c0dc1:;
    /* 004c0dc1 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004c0dc4 */
    if((f->flags&MI_ZF))goto mi_004c0dcb;
    /* 004c0dc6 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004c0dc9 */
    if(!(f->flags&MI_ZF))goto mi_004c0dd0;
mi_004c0dcb:;
    /* 004c0dcb */
    f->r[6]=0x00000003u;
mi_004c0dd0:;
    /* 004c0dd0 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0dd4 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c0dd5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0dd7 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004c0dd9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c0ddb */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*2u);
    /* 004c0dde */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c0de0 */
    f->r[0]=MI_U32((0x005df8b0u+f->r[0]*4u));
    /* 004c0de7 */
    a=f->r[0];b=0x0000023bu;t=mi_alu(f,a,b,32,1);
    /* 004c0dec */
    if(!(f->flags&MI_ZF))goto mi_004c0e2a;
    /* 004c0dee */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004c0df4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0df6 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004c0dfc */
    f->r[0]=MI_U32((0x00ea5f9cu+f->r[2]));
    /* 004c0e02 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0e08u;f->boundary(0x004c0e03u,0x004c52d0u,f);
    /* 004c0e08 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0e0b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c0e0d */
    if((f->flags&MI_ZF))goto mi_004c0e1f;
    /* 004c0e0f */
    f->r[1]=MI_U32((0x00f56dacu));
    /* 004c0e15 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004c0e17 */
    f->r[0]=(0x00000002u+f->r[1]);
    /* 004c0e1a */
    if((f->flags&MI_ZF))goto mi_004c0e73;
    /* 004c0e1c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0e1d */
    goto mi_004c0e73;
mi_004c0e1f:;
    /* 004c0e1f */
    f->r[2]=MI_U32((0x00f56d8cu));
    /* 004c0e25 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 004c0e28 */
    goto mi_004c0e71;
mi_004c0e2a:;
    /* 004c0e2a */
    a=f->r[0];b=0x0000023au;t=mi_alu(f,a,b,32,1);
    /* 004c0e2f */
    if(!(f->flags&MI_ZF))goto mi_004c0e68;
    /* 004c0e31 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004c0e36 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e38 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004c0e3e */
    f->r[1]=MI_U32((0x00ea5f9cu+f->r[0]));
    /* 004c0e44 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0e4au;f->boundary(0x004c0e45u,0x004c52d0u,f);
    /* 004c0e4a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0e4d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c0e4f */
    if((f->flags&MI_ZF))goto mi_004c0e5d;
    /* 004c0e51 */
    f->r[0]=MI_U32((0x00f56dacu));
    /* 004c0e56 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004c0e58 */
    if((f->flags&MI_ZF))goto mi_004c0e73;
    /* 004c0e5a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0e5b */
    goto mi_004c0e73;
mi_004c0e5d:;
    /* 004c0e5d */
    f->r[2]=MI_U32((0x00f56d84u));
    /* 004c0e63 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[6]*1u);
    /* 004c0e66 */
    goto mi_004c0e71;
mi_004c0e68:;
    /* 004c0e68 */
    f->r[0]=MI_U32((0x00f55bb4u+f->r[0]*8u));
    /* 004c0e6f */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004c0e71:;
    /* 004c0e71 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004c0e73:;
    /* 004c0e73 */
    f->r[6]=f->r[7];
    /* 004c0e75 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e77 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c0e7a */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c0e7c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e7d */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c0e80 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e82 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e84 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c0e8b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c0e92 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e93 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e94 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0e95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0e9au;f->boundary(0x004c0e95u,0x0047bf50u,f);
    /* 004c0e9a */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a12u+f->r[6]));
    /* 004c0ea1 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0ea4 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004c0ea5 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004c0ea7 */
    if(!(f->flags&MI_ZF))goto mi_004c0da1;
    /* 004c0ead */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c0eae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_004c0eaf:;
    /* 004c0eaf */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004c0eb3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0eb4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0eb9u;f->boundary(0x004c0eb4u,0x004c0be0u,f);
    /* 004c0eb9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0ebc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_004c0ebd:;
    /* 004c0ebd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c0ebe */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0ec1 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004c3420(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c3420 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3421 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3422 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3423 */
    f->r[7]=mi_read_004c3420((0x00000010u+f->r[4]),4u);
    /* 004c3427 */
    f->r[6]=f->r[7];
    /* 004c3429 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c342c */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c342e */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c3431 */
    a=mi_read_004c3420((0x00799a10u+f->r[6]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004c3439 */
    if(!(f->flags&MI_ZF))goto mi_004c3701;
    /* 004c343f */
    f->r[0]=mi_read_004c3420((0x00000024u+f->r[4]),4u);
    /* 004c3443 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3444 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c3446 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3447 */
    if((f->flags&MI_ZF))goto mi_004c3497;
    /* 004c3449 */
    f->r[0]=mi_read_004c3420((0x00eaac28u),4u);
    /* 004c344e */
    MI_U8((0x00799a81u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004c3455 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004c345b */
    MI_U16((0x00ea5ce8u+f->r[0]))=(uint16_t)(0x00000001u);
    /* 004c3464 */
    MI_U16((0x00ea5cfau+f->r[0]))=(uint16_t)(0x00000002u);
    /* 004c346d */
    MI_U32((0x00ea5f9cu+f->r[0]))=(uint32_t)(f->r[7]);
    /* 004c3473 */
    f->r[0]=mi_read_004c3420((0x00bc9714u),4u);
    /* 004c3478 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*1u);
    /* 004c347b */
    f->r[0]=mi_read_004c3420((0x00bca2e0u),4u);
    /* 004c3480 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004c3482 */
    if(!(f->flags&MI_SF))goto mi_004c3487;
    /* 004c3484 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_004c3487:;
    /* 004c3487 */
    f->r[0]=f->r[7];
    /* 004c3489 */
    MI_U8((0x00799a82u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004c348f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c3493 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004c3495 */
    goto mi_004c34ad;
mi_004c3497:;
    /* 004c3497 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004c3499 */
    f->r[0]=f->r[7];
    /* 004c349b */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read_004c3420((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 004c34a1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c34a5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004c34a7 */
    goto mi_004c34ad;
mi_004c34a9:;
    /* 004c34a9 */
    f->r[0]=mi_read_004c3420((0x00000018u+f->r[4]),4u);
mi_004c34ad:;
    /* 004c34ad */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004c34af */
    if(!(f->flags&MI_ZF))goto mi_004c34e9;
    /* 004c34b1 */
    f->r[0]=f->r[3];
    /* 004c34b3 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c34b4 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c34b6 */
    f->r[1]=f->r[0];
    /* 004c34b8 */
    f->r[0]=mi_read_004c3420((0x00bca2e0u),4u);
    /* 004c34bd */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c34be */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c34c0 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004c34c2 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004c34c4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004c34c7 */
    f->r[1]=mi_read_004c3420((0x0000002cu+f->r[4]),4u);
    /* 004c34cb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004c34cd */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004c34d0 */
    f->r[0]=mi_read_004c3420((0x005df8b0u+f->r[2]*4u),4u);
    /* 004c34d7 */
    if((f->flags&MI_ZF))goto mi_004c3645;
    /* 004c34dd */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004c34e4 */
    goto mi_004c361e;
mi_004c34e9:;
    /* 004c34e9 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004c34ec */
    if(!(f->flags&MI_ZF))goto mi_004c356d;
    /* 004c34ee */
    f->r[0]=mi_read_004c3420((0x0000002cu+f->r[4]),4u);
    /* 004c34f2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c34f4 */
    if((f->flags&MI_ZF))goto mi_004c3563;
    /* 004c34f6 */
    f->r[0]=mi_read_004c3420((0x005df974u+f->r[3]*8u),4u);
    /* 004c34fd */
    f->r[7]=mi_read_004c3420((0x00000020u+f->r[4]),4u);
    /* 004c3501 */
    f->r[1]=mi_read_004c3420((0x005df970u+f->r[3]*8u),4u);
    /* 004c3508 */
    f->r[2]=mi_read_004c3420((0x0000001cu+f->r[4]),4u);
    /* 004c350c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c350e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c3510 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3512 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c3514 */
    f->r[2]=mi_read_004c3420((0x00000030u+f->r[4]),4u);
    /* 004c3518 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3519 */
    f->r[0]=mi_read_004c3420((0x00000030u+f->r[4]),4u);
    /* 004c351d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c351e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c351f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3520 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3525u;f->boundary(0x004c3520u,0x0046a400u,f);
    /* 004c3525 */
    f->r[7]=f->r[0];
    /* 004c3527 */
    f->r[6]=f->r[7];
    /* 004c3529 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c352a */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c352d */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c352f */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c3532 */
    MI_U8((0x00799a82u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004c3538 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c353du;f->boundary(0x004c3538u,0x00485100u,f);
    /* 004c353d */
    f->r[0]=mi_read_004c3420((0x00000034u+f->r[4]),4u);
    /* 004c3541 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3544 */
    f->r[1]=f->r[0];
    /* 004c3546 */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004c354d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c3550 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c3552 */
    MI_U16((0x00799a12u+f->r[1]*8u))=(uint16_t)((f->r[7]&0xffffu));
    /* 004c355a */
    MI_U16((0x00799a12u+f->r[6]))=(uint16_t)(0x00000000u);
mi_004c3563:;
    /* 004c3563 */
    f->r[0]=0x0000023au;
    /* 004c3568 */
    goto mi_004c361e;
mi_004c356d:;
    /* 004c356d */
    f->r[2]=f->r[0];
    /* 004c356f */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c3572 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c3574 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004c3420((0x00799a12u+f->r[2]*8u),2u);
    /* 004c357c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c3580 */
    f->r[0]=mi_read_004c3420((0x0000002cu+f->r[4]),4u);
    /* 004c3584 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c3586 */
    if((f->flags&MI_ZF))goto mi_004c35fb;
    /* 004c3588 */
    f->r[1]=mi_read_004c3420((0x005df974u+f->r[3]*8u),4u);
    /* 004c358f */
    f->r[2]=mi_read_004c3420((0x005df970u+f->r[3]*8u),4u);
    /* 004c3596 */
    a=f->r[1];b=f->r[5];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004c3599 */
    a=f->r[2];b=f->r[5];t=mi_imul(f,a,b);f->r[2]=t;
    /* 004c359c */
    f->r[6]=mi_read_004c3420((0x00000020u+f->r[4]),4u);
    /* 004c35a0 */
    f->r[0]=mi_read_004c3420((0x0000001cu+f->r[4]),4u);
    /* 004c35a4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c35a6 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c35a8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c35aa */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c35ac */
    f->r[0]=mi_read_004c3420((0x00000030u+f->r[4]),4u);
    /* 004c35b0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c35b1 */
    f->r[1]=mi_read_004c3420((0x00000030u+f->r[4]),4u);
    /* 004c35b5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c35b6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c35b7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c35b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c35bdu;f->boundary(0x004c35b8u,0x0046a400u,f);
    /* 004c35bd */
    f->r[7]=f->r[0];
    /* 004c35bf */
    f->r[6]=f->r[7];
    /* 004c35c1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c35c2 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c35c5 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c35c7 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c35ca */
    MI_U8((0x00799a82u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004c35d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c35d5u;f->boundary(0x004c35d0u,0x00485100u,f);
    /* 004c35d5 */
    f->r[0]=mi_read_004c3420((0x0000002cu+f->r[4]),4u);
    /* 004c35d9 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c35dc */
    f->r[2]=f->r[0];
    /* 004c35de */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004c35e5 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004c35e8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c35ea */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[7]&0xffffu));
    /* 004c35f2 */
    MI_U16((0x00799a12u+f->r[6]))=(uint16_t)(0x00000000u);
mi_004c35fb:;
    /* 004c35fb */
    f->r[0]=f->r[3];
    /* 004c35fd */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c35fe */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3600 */
    f->r[1]=f->r[0];
    /* 004c3602 */
    f->r[0]=mi_read_004c3420((0x00bca2e0u),4u);
    /* 004c3607 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c3608 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c360a */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 004c360c */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004c360e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 004c3611 */
    f->r[2]=(0x00000000u+f->r[5]+f->r[0]*2u);
    /* 004c3615 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c3617 */
    f->r[0]=mi_read_004c3420((0x005df8b0u+f->r[0]*4u),4u);
mi_004c361e:;
    /* 004c361e */
    f->r[1]=mi_read_004c3420((0x0000002cu+f->r[4]),4u);
    /* 004c3622 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004c3624 */
    if((f->flags&MI_ZF))goto mi_004c3645;
    /* 004c3626 */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004c3629 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004c3645;
    /* 004c362b */
    switch(mi_read_004c3420((0x004c3708u+f->r[3]*4u),4u)){case 0x004c3632u:goto mi_004c3632;case 0x004c363bu:goto mi_004c363b;case 0x004c3645u:goto mi_004c3645;default:abort();}
mi_004c3632:;
    /* 004c3632 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004c3639 */
    goto mi_004c3645;
mi_004c363b:;
    /* 004c363b */
    f->r[1]=(0x00000003u+f->r[5]);
    /* 004c363e */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_004c3645:;
    /* 004c3645 */
    f->r[2]=mi_read_004c3420((0x00bca2e0u),4u);
    /* 004c364b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004c364d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004c364f */
    if((f->flags&MI_ZF))goto mi_004c3662;
    /* 004c3651 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c3654 */
    if((f->flags&MI_ZF))goto mi_004c3662;
    /* 004c3656 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004c3659 */
    if((f->flags&MI_ZF))goto mi_004c3671;
    /* 004c365b */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004c365e */
    if((f->flags&MI_ZF))goto mi_004c3671;
    /* 004c3660 */
    goto mi_004c366c;
mi_004c3662:;
    /* 004c3662 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004c3665 */
    if((f->flags&MI_ZF))goto mi_004c366c;
    /* 004c3667 */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004c366a */
    if(!(f->flags&MI_ZF))goto mi_004c3671;
mi_004c366c:;
    /* 004c366c */
    f->r[1]=0x00000003u;
mi_004c3671:;
    /* 004c3671 */
    f->r[2]=mi_read_004c3420((0x00f55bb4u+f->r[0]*8u),4u);
    /* 004c3678 */
    f->r[0]=mi_read_004c3420((0x005df974u+f->r[3]*8u),4u);
    /* 004c367f */
    a=f->r[0];b=f->r[5];t=mi_imul(f,a,b);f->r[0]=t;
    /* 004c3682 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c3684 */
    f->r[1]=mi_read_004c3420((0x005df970u+f->r[3]*8u),4u);
    /* 004c368b */
    a=f->r[1];b=f->r[5];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004c368e */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3690 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3691 */
    f->r[2]=mi_read_004c3420((0x00000028u+f->r[4]),4u);
    /* 004c3695 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3697 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c3699 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c369b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c369c */
    f->r[0]=mi_read_004c3420((0x00000030u+f->r[4]),4u);
    /* 004c36a0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c36a2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c36a3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c36a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c36a9u;f->boundary(0x004c36a4u,0x0047bf50u,f);
    /* 004c36a9 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c36ac */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004c36ad */
    a=f->r[5];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004c36b0 */
    if(mi_signed_less(f->flags))goto mi_004c34a9;
    /* 004c36b6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c36b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c36bcu;f->boundary(0x004c36b7u,0x00475b50u,f);
    /* 004c36bc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c36bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c36c2u;f->boundary(0x004c36bdu,0x004c0be0u,f);
    /* 004c36c2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c36c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c36c8u;f->boundary(0x004c36c3u,0x004c5120u,f);
    /* 004c36c8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c36cb */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004c36ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c36cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c36d0 */
    if(!(f->flags&MI_ZF))goto mi_004c36ec;
    /* 004c36d2 */
    f->r[2]=mi_read_004c3420((0x00eaac28u),4u);
    /* 004c36d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c36d9 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004c36df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c36e0 */
    MI_U32((0x00ea6200u+f->r[2]))=(uint32_t)(0x0000000au);
    /* 004c36ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c36eb */
    f->r[4]+=4;return;
mi_004c36ec:;
    /* 004c36ec */
    f->r[0]=mi_read_004c3420((0x00eaac28u),4u);
    /* 004c36f1 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004c36f7 */
    MI_U32((0x00ea6200u+f->r[0]))=(uint32_t)(0x00000000u);
mi_004c3701:;
    /* 004c3701 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c3702 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c3703 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004c3704 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004c0be0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c0be0 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c0be3 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 004c0be7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0be8 */
    f->r[1]=f->r[0];
    /* 004c0bea */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0beb */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c0bee */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c0bf0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c0bf2 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c0bf5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bf6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bf7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a82u+f->r[1])))&0xffu)<<0);
    /* 004c0bfd */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0c01 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c0c02 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 004c0c09 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0c0b */
    f->r[6]=f->r[0];
    /* 004c0c0d */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[6]=t;
    /* 004c0c0f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 004c0c16 */
    f->r[0]=MI_U32((0x005e00ccu+f->r[6]*8u));
    /* 004c0c1d */
    f->r[5]=MI_U32((0x005e00d0u+f->r[6]*8u));
    /* 004c0c24 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c0c26 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004c0c2b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c0c2c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0c2e */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c0c30 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004c0c32 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0c36 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0c3a */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 004c0c3d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0c41 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0c45 */
    f->r[2]=MI_U32((0x005e014cu+f->r[0]*4u));
    /* 004c0c4c */
    f->r[0]=0x005df9bcu;
    /* 004c0c51 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c0c55 */
    goto mi_004c0c5f;
mi_004c0c57:;
    /* 004c0c57 */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004c0c5b */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
mi_004c0c5f:;
    /* 004c0c5f */
    f->r[5]=f->r[7];
    /* 004c0c61 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0c65 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004c0c67 */
    f->r[2]=f->r[7];
    /* 004c0c69 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004c0c6b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c0c6f */
    f->r[3]=f->r[0];
    /* 004c0c71 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0c75 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x0000000du);
    /* 004c0c7d */
    goto mi_004c0c83;
mi_004c0c7f:;
    /* 004c0c7f */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
mi_004c0c83:;
    /* 004c0c83 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004c0c87 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0c8a */
    if((f->flags&MI_ZF))goto mi_004c0cbc;
    /* 004c0c8c */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0c8f */
    if((f->flags&MI_ZF))goto mi_004c0cb2;
    /* 004c0c91 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0c94 */
    if((f->flags&MI_ZF))goto mi_004c0ca2;
    /* 004c0c96 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004c0c9a */
    f->r[7]=(0x00000000u+f->r[1]+f->r[2]*1u);
    /* 004c0c9d */
    f->r[6]=(0x00000000u+f->r[0]+f->r[5]*1u);
    /* 004c0ca0 */
    goto mi_004c0cc2;
mi_004c0ca2:;
    /* 004c0ca2 */
    f->r[0]=MI_U32((0x00000038u+f->r[4]));
    /* 004c0ca6 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[5]*1u);
    /* 004c0ca9 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0cad */
    f->r[7]=(0x00000000u+f->r[0]+f->r[1]*1u);
    /* 004c0cb0 */
    goto mi_004c0cc2;
mi_004c0cb2:;
    /* 004c0cb2 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0cb6 */
    f->r[7]=MI_U32((0x00000038u+f->r[4]));
    /* 004c0cba */
    goto mi_004c0cc2;
mi_004c0cbc:;
    /* 004c0cbc */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004c0cc0 */
    f->r[6]=f->r[2];
mi_004c0cc2:;
    /* 004c0cc2 */
    f->r[1]=MI_U32((0xfffffff4u+f->r[3]));
    /* 004c0cc5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004c0cc7 */
    if((f->flags&MI_ZF))goto mi_004c0d06;
    /* 004c0cc9 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 004c0ccd */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0cd0 */
    if((f->flags&MI_ZF))goto mi_004c0cea;
    /* 004c0cd2 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0cd5 */
    if((f->flags&MI_ZF))goto mi_004c0ce5;
    /* 004c0cd7 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0cda */
    if((f->flags&MI_ZF))goto mi_004c0ce1;
    /* 004c0cdc */
    f->r[0]=MI_U32((0x00000004u+f->r[3]));
    /* 004c0cdf */
    goto mi_004c0ced;
mi_004c0ce1:;
    /* 004c0ce1 */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 004c0ce3 */
    goto mi_004c0ced;
mi_004c0ce5:;
    /* 004c0ce5 */
    f->r[0]=MI_U32((0xfffffffcu+f->r[3]));
    /* 004c0ce8 */
    goto mi_004c0ced;
mi_004c0cea:;
    /* 004c0cea */
    f->r[0]=MI_U32((0xfffffff8u+f->r[3]));
mi_004c0ced:;
    /* 004c0ced */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0cee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0cef */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0cf0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0cf1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0cf6u;f->boundary(0x004c0cf1u,0x004c0860u,f);
    /* 004c0cf6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0cf8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0cf9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0cfa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0cffu;f->boundary(0x004c0cfau,0x004746d0u,f);
    /* 004c0cff */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 004c0d03 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004c0d06:;
    /* 004c0d06 */
    f->r[6]=MI_U32((0x00000038u+f->r[4]));
    /* 004c0d0a */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004c0d0e */
    a=f->r[3];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c0d11 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c0d12 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004c0d13 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0d14 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c0d18 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c0d1c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0d20 */
    if(!(f->flags&MI_ZF))goto mi_004c0c7f;
    /* 004c0d26 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0d2a */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004c0d2e */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c0d2f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004c0d30 */
    a=f->r[3];b=0x005e00d8u;t=mi_alu(f,a,b,32,1);
    /* 004c0d36 */
    f->r[0]=f->r[3];
    /* 004c0d38 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c0d3c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c0d40 */
    if(mi_signed_less(f->flags))goto mi_004c0c57;
    /* 004c0d46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c0d47 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c0d48 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c0d49 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c0d4a */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0d4d */
    f->r[4]+=4;return;
}

void restore_building_control_C_004c0860(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c0860 */
    f->r[0]=MI_U32((0x00bca2ecu));
    /* 004c0865 */
    f->r[1]=MI_U32((0x00bca2e8u));
    /* 004c086b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c086c */
    f->r[6]=MI_U32((0x0000000cu+f->r[4]));
    /* 004c0870 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0871 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 004c0875 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c0877 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c0879 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c087a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c087b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0880u;f->boundary(0x004c087bu,0x0046f930u,f);
    /* 004c0880 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0883 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c0885 */
    if((f->flags&MI_ZF))goto mi_004c090f;
    /* 004c088b */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004c0891 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004c0894 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[2]*2u);
    /* 004c0897 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004c089b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004c089e */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004c08a1 */
    f->r[1]=MI_U32((0x00f55bb4u+f->r[2]*8u));
    /* 004c08a8 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004c08ac */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c08ae */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c08b0 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 004c08b5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[0])))&0xffu)<<0);
    /* 004c08bb */
    MI_U32((0x00b7482cu+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004c08c2 */
    a=(f->r[2]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c08c5 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 004c08cb */
    MI_U8((0x00b67d1cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004c08d1 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004c08d7 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004c08dd */
    a=(f->r[1]&0xffu);b=0x000000f0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c08e0 */
    MI_U8((0x00ae8e7cu+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004c08e7 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004c08ed */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00ea5f9cu+f->r[2])))&0xffffu)<<0);
    /* 004c08f4 */
    MI_U16((0x00b4e6fcu+f->r[0]*2u))=(uint16_t)((f->r[1]&0xffffu));
    /* 004c08fc */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004c0903 */
    f->r[0]=(0x00af598cu+f->r[0]*4u);
    /* 004c090a */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 004c090d */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_004c090f:;
    /* 004c090f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c0910 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c0911 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004c53d0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c53d0 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004c53d4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c53d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c53d7 */
    f->r[1]=0x007848e2u;
mi_004c53dc:;
    /* 004c53dc */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[1]));
    /* 004c53df */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004c53e1 */
    if((f->flags&MI_ZF))goto mi_004c53f2;
    /* 004c53e3 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c53e6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c53e7 */
    a=f->r[1];b=0x007848ecu;t=mi_alu(f,a,b,32,1);
    /* 004c53ed */
    if(mi_signed_less(f->flags))goto mi_004c53dc;
    /* 004c53ef */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004c53f2:;
    /* 004c53f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c53f3 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004b7c80(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b7c80 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004b7c83 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7c84 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004b7c86 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7c87 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7c88 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004b7c8c */
    f->r[2]=0x00bcba80u;
mi_004b7c91:;
    /* 004b7c91 */
    f->r[1]=0x00000008u;
    /* 004b7c96 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b7c98 */
    f->r[7]=(0xffffffe0u+f->r[2]);
    /* 004b7c9b */
    a=f->r[2];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b7ca1 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 004b7ca3 */
    MI_U8((0xffffff70u+f->r[2]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b7ca9 */
    MI_U8((0xffffff4bu+f->r[2]))=(uint8_t)(0x00000001u);
    /* 004b7cb0 */
    MI_U16((0xffffff92u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b7cb4 */
    MI_U16((0xffffff94u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b7cb8 */
    a=f->r[2];b=0x00bcd610u;t=mi_alu(f,a,b,32,1);
    /* 004b7cbe */
    if(mi_signed_less(f->flags))goto mi_004b7c91;
    /* 004b7cc0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7cc1 */
    f->r[7]=0x00000001u;
    /* 004b7cc6 */
    f->r[6]=0x00bcd786u;
mi_004b7ccb:;
    /* 004b7ccb */
    a=MI_U8((0x00000004u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b7ccf */
    if(!(f->flags&MI_ZF))goto mi_004b7da1;
    /* 004b7cd5 */
    a=MI_U8((0x0000002eu+f->r[6]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b7cd9 */
    if((f->flags&MI_ZF))goto mi_004b7da1;
    /* 004b7cdf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7ce1 */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004b7ce3 */
    if((f->flags&MI_CF))goto mi_004b7ce9;
    /* 004b7ce5 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004b7ce7 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b7d2a;
mi_004b7ce9:;
    /* 004b7ce9 */
    a=(f->r[0]&0xffu);b=0x0000002bu;t=mi_alu(f,a,b,8,1);
    /* 004b7ceb */
    if((f->flags&MI_CF))goto mi_004b7cf1;
    /* 004b7ced */
    a=(f->r[0]&0xffu);b=0x00000035u;t=mi_alu(f,a,b,8,1);
    /* 004b7cef */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b7d2a;
mi_004b7cf1:;
    /* 004b7cf1 */
    a=(f->r[0]&0xffu);b=0x00000039u;t=mi_alu(f,a,b,8,1);
    /* 004b7cf3 */
    if((f->flags&MI_ZF))goto mi_004b7d2a;
    /* 004b7cf5 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004b7cfa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7cfb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b7d00u;f->boundary(0x004b7cfbu,0x004ea9a0u,f);
    /* 004b7d00 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b7d03 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b7d05 */
    if(!(f->flags&MI_ZF))goto mi_004b7d2a;
    /* 004b7d07 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b7d09 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7d0b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7d0c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b7d11u;f->boundary(0x004b7d0cu,0x004ea9c0u,f);
    /* 004b7d11 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b7d14 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b7d16 */
    if(!(f->flags&MI_ZF))goto mi_004b7d2a;
    /* 004b7d18 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7d1a */
    a=(f->r[0]&0xffu);b=0x00000063u;t=mi_alu(f,a,b,8,1);
    /* 004b7d1c */
    if((f->flags&MI_ZF))goto mi_004b7d2a;
    /* 004b7d1e */
    a=(f->r[0]&0xffu);b=0x00000036u;t=mi_alu(f,a,b,8,1);
    /* 004b7d20 */
    if((f->flags&MI_ZF))goto mi_004b7d2a;
    /* 004b7d22 */
    a=(f->r[0]&0xffu);b=0x00000037u;t=mi_alu(f,a,b,8,1);
    /* 004b7d24 */
    if((f->flags&MI_ZF))goto mi_004b7d2a;
    /* 004b7d26 */
    a=(f->r[0]&0xffu);b=0x00000038u;t=mi_alu(f,a,b,8,1);
    /* 004b7d28 */
    if(!(f->flags&MI_ZF))goto mi_004b7da1;
mi_004b7d2a:;
    /* 004b7d2a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000056u+f->r[6]));
    /* 004b7d2e */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 004b7d31 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b7d34 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcb9f0u+f->r[0])))&0xffu)<<0);
    /* 004b7d3a */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b7d3c */
    MI_U8((0x00bcb9f0u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b7d42 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b7d44 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7d46 */
    f->r[5]=(0x00000000u+f->r[1]*8u);
    /* 004b7d4d */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004b7d4f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007090d0u+f->r[5]*8u)))&0xffffu)<<0);
    /* 004b7d57 */
    a=MI_U16((0x00bcba14u+f->r[0]));b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00bcba14u+f->r[0]))=(uint16_t)(t);
    /* 004b7d5e */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U8((0x00000013u+f->r[6])))&0xffffu)<<0);
    /* 004b7d63 */
    a=MI_U16((0x00bcba12u+f->r[0]));b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00bcba12u+f->r[0]))=(uint16_t)(t);
    /* 004b7d6a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000059u+f->r[6])))&0xffu)<<0);
    /* 004b7d6d */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b7d70 */
    if((f->flags&MI_ZF))goto mi_004b7d78;
    /* 004b7d72 */
    MI_U8((0x00bcb9cbu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
mi_004b7d78:;
    /* 004b7d78 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b7d7a */
    f->r[0]=(0x00bcb9d0u+f->r[0]);
mi_004b7d80:;
    /* 004b7d80 */
    a=MI_U16((0x00000000u+f->r[0]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b7d83 */
    if((f->flags&MI_ZF))goto mi_004b7d90;
    /* 004b7d85 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b7d86 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b7d89 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004b7d8c */
    if(mi_signed_less(f->flags))goto mi_004b7d80;
    /* 004b7d8e */
    goto mi_004b7da1;
mi_004b7d90:;
    /* 004b7d90 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 004b7d93 */
    MI_U8((0x00000058u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b7d96 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[2]*8u);
    /* 004b7d99 */
    MI_U16((0x00bcb9d0u+f->r[0]*2u))=(uint16_t)((f->r[7]&0xffffu));
mi_004b7da1:;
    /* 004b7da1 */
    a=f->r[6];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b7da7 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004b7da8 */
    a=f->r[6];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004b7dae */
    if(mi_signed_less(f->flags))goto mi_004b7ccb;
    /* 004b7db4 */
    MI_U32((0x00d0563cu))=(uint32_t)(f->r[3]);
    /* 004b7dba */
    MI_U32((0x00d05638u))=(uint32_t)(f->r[3]);
    /* 004b7dc0 */
    MI_U32((0x00d05634u))=(uint32_t)(f->r[3]);
    /* 004b7dc6 */
    MI_U32((0x00d05630u))=(uint32_t)(f->r[3]);
    /* 004b7dcc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00bca45cu);
    /* 004b7dd4 */
    f->r[6]=0x00bcba80u;
mi_004b7dd9:;
    /* 004b7dd9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffd8u+f->r[6])))&0xffu)<<0);
    /* 004b7ddc */
    f->r[5]=(0xffffffd8u+f->r[6]);
    /* 004b7ddf */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b7de1 */
    if(!(f->flags&MI_ZF))goto mi_004b7f1e;
    /* 004b7de7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000041u+f->r[6])))&0xffu)<<0);
    /* 004b7dea */
    f->r[7]=(0xffffffe0u+f->r[6]);
    /* 004b7ded */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7def */
    if(!(f->flags&MI_ZF))goto mi_004b7f1e;
    /* 004b7df5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000001au+f->r[6])))&0xffu)<<0);
    /* 004b7df8 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7dfa */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7dfc */
    if((f->flags&MI_ZF))goto mi_004b7ea1;
    /* 004b7e02 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7e04 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b7f1e;
    /* 004b7e0a */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000032u+f->r[6]));
    /* 004b7e0e */
    MI_U16((0x00000032u+f->r[6]))=(uint16_t)(0x00000001u);
    /* 004b7e14 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7e16 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b7e18 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7e1a */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b7e4d;
    /* 004b7e1c */
    f->r[2]=f->r[7];
mi_004b7e1e:;
    /* 004b7e1e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004b7e21 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b7e23 */
    if((f->flags&MI_ZF))goto mi_004b7e41;
    /* 004b7e25 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b7e28 */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 004b7e2b */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004b7e2d */
    a=MI_U8((0x00bcd609u+f->r[3]*4u));b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004b7e35 */
    if((f->flags&MI_ZF))goto mi_004b7e3f;
    /* 004b7e37 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004b7e39 */
    MI_U16((0x00000032u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b7e3d */
    goto mi_004b7e41;
mi_004b7e3f:;
    /* 004b7e3f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_004b7e41:;
    /* 004b7e41 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b7e43 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b7e44 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7e46 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b7e49 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b7e4b */
    if(mi_signed_less(f->flags))goto mi_004b7e1e;
mi_004b7e4d:;
    /* 004b7e4d */
    f->r[0]=MI_U32((0x00d05634u));
    /* 004b7e52 */
    f->r[2]=MI_U32((0x00d05630u));
    /* 004b7e58 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b7e5a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b7e5b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7e5d */
    MI_U32((0x00d05634u))=(uint32_t)(f->r[0]);
    /* 004b7e62 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b7e64 */
    a=MI_U16((0xfffffffcu+f->r[7]));b=0x0000000du;t=mi_alu(f,a,b,16,1);
    /* 004b7e69 */
    MI_U32((0x00d05630u))=(uint32_t)(f->r[2]);
    /* 004b7e6f */
    if(!(f->flags&MI_ZF))goto mi_004b7f1e;
    /* 004b7e75 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b7e77 */
    if(!(f->flags&MI_ZF))goto mi_004b7e8d;
    /* 004b7e79 */
    a=MI_U16((0x00000032u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004b7e7e */
    if(!(f->flags&MI_ZF))goto mi_004b7e8d;
    /* 004b7e80 */
    a=0x0000009eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7e85 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b7e8au;f->boundary(0x004b7e85u,0x0047d350u,f);
    /* 004b7e8a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004b7e8d:;
    /* 004b7e8d */
    f->r[0]=MI_U32((0x00d05630u));
    /* 004b7e92 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b7e94 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004b7e96 */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 004b7e98 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b7e9a */
    MI_U32((0x00d05630u))=(uint32_t)(f->r[0]);
    /* 004b7e9f */
    goto mi_004b7f1e;
mi_004b7ea1:;
    /* 004b7ea1 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7ea3 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b7ec7;
    /* 004b7ea5 */
    f->r[7]=MI_U32((0x00d0563cu));
    /* 004b7eab */
    f->r[1]=MI_U32((0x00d05638u));
    /* 004b7eb1 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004b7eb6 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004b7eb7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b7eb9 */
    MI_U32((0x00d0563cu))=(uint32_t)(f->r[7]);
    /* 004b7ebf */
    MI_U32((0x00d05638u))=(uint32_t)(f->r[1]);
    /* 004b7ec5 */
    goto mi_004b7f1e;
mi_004b7ec7:;
    /* 004b7ec7 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004b7ecb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b7ecd */
    a=f->r[1];b=0x00bca45cu;t=mi_alu(f,a,b,32,1);
    /* 004b7ed3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b7eee;
    /* 004b7ed5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000050u+f->r[6]));
    /* 004b7ed9 */
    f->r[1]=0x00bca45cu;
mi_004b7ede:;
    /* 004b7ede */
    a=MI_U32((0x00000000u+f->r[1]));b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004b7ee0 */
    if((f->flags&MI_ZF))goto mi_004b7f0f;
    /* 004b7ee2 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004b7ee6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b7ee7 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b7eea */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004b7eec */
    if(mi_signed_less(f->flags))goto mi_004b7ede;
mi_004b7eee:;
    /* 004b7eee */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000050u+f->r[6]));
    /* 004b7ef2 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004b7ef6 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004b7ef8 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004b7efc */
    MI_U8((0x00bca428u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 004b7f03 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b7f04 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b7f07 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004b7f0b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004b7f0f:;
    /* 004b7f0f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7f10 */
    a=0x00000090u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7f15 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7f16 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b7f1bu;f->boundary(0x004b7f16u,0x0040f8f0u,f);
    /* 004b7f1b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004b7f1e:;
    /* 004b7f1e */
    a=f->r[6];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b7f24 */
    a=f->r[6];b=0x00bcd610u;t=mi_alu(f,a,b,32,1);
    /* 004b7f2a */
    if(mi_signed_less(f->flags))goto mi_004b7dd9;
    /* 004b7f30 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004b7f35 */
    f->r[7]=0x00000001u;
    /* 004b7f3a */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004b7f40 */
    f->r[6]=0x00bcba9au;
    /* 004b7f45 */
    MI_U8((0x00ea5eddu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b7f4b */
    MI_U8((0x00ea5edfu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b7f51 */
    MI_U8((0x00ea5edcu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b7f57 */
    MI_U8((0x00ea5edau+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b7f5d */
    MI_U8((0x00ea5edbu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b7f63 */
    MI_U8((0x00ea5ed5u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004b7f69 */
    MI_U8((0x00ea5ed6u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
mi_004b7f6f:;
    /* 004b7f6f */
    a=MI_U8((0xffffffbeu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b7f73 */
    if(!(f->flags&MI_ZF))goto mi_004b801c;
    /* 004b7f79 */
    a=MI_U8((0x00000000u+f->r[6]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7f7b */
    if(!(f->flags&MI_ZF))goto mi_004b7fa8;
    /* 004b7f7d */
    f->r[5]=MI_U32((0x00000010u+f->r[4]));
    /* 004b7f81 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b7f83 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b7f85 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b801c;
    /* 004b7f8b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000036u+f->r[6]));
    /* 004b7f8f */
    f->r[1]=0x00bca45cu;
mi_004b7f94:;
    /* 004b7f94 */
    a=f->r[2];b=MI_U32((0x00000000u+f->r[1]));t=mi_alu(f,a,b,32,1);
    /* 004b7f96 */
    if(!(f->flags&MI_ZF))goto mi_004b7f9e;
    /* 004b7f98 */
    MI_U8((0x00bca428u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
mi_004b7f9e:;
    /* 004b7f9e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b7f9f */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b7fa2 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b7fa4 */
    if(mi_signed_less(f->flags))goto mi_004b7f94;
    /* 004b7fa6 */
    goto mi_004b801c;
mi_004b7fa8:;
    /* 004b7fa8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b7fa9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b7faeu;f->boundary(0x004b7fa9u,0x0040b5e0u,f);
    /* 004b7fae */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b7fb1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b7fb3 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004b7fb8 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b7fbb */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004b7fc1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea5edcu+f->r[0])))&0xffu)<<0);
    /* 004b7fc7 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b7fc9 */
    a=(f->r[1]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7fcb */
    MI_U8((0x00ea5edcu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004b7fd1 */
    if((f->flags&MI_ZF))goto mi_004b7fdb;
    /* 004b7fd3 */
    a=MI_U8((0x00ea5edau+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;MI_U8((0x00ea5edau+f->r[0]))=(uint8_t)(t);
    /* 004b7fd9 */
    goto mi_004b7fe1;
mi_004b7fdb:;
    /* 004b7fdb */
    a=MI_U8((0x00ea5edbu+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;MI_U8((0x00ea5edbu+f->r[0]))=(uint8_t)(t);
mi_004b7fe1:;
    /* 004b7fe1 */
    a=MI_U8((0x00000025u+f->r[6]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7fe4 */
    if((f->flags&MI_ZF))goto mi_004b800b;
    /* 004b7fe6 */
    a=MI_U8((0xffffffe6u+f->r[6]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7fe9 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b800b;
    /* 004b7feb */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea5eddu+f->r[0])))&0xffu)<<0);
    /* 004b7ff1 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b7ff3 */
    a=(f->r[1]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b7ff5 */
    MI_U8((0x00ea5eddu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004b7ffb */
    if((f->flags&MI_ZF))goto mi_004b8005;
    /* 004b7ffd */
    a=MI_U8((0x00ea5ed5u+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;MI_U8((0x00ea5ed5u+f->r[0]))=(uint8_t)(t);
    /* 004b8003 */
    goto mi_004b800b;
mi_004b8005:;
    /* 004b8005 */
    a=MI_U8((0x00ea5ed6u+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;MI_U8((0x00ea5ed6u+f->r[0]))=(uint8_t)(t);
mi_004b800b:;
    /* 004b800b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0xffffffe6u+f->r[6])))&0xffu)<<0);
    /* 004b800e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea5edfu+f->r[0])))&0xffu)<<0);
    /* 004b8014 */
    a=(f->r[2]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b8016 */
    MI_U8((0x00ea5edfu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_004b801c:;
    /* 004b801c */
    a=f->r[6];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b8022 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004b8023 */
    a=f->r[6];b=0x00bcd62au;t=mi_alu(f,a,b,32,1);
    /* 004b8029 */
    if(mi_signed_less(f->flags))goto mi_004b7f6f;
    /* 004b802f */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 004b8033 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b8035 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b8037 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b8038 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b8072;
    /* 004b803a */
    f->r[2]=0x00bca45cu;
mi_004b803f:;
    /* 004b803f */
    a=MI_U8((0x00bca428u+f->r[1]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b8045 */
    if((f->flags&MI_ZF))goto mi_004b806a;
    /* 004b8047 */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 004b8049 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b804b */
    if(mi_signed_less(f->flags))goto mi_004b806a;
    /* 004b804d */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 004b8053 */
    if(!mi_signed_less(f->flags))goto mi_004b806a;
    /* 004b8055 */
    f->r[7]=f->r[0];
    /* 004b8057 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004b805a */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004b805c */
    f->r[0]=(0x00785d74u+f->r[7]*4u);
    /* 004b8063 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b8065 */
    if((f->flags&MI_ZF))goto mi_004b806a;
    /* 004b8067 */
    MI_U8((0x00000032u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
mi_004b806a:;
    /* 004b806a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b806b */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b806e */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004b8070 */
    if(mi_signed_less(f->flags))goto mi_004b803f;
mi_004b8072:;
    /* 004b8072 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b8073 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b8074 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b8075 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8078 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004c52d0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c52d0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004c52d4 */
    f->r[1]=f->r[0];
    /* 004c52d6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004c52d9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c52db */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a81u+f->r[1]*8u)))&0xffu)<<0);
    /* 004c52e2 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004c52e4 */
    if(!(f->flags&MI_ZF))goto mi_004c52e9;
    /* 004c52e6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004c52e8 */
    f->r[4]+=4;return;
mi_004c52e9:;
    /* 004c52e9 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004c52ed */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004c52f2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004c52f4 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c52f7 */
    f->r[4]+=4;return;
}

/* Complete native housing evolution, consumption and footprint helpers. */
static __inline void mi_idiv32(struct restore_building_control_frame *f,uint32_t divisor_bits)
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


void restore_building_control_C_004528b0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004528b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004528b5u;f->boundary(0x004528b0u,0x00454d70u,f);
    /* 004528b5 */
    f->r[1]=0x00000001u;
    /* 004528ba */
    MI_U32((0x0078d588u))=(uint32_t)(0x00000000u);
    /* 004528c4 */
    MI_U32((0x0078d838u))=(uint32_t)(f->r[1]);
mi_004528ca:;
    /* 004528ca */
    f->r[0]=f->r[1];
    /* 004528cc */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004528cf */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004528d1 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004528d4 */
    a=MI_U8((0x007999d8u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004528db */
    if(!(f->flags&MI_ZF))goto mi_00452954;
    /* 004528dd */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[0])))&0xffffu)<<0);
    /* 004528e4 */
    a=(f->r[0]&0xffffu);b=0x0000000au;t=mi_alu(f,a,b,16,1);
    /* 004528e8 */
    if(mi_signed_less(f->flags))goto mi_00452954;
    /* 004528ea */
    a=(f->r[0]&0xffffu);b=0x0000001du;t=mi_alu(f,a,b,16,1);
    /* 004528ee */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00452954;
    /* 004528f0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004528f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004528f6u;f->boundary(0x004528f1u,0x0046b740u,f);
    /* 004528f6 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004528fb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004528fe */
    f->r[1]=f->r[0];
    /* 00452900 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0x00000000u);
    /* 0045290a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045290d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045290f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[1]*8u));
    /* 00452917 */
    p=MI_U32((0x005da3d4u+f->r[2]*4u));f->r[4]-=4;MI_U32(f->r[4])=0x0045291eu;f->boundary(0x00452917u,p,f);
    /* 0045291e */
    f->r[0]=MI_U32((0x0078d7c4u));
    /* 00452923 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452925 */
    if(!(f->flags&MI_ZF))goto mi_00452935;
    /* 00452927 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 0045292c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045292d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452932u;f->boundary(0x0045292du,0x00454b40u,f);
    /* 00452932 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452935:;
    /* 00452935 */
    f->r[0]=MI_U32((0x0078d7c4u));
    /* 0045293a */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452940 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 00452943 */
    if(!(f->flags&MI_ZF))goto mi_00452954;
    /* 00452945 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452946 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045294bu;f->boundary(0x00452946u,0x00454b40u,f);
    /* 0045294b */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452951 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452954:;
    /* 00452954 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00452955 */
    a=f->r[1];b=0x00000fa0u;t=mi_alu(f,a,b,32,1);
    /* 0045295b */
    MI_U32((0x0078d838u))=(uint32_t)(f->r[1]);
    /* 00452961 */
    if(mi_signed_less(f->flags))goto mi_004528ca;
    /* 00452967 */
    f->r[0]=MI_U32((0x0078d588u));
    /* 0045296c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045296e */
    if((f->flags&MI_ZF))goto mi_0045297a;
    /* 00452970 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452975u;f->boundary(0x00452970u,0x00517370u,f);
    /* 00452975 */
    f->boundary(0x00452975u,0x00517660u,f);return;
mi_0045297a:;
    /* 0045297a */
    f->r[4]+=4;return;
}

static const uint8_t mi_rom_00454d58[]={0x18,0x4c,0x45,0x00,0x46,0x4c,0x45,0x00,0x2f,0x4c,0x45,0x00,0x46,0x4c,0x45,0x00};
static __inline uint32_t mi_read_00454b40(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00454d58u&&width<=16u&&(offset=address-0x00454d58u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00454d58[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


void restore_building_control_C_00454b40(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00454b40 */
    f->r[0]=mi_read_00454b40((0x00000004u+f->r[4]),4u);
    /* 00454b44 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00454b47 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00454b48 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00454b49 */
    f->r[6]=f->r[0];
    /* 00454b4b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00454b4c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00454b4f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00454b51 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00454b54 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00454b40((0x007999eau+f->r[6]),2u);
    /* 00454b5b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_00454b40((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 00454b61 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00454b64 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00454b67 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00454b6a */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454b6d */
    f->r[0]=mi_read_00454b40((0x0070cdf8u+f->r[1]),4u);
    /* 00454b73 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00454b78;
    /* 00454b75 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
mi_00454b78:;
    /* 00454b78 */
    f->r[7]=mi_read_00454b40((0x0070cdfcu+f->r[1]),4u);
    /* 00454b7e */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454b81 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00454b85 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00454b8e;
    /* 00454b87 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00454b8a */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[7]);
mi_00454b8e:;
    /* 00454b8e */
    f->r[3]=mi_read_00454b40((0x0070ce00u+f->r[1]),4u);
    /* 00454b94 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454b97 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00454b9b */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00454ba4;
    /* 00454b9d */
    a=f->r[3];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 00454ba0 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[3]);
mi_00454ba4:;
    /* 00454ba4 */
    f->r[1]=mi_read_00454b40((0x0070ce04u+f->r[1]),4u);
    /* 00454baa */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00454bad */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00454bb1 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00454bc0;
    /* 00454bb3 */
    f->r[2]=(0x00000000u+f->r[1]*4u);
    /* 00454bba */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00454bbe */
    f->r[1]=f->r[2];
mi_00454bc0:;
    /* 00454bc0 */
    f->r[2]=f->r[0];
    /* 00454bc2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00454bc3 */
    a=f->r[2];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454bc9 */
    if(!(f->flags&MI_SF))goto mi_00454bd0;
    /* 00454bcb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00454bcc */
    a=f->r[2];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 00454bcf */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_00454bd0:;
    /* 00454bd0 */
    a=f->r[7];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 00454bd6 */
    if(!(f->flags&MI_SF))goto mi_00454bdd;
    /* 00454bd8 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00454bd9 */
    a=f->r[7];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 00454bdc */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_00454bdd:;
    /* 00454bdd */
    a=f->r[3];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 00454be3 */
    if(!(f->flags&MI_SF))goto mi_00454bea;
    /* 00454be5 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00454be6 */
    a=f->r[3];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 00454be9 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
mi_00454bea:;
    /* 00454bea */
    f->r[5]=f->r[1];
    /* 00454bec */
    a=f->r[5];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 00454bf2 */
    if(!(f->flags&MI_SF))goto mi_00454bf9;
    /* 00454bf4 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00454bf5 */
    a=f->r[5];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[5]=t;
    /* 00454bf8 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
mi_00454bf9:;
    /* 00454bf9 */
    f->r[1]=mi_read_00454b40((0x0078d7c4u),4u);
    /* 00454bff */
    a=f->r[1];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00454c05 */
    if(!(f->flags&MI_SF))goto mi_00454c0c;
    /* 00454c07 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00454c08 */
    a=f->r[1];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 00454c0b */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_00454c0c:;
    /* 00454c0c */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00454c0f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00454c58;
    /* 00454c11 */
    switch(mi_read_00454b40((0x00454d58u+f->r[1]*4u),4u)){case 0x00454c18u:goto mi_00454c18;case 0x00454c2fu:goto mi_00454c2f;case 0x00454c46u:goto mi_00454c46;default:abort();}
mi_00454c18:;
    /* 00454c18 */
    f->r[1]=0x00000001u;
    /* 00454c1d */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00454c25 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00454c29 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00454c2d */
    goto mi_00454c58;
mi_00454c2f:;
    /* 00454c2f */
    f->r[1]=0x00000001u;
    /* 00454c34 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00454c3c */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00454c40 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00454c44 */
    goto mi_00454c58;
mi_00454c46:;
    /* 00454c46 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00454c48 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 00454c50 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00454c54 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00454c58:;
    /* 00454c58 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00454c5a */
    if((f->flags&MI_ZF))goto mi_00454c62;
    /* 00454c5c */
    f->r[1]=mi_read_00454b40((0x00000018u+f->r[4]+f->r[2]*4u),4u);
    /* 00454c60 */
    goto mi_00454c6d;
mi_00454c62:;
    /* 00454c62 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00454c63 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454c66 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454c68 */
    f->r[1]=f->r[0];
    /* 00454c6a */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
mi_00454c6d:;
    /* 00454c6d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00454c6f */
    if((f->flags&MI_ZF))goto mi_00454c77;
    /* 00454c71 */
    f->r[7]=mi_read_00454b40((0x00000018u+f->r[4]+f->r[7]*4u),4u);
    /* 00454c75 */
    goto mi_00454c86;
mi_00454c77:;
    /* 00454c77 */
    f->r[0]=mi_read_00454b40((0x0000002cu+f->r[4]),4u);
    /* 00454c7b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00454c7c */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454c7f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454c81 */
    f->r[7]=f->r[0];
    /* 00454c83 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[7]=t;
mi_00454c86:;
    /* 00454c86 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00454c88 */
    if((f->flags&MI_ZF))goto mi_00454c90;
    /* 00454c8a */
    f->r[3]=mi_read_00454b40((0x00000018u+f->r[4]+f->r[3]*4u),4u);
    /* 00454c8e */
    goto mi_00454c9f;
mi_00454c90:;
    /* 00454c90 */
    f->r[0]=mi_read_00454b40((0x00000010u+f->r[4]),4u);
    /* 00454c94 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00454c95 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454c98 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454c9a */
    f->r[3]=f->r[0];
    /* 00454c9c */
    a=f->r[3];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[3]=t;
mi_00454c9f:;
    /* 00454c9f */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00454ca1 */
    if((f->flags&MI_ZF))goto mi_00454ca9;
    /* 00454ca3 */
    f->r[0]=mi_read_00454b40((0x00000018u+f->r[4]+f->r[5]*4u),4u);
    /* 00454ca7 */
    goto mi_00454cb6;
mi_00454ca9:;
    /* 00454ca9 */
    f->r[0]=mi_read_00454b40((0x00000014u+f->r[4]),4u);
    /* 00454cad */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00454cae */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00454cb1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00454cb3 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
mi_00454cb6:;
    /* 00454cb6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00454cb8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454cdc;
    /* 00454cba */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_00454b40((0x00799a3cu+f->r[6]),2u))&0xffffu)<<0);
    /* 00454cc1 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 00454cc4 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00454cc6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454cd3;
    /* 00454cc8 */
    MI_U16((0x00799a3cu+f->r[6]))=(uint16_t)(0x00000000u);
    /* 00454cd1 */
    goto mi_00454cdc;
mi_00454cd3:;
    /* 00454cd3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00454cd5 */
    MI_U16((0x00799a3cu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
mi_00454cdc:;
    /* 00454cdc */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00454cde */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00454ce0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454d02;
    /* 00454ce2 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_00454b40((0x00799a3eu+f->r[6]),2u))&0xffffu)<<0);
    /* 00454ce9 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00454cec */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00454cee */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454cf9;
    /* 00454cf0 */
    MI_U16((0x00799a3eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00454cf7 */
    goto mi_00454d02;
mi_00454cf9:;
    /* 00454cf9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00454cfb */
    MI_U16((0x00799a3eu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_00454d02:;
    /* 00454d02 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00454d04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00454d05 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454d27;
    /* 00454d07 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_00454b40((0x00799a40u+f->r[6]),2u))&0xffffu)<<0);
    /* 00454d0e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00454d11 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00454d13 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454d1e;
    /* 00454d15 */
    MI_U16((0x00799a40u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00454d1c */
    goto mi_00454d27;
mi_00454d1e:;
    /* 00454d1e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00454d20 */
    MI_U16((0x00799a40u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_00454d27:;
    /* 00454d27 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00454d29 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454d50;
    /* 00454d2b */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_00454b40((0x00799a42u+f->r[6]),2u))&0xffffu)<<0);
    /* 00454d32 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00454d35 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00454d37 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00454d47;
    /* 00454d39 */
    MI_U16((0x00799a42u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00454d40 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454d41 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454d42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454d43 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454d46 */
    f->r[4]+=4;return;
mi_00454d47:;
    /* 00454d47 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00454d49 */
    MI_U16((0x00799a42u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_00454d50:;
    /* 00454d50 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00454d51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00454d52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00454d53 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00454d56 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046b740(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046b740 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b741 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b742 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b743 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b747 */
    f->r[1]=f->r[6];
    /* 0046b749 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b74a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b74d */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b74f */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b752 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046b759 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046b760 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0046b763 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046b769 */
    MI_U8((0x00799a56u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0046b770 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[7]*2u);
    /* 0046b773 */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 0046b779 */
    f->r[5]=f->r[7];
    /* 0046b77b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046b77e */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046b780 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[0]*4u);
    /* 0046b784 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046b786 */
    if(!(f->flags&MI_ZF))goto mi_0046b794;
    /* 0046b788 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[2]*2u));
    /* 0046b790 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046b792 */
    if((f->flags&MI_ZF))goto mi_0046b7f7;
mi_0046b794:;
    /* 0046b794 */
    f->r[2]=MI_U32((0x0078d5c4u));
    /* 0046b79a */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0046b79f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046b7a0 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046b7a2 */
    MI_U32((0x0078d5c4u))=(uint32_t)(f->r[2]);
    /* 0046b7a8 */
    f->r[2]=f->r[7];
    /* 0046b7aa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046b7ac */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b7e5;
    /* 0046b7ae */
    f->r[7]=MI_U32((0x00bca2d0u));
mi_0046b7b4:;
    /* 0046b7b4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b7b6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0046b7b8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b7d3;
    /* 0046b7ba */
    f->r[6]=(0x00b4e6fcu+f->r[2]*2u);
mi_0046b7c1:;
    /* 0046b7c1 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0046b7c4 */
    a=f->r[5];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0046b7c8 */
    if((f->flags&MI_ZF))goto mi_0046b7fc;
    /* 0046b7ca */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046b7cb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046b7cc */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b7cf */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046b7d1 */
    if(mi_signed_less(f->flags))goto mi_0046b7c1;
mi_0046b7d3:;
    /* 0046b7d3 */
    f->r[6]=MI_U32((0x00bca2dcu));
    /* 0046b7d9 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0046b7de */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046b7df */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046b7e1 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046b7e3 */
    if(mi_signed_less(f->flags))goto mi_0046b7b4;
mi_0046b7e5:;
    /* 0046b7e5 */
    f->r[0]=MI_U32((0x0078d5c0u));
    /* 0046b7ea */
    MI_U8((0x007999d8u+f->r[1]))=(uint8_t)(0x00000004u);
    /* 0046b7f1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046b7f2 */
    MI_U32((0x0078d5c0u))=(uint32_t)(f->r[0]);
mi_0046b7f7:;
    /* 0046b7f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b7f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b7f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b7fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b7fb */
    f->r[4]+=4;return;
mi_0046b7fc:;
    /* 0046b7fc */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0046b801 */
    f->r[6]=f->r[2];
    /* 0046b803 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0046b805 */
    MI_U32((0x007999e4u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0046b80b */
    f->r[0]=f->r[6];
    /* 0046b80d */
    f->r[7]=0x000000e4u;
    /* 0046b812 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046b813 */
    a=f->r[7];mi_idiv32(f,a);
    /* 0046b815 */
    f->r[0]=0x8fb823efu;
    /* 0046b81a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b81b */
    MI_U16((0x007999deu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046b822 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046b824 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046b826 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b827 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0046b82a */
    f->r[0]=f->r[2];
    /* 0046b82c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b82d */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0046b830 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046b832 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b833 */
    MI_U16((0x007999e0u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046b83a */
    f->r[4]+=4;return;
}



void restore_building_control_C_0046b840(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046b840 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b841 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b842 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b843 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b847 */
    f->r[2]=f->r[7];
    /* 0046b849 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046b84c */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046b84e */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046b851 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dcu+f->r[2])))&0xffu)<<0);
    /* 0046b857 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b859 */
    if(!(f->flags&MI_ZF))goto mi_0046b92d;
    /* 0046b85f */
    f->r[5]=MI_U32((0x007999e4u+f->r[2]));
    /* 0046b865 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[5])))&0xffu)<<0);
    /* 0046b86b */
    a=(f->r[0]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0046b86d */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0046b86f */
    if(!(f->flags&MI_CF))goto mi_0046b92d;
    /* 0046b875 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0046b87d */
    f->r[6]=0x005da2f4u;
mi_0046b882:;
    /* 0046b882 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 0046b884 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b886 */
    a=MI_U8((0x00af598cu+f->r[0]*4u));b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0046b88e */
    if((f->flags&MI_ZF))goto mi_0046b8d7;
    /* 0046b890 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[0]*2u));
    /* 0046b898 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046b89a */
    if((f->flags&MI_ZF))goto mi_0046b8d3;
    /* 0046b89c */
    f->r[0]=f->r[1];
    /* 0046b89e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046b8a1 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b8a3 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046b8a6 */
    a=MI_U8((0x007999d8u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0046b8ad */
    if(!(f->flags&MI_ZF))goto mi_0046b8d7;
    /* 0046b8af */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 0046b8b5 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b8b7 */
    if((f->flags&MI_ZF))goto mi_0046b8d7;
    /* 0046b8b9 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[0])))&0xffffu)<<0);
    /* 0046b8c0 */
    a=(f->r[1]&0xffffu);b=MI_U16((0x007999eau+f->r[2]));t=mi_alu(f,a,b,16,1);
    /* 0046b8c7 */
    if(!(f->flags&MI_ZF))goto mi_0046b8d7;
    /* 0046b8c9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dcu+f->r[0])))&0xffu)<<0);
    /* 0046b8cf */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b8d1 */
    if(!(f->flags&MI_ZF))goto mi_0046b8d7;
mi_0046b8d3:;
    /* 0046b8d3 */
    a=MI_U32((0x00000010u+f->r[4]));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00000010u+f->r[4]))=(uint32_t)(t);
mi_0046b8d7:;
    /* 0046b8d7 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b8da */
    a=f->r[6];b=0x005da304u;t=mi_alu(f,a,b,32,1);
    /* 0046b8e0 */
    if(mi_signed_less(f->flags))goto mi_0046b882;
    /* 0046b8e2 */
    a=MI_U32((0x00000010u+f->r[4]));b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0046b8e7 */
    if(!(f->flags&MI_ZF))goto mi_0046b92d;
    /* 0046b8e9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[2]));
    /* 0046b8f0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[2]));
    /* 0046b8f7 */
    f->r[6]=MI_U32((0x005da2c4u));
    /* 0046b8fd */
    f->r[2]=MI_U32((0x005da2c8u));
    /* 0046b903 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b905 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b907 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b909 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b90a */
    MI_U32((0x0078d770u))=(uint32_t)(f->r[0]);
    /* 0046b90f */
    MI_U32((0x0078d76cu))=(uint32_t)(f->r[1]);
    /* 0046b915 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b91au;f->boundary(0x0046b915u,0x0046b9e0u,f);
    /* 0046b91a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b91b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b91c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b921u;f->boundary(0x0046b91cu,0x0046bda0u,f);
    /* 0046b921 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b924 */
    f->r[0]=0x00000001u;
    /* 0046b929 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b92a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b92b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b92c */
    f->r[4]+=4;return;
mi_0046b92d:;
    /* 0046b92d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b92e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b92f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b931 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b932 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046b480(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046b480 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0046b483 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b487 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b488 */
    f->r[2]=f->r[0];
    /* 0046b48a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b48b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046b48e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046b490 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b491 */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046b494 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b495 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0046b49d */
    f->r[0]=MI_U32((0x007999e4u+f->r[2]));
    /* 0046b4a3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046b4a7 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046b4ab */
    f->r[3]=0x005da2e4u;
mi_0046b4b0:;
    /* 0046b4b0 */
    f->r[6]=MI_U32((0x00000000u+f->r[3]));
    /* 0046b4b2 */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
    /* 0046b4b6 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0046b4ba */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b4bc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b4be */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046b4c0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b525;
    /* 0046b4c2 */
    f->r[7]=0x005da2f4u;
    /* 0046b4c7 */
    f->r[5]=f->r[1];
mi_0046b4c9:;
    /* 0046b4c9 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 0046b4cb */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b4cd */
    a=MI_U8((0x00af598cu+f->r[1]*4u));b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0046b4d5 */
    if((f->flags&MI_ZF))goto mi_0046b517;
    /* 0046b4d7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[1]*2u));
    /* 0046b4df */
    a=f->r[2];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0046b4e3 */
    if((f->flags&MI_ZF))goto mi_0046b516;
    /* 0046b4e5 */
    f->r[1]=f->r[2];
    /* 0046b4e7 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b4ea */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b4ec */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b4ef */
    a=MI_U8((0x007999d8u+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0046b4f6 */
    if(!(f->flags&MI_ZF))goto mi_0046b517;
    /* 0046b4f8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[1])))&0xffu)<<0);
    /* 0046b4fe */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b500 */
    if((f->flags&MI_ZF))goto mi_0046b517;
    /* 0046b502 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0046b506 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[1])))&0xffffu)<<0);
    /* 0046b50d */
    a=(f->r[1]&0xffffu);b=MI_U16((0x007999eau+f->r[2]));t=mi_alu(f,a,b,16,1);
    /* 0046b514 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046b517;
mi_0046b516:;
    /* 0046b516 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0046b517:;
    /* 0046b517 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046b51a */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0046b51b */
    if(!(f->flags&MI_ZF))goto mi_0046b4c9;
    /* 0046b51d */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0046b521 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
mi_0046b525:;
    /* 0046b525 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046b527 */
    if((f->flags&MI_ZF))goto mi_0046b66e;
    /* 0046b52d */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b531 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046b534 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046b535 */
    a=f->r[3];b=0x005da2f4u;t=mi_alu(f,a,b,32,1);
    /* 0046b53b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0046b53f */
    if(mi_signed_less(f->flags))goto mi_0046b4b0;
    /* 0046b545 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0046b54d */
    f->r[5]=0x005da2e4u;
mi_0046b552:;
    /* 0046b552 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 0046b556 */
    f->r[7]=MI_U32((0x00000000u+f->r[5]));
    /* 0046b559 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b55b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b55d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046b55f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b5c8;
    /* 0046b561 */
    f->r[7]=0x005da2f4u;
    /* 0046b566 */
    f->r[3]=f->r[1];
mi_0046b568:;
    /* 0046b568 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 0046b56a */
    f->r[2]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0046b56d */
    f->r[1]=MI_U32((0x00af598cu+f->r[2]*4u));
    /* 0046b574 */
    a=f->r[1];b=0xeefed77fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0046b57a */
    if(!(f->flags&MI_ZF))goto mi_0046b57d;
    /* 0046b57c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0046b57d:;
    /* 0046b57d */
    a=(f->r[1]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0046b580 */
    if((f->flags&MI_ZF))goto mi_0046b5c2;
    /* 0046b582 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[2]*2u));
    /* 0046b58a */
    a=f->r[2];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0046b58e */
    if((f->flags&MI_ZF))goto mi_0046b5c1;
    /* 0046b590 */
    f->r[1]=f->r[2];
    /* 0046b592 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b595 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b597 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b59a */
    a=MI_U8((0x007999d8u+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0046b5a1 */
    if(!(f->flags&MI_ZF))goto mi_0046b5c2;
    /* 0046b5a3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[1])))&0xffu)<<0);
    /* 0046b5a9 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b5ab */
    if((f->flags&MI_ZF))goto mi_0046b5c2;
    /* 0046b5ad */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0046b5b1 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[1])))&0xffffu)<<0);
    /* 0046b5b8 */
    a=(f->r[1]&0xffffu);b=MI_U16((0x007999eau+f->r[2]));t=mi_alu(f,a,b,16,1);
    /* 0046b5bf */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046b5c2;
mi_0046b5c1:;
    /* 0046b5c1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0046b5c2:;
    /* 0046b5c2 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046b5c5 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046b5c6 */
    if(!(f->flags&MI_ZF))goto mi_0046b568;
mi_0046b5c8:;
    /* 0046b5c8 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0046b5cc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046b5ce */
    if((f->flags&MI_ZF))goto mi_0046b6f0;
    /* 0046b5d4 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b5d8 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046b5db */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0046b5dc */
    a=f->r[5];b=0x005da2f4u;t=mi_alu(f,a,b,32,1);
    /* 0046b5e2 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0046b5e6 */
    if(mi_signed_less(f->flags))goto mi_0046b552;
    /* 0046b5ec */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0046b5f4 */
    f->r[5]=0x005da2e4u;
mi_0046b5f9:;
    /* 0046b5f9 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 0046b5fd */
    f->r[7]=MI_U32((0x00000000u+f->r[5]));
    /* 0046b600 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b602 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b604 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046b606 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b6bb;
    /* 0046b60c */
    f->r[7]=0x005da2f4u;
    /* 0046b611 */
    f->r[3]=f->r[1];
mi_0046b613:;
    /* 0046b613 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 0046b615 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[6]*1u);
    /* 0046b618 */
    f->r[1]=MI_U32((0x00af598cu+f->r[2]*4u));
    /* 0046b61f */
    a=f->r[1];b=0xeefed77fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0046b625 */
    if(!(f->flags&MI_ZF))goto mi_0046b628;
    /* 0046b627 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0046b628:;
    /* 0046b628 */
    a=(f->r[1]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0046b62b */
    if((f->flags&MI_ZF))goto mi_0046b6ab;
    /* 0046b62d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[2]*2u));
    /* 0046b635 */
    a=f->r[2];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0046b639 */
    if((f->flags&MI_ZF))goto mi_0046b6b0;
    /* 0046b63b */
    f->r[1]=f->r[2];
    /* 0046b63d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b640 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b642 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046b645 */
    a=MI_U8((0x007999d8u+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0046b64c */
    if(!(f->flags&MI_ZF))goto mi_0046b6b1;
    /* 0046b64e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[1])))&0xffu)<<0);
    /* 0046b654 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b656 */
    if((f->flags&MI_ZF))goto mi_0046b6b1;
    /* 0046b658 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[1])))&0xffffu)<<0);
    /* 0046b65f */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0046b663 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x007999eau+f->r[1]));t=mi_alu(f,a,b,16,1);
    /* 0046b66a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046b6b1;
    /* 0046b66c */
    goto mi_0046b6b0;
mi_0046b66e:;
    /* 0046b66e */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b672 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b673 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[2]));
    /* 0046b67a */
    f->r[6]=MI_U32((0x005da2c4u+f->r[0]*8u));
    /* 0046b681 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[2]));
    /* 0046b688 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b68a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b68b */
    MI_U32((0x0078d770u))=(uint32_t)(f->r[1]);
    /* 0046b691 */
    f->r[1]=MI_U32((0x005da2c8u+f->r[0]*8u));
    /* 0046b698 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046b69a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b69b */
    MI_U32((0x0078d76cu))=(uint32_t)(f->r[2]);
    /* 0046b6a1 */
    f->r[0]=0x00000001u;
    /* 0046b6a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b6a7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b6aa */
    f->r[4]+=4;return;
mi_0046b6ab:;
    /* 0046b6ab */
    a=(f->r[1]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,3);
    /* 0046b6ae */
    if((f->flags&MI_ZF))goto mi_0046b6b1;
mi_0046b6b0:;
    /* 0046b6b0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_0046b6b1:;
    /* 0046b6b1 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046b6b4 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046b6b5 */
    if(!(f->flags&MI_ZF))goto mi_0046b613;
mi_0046b6bb:;
    /* 0046b6bb */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0046b6bf */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046b6c1 */
    if((f->flags&MI_ZF))goto mi_0046b6f0;
    /* 0046b6c3 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b6c7 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046b6ca */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0046b6cb */
    a=f->r[5];b=0x005da2f4u;t=mi_alu(f,a,b,32,1);
    /* 0046b6d1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0046b6d5 */
    if(mi_signed_less(f->flags))goto mi_0046b5f9;
    /* 0046b6db */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0046b6df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b6e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b6e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b6e2 */
    MI_U8((0x00799a56u+f->r[2]))=(uint8_t)(0x00000001u);
    /* 0046b6e9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b6eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b6ec */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b6ef */
    f->r[4]+=4;return;
mi_0046b6f0:;
    /* 0046b6f0 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0046b6f4 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 0046b6f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b6f9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046b700 */
    f->r[6]=MI_U32((0x005da2c4u+f->r[0]*8u));
    /* 0046b707 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046b70e */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046b710 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b711 */
    MI_U32((0x0078d770u))=(uint32_t)(f->r[2]);
    /* 0046b717 */
    f->r[2]=MI_U32((0x005da2c8u+f->r[0]*8u));
    /* 0046b71e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046b720 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046b721 */
    MI_U32((0x0078d76cu))=(uint32_t)(f->r[1]);
    /* 0046b727 */
    f->r[0]=0x00000001u;
    /* 0046b72c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046b72d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046b730 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046b940(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046b940 */
    f->r[0]=MI_U32((0x0078d76cu));
    /* 0046b945 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b946 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0046b949 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0046b94c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046b94f */
    f->r[0]=MI_U32((0x0078d770u));
    /* 0046b954 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0046b957 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0046b95d */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 0046b961 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046b963 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046b965 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046b9d8;
    /* 0046b967 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b968 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b969 */
    f->r[7]=0x005da2f4u;
    /* 0046b96e */
    f->r[5]=f->r[0];
mi_0046b970:;
    /* 0046b970 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 0046b972 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b974 */
    a=MI_U8((0x00af598cu+f->r[0]*4u));b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0046b97c */
    if((f->flags&MI_ZF))goto mi_0046b9d0;
    /* 0046b97e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[0]*2u));
    /* 0046b986 */
    a=f->r[1];b=MI_U32((0x00000010u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0046b98a */
    if((f->flags&MI_ZF))goto mi_0046b9d0;
    /* 0046b98c */
    f->r[0]=f->r[1];
    /* 0046b98e */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046b991 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046b993 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046b996 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[0])))&0xffu)<<0);
    /* 0046b99c */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046b99e */
    if((f->flags&MI_ZF))goto mi_0046b9d0;
    /* 0046b9a0 */
    a=MI_U8((0x007999dcu+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0046b9a7 */
    if(!(f->flags&MI_ZF))goto mi_0046b9b1;
    /* 0046b9a9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b9aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b9afu;f->boundary(0x0046b9aau,0x0046be90u,f);
    /* 0046b9af */
    goto mi_0046b9cd;
mi_0046b9b1:;
    /* 0046b9b1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[0])))&0xffu)<<0);
    /* 0046b9b7 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0046b9b9 */
    if(!(f->flags&MI_ZF))goto mi_0046b9c3;
    /* 0046b9bb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b9bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b9c1u;f->boundary(0x0046b9bcu,0x0046c1c0u,f);
    /* 0046b9c1 */
    goto mi_0046b9cd;
mi_0046b9c3:;
    /* 0046b9c3 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0046b9c5 */
    if(!(f->flags&MI_ZF))goto mi_0046b9d0;
    /* 0046b9c7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b9c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046b9cdu;f->boundary(0x0046b9c8u,0x0046c500u,f);
mi_0046b9cd:;
    /* 0046b9cd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0046b9d0:;
    /* 0046b9d0 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046b9d3 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0046b9d4 */
    if(!(f->flags&MI_ZF))goto mi_0046b970;
    /* 0046b9d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046b9d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_0046b9d8:;
    /* 0046b9d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046b9d9 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046b9e0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046b9e0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b9e1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046b9e2 */
    f->r[1]=0x0000000cu;
    /* 0046b9e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046b9e9 */
    f->r[7]=0x0078d590u;
    /* 0046b9ee */
    f->r[2]=MI_U32((0x0078d770u));
    /* 0046b9f4 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 0046b9f6 */
    f->r[1]=MI_U32((0x0078d76cu));
    /* 0046b9fc */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0046b9fe */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0046ba01 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0046ba04 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0046ba07 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 0046ba0a */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 0046ba10 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046ba12 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0046ba16 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046ba18 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046baa5;
    /* 0046ba1e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ba1f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ba20 */
    f->r[3]=0x005da2f4u;
    /* 0046ba25 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0046ba29:;
    /* 0046ba29 */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 0046ba2b */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046ba2d */
    a=MI_U8((0x00af598cu+f->r[1]*4u));b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0046ba35 */
    if((f->flags&MI_ZF))goto mi_0046ba90;
    /* 0046ba37 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[1]*2u));
    /* 0046ba3f */
    a=f->r[1];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0046ba43 */
    if((f->flags&MI_ZF))goto mi_0046ba90;
    /* 0046ba45 */
    f->r[2]=f->r[1];
    /* 0046ba47 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046ba4a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046ba4c */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0046ba4f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999ddu+f->r[2])))&0xffu)<<0);
    /* 0046ba55 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046ba57 */
    if((f->flags&MI_ZF))goto mi_0046ba90;
    /* 0046ba59 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[2]));
    /* 0046ba60 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046ba62 */
    f->r[1]=0x0078d590u;
    /* 0046ba67 */
    f->r[6]=(0x00799a2cu+f->r[2]);
mi_0046ba6d:;
    /* 0046ba6d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0046ba70 */
    a=MI_U32((0x00000000u+f->r[1]));b=f->r[0];t=mi_alu(f,a,b,32,0);MI_U32((0x00000000u+f->r[1]))=(uint32_t)(t);
    /* 0046ba72 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046ba75 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046ba78 */
    a=f->r[1];b=0x0078d5c0u;t=mi_alu(f,a,b,32,1);
    /* 0046ba7e */
    if(mi_signed_less(f->flags))goto mi_0046ba6d;
    /* 0046ba80 */
    MI_U16((0x007999f4u+f->r[2]))=(uint16_t)(0x00000000u);
    /* 0046ba89 */
    MI_U8((0x007999d8u+f->r[2]))=(uint8_t)(0x00000005u);
mi_0046ba90:;
    /* 0046ba90 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0046ba94 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046ba97 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046ba98 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046ba9c */
    if(!(f->flags&MI_ZF))goto mi_0046ba29;
    /* 0046ba9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ba9f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046baa0 */
    f->r[0]=f->r[5];
    /* 0046baa2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046baa3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046baa4 */
    f->r[4]+=4;return;
mi_0046baa5:;
    /* 0046baa5 */
    f->r[0]=f->r[5];
    /* 0046baa7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046baa8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046baa9 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046bab0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046bab0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bab1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bab2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bab3 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0046bab7 */
    f->r[6]=f->r[7];
    /* 0046bab9 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000014u+f->r[4])))&0xffffu)<<0);
    /* 0046babe */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bac1 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046bac3 */
    f->r[2]=0x00000002u;
    /* 0046bac8 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bacb */
    f->r[1]=0x0078d590u;
    /* 0046bad0 */
    MI_U16((0x007999e8u+f->r[6]))=(uint16_t)(0x00000014u);
    /* 0046bad9 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)(0x0000000au);
    /* 0046bae2 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046bae8 */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046baee */
    a=MI_U16((0x007999f4u+f->r[6]));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x007999f4u+f->r[6]))=(uint16_t)(t);
    /* 0046baf5 */
    f->r[0]=(0x00799a2cu+f->r[6]);
mi_0046bafb:;
    /* 0046bafb */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 0046bafe */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bb01 */
    a=MI_U16((0x00000000u+f->r[0]));b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00000000u+f->r[0]))=(uint16_t)(t);
    /* 0046bb04 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bb06 */
    a=f->r[1];b=0x0078d5c0u;t=mi_alu(f,a,b,32,1);
    /* 0046bb0c */
    if(mi_signed_less(f->flags))goto mi_0046bafb;
    /* 0046bb0e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046bb15 */
    f->r[1]=MI_U32((0x007999e4u+f->r[6]));
    /* 0046bb1b */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046bb1e */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[1])))&0xffu)<<0);
    /* 0046bb24 */
    f->r[2]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046bb2a */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 0046bb2d */
    f->r[1]=MI_U32((0x00f55bb4u+f->r[2]*8u));
    /* 0046bb34 */
    f->r[2]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046bb3a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bb41 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046bb43 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bb44 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bb4b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bb4c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bb4d */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046bb4f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bb54u;f->boundary(0x0046bb4fu,0x0047c480u,f);
    /* 0046bb54 */
    f->r[0]=MI_U32((0x0078d76cu));
    /* 0046bb59 */
    f->r[1]=MI_U32((0x0078d770u));
    /* 0046bb5f */
    MI_U16((0x007999deu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0046bb66 */
    MI_U16((0x007999e0u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0046bb6d */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0046bb70 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bb72 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bb73 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0046bb76 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0046bb7c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046bb7f */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0046bb82 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bb84 */
    MI_U32((0x007999e4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0046bb8a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046bb8c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 0046bb92 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bb99 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bba0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bba1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bba2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bba3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bba4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bba5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bbaau;f->boundary(0x0046bba5u,0x0047bf50u,f);
    /* 0046bbaa */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046bbad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046bbae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046bbaf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046bbb0 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046bbc0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046bbc0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bbc1 */
    f->r[3]=MI_U32((0x00000008u+f->r[4]));
    /* 0046bbc5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bbc6 */
    f->r[6]=f->r[3];
    /* 0046bbc8 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bbcb */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046bbcd */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000003u)&0xffu)<<0);
    /* 0046bbcf */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bbd2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bbd3 */
    f->r[1]=0x0078d590u;
    /* 0046bbd8 */
    MI_U16((0x007999e8u+f->r[6]))=(uint16_t)(0x00000018u);
    /* 0046bbe1 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)(0x0000000eu);
    /* 0046bbea */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0046bbf0 */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0046bbf6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000014u+f->r[4])))&0xffffu)<<0);
    /* 0046bbfb */
    a=MI_U16((0x007999f4u+f->r[6]));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x007999f4u+f->r[6]))=(uint16_t)(t);
    /* 0046bc02 */
    f->r[0]=(0x00799a2cu+f->r[6]);
mi_0046bc08:;
    /* 0046bc08 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 0046bc0b */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bc0e */
    a=MI_U16((0x00000000u+f->r[0]));b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00000000u+f->r[0]))=(uint16_t)(t);
    /* 0046bc11 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bc14 */
    a=f->r[1];b=0x0078d5c0u;t=mi_alu(f,a,b,32,1);
    /* 0046bc1a */
    if(mi_signed_less(f->flags))goto mi_0046bc08;
    /* 0046bc1c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046bc23 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bc2a */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046bc2d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bc2e */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046bc34 */
    f->r[7]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 0046bc3b */
    f->r[1]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046bc41 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bc48 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bc49 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bc4a */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046bc4c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bc51u;f->boundary(0x0046bc4cu,0x0047c480u,f);
    /* 0046bc51 */
    f->r[0]=MI_U32((0x0078d76cu));
    /* 0046bc56 */
    f->r[1]=MI_U32((0x0078d770u));
    /* 0046bc5c */
    MI_U16((0x007999deu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0046bc63 */
    MI_U16((0x007999e0u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0046bc6a */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0046bc6d */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bc6f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bc70 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0046bc73 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0046bc79 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046bc7c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0046bc7f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bc81 */
    MI_U32((0x007999e4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0046bc87 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046bc89 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 0046bc8f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bc96 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bc9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bc9e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bc9f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bca0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bca1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bca2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bca7u;f->boundary(0x0046bca2u,0x0047bf50u,f);
    /* 0046bca7 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046bcaa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046bcab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046bcac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046bcad */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046bcb0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046bcb0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bcb1 */
    f->r[3]=MI_U32((0x00000008u+f->r[4]));
    /* 0046bcb5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bcb6 */
    f->r[6]=f->r[3];
    /* 0046bcb8 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bcbb */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000010u+f->r[4])))&0xffffu)<<0);
    /* 0046bcc0 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046bcc2 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bcc5 */
    f->r[2]=0x00000004u;
    /* 0046bcca */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bccb */
    MI_U16((0x007999e8u+f->r[6]))=(uint16_t)(0x0000001cu);
    /* 0046bcd4 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)(0x00000012u);
    /* 0046bcdd */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046bce3 */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046bce9 */
    a=MI_U16((0x007999f4u+f->r[6]));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x007999f4u+f->r[6]))=(uint16_t)(t);
    /* 0046bcf0 */
    f->r[1]=0x0078d590u;
    /* 0046bcf5 */
    f->r[0]=(0x00799a2cu+f->r[6]);
mi_0046bcfb:;
    /* 0046bcfb */
    f->r[7]=(f->r[7]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 0046bcfe */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bd00 */
    a=MI_U16((0x00000000u+f->r[0]));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00000000u+f->r[0]))=(uint16_t)(t);
    /* 0046bd03 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bd06 */
    a=f->r[1];b=0x0078d5c0u;t=mi_alu(f,a,b,32,1);
    /* 0046bd0c */
    if(mi_signed_less(f->flags))goto mi_0046bcfb;
    /* 0046bd0e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046bd15 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bd1c */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046bd1f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd20 */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046bd26 */
    f->r[7]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 0046bd2d */
    f->r[1]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046bd33 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bd3a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd3b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd3c */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046bd3e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bd43u;f->boundary(0x0046bd3eu,0x0047c480u,f);
    /* 0046bd43 */
    f->r[0]=MI_U32((0x0078d76cu));
    /* 0046bd48 */
    f->r[1]=MI_U32((0x0078d770u));
    /* 0046bd4e */
    MI_U16((0x007999deu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0046bd55 */
    MI_U16((0x007999e0u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0046bd5c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0046bd5f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd61 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd62 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0046bd65 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0046bd6b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046bd6e */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0046bd71 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bd73 */
    MI_U32((0x007999e4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0046bd79 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046bd7b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 0046bd81 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bd88 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bd8f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd90 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd91 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd92 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd93 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bd94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bd99u;f->boundary(0x0046bd94u,0x0047bf50u,f);
    /* 0046bd99 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046bd9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046bd9d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046bd9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046bd9f */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046c1c0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046c1c0 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0046c1c3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c1c4 */
    f->r[3]=MI_U32((0x0000007cu+f->r[4]));
    /* 0046c1c8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c1c9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c1ca */
    f->r[6]=f->r[3];
    /* 0046c1cc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c1cd */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046c1d0 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046c1d2 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046c1d5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046c1d7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[6]));
    /* 0046c1de */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c1e2 */
    f->r[5]=(0x00799a2cu+f->r[6]);
mi_0046c1e8:;
    /* 0046c1e8 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 0046c1ec */
    f->r[0]=f->r[7];
    /* 0046c1ee */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046c1ef */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c1f2 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046c1f4 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046c1f7 */
    a=f->r[7];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0046c1fd */
    MI_U32((0x00000024u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[0]);
    /* 0046c201 */
    if(!(f->flags&MI_SF))goto mi_0046c208;
    /* 0046c203 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046c204 */
    a=f->r[7];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0046c207 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046c208:;
    /* 0046c208 */
    MI_U32((0x00000054u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[7]);
    /* 0046c20c */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c20f */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046c212 */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046c215 */
    if(mi_signed_less(f->flags))goto mi_0046c1e8;
    /* 0046c217 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 0046c21b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046c222 */
    f->r[0]=f->r[7];
    /* 0046c224 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c225 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046c226 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c229 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046c22b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046c232 */
    f->r[5]=f->r[0];
    /* 0046c234 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c235 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c236 */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[5]=t;
    /* 0046c239 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c23eu;f->boundary(0x0046c239u,0x0047c480u,f);
    /* 0046c23e */
    MI_U16((0x007999e8u+f->r[6]))=(uint16_t)(0x00000013u);
    /* 0046c247 */
    f->r[2]=0x00000001u;
    /* 0046c24c */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)(0x00000009u);
    /* 0046c255 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c258 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046c25e */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046c264 */
    a=f->r[7];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0046c26a */
    MI_U8((0x007999dcu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0046c271 */
    if(!(f->flags&MI_SF))goto mi_0046c278;
    /* 0046c273 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046c274 */
    a=f->r[7];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0046c277 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046c278:;
    /* 0046c278 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046c27a */
    f->r[1]=(0x00799a2cu+f->r[6]);
    /* 0046c280 */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0046c287 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0046c289:;
    /* 0046c289 */
    f->r[7]=(f->r[7]&0xffff0000u)|(((MI_U16((0x00000054u+f->r[4]+f->r[0]*1u)))&0xffffu)<<0);
    /* 0046c28e */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c291 */
    a=(f->r[7]&0xffffu);b=MI_U16((0x00000024u+f->r[4]+f->r[0]*1u));t=mi_alu(f,a,b,16,0);f->r[7]=(f->r[7]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0046c296 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046c299 */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046c29c */
    MI_U16((0xfffffffeu+f->r[1]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0046c2a0 */
    if(mi_signed_less(f->flags))goto mi_0046c289;
    /* 0046c2a2 */
    MI_U16((0x007999f8u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0046c2ab */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c2ad */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046c2b4 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046c2b7 */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046c2bd */
    f->r[7]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 0046c2c4 */
    f->r[1]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046c2ca */
    f->r[0]=MI_U32((0x007999e4u+f->r[6]));
    /* 0046c2d0 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046c2d2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[0])))&0xffu)<<0);
    /* 0046c2d8 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0046c2da */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046c2e1 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c2e3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c2e4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c2e5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c2e6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046c2ed */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c2ee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c2ef */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c2f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c2f5u;f->boundary(0x0046c2f0u,0x0047bf50u,f);
    /* 0046c2f5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046c2fc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046c303 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0046c309 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c30b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c30d */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0046c311 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c312 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c316 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 0046c31d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046c31e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c31f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c320 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c321 */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c325 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c32au;f->boundary(0x0046c325u,0x0046a400u,f);
    /* 0046c32a */
    f->r[1]=f->r[0];
    /* 0046c32c */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c32f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c332 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c334 */
    f->r[2]=(0x00000024u+f->r[4]);
    /* 0046c338 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c33b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c33f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c347 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c34e */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c354:;
    /* 0046c354 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c358 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c35b */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046c35e */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c362 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c366 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046c369 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c36d */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046c371 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c372 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c376 */
    if(!(f->flags&MI_ZF))goto mi_0046c354;
    /* 0046c378 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c381 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c387 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c389 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c38f */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c392 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c394 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c395 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c397 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c39e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c3a5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3a7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3a8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c3afu;f->boundary(0x0046c3aau,0x0047bf50u,f);
    /* 0046c3af */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 0046c3b3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3b5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 0046c3bc */
    f->r[0]=(0x00000001u+f->r[2]);
    /* 0046c3bf */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0046c3c5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3c7 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c3cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3cc */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 0046c3d0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3d1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3d2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c3d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c3d8u;f->boundary(0x0046c3d3u,0x0046a400u,f);
    /* 0046c3d8 */
    f->r[1]=f->r[0];
    /* 0046c3da */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c3dd */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c3e0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c3e2 */
    f->r[2]=(0x00000024u+f->r[4]);
    /* 0046c3e6 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c3e9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c3ed */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c3f5 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c3fc */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c402:;
    /* 0046c402 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c406 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c409 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046c40c */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c410 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c414 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046c417 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c41b */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046c41f */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c420 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c424 */
    if(!(f->flags&MI_ZF))goto mi_0046c402;
    /* 0046c426 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c42f */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c435 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c437 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c43d */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c440 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c442 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c443 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c445 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c44c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c453 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c455 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c456 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c457 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c458 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c45du;f->boundary(0x0046c458u,0x0047bf50u,f);
    /* 0046c45d */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 0046c461 */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 0046c465 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 0046c46c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c46e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c470 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c471 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0046c477 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c478 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c479 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c47a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c47fu;f->boundary(0x0046c47au,0x0046a400u,f);
    /* 0046c47f */
    f->r[1]=f->r[0];
    /* 0046c481 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c484 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c487 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c489 */
    f->r[6]=(0x00000024u+f->r[4]);
    /* 0046c48d */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c490 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c497 */
    f->r[2]=(0x00799a2cu+f->r[1]);
    /* 0046c49d */
    f->r[5]=0x0000000cu;
mi_0046c4a2:;
    /* 0046c4a2 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 0046c4a5 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046c4a8 */
    MI_U16((0x00000000u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c4ab */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c4ae */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0046c4af */
    if(!(f->flags&MI_ZF))goto mi_0046c4a2;
    /* 0046c4b1 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c4ba */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c4c0 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c4c2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c4c8 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c4cb */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c4cd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c4ce */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c4d0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c4d7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c4de */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c4e0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c4e1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c4e2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c4e3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c4e8u;f->boundary(0x0046c4e3u,0x0047bf50u,f);
    /* 0046c4e8 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c4eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046c4ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046c4ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046c4ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046c4ef */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c4f2 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046cb30(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046cb30 */
    a=f->r[4];b=0x00000060u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0046cb33 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb34 */
    f->r[3]=MI_U32((0x00000068u+f->r[4]));
    /* 0046cb38 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb39 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb3a */
    f->r[6]=f->r[3];
    /* 0046cb3c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb3d */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046cb40 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046cb42 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046cb45 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046cb47 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[6]));
    /* 0046cb4e */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046cb52 */
    f->r[5]=(0x00799a2cu+f->r[6]);
mi_0046cb58:;
    /* 0046cb58 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 0046cb5c */
    f->r[0]=0x2aaaaaabu;
    /* 0046cb61 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046cb64 */
    a=f->r[0];b=f->r[7];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046cb66 */
    f->r[0]=f->r[2];
    /* 0046cb68 */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046cb6b */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0046cb6e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046cb70 */
    f->r[0]=f->r[7];
    /* 0046cb72 */
    MI_U32((0x0000003cu+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[2]);
    /* 0046cb76 */
    f->r[7]=0x00000006u;
    /* 0046cb7b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046cb7c */
    a=f->r[7];mi_idiv32(f,a);
    /* 0046cb7e */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046cb81 */
    MI_U32((0x0000000cu+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[2]);
    /* 0046cb85 */
    if(mi_signed_less(f->flags))goto mi_0046cb58;
    /* 0046cb87 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046cb8e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046cb95 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb96 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb97 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb98 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046cb9du;f->boundary(0x0046cb98u,0x0047c480u,f);
    /* 0046cb9d */
    f->r[1]=MI_U32((0x00000080u+f->r[4]));
    /* 0046cba4 */
    MI_U16((0x007999e8u+f->r[6]))=(uint16_t)(0x00000017u);
    /* 0046cbad */
    f->r[0]=f->r[1];
    /* 0046cbaf */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)(0x0000000du);
    /* 0046cbb8 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046cbb9 */
    a=f->r[7];mi_idiv32(f,a);
    /* 0046cbbb */
    f->r[0]=0x2aaaaaabu;
    /* 0046cbc0 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0046cbc7 */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0046cbce */
    MI_U8((0x007999dcu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0046cbd5 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046cbd8 */
    f->r[7]=f->r[2];
    /* 0046cbda */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046cbdc */
    f->r[0]=f->r[2];
    /* 0046cbde */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046cbe0 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0046cbe3 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046cbe5 */
    f->r[1]=(0x00799a2cu+f->r[6]);
    /* 0046cbeb */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0046cbf2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0046cbf4:;
    /* 0046cbf4 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000010u+f->r[4]+f->r[0]*1u)))&0xffffu)<<0);
    /* 0046cbf9 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046cbfc */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00000040u+f->r[4]+f->r[0]*1u));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0046cc01 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046cc04 */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046cc07 */
    MI_U16((0xfffffffeu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046cc0b */
    if(mi_signed_less(f->flags))goto mi_0046cbf4;
    /* 0046cc0d */
    MI_U16((0x007999f8u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0046cc16 */
    f->r[1]=MI_U32((0x007999e4u+f->r[6]));
    /* 0046cc1c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046cc23 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[1])))&0xffu)<<0);
    /* 0046cc29 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cc2b */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046cc2e */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046cc31 */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046cc37 */
    f->r[7]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046cc3d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046cc44 */
    a=f->r[2];b=MI_U32((0x00f55bb4u+f->r[1]*8u));t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046cc4b */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046cc4d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cc4e */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cc50 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046cc57 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cc59 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cc5a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cc5b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cc5c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046cc61u;f->boundary(0x0046cc5cu,0x0047bf50u,f);
    /* 0046cc61 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046cc68 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046cc6f */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046cc72 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0046cc75 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[1]*2u);
    /* 0046cc78 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046cc7b */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0046cc80 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0046cc83 */
    f->r[2]=0x00000003u;
    /* 0046cc88 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046cc8a */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046cc8d */
    f->r[1]=(0x00af60acu+f->r[0]);
    /* 0046cc93 */
    f->r[0]=(0x00af5994u+f->r[0]);
mi_0046cc99:;
    /* 0046cc99 */
    f->r[3]=MI_U32((0x00000000u+f->r[0]));
    /* 0046cc9b */
    a=f->r[0];b=0x00000390u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046cca0 */
    a=f->r[3];b=0x00000020u;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0046cca3 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046cca6 */
    MI_U32((0xfffffc70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0046ccac */
    f->r[5]=MI_U32((0xfffffffcu+f->r[1]));
    /* 0046ccaf */
    a=f->r[5];b=0x00000020u;t=mi_alu(f,a,b,32,4);f->r[5]=t;
    /* 0046ccb2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046ccb3 */
    MI_U32((0xfffffffcu+f->r[1]))=(uint32_t)(f->r[5]);
    /* 0046ccb6 */
    if(!(f->flags&MI_ZF))goto mi_0046cc99;
    /* 0046ccb8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ccbdu;f->boundary(0x0046ccb8u,0x00476530u,f);
    /* 0046ccbd */
    f->r[3]=(0x00000002u+f->r[6]);
    /* 0046ccc0 */
    f->r[5]=(0x00000002u+f->r[7]);
    /* 0046ccc3 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccc5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccc6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccc7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccc8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccc9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ccceu;f->boundary(0x0046ccc9u,0x00516f90u,f);
    /* 0046ccce */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cccf */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccd0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccd1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccd2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ccd7u;f->boundary(0x0046ccd2u,0x005173a0u,f);
    /* 0046ccd7 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ccda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ccdb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ccdc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ccdd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ccde */
    a=f->r[4];b=0x00000060u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046cce1 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046ccf0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046ccf0 */
    a=f->r[4];b=0x00000060u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0046ccf3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccf4 */
    f->r[3]=MI_U32((0x00000068u+f->r[4]));
    /* 0046ccf8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccf9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccfa */
    f->r[6]=f->r[3];
    /* 0046ccfc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ccfd */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046cd00 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046cd02 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046cd05 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046cd07 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[6]));
    /* 0046cd0e */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046cd12 */
    f->r[5]=(0x00799a2cu+f->r[6]);
mi_0046cd18:;
    /* 0046cd18 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 0046cd1c */
    f->r[0]=f->r[7];
    /* 0046cd1e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046cd1f */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046cd22 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046cd24 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046cd27 */
    a=f->r[7];b=0x80000007u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0046cd2d */
    MI_U32((0x00000040u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[0]);
    /* 0046cd31 */
    if(!(f->flags&MI_SF))goto mi_0046cd38;
    /* 0046cd33 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046cd34 */
    a=f->r[7];b=0xfffffff8u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0046cd37 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046cd38:;
    /* 0046cd38 */
    MI_U32((0x00000010u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[7]);
    /* 0046cd3c */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046cd3f */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046cd42 */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046cd45 */
    if(mi_signed_less(f->flags))goto mi_0046cd18;
    /* 0046cd47 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046cd4e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046cd55 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cd56 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cd57 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cd58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046cd5du;f->boundary(0x0046cd58u,0x0047c480u,f);
    /* 0046cd5d */
    f->r[0]=MI_U32((0x00000080u+f->r[4]));
    /* 0046cd64 */
    MI_U16((0x007999e8u+f->r[6]))=(uint16_t)(0x0000001bu);
    /* 0046cd6d */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)(0x00000011u);
    /* 0046cd76 */
    f->r[1]=f->r[0];
    /* 0046cd78 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046cd7b */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0046cd82 */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)(0x00000003u);
    /* 0046cd89 */
    a=f->r[1];b=0x80000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0046cd8f */
    MI_U8((0x007999dcu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0046cd96 */
    if(!(f->flags&MI_SF))goto mi_0046cd9d;
    /* 0046cd98 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0046cd99 */
    a=f->r[1];b=0xfffffff8u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 0046cd9c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_0046cd9d:;
    /* 0046cd9d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046cd9e */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046cda1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046cda3 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046cda6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046cda8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046cdaa */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0046cdb1 */
    f->r[1]=(0x00799a2cu+f->r[6]);
mi_0046cdb7:;
    /* 0046cdb7 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000010u+f->r[4]+f->r[0]*1u)))&0xffffu)<<0);
    /* 0046cdbc */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046cdbf */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00000040u+f->r[4]+f->r[0]*1u));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0046cdc4 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046cdc7 */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046cdca */
    MI_U16((0xfffffffeu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046cdce */
    if(mi_signed_less(f->flags))goto mi_0046cdb7;
    /* 0046cdd0 */
    MI_U16((0x007999f8u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0046cdd9 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cddb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046cde2 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046cde5 */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046cdeb */
    f->r[2]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 0046cdf2 */
    f->r[1]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046cdf8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046cdff */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046ce01 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046ce08 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce09 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce0b */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce0d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce0e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce0f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce10 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ce15u;f->boundary(0x0046ce10u,0x0047bf50u,f);
    /* 0046ce15 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046ce1c */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046ce23 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 0046ce29 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ce2c */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0046ce2f */
    f->r[0]=(0x00000000u+f->r[6]+f->r[2]*2u);
    /* 0046ce32 */
    f->r[2]=0x00000004u;
    /* 0046ce37 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046ce3a */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 0046ce3d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046ce3f */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046ce42 */
    f->r[1]=(0x00af643cu+f->r[0]);
    /* 0046ce48 */
    f->r[0]=(0x00af5998u+f->r[0]);
mi_0046ce4e:;
    /* 0046ce4e */
    f->r[3]=MI_U32((0x00000000u+f->r[0]));
    /* 0046ce50 */
    a=f->r[0];b=0x00000390u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046ce55 */
    a=f->r[3];b=0x00000020u;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0046ce58 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046ce5b */
    MI_U32((0xfffffc70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0046ce61 */
    f->r[5]=MI_U32((0xfffffffcu+f->r[1]));
    /* 0046ce64 */
    a=f->r[5];b=0x00000020u;t=mi_alu(f,a,b,32,4);f->r[5]=t;
    /* 0046ce67 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0046ce68 */
    MI_U32((0xfffffffcu+f->r[1]))=(uint32_t)(f->r[5]);
    /* 0046ce6b */
    if(!(f->flags&MI_ZF))goto mi_0046ce4e;
    /* 0046ce6d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ce72u;f->boundary(0x0046ce6du,0x00476530u,f);
    /* 0046ce72 */
    f->r[3]=(0x00000003u+f->r[6]);
    /* 0046ce75 */
    f->r[5]=(0x00000003u+f->r[7]);
    /* 0046ce78 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce7a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce7b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce7c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce7d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce7e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ce83u;f->boundary(0x0046ce7eu,0x00516f90u,f);
    /* 0046ce83 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce84 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce85 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce86 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ce87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ce8cu;f->boundary(0x0046ce87u,0x005173a0u,f);
    /* 0046ce8c */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ce8f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ce90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ce91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046ce92 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ce93 */
    a=f->r[4];b=0x00000060u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ce96 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046bda0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046bda0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bda1 */
    f->r[5]=MI_U32((0x00000008u+f->r[4]));
    /* 0046bda5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bda6 */
    f->r[6]=f->r[5];
    /* 0046bda8 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000010u+f->r[4])))&0xffffu)<<0);
    /* 0046bdad */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bdae */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bdb1 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046bdb3 */
    f->r[1]=0x0078d590u;
    /* 0046bdb8 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bdbb */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0046bdc2 */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0046bdc9 */
    a=MI_U16((0x007999f4u+f->r[6]));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x007999f4u+f->r[6]))=(uint16_t)(t);
    /* 0046bdd0 */
    f->r[0]=(0x00799a2cu+f->r[6]);
mi_0046bdd6:;
    /* 0046bdd6 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 0046bdd9 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bddc */
    a=MI_U16((0x00000000u+f->r[0]));b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0x00000000u+f->r[0]))=(uint16_t)(t);
    /* 0046bddf */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bde2 */
    a=f->r[1];b=0x0078d5c0u;t=mi_alu(f,a,b,32,1);
    /* 0046bde8 */
    if(mi_signed_less(f->flags))goto mi_0046bdd6;
    /* 0046bdea */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046bdf1 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046bdf4 */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046bdfa */
    f->r[7]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 0046be01 */
    f->r[1]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046be07 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046be0a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046be0c */
    if((f->flags&MI_ZF))goto mi_0046be0f;
    /* 0046be0e */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046be0f:;
    /* 0046be0f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046be16 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046be1d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be1e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be1f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be20 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046be25u;f->boundary(0x0046be20u,0x0047c480u,f);
    /* 0046be25 */
    f->r[0]=MI_U32((0x0078d76cu));
    /* 0046be2a */
    f->r[1]=MI_U32((0x0078d770u));
    /* 0046be30 */
    MI_U16((0x007999deu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0046be37 */
    MI_U16((0x007999e0u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0046be3e */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0046be41 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be43 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be44 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be46 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0046be49 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 0046be4f */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be51 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0046be54 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0046be57 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046be59 */
    MI_U32((0x007999e4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0046be5f */
    MI_U8((0x007999dcu+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0046be66 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046be6d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046be74 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be75 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be76 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be77 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046be7cu;f->boundary(0x0046be77u,0x0047bf50u,f);
    /* 0046be7c */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046be7f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046be80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046be81 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046be82 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046be90(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046be90 */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0046be93 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be94 */
    f->r[3]=MI_U32((0x0000007cu+f->r[4]));
    /* 0046be98 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be99 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be9a */
    f->r[6]=f->r[3];
    /* 0046be9c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046be9d */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bea0 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046bea2 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046bea5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046bea7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[6]));
    /* 0046beae */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046beb2 */
    f->r[5]=(0x00799a2cu+f->r[6]);
mi_0046beb8:;
    /* 0046beb8 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 0046bebc */
    f->r[0]=f->r[7];
    /* 0046bebe */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046bebf */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046bec2 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bec4 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046bec7 */
    a=f->r[7];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0046becd */
    MI_U32((0x00000024u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[0]);
    /* 0046bed1 */
    if(!(f->flags&MI_SF))goto mi_0046bed8;
    /* 0046bed3 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046bed4 */
    a=f->r[7];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0046bed7 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046bed8:;
    /* 0046bed8 */
    MI_U32((0x00000054u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[7]);
    /* 0046bedc */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bedf */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046bee2 */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046bee5 */
    if(mi_signed_less(f->flags))goto mi_0046beb8;
    /* 0046bee7 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 0046beeb */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bef2 */
    f->r[0]=f->r[7];
    /* 0046bef4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bef5 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046bef6 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046bef9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046befb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bf02 */
    f->r[5]=f->r[0];
    /* 0046bf04 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bf05 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bf06 */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[5]=t;
    /* 0046bf09 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bf0eu;f->boundary(0x0046bf09u,0x0047c480u,f);
    /* 0046bf0e */
    f->r[2]=0x00000001u;
    /* 0046bf13 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046bf16 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046bf1c */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0046bf22 */
    a=f->r[7];b=0x80000003u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0046bf28 */
    MI_U8((0x007999dcu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0046bf2f */
    if(!(f->flags&MI_SF))goto mi_0046bf36;
    /* 0046bf31 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046bf32 */
    a=f->r[7];b=0xfffffffcu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0046bf35 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0046bf36:;
    /* 0046bf36 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046bf38 */
    f->r[1]=(0x00799a2cu+f->r[6]);
    /* 0046bf3e */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0046bf45 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0046bf47:;
    /* 0046bf47 */
    f->r[7]=(f->r[7]&0xffff0000u)|(((MI_U16((0x00000054u+f->r[4]+f->r[0]*1u)))&0xffffu)<<0);
    /* 0046bf4c */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bf4f */
    a=(f->r[7]&0xffffu);b=MI_U16((0x00000024u+f->r[4]+f->r[0]*1u));t=mi_alu(f,a,b,16,0);f->r[7]=(f->r[7]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0046bf54 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046bf57 */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046bf5a */
    MI_U16((0xfffffffeu+f->r[1]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0046bf5e */
    if(mi_signed_less(f->flags))goto mi_0046bf47;
    /* 0046bf60 */
    MI_U16((0x007999f8u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0046bf69 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bf6b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046bf72 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046bf75 */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046bf7b */
    f->r[7]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 0046bf82 */
    f->r[1]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046bf88 */
    f->r[0]=MI_U32((0x007999e4u+f->r[6]));
    /* 0046bf8e */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046bf90 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[0])))&0xffu)<<0);
    /* 0046bf96 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0046bf98 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bf9f */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bfa1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfa2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfa3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfa4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bfab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfad */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfae */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bfb3u;f->boundary(0x0046bfaeu,0x0047bf50u,f);
    /* 0046bfb3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046bfba */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046bfc1 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0046bfc7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfc9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfcb */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0046bfcf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfd0 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046bfd4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 0046bfdb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046bfdc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfdd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfde */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046bfdf */
    MI_U32((0x00000054u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046bfe3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046bfe8u;f->boundary(0x0046bfe3u,0x0046a400u,f);
    /* 0046bfe8 */
    f->r[1]=f->r[0];
    /* 0046bfea */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046bfed */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046bff0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046bff2 */
    f->r[2]=(0x00000024u+f->r[4]);
    /* 0046bff6 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046bff9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046bffd */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c005 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c00c */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c012:;
    /* 0046c012 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c016 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c019 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046c01c */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c020 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c024 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046c027 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c02b */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046c02f */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c030 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c034 */
    if(!(f->flags&MI_ZF))goto mi_0046c012;
    /* 0046c036 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c03f */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c045 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c047 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c04d */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c050 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c052 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c053 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c055 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c05c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c063 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c065 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c066 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c067 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c068 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c06du;f->boundary(0x0046c068u,0x0047bf50u,f);
    /* 0046c06d */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 0046c071 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c073 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 0046c07a */
    f->r[0]=(0x00000001u+f->r[2]);
    /* 0046c07d */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0046c083 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c085 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c089 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c08a */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 0046c08e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c08f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c090 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c091 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c096u;f->boundary(0x0046c091u,0x0046a400u,f);
    /* 0046c096 */
    f->r[1]=f->r[0];
    /* 0046c098 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c09b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c09e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c0a0 */
    f->r[2]=(0x00000024u+f->r[4]);
    /* 0046c0a4 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c0a7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c0ab */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c0b3 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c0ba */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c0c0:;
    /* 0046c0c0 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c0c4 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c0c7 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046c0ca */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c0ce */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c0d2 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046c0d5 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c0d9 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046c0dd */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c0de */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c0e2 */
    if(!(f->flags&MI_ZF))goto mi_0046c0c0;
    /* 0046c0e4 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c0ed */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c0f3 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c0f5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c0fb */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c0fe */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c100 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c101 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c103 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c10a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c111 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c113 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c114 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c115 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c116 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c11bu;f->boundary(0x0046c116u,0x0047bf50u,f);
    /* 0046c11b */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 0046c11f */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 0046c123 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]));
    /* 0046c12a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c12c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c12e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c12f */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0046c135 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c136 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c137 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c138 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c13du;f->boundary(0x0046c138u,0x0046a400u,f);
    /* 0046c13d */
    f->r[1]=f->r[0];
    /* 0046c13f */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c142 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c145 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c147 */
    f->r[6]=(0x00000024u+f->r[4]);
    /* 0046c14b */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c14e */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c155 */
    f->r[2]=(0x00799a2cu+f->r[1]);
    /* 0046c15b */
    f->r[5]=0x0000000cu;
mi_0046c160:;
    /* 0046c160 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 0046c163 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046c166 */
    MI_U16((0x00000000u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c169 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c16c */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0046c16d */
    if(!(f->flags&MI_ZF))goto mi_0046c160;
    /* 0046c16f */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c178 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c17e */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c180 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c186 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c189 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c18b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c18c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c18e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c195 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c19c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c19e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c19f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c1a0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c1a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c1a6u;f->boundary(0x0046c1a1u,0x0047bf50u,f);
    /* 0046c1a6 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c1a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046c1aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046c1ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046c1ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046c1ad */
    a=f->r[4];b=0x00000074u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c1b0 */
    f->r[4]+=4;return;
}

void restore_building_control_C_0046c500(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046c500 */
    a=f->r[4];b=0x00000078u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0046c503 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c504 */
    f->r[3]=MI_U32((0x00000080u+f->r[4]));
    /* 0046c50b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c50c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c50d */
    f->r[6]=f->r[3];
    /* 0046c50f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c510 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046c513 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046c515 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046c518 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046c51a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[6]));
    /* 0046c521 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c525 */
    f->r[5]=(0x00799a2cu+f->r[6]);
mi_0046c52b:;
    /* 0046c52b */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 0046c52f */
    f->r[0]=0x38e38e39u;
    /* 0046c534 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c537 */
    a=f->r[0];b=f->r[7];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046c539 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0046c53b */
    f->r[0]=f->r[2];
    /* 0046c53d */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046c540 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0046c543 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c545 */
    f->r[0]=f->r[7];
    /* 0046c547 */
    MI_U32((0x00000024u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[2]);
    /* 0046c54b */
    f->r[7]=0x00000009u;
    /* 0046c550 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046c551 */
    a=f->r[7];mi_idiv32(f,a);
    /* 0046c553 */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046c556 */
    MI_U32((0x00000054u+f->r[4]+f->r[1]*1u))=(uint32_t)(f->r[2]);
    /* 0046c55a */
    if(mi_signed_less(f->flags))goto mi_0046c52b;
    /* 0046c55c */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 0046c560 */
    f->r[0]=0x38e38e39u;
    /* 0046c565 */
    a=f->r[0];b=f->r[7];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0046c567 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0046c569 */
    f->r[1]=f->r[2];
    /* 0046c56b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046c572 */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 0046c575 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c577 */
    f->r[5]=f->r[2];
    /* 0046c579 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046c580 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c581 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c582 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c583 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c588u;f->boundary(0x0046c583u,0x0047c480u,f);
    /* 0046c588 */
    f->r[0]=f->r[7];
    /* 0046c58a */
    f->r[1]=0x00000009u;
    /* 0046c58f */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046c590 */
    a=f->r[1];mi_idiv32(f,a);
    /* 0046c592 */
    MI_U16((0x007999e8u+f->r[6]))=(uint16_t)(0x00000013u);
    /* 0046c59b */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)(0x00000009u);
    /* 0046c5a4 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0046c5ab */
    MI_U8((0x007999ddu+f->r[6]))=(uint8_t)(0x00000001u);
    /* 0046c5b2 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c5b5 */
    MI_U8((0x007999dcu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0046c5bc */
    f->r[1]=(0x00799a2cu+f->r[6]);
    /* 0046c5c2 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c5c4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046c5c6 */
    MI_U16((0x007999f4u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
mi_0046c5cd:;
    /* 0046c5cd */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000058u+f->r[4]+f->r[0]*1u)))&0xffffu)<<0);
    /* 0046c5d2 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c5d5 */
    a=(f->r[2]&0xffffu);b=MI_U16((0x00000028u+f->r[4]+f->r[0]*1u));t=mi_alu(f,a,b,16,0);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0046c5da */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046c5dd */
    a=f->r[0];b=0x00000030u;t=mi_alu(f,a,b,32,1);
    /* 0046c5e0 */
    MI_U16((0xfffffffeu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0046c5e4 */
    if(mi_signed_less(f->flags))goto mi_0046c5cd;
    /* 0046c5e6 */
    MI_U16((0x007999f8u+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0046c5ef */
    f->r[2]=MI_U32((0x007999e4u+f->r[6]));
    /* 0046c5f5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 0046c5fc */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0046c5ff */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c601 */
    f->r[1]=MI_U32((0x005d03ecu+f->r[0]));
    /* 0046c607 */
    f->r[7]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 0046c60e */
    f->r[1]=MI_U32((0x005d043cu+f->r[0]));
    /* 0046c614 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c61a */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0046c61c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046c623 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046c62a */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046c62d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046c62f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c630 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c632 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c634 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c635 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c636 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c637 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c63cu;f->boundary(0x0046c637u,0x0047bf50u,f);
    /* 0046c63c */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046c643 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046c64a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c64c */
    f->r[0]=(0x00000001u+f->r[3]);
    /* 0046c64f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c651 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c652 */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c656 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c657 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046c65c */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c65e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c65f */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c663 */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0046c667 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c66cu;f->boundary(0x0046c667u,0x0046a400u,f);
    /* 0046c66c */
    f->r[1]=f->r[0];
    /* 0046c66e */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c671 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c674 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c676 */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 0046c67a */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c67d */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c681 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c689 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c690 */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c696:;
    /* 0046c696 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c69a */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c69d */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 0046c6a0 */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[6]&0xffffu));
    /* 0046c6a4 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c6a8 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046c6ab */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0046c6af */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 0046c6b3 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0046c6b4 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0046c6b8 */
    if(!(f->flags&MI_ZF))goto mi_0046c696;
    /* 0046c6ba */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c6c3 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c6c9 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6cb */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c6d1 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c6d4 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c6d6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6d7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6d9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c6e0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c6e7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6e9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6ea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c6f1u;f->boundary(0x0046c6ecu,0x0047bf50u,f);
    /* 0046c6f1 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 0046c6f5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6f7 */
    f->r[0]=(0x00000002u+f->r[3]);
    /* 0046c6fa */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6fc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c6fd */
    MI_U32((0x00000048u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0046c701 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c702 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046c707 */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c709 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c70a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c70fu;f->boundary(0x0046c70au,0x0046a400u,f);
    /* 0046c70f */
    f->r[1]=f->r[0];
    /* 0046c711 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c714 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c717 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c719 */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 0046c71d */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c720 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c724 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c72c */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c733 */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c739:;
    /* 0046c739 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c73d */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c740 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 0046c743 */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[6]&0xffffu));
    /* 0046c747 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c74b */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046c74e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0046c752 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 0046c756 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0046c757 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0046c75b */
    if(!(f->flags&MI_ZF))goto mi_0046c739;
    /* 0046c75d */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c766 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c76c */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c76e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c774 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c777 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c779 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c77a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c77c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c783 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c78a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c78c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c78d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c78e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c78f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c794u;f->boundary(0x0046c78fu,0x0047bf50u,f);
    /* 0046c794 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 0046c798 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046c79d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c79f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c7a1 */
    f->r[6]=(0x00000001u+f->r[2]);
    /* 0046c7a4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c7a5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c7a6 */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c7a8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c7a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c7aeu;f->boundary(0x0046c7a9u,0x0046a400u,f);
    /* 0046c7ae */
    f->r[1]=f->r[0];
    /* 0046c7b0 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c7b3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c7b6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c7b8 */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 0046c7bc */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c7bf */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c7c3 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c7cb */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c7d2 */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c7d8:;
    /* 0046c7d8 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c7dc */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c7df */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046c7e2 */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c7e6 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c7ea */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046c7ed */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c7f1 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0046c7f5 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c7f6 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c7fa */
    if(!(f->flags&MI_ZF))goto mi_0046c7d8;
    /* 0046c7fc */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c805 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c80b */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c80d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c813 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c816 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c818 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c819 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c81b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c822 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c829 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c82b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c82c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c82d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c82e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c833u;f->boundary(0x0046c82eu,0x0047bf50u,f);
    /* 0046c833 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 0046c837 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046c83c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c83e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c840 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c841 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c842 */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c844 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c845 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c84au;f->boundary(0x0046c845u,0x0046a400u,f);
    /* 0046c84a */
    f->r[1]=f->r[0];
    /* 0046c84c */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c84f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c852 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c854 */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 0046c858 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c85b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c85f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c867 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c86e */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c874:;
    /* 0046c874 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c878 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c87b */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046c87e */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c882 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0046c886 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046c889 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c88d */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0046c891 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c892 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c896 */
    if(!(f->flags&MI_ZF))goto mi_0046c874;
    /* 0046c898 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c8a1 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c8a7 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8a9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c8af */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c8b2 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c8b4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8b5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8b7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c8be */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c8c5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8c7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8c8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8c9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c8cfu;f->boundary(0x0046c8cau,0x0047bf50u,f);
    /* 0046c8cf */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 0046c8d3 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046c8d8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8da */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8dc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8dd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8de */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8e0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c8e1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c8e6u;f->boundary(0x0046c8e1u,0x0046a400u,f);
    /* 0046c8e6 */
    f->r[1]=f->r[0];
    /* 0046c8e8 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c8eb */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c8ee */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c8f0 */
    f->r[6]=(0x00000028u+f->r[4]);
    /* 0046c8f4 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c8f7 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c8ff */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c906 */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c90c:;
    /* 0046c90c */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 0046c90f */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046c912 */
    MI_U16((0x00000000u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c915 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0046c919 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c91c */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c91d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c921 */
    if(!(f->flags&MI_ZF))goto mi_0046c90c;
    /* 0046c923 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c92c */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c932 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c934 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c93a */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c93d */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c93f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c940 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c942 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c949 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c950 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c952 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c953 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c954 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c955 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c95au;f->boundary(0x0046c955u,0x0047bf50u,f);
    /* 0046c95a */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 0046c95e */
    f->r[0]=MI_U32((0x00000040u+f->r[4]));
    /* 0046c962 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0046c968 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c96a */
    f->r[6]=(0x00000002u+f->r[2]);
    /* 0046c96d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c96f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c970 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c971 */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c973 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c974 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c979u;f->boundary(0x0046c974u,0x0046a400u,f);
    /* 0046c979 */
    f->r[1]=f->r[0];
    /* 0046c97b */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046c97e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c981 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046c983 */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 0046c987 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046c98a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046c98e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046c996 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046c99d */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046c9a3:;
    /* 0046c9a3 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046c9a7 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c9aa */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046c9ad */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046c9b1 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046c9b5 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046c9b8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c9bc */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0046c9c0 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046c9c1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046c9c5 */
    if(!(f->flags&MI_ZF))goto mi_0046c9a3;
    /* 0046c9c7 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046c9d0 */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046c9d6 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c9d8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046c9de */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046c9e1 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046c9e3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c9e4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c9e6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046c9ed */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046c9f4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c9f6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c9f7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c9f8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046c9f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046c9feu;f->boundary(0x0046c9f9u,0x0047bf50u,f);
    /* 0046c9fe */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 0046ca02 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046ca07 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca09 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca0b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca0c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca0d */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca0f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca10 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ca15u;f->boundary(0x0046ca10u,0x0046a400u,f);
    /* 0046ca15 */
    f->r[1]=f->r[0];
    /* 0046ca17 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ca1a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046ca1d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046ca1f */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 0046ca23 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046ca26 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0046ca2a */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0046ca32 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046ca39 */
    f->r[2]=(0x00799a2cu+f->r[1]);
mi_0046ca3f:;
    /* 0046ca3f */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046ca43 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046ca46 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 0046ca49 */
    MI_U16((0xfffffffeu+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046ca4d */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0046ca51 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0046ca54 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046ca58 */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 0046ca5c */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0046ca5d */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0046ca61 */
    if(!(f->flags&MI_ZF))goto mi_0046ca3f;
    /* 0046ca63 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046ca6c */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046ca72 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca74 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046ca7a */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046ca7d */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046ca7f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca80 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca82 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046ca89 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046ca90 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca92 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca93 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca94 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ca95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ca9au;f->boundary(0x0046ca95u,0x0047bf50u,f);
    /* 0046ca9a */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 0046ca9e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046caa3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046caa5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046caa7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046caa8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046caa9 */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046caab */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046caac */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046cab1u;f->boundary(0x0046caacu,0x0046a400u,f);
    /* 0046cab1 */
    f->r[1]=f->r[0];
    /* 0046cab3 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046cab6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046cab9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046cabb */
    f->r[6]=(0x00000028u+f->r[4]);
    /* 0046cabf */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046cac2 */
    MI_U16((0x007999f4u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0046cac9 */
    f->r[2]=(0x00799a2cu+f->r[1]);
    /* 0046cacf */
    f->r[5]=0x0000000cu;
mi_0046cad4:;
    /* 0046cad4 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 0046cad7 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046cada */
    MI_U16((0x00000000u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0046cadd */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046cae0 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0046cae1 */
    if(!(f->flags&MI_ZF))goto mi_0046cad4;
    /* 0046cae3 */
    MI_U16((0x007999f8u+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0046caec */
    f->r[2]=MI_U32((0x007999e4u+f->r[1]));
    /* 0046caf2 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046caf4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[2])))&0xffu)<<0);
    /* 0046cafa */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0046cafd */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046caff */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb00 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb02 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0046cb09 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0046cb10 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb12 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb13 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb14 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046cb15 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046cb1au;f->boundary(0x0046cb15u,0x0047bf50u,f);
    /* 0046cb1a */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046cb1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046cb1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046cb1f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046cb20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046cb21 */
    a=f->r[4];b=0x00000078u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046cb24 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452980(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452980 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452985 */
    f->r[1]=f->r[0];
    /* 00452987 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045298a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045298c */
    a=MI_U16((0x007999f4u+f->r[1]*8u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00452995 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00452a12;
    /* 00452997 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452998 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045299du;f->boundary(0x00452998u,0x0046b840u,f);
    /* 0045299d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004529a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004529a5u;f->boundary(0x004529a0u,0x004539a0u,f);
    /* 004529a5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004529a7 */
    if(!(f->flags&MI_ZF))goto mi_00452a12;
    /* 004529a9 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004529af */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004529b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004529b5u;f->boundary(0x004529b0u,0x00453af0u,f);
    /* 004529b5 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 004529ba */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004529bf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004529c1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004529c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004529c7u;f->boundary(0x004529c2u,0x00453b40u,f);
    /* 004529c7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004529ca */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004529cc */
    if(!(f->flags&MI_ZF))goto mi_004529d9;
    /* 004529ce */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 004529d8 */
    f->r[4]+=4;return;
mi_004529d9:;
    /* 004529d9 */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 004529de */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004529e1 */
    if(!(f->flags&MI_ZF))goto mi_004529f8;
    /* 004529e3 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004529e9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004529ea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004529eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004529f0u;f->boundary(0x004529ebu,0x00453b40u,f);
    /* 004529f0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004529f3 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_004529f8:;
    /* 004529f8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004529fa */
    if((f->flags&MI_ZF))goto mi_00452a12;
    /* 004529fc */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004529ff */
    if((f->flags&MI_ZF))goto mi_00452a12;
    /* 00452a01 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452a07 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a09 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a0a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452a0fu;f->boundary(0x00452a0au,0x0046cea0u,f);
    /* 00452a0f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452a12:;
    /* 00452a12 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452a20(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452a20 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452a25 */
    f->r[1]=f->r[0];
    /* 00452a27 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00452a2a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00452a2c */
    a=MI_U16((0x007999f4u+f->r[1]*8u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00452a35 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00452afd;
    /* 00452a3b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a3c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452a41u;f->boundary(0x00452a3cu,0x0046b840u,f);
    /* 00452a41 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452a44 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452a49u;f->boundary(0x00452a44u,0x004539a0u,f);
    /* 00452a49 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452a4b */
    if(!(f->flags&MI_ZF))goto mi_00452afd;
    /* 00452a51 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452a57 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452a5du;f->boundary(0x00452a58u,0x00453af0u,f);
    /* 00452a5d */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00452a62 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452a67 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a69 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a6a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452a6fu;f->boundary(0x00452a6au,0x00453b40u,f);
    /* 00452a6f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452a72 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452a74 */
    if(!(f->flags&MI_ZF))goto mi_00452a82;
    /* 00452a76 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00452a80 */
    goto mi_00452aa6;
mi_00452a82:;
    /* 00452a82 */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00452a87 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00452a8a */
    if(!(f->flags&MI_ZF))goto mi_00452aa1;
    /* 00452a8c */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452a92 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a93 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452a94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452a99u;f->boundary(0x00452a94u,0x00453b40u,f);
    /* 00452a99 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452a9c */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00452aa1:;
    /* 00452aa1 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00452aa4 */
    if(!(f->flags&MI_ZF))goto mi_00452ad9;
mi_00452aa6:;
    /* 00452aa6 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452aac */
    f->r[0]=f->r[2];
    /* 00452aae */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00452ab1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00452ab3 */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 00452aba */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 00452ac1 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00452ac3 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00452aca;
    /* 00452ac5 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00452ac7 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00452ac9 */
    f->r[4]+=4;return;
mi_00452aca:;
    /* 00452aca */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452acc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452acd */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00452ad0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452ad5u;f->boundary(0x00452ad0u,0x0046cea0u,f);
    /* 00452ad5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452ad8 */
    f->r[4]+=4;return;
mi_00452ad9:;
    /* 00452ad9 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452adf */
    f->r[2]=f->r[1];
    /* 00452ae1 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00452ae4 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00452ae6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452ae8 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00452af0 */
    if((f->flags&MI_ZF))goto mi_00452afd;
    /* 00452af2 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452af4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452af5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452afau;f->boundary(0x00452af5u,0x0046cea0u,f);
    /* 00452afa */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452afd:;
    /* 00452afd */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452b00(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452b00 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452b05 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b06 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452b0bu;f->boundary(0x00452b06u,0x0046b840u,f);
    /* 00452b0b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452b0e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452b13u;f->boundary(0x00452b0eu,0x004539a0u,f);
    /* 00452b13 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452b15 */
    if(!(f->flags&MI_ZF))goto mi_00452bc7;
    /* 00452b1b */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452b21 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b22 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452b27u;f->boundary(0x00452b22u,0x00453af0u,f);
    /* 00452b27 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452b2d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b2f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b30 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00452b35 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452b3au;f->boundary(0x00452b35u,0x00453b40u,f);
    /* 00452b3a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452b3d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452b3f */
    if(!(f->flags&MI_ZF))goto mi_00452b4d;
    /* 00452b41 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00452b4b */
    goto mi_00452b70;
mi_00452b4d:;
    /* 00452b4d */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00452b52 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00452b55 */
    if(!(f->flags&MI_ZF))goto mi_00452b6b;
    /* 00452b57 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b58 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452b5d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b5e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452b63u;f->boundary(0x00452b5eu,0x00453b40u,f);
    /* 00452b63 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452b66 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00452b6b:;
    /* 00452b6b */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00452b6e */
    if(!(f->flags&MI_ZF))goto mi_00452ba3;
mi_00452b70:;
    /* 00452b70 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452b76 */
    f->r[1]=f->r[2];
    /* 00452b78 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00452b7b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00452b7d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00452b84 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 00452b8b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00452b8d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00452b94;
    /* 00452b8f */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00452b91 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00452b93 */
    f->r[4]+=4;return;
mi_00452b94:;
    /* 00452b94 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b96 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452b97 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00452b9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452b9fu;f->boundary(0x00452b9au,0x0046cea0u,f);
    /* 00452b9f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452ba2 */
    f->r[4]+=4;return;
mi_00452ba3:;
    /* 00452ba3 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452ba9 */
    f->r[2]=f->r[1];
    /* 00452bab */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00452bae */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00452bb0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452bb2 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00452bba */
    if((f->flags&MI_ZF))goto mi_00452bc7;
    /* 00452bbc */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452bbe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452bbf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452bc4u;f->boundary(0x00452bbfu,0x0046cea0u,f);
    /* 00452bc4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452bc7:;
    /* 00452bc7 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452bd0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452bd0 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452bd5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452bd6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452bdbu;f->boundary(0x00452bd6u,0x0046b840u,f);
    /* 00452bdb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452bde */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452be3u;f->boundary(0x00452bdeu,0x004539a0u,f);
    /* 00452be3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452be5 */
    if(!(f->flags&MI_ZF))goto mi_00452c97;
    /* 00452beb */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452bf1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452bf2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452bf7u;f->boundary(0x00452bf2u,0x00453af0u,f);
    /* 00452bf7 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452bfd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452bff */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452c00 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00452c05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452c0au;f->boundary(0x00452c05u,0x00453b40u,f);
    /* 00452c0a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452c0d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452c0f */
    if(!(f->flags&MI_ZF))goto mi_00452c1d;
    /* 00452c11 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00452c1b */
    goto mi_00452c40;
mi_00452c1d:;
    /* 00452c1d */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00452c22 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00452c25 */
    if(!(f->flags&MI_ZF))goto mi_00452c3b;
    /* 00452c27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452c28 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452c2d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452c2e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452c33u;f->boundary(0x00452c2eu,0x00453b40u,f);
    /* 00452c33 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452c36 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00452c3b:;
    /* 00452c3b */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00452c3e */
    if(!(f->flags&MI_ZF))goto mi_00452c73;
mi_00452c40:;
    /* 00452c40 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452c46 */
    f->r[1]=f->r[2];
    /* 00452c48 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00452c4b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00452c4d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00452c54 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 00452c5b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00452c5d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00452c64;
    /* 00452c5f */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00452c61 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00452c63 */
    f->r[4]+=4;return;
mi_00452c64:;
    /* 00452c64 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452c66 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452c67 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00452c6a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452c6fu;f->boundary(0x00452c6au,0x0046cea0u,f);
    /* 00452c6f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452c72 */
    f->r[4]+=4;return;
mi_00452c73:;
    /* 00452c73 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452c79 */
    f->r[2]=f->r[1];
    /* 00452c7b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00452c7e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00452c80 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452c82 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00452c8a */
    if((f->flags&MI_ZF))goto mi_00452c97;
    /* 00452c8c */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452c8e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452c8f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452c94u;f->boundary(0x00452c8fu,0x0046cea0u,f);
    /* 00452c94 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452c97:;
    /* 00452c97 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452ca0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452ca0 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452ca5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452ca6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452cabu;f->boundary(0x00452ca6u,0x0046b840u,f);
    /* 00452cab */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452cae */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452cb3u;f->boundary(0x00452caeu,0x004539a0u,f);
    /* 00452cb3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452cb5 */
    if(!(f->flags&MI_ZF))goto mi_00452d67;
    /* 00452cbb */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452cc1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452cc2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452cc7u;f->boundary(0x00452cc2u,0x00453af0u,f);
    /* 00452cc7 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452ccd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452ccf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452cd0 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00452cd5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452cdau;f->boundary(0x00452cd5u,0x00453b40u,f);
    /* 00452cda */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452cdd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452cdf */
    if(!(f->flags&MI_ZF))goto mi_00452ced;
    /* 00452ce1 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00452ceb */
    goto mi_00452d10;
mi_00452ced:;
    /* 00452ced */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00452cf2 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00452cf5 */
    if(!(f->flags&MI_ZF))goto mi_00452d0b;
    /* 00452cf7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452cf8 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452cfd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452cfe */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452d03u;f->boundary(0x00452cfeu,0x00453b40u,f);
    /* 00452d03 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452d06 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00452d0b:;
    /* 00452d0b */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00452d0e */
    if(!(f->flags&MI_ZF))goto mi_00452d43;
mi_00452d10:;
    /* 00452d10 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452d16 */
    f->r[1]=f->r[2];
    /* 00452d18 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00452d1b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00452d1d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00452d24 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 00452d2b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00452d2d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00452d34;
    /* 00452d2f */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00452d31 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00452d33 */
    f->r[4]+=4;return;
mi_00452d34:;
    /* 00452d34 */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452d36 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452d37 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00452d3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452d3fu;f->boundary(0x00452d3au,0x0046cea0u,f);
    /* 00452d3f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452d42 */
    f->r[4]+=4;return;
mi_00452d43:;
    /* 00452d43 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452d49 */
    f->r[2]=f->r[1];
    /* 00452d4b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00452d4e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00452d50 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452d52 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00452d5a */
    if((f->flags&MI_ZF))goto mi_00452d67;
    /* 00452d5c */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452d5e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452d5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452d64u;f->boundary(0x00452d5fu,0x0046cea0u,f);
    /* 00452d64 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452d67:;
    /* 00452d67 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452d70(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452d70 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452d75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452d76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452d7bu;f->boundary(0x00452d76u,0x0046b840u,f);
    /* 00452d7b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452d7e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452d83u;f->boundary(0x00452d7eu,0x004539a0u,f);
    /* 00452d83 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452d85 */
    if(!(f->flags&MI_ZF))goto mi_00452e37;
    /* 00452d8b */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452d91 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452d92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452d97u;f->boundary(0x00452d92u,0x00453af0u,f);
    /* 00452d97 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452d9d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452d9f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452da0 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00452da5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452daau;f->boundary(0x00452da5u,0x00453b40u,f);
    /* 00452daa */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452dad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452daf */
    if(!(f->flags&MI_ZF))goto mi_00452dbd;
    /* 00452db1 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00452dbb */
    goto mi_00452de0;
mi_00452dbd:;
    /* 00452dbd */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00452dc2 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00452dc5 */
    if(!(f->flags&MI_ZF))goto mi_00452ddb;
    /* 00452dc7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452dc8 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452dcd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452dce */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452dd3u;f->boundary(0x00452dceu,0x00453b40u,f);
    /* 00452dd3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452dd6 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00452ddb:;
    /* 00452ddb */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00452dde */
    if(!(f->flags&MI_ZF))goto mi_00452e13;
mi_00452de0:;
    /* 00452de0 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452de6 */
    f->r[1]=f->r[2];
    /* 00452de8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00452deb */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00452ded */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00452df4 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 00452dfb */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00452dfd */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00452e04;
    /* 00452dff */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00452e01 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00452e03 */
    f->r[4]+=4;return;
mi_00452e04:;
    /* 00452e04 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e06 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e07 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00452e0a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452e0fu;f->boundary(0x00452e0au,0x0046cea0u,f);
    /* 00452e0f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452e12 */
    f->r[4]+=4;return;
mi_00452e13:;
    /* 00452e13 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452e19 */
    f->r[2]=f->r[1];
    /* 00452e1b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00452e1e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00452e20 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452e22 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00452e2a */
    if((f->flags&MI_ZF))goto mi_00452e37;
    /* 00452e2c */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e2e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e2f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452e34u;f->boundary(0x00452e2fu,0x0046cea0u,f);
    /* 00452e34 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452e37:;
    /* 00452e37 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452e40(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452e40 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452e45 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e46 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452e4bu;f->boundary(0x00452e46u,0x0046b840u,f);
    /* 00452e4b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452e4e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452e53u;f->boundary(0x00452e4eu,0x004539a0u,f);
    /* 00452e53 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452e55 */
    if(!(f->flags&MI_ZF))goto mi_00452f07;
    /* 00452e5b */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452e61 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e62 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452e67u;f->boundary(0x00452e62u,0x00453af0u,f);
    /* 00452e67 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452e6d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e6f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e70 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00452e75 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452e7au;f->boundary(0x00452e75u,0x00453b40u,f);
    /* 00452e7a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452e7d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452e7f */
    if(!(f->flags&MI_ZF))goto mi_00452e8d;
    /* 00452e81 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00452e8b */
    goto mi_00452eb0;
mi_00452e8d:;
    /* 00452e8d */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00452e92 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00452e95 */
    if(!(f->flags&MI_ZF))goto mi_00452eab;
    /* 00452e97 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e98 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452e9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452e9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452ea3u;f->boundary(0x00452e9eu,0x00453b40u,f);
    /* 00452ea3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452ea6 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00452eab:;
    /* 00452eab */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00452eae */
    if(!(f->flags&MI_ZF))goto mi_00452ee3;
mi_00452eb0:;
    /* 00452eb0 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452eb6 */
    f->r[1]=f->r[2];
    /* 00452eb8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00452ebb */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00452ebd */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00452ec4 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 00452ecb */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00452ecd */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00452ed4;
    /* 00452ecf */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00452ed1 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00452ed3 */
    f->r[4]+=4;return;
mi_00452ed4:;
    /* 00452ed4 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452ed6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452ed7 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00452eda */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452edfu;f->boundary(0x00452edau,0x0046cea0u,f);
    /* 00452edf */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452ee2 */
    f->r[4]+=4;return;
mi_00452ee3:;
    /* 00452ee3 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452ee9 */
    f->r[2]=f->r[1];
    /* 00452eeb */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00452eee */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00452ef0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452ef2 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00452efa */
    if((f->flags&MI_ZF))goto mi_00452f07;
    /* 00452efc */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452efe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452eff */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452f04u;f->boundary(0x00452effu,0x0046cea0u,f);
    /* 00452f04 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452f07:;
    /* 00452f07 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452f10(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452f10 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452f15 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452f16 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452f1bu;f->boundary(0x00452f16u,0x0046b840u,f);
    /* 00452f1b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452f1e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452f23u;f->boundary(0x00452f1eu,0x004539a0u,f);
    /* 00452f23 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452f25 */
    if(!(f->flags&MI_ZF))goto mi_00452fd7;
    /* 00452f2b */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452f31 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452f32 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452f37u;f->boundary(0x00452f32u,0x00453af0u,f);
    /* 00452f37 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452f3d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452f3f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452f40 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00452f45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452f4au;f->boundary(0x00452f45u,0x00453b40u,f);
    /* 00452f4a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452f4d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452f4f */
    if(!(f->flags&MI_ZF))goto mi_00452f5d;
    /* 00452f51 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00452f5b */
    goto mi_00452f80;
mi_00452f5d:;
    /* 00452f5d */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00452f62 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00452f65 */
    if(!(f->flags&MI_ZF))goto mi_00452f7b;
    /* 00452f67 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452f68 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452f6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452f6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452f73u;f->boundary(0x00452f6eu,0x00453b40u,f);
    /* 00452f73 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452f76 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00452f7b:;
    /* 00452f7b */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00452f7e */
    if(!(f->flags&MI_ZF))goto mi_00452fb3;
mi_00452f80:;
    /* 00452f80 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00452f86 */
    f->r[1]=f->r[2];
    /* 00452f88 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00452f8b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00452f8d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00452f94 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 00452f9b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00452f9d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00452fa4;
    /* 00452f9f */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00452fa1 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00452fa3 */
    f->r[4]+=4;return;
mi_00452fa4:;
    /* 00452fa4 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452fa6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452fa7 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00452faa */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452fafu;f->boundary(0x00452faau,0x0046cea0u,f);
    /* 00452faf */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452fb2 */
    f->r[4]+=4;return;
mi_00452fb3:;
    /* 00452fb3 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00452fb9 */
    f->r[2]=f->r[1];
    /* 00452fbb */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00452fbe */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00452fc0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452fc2 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00452fca */
    if((f->flags&MI_ZF))goto mi_00452fd7;
    /* 00452fcc */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452fce */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452fcf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452fd4u;f->boundary(0x00452fcfu,0x0046cea0u,f);
    /* 00452fd4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00452fd7:;
    /* 00452fd7 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00452fe0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00452fe0 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00452fe5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00452fe6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452febu;f->boundary(0x00452fe6u,0x0046b840u,f);
    /* 00452feb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00452fee */
    f->r[4]-=4;MI_U32(f->r[4])=0x00452ff3u;f->boundary(0x00452feeu,0x004539a0u,f);
    /* 00452ff3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00452ff5 */
    if(!(f->flags&MI_ZF))goto mi_004530a7;
    /* 00452ffb */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00453001 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453002 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453007u;f->boundary(0x00453002u,0x00453af0u,f);
    /* 00453007 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045300d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045300f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453010 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00453015 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045301au;f->boundary(0x00453015u,0x00453b40u,f);
    /* 0045301a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045301d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045301f */
    if(!(f->flags&MI_ZF))goto mi_0045302d;
    /* 00453021 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 0045302b */
    goto mi_00453050;
mi_0045302d:;
    /* 0045302d */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00453032 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453035 */
    if(!(f->flags&MI_ZF))goto mi_0045304b;
    /* 00453037 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453038 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 0045303d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045303e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453043u;f->boundary(0x0045303eu,0x00453b40u,f);
    /* 00453043 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453046 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_0045304b:;
    /* 0045304b */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0045304e */
    if(!(f->flags&MI_ZF))goto mi_00453083;
mi_00453050:;
    /* 00453050 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00453056 */
    f->r[1]=f->r[2];
    /* 00453058 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0045305b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045305d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00453064 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 0045306b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045306d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453074;
    /* 0045306f */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00453071 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00453073 */
    f->r[4]+=4;return;
mi_00453074:;
    /* 00453074 */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453076 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453077 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0045307a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045307fu;f->boundary(0x0045307au,0x0046cea0u,f);
    /* 0045307f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453082 */
    f->r[4]+=4;return;
mi_00453083:;
    /* 00453083 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00453089 */
    f->r[2]=f->r[1];
    /* 0045308b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045308e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00453090 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453092 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 0045309a */
    if((f->flags&MI_ZF))goto mi_004530a7;
    /* 0045309c */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045309e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045309f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004530a4u;f->boundary(0x0045309fu,0x0046cea0u,f);
    /* 004530a4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004530a7:;
    /* 004530a7 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004530b0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004530b0 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004530b5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004530b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004530bbu;f->boundary(0x004530b6u,0x0046b840u,f);
    /* 004530bb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004530be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004530c3u;f->boundary(0x004530beu,0x004539a0u,f);
    /* 004530c3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004530c5 */
    if(!(f->flags&MI_ZF))goto mi_0045313f;
    /* 004530c7 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004530cd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004530ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004530d3u;f->boundary(0x004530ceu,0x00453af0u,f);
    /* 004530d3 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004530d9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004530db */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004530dc */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 004530e1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004530e6u;f->boundary(0x004530e1u,0x00453b40u,f);
    /* 004530e6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004530e9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004530eb */
    if(!(f->flags&MI_ZF))goto mi_004530f9;
    /* 004530ed */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 004530f7 */
    goto mi_0045311c;
mi_004530f9:;
    /* 004530f9 */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 004530fe */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453101 */
    if(!(f->flags&MI_ZF))goto mi_00453117;
    /* 00453103 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453104 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453109 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045310a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045310fu;f->boundary(0x0045310au,0x00453b40u,f);
    /* 0045310f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453112 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00453117:;
    /* 00453117 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0045311a */
    if(!(f->flags&MI_ZF))goto mi_0045314f;
mi_0045311c:;
    /* 0045311c */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00453122 */
    f->r[1]=f->r[2];
    /* 00453124 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00453127 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00453129 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[1]*8u)))&0xffu)<<0);
    /* 00453130 */
    f->r[1]=(0x00799a5cu+f->r[1]*8u);
    /* 00453137 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00453139 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453140;
    /* 0045313b */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045313d */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
mi_0045313f:;
    /* 0045313f */
    f->r[4]+=4;return;
mi_00453140:;
    /* 00453140 */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453142 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453143 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00453146 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045314bu;f->boundary(0x00453146u,0x0046cea0u,f);
    /* 0045314b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045314e */
    f->r[4]+=4;return;
mi_0045314f:;
    /* 0045314f */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00453155 */
    f->r[2]=f->r[1];
    /* 00453157 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045315a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045315c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045315e */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00453166 */
    if((f->flags&MI_ZF))goto mi_0045313f;
    /* 00453168 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045316a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045316b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453170u;f->boundary(0x0045316bu,0x0046b480u,f);
    /* 00453170 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453173 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453175 */
    if((f->flags&MI_ZF))goto mi_0045313f;
    /* 00453177 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 0045317c */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045317e */
    f->r[1]=f->r[0];
    /* 00453180 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453181 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00453184 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00453186 */
    MI_U8((0x007999dcu+f->r[1]*8u))=(uint8_t)(0x00000000u);
    /* 0045318e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453193u;f->boundary(0x0045318eu,0x0046b940u,f);
    /* 00453193 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00453199 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045319b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045319c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004531a1u;f->boundary(0x0045319cu,0x0046b9e0u,f);
    /* 004531a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004531a2 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004531a7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004531a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004531adu;f->boundary(0x004531a8u,0x0046bab0u,f);
    /* 004531ad */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004531b0 */
    MI_U32((0x0078d588u))=(uint32_t)(0x00000001u);
    /* 004531ba */
    f->boundary(0x004531bau,0x00476530u,f);return;
}

void restore_building_control_C_004531c0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004531c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004531c5u;f->boundary(0x004531c0u,0x004539a0u,f);
    /* 004531c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004531c7 */
    if(!(f->flags&MI_ZF))goto mi_00453277;
    /* 004531cd */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004531d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004531d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004531d8u;f->boundary(0x004531d3u,0x00453af0u,f);
    /* 004531d8 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004531de */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004531e0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004531e1 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 004531e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004531ebu;f->boundary(0x004531e6u,0x00453b40u,f);
    /* 004531eb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004531ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004531f0 */
    if(!(f->flags&MI_ZF))goto mi_004531fe;
    /* 004531f2 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 004531fc */
    goto mi_00453222;
mi_004531fe:;
    /* 004531fe */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00453203 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453206 */
    if(!(f->flags&MI_ZF))goto mi_0045321d;
    /* 00453208 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045320e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045320f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453210 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453215u;f->boundary(0x00453210u,0x00453b40u,f);
    /* 00453215 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453218 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_0045321d:;
    /* 0045321d */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00453220 */
    if(!(f->flags&MI_ZF))goto mi_00453253;
mi_00453222:;
    /* 00453222 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00453228 */
    f->r[0]=f->r[2];
    /* 0045322a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045322d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045322f */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 00453236 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 0045323d */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045323f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453246;
    /* 00453241 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00453243 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00453245 */
    f->r[4]+=4;return;
mi_00453246:;
    /* 00453246 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453247 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0045324a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045324fu;f->boundary(0x0045324au,0x0046c1c0u,f);
    /* 0045324f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453252 */
    f->r[4]+=4;return;
mi_00453253:;
    /* 00453253 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00453259 */
    f->r[2]=f->r[1];
    /* 0045325b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045325e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00453260 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453262 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 0045326a */
    if((f->flags&MI_ZF))goto mi_00453277;
    /* 0045326c */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045326e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045326f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453274u;f->boundary(0x0045326fu,0x0046cea0u,f);
    /* 00453274 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00453277:;
    /* 00453277 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00453280(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453280 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453285u;f->boundary(0x00453280u,0x004539a0u,f);
    /* 00453285 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453287 */
    if(!(f->flags&MI_ZF))goto mi_00453339;
    /* 0045328d */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453292 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453293 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453298u;f->boundary(0x00453293u,0x00453af0u,f);
    /* 00453298 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045329e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004532a0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004532a1 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 004532a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004532abu;f->boundary(0x004532a6u,0x00453b40u,f);
    /* 004532ab */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004532ae */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004532b0 */
    if(!(f->flags&MI_ZF))goto mi_004532be;
    /* 004532b2 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 004532bc */
    goto mi_004532e2;
mi_004532be:;
    /* 004532be */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 004532c3 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004532c6 */
    if(!(f->flags&MI_ZF))goto mi_004532dd;
    /* 004532c8 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004532ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004532cf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004532d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004532d5u;f->boundary(0x004532d0u,0x00453b40u,f);
    /* 004532d5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004532d8 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_004532dd:;
    /* 004532dd */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004532e0 */
    if(!(f->flags&MI_ZF))goto mi_00453315;
mi_004532e2:;
    /* 004532e2 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004532e8 */
    f->r[0]=f->r[2];
    /* 004532ea */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004532ed */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004532ef */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 004532f6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 004532fd */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004532ff */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453306;
    /* 00453301 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00453303 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00453305 */
    f->r[4]+=4;return;
mi_00453306:;
    /* 00453306 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453308 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453309 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0045330c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453311u;f->boundary(0x0045330cu,0x0046cea0u,f);
    /* 00453311 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453314 */
    f->r[4]+=4;return;
mi_00453315:;
    /* 00453315 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045331b */
    f->r[2]=f->r[1];
    /* 0045331d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00453320 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00453322 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453324 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 0045332c */
    if((f->flags&MI_ZF))goto mi_00453339;
    /* 0045332e */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453330 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453331 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453336u;f->boundary(0x00453331u,0x0046cea0u,f);
    /* 00453336 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00453339:;
    /* 00453339 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00453340(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453340 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453345u;f->boundary(0x00453340u,0x004539a0u,f);
    /* 00453345 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453347 */
    if(!(f->flags&MI_ZF))goto mi_004533f9;
    /* 0045334d */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453352 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453353 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453358u;f->boundary(0x00453353u,0x00453af0u,f);
    /* 00453358 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045335e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453360 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453361 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00453366 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045336bu;f->boundary(0x00453366u,0x00453b40u,f);
    /* 0045336b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045336e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453370 */
    if(!(f->flags&MI_ZF))goto mi_0045337e;
    /* 00453372 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 0045337c */
    goto mi_004533a2;
mi_0045337e:;
    /* 0045337e */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00453383 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453386 */
    if(!(f->flags&MI_ZF))goto mi_0045339d;
    /* 00453388 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045338e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045338f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453390 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453395u;f->boundary(0x00453390u,0x00453b40u,f);
    /* 00453395 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453398 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_0045339d:;
    /* 0045339d */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004533a0 */
    if(!(f->flags&MI_ZF))goto mi_004533d5;
mi_004533a2:;
    /* 004533a2 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004533a8 */
    f->r[0]=f->r[2];
    /* 004533aa */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004533ad */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004533af */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 004533b6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 004533bd */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004533bf */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004533c6;
    /* 004533c1 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004533c3 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 004533c5 */
    f->r[4]+=4;return;
mi_004533c6:;
    /* 004533c6 */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004533c8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004533c9 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004533cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004533d1u;f->boundary(0x004533ccu,0x0046cea0u,f);
    /* 004533d1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004533d4 */
    f->r[4]+=4;return;
mi_004533d5:;
    /* 004533d5 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004533db */
    f->r[2]=f->r[1];
    /* 004533dd */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004533e0 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004533e2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004533e4 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 004533ec */
    if((f->flags&MI_ZF))goto mi_004533f9;
    /* 004533ee */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004533f0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004533f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004533f6u;f->boundary(0x004533f1u,0x0046cea0u,f);
    /* 004533f6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004533f9:;
    /* 004533f9 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00453400(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453400 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453405u;f->boundary(0x00453400u,0x004539a0u,f);
    /* 00453405 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453407 */
    if(!(f->flags&MI_ZF))goto mi_00453481;
    /* 00453409 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 0045340e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045340f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453414u;f->boundary(0x0045340fu,0x00453af0u,f);
    /* 00453414 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045341a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045341c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045341d */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00453422 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453427u;f->boundary(0x00453422u,0x00453b40u,f);
    /* 00453427 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045342a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045342c */
    if(!(f->flags&MI_ZF))goto mi_0045343a;
    /* 0045342e */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00453438 */
    goto mi_0045345e;
mi_0045343a:;
    /* 0045343a */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 0045343f */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453442 */
    if(!(f->flags&MI_ZF))goto mi_00453459;
    /* 00453444 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045344a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045344b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045344c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453451u;f->boundary(0x0045344cu,0x00453b40u,f);
    /* 00453451 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453454 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00453459:;
    /* 00453459 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0045345c */
    if(!(f->flags&MI_ZF))goto mi_00453491;
mi_0045345e:;
    /* 0045345e */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00453464 */
    f->r[0]=f->r[2];
    /* 00453466 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00453469 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045346b */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 00453472 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 00453479 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045347b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453482;
    /* 0045347d */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045347f */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
mi_00453481:;
    /* 00453481 */
    f->r[4]+=4;return;
mi_00453482:;
    /* 00453482 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453484 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453485 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 00453488 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045348du;f->boundary(0x00453488u,0x0046cea0u,f);
    /* 0045348d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453490 */
    f->r[4]+=4;return;
mi_00453491:;
    /* 00453491 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00453497 */
    f->r[2]=f->r[1];
    /* 00453499 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045349c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0045349e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004534a0 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 004534a8 */
    if((f->flags&MI_ZF))goto mi_00453481;
    /* 004534aa */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534ac */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004534b2u;f->boundary(0x004534adu,0x0046b480u,f);
    /* 004534b2 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004534b5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004534b7 */
    if((f->flags&MI_ZF))goto mi_00453481;
    /* 004534b9 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004534be */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534c0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534c1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004534c6u;f->boundary(0x004534c1u,0x0046b940u,f);
    /* 004534c6 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004534cc */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534ce */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004534d4u;f->boundary(0x004534cfu,0x0046b9e0u,f);
    /* 004534d4 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004534da */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534db */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004534dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004534e1u;f->boundary(0x004534dcu,0x0046bbc0u,f);
    /* 004534e1 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004534e4 */
    MI_U32((0x0078d588u))=(uint32_t)(0x00000001u);
    /* 004534ee */
    f->boundary(0x004534eeu,0x00476530u,f);return;
}

void restore_building_control_C_00453500(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453500 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453505u;f->boundary(0x00453500u,0x004539a0u,f);
    /* 00453505 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453507 */
    if(!(f->flags&MI_ZF))goto mi_004535b7;
    /* 0045350d */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453512 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453513 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453518u;f->boundary(0x00453513u,0x00453af0u,f);
    /* 00453518 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045351e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453520 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453521 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00453526 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045352bu;f->boundary(0x00453526u,0x00453b40u,f);
    /* 0045352b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045352e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453530 */
    if(!(f->flags&MI_ZF))goto mi_0045353e;
    /* 00453532 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 0045353c */
    goto mi_00453562;
mi_0045353e:;
    /* 0045353e */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00453543 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453546 */
    if(!(f->flags&MI_ZF))goto mi_0045355d;
    /* 00453548 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045354e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045354f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453550 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453555u;f->boundary(0x00453550u,0x00453b40u,f);
    /* 00453555 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453558 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_0045355d:;
    /* 0045355d */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00453560 */
    if(!(f->flags&MI_ZF))goto mi_00453593;
mi_00453562:;
    /* 00453562 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00453568 */
    f->r[0]=f->r[2];
    /* 0045356a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045356d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045356f */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 00453576 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 0045357d */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045357f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453586;
    /* 00453581 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00453583 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00453585 */
    f->r[4]+=4;return;
mi_00453586:;
    /* 00453586 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453587 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0045358a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045358fu;f->boundary(0x0045358au,0x0046cb30u,f);
    /* 0045358f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453592 */
    f->r[4]+=4;return;
mi_00453593:;
    /* 00453593 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 00453599 */
    f->r[2]=f->r[1];
    /* 0045359b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0045359e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004535a0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004535a2 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 004535aa */
    if((f->flags&MI_ZF))goto mi_004535b7;
    /* 004535ac */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004535ae */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004535af */
    f->r[4]-=4;MI_U32(f->r[4])=0x004535b4u;f->boundary(0x004535afu,0x0046cea0u,f);
    /* 004535b4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004535b7:;
    /* 004535b7 */
    f->r[4]+=4;return;
}

void restore_building_control_C_004535c0(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004535c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004535c5u;f->boundary(0x004535c0u,0x004539a0u,f);
    /* 004535c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004535c7 */
    if(!(f->flags&MI_ZF))goto mi_00453679;
    /* 004535cd */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004535d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004535d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004535d8u;f->boundary(0x004535d3u,0x00453af0u,f);
    /* 004535d8 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004535de */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004535e0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004535e1 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 004535e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004535ebu;f->boundary(0x004535e6u,0x00453b40u,f);
    /* 004535eb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004535ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004535f0 */
    if(!(f->flags&MI_ZF))goto mi_004535fe;
    /* 004535f2 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 004535fc */
    goto mi_00453622;
mi_004535fe:;
    /* 004535fe */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00453603 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453606 */
    if(!(f->flags&MI_ZF))goto mi_0045361d;
    /* 00453608 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045360e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045360f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453610 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453615u;f->boundary(0x00453610u,0x00453b40u,f);
    /* 00453615 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453618 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_0045361d:;
    /* 0045361d */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00453620 */
    if(!(f->flags&MI_ZF))goto mi_00453655;
mi_00453622:;
    /* 00453622 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 00453628 */
    f->r[0]=f->r[2];
    /* 0045362a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0045362d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045362f */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 00453636 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 0045363d */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0045363f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453646;
    /* 00453641 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00453643 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00453645 */
    f->r[4]+=4;return;
mi_00453646:;
    /* 00453646 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453648 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453649 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0045364c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453651u;f->boundary(0x0045364cu,0x0046cea0u,f);
    /* 00453651 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453654 */
    f->r[4]+=4;return;
mi_00453655:;
    /* 00453655 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045365b */
    f->r[2]=f->r[1];
    /* 0045365d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00453660 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00453662 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453664 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 0045366c */
    if((f->flags&MI_ZF))goto mi_00453679;
    /* 0045366e */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453670 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453671 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453676u;f->boundary(0x00453671u,0x0046cea0u,f);
    /* 00453676 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00453679:;
    /* 00453679 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00453680(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453680 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453685u;f->boundary(0x00453680u,0x004539a0u,f);
    /* 00453685 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453687 */
    if(!(f->flags&MI_ZF))goto mi_00453739;
    /* 0045368d */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453692 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453693 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453698u;f->boundary(0x00453693u,0x00453af0u,f);
    /* 00453698 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045369e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004536a0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004536a1 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 004536a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004536abu;f->boundary(0x004536a6u,0x00453b40u,f);
    /* 004536ab */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004536ae */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004536b0 */
    if(!(f->flags&MI_ZF))goto mi_004536be;
    /* 004536b2 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 004536bc */
    goto mi_004536e2;
mi_004536be:;
    /* 004536be */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 004536c3 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004536c6 */
    if(!(f->flags&MI_ZF))goto mi_004536dd;
    /* 004536c8 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004536ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004536cf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004536d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004536d5u;f->boundary(0x004536d0u,0x00453b40u,f);
    /* 004536d5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004536d8 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_004536dd:;
    /* 004536dd */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004536e0 */
    if(!(f->flags&MI_ZF))goto mi_00453715;
mi_004536e2:;
    /* 004536e2 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004536e8 */
    f->r[0]=f->r[2];
    /* 004536ea */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004536ed */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004536ef */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 004536f6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 004536fd */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004536ff */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00453706;
    /* 00453701 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00453703 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00453705 */
    f->r[4]+=4;return;
mi_00453706:;
    /* 00453706 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453708 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453709 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0045370c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453711u;f->boundary(0x0045370cu,0x0046cea0u,f);
    /* 00453711 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453714 */
    f->r[4]+=4;return;
mi_00453715:;
    /* 00453715 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045371b */
    f->r[2]=f->r[1];
    /* 0045371d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00453720 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00453722 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453724 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 0045372c */
    if((f->flags&MI_ZF))goto mi_00453739;
    /* 0045372e */
    a=0x0000001bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453730 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453731 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453736u;f->boundary(0x00453731u,0x0046cea0u,f);
    /* 00453736 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00453739:;
    /* 00453739 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00453740(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453740 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453745u;f->boundary(0x00453740u,0x004539a0u,f);
    /* 00453745 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453747 */
    if(!(f->flags&MI_ZF))goto mi_004537c1;
    /* 00453749 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 0045374e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045374f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453754u;f->boundary(0x0045374fu,0x00453af0u,f);
    /* 00453754 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045375a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045375c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045375d */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00453762 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453767u;f->boundary(0x00453762u,0x00453b40u,f);
    /* 00453767 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045376a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0045376c */
    if(!(f->flags&MI_ZF))goto mi_0045377a;
    /* 0045376e */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 00453778 */
    goto mi_0045379e;
mi_0045377a:;
    /* 0045377a */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 0045377f */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453782 */
    if(!(f->flags&MI_ZF))goto mi_00453799;
    /* 00453784 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045378a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045378b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045378c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453791u;f->boundary(0x0045378cu,0x00453b40u,f);
    /* 00453791 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453794 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_00453799:;
    /* 00453799 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0045379c */
    if(!(f->flags&MI_ZF))goto mi_004537d1;
mi_0045379e:;
    /* 0045379e */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004537a4 */
    f->r[0]=f->r[2];
    /* 004537a6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004537a9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004537ab */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 004537b2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 004537b9 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004537bb */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004537c2;
    /* 004537bd */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004537bf */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
mi_004537c1:;
    /* 004537c1 */
    f->r[4]+=4;return;
mi_004537c2:;
    /* 004537c2 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004537c4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004537c5 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004537c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004537cdu;f->boundary(0x004537c8u,0x0046cea0u,f);
    /* 004537cd */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004537d0 */
    f->r[4]+=4;return;
mi_004537d1:;
    /* 004537d1 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004537d7 */
    f->r[2]=f->r[1];
    /* 004537d9 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004537dc */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004537de */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004537e0 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 004537e8 */
    if((f->flags&MI_ZF))goto mi_004537c1;
    /* 004537ea */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004537ec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004537ed */
    f->r[4]-=4;MI_U32(f->r[4])=0x004537f2u;f->boundary(0x004537edu,0x0046b480u,f);
    /* 004537f2 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004537f5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004537f7 */
    if((f->flags&MI_ZF))goto mi_004537c1;
    /* 004537f9 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 004537fe */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453800 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453801 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453806u;f->boundary(0x00453801u,0x0046b940u,f);
    /* 00453806 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045380c */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045380e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045380f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453814u;f->boundary(0x0045380fu,0x0046b9e0u,f);
    /* 00453814 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045381a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045381b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045381c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453821u;f->boundary(0x0045381cu,0x0046bcb0u,f);
    /* 00453821 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453824 */
    MI_U32((0x0078d588u))=(uint32_t)(0x00000001u);
    /* 0045382e */
    f->boundary(0x0045382eu,0x00476530u,f);return;
}

void restore_building_control_C_00453840(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453840 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453845u;f->boundary(0x00453840u,0x004539a0u,f);
    /* 00453845 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453847 */
    if(!(f->flags&MI_ZF))goto mi_004538f7;
    /* 0045384d */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453852 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453853 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453858u;f->boundary(0x00453853u,0x00453af0u,f);
    /* 00453858 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045385e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453860 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453861 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00453866 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045386bu;f->boundary(0x00453866u,0x00453b40u,f);
    /* 0045386b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045386e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453870 */
    if(!(f->flags&MI_ZF))goto mi_0045387e;
    /* 00453872 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 0045387c */
    goto mi_004538a2;
mi_0045387e:;
    /* 0045387e */
    f->r[0]=MI_U32((0x0078d7e4u));
    /* 00453883 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00453886 */
    if(!(f->flags&MI_ZF))goto mi_0045389d;
    /* 00453888 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 0045388e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045388f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453890 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453895u;f->boundary(0x00453890u,0x00453b40u,f);
    /* 00453895 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00453898 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
mi_0045389d:;
    /* 0045389d */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004538a0 */
    if(!(f->flags&MI_ZF))goto mi_004538d3;
mi_004538a2:;
    /* 004538a2 */
    f->r[2]=MI_U32((0x0078d838u));
    /* 004538a8 */
    f->r[0]=f->r[2];
    /* 004538aa */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004538ad */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004538af */
    f->r[1]=(0x00799a5cu+f->r[0]*8u);
    /* 004538b6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[0]*8u)))&0xffu)<<0);
    /* 004538bd */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004538bf */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004538c6;
    /* 004538c1 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004538c3 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 004538c5 */
    f->r[4]+=4;return;
mi_004538c6:;
    /* 004538c6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004538c7 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 004538ca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004538cfu;f->boundary(0x004538cau,0x0046ccf0u,f);
    /* 004538cf */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004538d2 */
    f->r[4]+=4;return;
mi_004538d3:;
    /* 004538d3 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 004538d9 */
    f->r[2]=f->r[1];
    /* 004538db */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004538de */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004538e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004538e2 */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 004538ea */
    if((f->flags&MI_ZF))goto mi_004538f7;
    /* 004538ec */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004538ee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004538ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004538f4u;f->boundary(0x004538efu,0x0046cea0u,f);
    /* 004538f4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004538f7:;
    /* 004538f7 */
    f->r[4]+=4;return;
}

void restore_building_control_C_00453900(struct restore_building_control_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00453900 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453905u;f->boundary(0x00453900u,0x004539a0u,f);
    /* 00453905 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453907 */
    if(!(f->flags&MI_ZF))goto mi_0045399f;
    /* 0045390d */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453912 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453913 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453918u;f->boundary(0x00453913u,0x00453af0u,f);
    /* 00453918 */
    f->r[1]=MI_U32((0x0078d838u));
    /* 0045391e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453920 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453921 */
    MI_U32((0x0078d7e4u))=(uint32_t)(f->r[0]);
    /* 00453926 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045392bu;f->boundary(0x00453926u,0x00453b40u,f);
    /* 0045392b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045392e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00453930 */
    if(!(f->flags&MI_ZF))goto mi_0045393e;
    /* 00453932 */
    MI_U32((0x0078d7e4u))=(uint32_t)(0xffffffffu);
    /* 0045393c */
    goto mi_00453949;
mi_0045393e:;
    /* 0045393e */
    f->r[1]=MI_U32((0x0078d7e4u));
    /* 00453944 */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00453947 */
    if(!(f->flags&MI_ZF))goto mi_0045397c;
mi_00453949:;
    /* 00453949 */
    f->r[0]=MI_U32((0x0078d838u));
    /* 0045394e */
    f->r[2]=f->r[0];
    /* 00453950 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00453953 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00453955 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00799a5cu+f->r[2]*8u)))&0xffu)<<0);
    /* 0045395c */
    f->r[2]=(0x00799a5cu+f->r[2]*8u);
    /* 00453963 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00453966 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0045396d;
    /* 00453968 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045396a */
    MI_U8((0x00000000u+f->r[2]))=(uint8_t)((f->r[1]&0xffu));
    /* 0045396c */
    f->r[4]+=4;return;
mi_0045396d:;
    /* 0045396d */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045396f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453970 */
    MI_U8((0x00000000u+f->r[2]))=(uint8_t)(0x00000000u);
    /* 00453973 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00453978u;f->boundary(0x00453973u,0x0046cea0u,f);
    /* 00453978 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045397b */
    f->r[4]+=4;return;
mi_0045397c:;
    /* 0045397c */
    f->r[0]=MI_U32((0x0078d838u));
    /* 00453981 */
    f->r[2]=f->r[0];
    /* 00453983 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00453986 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00453988 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0045398a */
    MI_U8((0x00799a5cu+f->r[2]*8u))=(uint8_t)(0x00000000u);
    /* 00453992 */
    if((f->flags&MI_ZF))goto mi_0045399f;
    /* 00453994 */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453996 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00453997 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045399cu;f->boundary(0x00453997u,0x0046cea0u,f);
    /* 0045399c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0045399f:;
    /* 0045399f */
    f->r[4]+=4;return;
}
