/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "diplomacy_input_actions.h"
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
static __inline uint32_t mi_alu(struct restore_diplomacy_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_diplomacy_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_diplomacy_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_0040b340[]={0x78,0xb2,0x40,0x00,0xfb,0xb2,0x40,0x00,0xfb,0xb2,0x40,0x00,0xfb,0xb2,0x40,0x00,0xfb,0xb2,0x40,0x00,0x98,0xb2,0x40,0x00,0xb8,0xb2,0x40,0x00,0xd8,0xb2,0x40,0x00};
static const uint8_t mi_rom_0040b360[]={0x36,0xb3,0x40,0x00,0xfb,0xb2,0x40,0x00,0xfb,0xb2,0x40,0x00,0xfb,0xb2,0x40,0x00,0xfb,0xb2,0x40,0x00,0xb0,0xb2,0x40,0x00,0xd0,0xb2,0x40,0x00,0xf0,0xb2,0x40,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0040b340u&&width<=32u&&(offset=address-0x0040b340u)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0040b340[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0040b360u&&width<=32u&&(offset=address-0x0040b360u)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0040b360[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


void restore_diplomacy_C_0040ae10(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040ae10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae11 */
    f->r[1]=MI_U32((0x00d3b198u));
    /* 0040ae17 */
    f->r[0]=(0x00000000u+f->r[4]);
    /* 0040ae1b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae1c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae1d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ae22u;f->boundary(0x0040ae1du,0x0044d520u,f);
    /* 0040ae22 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ae25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040ae27 */
    if((f->flags&MI_ZF))goto mi_0040aef7;
    /* 0040ae2d */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0040ae33 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0040ae39 */
    a=f->r[0];b=0xfffffffau;t=mi_alu(f,a,b,32,1);
    /* 0040ae3c */
    MI_U8((0x00ea5eddu+f->r[2]))=(uint8_t)(0x00000000u);
    /* 0040ae43 */
    if(!(f->flags&MI_ZF))goto mi_0040ae58;
    /* 0040ae45 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae47 */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae4c */
    a=0x00000062u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae4e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ae53u;f->boundary(0x0040ae4eu,0x004264d0u,f);
    /* 0040ae53 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ae56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040ae57 */
    f->r[4]+=4;return;
mi_0040ae58:;
    /* 0040ae58 */
    a=f->r[0];b=0xfffffffbu;t=mi_alu(f,a,b,32,1);
    /* 0040ae5b */
    if(!(f->flags&MI_ZF))goto mi_0040ae70;
    /* 0040ae5d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae5f */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae64 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae66 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ae6bu;f->boundary(0x0040ae66u,0x004264d0u,f);
    /* 0040ae6b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ae6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040ae6f */
    f->r[4]+=4;return;
mi_0040ae70:;
    /* 0040ae70 */
    a=f->r[0];b=0xfffffffcu;t=mi_alu(f,a,b,32,1);
    /* 0040ae73 */
    if(!(f->flags&MI_ZF))goto mi_0040ae88;
    /* 0040ae75 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae77 */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae7c */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae7e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ae83u;f->boundary(0x0040ae7eu,0x004264d0u,f);
    /* 0040ae83 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ae86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040ae87 */
    f->r[4]+=4;return;
mi_0040ae88:;
    /* 0040ae88 */
    a=f->r[0];b=0xfffffffdu;t=mi_alu(f,a,b,32,1);
    /* 0040ae8b */
    if(!(f->flags&MI_ZF))goto mi_0040aea0;
    /* 0040ae8d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae8f */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae94 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040ae96 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040ae9bu;f->boundary(0x0040ae96u,0x004264d0u,f);
    /* 0040ae9b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040ae9e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040ae9f */
    f->r[4]+=4;return;
mi_0040aea0:;
    /* 0040aea0 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,1);
    /* 0040aea3 */
    if(!(f->flags&MI_ZF))goto mi_0040aec1;
    /* 0040aea5 */
    f->r[0]=MI_U32((0x00000000u+f->r[4]));
    /* 0040aea9 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aeab */
    a=0x00402340u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aeb0 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aeb2 */
    MI_U32((0x006057e4u))=(uint32_t)(f->r[0]);
    /* 0040aeb7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040aebcu;f->boundary(0x0040aeb7u,0x004264d0u,f);
    /* 0040aebc */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040aebf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040aec0 */
    f->r[4]+=4;return;
mi_0040aec1:;
    /* 0040aec1 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0040aec4 */
    if(!(f->flags&MI_ZF))goto mi_0040aed9;
    /* 0040aec6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aec8 */
    a=0x0051f780u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aecd */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aecf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040aed4u;f->boundary(0x0040aecfu,0x004264d0u,f);
    /* 0040aed4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040aed7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040aed8 */
    f->r[4]+=4;return;
mi_0040aed9:;
    /* 0040aed9 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aedb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040aedc */
    a=0x00402340u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aee1 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040aee3 */
    MI_U32((0x006057e4u))=(uint32_t)(f->r[0]);
    /* 0040aee8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040aeedu;f->boundary(0x0040aee8u,0x004264d0u,f);
    /* 0040aeed */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040aef0 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0040aef7:;
    /* 0040aef7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0040aef8 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0040af30(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040af30 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040af35 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040af3b */
    f->r[1]=MI_U32((0x00ea5d44u+f->r[0]));
    /* 0040af41 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0040af43 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040af88;
    /* 0040af45 */
    f->r[1]=MI_U32((0x00d3b198u));
    /* 0040af4b */
    MI_U32((0x00ea450cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0040af51 */
    f->r[1]=MI_U32((0x005d0320u+f->r[1]*4u));
    /* 0040af58 */
    MI_U32((0x00ea4508u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0040af5e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040af63u;f->boundary(0x0040af5eu,0x0045a970u,f);
    /* 0040af63 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040af68u;f->boundary(0x0040af63u,0x0046ec70u,f);
    /* 0040af68 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 0040af6e */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040af75 */
    a=(f->r[1]&0xffu);b=0x0000002cu;t=mi_alu(f,a,b,8,1);
    /* 0040af78 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0040af7b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040af7c */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0040af7f */
    a=f->r[2];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0040af82 */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[2]&0xffu));
mi_0040af88:;
    /* 0040af88 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0040b140(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040b140 */
    f->r[2]=MI_U32((0x00d3b198u));
    /* 0040b146 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040b14b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b14c */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040b14f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b150 */
    if(!(f->flags&MI_ZF))goto mi_0040b16c;
    /* 0040b152 */
    f->r[1]=f->r[0];
    /* 0040b154 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0040b15a */
    f->r[6]=MI_U32((0x00ea5de4u+f->r[1]));
    /* 0040b160 */
    f->r[7]=MI_U32((0x00ea5cc0u+f->r[1]));
    /* 0040b166 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0040b168 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040b1bc;
    /* 0040b16a */
    goto mi_0040b1a8;
mi_0040b16c:;
    /* 0040b16c */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040b16f */
    if(!(f->flags&MI_ZF))goto mi_0040b18b;
    /* 0040b171 */
    f->r[1]=f->r[0];
    /* 0040b173 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0040b179 */
    f->r[6]=MI_U32((0x00ea5de8u+f->r[1]));
    /* 0040b17f */
    f->r[7]=MI_U32((0x00ea5cc0u+f->r[1]));
    /* 0040b185 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0040b187 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040b1bc;
    /* 0040b189 */
    goto mi_0040b1a8;
mi_0040b18b:;
    /* 0040b18b */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0040b18e */
    if(!(f->flags&MI_ZF))goto mi_0040b1a8;
    /* 0040b190 */
    f->r[1]=f->r[0];
    /* 0040b192 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0040b198 */
    f->r[6]=MI_U32((0x00ea5decu+f->r[1]));
    /* 0040b19e */
    f->r[7]=MI_U32((0x00ea5cc0u+f->r[1]));
    /* 0040b1a4 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0040b1a6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040b1bc;
mi_0040b1a8:;
    /* 0040b1a8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040b1a9 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040b1af */
    MI_U32((0x00ea5dc8u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0040b1b5 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0040b1bc:;
    /* 0040b1bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040b1bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040b1be */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0040b1c0(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040b1c0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040b1c5 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040b1cb */
    f->r[1]=MI_U32((0x00ea5de4u+f->r[0]));
    /* 0040b1d1 */
    f->r[2]=MI_U32((0x00ea5cc0u+f->r[0]));
    /* 0040b1d7 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0040b1d9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040b1ee;
    /* 0040b1db */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b1e0u;f->boundary(0x0040b1dbu,0x0046edd0u,f);
    /* 0040b1e0 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000007u);
    /* 0040b1e7 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0040b1ee:;
    /* 0040b1ee */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0040b1f0(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040b1f0 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0040b1f6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b1f7 */
    f->r[0]=f->r[2];
    /* 0040b1f9 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040b1ff */
    f->r[1]=mi_read((0x00ea41ecu+f->r[0]),4u);
    /* 0040b205 */
    f->r[0]=mi_read((0x00ea41f4u+f->r[0]),4u);
    /* 0040b20b */
    f->r[6]=mi_read((0x00784e78u+f->r[1]*4u),4u);
    /* 0040b212 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040b214 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0040b217 */
    MI_U32((0x00784e78u+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0040b21e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0040b221 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0040b224 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b225 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b226 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b227 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b22cu;f->boundary(0x0040b227u,0x0045b4d0u,f);
    /* 0040b22c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040b22f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b234u;f->boundary(0x0040b22fu,0x004ea930u,f);
    /* 0040b234 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040b236 */
    if((f->flags&MI_ZF))goto mi_0040b315;
    /* 0040b23c */
    f->r[0]=mi_read((0x0078d790u),4u);
    /* 0040b241 */
    f->r[6]=0x00000001u;
    /* 0040b246 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040b248 */
    if(mi_signed_less(f->flags))goto mi_0040b310;
    /* 0040b24e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b24f */
    f->r[7]=0x00799b38u;
mi_0040b254:;
    /* 0040b254 */
    a=mi_read((0xffffffbau+f->r[7]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040b259 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0xffffffb8u+f->r[7]),2u);
    /* 0040b25d */
    if(!(f->flags&MI_ZF))goto mi_0040b325;
    /* 0040b263 */
    a=f->r[0];b=0xffffff1bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040b268 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0040b26b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040b2fb;
    /* 0040b271 */
    switch(mi_read((0x0040b340u+f->r[0]*4u),4u)){case 0x0040b278u:goto mi_0040b278;case 0x0040b298u:goto mi_0040b298;case 0x0040b2b8u:goto mi_0040b2b8;case 0x0040b2d8u:goto mi_0040b2d8;case 0x0040b2fbu:goto mi_0040b2fb;default:abort();}
mi_0040b278:;
    /* 0040b278 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b27a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b27b */
    f->r[1]=0x006f9320u;
    /* 0040b280 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b285u;f->boundary(0x0040b280u,0x00545740u,f);
    /* 0040b285 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0040b288 */
    if(!(f->flags&MI_ZF))goto mi_0040b2fb;
    /* 0040b28a */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
    /* 0040b290 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b291 */
    f->r[1]=0x006f9320u;
    /* 0040b296 */
    goto mi_0040b2f6;
mi_0040b298:;
    /* 0040b298 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b29a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b29b */
    f->r[1]=0x006f5648u;
    /* 0040b2a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b2a5u;f->boundary(0x0040b2a0u,0x00558be0u,f);
    /* 0040b2a5 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0040b2a8 */
    if(!(f->flags&MI_ZF))goto mi_0040b2fb;
    /* 0040b2aa */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_0040b2b0:;
    /* 0040b2b0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b2b1 */
    f->r[1]=0x006f5648u;
    /* 0040b2b6 */
    goto mi_0040b2f6;
mi_0040b2b8:;
    /* 0040b2b8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b2ba */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b2bb */
    f->r[1]=0x006de9e0u;
    /* 0040b2c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b2c5u;f->boundary(0x0040b2c0u,0x00555e80u,f);
    /* 0040b2c5 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0040b2c8 */
    if(!(f->flags&MI_ZF))goto mi_0040b2fb;
    /* 0040b2ca */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_0040b2d0:;
    /* 0040b2d0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b2d1 */
    f->r[1]=0x006de9e0u;
    /* 0040b2d6 */
    goto mi_0040b2f6;
mi_0040b2d8:;
    /* 0040b2d8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b2da */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b2db */
    f->r[1]=0x006e9188u;
    /* 0040b2e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b2e5u;f->boundary(0x0040b2e0u,0x0054f470u,f);
    /* 0040b2e5 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0040b2e8 */
    if(!(f->flags&MI_ZF))goto mi_0040b2fb;
    /* 0040b2ea */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000001u);
mi_0040b2f0:;
    /* 0040b2f0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b2f1 */
    f->r[1]=0x006e9188u;
mi_0040b2f6:;
    /* 0040b2f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b2fbu;f->boundary(0x0040b2f6u,0x005468d0u,f);
mi_0040b2fb:;
    /* 0040b2fb */
    f->r[0]=mi_read((0x0078d790u),4u);
    /* 0040b300 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0040b301 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0040b307 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0040b309 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040b254;
    /* 0040b30f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_0040b310:;
    /* 0040b310 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040b315u;f->boundary(0x0040b310u,0x004dea50u,f);
mi_0040b315:;
    /* 0040b315 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000007u);
    /* 0040b31c */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040b323 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040b324 */
    f->r[4]+=4;return;
mi_0040b325:;
    /* 0040b325 */
    a=f->r[0];b=0xffffff1bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040b32a */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0040b32d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0040b2fb;
    /* 0040b32f */
    switch(mi_read((0x0040b360u+f->r[0]*4u),4u)){case 0x0040b2b0u:goto mi_0040b2b0;case 0x0040b2d0u:goto mi_0040b2d0;case 0x0040b2f0u:goto mi_0040b2f0;case 0x0040b2fbu:goto mi_0040b2fb;case 0x0040b336u:goto mi_0040b336;default:abort();}
mi_0040b336:;
    /* 0040b336 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b337 */
    f->r[1]=0x006f9320u;
    /* 0040b33c */
    goto mi_0040b2f6;
}
void restore_diplomacy_C_0040b380(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040b380 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040b385 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000007u);
    /* 0040b38c */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040b392 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040b399 */
    f->r[1]=MI_U32((0x00ea41ecu+f->r[0]));
    /* 0040b39f */
    f->r[2]=MI_U32((0x00ea41f0u+f->r[0]));
    /* 0040b3a5 */
    MI_U32((0x00784e78u+f->r[1]*4u))=(uint32_t)(f->r[2]);
    /* 0040b3ac */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0040b3b0(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040b3b0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0040b3b5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b3b6 */
    f->r[1]=f->r[0];
    /* 0040b3b8 */
    a=f->r[0];b=0x00001276u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0040b3be */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0040b3c4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040b3c5 */
    f->r[2]=MI_U32((0x00ea41ecu+f->r[1]));
    /* 0040b3cb */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040b3cd */
    f->r[6]=MI_U32((0x00784de8u+f->r[2]*4u));
    /* 0040b3d4 */
    f->r[7]=MI_U32((0x00784e78u+f->r[2]*4u));
    /* 0040b3db */
    f->r[2]=MI_U32((0x00ea6160u+f->r[0]*4u));
    /* 0040b3e2 */
    f->r[0]=0x51eb851fu;
    /* 0040b3e7 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0040b3e9 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0040b3ec */
    f->r[0]=f->r[2];
    /* 0040b3ee */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0040b3f0 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0040b3f3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0040b3f5 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0040b3f7 */
    if(!mi_signed_less(f->flags))goto mi_0040b409;
    /* 0040b3f9 */
    MI_U32((0x00ea41f4u+f->r[1]))=(uint32_t)(f->r[6]);
    /* 0040b3ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040b400 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040b407 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040b408 */
    f->r[4]+=4;return;
mi_0040b409:;
    /* 0040b409 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040b40a */
    MI_U32((0x00ea41f4u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0040b410 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040b417 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040b418 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0044d520(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044d520 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d521 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d522 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d523 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0044d525 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d526 */
    f->r[1]=0x00785d70u;
    /* 0044d52b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d530u;f->boundary(0x0044d52bu,0x0044baf0u,f);
    /* 0044d530 */
    f->r[6]=f->r[0];
    /* 0044d532 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044d534 */
    if((f->flags&MI_ZF))goto mi_0044d55b;
    /* 0044d536 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
mi_0044d53a:;
    /* 0044d53a */
    f->r[1]=f->r[6];
    /* 0044d53c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d541u;f->boundary(0x0044d53cu,0x0044d4c0u,f);
    /* 0044d541 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d543 */
    if((f->flags&MI_ZF))goto mi_0044d54a;
    /* 0044d545 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0044d547 */
    if((f->flags&MI_ZF))goto mi_0044d561;
    /* 0044d549 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_0044d54a:;
    /* 0044d54a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d54b */
    f->r[1]=0x00785d70u;
    /* 0044d550 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d555u;f->boundary(0x0044d550u,0x0044baf0u,f);
    /* 0044d555 */
    f->r[6]=f->r[0];
    /* 0044d557 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044d559 */
    if(!(f->flags&MI_ZF))goto mi_0044d53a;
mi_0044d55b:;
    /* 0044d55b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d55c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d55d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044d55f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d560 */
    f->r[4]+=4;return;
mi_0044d561:;
    /* 0044d561 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044d564 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0044d568 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0044d56a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044d56e */
    a=(f->r[0]&0xffffu);b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044d572 */
    if(!(f->flags&MI_ZF))goto mi_0044d597;
    /* 0044d574 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0044d57a */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0044d580 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044d584 */
    a=MI_U32((0x00ea187cu+f->r[2]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044d58a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044d66d;
    /* 0044d590 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d591 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d592 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044d595 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d596 */
    f->r[4]+=4;return;
mi_0044d597:;
    /* 0044d597 */
    a=(f->r[0]&0xffffu);b=0x00000025u;t=mi_alu(f,a,b,16,1);
    /* 0044d59b */
    if(!(f->flags&MI_ZF))goto mi_0044d63d;
    /* 0044d5a1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 0044d5a5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044d5a7 */
    if(mi_signed_less(f->flags))goto mi_0044d603;
    /* 0044d5a9 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 0044d5ac */
    if(!mi_signed_less(f->flags))goto mi_0044d603;
    /* 0044d5ae */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044d5b1 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044d5b4 */
    a=MI_U8((0x007770f0u+f->r[1]*4u));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0044d5bc */
    if(!(f->flags&MI_ZF))goto mi_0044d603;
    /* 0044d5be */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044d5c3 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044d5c9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ea5ed8u+f->r[0])))&0xffu)<<0);
    /* 0044d5cf */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d5d1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d5e6;
    /* 0044d5d3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ea5edau+f->r[0])))&0xffu)<<0);
    /* 0044d5d9 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d5db */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d5e6;
    /* 0044d5dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d5de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d5df */
    f->r[0]=0xfffffffau;
    /* 0044d5e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d5e5 */
    f->r[4]+=4;return;
mi_0044d5e6:;
    /* 0044d5e6 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ea5ed9u+f->r[0])))&0xffu)<<0);
    /* 0044d5ec */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d5ee */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d634;
    /* 0044d5f0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ea5ed5u+f->r[0])))&0xffu)<<0);
    /* 0044d5f6 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d5f8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d634;
    /* 0044d5fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d5fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d5fc */
    f->r[0]=0xfffffffbu;
    /* 0044d601 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d602 */
    f->r[4]+=4;return;
mi_0044d603:;
    /* 0044d603 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044d608 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044d60e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ea5edbu+f->r[0])))&0xffu)<<0);
    /* 0044d614 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d616 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d621;
    /* 0044d618 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d619 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d61a */
    f->r[0]=0xfffffffcu;
    /* 0044d61f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d620 */
    f->r[4]+=4;return;
mi_0044d621:;
    /* 0044d621 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ea5ed6u+f->r[0])))&0xffu)<<0);
    /* 0044d627 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d629 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d634;
    /* 0044d62b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d62c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d62d */
    f->r[0]=0xfffffffdu;
    /* 0044d632 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d633 */
    f->r[4]+=4;return;
mi_0044d634:;
    /* 0044d634 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d635 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d636 */
    f->r[0]=0xfffffffeu;
    /* 0044d63b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d63c */
    f->r[4]+=4;return;
mi_0044d63d:;
    /* 0044d63d */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0044d643 */
    a=f->r[2];b=0x00001276u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0044d649 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0044d64c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044d64e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044d652 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044d655 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044d658 */
    f->r[0]=MI_U32((0x00ea6160u+f->r[2]*4u));
    /* 0044d65f */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044d662 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0044d664 */
    if(!mi_signed_less(f->flags))goto mi_0044d66d;
    /* 0044d666 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d667 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d668 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044d66b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d66c */
    f->r[4]+=4;return;
mi_0044d66d:;
    /* 0044d66d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044d670 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d671 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d672 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044d673 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d674 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0045a970(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045a970 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0045a975 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045a97b */
    f->r[1]=MI_U32((0x00ea4510u+f->r[0]));
    /* 0045a981 */
    MI_U32((0x00ea4504u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0045a987 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0046ec70(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046ec70 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046ec75 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046ec7b */
    MI_U32((0x00ea5df0u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0046ec85 */
    f->r[2]=MI_U32((0x00ea5cbcu+f->r[0]));
    /* 0046ec8b */
    f->r[1]=MI_U32((0x00ea450cu+f->r[0]));
    /* 0046ec91 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046ec93 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0046ec95 */
    if(!(f->flags&MI_ZF))goto mi_0046ec9d;
    /* 0046ec97 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046ec99 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046eca8;
    /* 0046ec9b */
    goto mi_0046eca2;
mi_0046ec9d:;
    /* 0046ec9d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046ec9f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0046eca8;
    /* 0046eca1 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_0046eca2:;
    /* 0046eca2 */
    MI_U32((0x00ea5df0u+f->r[0]))=(uint32_t)(f->r[1]);
mi_0046eca8:;
    /* 0046eca8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046eca9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ecaeu;f->boundary(0x0046eca9u,0x0046e880u,f);
    /* 0046ecae */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046ecb3 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046ecb9 */
    f->r[1]=MI_U32((0x00ea5df0u+f->r[0]));
    /* 0046ecbf */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0046ecc2 */
    if(mi_signed_less(f->flags))goto mi_0046ecd0;
    /* 0046ecc4 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0046ecce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046eccf */
    f->r[4]+=4;return;
mi_0046ecd0:;
    /* 0046ecd0 */
    f->r[6]=MI_U32((0x00ea5d5cu+f->r[0]));
    /* 0046ecd6 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0046ecd9 */
    if(mi_signed_less(f->flags))goto mi_0046ece7;
    /* 0046ecdb */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000002u);
    /* 0046ece5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ece6 */
    f->r[4]+=4;return;
mi_0046ece7:;
    /* 0046ece7 */
    f->r[2]=MI_U32((0x00ea5df8u+f->r[0]));
    /* 0046eced */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0046ecf0 */
    if(mi_signed_less(f->flags))goto mi_0046ecfe;
    /* 0046ecf2 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000003u);
    /* 0046ecfc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ecfd */
    f->r[4]+=4;return;
mi_0046ecfe:;
    /* 0046ecfe */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0046ed01 */
    if(mi_signed_less(f->flags))goto mi_0046ed0f;
    /* 0046ed03 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000004u);
    /* 0046ed0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed0e */
    f->r[4]+=4;return;
mi_0046ed0f:;
    /* 0046ed0f */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046ed12 */
    if(mi_signed_less(f->flags))goto mi_0046ed20;
    /* 0046ed14 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000005u);
    /* 0046ed1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed1f */
    f->r[4]+=4;return;
mi_0046ed20:;
    /* 0046ed20 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0046ed23 */
    if(mi_signed_less(f->flags))goto mi_0046ed31;
    /* 0046ed25 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000006u);
    /* 0046ed2f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed30 */
    f->r[4]+=4;return;
mi_0046ed31:;
    /* 0046ed31 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0046ed34 */
    if(mi_signed_less(f->flags))goto mi_0046ed42;
    /* 0046ed36 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000007u);
    /* 0046ed40 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed41 */
    f->r[4]+=4;return;
mi_0046ed42:;
    /* 0046ed42 */
    f->r[2]=MI_U32((0x00ea5d58u+f->r[0]));
    /* 0046ed48 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0046ed4a */
    if((f->flags&MI_ZF))goto mi_0046ed58;
    /* 0046ed4c */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000008u);
    /* 0046ed56 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed57 */
    f->r[4]+=4;return;
mi_0046ed58:;
    /* 0046ed58 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046ed5b */
    if(mi_signed_less(f->flags))goto mi_0046ed69;
    /* 0046ed5d */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000009u);
    /* 0046ed67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed68 */
    f->r[4]+=4;return;
mi_0046ed69:;
    /* 0046ed69 */
    f->r[2]=MI_U32((0x00ea5df4u+f->r[0]));
    /* 0046ed6f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0046ed71 */
    if((f->flags&MI_ZF))goto mi_0046ed7f;
    /* 0046ed73 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x0000000au);
    /* 0046ed7d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed7e */
    f->r[4]+=4;return;
mi_0046ed7f:;
    /* 0046ed7f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046ed81 */
    if((f->flags&MI_ZF))goto mi_0046ed8f;
    /* 0046ed83 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x0000000bu);
    /* 0046ed8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ed8e */
    f->r[4]+=4;return;
mi_0046ed8f:;
    /* 0046ed8f */
    f->r[1]=MI_U32((0x00ea5e00u+f->r[0]));
    /* 0046ed95 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046ed98 */
    if(!(f->flags&MI_ZF))goto mi_0046eda6;
    /* 0046ed9a */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x0000000cu);
    /* 0046eda4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046eda5 */
    f->r[4]+=4;return;
mi_0046eda6:;
    /* 0046eda6 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046eda9 */
    if(!(f->flags&MI_ZF))goto mi_0046edb7;
    /* 0046edab */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x0000000du);
    /* 0046edb5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046edb6 */
    f->r[4]+=4;return;
mi_0046edb7:;
    /* 0046edb7 */
    MI_U32((0x00ea5cb0u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0046edc1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046edc2 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0046e880(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046e880 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046e885 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046e88b */
    f->r[1]=MI_U32((0x00ea5ea4u+f->r[0]));
    /* 0046e891 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046e893 */
    if((f->flags&MI_ZF))goto mi_0046e8a0;
    /* 0046e895 */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000008u);
    /* 0046e89f */
    f->r[4]+=4;return;
mi_0046e8a0:;
    /* 0046e8a0 */
    f->r[1]=MI_U32((0x00ea5eb4u+f->r[0]));
    /* 0046e8a6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046e8a8 */
    if((f->flags&MI_ZF))goto mi_0046e8b5;
    /* 0046e8aa */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000007u);
    /* 0046e8b4 */
    f->r[4]+=4;return;
mi_0046e8b5:;
    /* 0046e8b5 */
    f->r[1]=MI_U32((0x00ea5c9cu+f->r[0]));
    /* 0046e8bb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0046e8bd */
    if((f->flags&MI_ZF))goto mi_0046e8ca;
    /* 0046e8bf */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000006u);
    /* 0046e8c9 */
    f->r[4]+=4;return;
mi_0046e8ca:;
    /* 0046e8ca */
    f->r[1]=MI_U32((0x00ea5b4cu+f->r[0]));
    /* 0046e8d0 */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0046e8d3 */
    if(!mi_signed_less(f->flags))goto mi_0046e8e0;
    /* 0046e8d5 */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0046e8df */
    f->r[4]+=4;return;
mi_0046e8e0:;
    /* 0046e8e0 */
    a=f->r[1];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0046e8e3 */
    if(!mi_signed_less(f->flags))goto mi_0046e8f0;
    /* 0046e8e5 */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0046e8ef */
    f->r[4]+=4;return;
mi_0046e8f0:;
    /* 0046e8f0 */
    a=f->r[1];b=0x0000003cu;t=mi_alu(f,a,b,32,1);
    /* 0046e8f3 */
    if(!mi_signed_less(f->flags))goto mi_0046e900;
    /* 0046e8f5 */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000002u);
    /* 0046e8ff */
    f->r[4]+=4;return;
mi_0046e900:;
    /* 0046e900 */
    a=f->r[1];b=0x0000005au;t=mi_alu(f,a,b,32,1);
    /* 0046e903 */
    if(!mi_signed_less(f->flags))goto mi_0046e910;
    /* 0046e905 */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000003u);
    /* 0046e90f */
    f->r[4]+=4;return;
mi_0046e910:;
    /* 0046e910 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0046e913 */
    if(!mi_signed_less(f->flags))goto mi_0046e920;
    /* 0046e915 */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000004u);
    /* 0046e91f */
    f->r[4]+=4;return;
mi_0046e920:;
    /* 0046e920 */
    MI_U32((0x00ea5cb4u+f->r[0]))=(uint32_t)(0x00000005u);
    /* 0046e92a */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0046edd0(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046edd0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046edd5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046edd6 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046eddc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046eddd */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0046eddf */
    f->r[1]=MI_U32((0x00ea5dc8u+f->r[0]));
    /* 0046ede5 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046ede7 */
    if(!(f->flags&MI_ZF))goto mi_0046edf1;
    /* 0046ede9 */
    f->r[3]=MI_U32((0x00ea5de4u+f->r[0]));
    /* 0046edef */
    goto mi_0046ee04;
mi_0046edf1:;
    /* 0046edf1 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046edf4 */
    if(!(f->flags&MI_ZF))goto mi_0046edfe;
    /* 0046edf6 */
    f->r[3]=MI_U32((0x00ea5de8u+f->r[0]));
    /* 0046edfc */
    goto mi_0046ee04;
mi_0046edfe:;
    /* 0046edfe */
    f->r[3]=MI_U32((0x00ea5decu+f->r[0]));
mi_0046ee04:;
    /* 0046ee04 */
    a=f->r[3];b=MI_U32((0x00ea5cc0u+f->r[0]));t=mi_alu(f,a,b,32,1);
    /* 0046ee0a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046ef45;
    /* 0046ee10 */
    f->r[1]=MI_U32((0x00ea5dd0u+f->r[0]));
    /* 0046ee16 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ee17 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046ee19 */
    f->r[7]=0x00000004u;
    /* 0046ee1e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0046ee54;
    /* 0046ee20 */
    MI_U32((0x00ea5dd0u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0046ee2a */
    f->r[0]=MI_U32((0x00ea5dc8u+f->r[0]));
    /* 0046ee30 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046ee32 */
    if(!(f->flags&MI_ZF))goto mi_0046ee3b;
    /* 0046ee34 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ee36 */
    goto mi_0046eecf;
mi_0046ee3b:;
    /* 0046ee3b */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046ee3e */
    if(!(f->flags&MI_ZF))goto mi_0046ee47;
    /* 0046ee40 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ee42 */
    goto mi_0046eecf;
mi_0046ee47:;
    /* 0046ee47 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046ee4a */
    if(!(f->flags&MI_ZF))goto mi_0046eed7;
    /* 0046ee50 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ee52 */
    goto mi_0046eecf;
mi_0046ee54:;
    /* 0046ee54 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046ee57 */
    if(!(f->flags&MI_ZF))goto mi_0046ee7e;
    /* 0046ee59 */
    MI_U32((0x00ea5dd0u+f->r[0]))=(uint32_t)(0x00000002u);
    /* 0046ee63 */
    f->r[0]=MI_U32((0x00ea5dc8u+f->r[0]));
    /* 0046ee69 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046ee6b */
    if((f->flags&MI_ZF))goto mi_0046eecd;
    /* 0046ee6d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0046ee6f */
    if(!(f->flags&MI_ZF))goto mi_0046ee75;
    /* 0046ee71 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ee73 */
    goto mi_0046eecf;
mi_0046ee75:;
    /* 0046ee75 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046ee78 */
    if(!(f->flags&MI_ZF))goto mi_0046eed7;
    /* 0046ee7a */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ee7c */
    goto mi_0046eecf;
mi_0046ee7e:;
    /* 0046ee7e */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046ee81 */
    if(!(f->flags&MI_ZF))goto mi_0046eea8;
    /* 0046ee83 */
    MI_U32((0x00ea5dd0u+f->r[0]))=(uint32_t)(0x00000003u);
    /* 0046ee8d */
    f->r[0]=MI_U32((0x00ea5dc8u+f->r[0]));
    /* 0046ee93 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046ee95 */
    if(!(f->flags&MI_ZF))goto mi_0046ee9a;
    /* 0046ee97 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ee98 */
    goto mi_0046eecf;
mi_0046ee9a:;
    /* 0046ee9a */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046ee9d */
    if((f->flags&MI_ZF))goto mi_0046eecd;
    /* 0046ee9f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046eea2 */
    if(!(f->flags&MI_ZF))goto mi_0046eed7;
    /* 0046eea4 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046eea6 */
    goto mi_0046eecf;
mi_0046eea8:;
    /* 0046eea8 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0046eeab */
    if(!(f->flags&MI_ZF))goto mi_0046eed7;
    /* 0046eead */
    MI_U32((0x00ea5dd0u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 0046eeb3 */
    f->r[0]=MI_U32((0x00ea5dc8u+f->r[0]));
    /* 0046eeb9 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046eebb */
    if(!(f->flags&MI_ZF))goto mi_0046eec0;
    /* 0046eebd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046eebe */
    goto mi_0046eecf;
mi_0046eec0:;
    /* 0046eec0 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046eec3 */
    if(!(f->flags&MI_ZF))goto mi_0046eec8;
    /* 0046eec5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046eec6 */
    goto mi_0046eecf;
mi_0046eec8:;
    /* 0046eec8 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046eecb */
    if(!(f->flags&MI_ZF))goto mi_0046eed7;
mi_0046eecd:;
    /* 0046eecd */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0046eecf:;
    /* 0046eecf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046eed4u;f->boundary(0x0046eecfu,0x004e96c0u,f);
    /* 0046eed4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0046eed7:;
    /* 0046eed7 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0046eedc */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046eee2 */
    MI_U32((0x00ea5dccu+f->r[0]))=(uint32_t)(f->r[6]);
    /* 0046eee8 */
    f->r[1]=MI_U32((0x00ea5dc8u+f->r[0]));
    /* 0046eeee */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0046eef0 */
    if(!(f->flags&MI_ZF))goto mi_0046eefa;
    /* 0046eef2 */
    a=MI_U32((0x00ea5dd8u+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00ea5dd8u+f->r[0]))=(uint32_t)(t);
    /* 0046eef8 */
    goto mi_0046ef12;
mi_0046eefa:;
    /* 0046eefa */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0046eefd */
    if(!(f->flags&MI_ZF))goto mi_0046ef07;
    /* 0046eeff */
    a=MI_U32((0x00ea5ddcu+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00ea5ddcu+f->r[0]))=(uint32_t)(t);
    /* 0046ef05 */
    goto mi_0046ef12;
mi_0046ef07:;
    /* 0046ef07 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0046ef0a */
    if(!(f->flags&MI_ZF))goto mi_0046ef12;
    /* 0046ef0c */
    a=MI_U32((0x00ea5de0u+f->r[0]));b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;MI_U32((0x00ea5de0u+f->r[0]))=(uint32_t)(t);
mi_0046ef12:;
    /* 0046ef12 */
    a=MI_U32((0x00ea5dd8u+f->r[0]));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046ef18 */
    if(mi_signed_less(f->flags))goto mi_0046ef20;
    /* 0046ef1a */
    MI_U32((0x00ea5dd8u+f->r[0]))=(uint32_t)(f->r[6]);
mi_0046ef20:;
    /* 0046ef20 */
    a=MI_U32((0x00ea5ddcu+f->r[0]));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046ef26 */
    if(mi_signed_less(f->flags))goto mi_0046ef2e;
    /* 0046ef28 */
    MI_U32((0x00ea5ddcu+f->r[0]))=(uint32_t)(f->r[6]);
mi_0046ef2e:;
    /* 0046ef2e */
    f->r[1]=MI_U32((0x00ea5de0u+f->r[0]));
    /* 0046ef34 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0046ef36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046ef37 */
    if(mi_signed_less(f->flags))goto mi_0046ef3f;
    /* 0046ef39 */
    MI_U32((0x00ea5de0u+f->r[0]))=(uint32_t)(f->r[6]);
mi_0046ef3f:;
    /* 0046ef3f */
    a=MI_U32((0x00ea5cc0u+f->r[0]));b=f->r[3];t=mi_alu(f,a,b,32,1);MI_U32((0x00ea5cc0u+f->r[0]))=(uint32_t)(t);
mi_0046ef45:;
    /* 0046ef45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046ef46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046ef47 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_00402340(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00402340 */
    a=MI_U8((0x00d3b439u));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00402347 */
    if(!(f->flags&MI_ZF))goto mi_00402355;
    /* 00402349 */
    f->r[0]=MI_U32((0x006057e4u));
    /* 0040234e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040234f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00402354u;f->boundary(0x0040234fu,0x0044c1a0u,f);
    /* 00402354 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
mi_00402355:;
    /* 00402355 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_0044c1a0(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c1a0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0044c1a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044c1a6 */
    if(mi_signed_less(f->flags))goto mi_0044c1c7;
    /* 0044c1a8 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044c1ae */
    if(!mi_signed_less(f->flags))goto mi_0044c1c7;
    /* 0044c1b0 */
    f->r[1]=f->r[0];
    /* 0044c1b2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044c1b5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044c1b7 */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044c1be */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0044c1c0 */
    if((f->flags&MI_ZF))goto mi_0044c1c7;
    /* 0044c1c2 */
    goto mi_0044c1d0;
mi_0044c1c7:;
    /* 0044c1c7 */
    f->r[4]+=4;return;
    /* 0044c1c8 */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 0044c1c9 */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 0044c1ca */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 0044c1cb */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 0044c1cc */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 0044c1cd */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 0044c1ce */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 0044c1cf */
    ; /* Native NOP preserves registers, memory and flags. */
mi_0044c1d0:;
    /* 0044c1d0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044c1d3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c1d4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c1d5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c1d6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c1d7 */
    f->r[7]=f->r[1];
    /* 0044c1d9 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0044c1db */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044c1dd */
    a=MI_U16((0x00000030u+f->r[7]));b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044c1e1 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[7]));
    /* 0044c1e5 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c1e8 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044c1eb */
    MI_U16((0x00000030u+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044c1ef */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[7]));
    /* 0044c1f3 */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0044c1f6 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0044c1fa */
    if(!(f->flags&MI_ZF))goto mi_0044c2fb;
    /* 0044c200 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 0044c205 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 0044c20b */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0044c212 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044c213 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0044c216 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 0044c21b */
    if(mi_signed_less(f->flags))goto mi_0044c223;
    /* 0044c21d */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[6]);
mi_0044c223:;
    /* 0044c223 */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 0044c229 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 0044c22e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_0044c236:;
    /* 0044c236 */
    f->r[2]=f->r[1];
    /* 0044c238 */
    f->r[6]=f->r[1];
    /* 0044c23a */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 0044c23d */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044c240 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0044c243 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044c245 */
    f->r[6]=f->r[0];
    /* 0044c247 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 0044c24a */
    f->r[5]=f->r[0];
    /* 0044c24c */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0044c24f */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0044c252 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0044c254 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 0044c256 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 0044c258 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0044c25a */
    if((f->flags&MI_ZF))goto mi_0044c262;
    /* 0044c25c */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_0044c262:;
    /* 0044c262 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044c264 */
    if((f->flags&MI_ZF))goto mi_0044c26b;
    /* 0044c266 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0044c26b:;
    /* 0044c26b */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0044c26f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044c270 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0044c274 */
    if(!(f->flags&MI_ZF))goto mi_0044c236;
    /* 0044c276 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 0044c27c */
    f->r[2]=f->r[1];
    /* 0044c27e */
    f->r[6]=f->r[1];
    /* 0044c280 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044c283 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 0044c289 */
    f->r[1]=f->r[0];
    /* 0044c28b */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044c291 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044c297 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 0044c29d */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 0044c2a3 */
    f->r[1]=f->r[0];
    /* 0044c2a5 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 0044c2aa */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044c2ad */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0044c2b0 */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044c2b3 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0044c2b6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044c2b7 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 0044c2bd */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[1]);
    /* 0044c2c3 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 0044c2c8 */
    MI_U16((0x0000002eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044c2cc */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044c2d1 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044c2d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044c2d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c2d9 */
    f->r[2]=MI_U32((0x00ea187cu+f->r[0]));
    /* 0044c2df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044c2e0 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044c2e2 */
    MI_U32((0x00ea187cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0044c2e8 */
    f->r[1]=MI_U32((0x00ea5fc8u+f->r[0]));
    /* 0044c2ee */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044c2f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044c2f1 */
    MI_U32((0x00ea5fc8u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0044c2f7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c2fa */
    f->r[4]+=4;return;
mi_0044c2fb:;
    /* 0044c2fb */
    a=f->r[0];b=0x00000025u;t=mi_alu(f,a,b,32,1);
    /* 0044c2fe */
    if(!(f->flags&MI_ZF))goto mi_0044c335;
    /* 0044c300 */
    f->r[1]=f->r[7];
    /* 0044c302 */
    MI_U16((0x00000030u+f->r[7]))=(uint16_t)(0x00000005u);
    /* 0044c308 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c30du;f->boundary(0x0044c308u,0x0044c110u,f);
    /* 0044c30d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 0044c310 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c311 */
    MI_U16((0x0000002eu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044c315 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c31au;f->boundary(0x0044c315u,0x004b84b0u,f);
    /* 0044c31a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c31d */
    a=MI_U16((0x0000002eu+f->r[7]));b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044c321 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044c41e;
    /* 0044c327 */
    MI_U16((0x0000002eu+f->r[7]))=(uint16_t)(0x0000000cu);
    /* 0044c32d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044c32e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c32f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044c330 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044c331 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c334 */
    f->r[4]+=4;return;
mi_0044c335:;
    /* 0044c335 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 0044c33a */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 0044c340 */
    f->r[3]=(0x00000000u+f->r[3]+f->r[3]*4u);
    /* 0044c343 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0044c34a */
    f->r[3]=(0x00000000u+f->r[3]+f->r[3]*4u);
    /* 0044c34d */
    a=f->r[3];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 0044c350 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044c351 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0044c354 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 0044c359 */
    if(mi_signed_less(f->flags))goto mi_0044c361;
    /* 0044c35b */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[6]);
mi_0044c361:;
    /* 0044c361 */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 0044c367 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 0044c36c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_0044c374:;
    /* 0044c374 */
    f->r[2]=f->r[1];
    /* 0044c376 */
    f->r[6]=f->r[1];
    /* 0044c378 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 0044c37b */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044c37e */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0044c381 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044c383 */
    f->r[6]=f->r[0];
    /* 0044c385 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 0044c388 */
    f->r[5]=f->r[0];
    /* 0044c38a */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0044c38d */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0044c390 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0044c392 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 0044c394 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 0044c396 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0044c398 */
    if((f->flags&MI_ZF))goto mi_0044c3a0;
    /* 0044c39a */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_0044c3a0:;
    /* 0044c3a0 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044c3a2 */
    if((f->flags&MI_ZF))goto mi_0044c3a9;
    /* 0044c3a4 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0044c3a9:;
    /* 0044c3a9 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0044c3ad */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044c3ae */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0044c3b2 */
    if(!(f->flags&MI_ZF))goto mi_0044c374;
    /* 0044c3b4 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 0044c3ba */
    f->r[2]=f->r[1];
    /* 0044c3bc */
    f->r[6]=f->r[1];
    /* 0044c3be */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044c3c1 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 0044c3c7 */
    f->r[1]=f->r[0];
    /* 0044c3c9 */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044c3cf */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044c3d5 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 0044c3db */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 0044c3e1 */
    f->r[1]=f->r[0];
    /* 0044c3e3 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 0044c3e8 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044c3eb */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0044c3ee */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044c3f1 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0044c3f4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044c3f5 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 0044c3fb */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[1]);
    /* 0044c401 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 0044c406 */
    MI_U16((0x0000002eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044c40a */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0044c40e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044c413 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c414 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c415 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c416 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c41bu;f->boundary(0x0044c416u,0x0045b4d0u,f);
    /* 0044c41b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044c41e:;
    /* 0044c41e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044c41f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c420 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044c421 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044c422 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c425 */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_004b84b0(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b84b0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b84b1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b84b2 */
    f->r[5]=MI_U32((0x0000000cu+f->r[4]));
    /* 004b84b6 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004b84b8 */
    if(mi_signed_less(f->flags))goto mi_004b8688;
    /* 004b84be */
    a=f->r[5];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 004b84c4 */
    if(!mi_signed_less(f->flags))goto mi_004b8688;
    /* 004b84ca */
    f->r[0]=f->r[5];
    /* 004b84cc */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b84cf */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b84d1 */
    f->r[0]=(0x00785d74u+f->r[0]*4u);
    /* 004b84d8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b84da */
    if((f->flags&MI_ZF))goto mi_004b8688;
    /* 004b84e0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[0]));
    /* 004b84e4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b84e6 */
    if(mi_signed_less(f->flags))goto mi_004b8688;
    /* 004b84ec */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004b84ef */
    if(!mi_signed_less(f->flags))goto mi_004b8688;
    /* 004b84f5 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004b84f8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b84f9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b84fa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b84fb */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004b84fe */
    f->r[3]=0x00000001u;
    /* 004b8503 */
    f->r[6]=0x00bcba9au;
    /* 004b8508 */
    a=MI_U8((0x007770f0u+f->r[1]*4u));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b8510 */
    t=((f->flags&MI_ZF))?1u:0u;MI_U8((0x00000013u+f->r[4]))=(uint8_t)(t);
mi_004b8515:;
    /* 004b8515 */
    a=MI_U8((0xffffffbeu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b8519 */
    if(!(f->flags&MI_ZF))goto mi_004b8618;
    /* 004b851f */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004b8522 */
    if((f->flags&MI_ZF))goto mi_004b8618;
    /* 004b8528 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000025u+f->r[6])))&0xffu)<<0);
    /* 004b852b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b852d */
    if((f->flags&MI_ZF))goto mi_004b8618;
    /* 004b8533 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffe6u+f->r[6])))&0xffu)<<0);
    /* 004b8536 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8538 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b8618;
    /* 004b853e */
    a=MI_U16((0x00000040u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004b8543 */
    if(!mi_signed_less(f->flags))goto mi_004b8618;
    /* 004b8549 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b854a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b854fu;f->boundary(0x004b854au,0x0040b5e0u,f);
    /* 004b854f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000017u+f->r[4])))&0xffu)<<0);
    /* 004b8553 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8556 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b8558 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b855b */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b855d */
    if((f->flags&MI_ZF))goto mi_004b8569;
    /* 004b855f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8561 */
    if((f->flags&MI_ZF))goto mi_004b8618;
    /* 004b8567 */
    goto mi_004b8571;
mi_004b8569:;
    /* 004b8569 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b856b */
    if(!(f->flags&MI_ZF))goto mi_004b8618;
mi_004b8571:;
    /* 004b8571 */
    MI_U16((0x00000040u+f->r[6]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004b8575 */
    MI_U8((0xffffffc1u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004b8579 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b857b */
    if((f->flags&MI_ZF))goto mi_004b85a5;
    /* 004b857d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b857e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b8583u;f->boundary(0x004b857eu,0x004b8460u,f);
    /* 004b8583 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8586 */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[0]);
    /* 004b858b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b858d */
    if((f->flags&MI_ZF))goto mi_004b8618;
    /* 004b8593 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b8596 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b8599 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b859b */
    MI_U8((0x00bcd630u+f->r[2]*4u))=(uint8_t)(0x00000011u);
    /* 004b85a3 */
    goto mi_004b8618;
mi_004b85a5:;
    /* 004b85a5 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004b85aa */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004b85ac */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004b85b2 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00ea3ff0u+f->r[0])))&0xffffu)<<0);
    /* 004b85b9 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00ea3ff2u+f->r[0])))&0xffffu)<<0);
    /* 004b85c0 */
    MI_U16((0xfffffff0u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b85c4 */
    MI_U16((0xfffffff2u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b85c8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffe6u+f->r[6])))&0xffu)<<0);
    /* 004b85cb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b85cd */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b8618;
    /* 004b85cf */
    f->r[5]=(0xffffffc6u+f->r[6]);
mi_004b85d2:;
    /* 004b85d2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004b85d6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b85d8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b8607;
    /* 004b85da */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b85dd */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b85e0 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b85e2 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b85e5 */
    a=MI_U8((0x00bcd606u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b85ec */
    if(!(f->flags&MI_ZF))goto mi_004b8607;
    /* 004b85ee */
    a=MI_U8((0x00bcd630u+f->r[0]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b85f5 */
    if((f->flags&MI_ZF))goto mi_004b8607;
    /* 004b85f7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b85f8 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000fu);
    /* 004b85ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b8604u;f->boundary(0x004b85ffu,0x004b3250u,f);
    /* 004b8604 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004b8607:;
    /* 004b8607 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b8609 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004b860a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffe6u+f->r[6])))&0xffu)<<0);
    /* 004b860d */
    a=f->r[5];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004b8610 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b8612 */
    if(mi_signed_less(f->flags))goto mi_004b85d2;
    /* 004b8614 */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
mi_004b8618:;
    /* 004b8618 */
    a=f->r[6];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b861e */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004b861f */
    a=f->r[6];b=0x00bcd62au;t=mi_alu(f,a,b,32,1);
    /* 004b8625 */
    if(mi_signed_less(f->flags))goto mi_004b8515;
    /* 004b862b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000013u+f->r[4])))&0xffu)<<0);
    /* 004b862f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b8630 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b8631 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b8632 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8634 */
    if((f->flags&MI_ZF))goto mi_004b8688;
    /* 004b8636 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 004b863c */
    f->r[0]=0x00bcd786u;
    /* 004b8641 */
    MI_U32((0x00d056f8u))=(uint32_t)(0x000007d0u);
mi_004b864b:;
    /* 004b864b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[0])))&0xffu)<<0);
    /* 004b864e */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8650 */
    if((f->flags&MI_ZF))goto mi_004b867c;
    /* 004b8652 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b8655 */
    if(!(f->flags&MI_ZF))goto mi_004b867c;
    /* 004b8657 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b8659 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000005u+f->r[0])))&0xffu)<<0);
    /* 004b865c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004b865e */
    if(!(f->flags&MI_ZF))goto mi_004b867c;
    /* 004b8660 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffff8u+f->r[0])))&0xffu)<<0);
    /* 004b8663 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8665 */
    if((f->flags&MI_ZF))goto mi_004b867c;
    /* 004b8667 */
    a=MI_U16((0x00000128u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004b866f */
    if(!mi_signed_less(f->flags))goto mi_004b867c;
    /* 004b8671 */
    MI_U16((0x00000128u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004b8678 */
    MI_U8((0x0000002eu+f->r[0]))=(uint8_t)(0x00000011u);
mi_004b867c:;
    /* 004b867c */
    a=f->r[0];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b8681 */
    a=f->r[0];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004b8686 */
    if(mi_signed_less(f->flags))goto mi_004b864b;
mi_004b8688:;
    /* 004b8688 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b8689 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b868a */
    f->r[4]+=4;return;
}
void restore_diplomacy_C_004b8460(struct restore_diplomacy_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b8460 */
    f->r[0]=0x00000001u;
    /* 004b8465 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8466 */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 004b846a */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[0]);
    /* 004b846f */
    f->r[1]=0x00bcd786u;
mi_004b8474:;
    /* 004b8474 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[1])))&0xffu)<<0);
    /* 004b8477 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8479 */
    if((f->flags&MI_ZF))goto mi_004b848c;
    /* 004b847b */
    a=MI_U8((0x00000000u+f->r[1]));b=0x0000004du;t=mi_alu(f,a,b,8,1);
    /* 004b847e */
    if(!(f->flags&MI_ZF))goto mi_004b848c;
    /* 004b8480 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b8482 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000088u+f->r[1])))&0xffu)<<0);
    /* 004b8488 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004b848a */
    if((f->flags&MI_ZF))goto mi_004b84a4;
mi_004b848c:;
    /* 004b848c */
    a=f->r[1];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b8492 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b8493 */
    a=f->r[1];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004b8499 */
    if(mi_signed_less(f->flags))goto mi_004b8474;
    /* 004b849b */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[0]);
    /* 004b84a0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b84a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b84a3 */
    f->r[4]+=4;return;
mi_004b84a4:;
    /* 004b84a4 */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[0]);
    /* 004b84a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b84aa */
    f->r[4]+=4;return;
}
