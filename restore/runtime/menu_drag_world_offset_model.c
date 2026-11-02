/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_offset_model.h"
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

static const uint8_t mi_rom_004406d8[]={0xd5,0x05,0x44,0x00,0xd6,0x06,0x44,0x00,0x66,0x06,0x44,0x00,0xd6,0x06,0x44,0x00,0x1d,0x06,0x44,0x00,0xd6,0x06,0x44,0x00,0x93,0x06,0x44,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004406d8u&&width<=28u&&(offset=address-0x004406d8u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004406d8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_004405c0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004405c0 */
    f->r[0]=mi_read((0x00bca2e0u),4u);
    /* 004405c5 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004405c8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004406d6;
    /* 004405ce */
    switch(mi_read((0x004406d8u+f->r[0]*4u),4u)){case 0x004405d5u:goto mi_004405d5;case 0x0044061du:goto mi_0044061d;case 0x00440666u:goto mi_00440666;case 0x00440693u:goto mi_00440693;case 0x004406d6u:goto mi_004406d6;default:abort();}
mi_004405d5:;
    /* 004405d5 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004405d9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004405db */
    if(!(f->flags&MI_ZF))goto mi_004405f6;
    /* 004405dd */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004405e1 */
    f->r[0]=mi_read((0x00f566fcu),4u);
    /* 004405e6 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004405e8 */
    f->r[2]=mi_read((0x00f566fcu),4u);
    /* 004405ee */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004405f2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004405f3 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004405f5 */
    f->r[4]+=4;return;
mi_004405f6:;
    /* 004405f6 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004405f9 */
    if(!(f->flags&MI_ZF))goto mi_004406d6;
    /* 004405ff */
    f->r[1]=mi_read((0x00f566fcu),4u);
    /* 00440605 */
    f->r[2]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00440609 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044060c */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0044060e */
    f->r[0]=mi_read((0x00f566fcu),4u);
    /* 00440613 */
    f->r[1]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00440617 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044061a */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0044061c */
    f->r[4]+=4;return;
mi_0044061d:;
    /* 0044061d */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00440621 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440623 */
    if(!(f->flags&MI_ZF))goto mi_0044063f;
mi_00440625:;
    /* 00440625 */
    f->r[2]=mi_read((0x00f566fcu),4u);
    /* 0044062b */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0044062f */
    f->r[1]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00440633 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00440634 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00440636 */
    f->r[2]=mi_read((0x00f566fcu),4u);
    /* 0044063c */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0044063e */
    f->r[4]+=4;return;
mi_0044063f:;
    /* 0044063f */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00440642 */
    if(!(f->flags&MI_ZF))goto mi_004406d6;
    /* 00440648 */
    f->r[0]=mi_read((0x00f566fcu),4u);
    /* 0044064d */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00440651 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00440654 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 00440656 */
    f->r[2]=mi_read((0x00f566fcu),4u);
    /* 0044065c */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00440660 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00440663 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00440665 */
    f->r[4]+=4;return;
mi_00440666:;
    /* 00440666 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0044066a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044066c */
    if(!(f->flags&MI_ZF))goto mi_0044068c;
    /* 0044066e */
    f->r[1]=mi_read((0x00f566fcu),4u);
    /* 00440674 */
    f->r[2]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 00440678 */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044067b */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0044067d */
    f->r[0]=mi_read((0x00f566fcu),4u);
    /* 00440682 */
    f->r[1]=mi_read((0x00000008u+f->r[4]),4u);
    /* 00440686 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00440689 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0044068b */
    f->r[4]+=4;return;
mi_0044068c:;
    /* 0044068c */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0044068f */
    if(!(f->flags&MI_ZF))goto mi_004406d6;
    /* 00440691 */
    goto mi_00440625;
mi_00440693:;
    /* 00440693 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00440697 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440699 */
    if(!(f->flags&MI_ZF))goto mi_004406b9;
    /* 0044069b */
    f->r[0]=mi_read((0x00f566fcu),4u);
    /* 004406a0 */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004406a4 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004406a7 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004406a9 */
    f->r[2]=mi_read((0x00f566fcu),4u);
    /* 004406af */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004406b3 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004406b6 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004406b8 */
    f->r[4]+=4;return;
mi_004406b9:;
    /* 004406b9 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004406bc */
    if(!(f->flags&MI_ZF))goto mi_004406d6;
    /* 004406be */
    f->r[2]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 004406c2 */
    f->r[1]=mi_read((0x00f566fcu),4u);
    /* 004406c8 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 004406ca */
    f->r[0]=mi_read((0x00f566fcu),4u);
    /* 004406cf */
    f->r[1]=mi_read((0x00000008u+f->r[4]),4u);
    /* 004406d3 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004406d4 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
mi_004406d6:;
    /* 004406d6 */
    f->r[4]+=4;return;
}
