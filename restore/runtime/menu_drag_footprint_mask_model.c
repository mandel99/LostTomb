/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "road_scan_model.h"
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

static const uint8_t mi_rom_0043f3fc[]={0xb3,0xf3,0x43,0x00,0xc3,0xf3,0x43,0x00,0xd6,0xf3,0x43,0x00,0xe6,0xf3,0x43,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0043f3fcu&&width<=16u&&(offset=address-0x0043f3fcu)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0043f3fc[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_0043f3a0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0043f3a0 */
    f->r[0]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0043f3a4 */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043f3a7 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043f3aa */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0043f3f9;
    /* 0043f3ac */
    switch(mi_read((0x0043f3fcu+f->r[0]*4u),4u)){case 0x0043f3b3u:goto mi_0043f3b3;case 0x0043f3c3u:goto mi_0043f3c3;case 0x0043f3d6u:goto mi_0043f3d6;case 0x0043f3e6u:goto mi_0043f3e6;default:abort();}
mi_0043f3b3:;
    /* 0043f3b3 */
    f->r[1]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0043f3b7 */
    f->r[2]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0043f3bb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x005d2924u+f->r[1]+f->r[2]*2u),1u))&0xffu)<<0);
    /* 0043f3c2 */
    f->r[4]+=4;return;
mi_0043f3c3:;
    /* 0043f3c3 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0043f3c7 */
    f->r[1]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0043f3cb */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0043f3ce */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x005d2928u+f->r[0]+f->r[2]*1u),1u))&0xffu)<<0);
    /* 0043f3d5 */
    f->r[4]+=4;return;
mi_0043f3d6:;
    /* 0043f3d6 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0043f3da */
    f->r[1]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0043f3de */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x005d2934u+f->r[0]+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0043f3e5 */
    f->r[4]+=4;return;
mi_0043f3e6:;
    /* 0043f3e6 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0043f3ea */
    f->r[2]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0043f3ee */
    f->r[1]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0043f3f1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x005d2944u+f->r[0]+f->r[1]*1u),1u))&0xffu)<<0);
    /* 0043f3f8 */
    f->r[4]+=4;return;
mi_0043f3f9:;
    /* 0043f3f9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0043f3fb */
    f->r[4]+=4;return;
}
