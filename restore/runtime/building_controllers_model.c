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



