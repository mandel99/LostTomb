/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_004f7e50.h"
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
static __inline uint32_t mi_alu(struct restore_menu_input_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_menu_input_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_menu_input_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_004f8900[]={0x11,0x87,0x4f,0x00,0xdd,0x86,0x4f,0x00,0xcc,0x86,0x4f,0x00,0xf4,0x86,0x4f,0x00};
static const uint8_t mi_rom_004f8910[]={0x00,0x03,0x03,0x03,0x03,0x03,0x00,0x03,0x03,0x03,0x03,0x01,0x01,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x03,0x00,0x03,0x03,0x02,0x02};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004f8900u&&width<=16u&&(offset=address-0x004f8900u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004f8900[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004f8910u&&width<=79u&&(offset=address-0x004f8910u)<=79u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004f8910[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_004f7e50(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004f7e50 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7e51 */
    f->r[0]=mi_read((0x00605a3cu),4u);
    /* 004f7e56 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7e57 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7e58 */
    f->r[5]=0x00000001u;
    /* 004f7e5d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7e5e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004f7e60 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7e61 */
    if(!(f->flags&MI_ZF))goto mi_004f7e6b;
    /* 004f7e63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f7e64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f7e65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f7e66 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f7e68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f7e69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004f7e6a */
    f->r[4]+=4;return;
mi_004f7e6b:;
    /* 004f7e6b */
    f->r[0]=mi_read((0x00e950d8u),4u);
    /* 004f7e70 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004f7e72 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004f7e74 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f7e83;
    /* 004f7e76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f7e7bu;f->boundary(0x004f7e76u,0x004e2ab0u,f);
    /* 004f7e7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f7e7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f7e7d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f7e7e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f7e80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f7e81 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004f7e82 */
    f->r[4]+=4;return;
mi_004f7e83:;
    /* 004f7e83 */
    f->r[0]=mi_read((0x00bca2ccu),4u);
    /* 004f7e88 */
    f->r[2]=mi_read((0x00bca2c8u),4u);
    /* 004f7e8e */
    f->r[7]=mi_read((0x00bca2d8u),4u);
    /* 004f7e94 */
    MI_U32((0x00eadd78u))=(uint32_t)(f->r[0]);
    /* 004f7e99 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004f7e9c */
    MI_U32((0x00eadd7cu))=(uint32_t)(f->r[2]);
    /* 004f7ea2 */
    f->r[3]=0x00000009u;
    /* 004f7ea7 */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[6]);
    /* 004f7ead */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004f7eb0 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[6]);
    /* 004f7eb6 */
    MI_U32((0x007056b8u))=(uint32_t)(f->r[6]);
    /* 004f7ebc */
    MI_U32((0x00eadd94u))=(uint32_t)(f->r[5]);
    /* 004f7ec2 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004f7ec5 */
    MI_U32((0x00eadcc0u))=(uint32_t)(f->r[6]);
    /* 004f7ecb */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004f7ed1 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004f7ed8 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 004f7edb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f7edd */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f7edf */
    f->r[7]=f->r[1];
    /* 004f7ee1 */
    MI_U32((0x00eadd80u))=(uint32_t)(f->r[1]);
    /* 004f7ee7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004f7eeb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[7]*2u),2u);
    /* 004f7ef3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00a9010cu+f->r[7]),1u))&0xffu)<<0);
    /* 004f7ef9 */
    f->r[1]=f->r[0];
    /* 004f7efb */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f7efe */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f7f00 */
    MI_U32((0x00eadd6cu))=(uint32_t)(f->r[0]);
    /* 004f7f05 */
    MI_U32((0x00eadd68u))=(uint32_t)(f->r[2]);
    /* 004f7f0b */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 004f7f13 */
    a=(f->r[1]&0xffffu);b=0x000000dau;t=mi_alu(f,a,b,16,1);
    /* 004f7f18 */
    if((f->flags&MI_ZF))goto mi_004f7f28;
    /* 004f7f1a */
    a=(f->r[1]&0xffffu);b=0x000000dbu;t=mi_alu(f,a,b,16,1);
    /* 004f7f1f */
    if((f->flags&MI_ZF))goto mi_004f7f28;
    /* 004f7f21 */
    a=(f->r[1]&0xffffu);b=0x000000dcu;t=mi_alu(f,a,b,16,1);
    /* 004f7f26 */
    if(!(f->flags&MI_ZF))goto mi_004f7f36;
mi_004f7f28:;
    /* 004f7f28 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7f29 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f7f2eu;f->boundary(0x004f7f29u,0x004f5d90u,f);
    /* 004f7f2e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f7f31 */
    MI_U32((0x00eadd6cu))=(uint32_t)(f->r[0]);
mi_004f7f36:;
    /* 004f7f36 */
    f->r[2]=f->r[0];
    /* 004f7f38 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f7f3b */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f7f3d */
    a=mi_read((0x007999e8u+f->r[2]*8u),2u);b=0x000000d7u;t=mi_alu(f,a,b,16,1);
    /* 004f7f47 */
    if(!(f->flags&MI_ZF))goto mi_004f7f57;
    /* 004f7f49 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7f4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f7f4fu;f->boundary(0x004f7f4au,0x004f6000u,f);
    /* 004f7f4f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f7f52 */
    MI_U32((0x00eadd6cu))=(uint32_t)(f->r[0]);
mi_004f7f57:;
    /* 004f7f57 */
    f->r[1]=mi_read((0x00af598cu+f->r[7]*4u),4u);
    /* 004f7f5e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f7f60 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00ae8e7cu+f->r[7]),1u))&0xffu)<<0);
    /* 004f7f66 */
    a=f->r[1];b=0x00000080u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004f7f6c */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004f7f6e */
    MI_U32((0x00eadd64u))=(uint32_t)(f->r[1]);
    /* 004f7f74 */
    MI_U32((0x00eadd60u))=(uint32_t)(f->r[2]);
    /* 004f7f7a */
    if((f->flags&MI_ZF))goto mi_004f7f9d;
    /* 004f7f7c */
    f->r[1]=f->r[0];
    /* 004f7f7e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f7f81 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f7f83 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999d8u+f->r[1]*8u),1u))&0xffu)<<0);
    /* 004f7f8a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f7f8c */
    if(!(f->flags&MI_ZF))goto mi_004f7f9d;
    /* 004f7f8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f7f8f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f7f90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f7f91 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004f7f98 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f7f9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f7f9b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004f7f9c */
    f->r[4]+=4;return;
mi_004f7f9d:;
    /* 004f7f9d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f7fa2u;f->boundary(0x004f7f9du,0x005596d0u,f);
    /* 004f7fa2 */
    f->r[2]=mi_read((0x00eadd6cu),4u);
    /* 004f7fa8 */
    f->r[0]=f->r[2];
    /* 004f7faa */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f7fad */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f7faf */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[0]*8u),2u))&0xffffu)<<0);
    /* 004f7fb7 */
    a=(f->r[0]&0xffffu);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004f7fbb */
    if(!(f->flags&MI_ZF))goto mi_004f7fd0;
    /* 004f7fbd */
    a=0x00eadc48u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7fc2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7fc3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f7fc8u;f->boundary(0x004f7fc3u,0x004f1310u,f);
    /* 004f7fc8 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f7fcb */
    goto mi_004f80c6;
mi_004f7fd0:;
    /* 004f7fd0 */
    a=(f->r[0]&0xffffu);b=0x000000d5u;t=mi_alu(f,a,b,16,1);
    /* 004f7fd4 */
    if(!(f->flags&MI_ZF))goto mi_004f7fe9;
    /* 004f7fd6 */
    a=0x00eadc10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7fdb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7fdc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f7fe1u;f->boundary(0x004f7fdcu,0x00542ad0u,f);
    /* 004f7fe1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f7fe4 */
    goto mi_004f80c6;
mi_004f7fe9:;
    /* 004f7fe9 */
    a=(f->r[0]&0xffffu);b=0x000000e1u;t=mi_alu(f,a,b,16,1);
    /* 004f7fed */
    if(!(f->flags&MI_ZF))goto mi_004f8004;
    /* 004f7fef */
    a=0x00eadbd8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7ff4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f7ff5 */
    f->r[1]=0x006fd5d8u;
    /* 004f7ffa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f7fffu;f->boundary(0x004f7ffau,0x0040d940u,f);
    /* 004f7fff */
    goto mi_004f80c6;
mi_004f8004:;
    /* 004f8004 */
    a=(f->r[0]&0xffffu);b=0x000000e6u;t=mi_alu(f,a,b,16,1);
    /* 004f8008 */
    if(!(f->flags&MI_ZF))goto mi_004f801f;
    /* 004f800a */
    a=0x00eadbc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f800f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8010 */
    f->r[1]=0x006de650u;
    /* 004f8015 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f801au;f->boundary(0x004f8015u,0x00401540u,f);
    /* 004f801a */
    goto mi_004f80c6;
mi_004f801f:;
    /* 004f801f */
    a=(f->r[0]&0xffffu);b=0x000000e3u;t=mi_alu(f,a,b,16,1);
    /* 004f8023 */
    if(!(f->flags&MI_ZF))goto mi_004f803a;
    /* 004f8025 */
    a=0x00eadba8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f802a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f802b */
    f->r[1]=0x006fd368u;
    /* 004f8030 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8035u;f->boundary(0x004f8030u,0x00417050u,f);
    /* 004f8035 */
    goto mi_004f80c6;
mi_004f803a:;
    /* 004f803a */
    a=(f->r[0]&0xffffu);b=0x000000e4u;t=mi_alu(f,a,b,16,1);
    /* 004f803e */
    if(!(f->flags&MI_ZF))goto mi_004f8052;
    /* 004f8040 */
    a=0x00eadb88u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8045 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8046 */
    f->r[1]=0x006de548u;
    /* 004f804b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8050u;f->boundary(0x004f804bu,0x004e6710u,f);
    /* 004f8050 */
    goto mi_004f80c6;
mi_004f8052:;
    /* 004f8052 */
    a=(f->r[0]&0xffffu);b=0x000000deu;t=mi_alu(f,a,b,16,1);
    /* 004f8056 */
    if(!(f->flags&MI_ZF))goto mi_004f8068;
    /* 004f8058 */
    a=0x00eadb58u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f805d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f805e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8063u;f->boundary(0x004f805eu,0x004df040u,f);
    /* 004f8063 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f8066 */
    goto mi_004f80c6;
mi_004f8068:;
    /* 004f8068 */
    a=(f->r[0]&0xffffu);b=0x000000e5u;t=mi_alu(f,a,b,16,1);
    /* 004f806c */
    if(!(f->flags&MI_ZF))goto mi_004f8080;
    /* 004f806e */
    a=0x00eadb38u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8073 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8074 */
    f->r[1]=0x006f9320u;
    /* 004f8079 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f807eu;f->boundary(0x004f8079u,0x00545740u,f);
    /* 004f807e */
    goto mi_004f80c6;
mi_004f8080:;
    /* 004f8080 */
    a=(f->r[0]&0xffffu);b=0x000000eau;t=mi_alu(f,a,b,16,1);
    /* 004f8084 */
    if(!(f->flags&MI_ZF))goto mi_004f8098;
    /* 004f8086 */
    a=0x00eadb38u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f808b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f808c */
    f->r[1]=0x006f5648u;
    /* 004f8091 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8096u;f->boundary(0x004f8091u,0x00558be0u,f);
    /* 004f8096 */
    goto mi_004f80c6;
mi_004f8098:;
    /* 004f8098 */
    a=(f->r[0]&0xffffu);b=0x000000ebu;t=mi_alu(f,a,b,16,1);
    /* 004f809c */
    if(!(f->flags&MI_ZF))goto mi_004f80b0;
    /* 004f809e */
    a=0x00eadb38u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f80a3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f80a4 */
    f->r[1]=0x006de9e0u;
    /* 004f80a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f80aeu;f->boundary(0x004f80a9u,0x00555e80u,f);
    /* 004f80ae */
    goto mi_004f80c6;
mi_004f80b0:;
    /* 004f80b0 */
    a=(f->r[0]&0xffffu);b=0x000000ecu;t=mi_alu(f,a,b,16,1);
    /* 004f80b4 */
    if(!(f->flags&MI_ZF))goto mi_004f80cc;
    /* 004f80b6 */
    a=0x00eadb38u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f80bb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f80bc */
    f->r[1]=0x006e9188u;
    /* 004f80c1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f80c6u;f->boundary(0x004f80c1u,0x0054f470u,f);
mi_004f80c6:;
    /* 004f80c6 */
    f->r[2]=mi_read((0x00eadd6cu),4u);
mi_004f80cc:;
    /* 004f80cc */
    a=mi_read((0x00b4e6fcu+f->r[7]*2u),2u);b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004f80d4 */
    MI_U32((0x00eadd50u))=(uint32_t)(f->r[5]);
    /* 004f80da */
    MI_U32((0x00eadd48u))=(uint32_t)(f->r[6]);
    /* 004f80e0 */
    if(!(f->flags&MI_ZF))goto mi_004f810d;
    /* 004f80e2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00adc36cu+f->r[7]),1u))&0xffu)<<0);
    /* 004f80e8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f80ea */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004f810d;
    /* 004f80ec */
    f->r[1]=mi_read((0x00af598cu+f->r[7]*4u),4u);
    /* 004f80f3 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004f80f9 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004f80fc */
    a=f->r[1];b=0x00000028u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 004f80ff */
    a=f->r[1];t=a>>2u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>1u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004f8102 */
    MI_U32((0x00eadd4cu))=(uint32_t)(f->r[1]);
    /* 004f8108 */
    goto mi_004f8655;
mi_004f810d:;
    /* 004f810d */
    a=mi_read((0x00b285ccu+f->r[7]),1u);b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 004f8114 */
    f->r[0]=mi_read((0x00af598cu+f->r[7]*4u),4u);
    /* 004f811b */
    if((f->flags&MI_ZF))goto mi_004f8153;
    /* 004f811d */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004f811f */
    if((f->flags&MI_ZF))goto mi_004f812b;
    /* 004f8121 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x0000000du);
mi_004f812b:;
    /* 004f812b */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,3);
    /* 004f812d */
    if((f->flags&MI_ZF))goto mi_004f8139;
    /* 004f812f */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000005u);
mi_004f8139:;
    /* 004f8139 */
    a=f->r[0];b=0x02000000u;t=mi_alu(f,a,b,32,3);
    /* 004f813e */
    if((f->flags&MI_ZF))goto mi_004f8655;
    /* 004f8144 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000016u);
    /* 004f814e */
    goto mi_004f8655;
mi_004f8153:;
    /* 004f8153 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 004f8155 */
    if((f->flags&MI_ZF))goto mi_004f8162;
    /* 004f8157 */
    MI_U32((0x00eadd4cu))=(uint32_t)(f->r[5]);
    /* 004f815d */
    goto mi_004f8655;
mi_004f8162:;
    /* 004f8162 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,3);
    /* 004f8164 */
    if((f->flags&MI_ZF))goto mi_004f81f2;
    /* 004f816a */
    f->r[1]=f->r[2];
    /* 004f816c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f816f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f8171 */
    a=mi_read((0x007999e8u+f->r[1]*8u),2u);b=0x000000e4u;t=mi_alu(f,a,b,16,1);
    /* 004f817b */
    if((f->flags&MI_ZF))goto mi_004f81f2;
    /* 004f817d */
    a=f->r[7];b=mi_read((0x00bc96c4u),4u);t=mi_alu(f,a,b,32,1);
    /* 004f8183 */
    if(!(f->flags&MI_ZF))goto mi_004f8194;
    /* 004f8185 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x0000000eu);
    /* 004f818f */
    goto mi_004f8655;
mi_004f8194:;
    /* 004f8194 */
    a=f->r[7];b=mi_read((0x00bc96c0u),4u);t=mi_alu(f,a,b,32,1);
    /* 004f819a */
    if(!(f->flags&MI_ZF))goto mi_004f81ab;
    /* 004f819c */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x0000000fu);
    /* 004f81a6 */
    goto mi_004f8655;
mi_004f81ab:;
    /* 004f81ab */
    a=f->r[0];b=0x20300002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004f81b0 */
    a=f->r[0];b=0x00100002u;t=mi_alu(f,a,b,32,1);
    /* 004f81b5 */
    if(!(f->flags&MI_ZF))goto mi_004f81c6;
    /* 004f81b7 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000010u);
    /* 004f81c1 */
    goto mi_004f8655;
mi_004f81c6:;
    /* 004f81c6 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004f81c9 */
    if(!(f->flags&MI_ZF))goto mi_004f81da;
    /* 004f81cb */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000011u);
    /* 004f81d5 */
    goto mi_004f8655;
mi_004f81da:;
    /* 004f81da */
    a=f->r[0];b=0x20000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f81df */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004f81e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004f81e3 */
    a=(f->r[0]&0xffu);b=0x000000e8u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004f81e5 */
    a=f->r[0];b=0x0000001au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f81e8 */
    MI_U32((0x00eadd4cu))=(uint32_t)(f->r[0]);
    /* 004f81ed */
    goto mi_004f8655;
mi_004f81f2:;
    /* 004f81f2 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 004f81f4 */
    if((f->flags&MI_ZF))goto mi_004f820c;
    /* 004f81f6 */
    a=f->r[0];b=0x00010008u;t=mi_alu(f,a,b,32,3);
    /* 004f81fb */
    if(!(f->flags&MI_ZF))goto mi_004f820c;
    /* 004f81fd */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000003u);
    /* 004f8207 */
    goto mi_004f8655;
mi_004f820c:;
    /* 004f820c */
    a=(f->r[0]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 004f820e */
    if((f->flags&MI_ZF))goto mi_004f821f;
    /* 004f8210 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000004u);
    /* 004f821a */
    goto mi_004f8655;
mi_004f821f:;
    /* 004f821f */
    a=(f->r[0]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,3);
    /* 004f8221 */
    if((f->flags&MI_ZF))goto mi_004f8232;
    /* 004f8223 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x0000000cu);
    /* 004f822d */
    goto mi_004f8655;
mi_004f8232:;
    /* 004f8232 */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004f8234 */
    if((f->flags&MI_ZF))goto mi_004f8249;
    /* 004f8236 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004f8238 */
    if(!(f->flags&MI_ZF))goto mi_004f8249;
    /* 004f823a */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000006u);
    /* 004f8244 */
    goto mi_004f8655;
mi_004f8249:;
    /* 004f8249 */
    a=((f->r[0]>>8)&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 004f824c */
    if((f->flags&MI_ZF))goto mi_004f825d;
    /* 004f824e */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000007u);
    /* 004f8258 */
    goto mi_004f8655;
mi_004f825d:;
    /* 004f825d */
    a=((f->r[0]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 004f8260 */
    if((f->flags&MI_ZF))goto mi_004f8271;
    /* 004f8262 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000008u);
    /* 004f826c */
    goto mi_004f8655;
mi_004f8271:;
    /* 004f8271 */
    a=f->r[0];b=0x00040000u;t=mi_alu(f,a,b,32,3);
    /* 004f8276 */
    if((f->flags&MI_ZF))goto mi_004f8287;
    /* 004f8278 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000015u);
    /* 004f8282 */
    goto mi_004f8655;
mi_004f8287:;
    /* 004f8287 */
    a=f->r[0];b=0x02000000u;t=mi_alu(f,a,b,32,3);
    /* 004f828c */
    if((f->flags&MI_ZF))goto mi_004f829d;
    /* 004f828e */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000016u);
    /* 004f8298 */
    goto mi_004f8655;
mi_004f829d:;
    /* 004f829d */
    f->r[1]=f->r[0];
    /* 004f829f */
    a=f->r[1];b=0x00c04000u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004f82a5 */
    a=f->r[1];b=0x00004000u;t=mi_alu(f,a,b,32,1);
    /* 004f82ab */
    if(!(f->flags&MI_ZF))goto mi_004f82b8;
    /* 004f82ad */
    MI_U32((0x00eadd4cu))=(uint32_t)(f->r[3]);
    /* 004f82b3 */
    goto mi_004f8655;
mi_004f82b8:;
    /* 004f82b8 */
    a=f->r[1];b=0x00404000u;t=mi_alu(f,a,b,32,1);
    /* 004f82be */
    if(!(f->flags&MI_ZF))goto mi_004f82cf;
    /* 004f82c0 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000017u);
    /* 004f82ca */
    goto mi_004f8655;
mi_004f82cf:;
    /* 004f82cf */
    a=f->r[1];b=0x00804000u;t=mi_alu(f,a,b,32,1);
    /* 004f82d5 */
    if(!(f->flags&MI_ZF))goto mi_004f82e6;
    /* 004f82d7 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000018u);
    /* 004f82e1 */
    goto mi_004f8655;
mi_004f82e6:;
    /* 004f82e6 */
    f->r[1]=f->r[0];
    /* 004f82e8 */
    a=f->r[1];b=0x00010004u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004f82ee */
    a=f->r[1];b=0x00010000u;t=mi_alu(f,a,b,32,1);
    /* 004f82f4 */
    if(!(f->flags&MI_ZF))goto mi_004f8309;
    /* 004f82f6 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004f82f8 */
    if(!(f->flags&MI_ZF))goto mi_004f8309;
    /* 004f82fa */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000013u);
    /* 004f8304 */
    goto mi_004f8655;
mi_004f8309:;
    /* 004f8309 */
    a=((f->r[0]>>8)&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004f830c */
    if((f->flags&MI_ZF))goto mi_004f8321;
    /* 004f830e */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004f8310 */
    if(!(f->flags&MI_ZF))goto mi_004f8321;
    /* 004f8312 */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000019u);
    /* 004f831c */
    goto mi_004f8655;
mi_004f8321:;
    /* 004f8321 */
    a=f->r[0];b=0x00010000u;t=mi_alu(f,a,b,32,3);
    /* 004f8326 */
    if((f->flags&MI_ZF))goto mi_004f833b;
    /* 004f8328 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 004f832a */
    if(!(f->flags&MI_ZF))goto mi_004f833b;
    /* 004f832c */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x00000014u);
    /* 004f8336 */
    goto mi_004f8655;
mi_004f833b:;
    /* 004f833b */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004f833d */
    if((f->flags&MI_ZF))goto mi_004f864b;
    /* 004f8343 */
    f->r[0]=f->r[2];
    /* 004f8345 */
    MI_U32((0x00eadd50u))=(uint32_t)(0x00000002u);
    /* 004f834f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f8352 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f8354 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f8357 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[0]),2u);
    /* 004f835e */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 004f8361 */
    f->r[1]=mi_read((0x00705eb8u+f->r[2]*8u),4u);
    /* 004f8368 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a18u+f->r[0]),2u);
    /* 004f836f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8370 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8371 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8376u;f->boundary(0x004f8371u,0x004cde60u,f);
    /* 004f8376 */
    MI_U32((0x00eadd5cu))=(uint32_t)(f->r[0]);
    /* 004f837b */
    f->r[0]=mi_read((0x00eadd6cu),4u);
    /* 004f8380 */
    f->r[1]=f->r[0];
    /* 004f8382 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f8385 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f8388 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f838a */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f838d */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]),2u))&0xffffu)<<0);
    /* 004f8394 */
    a=(f->r[2]&0xffffu);b=0x00000036u;t=mi_alu(f,a,b,16,1);
    /* 004f8398 */
    if(!(f->flags&MI_ZF))goto mi_004f83e8;
    /* 004f839a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a10u+f->r[1]),2u);
    /* 004f83a1 */
    f->r[1]=f->r[0];
    /* 004f83a3 */
    MI_U32((0x00eadd6cu))=(uint32_t)(f->r[0]);
    /* 004f83a8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f83ab */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f83ad */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f83b0 */
    f->r[2]=mi_read((0x007999e4u+f->r[1]),4u);
    /* 004f83b6 */
    MI_U32((0x00eadd80u))=(uint32_t)(f->r[2]);
    /* 004f83bc */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[1]),2u);
    /* 004f83c3 */
    MI_U32((0x00eadd7cu))=(uint32_t)(f->r[2]);
    /* 004f83c9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[1]),2u);
    /* 004f83d0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a28u+f->r[1]),2u);
    /* 004f83d7 */
    MI_U32((0x00eadd78u))=(uint32_t)(f->r[2]);
    /* 004f83dd */
    MI_U32((0x00eadcccu))=(uint32_t)(f->r[1]);
    /* 004f83e3 */
    goto mi_004f850c;
mi_004f83e8:;
    /* 004f83e8 */
    a=(f->r[2]&0xffffu);b=0x00000039u;t=mi_alu(f,a,b,16,1);
    /* 004f83ec */
    if(!(f->flags&MI_ZF))goto mi_004f8400;
    /* 004f83ee */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a28u+f->r[1]),2u);
    /* 004f83f5 */
    MI_U32((0x00eadcccu))=(uint32_t)(f->r[2]);
    /* 004f83fb */
    goto mi_004f850c;
mi_004f8400:;
    /* 004f8400 */
    a=(f->r[2]&0xffffu);b=0x00000049u;t=mi_alu(f,a,b,16,1);
    /* 004f8404 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f840a */
    a=(f->r[2]&0xffffu);b=0x00000020u;t=mi_alu(f,a,b,16,1);
    /* 004f840e */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f8414 */
    a=(f->r[2]&0xffffu);b=0x000000e2u;t=mi_alu(f,a,b,16,1);
    /* 004f8419 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f841f */
    a=(f->r[2]&0xffffu);b=0x000000d2u;t=mi_alu(f,a,b,16,1);
    /* 004f8424 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f842a */
    a=(f->r[2]&0xffffu);b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 004f842e */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f8434 */
    a=(f->r[2]&0xffffu);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 004f8438 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f843e */
    a=(f->r[2]&0xffffu);b=0x00000021u;t=mi_alu(f,a,b,16,1);
    /* 004f8442 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f8448 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x007999ddu+f->r[1]),1u))&0xffu)<<0);
    /* 004f844e */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004f8450 */
    if((f->flags&MI_ZF))goto mi_004f8470;
    /* 004f8452 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8457u;f->boundary(0x004f8452u,0x0050f0f0u,f);
    /* 004f8457 */
    f->r[2]=mi_read((0x00eadd6cu),4u);
    /* 004f845d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f845e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8463u;f->boundary(0x004f845eu,0x00454190u,f);
    /* 004f8463 */
    f->r[0]=mi_read((0x00eadd6cu),4u);
    /* 004f8468 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f846b */
    goto mi_004f850c;
mi_004f8470:;
    /* 004f8470 */
    a=(f->r[2]&0xffffu);b=0x0000005fu;t=mi_alu(f,a,b,16,1);
    /* 004f8474 */
    if(!(f->flags&MI_ZF))goto mi_004f848d;
    /* 004f8476 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f847bu;f->boundary(0x004f8476u,0x004b7590u,f);
    /* 004f847b */
    a=f->r[0];b=mi_read((0x00bcb9acu),4u);t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f8481 */
    MI_U32((0x00eadcd0u))=(uint32_t)(f->r[0]);
    /* 004f8486 */
    f->r[0]=mi_read((0x00eadd6cu),4u);
    /* 004f848b */
    goto mi_004f850c;
mi_004f848d:;
    /* 004f848d */
    a=(f->r[2]&0xffffu);b=0x00000041u;t=mi_alu(f,a,b,16,1);
    /* 004f8491 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f8493 */
    a=(f->r[2]&0xffffu);b=0x00000042u;t=mi_alu(f,a,b,16,1);
    /* 004f8497 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f8499 */
    a=(f->r[2]&0xffffu);b=0x00000043u;t=mi_alu(f,a,b,16,1);
    /* 004f849d */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f849f */
    a=(f->r[2]&0xffffu);b=0x00000044u;t=mi_alu(f,a,b,16,1);
    /* 004f84a3 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f84a5 */
    a=(f->r[2]&0xffffu);b=0x00000045u;t=mi_alu(f,a,b,16,1);
    /* 004f84a9 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f84ab */
    a=(f->r[2]&0xffffu);b=0x0000009cu;t=mi_alu(f,a,b,16,1);
    /* 004f84b0 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f84b2 */
    a=(f->r[2]&0xffffu);b=0x0000009du;t=mi_alu(f,a,b,16,1);
    /* 004f84b7 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f84b9 */
    a=(f->r[2]&0xffffu);b=0x0000009eu;t=mi_alu(f,a,b,16,1);
    /* 004f84be */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f84c0 */
    a=(f->r[2]&0xffffu);b=0x0000009fu;t=mi_alu(f,a,b,16,1);
    /* 004f84c5 */
    if((f->flags&MI_ZF))goto mi_004f84ce;
    /* 004f84c7 */
    a=(f->r[2]&0xffffu);b=0x000000a0u;t=mi_alu(f,a,b,16,1);
    /* 004f84cc */
    if(!(f->flags&MI_ZF))goto mi_004f850c;
mi_004f84ce:;
    /* 004f84ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f84cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f84d4u;f->boundary(0x004f84cfu,0x00475b50u,f);
    /* 004f84d4 */
    f->r[1]=f->r[0];
    /* 004f84d6 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f84d9 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f84dc */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f84de */
    MI_U32((0x00eadd6cu))=(uint32_t)(f->r[0]);
    /* 004f84e3 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f84e6 */
    f->r[2]=mi_read((0x007999e4u+f->r[1]),4u);
    /* 004f84ec */
    MI_U32((0x00eadd80u))=(uint32_t)(f->r[2]);
    /* 004f84f2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[1]),2u);
    /* 004f84f9 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[1]),2u);
    /* 004f8500 */
    MI_U32((0x00eadd7cu))=(uint32_t)(f->r[2]);
    /* 004f8506 */
    MI_U32((0x00eadd78u))=(uint32_t)(f->r[1]);
mi_004f850c:;
    /* 004f850c */
    f->r[1]=f->r[0];
    /* 004f850e */
    MI_U32((0x00eadd58u))=(uint32_t)(f->r[6]);
    /* 004f8514 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f8517 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f8519 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f851c */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]),2u))&0xffffu)<<0);
    /* 004f8523 */
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 004f8527 */
    if((f->flags&MI_ZF))goto mi_004f862e;
    /* 004f852d */
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 004f8531 */
    if((f->flags&MI_ZF))goto mi_004f862e;
    /* 004f8537 */
    a=(f->r[0]&0xffffu);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 004f853b */
    if((f->flags&MI_ZF))goto mi_004f85d4;
    /* 004f8541 */
    a=(f->r[0]&0xffffu);b=0x000000c1u;t=mi_alu(f,a,b,16,1);
    /* 004f8545 */
    if((f->flags&MI_ZF))goto mi_004f85d4;
    /* 004f854b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004f854e */
    a=f->r[0];b=0x0000008cu;t=mi_alu(f,a,b,32,1);
    /* 004f8553 */
    if(mi_signed_less(f->flags))goto mi_004f8576;
    /* 004f8555 */
    a=f->r[0];b=0x00000095u;t=mi_alu(f,a,b,32,1);
    /* 004f855a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004f8576;
    /* 004f855c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[1]),2u);
    /* 004f8563 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[1]),2u);
    /* 004f856a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f856b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f856c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8571u;f->boundary(0x004f856cu,0x004802c0u,f);
    /* 004f8571 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f8574 */
    goto mi_004f85c1;
mi_004f8576:;
    /* 004f8576 */
    a=f->r[0];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 004f8579 */
    if(mi_signed_less(f->flags))goto mi_004f85a0;
    /* 004f857b */
    a=f->r[0];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 004f857e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004f85a0;
    /* 004f8580 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[1]),2u);
    /* 004f8587 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f8589 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[1]),1u))&0xffu)<<0);
    /* 004f858f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[1]),2u);
    /* 004f8596 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8597 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8598 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f8599 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f859eu;f->boundary(0x004f8599u,0x00480460u,f);
    /* 004f859e */
    goto mi_004f85be;
mi_004f85a0:;
    /* 004f85a0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[1]),2u);
    /* 004f85a7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f85a9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[1]),1u))&0xffu)<<0);
    /* 004f85af */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[1]),2u);
    /* 004f85b6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f85b7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f85b8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f85b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f85beu;f->boundary(0x004f85b9u,0x00480180u,f);
mi_004f85be:;
    /* 004f85be */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004f85c1:;
    /* 004f85c1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f85c3 */
    if((f->flags&MI_ZF))goto mi_004f8655;
    /* 004f85c9 */
    MI_U32((0x00eadd58u))=(uint32_t)(f->r[5]);
    /* 004f85cf */
    goto mi_004f8655;
mi_004f85d4:;
    /* 004f85d4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[1]),2u);
    /* 004f85db */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[1]),2u);
    /* 004f85e2 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f85e4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f85e5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f85e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f85ebu;f->boundary(0x004f85e6u,0x00480180u,f);
    /* 004f85eb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f85ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f85f0 */
    if((f->flags&MI_ZF))goto mi_004f85f8;
    /* 004f85f2 */
    MI_U32((0x00eadd58u))=(uint32_t)(f->r[5]);
mi_004f85f8:;
    /* 004f85f8 */
    f->r[1]=mi_read((0x00eadd6cu),4u);
    /* 004f85fe */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f85ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8604u;f->boundary(0x004f85ffu,0x0045d570u,f);
    /* 004f8604 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f8607 */
    MI_U32((0x00eadcb8u))=(uint32_t)(f->r[0]);
    /* 004f860c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8611u;f->boundary(0x004f860cu,0x00502780u,f);
    /* 004f8611 */
    a=f->r[0];b=mi_read((0x00f53e6cu),4u);t=mi_alu(f,a,b,32,1);
    /* 004f8617 */
    if(mi_signed_less(f->flags))goto mi_004f8655;
    /* 004f8619 */
    f->r[0]=mi_read((0x00eadd6cu),4u);
    /* 004f861e */
    f->r[2]=f->r[0];
    /* 004f8620 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f8623 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f8625 */
    MI_U32((0x00799ab8u+f->r[2]*8u))=(uint32_t)(f->r[5]);
    /* 004f862c */
    goto mi_004f8655;
mi_004f862e:;
    /* 004f862e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[1]),2u);
    /* 004f8635 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[1]),2u);
    /* 004f863c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f863d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f863e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8643u;f->boundary(0x004f863eu,0x004806a0u,f);
    /* 004f8643 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004f8646 */
    goto mi_004f85c1;
mi_004f864b:;
    /* 004f864b */
    MI_U32((0x00eadd4cu))=(uint32_t)(0x0000000au);
mi_004f8655:;
    /* 004f8655 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004f8657 */
    f->r[1]=0x00000007u;
    /* 004f865c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004f865e */
    f->r[7]=0x00eadd24u;
    /* 004f8663 */
    MI_U32((0x00eadce4u))=(uint32_t)(f->r[6]);
    /* 004f8669 */
    MI_U32((0x00eadd40u))=(uint32_t)(f->r[2]);
    /* 004f866f */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 004f8671 */
    f->r[5]=0x005f76c4u;
mi_004f8676:;
    /* 004f8676 */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 004f867a */
    f->r[7]=mi_read((0x00000000u+f->r[5]),4u);
    /* 004f867d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f867f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b350dcu+f->r[0]*2u),2u);
    /* 004f8687 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004f8689 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f8727;
mi_004f868f:;
    /* 004f868f */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004f8692 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004f8695 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004f8697 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004f869a */
    a=mi_read((0x00bcd606u+f->r[6]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004f86a1 */
    if((f->flags&MI_ZF))goto mi_004f8711;
    /* 004f86a3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 004f86a9 */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004f86ac */
    if((f->flags&MI_ZF))goto mi_004f8711;
    /* 004f86ae */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004f86b0 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[6]),1u))&0xffu)<<0);
    /* 004f86b6 */
    f->r[7]=f->r[3];
    /* 004f86b8 */
    a=f->r[7];b=0x0000004eu;t=mi_alu(f,a,b,32,1);
    /* 004f86bb */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004f86f4;
    /* 004f86bd */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004f86bf */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x004f8910u+f->r[7]),1u))&0xffu)<<0);
    /* 004f86c5 */
    switch(mi_read((0x004f8900u+f->r[3]*4u),4u)){case 0x004f86ccu:goto mi_004f86cc;case 0x004f86ddu:goto mi_004f86dd;case 0x004f86f4u:goto mi_004f86f4;case 0x004f8711u:goto mi_004f8711;default:abort();}
mi_004f86cc:;
    /* 004f86cc */
    a=mi_read((0x00bcd72au+f->r[6]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004f86d4 */
    if(!mi_signed_less(f->flags))goto mi_004f8711;
    /* 004f86d6 */
    a=(f->r[1]&0xffu);b=0x00000011u;t=mi_alu(f,a,b,8,1);
    /* 004f86d9 */
    if((f->flags&MI_ZF))goto mi_004f8711;
    /* 004f86db */
    goto mi_004f86f4;
mi_004f86dd:;
    /* 004f86dd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd658u+f->r[6]),2u);
    /* 004f86e4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004f86e7 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004f86ea */
    a=mi_read((0x00bcba4au+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004f86f2 */
    if(!mi_signed_less(f->flags))goto mi_004f8711;
mi_004f86f4:;
    /* 004f86f4 */
    MI_U32((0x00eadd24u+f->r[2]*4u))=(uint32_t)(f->r[0]);
    /* 004f86fb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004f86fc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f86fd */
    MI_U32((0x00eadd40u))=(uint32_t)(f->r[2]);
    /* 004f8703 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f8708u;f->boundary(0x004f8703u,0x0053c100u,f);
    /* 004f8708 */
    f->r[2]=mi_read((0x00eadd40u),4u);
    /* 004f870e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004f8711:;
    /* 004f8711 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd600u+f->r[6]),2u);
    /* 004f8718 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004f871b */
    if(!mi_signed_less(f->flags))goto mi_004f873b;
    /* 004f871d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004f871f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004f868f;
    /* 004f8725 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004f8727:;
    /* 004f8727 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004f872a */
    if(!mi_signed_less(f->flags))goto mi_004f873b;
    /* 004f872c */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004f872f */
    a=f->r[5];b=0x005f76e8u;t=mi_alu(f,a,b,32,1);
    /* 004f8735 */
    if(mi_signed_less(f->flags))goto mi_004f8676;
mi_004f873b:;
    /* 004f873b */
    f->r[2]=0x00eadd24u;
mi_004f8740:;
    /* 004f8740 */
    f->r[1]=mi_read((0x00000000u+f->r[2]),4u);
    /* 004f8742 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004f8744 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f876d;
    /* 004f8746 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004f8749 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f874c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f874e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[0]*4u),1u))&0xffu)<<0);
    /* 004f8755 */
    a=(f->r[0]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 004f8757 */
    if((f->flags&MI_ZF))goto mi_004f879e;
    /* 004f8759 */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004f875b */
    if((f->flags&MI_ZF))goto mi_004f879e;
    /* 004f875d */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004f875f */
    if((f->flags&MI_ZF))goto mi_004f879e;
    /* 004f8761 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004f8763 */
    if((f->flags&MI_ZF))goto mi_004f879e;
    /* 004f8765 */
    a=(f->r[0]&0xffu);b=0x0000004du;t=mi_alu(f,a,b,8,1);
    /* 004f8767 */
    if((f->flags&MI_ZF))goto mi_004f877a;
    /* 004f8769 */
    a=(f->r[0]&0xffu);b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004f876b */
    if((f->flags&MI_ZF))goto mi_004f878c;
mi_004f876d:;
    /* 004f876d */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f8770 */
    a=f->r[2];b=0x00eadd40u;t=mi_alu(f,a,b,32,1);
    /* 004f8776 */
    if(mi_signed_less(f->flags))goto mi_004f8740;
    /* 004f8778 */
    goto mi_004f87bd;
mi_004f877a:;
    /* 004f877a */
    MI_U32((0x00eadd50u))=(uint32_t)(0x00000006u);
    /* 004f8784 */
    MI_U32((0x00eadcc8u))=(uint32_t)(f->r[1]);
    /* 004f878a */
    goto mi_004f87bd;
mi_004f878c:;
    /* 004f878c */
    MI_U32((0x00eadd50u))=(uint32_t)(0x00000005u);
    /* 004f8796 */
    MI_U32((0x00eadcc4u))=(uint32_t)(f->r[1]);
    /* 004f879c */
    goto mi_004f87bd;
mi_004f879e:;
    /* 004f879e */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004f87a1 */
    MI_U32((0x00eadd50u))=(uint32_t)(0x00000004u);
    /* 004f87ab */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f87ae */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f87b0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd658u+f->r[2]*4u),2u);
    /* 004f87b8 */
    MI_U32((0x00eadcccu))=(uint32_t)(f->r[0]);
mi_004f87bd:;
    /* 004f87bd */
    f->r[6]=0x00000020u;
    /* 004f87c2 */
    MI_U32((0x00eadd90u))=(uint32_t)(0x00000008u);
    /* 004f87cc */
    MI_U32((0x00eadd8cu))=(uint32_t)(f->r[6]);
    /* 004f87d2 */
    MI_U32((0x00eadd88u))=(uint32_t)(0x0000001du);
    /* 004f87dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f87e1u;f->boundary(0x004f87dcu,0x004f8960u,f);
    /* 004f87e1 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004f87e4 */
    if(!(f->flags&MI_ZF))goto mi_004f87ed;
    /* 004f87e6 */
    f->r[0]=0x00000010u;
    /* 004f87eb */
    goto mi_004f881d;
mi_004f87ed:;
    /* 004f87ed */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004f87f0 */
    if(!(f->flags&MI_ZF))goto mi_004f87f9;
    /* 004f87f2 */
    f->r[0]=0x00000012u;
    /* 004f87f7 */
    goto mi_004f881d;
mi_004f87f9:;
    /* 004f87f9 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004f87fc */
    if(!(f->flags&MI_ZF))goto mi_004f8805;
    /* 004f87fe */
    f->r[0]=0x00000013u;
    /* 004f8803 */
    goto mi_004f881d;
mi_004f8805:;
    /* 004f8805 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004f8808 */
    if(!(f->flags&MI_ZF))goto mi_004f8811;
    /* 004f880a */
    f->r[0]=0x0000000eu;
    /* 004f880f */
    goto mi_004f881d;
mi_004f8811:;
    /* 004f8811 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004f8814 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004f8816 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004f8818 */
    a=(f->r[0]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004f881a */
    a=f->r[0];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004f881d:;
    /* 004f881d */
    f->r[1]=mi_read((0x00e38dacu),4u);
    /* 004f8823 */
    MI_U32((0x00eadd84u))=(uint32_t)(f->r[0]);
    /* 004f8828 */
    a=f->r[1];b=0x000001e0u;t=mi_alu(f,a,b,32,1);
    /* 004f882e */
    if(!(f->flags&MI_ZF))goto mi_004f8852;
    /* 004f8830 */
    f->r[1]=mi_read((0x00f53f78u),4u);
    /* 004f8836 */
    MI_U32((0x00eadd8cu))=(uint32_t)(f->r[6]);
    /* 004f883c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004f883d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[1]=t;
    /* 004f883f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 004f8841 */
    a=f->r[1];b=0xffffffc4u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004f8844 */
    a=f->r[1];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004f8847 */
    MI_U32((0x00eadd90u))=(uint32_t)(f->r[1]);
    /* 004f884d */
    goto mi_004f88dc;
mi_004f8852:;
    /* 004f8852 */
    a=f->r[1];b=0x00000258u;t=mi_alu(f,a,b,32,1);
    /* 004f8858 */
    if(!(f->flags&MI_ZF))goto mi_004f8896;
    /* 004f885a */
    a=mi_read((0x00e38e30u),4u);b=0x00000138u;t=mi_alu(f,a,b,32,1);
    /* 004f8864 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f886e;
    /* 004f8866 */
    MI_U32((0x00eadd8cu))=(uint32_t)(f->r[6]);
    /* 004f886c */
    goto mi_004f887e;
mi_004f886e:;
    /* 004f886e */
    f->r[2]=0x00000025u;
    /* 004f8873 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004f8875 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004f8878 */
    MI_U32((0x00eadd8cu))=(uint32_t)(f->r[2]);
mi_004f887e:;
    /* 004f887e */
    f->r[0]=mi_read((0x00f53f78u),4u);
    /* 004f8883 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004f8884 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004f8886 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004f8888 */
    a=(f->r[0]&0xffu);b=0x000000c4u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004f888a */
    a=f->r[0];b=0x00000080u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004f888f */
    MI_U32((0x00eadd90u))=(uint32_t)(f->r[0]);
    /* 004f8894 */
    goto mi_004f88dc;
mi_004f8896:;
    /* 004f8896 */
    a=f->r[1];b=0x00000300u;t=mi_alu(f,a,b,32,1);
    /* 004f889c */
    if(!(f->flags&MI_ZF))goto mi_004f88dc;
    /* 004f889e */
    a=mi_read((0x00e38e30u),4u);b=0x0000018cu;t=mi_alu(f,a,b,32,1);
    /* 004f88a8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004f88b2;
    /* 004f88aa */
    MI_U32((0x00eadd8cu))=(uint32_t)(f->r[6]);
    /* 004f88b0 */
    goto mi_004f88c2;
mi_004f88b2:;
    /* 004f88b2 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004f88b5 */
    f->r[1]=0x000002f8u;
    /* 004f88ba */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004f88bc */
    MI_U32((0x00eadd8cu))=(uint32_t)(f->r[1]);
mi_004f88c2:;
    /* 004f88c2 */
    f->r[2]=mi_read((0x00f53f78u),4u);
    /* 004f88c8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004f88c9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[2]=t;
    /* 004f88cb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 004f88cd */
    a=f->r[2];b=0xffffffc4u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004f88d0 */
    a=f->r[2];b=0x000000f8u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004f88d6 */
    MI_U32((0x00eadd90u))=(uint32_t)(f->r[2]);
mi_004f88dc:;
    /* 004f88dc */
    a=mi_read((0x00eadd50u),4u);b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004f88e3 */
    if(!(f->flags&MI_ZF))goto mi_004f88f2;
    /* 004f88e5 */
    a=0x00eadce8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004f88ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004f88efu;f->boundary(0x004f88eau,0x00515130u,f);
    /* 004f88ef */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004f88f2:;
    /* 004f88f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004f88f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004f88f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004f88f5 */
    f->r[0]=0x00000001u;
    /* 004f88fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004f88fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004f88fc */
    f->r[4]+=4;return;
}
