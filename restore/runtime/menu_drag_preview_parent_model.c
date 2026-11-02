/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "preview_parent_model.h"
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

static const uint8_t mi_rom_00470774[]={0xef,0x00,0x47,0x00,0x77,0x00,0x47,0x00,0x25,0x00,0x47,0x00,0x64,0x01,0x47,0x00,0x05,0x00,0x47,0x00,0x81,0x01,0x47,0x00,0xa5,0xff,0x46,0x00,0x85,0xff,0x46,0x00,0xc5,0xff,0x46,0x00,0x0f,0x01,0x47,0x00,0x2c,0x01,0x47,0x00,0x6b,0x02,0x47,0x00,0xe3,0x05,0x47,0x00,0x3d,0x02,0x47,0x00,0x08,0x02,0x47,0x00,0xe0,0x02,0x47,0x00,0x54,0x02,0x47,0x00,0x37,0x03,0x47,0x00,0xd6,0x04,0x47,0x00,0xfd,0x02,0x47,0x00,0x49,0x05,0x47,0x00,0x49,0x01,0x47,0x00,0x19,0x05,0x47,0x00,0xec,0x04,0x47,0x00,0xb3,0x05,0x47,0x00,0xa3,0x05,0x47,0x00,0x48,0x03,0x47,0x00,0x9f,0x00,0x47,0x00,0xc7,0x00,0x47,0x00,0xbd,0x01,0x47,0x00,0xd4,0x01,0x47,0x00,0x10,0x03,0x47,0x00,0xf1,0x01,0x47,0x00,0x1f,0x02,0x47,0x00,0xb2,0x02,0x47,0x00,0xc9,0x02,0x47,0x00,0xe5,0xff,0x46,0x00,0x95,0x02,0x47,0x00,0x5b,0x07,0x47,0x00,0x13,0x06,0x47,0x00};
static const uint8_t mi_rom_00470814[]={0x00,0x01,0x02,0x03,0x04,0x05,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x06,0x07,0x27,0x08,0x27,0x27,0x27,0x27,0x09,0x0a,0x0b,0x27,0x27,0x27,0x0b,0x0b,0x27,0x27,0x27,0x27,0x0c,0x27,0x27,0x27,0x27,0x27,0x0d,0x27,0x0e,0x27,0x27,0x27,0x27,0x27,0x27,0x0f,0x0f,0x0f,0x0f,0x0f,0x27,0x27,0x10,0x27,0x11,0x12,0x13,0x14,0x14,0x14,0x27,0x27,0x15,0x15,0x16,0x16,0x16,0x16,0x27,0x27,0x17,0x27,0x27,0x27,0x18,0x19,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x1a,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x0f,0x0f,0x0f,0x0f,0x0f,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x1b,0x1c,0x1d,0x1e,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x13,0x1f,0x27,0x27,0x27,0x27,0x16,0x16,0x16,0x27,0x27,0x27,0x10,0x27,0x27,0x27,0x27,0x27,0x27,0x20,0x0e,0x21,0x27,0x27,0x27,0x27,0x22,0x23,0x24,0x25,0x27,0x27,0x26,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x27,0x26,0x27,0x27,0x26,0x27,0x26,0x26,0x26,0x26,0x27,0x27,0x27,0x26,0x26,0x26};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00470774u&&width<=160u&&(offset=address-0x00470774u)<=160u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00470774[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00470814u&&width<=232u&&(offset=address-0x00470814u)<=232u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00470814[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_C_0046fef0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0046fef0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fef1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fef2 */
    f->r[5]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0046fef6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046fef8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fef9 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0046fefb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fefc */
    MI_U32((0x00bc9724u))=(uint32_t)(f->r[0]);
    /* 0046ff01 */
    MI_U32((0x00bc9718u))=(uint32_t)(f->r[0]);
    /* 0046ff06 */
    if((f->flags&MI_ZF))goto mi_0047076d;
    /* 0046ff0c */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0046ff11 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0046ff17 */
    a=mi_read((0x00ea187cu+f->r[0]),4u);b=0xffffec78u;t=mi_alu(f,a,b,32,1);
    /* 0046ff21 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047076d;
    /* 0046ff27 */
    a=0x000000afu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff2c */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff31 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff36 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ff3bu;f->boundary(0x0046ff36u,0x0040f920u,f);
    /* 0046ff3b */
    f->r[1]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 0046ff3f */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*4u);
    /* 0046ff43 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046ff46 */
    f->r[2]=mi_read((0x00705ea4u+f->r[1]*8u),4u);
    /* 0046ff4d */
    f->r[0]=mi_read((0x005d06dcu+f->r[0]*4u),4u);
    /* 0046ff54 */
    f->r[1]=(0xfffffffbu+f->r[5]);
    /* 0046ff57 */
    MI_U32((0x00bc9724u))=(uint32_t)(f->r[2]);
    /* 0046ff5d */
    a=f->r[1];b=0x000000e7u;t=mi_alu(f,a,b,32,1);
    /* 0046ff63 */
    MI_U32((0x00bc96d0u))=(uint32_t)(f->r[0]);
    /* 0046ff68 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00470613;
    /* 0046ff6e */
    f->r[6]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0046ff72 */
    f->r[7]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0046ff76 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0046ff78 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00470814u+f->r[1]),1u))&0xffu)<<0);
    /* 0046ff7e */
    switch(mi_read((0x00470774u+f->r[3]*4u),4u)){case 0x0046ff85u:goto mi_0046ff85;case 0x0046ffa5u:goto mi_0046ffa5;case 0x0046ffc5u:goto mi_0046ffc5;case 0x0046ffe5u:goto mi_0046ffe5;case 0x00470005u:goto mi_00470005;case 0x00470025u:goto mi_00470025;case 0x00470077u:goto mi_00470077;case 0x0047009fu:goto mi_0047009f;case 0x004700c7u:goto mi_004700c7;case 0x004700efu:goto mi_004700ef;case 0x0047010fu:goto mi_0047010f;case 0x0047012cu:goto mi_0047012c;case 0x00470149u:goto mi_00470149;case 0x00470164u:goto mi_00470164;case 0x00470181u:goto mi_00470181;case 0x004701bdu:goto mi_004701bd;case 0x004701d4u:goto mi_004701d4;case 0x004701f1u:goto mi_004701f1;case 0x00470208u:goto mi_00470208;case 0x0047021fu:goto mi_0047021f;case 0x0047023du:goto mi_0047023d;case 0x00470254u:goto mi_00470254;case 0x0047026bu:goto mi_0047026b;case 0x00470295u:goto mi_00470295;case 0x004702b2u:goto mi_004702b2;case 0x004702c9u:goto mi_004702c9;case 0x004702e0u:goto mi_004702e0;case 0x004702fdu:goto mi_004702fd;case 0x00470310u:goto mi_00470310;case 0x00470337u:goto mi_00470337;case 0x00470348u:goto mi_00470348;case 0x004704d6u:goto mi_004704d6;case 0x004704ecu:goto mi_004704ec;case 0x00470519u:goto mi_00470519;case 0x00470549u:goto mi_00470549;case 0x004705a3u:goto mi_004705a3;case 0x004705b3u:goto mi_004705b3;case 0x004705e3u:goto mi_004705e3;case 0x00470613u:goto mi_00470613;case 0x0047075bu:goto mi_0047075b;default:abort();}
mi_0046ff85:;
    /* 0046ff85 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0046ff89 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0046ff8d */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046ff91 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff92 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046ff96 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff97 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff98 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff99 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ff9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ffa0u;f->boundary(0x0046ff9bu,0x00440700u,f);
    /* 0046ffa0 */
    goto mi_0047019c;
mi_0046ffa5:;
    /* 0046ffa5 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0046ffa9 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0046ffad */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046ffb1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffb2 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046ffb6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffb7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffb8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffb9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffbb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ffc0u;f->boundary(0x0046ffbbu,0x00440850u,f);
    /* 0046ffc0 */
    goto mi_0047019c;
mi_0046ffc5:;
    /* 0046ffc5 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0046ffc9 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0046ffcd */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046ffd1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffd2 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046ffd6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffd7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffd8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffd9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046ffdb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046ffe0u;f->boundary(0x0046ffdbu,0x00440b10u,f);
    /* 0046ffe0 */
    goto mi_0047019c;
mi_0046ffe5:;
    /* 0046ffe5 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0046ffe9 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0046ffed */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046fff1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fff2 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0046fff6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fff7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fff8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fff9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046fffb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470000u;f->boundary(0x0046fffbu,0x00440ed0u,f);
    /* 00470000 */
    goto mi_0047019c;
mi_00470005:;
    /* 00470005 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470009 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047000d */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470011 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470012 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470016 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470017 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470018 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470019 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047001b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470020u;f->boundary(0x0047001bu,0x004743f0u,f);
    /* 00470020 */
    goto mi_0047019c;
mi_00470025:;
    /* 00470025 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470029 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047002d */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470031 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470032 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470036 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470037 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470038 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470039 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047003b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470040u;f->boundary(0x0047003bu,0x0047eda0u,f);
    /* 00470040 */
    f->r[1]=mi_read((0x00bca2d4u),4u);
    /* 00470046 */
    f->r[2]=mi_read((0x00bca2d0u),4u);
    /* 0047004c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047004d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047004f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00470050 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470051 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470052 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470054 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470056 */
    MI_U32((0x00bca328u))=(uint32_t)(0x00000001u);
    /* 00470060 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470065u;f->boundary(0x00470060u,0x0047bae0u,f);
    /* 00470065 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470068 */
    MI_U32((0x00605994u))=(uint32_t)(0x00000000u);
    /* 00470072 */
    goto mi_0047075b;
mi_00470077:;
    /* 00470077 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0047007b */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047007f */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470083 */
    a=0x00004000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470088 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470089 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047008d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047008e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047008f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470090 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470092 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470097u;f->boundary(0x00470092u,0x00475220u,f);
    /* 00470097 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047009a */
    goto mi_0047019f;
mi_0047009f:;
    /* 0047009f */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004700a3 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004700a7 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004700ab */
    a=0x00404000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700b0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700b1 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004700b5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700b6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700b7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700b8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004700bfu;f->boundary(0x004700bau,0x00475220u,f);
    /* 004700bf */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004700c2 */
    goto mi_0047019f;
mi_004700c7:;
    /* 004700c7 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004700cb */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004700cf */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004700d3 */
    a=0x00804000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700d8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700d9 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004700dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700de */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700df */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700e0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004700e7u;f->boundary(0x004700e2u,0x00475220u,f);
    /* 004700e7 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004700ea */
    goto mi_0047019f;
mi_004700ef:;
    /* 004700ef */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004700f3 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004700f7 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 004700fb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004700fc */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470100 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470101 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470102 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470103 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470105 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047010au;f->boundary(0x00470105u,0x00474f50u,f);
    /* 0047010a */
    goto mi_0047019c;
mi_0047010f:;
    /* 0047010f */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470113 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470117 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0047011b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047011c */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470120 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470121 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470122 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470123 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470125 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047012au;f->boundary(0x00470125u,0x004754a0u,f);
    /* 0047012a */
    goto mi_0047019c;
mi_0047012c:;
    /* 0047012c */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470130 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470134 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470138 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470139 */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0047013d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047013e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047013f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470140 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470142 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470147u;f->boundary(0x00470142u,0x004759e0u,f);
    /* 00470147 */
    goto mi_0047019c;
mi_00470149:;
    /* 00470149 */
    f->r[0]=mi_read((0x00bc96f8u),4u);
    /* 0047014e */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00470151 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047075b;
    /* 00470157 */
    a=f->r[0];b=f->r[2];t=mi_imul(f,a,b);f->r[0]=t;
    /* 0047015a */
    MI_U32((0x00bc9724u))=(uint32_t)(f->r[0]);
    /* 0047015f */
    goto mi_0047075b;
mi_00470164:;
    /* 00470164 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470168 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047016c */
    f->r[2]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470170 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470171 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470175 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470176 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470177 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470178 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047017a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047017fu;f->boundary(0x0047017au,0x00475090u,f);
    /* 0047017f */
    goto mi_0047019c;
mi_00470181:;
    /* 00470181 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470185 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470189 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0047018d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047018e */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470192 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470193 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470194 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470195 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470197 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047019cu;f->boundary(0x00470197u,0x00474130u,f);
mi_0047019c:;
    /* 0047019c */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047019f:;
    /* 0047019f */
    f->r[0]=mi_read((0x00bc971cu),4u);
    /* 004701a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004701a6 */
    if(mi_signed_less(f->flags))goto mi_0047075b;
    /* 004701ac */
    a=f->r[0];b=mi_read((0x00bc9724u),4u);t=mi_imul(f,a,b);f->r[0]=t;
    /* 004701b3 */
    MI_U32((0x00bc9724u))=(uint32_t)(f->r[0]);
    /* 004701b8 */
    goto mi_0047075b;
mi_004701bd:;
    /* 004701bd */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004701c1 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004701c5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701c7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701c9 */
    a=0xffffffbfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701cb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701cc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701cd */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701cf */
    goto mi_00470753;
mi_004701d4:;
    /* 004701d4 */
    f->r[0]=mi_read((0x00bc9714u),4u);
    /* 004701d9 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004701dd */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004701e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701e2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701e3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701e4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004701e9u;f->boundary(0x004701e4u,0x004bfa50u,f);
    /* 004701e9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004701ec */
    goto mi_0047075b;
mi_004701f1:;
    /* 004701f1 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004701f5 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004701f9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701fb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701fd */
    a=0xffffffbfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004701ff */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470200 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470201 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470203 */
    goto mi_00470753;
mi_00470208:;
    /* 00470208 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0047020c */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470210 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470212 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470214 */
    a=0xffffffbfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470216 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470217 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470218 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047021a */
    goto mi_00470753;
mi_0047021f:;
    /* 0047021f */
    f->r[1]=mi_read((0x00bc9714u),4u);
    /* 00470225 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470229 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047022d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047022e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047022f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470230 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470235u;f->boundary(0x00470230u,0x004bfa50u,f);
    /* 00470235 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470238 */
    goto mi_0047075b;
mi_0047023d:;
    /* 0047023d */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470241 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470245 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470247 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470249 */
    a=0xffffffbfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047024b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047024c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047024d */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047024f */
    goto mi_00470753;
mi_00470254:;
    /* 00470254 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470258 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047025c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047025e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470260 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470262 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470263 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470264 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470266 */
    goto mi_00470753;
mi_0047026b:;
    /* 0047026b */
    a=mi_read((0x00bcb9b4u),4u);b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00470272 */
    if(!mi_signed_less(f->flags))goto mi_0047076d;
    /* 00470278 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0047027c */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470280 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470282 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470284 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470285 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470286 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470288 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047028du;f->boundary(0x00470288u,0x0047d530u,f);
    /* 0047028d */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470290 */
    goto mi_0047075b;
mi_00470295:;
    /* 00470295 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470299 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047029d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047029f */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702a1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702a2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702a3 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702a5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004702aau;f->boundary(0x004702a5u,0x0047d530u,f);
    /* 004702aa */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004702ad */
    goto mi_0047075b;
mi_004702b2:;
    /* 004702b2 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004702b6 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004702ba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702bc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702be */
    a=0xffffffbfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702c0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702c1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702c2 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702c4 */
    goto mi_00470753;
mi_004702c9:;
    /* 004702c9 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004702cd */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004702d1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702d3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702d5 */
    a=0xffffffbfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702d7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702d8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702d9 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702db */
    goto mi_00470753;
mi_004702e0:;
    /* 004702e0 */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004702e4 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004702e8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702ea */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702ec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702ed */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702ee */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004702f0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004702f5u;f->boundary(0x004702f0u,0x0047d530u,f);
    /* 004702f5 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004702f8 */
    goto mi_0047075b;
mi_004702fd:;
    /* 004702fd */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470301 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470305 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470307 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470309 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047030a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047030b */
    goto mi_004704fa;
mi_00470310:;
    /* 00470310 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470312 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470313 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470314 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470319u;f->boundary(0x00470314u,0x0047d060u,f);
    /* 00470319 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047031c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047031e */
    if(!(f->flags&MI_ZF))goto mi_0047032a;
    /* 00470320 */
    MI_U32((0x00605994u))=(uint32_t)(0x00000001u);
mi_0047032a:;
    /* 0047032a */
    a=mi_read((0x00bcb9b0u),4u);b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00470331 */
    if(!mi_signed_less(f->flags))goto mi_0047076d;
mi_00470337:;
    /* 00470337 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470339 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047033a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047033b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470340u;f->boundary(0x0047033bu,0x0047d060u,f);
    /* 00470340 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470343 */
    goto mi_00470502;
mi_00470348:;
    /* 00470348 */
    f->r[5]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0047034c */
    f->r[3]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470350 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470352 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470354 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470355 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470356 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047035bu;f->boundary(0x00470356u,0x0047cc70u,f);
    /* 0047035b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047035e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470360 */
    if(!(f->flags&MI_ZF))goto mi_0047036c;
    /* 00470362 */
    MI_U32((0x00605994u))=(uint32_t)(0x00000001u);
mi_0047036c:;
    /* 0047036c */
    f->r[2]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 00470370 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[2]*2u);
    /* 00470374 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00470377 */
    f->r[0]=mi_read((0x00bca2d8u),4u);
    /* 0047037c */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0047037f */
    f->r[6]=(0x00000000u+f->r[7]+f->r[0]*1u);
    /* 00470382 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00470384 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470385 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047038au;f->boundary(0x00470385u,0x00486cf0u,f);
    /* 0047038a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047038d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047038f */
    if((f->flags&MI_ZF))goto mi_0047075b;
    /* 00470395 */
    f->r[0]=mi_read((0x00bca310u),4u);
    /* 0047039a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047039c */
    if(!(f->flags&MI_ZF))goto mi_004703ee;
    /* 0047039e */
    MI_U8((0x00bca315u))=(uint8_t)((f->r[0]&0xffu));
    /* 004703a3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[6]*2u),2u);
    /* 004703ab */
    f->r[1]=f->r[0];
    /* 004703ad */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004703b0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004703b2 */
    f->r[0]=mi_read((0x00799a9cu+f->r[1]*8u),4u);
    /* 004703b9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004703bb */
    if(!mi_signed_less(f->flags))goto mi_0047075b;
    /* 004703c1 */
    f->r[2]=mi_read((0x00bca2d8u),4u);
    /* 004703c7 */
    MI_U32((0x00bca310u))=(uint32_t)(0x00000002u);
    /* 004703d1 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004703d3 */
    MI_U8((0x00bca315u))=(uint8_t)(0x00000001u);
    /* 004703da */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004703dc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[7]*2u),2u);
    /* 004703e4 */
    MI_U32((0x00bc9800u))=(uint32_t)(f->r[0]);
    /* 004703e9 */
    goto mi_0047075b;
mi_004703ee:;
    /* 004703ee */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004703f1 */
    if(!(f->flags&MI_ZF))goto mi_0047075b;
    /* 004703f7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[6]*2u),2u);
    /* 004703ff */
    f->r[1]=f->r[0];
    /* 00470401 */
    f->r[2]=mi_read((0x00bc9800u),4u);
    /* 00470407 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0047040a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047040c */
    MI_U8((0x00bca315u))=(uint8_t)(0x00000000u);
    /* 00470413 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00470416 */
    f->r[0]=mi_read((0x00799a9cu+f->r[1]),4u);
    /* 0047041c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047041e */
    if(!mi_signed_less(f->flags))goto mi_0047075b;
    /* 00470424 */
    f->r[0]=f->r[2];
    /* 00470426 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470429 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047042b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0047042d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00470430 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[0]),1u))&0xffu)<<0);
    /* 00470436 */
    f->r[2]=(0x00000004u+f->r[2]+f->r[2]*1u);
    /* 0047043a */
    a=f->r[2];b=0x80000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00470440 */
    if(!(f->flags&MI_SF))goto mi_00470447;
    /* 00470442 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00470443 */
    a=f->r[2];b=0xfffffff8u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 00470446 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_00470447:;
    /* 00470447 */
    f->r[7]=(0x00000020u+f->r[4]);
    /* 0047044b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00470453 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470454 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470456 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470457 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00470459 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a82u+f->r[1]),1u))&0xffu)<<0);
    /* 0047045f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00470461 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x007999dbu+f->r[0]),1u))&0xffu)<<0);
    /* 00470467 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00470469 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047046a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047046b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e0u+f->r[0]),2u);
    /* 00470472 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999deu+f->r[0]),2u);
    /* 00470479 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047047a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047047b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047047c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047047d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470482u;f->boundary(0x0047047du,0x00486a30u,f);
    /* 00470482 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470485 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470487 */
    if((f->flags&MI_ZF))goto mi_0047075b;
    /* 0047048d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00b4e6fcu+f->r[6]*2u),2u);
    /* 00470495 */
    f->r[1]=mi_read((0x00bc9800u),4u);
    /* 0047049b */
    f->r[2]=f->r[0];
    /* 0047049d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004704a0 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004704a2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004704a3 */
    MI_U32((0x00bca310u))=(uint32_t)(0x00000003u);
    /* 004704ad */
    MI_U8((0x00bca315u))=(uint8_t)(0x00000001u);
    /* 004704b4 */
    MI_U32((0x00799a9cu+f->r[2]*8u))=(uint32_t)(f->r[1]);
    /* 004704bb */
    f->r[2]=f->r[1];
    /* 004704bd */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004704c0 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004704c2 */
    MI_U32((0x00799a9cu+f->r[2]*8u))=(uint32_t)(f->r[0]);
    /* 004704c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004704ceu;f->boundary(0x004704c9u,0x00486020u,f);
    /* 004704ce */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004704d1 */
    goto mi_0047075b;
mi_004704d6:;
    /* 004704d6 */
    f->r[0]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004704da */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004704de */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004704e0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004704e1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004704e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004704e7u;f->boundary(0x004704e2u,0x0047d060u,f);
    /* 004704e7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004704ea */
    goto mi_00470502;
mi_004704ec:;
    /* 004704ec */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004704f0 */
    f->r[0]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004704f4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004704f6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004704f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004704f9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004704fa:;
    /* 004704fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004704ffu;f->boundary(0x004704fau,0x0047cc70u,f);
    /* 004704ff */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00470502:;
    /* 00470502 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470504 */
    if(!(f->flags&MI_ZF))goto mi_0047075b;
    /* 0047050a */
    MI_U32((0x00605994u))=(uint32_t)(0x00000001u);
    /* 00470514 */
    goto mi_0047075b;
mi_00470519:;
    /* 00470519 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0047051f */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00470525 */
    a=mi_read((0x00ea5cf6u+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0047052d */
    if(!(f->flags&MI_ZF))goto mi_0047075b;
    /* 00470533 */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470537 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 0047053b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047053d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047053f */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470541 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470542 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470543 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470544 */
    goto mi_00470753;
mi_00470549:;
    /* 00470549 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0047054f */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00470555 */
    f->r[1]=mi_read((0x00ea620cu+f->r[2]),4u);
    /* 0047055b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047055d */
    if(!(f->flags&MI_ZF))goto mi_0047075b;
    /* 00470563 */
    f->r[1]=mi_read((0x006059b4u),4u);
    /* 00470569 */
    f->r[6]=mi_read((0x00000020u+f->r[4]),4u);
    /* 0047056d */
    f->r[7]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470571 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00470573 */
    if((f->flags&MI_ZF))goto mi_00470595;
    /* 00470575 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470576 */
    a=0x00000080u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047057b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047057d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047057e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047057f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470580 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470585u;f->boundary(0x00470580u,0x004829b0u,f);
    /* 00470585 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470588 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047058a */
    if((f->flags&MI_ZF))goto mi_0047076d;
    /* 00470590 */
    f->r[0]=mi_read((0x00bc96d0u),4u);
mi_00470595:;
    /* 00470595 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470597 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470599 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047059b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047059c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047059d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047059e */
    goto mi_00470753;
mi_004705a3:;
    /* 004705a3 */
    f->r[1]=mi_read((0x00705c6cu),4u);
    /* 004705a9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004705ab */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047075b;
    /* 004705b1 */
    goto mi_004705fd;
mi_004705b3:;
    /* 004705b3 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 004705b9 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004705bf */
    f->r[2]=mi_read((0x00ea6240u+f->r[1]),4u);
    /* 004705c5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004705c7 */
    if(!(f->flags&MI_ZF))goto mi_0047075b;
    /* 004705cd */
    f->r[2]=mi_read((0x00000020u+f->r[4]),4u);
    /* 004705d1 */
    f->r[1]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 004705d5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004705d7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004705d9 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004705db */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004705dc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004705dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004705de */
    goto mi_00470753;
mi_004705e3:;
    /* 004705e3 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004705e9 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004705ef */
    f->r[1]=mi_read((0x00ea5e70u+f->r[2]),4u);
    /* 004705f5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004705f7 */
    if(!(f->flags&MI_ZF))goto mi_0047075b;
mi_004705fd:;
    /* 004705fd */
    f->r[1]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470601 */
    f->r[2]=mi_read((0x0000001cu+f->r[4]),4u);
    /* 00470605 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470607 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470609 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047060b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047060c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047060d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047060e */
    goto mi_00470753;
mi_00470613:;
    /* 00470613 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470614 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470619u;f->boundary(0x00470614u,0x0046fde0u,f);
    /* 00470619 */
    f->r[6]=mi_read((0x00000024u+f->r[4]),4u);
    /* 0047061d */
    f->r[7]=mi_read((0x00000020u+f->r[4]),4u);
    /* 00470621 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470624 */
    MI_U8((0x00000018u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 00470628 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047062a */
    if(!(f->flags&MI_ZF))goto mi_0047064b;
    /* 0047062c */
    f->r[0]=mi_read((0x00bc96d0u),4u);
    /* 00470631 */
    a=0x00010000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470636 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470638 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470639 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047063a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047063b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470640u;f->boundary(0x0047063bu,0x004828e0u,f);
    /* 00470640 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00470643 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470645 */
    if(!(f->flags&MI_ZF))goto mi_0047076d;
mi_0047064b:;
    /* 0047064b */
    f->r[0]=mi_read((0x006059ccu),4u);
    /* 00470650 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470652 */
    if((f->flags&MI_ZF))goto mi_00470674;
    /* 00470654 */
    a=0x00010800u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470659 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047065b */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0047065d:;
    /* 0047065d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047065e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047065f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470664u;f->boundary(0x0047065fu,0x00482650u,f);
mi_00470664:;
    /* 00470664 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00470667:;
    /* 00470667 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470669 */
    if((f->flags&MI_ZF))goto mi_0047076d;
    /* 0047066f */
    goto mi_0047075b;
mi_00470674:;
    /* 00470674 */
    f->r[0]=mi_read((0x00605a48u),4u);
    /* 00470679 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047067b */
    if((f->flags&MI_ZF))goto mi_00470699;
    /* 0047067d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047067e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470680 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470682 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470683 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470684 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470689u;f->boundary(0x00470684u,0x00482720u,f);
    /* 00470689 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047068c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047068e */
    if((f->flags&MI_ZF))goto mi_0047076d;
    /* 00470694 */
    goto mi_0047075b;
mi_00470699:;
    /* 00470699 */
    f->r[0]=mi_read((0x006059c8u),4u);
    /* 0047069e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004706a0 */
    if((f->flags&MI_ZF))goto mi_004706aa;
    /* 004706a2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706a4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706a6 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706a8 */
    goto mi_0047065d;
mi_004706aa:;
    /* 004706aa */
    f->r[0]=mi_read((0x006059c4u),4u);
    /* 004706af */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004706b1 */
    if((f->flags&MI_ZF))goto mi_004706bb;
    /* 004706b3 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706b5 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706b7 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706b9 */
    goto mi_0047065d;
mi_004706bb:;
    /* 004706bb */
    f->r[0]=mi_read((0x006059bcu),4u);
    /* 004706c0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004706c2 */
    if((f->flags&MI_ZF))goto mi_004706d6;
    /* 004706c4 */
    a=0x00004000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706c9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706cb */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706cd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706ce */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004706d4u;f->boundary(0x004706cfu,0x004828e0u,f);
    /* 004706d4 */
    goto mi_00470664;
mi_004706d6:;
    /* 004706d6 */
    f->r[0]=mi_read((0x006059b8u),4u);
    /* 004706db */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004706dd */
    if((f->flags&MI_ZF))goto mi_004706fb;
    /* 004706df */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706e1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706e3 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706e6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004706ecu;f->boundary(0x004706e7u,0x004828e0u,f);
    /* 004706ec */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004706ef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004706f1 */
    if((f->flags&MI_ZF))goto mi_0047076d;
    /* 004706f3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706f5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706f7 */
    a=0xffffffbfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004706f9 */
    goto mi_0047074a;
mi_004706fb:;
    /* 004706fb */
    f->r[0]=mi_read((0x006059b4u),4u);
    /* 00470700 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470702 */
    if((f->flags&MI_ZF))goto mi_00470722;
    /* 00470704 */
    f->r[2]=mi_read((0x00bc96d0u),4u);
    /* 0047070a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047070b */
    a=0x00000080u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470710 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470712 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470713 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470714 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470715 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047071au;f->boundary(0x00470715u,0x004829b0u,f);
    /* 0047071a */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047071d */
    goto mi_00470667;
mi_00470722:;
    /* 00470722 */
    f->r[0]=mi_read((0x006059c0u),4u);
    /* 00470727 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00470729 */
    if((f->flags&MI_ZF))goto mi_00470739;
    /* 0047072b */
    a=0x00040000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470730 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470732 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470734 */
    goto mi_0047065d;
mi_00470739:;
    /* 00470739 */
    a=f->r[5];b=0x000000b7u;t=mi_alu(f,a,b,32,1);
    /* 0047073f */
    if((f->flags&MI_ZF))goto mi_0047075b;
    /* 00470741 */
    f->r[0]=mi_read((0x00000018u+f->r[4]),4u);
    /* 00470745 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470746 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470748 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0047074a:;
    /* 0047074a */
    f->r[1]=mi_read((0x00bc96d0u),4u);
    /* 00470750 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470751 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00470752 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00470753:;
    /* 00470753 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00470758u;f->boundary(0x00470753u,0x0047d360u,f);
    /* 00470758 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047075b:;
    /* 0047075b */
    f->r[2]=mi_read((0x00bc9718u),4u);
    /* 00470761 */
    f->r[0]=mi_read((0x00bc9724u),4u);
    /* 00470766 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00470768 */
    MI_U32((0x00bc9724u))=(uint32_t)(f->r[0]);
mi_0047076d:;
    /* 0047076d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047076e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047076f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00470770 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00470771 */
    f->r[4]+=4;return;
}
