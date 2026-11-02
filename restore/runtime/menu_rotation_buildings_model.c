/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_rotation_actions.h"
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

static const uint8_t mi_rom_0046e124[]={0x9e,0xe0,0x46,0x00,0x86,0xe0,0x46,0x00,0xf6,0xdf,0x46,0x00,0x23,0xe0,0x46,0x00,0x69,0xe0,0x46,0x00,0xdd,0xe0,0x46,0x00,0xa9,0xe0,0x46,0x00,0xc3,0xe0,0x46,0x00,0xd0,0xe0,0x46,0x00,0xe8,0xe0,0x46,0x00,0xb6,0xe0,0x46,0x00,0xf0,0xe0,0x46,0x00,0xf8,0xe0,0x46,0x00,0x00,0xe1,0x46,0x00,0x0b,0xe1,0x46,0x00};
static const uint8_t mi_rom_0046e160[]={0x00,0x0e,0x01,0x0e,0x0e,0x02,0x03,0x04,0x0e,0x05,0x0e,0x0e,0x06,0x0e,0x07,0x08,0x09,0x0a,0x0e,0x0e,0x0e,0x0b,0x0c,0x0d};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0046e124u&&width<=60u&&(offset=address-0x0046e124u)<=60u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0046e124[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0046e160u&&width<=24u&&(offset=address-0x0046e160u)<=24u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0046e160[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_rotation_C_0046d840(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046d840 */
    f->r[0]=mi_read((0x0078d790u),4u);
    /* 0046d845 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d846 */
    f->r[7]=0x00000001u;
    /* 0046d84b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046d84d */
    if(mi_signed_less(f->flags))goto mi_0046e11c;
    /* 0046d853 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d854 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d855 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0046d856:;
    /* 0046d856 */
    f->r[6]=f->r[7];
    /* 0046d858 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046d85b */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0046d85d */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0046d860 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999d8u+f->r[6]),1u))&0xffu)<<0);
    /* 0046d866 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046d868 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046d86e */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[6]),2u))&0xffffu)<<0);
    /* 0046d875 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0046d878 */
    a=f->r[5];b=0x0000008cu;t=mi_alu(f,a,b,32,1);
    /* 0046d87e */
    if(mi_signed_less(f->flags))goto mi_0046d8c5;
    /* 0046d880 */
    a=f->r[5];b=0x00000095u;t=mi_alu(f,a,b,32,1);
    /* 0046d886 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046d8c5;
    /* 0046d888 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 0046d88c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046d88e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0046d894 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d896 */
    f->r[2]=mi_read((0x005d06e4u+f->r[1]*4u),4u);
    /* 0046d89d */
    f->r[1]=mi_read((0x00f55bb4u+f->r[2]*8u),4u);
    /* 0046d8a4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d8a5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d8a6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0046d8a7:;
    /* 0046d8a7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046d8ae */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046d8b5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d8b6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d8b7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d8b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d8bdu;f->boundary(0x0046d8b8u,0x0047bf50u,f);
    /* 0046d8bd */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d8c0 */
    goto mi_0046e10b;
mi_0046d8c5:;
    /* 0046d8c5 */
    a=f->r[5];b=0x00000029u;t=mi_alu(f,a,b,32,1);
    /* 0046d8c8 */
    if(mi_signed_less(f->flags))goto mi_0046d96c;
    /* 0046d8ce */
    a=f->r[5];b=0x0000002bu;t=mi_alu(f,a,b,32,1);
    /* 0046d8d1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046d96c;
    /* 0046d8d7 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046d8dc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046d8de */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046d8e4 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046d8e5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046d8e7 */
    f->r[3]=f->r[1];
    /* 0046d8e9 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0046d8ec */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046d8ee */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046d8f0 */
    a=f->r[3];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[3]=t;
    /* 0046d8f3 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0046d8f4 */
    if(!(f->flags&MI_SF))goto mi_0046d8f9;
    /* 0046d8f6 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_0046d8f9:;
    /* 0046d8f9 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046d8fc */
    if(!mi_signed_less(f->flags))goto mi_0046d915;
    /* 0046d8fe */
    f->r[2]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 0046d902 */
    f->r[0]=mi_read((0x005d06e4u+f->r[2]*4u),4u);
    /* 0046d909 */
    f->r[2]=mi_read((0x00f55bb4u+f->r[0]*8u),4u);
    /* 0046d910 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*4u);
    /* 0046d913 */
    goto mi_0046d93f;
mi_0046d915:;
    /* 0046d915 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0046d91b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0046d91d */
    if(!(f->flags&MI_ZF))goto mi_0046d926;
    /* 0046d91f */
    f->r[0]=mi_read((0x00f5730cu),4u);
    /* 0046d924 */
    goto mi_0046d93b;
mi_0046d926:;
    /* 0046d926 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0046d928 */
    if(!(f->flags&MI_ZF))goto mi_0046d936;
    /* 0046d92a */
    f->r[2]=mi_read((0x00f57304u),4u);
    /* 0046d930 */
    f->r[0]=(0xfffffff8u+f->r[2]+f->r[3]*4u);
    /* 0046d934 */
    goto mi_0046d93f;
mi_0046d936:;
    /* 0046d936 */
    f->r[0]=mi_read((0x00f572fcu),4u);
mi_0046d93b:;
    /* 0046d93b */
    f->r[0]=(0xfffffff8u+f->r[0]+f->r[3]*4u);
mi_0046d93f:;
    /* 0046d93f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046d941 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046d943 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0046d949 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d94b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046d952 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d953 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d954 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d955 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046d95c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d95d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d95e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d95f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d964u;f->boundary(0x0046d95fu,0x0047bf50u,f);
    /* 0046d964 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d967 */
    goto mi_0046e10b;
mi_0046d96c:;
    /* 0046d96c */
    a=(f->r[0]&0xffffu);b=0x0000003au;t=mi_alu(f,a,b,16,1);
    /* 0046d970 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046d976 */
    a=(f->r[0]&0xffffu);b=0x000000c8u;t=mi_alu(f,a,b,16,1);
    /* 0046d97a */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046d980 */
    a=(f->r[0]&0xffffu);b=0x000000aau;t=mi_alu(f,a,b,16,1);
    /* 0046d984 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046d98a */
    a=(f->r[0]&0xffffu);b=0x000000c9u;t=mi_alu(f,a,b,16,1);
    /* 0046d98e */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046d994 */
    a=(f->r[0]&0xffffu);b=0x000000abu;t=mi_alu(f,a,b,16,1);
    /* 0046d998 */
    if(!(f->flags&MI_ZF))goto mi_0046d9a8;
    /* 0046d99a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d99b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d9a0u;f->boundary(0x0046d99bu,0x004bfb30u,f);
    /* 0046d9a0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d9a3 */
    goto mi_0046e10b;
mi_0046d9a8:;
    /* 0046d9a8 */
    a=(f->r[0]&0xffffu);b=0x000000cau;t=mi_alu(f,a,b,16,1);
    /* 0046d9ac */
    if(!(f->flags&MI_ZF))goto mi_0046d9bc;
    /* 0046d9ae */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046d9af */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046d9b4u;f->boundary(0x0046d9afu,0x004bfb30u,f);
    /* 0046d9b4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046d9b7 */
    goto mi_0046e10b;
mi_0046d9bc:;
    /* 0046d9bc */
    a=(f->r[0]&0xffffu);b=0x00000038u;t=mi_alu(f,a,b,16,1);
    /* 0046d9c0 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046d9c6 */
    a=(f->r[0]&0xffffu);b=0x000000d2u;t=mi_alu(f,a,b,16,1);
    /* 0046d9ca */
    if(!(f->flags&MI_ZF))goto mi_0046db48;
    /* 0046d9d0 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046d9d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046d9d7 */
    if(!(f->flags&MI_ZF))goto mi_0046da3d;
    /* 0046d9d9 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[6]),2u))&0xffffu)<<0);
    /* 0046d9e0 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0046d9e3 */
    if(!(f->flags&MI_ZF))goto mi_0046d9f0;
    /* 0046d9e5 */
    f->r[0]=mi_read((0x00f56d54u),4u);
    /* 0046d9ea */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046d9eb */
    goto mi_0046db1d;
mi_0046d9f0:;
    /* 0046d9f0 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0046d9f4 */
    if(!(f->flags&MI_ZF))goto mi_0046da04;
    /* 0046d9f6 */
    f->r[1]=mi_read((0x00f56d5cu),4u);
    /* 0046d9fc */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 0046d9ff */
    goto mi_0046db1d;
mi_0046da04:;
    /* 0046da04 */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0046da08 */
    if(!(f->flags&MI_ZF))goto mi_0046da18;
mi_0046da0a:;
    /* 0046da0a */
    f->r[2]=mi_read((0x00f56d64u),4u);
    /* 0046da10 */
    f->r[0]=(0x00000001u+f->r[2]);
    /* 0046da13 */
    goto mi_0046db1d;
mi_0046da18:;
    /* 0046da18 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0046da1c */
    if(!(f->flags&MI_ZF))goto mi_0046da29;
    /* 0046da1e */
    f->r[0]=mi_read((0x00f56d54u),4u);
    /* 0046da23 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046da24 */
    goto mi_0046db1d;
mi_0046da29:;
    /* 0046da29 */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0046da2d */
    if(!(f->flags&MI_ZF))goto mi_0046da0a;
    /* 0046da2f */
    f->r[1]=mi_read((0x00f56d5cu),4u);
    /* 0046da35 */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 0046da38 */
    goto mi_0046db1d;
mi_0046da3d:;
    /* 0046da3d */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0046da40 */
    if(!(f->flags&MI_ZF))goto mi_0046daa3;
    /* 0046da42 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[6]),2u))&0xffffu)<<0);
    /* 0046da49 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0046da4c */
    if(!(f->flags&MI_ZF))goto mi_0046da59;
    /* 0046da4e */
    f->r[0]=mi_read((0x00f56d64u),4u);
    /* 0046da53 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046da54 */
    goto mi_0046db1d;
mi_0046da59:;
    /* 0046da59 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0046da5d */
    if(!(f->flags&MI_ZF))goto mi_0046da6d;
    /* 0046da5f */
    f->r[1]=mi_read((0x00f56d5cu),4u);
    /* 0046da65 */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 0046da68 */
    goto mi_0046db1d;
mi_0046da6d:;
    /* 0046da6d */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0046da71 */
    if(!(f->flags&MI_ZF))goto mi_0046da81;
mi_0046da73:;
    /* 0046da73 */
    f->r[2]=mi_read((0x00f56d54u),4u);
    /* 0046da79 */
    f->r[0]=(0x00000001u+f->r[2]);
    /* 0046da7c */
    goto mi_0046db1d;
mi_0046da81:;
    /* 0046da81 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0046da85 */
    if(!(f->flags&MI_ZF))goto mi_0046da92;
    /* 0046da87 */
    f->r[0]=mi_read((0x00f56d64u),4u);
    /* 0046da8c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0046da8d */
    goto mi_0046db1d;
mi_0046da92:;
    /* 0046da92 */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0046da96 */
    if(!(f->flags&MI_ZF))goto mi_0046da73;
    /* 0046da98 */
    f->r[1]=mi_read((0x00f56d5cu),4u);
    /* 0046da9e */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 0046daa1 */
    goto mi_0046db1d;
mi_0046daa3:;
    /* 0046daa3 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0046daa6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999eau+f->r[6]),2u))&0xffffu)<<0);
    /* 0046daad */
    if(!(f->flags&MI_ZF))goto mi_0046dae1;
    /* 0046daaf */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0046dab2 */
    if((f->flags&MI_ZF))goto mi_0046db18;
    /* 0046dab4 */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0046dab8 */
    if(!(f->flags&MI_ZF))goto mi_0046dac1;
    /* 0046daba */
    f->r[0]=mi_read((0x00f56d5cu),4u);
    /* 0046dabf */
    goto mi_0046db1d;
mi_0046dac1:;
    /* 0046dac1 */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0046dac5 */
    if(!(f->flags&MI_ZF))goto mi_0046dace;
mi_0046dac7:;
    /* 0046dac7 */
    f->r[0]=mi_read((0x00f56d64u),4u);
    /* 0046dacc */
    goto mi_0046db1d;
mi_0046dace:;
    /* 0046dace */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0046dad2 */
    if((f->flags&MI_ZF))goto mi_0046db18;
    /* 0046dad4 */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0046dad8 */
    if(!(f->flags&MI_ZF))goto mi_0046dac7;
    /* 0046dada */
    f->r[0]=mi_read((0x00f56d5cu),4u);
    /* 0046dadf */
    goto mi_0046db1d;
mi_0046dae1:;
    /* 0046dae1 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0046dae4 */
    if(!(f->flags&MI_ZF))goto mi_0046daed;
    /* 0046dae6 */
    f->r[0]=mi_read((0x00f56d64u),4u);
    /* 0046daeb */
    goto mi_0046db1d;
mi_0046daed:;
    /* 0046daed */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 0046daf1 */
    if(!(f->flags&MI_ZF))goto mi_0046dafa;
    /* 0046daf3 */
    f->r[0]=mi_read((0x00f56d5cu),4u);
    /* 0046daf8 */
    goto mi_0046db1d;
mi_0046dafa:;
    /* 0046dafa */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0046dafe */
    if((f->flags&MI_ZF))goto mi_0046db18;
    /* 0046db00 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0046db04 */
    if(!(f->flags&MI_ZF))goto mi_0046db0d;
    /* 0046db06 */
    f->r[0]=mi_read((0x00f56d64u),4u);
    /* 0046db0b */
    goto mi_0046db1d;
mi_0046db0d:;
    /* 0046db0d */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0046db11 */
    f->r[0]=mi_read((0x00f56d5cu),4u);
    /* 0046db16 */
    if((f->flags&MI_ZF))goto mi_0046db1d;
mi_0046db18:;
    /* 0046db18 */
    f->r[0]=mi_read((0x00f56d54u),4u);
mi_0046db1d:;
    /* 0046db1d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046db1f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db21 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0046db27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db28 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db29 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0046db2a:;
    /* 0046db2a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046db31 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046db38 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db39 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db3a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db3b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046db40u;f->boundary(0x0046db3bu,0x0047bf50u,f);
    /* 0046db40 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046db43 */
    goto mi_0046e10b;
mi_0046db48:;
    /* 0046db48 */
    a=(f->r[0]&0xffffu);b=0x0000004au;t=mi_alu(f,a,b,16,1);
    /* 0046db4c */
    if(!(f->flags&MI_ZF))goto mi_0046dbd5;
    /* 0046db52 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046db57 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046db59 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046db5f */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046db60 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046db62 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046db64 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0046db66 */
    if(!(f->flags&MI_SF))goto mi_0046db6b;
    /* 0046db68 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0046db6b:;
    /* 0046db6b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046db72 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046db79 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db7a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046db80u;f->boundary(0x0046db7bu,0x0047a1c0u,f);
    /* 0046db80 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046db83 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046db85 */
    if((f->flags&MI_ZF))goto mi_0046dbae;
    /* 0046db87 */
    f->r[1]=mi_read((0x00f5724cu),4u);
    /* 0046db8d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046db94 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046db9b */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046db9d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db9e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046db9f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dba0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dba1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dba6u;f->boundary(0x0046dba1u,0x0047d1f0u,f);
    /* 0046dba6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dba9 */
    goto mi_0046e10b;
mi_0046dbae:;
    /* 0046dbae */
    f->r[1]=mi_read((0x00f56c0cu),4u);
    /* 0046dbb4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dbbb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dbc2 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046dbc4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dbc5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dbc6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dbc7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dbc8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dbcdu;f->boundary(0x0046dbc8u,0x0047d1f0u,f);
    /* 0046dbcd */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dbd0 */
    goto mi_0046e10b;
mi_0046dbd5:;
    /* 0046dbd5 */
    a=(f->r[0]&0xffffu);b=0x0000004cu;t=mi_alu(f,a,b,16,1);
    /* 0046dbd9 */
    if(!(f->flags&MI_ZF))goto mi_0046dc60;
    /* 0046dbdf */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046dbe4 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046dbe6 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046dbec */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046dbed */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046dbef */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046dbf1 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0046dbf3 */
    if(!(f->flags&MI_SF))goto mi_0046dbf8;
    /* 0046dbf5 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0046dbf8:;
    /* 0046dbf8 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dbff */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dc06 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc07 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc08 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dc0du;f->boundary(0x0046dc08u,0x0047a1c0u,f);
    /* 0046dc0d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dc10 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046dc12 */
    if((f->flags&MI_ZF))goto mi_0046dc3a;
    /* 0046dc14 */
    f->r[0]=mi_read((0x00f5726cu),4u);
    /* 0046dc19 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dc20 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dc27 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046dc29 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc2a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc2b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc2c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc2d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dc32u;f->boundary(0x0046dc2du,0x0047cf00u,f);
    /* 0046dc32 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dc35 */
    goto mi_0046e10b;
mi_0046dc3a:;
    /* 0046dc3a */
    f->r[0]=mi_read((0x00f56bccu),4u);
    /* 0046dc3f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dc46 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dc4d */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046dc4f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc50 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc51 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc52 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dc58u;f->boundary(0x0046dc53u,0x0047cf00u,f);
    /* 0046dc58 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dc5b */
    goto mi_0046e10b;
mi_0046dc60:;
    /* 0046dc60 */
    a=(f->r[0]&0xffffu);b=0x000000b5u;t=mi_alu(f,a,b,16,1);
    /* 0046dc64 */
    if(!(f->flags&MI_ZF))goto mi_0046dced;
    /* 0046dc6a */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046dc6f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046dc71 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046dc77 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046dc78 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046dc7a */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046dc7c */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0046dc7e */
    if(!(f->flags&MI_SF))goto mi_0046dc83;
    /* 0046dc80 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0046dc83:;
    /* 0046dc83 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dc8a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dc91 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc92 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dc93 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dc98u;f->boundary(0x0046dc93u,0x0047a1c0u,f);
    /* 0046dc98 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dc9b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046dc9d */
    if((f->flags&MI_ZF))goto mi_0046dcc6;
    /* 0046dc9f */
    f->r[2]=mi_read((0x00f5725cu),4u);
    /* 0046dca5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dcac */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dcb3 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046dcb5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dcb6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dcb7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dcb8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dcb9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dcbeu;f->boundary(0x0046dcb9u,0x0047cf00u,f);
    /* 0046dcbe */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dcc1 */
    goto mi_0046e10b;
mi_0046dcc6:;
    /* 0046dcc6 */
    f->r[2]=mi_read((0x00f56bc4u),4u);
    /* 0046dccc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dcd3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dcda */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046dcdc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dcdd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dcde */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dcdf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dce0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dce5u;f->boundary(0x0046dce0u,0x0047cf00u,f);
    /* 0046dce5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dce8 */
    goto mi_0046e10b;
mi_0046dced:;
    /* 0046dced */
    a=(f->r[0]&0xffffu);b=0x000000b6u;t=mi_alu(f,a,b,16,1);
    /* 0046dcf1 */
    if(!(f->flags&MI_ZF))goto mi_0046dd7a;
    /* 0046dcf7 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046dcfc */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046dcfe */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046dd04 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046dd05 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046dd07 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046dd09 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0046dd0b */
    if(!(f->flags&MI_SF))goto mi_0046dd10;
    /* 0046dd0d */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0046dd10:;
    /* 0046dd10 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dd17 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dd1e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd1f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd20 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dd25u;f->boundary(0x0046dd20u,0x0047a1c0u,f);
    /* 0046dd25 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dd28 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046dd2a */
    if((f->flags&MI_ZF))goto mi_0046dd53;
    /* 0046dd2c */
    f->r[1]=mi_read((0x00f57254u),4u);
    /* 0046dd32 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046dd34 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0046dd35:;
    /* 0046dd35 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dd3c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dd43 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd44 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd45 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd46 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dd4bu;f->boundary(0x0046dd46u,0x0047d1f0u,f);
    /* 0046dd4b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dd4e */
    goto mi_0046e10b;
mi_0046dd53:;
    /* 0046dd53 */
    f->r[1]=mi_read((0x00f56c1cu),4u);
    /* 0046dd59 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046dd60 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046dd67 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046dd69 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd6a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd6b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd6c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd6d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dd72u;f->boundary(0x0046dd6du,0x0047d1f0u,f);
    /* 0046dd72 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dd75 */
    goto mi_0046e10b;
mi_0046dd7a:;
    /* 0046dd7a */
    a=(f->r[0]&0xffffu);b=0x0000005au;t=mi_alu(f,a,b,16,1);
    /* 0046dd7e */
    if(!(f->flags&MI_ZF))goto mi_0046dd8e;
    /* 0046dd80 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dd81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dd86u;f->boundary(0x0046dd81u,0x00487ec0u,f);
    /* 0046dd86 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dd89 */
    goto mi_0046e10b;
mi_0046dd8e:;
    /* 0046dd8e */
    a=(f->r[0]&0xffffu);b=0x0000004bu;t=mi_alu(f,a,b,16,1);
    /* 0046dd92 */
    if(!(f->flags&MI_ZF))goto mi_0046dde5;
    /* 0046dd94 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046dd99 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046dd9b */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046dda1 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046dda2 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046dda4 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046dda6 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0046dda8 */
    if(!(f->flags&MI_SF))goto mi_0046ddad;
    /* 0046ddaa */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0046ddad:;
    /* 0046ddad */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046ddb4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046ddbb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ddbc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ddbd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ddc2u;f->boundary(0x0046ddbdu,0x0047a1c0u,f);
    /* 0046ddc2 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ddc5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046ddc7 */
    if((f->flags&MI_ZF))goto mi_0046ddd6;
    /* 0046ddc9 */
    f->r[0]=mi_read((0x00f5727cu),4u);
    /* 0046ddce */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046ddd0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ddd1 */
    goto mi_0046dd35;
mi_0046ddd6:;
    /* 0046ddd6 */
    f->r[1]=mi_read((0x00f56cc4u),4u);
    /* 0046dddc */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 0046dddf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dde0 */
    goto mi_0046dd35;
mi_0046dde5:;
    /* 0046dde5 */
    a=(f->r[0]&0xffffu);b=0x000000c0u;t=mi_alu(f,a,b,16,1);
    /* 0046dde9 */
    if(!(f->flags&MI_ZF))goto mi_0046de2b;
    /* 0046ddeb */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046ddf0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046ddf2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046ddf8 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046ddf9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046ddfb */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046ddfd */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046ddff */
    if(!(f->flags&MI_SF))goto mi_0046de04;
    /* 0046de01 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_0046de04:;
    /* 0046de04 */
    f->r[2]=mi_read((0x00f56c14u),4u);
    /* 0046de0a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046de11 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0046de13 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046de1a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de1b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de1c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de1d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de1e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046de23u;f->boundary(0x0046de1eu,0x0047d1f0u,f);
    /* 0046de23 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046de26 */
    goto mi_0046e10b;
mi_0046de2b:;
    /* 0046de2b */
    a=(f->r[0]&0xffffu);b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 0046de2f */
    if(!(f->flags&MI_ZF))goto mi_0046de99;
    /* 0046de31 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046de36 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046de38 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[6]),1u))&0xffu)<<0);
    /* 0046de3e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046de3f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046de41 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046de43 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0046de45 */
    if(!(f->flags&MI_SF))goto mi_0046de4a;
    /* 0046de47 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
mi_0046de4a:;
    /* 0046de4a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046de51 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046de58 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de59 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046de5fu;f->boundary(0x0046de5au,0x0047a1c0u,f);
    /* 0046de5f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046de62 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0046de64 */
    if((f->flags&MI_ZF))goto mi_0046de71;
    /* 0046de66 */
    f->r[1]=mi_read((0x00f57274u),4u);
    /* 0046de6c */
    f->r[0]=(0x00000000u+f->r[1]+f->r[3]*1u);
    /* 0046de6f */
    goto mi_0046de7a;
mi_0046de71:;
    /* 0046de71 */
    f->r[2]=mi_read((0x00f56bf4u),4u);
    /* 0046de77 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[3]*1u);
mi_0046de7a:;
    /* 0046de7a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046de81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de82 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046de89 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de8a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de8b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046de8c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046de91u;f->boundary(0x0046de8cu,0x0047cf00u,f);
    /* 0046de91 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046de94 */
    goto mi_0046e10b;
mi_0046de99:;
    /* 0046de99 */
    a=(f->r[0]&0xffffu);b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 0046de9d */
    if(!(f->flags&MI_ZF))goto mi_0046ded5;
    /* 0046de9f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dea0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dea5u;f->boundary(0x0046dea0u,0x00475b50u,f);
    /* 0046dea5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dea8 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046deaa */
    if(!(f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046deb0 */
    f->r[0]=mi_read((0x00799ab0u+f->r[6]),4u);
    /* 0046deb6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046deb8 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046debe */
    f->r[2]=mi_read((0x00799aa8u+f->r[6]),4u);
    /* 0046dec4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dec5 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dec7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dec8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046decdu;f->boundary(0x0046dec8u,0x00440060u,f);
    /* 0046decd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ded0 */
    goto mi_0046e10b;
mi_0046ded5:;
    /* 0046ded5 */
    a=(f->r[0]&0xffffu);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 0046ded9 */
    if(!(f->flags&MI_ZF))goto mi_0046df43;
    /* 0046dedb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a12u+f->r[6]),2u);
    /* 0046dee2 */
    f->r[1]=f->r[0];
    /* 0046dee4 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0046dee7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0046dee9 */
    a=mi_read((0x007999e8u+f->r[1]*8u),2u);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 0046def2 */
    if(!(f->flags&MI_ZF))goto mi_0046df0d;
    /* 0046def4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046defb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046df02 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df03 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df04 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046df0au;f->boundary(0x0046df05u,0x00440540u,f);
    /* 0046df0a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0046df0d:;
    /* 0046df0d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df0e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046df13u;f->boundary(0x0046df0eu,0x00475b50u,f);
    /* 0046df13 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046df16 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046df18 */
    if(!(f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046df1e */
    f->r[0]=mi_read((0x00799ab0u+f->r[6]),4u);
    /* 0046df24 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046df26 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046df2c */
    f->r[1]=mi_read((0x00799aa8u+f->r[6]),4u);
    /* 0046df32 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df33 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df35 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df36 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046df3bu;f->boundary(0x0046df36u,0x00440060u,f);
    /* 0046df3b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046df3e */
    goto mi_0046e10b;
mi_0046df43:;
    /* 0046df43 */
    a=(f->r[0]&0xffffu);b=0x00000021u;t=mi_alu(f,a,b,16,1);
    /* 0046df47 */
    if(!(f->flags&MI_ZF))goto mi_0046df7f;
    /* 0046df49 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046df4fu;f->boundary(0x0046df4au,0x00475b50u,f);
    /* 0046df4f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046df52 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046df54 */
    if(!(f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046df5a */
    f->r[0]=mi_read((0x00799ab0u+f->r[6]),4u);
    /* 0046df60 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046df62 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046df68 */
    f->r[2]=mi_read((0x00799aa8u+f->r[6]),4u);
    /* 0046df6e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df6f */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df71 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046df77u;f->boundary(0x0046df72u,0x00440060u,f);
    /* 0046df77 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046df7a */
    goto mi_0046e10b;
mi_0046df7f:;
    /* 0046df7f */
    a=(f->r[0]&0xffffu);b=0x000000d1u;t=mi_alu(f,a,b,16,1);
    /* 0046df83 */
    if(!(f->flags&MI_ZF))goto mi_0046dfbe;
    /* 0046df85 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046df86 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046df8bu;f->boundary(0x0046df86u,0x00475b50u,f);
    /* 0046df8b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046df8e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046df90 */
    if(!(f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046df96 */
    f->r[0]=mi_read((0x00799ab0u+f->r[6]),4u);
    /* 0046df9c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0046df9e */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046dfa4 */
    f->r[0]=mi_read((0x00799aa8u+f->r[6]),4u);
    /* 0046dfaa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dfab */
    a=0x000000d1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dfb0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dfb1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dfb6u;f->boundary(0x0046dfb1u,0x00440060u,f);
    /* 0046dfb6 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dfb9 */
    goto mi_0046e10b;
mi_0046dfbe:;
    /* 0046dfbe */
    a=f->r[5];b=0x00000041u;t=mi_alu(f,a,b,32,1);
    /* 0046dfc1 */
    if(mi_signed_less(f->flags))goto mi_0046dfd8;
    /* 0046dfc3 */
    a=f->r[5];b=0x00000045u;t=mi_alu(f,a,b,32,1);
    /* 0046dfc6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046dfd8;
    /* 0046dfc8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dfca */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046dfcb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046dfd0u;f->boundary(0x0046dfcbu,0x004c0d50u,f);
    /* 0046dfd0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046dfd3 */
    goto mi_0046e10b;
mi_0046dfd8:;
    /* 0046dfd8 */
    a=f->r[5];b=0xffffff2bu;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0046dfde */
    a=f->r[5];b=0x00000017u;t=mi_alu(f,a,b,32,1);
    /* 0046dfe1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0046e10b;
    /* 0046dfe7 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046dfe9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x0046e160u+f->r[5]),1u))&0xffu)<<0);
    /* 0046dfef */
    switch(mi_read((0x0046e124u+f->r[1]*4u),4u)){case 0x0046dff6u:goto mi_0046dff6;case 0x0046e023u:goto mi_0046e023;case 0x0046e069u:goto mi_0046e069;case 0x0046e086u:goto mi_0046e086;case 0x0046e09eu:goto mi_0046e09e;case 0x0046e0a9u:goto mi_0046e0a9;case 0x0046e0b6u:goto mi_0046e0b6;case 0x0046e0c3u:goto mi_0046e0c3;case 0x0046e0d0u:goto mi_0046e0d0;case 0x0046e0ddu:goto mi_0046e0dd;case 0x0046e0e8u:goto mi_0046e0e8;case 0x0046e0f0u:goto mi_0046e0f0;case 0x0046e0f8u:goto mi_0046e0f8;case 0x0046e100u:goto mi_0046e100;case 0x0046e10bu:goto mi_0046e10b;default:abort();}
mi_0046dff6:;
    /* 0046dff6 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046dffb */
    f->r[1]=mi_read((0x00f57374u),4u);
    /* 0046e001 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046e002 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e004 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046e006 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046e009 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046e00b */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046e011 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046e013 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e015 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0046e01b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e01c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e01d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e01e */
    goto mi_0046d8a7;
mi_0046e023:;
    /* 0046e023 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046e028 */
    f->r[1]=mi_read((0x00f5737cu),4u);
    /* 0046e02e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046e02f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e031 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046e033 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0046e036 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046e038 */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046e03e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[6]),2u);
    /* 0046e045 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0046e047 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e049 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0046e04f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e050 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e051 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e052 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[6]),2u);
    /* 0046e059 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e05a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e05b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e05c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e061u;f->boundary(0x0046e05cu,0x0047bf50u,f);
    /* 0046e061 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e064 */
    goto mi_0046e10b;
mi_0046e069:;
    /* 0046e069 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 0046e06e */
    f->r[1]=mi_read((0x00f57384u),4u);
    /* 0046e074 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0046e075 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0046e077 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0046e079 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0046e07b */
    if((f->flags&MI_ZF))goto mi_0046e10b;
    /* 0046e081 */
    goto mi_0046db1d;
mi_0046e086:;
    /* 0046e086 */
    f->r[2]=mi_read((0x00f56b3cu),4u);
    /* 0046e08c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046e08e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 0046e094 */
    a=0x00000048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e096 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e097 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e098 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e099 */
    goto mi_0046db2a;
mi_0046e09e:;
    /* 0046e09e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e09f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e0a4u;f->boundary(0x0046e09fu,0x00541920u,f);
    /* 0046e0a4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e0a7 */
    goto mi_0046e10b;
mi_0046e0a9:;
    /* 0046e0a9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0aa */
    f->r[1]=0x006fd5d8u;
    /* 0046e0af */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e0b4u;f->boundary(0x0046e0afu,0x0040dca0u,f);
    /* 0046e0b4 */
    goto mi_0046e10b;
mi_0046e0b6:;
    /* 0046e0b6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0b7 */
    f->r[1]=0x006de650u;
    /* 0046e0bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e0c1u;f->boundary(0x0046e0bcu,0x004017a0u,f);
    /* 0046e0c1 */
    goto mi_0046e10b;
mi_0046e0c3:;
    /* 0046e0c3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0c4 */
    f->r[1]=0x006fd368u;
    /* 0046e0c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e0ceu;f->boundary(0x0046e0c9u,0x004017a0u,f);
    /* 0046e0ce */
    goto mi_0046e10b;
mi_0046e0d0:;
    /* 0046e0d0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0d1 */
    f->r[1]=0x006de548u;
    /* 0046e0d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e0dbu;f->boundary(0x0046e0d6u,0x004e69c0u,f);
    /* 0046e0db */
    goto mi_0046e10b;
mi_0046e0dd:;
    /* 0046e0dd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0de */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e0e3u;f->boundary(0x0046e0deu,0x004dfd90u,f);
    /* 0046e0e3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046e0e6 */
    goto mi_0046e10b;
mi_0046e0e8:;
    /* 0046e0e8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0e9 */
    f->r[1]=0x006f9320u;
    /* 0046e0ee */
    goto mi_0046e106;
mi_0046e0f0:;
    /* 0046e0f0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0f1 */
    f->r[1]=0x006f5648u;
    /* 0046e0f6 */
    goto mi_0046e106;
mi_0046e0f8:;
    /* 0046e0f8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e0f9 */
    f->r[1]=0x006de9e0u;
    /* 0046e0fe */
    goto mi_0046e106;
mi_0046e100:;
    /* 0046e100 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046e101 */
    f->r[1]=0x006e9188u;
mi_0046e106:;
    /* 0046e106 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e10bu;f->boundary(0x0046e106u,0x005468d0u,f);
mi_0046e10b:;
    /* 0046e10b */
    f->r[0]=mi_read((0x0078d790u),4u);
    /* 0046e110 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0046e111 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046e113 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046d856;
    /* 0046e119 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046e11a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0046e11b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0046e11c:;
    /* 0046e11c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046e121u;f->boundary(0x0046e11cu,0x004ee560u,f);
    /* 0046e121 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046e122 */
    f->r[4]+=4;return;
}
