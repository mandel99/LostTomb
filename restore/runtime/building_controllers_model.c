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





