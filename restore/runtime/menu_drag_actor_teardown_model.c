/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_teardown_model.h"
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

static const uint8_t mi_rom_004b4f14[]={0xbf,0x4c,0x4b,0x00,0xee,0x4d,0x4b,0x00};
static const uint8_t mi_rom_004b4f1c[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
static const uint8_t mi_rom_004b4fe8[]={0xfe,0x4c,0x4b,0x00,0xb8,0x4d,0x4b,0x00,0x12,0x4d,0x4b,0x00,0x1c,0x4d,0x4b,0x00,0x26,0x4d,0x4b,0x00,0x30,0x4d,0x4b,0x00,0x3a,0x4d,0x4b,0x00,0x48,0x4d,0x4b,0x00,0x4f,0x4d,0x4b,0x00,0x56,0x4d,0x4b,0x00,0x5d,0x4d,0x4b,0x00,0x64,0x4d,0x4b,0x00,0x41,0x4d,0x4b,0x00,0x87,0x4d,0x4b,0x00,0x6b,0x4d,0x4b,0x00,0x72,0x4d,0x4b,0x00,0x79,0x4d,0x4b,0x00,0x80,0x4d,0x4b,0x00,0xb8,0x4d,0x4b,0x00,0x08,0x4d,0x4b,0x00,0x8e,0x4d,0x4b,0x00,0x95,0x4d,0x4b,0x00,0x9c,0x4d,0x4b,0x00,0xa3,0x4d,0x4b,0x00,0xaa,0x4d,0x4b,0x00,0xb1,0x4d,0x4b,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004b4f14u&&width<=8u&&(offset=address-0x004b4f14u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b4f14[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b4f1cu&&width<=202u&&(offset=address-0x004b4f1cu)<=202u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b4f1c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004b4fe8u&&width<=104u&&(offset=address-0x004b4fe8u)<=104u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004b4fe8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_004b4980(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b4980 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4981 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4982 */
    f->r[5]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004b4986 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4987 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4988 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004b498e */
    f->r[7]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004b4992 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004b4995 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004b4997 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004b499a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[7]),1u))&0xffu)<<0);
    /* 004b49a0 */
    a=(f->r[0]&0xffu);b=0x00000039u;t=mi_alu(f,a,b,8,1);
    /* 004b49a2 */
    if(!(f->flags&MI_ZF))goto mi_004b49bb;
    /* 004b49a4 */
    f->r[1]=f->r[2];
    /* 004b49a6 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004b49ac */
    f->r[6]=mi_read((0x00ea5f04u+f->r[1]),4u);
    /* 004b49b2 */
    f->r[1]=(0x00ea5f04u+f->r[1]);
    /* 004b49b8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004b49b9 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[6]);
mi_004b49bb:;
    /* 004b49bb */
    a=(f->r[0]&0xffu);b=0x0000005du;t=mi_alu(f,a,b,8,1);
    /* 004b49bd */
    if((f->flags&MI_ZF))goto mi_004b49c3;
    /* 004b49bf */
    a=(f->r[0]&0xffu);b=0x00000064u;t=mi_alu(f,a,b,8,1);
    /* 004b49c1 */
    if(!(f->flags&MI_ZF))goto mi_004b49da;
mi_004b49c3:;
    /* 004b49c3 */
    f->r[1]=f->r[2];
    /* 004b49c5 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004b49cb */
    f->r[6]=mi_read((0x00ea6210u+f->r[1]),4u);
    /* 004b49d1 */
    f->r[1]=(0x00ea6210u+f->r[1]);
    /* 004b49d7 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004b49d8 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[6]);
mi_004b49da:;
    /* 004b49da */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004b49dc */
    a=(f->r[0]&0xffu);b=0x0000005cu;t=mi_alu(f,a,b,8,1);
    /* 004b49de */
    if((f->flags&MI_ZF))goto mi_004b49e8;
    /* 004b49e0 */
    a=(f->r[0]&0xffu);b=0x00000065u;t=mi_alu(f,a,b,8,1);
    /* 004b49e2 */
    if((f->flags&MI_ZF))goto mi_004b49e8;
    /* 004b49e4 */
    a=(f->r[0]&0xffu);b=0x0000004du;t=mi_alu(f,a,b,8,1);
    /* 004b49e6 */
    if(!(f->flags&MI_ZF))goto mi_004b4a04;
mi_004b49e8:;
    /* 004b49e8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd68au+f->r[7]),1u))&0xffu)<<0);
    /* 004b49ee */
    a=(f->r[1]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b49f0 */
    if((f->flags&MI_ZF))goto mi_004b4a04;
    /* 004b49f2 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004b49f8 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004b49fb */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b49fe */
    MI_U32((0x00bcba4cu+f->r[1]))=(uint32_t)(f->r[3]);
mi_004b4a04:;
    /* 004b4a04 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004b4a06 */
    if(!(f->flags&MI_ZF))goto mi_004b4a2f;
    /* 004b4a08 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd652u+f->r[7]),2u))&0xffffu)<<0);
    /* 004b4a0f */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b4a12 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4a18 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004b4a1b */
    f->r[2]=f->r[0];
    /* 004b4a1d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b4a20 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b4a22 */
    MI_U16((0x00799a04u+f->r[2]*8u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4a2a */
    goto mi_004b4e5b;
mi_004b4a2f:;
    /* 004b4a2f */
    a=(f->r[0]&0xffu);b=0x00000027u;t=mi_alu(f,a,b,8,1);
    /* 004b4a31 */
    if(!(f->flags&MI_ZF))goto mi_004b4a5a;
    /* 004b4a33 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd652u+f->r[7]),2u))&0xffffu)<<0);
    /* 004b4a3a */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b4a3d */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4a43 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004b4a46 */
    f->r[1]=f->r[0];
    /* 004b4a48 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4a4b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4a4d */
    MI_U16((0x00799a04u+f->r[1]*8u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4a55 */
    goto mi_004b4e5b;
mi_004b4a5a:;
    /* 004b4a5a */
    a=(f->r[0]&0xffu);b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b4a5c */
    if((f->flags&MI_ZF))goto mi_004b4e52;
    /* 004b4a62 */
    a=(f->r[0]&0xffu);b=0x0000004du;t=mi_alu(f,a,b,8,1);
    /* 004b4a64 */
    if((f->flags&MI_ZF))goto mi_004b4e52;
    /* 004b4a6a */
    a=(f->r[0]&0xffu);b=0x0000003eu;t=mi_alu(f,a,b,8,1);
    /* 004b4a6c */
    if(!(f->flags&MI_ZF))goto mi_004b4a89;
    /* 004b4a6e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4a75 */
    f->r[2]=f->r[0];
    /* 004b4a77 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b4a7a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b4a7c */
    MI_U16((0x00799a08u+f->r[2]*8u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4a84 */
    goto mi_004b4e5b;
mi_004b4a89:;
    /* 004b4a89 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004b4a8b */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4a91 */
    a=(f->r[0]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 004b4a93 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4a99 */
    a=(f->r[0]&0xffu);b=0x0000003bu;t=mi_alu(f,a,b,8,1);
    /* 004b4a9b */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4aa1 */
    a=(f->r[0]&0xffu);b=0x0000003cu;t=mi_alu(f,a,b,8,1);
    /* 004b4aa3 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4aa9 */
    a=(f->r[0]&0xffu);b=0x0000003du;t=mi_alu(f,a,b,8,1);
    /* 004b4aab */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ab1 */
    a=(f->r[0]&0xffu);b=0x00000047u;t=mi_alu(f,a,b,8,1);
    /* 004b4ab3 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ab9 */
    a=(f->r[0]&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,1);
    /* 004b4abb */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ac1 */
    a=(f->r[0]&0xffu);b=0x00000044u;t=mi_alu(f,a,b,8,1);
    /* 004b4ac3 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ac9 */
    a=(f->r[0]&0xffu);b=0x00000052u;t=mi_alu(f,a,b,8,1);
    /* 004b4acb */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ad1 */
    a=(f->r[0]&0xffu);b=0x00000046u;t=mi_alu(f,a,b,8,1);
    /* 004b4ad3 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ad9 */
    a=(f->r[0]&0xffu);b=0x00000054u;t=mi_alu(f,a,b,8,1);
    /* 004b4adb */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ae1 */
    a=(f->r[0]&0xffu);b=0x00000053u;t=mi_alu(f,a,b,8,1);
    /* 004b4ae3 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4ae9 */
    a=(f->r[0]&0xffu);b=0x00000067u;t=mi_alu(f,a,b,8,1);
    /* 004b4aeb */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4af1 */
    a=(f->r[0]&0xffu);b=0x00000068u;t=mi_alu(f,a,b,8,1);
    /* 004b4af3 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4af9 */
    a=(f->r[0]&0xffu);b=0x00000066u;t=mi_alu(f,a,b,8,1);
    /* 004b4afb */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4b01 */
    a=(f->r[0]&0xffu);b=0x0000006du;t=mi_alu(f,a,b,8,1);
    /* 004b4b03 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4b09 */
    a=(f->r[0]&0xffu);b=0x00000042u;t=mi_alu(f,a,b,8,1);
    /* 004b4b0b */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4b11 */
    a=(f->r[0]&0xffu);b=0x00000028u;t=mi_alu(f,a,b,8,1);
    /* 004b4b13 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4b19 */
    a=(f->r[0]&0xffu);b=0x0000004bu;t=mi_alu(f,a,b,8,1);
    /* 004b4b1b */
    if((f->flags&MI_ZF))goto mi_004b4e30;
    /* 004b4b21 */
    a=(f->r[0]&0xffu);b=0x0000005au;t=mi_alu(f,a,b,8,1);
    /* 004b4b23 */
    if((f->flags&MI_ZF))goto mi_004b4e30;
    /* 004b4b29 */
    a=(f->r[0]&0xffu);b=0x00000049u;t=mi_alu(f,a,b,8,1);
    /* 004b4b2b */
    if(!(f->flags&MI_ZF))goto mi_004b4b48;
    /* 004b4b2d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4b34 */
    f->r[6]=f->r[1];
    /* 004b4b36 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b4b39 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b4b3b */
    a=mi_read((0x00799a7eu+f->r[6]*8u),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b4b42 */
    if(!(f->flags&MI_ZF))goto mi_004b4e30;
mi_004b4b48:;
    /* 004b4b48 */
    a=(f->r[0]&0xffu);b=0x00000062u;t=mi_alu(f,a,b,8,1);
    /* 004b4b4a */
    if(!(f->flags&MI_ZF))goto mi_004b4b71;
    /* 004b4b4c */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004b4b52 */
    f->r[1]=mi_read((0x00ea61f0u+f->r[2]),4u);
    /* 004b4b58 */
    f->r[0]=(0x00ea61f0u+f->r[2]);
    /* 004b4b5e */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b4b60 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b4b6a;
    /* 004b4b62 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b4b63 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004b4b65 */
    goto mi_004b4e5b;
mi_004b4b6a:;
    /* 004b4b6a */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004b4b6c */
    goto mi_004b4e5b;
mi_004b4b71:;
    /* 004b4b71 */
    a=(f->r[0]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 004b4b73 */
    if(!(f->flags&MI_ZF))goto mi_004b4b90;
    /* 004b4b75 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4b7c */
    f->r[1]=f->r[0];
    /* 004b4b7e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4b81 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4b83 */
    MI_U16((0x00799a76u+f->r[1]*8u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4b8b */
    goto mi_004b4e5b;
mi_004b4b90:;
    /* 004b4b90 */
    a=(f->r[0]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 004b4b92 */
    if(!(f->flags&MI_ZF))goto mi_004b4baf;
    /* 004b4b94 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4b9b */
    f->r[2]=f->r[0];
    /* 004b4b9d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b4ba0 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b4ba2 */
    MI_U16((0x00799a78u+f->r[2]*8u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4baa */
    goto mi_004b4e5b;
mi_004b4baf:;
    /* 004b4baf */
    a=(f->r[0]&0xffu);b=0x00000011u;t=mi_alu(f,a,b,8,1);
    /* 004b4bb1 */
    if(!(f->flags&MI_ZF))goto mi_004b4bce;
    /* 004b4bb3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4bba */
    f->r[1]=f->r[0];
    /* 004b4bbc */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4bbf */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4bc1 */
    MI_U16((0x00799a7au+f->r[1]*8u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4bc9 */
    goto mi_004b4e5b;
mi_004b4bce:;
    /* 004b4bce */
    a=(f->r[0]&0xffu);b=0x00000055u;t=mi_alu(f,a,b,8,1);
    /* 004b4bd0 */
    if(!(f->flags&MI_ZF))goto mi_004b4c00;
    /* 004b4bd2 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[7]),2u);
    /* 004b4bd9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4bda */
    f->r[1]=0x00bca350u;
    /* 004b4bdf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b4be4u;f->boundary(0x004b4bdfu,0x00488050u,f);
    /* 004b4be4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b4be6 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4bec */
    f->r[2]=f->r[6];
    /* 004b4bee */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b4bf1 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b4bf3 */
    MI_U16((0x00799a8cu+f->r[2]*8u))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4bfb */
    goto mi_004b4e5b;
mi_004b4c00:;
    /* 004b4c00 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4c07 */
    a=(f->r[0]&0xffu);b=0x00000026u;t=mi_alu(f,a,b,8,1);
    /* 004b4c09 */
    if(!(f->flags&MI_ZF))goto mi_004b4c33;
    /* 004b4c0b */
    f->r[0]=f->r[2];
    /* 004b4c0d */
    f->r[1]=0x00000003u;
    /* 004b4c12 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4c15 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b4c17 */
    f->r[0]=(0x00799a86u+f->r[0]*8u);
mi_004b4c1e:;
    /* 004b4c1e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[0]),2u);
    /* 004b4c21 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b4c23 */
    if(!(f->flags&MI_ZF))goto mi_004b4c28;
    /* 004b4c25 */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
mi_004b4c28:;
    /* 004b4c28 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b4c2b */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004b4c2c */
    if(!(f->flags&MI_ZF))goto mi_004b4c1e;
    /* 004b4c2e */
    goto mi_004b4e5b;
mi_004b4c33:;
    /* 004b4c33 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b4c35 */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4c3b */
    f->r[1]=f->r[2];
    /* 004b4c3d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4c40 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4c42 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4c45 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a02u+f->r[1]),2u);
    /* 004b4c4c */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b4c4e */
    if(!(f->flags&MI_ZF))goto mi_004b4c59;
    /* 004b4c50 */
    MI_U16((0x00799a02u+f->r[1]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4c57 */
    goto mi_004b4c6b;
mi_004b4c59:;
    /* 004b4c59 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8eu+f->r[1]),2u);
    /* 004b4c60 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b4c62 */
    if(!(f->flags&MI_ZF))goto mi_004b4c6b;
    /* 004b4c64 */
    MI_U16((0x00799a8eu+f->r[1]))=(uint16_t)((f->r[3]&0xffffu));
mi_004b4c6b:;
    /* 004b4c6b */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b4c6d */
    if((f->flags&MI_ZF))goto mi_004b4c77;
    /* 004b4c6f */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004b4c71 */
    if(!(f->flags&MI_ZF))goto mi_004b4e5b;
mi_004b4c77:;
    /* 004b4c77 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[7]),2u);
    /* 004b4c7e */
    f->r[0]=f->r[6];
    /* 004b4c80 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 004b4c86 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4c89 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b4c8b */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[0]*8u),2u))&0xffffu)<<0);
    /* 004b4c93 */
    a=(f->r[0]&0xffffu);b=0x0000006eu;t=mi_alu(f,a,b,16,1);
    /* 004b4c97 */
    if(mi_signed_less(f->flags))goto mi_004b4c9f;
    /* 004b4c99 */
    a=(f->r[0]&0xffffu);b=0x00000072u;t=mi_alu(f,a,b,16,1);
    /* 004b4c9d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b4cbf;
mi_004b4c9f:;
    /* 004b4c9f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004b4ca2 */
    a=f->r[0];b=0xffffffe0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b4ca5 */
    a=f->r[0];b=0x000000c9u;t=mi_alu(f,a,b,32,1);
    /* 004b4caa */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b4dee;
    /* 004b4cb0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004b4cb2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004b4f1cu+f->r[0]),1u))&0xffu)<<0);
    /* 004b4cb8 */
    switch(mi_read((0x004b4f14u+f->r[1]*4u),4u)){case 0x004b4cbfu:goto mi_004b4cbf;case 0x004b4deeu:goto mi_004b4dee;default:abort();}
mi_004b4cbf:;
    /* 004b4cbf */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004b4cc2 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4cc5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b4cc7 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4cca */
    a=mi_read((0x00bcd66cu+f->r[0]),2u);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b4cd1 */
    if((f->flags&MI_ZF))goto mi_004b4dee;
    /* 004b4cd7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[0]),2u);
    /* 004b4cde */
    f->r[2]=f->r[0];
    /* 004b4ce0 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b4ce3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b4ce5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799aa6u+f->r[2]*8u),2u);
    /* 004b4ced */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b4cee */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 004b4cf1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004b4db8;
    /* 004b4cf7 */
    switch(mi_read((0x004b4fe8u+f->r[0]*4u),4u)){case 0x004b4cfeu:goto mi_004b4cfe;case 0x004b4d08u:goto mi_004b4d08;case 0x004b4d12u:goto mi_004b4d12;case 0x004b4d1cu:goto mi_004b4d1c;case 0x004b4d26u:goto mi_004b4d26;case 0x004b4d30u:goto mi_004b4d30;case 0x004b4d3au:goto mi_004b4d3a;case 0x004b4d41u:goto mi_004b4d41;case 0x004b4d48u:goto mi_004b4d48;case 0x004b4d4fu:goto mi_004b4d4f;case 0x004b4d56u:goto mi_004b4d56;case 0x004b4d5du:goto mi_004b4d5d;case 0x004b4d64u:goto mi_004b4d64;case 0x004b4d6bu:goto mi_004b4d6b;case 0x004b4d72u:goto mi_004b4d72;case 0x004b4d79u:goto mi_004b4d79;case 0x004b4d80u:goto mi_004b4d80;case 0x004b4d87u:goto mi_004b4d87;case 0x004b4d8eu:goto mi_004b4d8e;case 0x004b4d95u:goto mi_004b4d95;case 0x004b4d9cu:goto mi_004b4d9c;case 0x004b4da3u:goto mi_004b4da3;case 0x004b4daau:goto mi_004b4daa;case 0x004b4db1u:goto mi_004b4db1;case 0x004b4db8u:goto mi_004b4db8;default:abort();}
mi_004b4cfe:;
    /* 004b4cfe */
    f->r[0]=0x00000010u;
    /* 004b4d03 */
    goto mi_004b4dbb;
mi_004b4d08:;
    /* 004b4d08 */
    f->r[0]=0x00000020u;
    /* 004b4d0d */
    goto mi_004b4dbb;
mi_004b4d12:;
    /* 004b4d12 */
    f->r[0]=0x00000018u;
    /* 004b4d17 */
    goto mi_004b4dbb;
mi_004b4d1c:;
    /* 004b4d1c */
    f->r[0]=0x00000014u;
    /* 004b4d21 */
    goto mi_004b4dbb;
mi_004b4d26:;
    /* 004b4d26 */
    f->r[0]=0x0000000bu;
    /* 004b4d2b */
    goto mi_004b4dbb;
mi_004b4d30:;
    /* 004b4d30 */
    f->r[0]=0x00000019u;
    /* 004b4d35 */
    goto mi_004b4dbb;
mi_004b4d3a:;
    /* 004b4d3a */
    f->r[0]=0x0000000cu;
    /* 004b4d3f */
    goto mi_004b4dbb;
mi_004b4d41:;
    /* 004b4d41 */
    f->r[0]=0x00000009u;
    /* 004b4d46 */
    goto mi_004b4dbb;
mi_004b4d48:;
    /* 004b4d48 */
    f->r[0]=0x0000000au;
    /* 004b4d4d */
    goto mi_004b4dbb;
mi_004b4d4f:;
    /* 004b4d4f */
    f->r[0]=0x0000000eu;
    /* 004b4d54 */
    goto mi_004b4dbb;
mi_004b4d56:;
    /* 004b4d56 */
    f->r[0]=0x00000012u;
    /* 004b4d5b */
    goto mi_004b4dbb;
mi_004b4d5d:;
    /* 004b4d5d */
    f->r[0]=0x00000016u;
    /* 004b4d62 */
    goto mi_004b4dbb;
mi_004b4d64:;
    /* 004b4d64 */
    f->r[0]=0x00000017u;
    /* 004b4d69 */
    goto mi_004b4dbb;
mi_004b4d6b:;
    /* 004b4d6b */
    f->r[0]=0x00000015u;
    /* 004b4d70 */
    goto mi_004b4dbb;
mi_004b4d72:;
    /* 004b4d72 */
    f->r[0]=0x00000011u;
    /* 004b4d77 */
    goto mi_004b4dbb;
mi_004b4d79:;
    /* 004b4d79 */
    f->r[0]=0x0000001du;
    /* 004b4d7e */
    goto mi_004b4dbb;
mi_004b4d80:;
    /* 004b4d80 */
    f->r[0]=0x0000001eu;
    /* 004b4d85 */
    goto mi_004b4dbb;
mi_004b4d87:;
    /* 004b4d87 */
    f->r[0]=0x0000001cu;
    /* 004b4d8c */
    goto mi_004b4dbb;
mi_004b4d8e:;
    /* 004b4d8e */
    f->r[0]=0x00000002u;
    /* 004b4d93 */
    goto mi_004b4dbb;
mi_004b4d95:;
    /* 004b4d95 */
    f->r[0]=0x00000008u;
    /* 004b4d9a */
    goto mi_004b4dbb;
mi_004b4d9c:;
    /* 004b4d9c */
    f->r[0]=0x0000001fu;
    /* 004b4da1 */
    goto mi_004b4dbb;
mi_004b4da3:;
    /* 004b4da3 */
    f->r[0]=0x0000000du;
    /* 004b4da8 */
    goto mi_004b4dbb;
mi_004b4daa:;
    /* 004b4daa */
    f->r[0]=0x00000021u;
    /* 004b4daf */
    goto mi_004b4dbb;
mi_004b4db1:;
    /* 004b4db1 */
    f->r[0]=0x00000022u;
    /* 004b4db6 */
    goto mi_004b4dbb;
mi_004b4db8:;
    /* 004b4db8 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004b4dbb:;
    /* 004b4dbb */
    f->r[1]=f->r[6];
    /* 004b4dbd */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4dc0 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4dc2 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4dc5 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x000000ccu;t=mi_alu(f,a,b,16,1);
    /* 004b4dce */
    if(!(f->flags&MI_ZF))goto mi_004b4de5;
    /* 004b4dd0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b4dd2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[7]),1u))&0xffu)<<0);
    /* 004b4dd8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b4dda */
    if(!(f->flags&MI_ZF))goto mi_004b4de5;
    /* 004b4ddc */
    a=mi_read((0x00799adeu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adeu+f->r[1]))=(uint16_t)(t);
    /* 004b4de3 */
    goto mi_004b4e5b;
mi_004b4de5:;
    /* 004b4de5 */
    a=mi_read((0x00799adcu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adcu+f->r[1]))=(uint16_t)(t);
    /* 004b4dec */
    goto mi_004b4e5b;
mi_004b4dee:;
    /* 004b4dee */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004b4df1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4df4 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4df6 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4df9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[1]),2u);
    /* 004b4e00 */
    f->r[0]=f->r[2];
    /* 004b4e02 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4e05 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b4e07 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4e0a */
    a=mi_read((0x007999e8u+f->r[0]),2u);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 004b4e12 */
    if(!(f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4e14 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00bcd66cu+f->r[1]),2u))&0xffffu)<<0);
    /* 004b4e1b */
    a=(f->r[1]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b4e1e */
    if((f->flags&MI_ZF))goto mi_004b4e5b;
    /* 004b4e20 */
    a=mi_read((0x00799adcu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adcu+f->r[0]))=(uint16_t)(t);
    /* 004b4e27 */
    a=mi_read((0x00799adeu+f->r[0]),2u);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799adeu+f->r[0]))=(uint16_t)(t);
    /* 004b4e2e */
    goto mi_004b4e5b;
mi_004b4e30:;
    /* 004b4e30 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4e37 */
    f->r[1]=f->r[0];
    /* 004b4e39 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4e3c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4e3e */
    f->r[0]=(0x00799a7eu+f->r[1]*8u);
    /* 004b4e45 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a7eu+f->r[1]*8u),1u))&0xffu)<<0);
    /* 004b4e4c */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b4e4e */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004b4e50 */
    goto mi_004b4e5b;
mi_004b4e52:;
    /* 004b4e52 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4e53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b4e58u;f->boundary(0x004b4e53u,0x004a43a0u,f);
    /* 004b4e58 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004b4e5b:;
    /* 004b4e5b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd666u+f->r[7]),1u))&0xffu)<<0);
    /* 004b4e61 */
    a=(f->r[1]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b4e63 */
    if((f->flags&MI_ZF))goto mi_004b4eb8;
    /* 004b4e65 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004b4e6b */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b4e6e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004b4e71 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4e73 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b4e75 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b4e77 */
    MI_U16((0x0077de54u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4e7e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x0077de5au+f->r[0]),2u);
    /* 004b4e85 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b4e87 */
    if(!(f->flags&MI_ZF))goto mi_004b4e92;
    /* 004b4e89 */
    MI_U16((0x0077de5au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4e90 */
    goto mi_004b4eb8;
mi_004b4e92:;
    /* 004b4e92 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x0077de5cu+f->r[0]),2u);
    /* 004b4e99 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b4e9b */
    if(!(f->flags&MI_ZF))goto mi_004b4ea6;
    /* 004b4e9d */
    MI_U16((0x0077de5cu+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004b4ea4 */
    goto mi_004b4eb8;
mi_004b4ea6:;
    /* 004b4ea6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x0077de5eu+f->r[0]),2u);
    /* 004b4ead */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b4eaf */
    if(!(f->flags&MI_ZF))goto mi_004b4eb8;
    /* 004b4eb1 */
    MI_U16((0x0077de5eu+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
mi_004b4eb8:;
    /* 004b4eb8 */
    f->r[0]=mi_read((0x00bcd714u+f->r[7]),4u);
    /* 004b4ebe */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b4ec0 */
    if((f->flags&MI_ZF))goto mi_004b4ecf;
    /* 004b4ec2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004b4ec5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004b4ec8 */
    MI_U8((0x009784d8u+f->r[2]*4u))=(uint8_t)((f->r[3]&0xffu));
mi_004b4ecf:;
    /* 004b4ecf */
    a=mi_read((0x00bcd654u+f->r[7]),2u);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b4ed6 */
    if((f->flags&MI_ZF))goto mi_004b4eee;
    /* 004b4ed8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[7]),2u);
    /* 004b4edf */
    f->r[1]=f->r[0];
    /* 004b4ee1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b4ee4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b4ee6 */
    MI_U16((0x00799a06u+f->r[1]*8u))=(uint16_t)((f->r[3]&0xffffu));
mi_004b4eee:;
    /* 004b4eee */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4eef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b4ef4u;f->boundary(0x004b4eefu,0x004b3250u,f);
    /* 004b4ef4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b4ef5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b4efau;f->boundary(0x004b4ef5u,0x004b5420u,f);
    /* 004b4efa */
    f->r[1]=0x00000061u;
    /* 004b4eff */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b4f01 */
    f->r[7]=(0x00bcd5f8u+f->r[7]);
    /* 004b4f07 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b4f0a */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 004b4f0c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b4f0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b4f0e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b4f0f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b4f10 */
    f->r[4]+=4;return;
}
