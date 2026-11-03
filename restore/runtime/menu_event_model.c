/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_event_actions.h"
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
static __inline uint32_t mi_alu(struct restore_event_actions_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_event_actions_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_event_actions_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static __inline void mi_idiv32(struct restore_event_actions_frame *f,uint32_t divisor_bits)
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
static const uint8_t mi_rom_0044c700[]={0x72,0xc6,0x44,0x00,0xd3,0xc6,0x44,0x00,0xd3,0xc6,0x44,0x00,0x8b,0xc6,0x44,0x00,0x90,0xc6,0x44,0x00,0x95,0xc6,0x44,0x00,0x9a,0xc6,0x44,0x00,0x9f,0xc6,0x44,0x00,0xa4,0xc6,0x44,0x00,0xa9,0xc6,0x44,0x00,0xae,0xc6,0x44,0x00,0xb3,0xc6,0x44,0x00,0x81,0xc6,0x44,0x00,0x86,0xc6,0x44,0x00,0xb8,0xc6,0x44,0x00,0xbd,0xc6,0x44,0x00,0x77,0xc6,0x44,0x00,0x7c,0xc6,0x44,0x00,0xc2,0xc6,0x44,0x00,0xc7,0xc6,0x44,0x00,0xcc,0xc6,0x44,0x00,0xd4,0xc6,0x44,0x00,0xdc,0xc6,0x44,0x00,0xe1,0xc6,0x44,0x00,0xe6,0xc6,0x44,0x00,0xeb,0xc6,0x44,0x00,0xf0,0xc6,0x44,0x00,0xf5,0xc6,0x44,0x00,0xfa,0xc6,0x44,0x00};
static const uint8_t mi_rom_0044cd9c[]={0x33,0xcc,0x44,0x00,0x92,0xcd,0x44,0x00,0x92,0xcd,0x44,0x00,0x92,0xcd,0x44,0x00,0x92,0xcd,0x44,0x00,0xa0,0xcb,0x44,0x00,0xff,0xcb,0x44,0x00,0xaa,0xcb,0x44,0x00};
static const uint8_t mi_rom_0044d3d4[]={0x1a,0xce,0x44,0x00,0x9f,0xce,0x44,0x00,0x9f,0xce,0x44,0x00,0x23,0xce,0x44,0x00,0x31,0xce,0x44,0x00,0x3f,0xce,0x44,0x00,0x4d,0xce,0x44,0x00};
static const uint8_t mi_rom_0044d3f0[]={0x67,0xce,0x44,0x00,0x9f,0xce,0x44,0x00,0x9f,0xce,0x44,0x00,0x6e,0xce,0x44,0x00,0x7c,0xce,0x44,0x00,0x8a,0xce,0x44,0x00,0x98,0xce,0x44,0x00};
static const uint8_t mi_rom_0044d40c[]={0x1d,0xcf,0x44,0x00,0x86,0xcf,0x44,0x00,0x86,0xcf,0x44,0x00,0x26,0xcf,0x44,0x00,0x2f,0xcf,0x44,0x00,0x38,0xcf,0x44,0x00,0x41,0xcf,0x44,0x00};
static const uint8_t mi_rom_0044d428[]={0x5b,0xcf,0x44,0x00,0x86,0xcf,0x44,0x00,0x86,0xcf,0x44,0x00,0x64,0xcf,0x44,0x00,0x6d,0xcf,0x44,0x00,0x76,0xcf,0x44,0x00,0x7f,0xcf,0x44,0x00};
static const uint8_t mi_rom_0044d444[]={0xc8,0xcf,0x44,0x00,0x31,0xd0,0x44,0x00,0x31,0xd0,0x44,0x00,0xd1,0xcf,0x44,0x00,0xda,0xcf,0x44,0x00,0xe3,0xcf,0x44,0x00,0xec,0xcf,0x44,0x00};
static const uint8_t mi_rom_0044d460[]={0x06,0xd0,0x44,0x00,0x31,0xd0,0x44,0x00,0x31,0xd0,0x44,0x00,0x0f,0xd0,0x44,0x00,0x18,0xd0,0x44,0x00,0x21,0xd0,0x44,0x00,0x2a,0xd0,0x44,0x00};
static const uint8_t mi_rom_0044d47c[]={0x68,0xd2,0x44,0x00,0xef,0xd2,0x44,0x00,0xef,0xd2,0x44,0x00,0x71,0xd2,0x44,0x00,0x7f,0xd2,0x44,0x00,0x8d,0xd2,0x44,0x00,0x9b,0xd2,0x44,0x00};
static const uint8_t mi_rom_0044d498[]={0xb5,0xd2,0x44,0x00,0xef,0xd2,0x44,0x00,0xef,0xd2,0x44,0x00,0xbe,0xd2,0x44,0x00,0xcc,0xd2,0x44,0x00,0xda,0xd2,0x44,0x00,0xe8,0xd2,0x44,0x00};
static const uint8_t mi_rom_0044e5bc[]={0x0f,0xe3,0x44,0x00,0x61,0xe3,0x44,0x00,0xb3,0xe3,0x44,0x00,0x1c,0xe4,0x44,0x00,0x65,0xe4,0x44,0x00};
static const uint8_t mi_rom_0044e6e0[]={0x0f,0xe6,0x44,0x00,0x30,0xe6,0x44,0x00,0x51,0xe6,0x44,0x00,0x68,0xe6,0x44,0x00};
static const uint8_t mi_rom_0044f158[]={0x30,0xf1,0x44,0x00,0x38,0xf1,0x44,0x00,0x40,0xf1,0x44,0x00,0x48,0xf1,0x44,0x00};
static const uint8_t mi_rom_0044f2ac[]={0x86,0xf1,0x44,0x00,0xc2,0xf1,0x44,0x00,0xef,0xf1,0x44,0x00,0xa7,0xf2,0x44,0x00,0xa7,0xf2,0x44,0x00,0xf5,0xf1,0x44,0x00,0xfb,0xf1,0x44,0x00,0x01,0xf2,0x44,0x00,0x07,0xf2,0x44,0x00,0x0d,0xf2,0x44,0x00,0x13,0xf2,0x44,0x00,0x13,0xf2,0x44,0x00,0x19,0xf2,0x44,0x00,0x1f,0xf2,0x44,0x00,0x25,0xf2,0x44,0x00,0x2b,0xf2,0x44,0x00,0x31,0xf2,0x44,0x00,0x37,0xf2,0x44,0x00,0x3d,0xf2,0x44,0x00,0x6c,0xf2,0x44,0x00,0x95,0xf2,0x44,0x00,0x9b,0xf2,0x44,0x00,0xa1,0xf2,0x44,0x00};
static const uint8_t mi_rom_0044f308[]={0x9b,0xf1,0x44,0x00,0xa1,0xf1,0x44,0x00,0xa4,0xf1,0x44,0x00,0xaa,0xf1,0x44,0x00,0xb0,0xf1,0x44,0x00,0xb6,0xf1,0x44,0x00,0xbc,0xf1,0x44,0x00};
static const uint8_t mi_rom_0044f324[]={0xd7,0xf1,0x44,0x00,0xdd,0xf1,0x44,0x00,0xe9,0xf1,0x44,0x00,0xe3,0xf1,0x44,0x00};
static const uint8_t mi_rom_0044f334[]={0x4e,0xf2,0x44,0x00,0x54,0xf2,0x44,0x00,0x5a,0xf2,0x44,0x00,0x60,0xf2,0x44,0x00,0x66,0xf2,0x44,0x00};
static const uint8_t mi_rom_0044f348[]={0x7d,0xf2,0x44,0x00,0x83,0xf2,0x44,0x00,0x89,0xf2,0x44,0x00,0x8f,0xf2,0x44,0x00};
static const uint8_t mi_rom_0044f3b4[]={0x84,0xf3,0x44,0x00,0x8a,0xf3,0x44,0x00,0x90,0xf3,0x44,0x00,0x96,0xf3,0x44,0x00,0x9c,0xf3,0x44,0x00,0xa2,0xf3,0x44,0x00,0xa8,0xf3,0x44,0x00};
static const uint8_t mi_rom_0044f424[]={0xf4,0xf3,0x44,0x00,0xfa,0xf3,0x44,0x00,0x00,0xf4,0x44,0x00,0x06,0xf4,0x44,0x00,0x0c,0xf4,0x44,0x00,0x12,0xf4,0x44,0x00,0x18,0xf4,0x44,0x00};
static const uint8_t mi_rom_0044f5ac[]={0x86,0xf4,0x44,0x00,0xc2,0xf4,0x44,0x00,0xef,0xf4,0x44,0x00,0xa7,0xf5,0x44,0x00,0xa7,0xf5,0x44,0x00,0xf5,0xf4,0x44,0x00,0xfb,0xf4,0x44,0x00,0x01,0xf5,0x44,0x00,0x07,0xf5,0x44,0x00,0x0d,0xf5,0x44,0x00,0x13,0xf5,0x44,0x00,0x13,0xf5,0x44,0x00,0x19,0xf5,0x44,0x00,0x1f,0xf5,0x44,0x00,0x25,0xf5,0x44,0x00,0x2b,0xf5,0x44,0x00,0x31,0xf5,0x44,0x00,0x37,0xf5,0x44,0x00,0x3d,0xf5,0x44,0x00,0x6c,0xf5,0x44,0x00,0x95,0xf5,0x44,0x00,0x9b,0xf5,0x44,0x00,0xa1,0xf5,0x44,0x00};
static const uint8_t mi_rom_0044f608[]={0x9b,0xf4,0x44,0x00,0xa1,0xf4,0x44,0x00,0xa4,0xf4,0x44,0x00,0xaa,0xf4,0x44,0x00,0xb0,0xf4,0x44,0x00,0xb6,0xf4,0x44,0x00,0xbc,0xf4,0x44,0x00};
static const uint8_t mi_rom_0044f624[]={0xd7,0xf4,0x44,0x00,0xdd,0xf4,0x44,0x00,0xe9,0xf4,0x44,0x00,0xe3,0xf4,0x44,0x00};
static const uint8_t mi_rom_0044f634[]={0x4e,0xf5,0x44,0x00,0x54,0xf5,0x44,0x00,0x5a,0xf5,0x44,0x00,0x60,0xf5,0x44,0x00,0x66,0xf5,0x44,0x00};
static const uint8_t mi_rom_0044f648[]={0x7d,0xf5,0x44,0x00,0x83,0xf5,0x44,0x00,0x89,0xf5,0x44,0x00,0x8f,0xf5,0x44,0x00};
static const uint8_t mi_rom_0044f7ec[]={0x76,0xf6,0x44,0x00,0xfb,0xf6,0x44,0x00,0x28,0xf7,0x44,0x00,0xe7,0xf7,0x44,0x00,0xe7,0xf7,0x44,0x00,0x2e,0xf7,0x44,0x00,0x34,0xf7,0x44,0x00,0x3a,0xf7,0x44,0x00,0x40,0xf7,0x44,0x00,0x46,0xf7,0x44,0x00,0x4c,0xf7,0x44,0x00,0xe7,0xf7,0x44,0x00,0x52,0xf7,0x44,0x00,0x58,0xf7,0x44,0x00,0x5e,0xf7,0x44,0x00,0x64,0xf7,0x44,0x00,0x6a,0xf7,0x44,0x00,0x70,0xf7,0x44,0x00,0x76,0xf7,0x44,0x00,0xa5,0xf7,0x44,0x00,0xce,0xf7,0x44,0x00,0xd4,0xf7,0x44,0x00,0xda,0xf7,0x44,0x00};
static const uint8_t mi_rom_0044f848[]={0x92,0xf6,0x44,0x00,0x98,0xf6,0x44,0x00,0x9e,0xf6,0x44,0x00,0xa4,0xf6,0x44,0x00,0xaa,0xf6,0x44,0x00,0xb0,0xf6,0x44,0x00,0xb6,0xf6,0x44,0x00};
static const uint8_t mi_rom_0044f864[]={0xd1,0xf6,0x44,0x00,0xd7,0xf6,0x44,0x00,0xdd,0xf6,0x44,0x00,0xe3,0xf6,0x44,0x00,0xe9,0xf6,0x44,0x00,0xef,0xf6,0x44,0x00,0xf5,0xf6,0x44,0x00};
static const uint8_t mi_rom_0044f880[]={0x10,0xf7,0x44,0x00,0x16,0xf7,0x44,0x00,0x22,0xf7,0x44,0x00,0x1c,0xf7,0x44,0x00};
static const uint8_t mi_rom_0044f890[]={0x87,0xf7,0x44,0x00,0x8d,0xf7,0x44,0x00,0x93,0xf7,0x44,0x00,0x99,0xf7,0x44,0x00,0x9f,0xf7,0x44,0x00};
static const uint8_t mi_rom_0044f8a4[]={0xb6,0xf7,0x44,0x00,0xbc,0xf7,0x44,0x00,0xc2,0xf7,0x44,0x00,0xc8,0xf7,0x44,0x00};
static const uint8_t mi_rom_0044fa4c[]={0xd6,0xf8,0x44,0x00,0x5b,0xf9,0x44,0x00,0x88,0xf9,0x44,0x00,0x47,0xfa,0x44,0x00,0x47,0xfa,0x44,0x00,0x8e,0xf9,0x44,0x00,0x94,0xf9,0x44,0x00,0x9a,0xf9,0x44,0x00,0xa0,0xf9,0x44,0x00,0xa6,0xf9,0x44,0x00,0xac,0xf9,0x44,0x00,0x47,0xfa,0x44,0x00,0xb2,0xf9,0x44,0x00,0xb8,0xf9,0x44,0x00,0xbe,0xf9,0x44,0x00,0xc4,0xf9,0x44,0x00,0xca,0xf9,0x44,0x00,0xd0,0xf9,0x44,0x00,0xd6,0xf9,0x44,0x00,0x05,0xfa,0x44,0x00,0x2e,0xfa,0x44,0x00,0x34,0xfa,0x44,0x00,0x3a,0xfa,0x44,0x00};
static const uint8_t mi_rom_0044faa8[]={0xf2,0xf8,0x44,0x00,0xf8,0xf8,0x44,0x00,0xfe,0xf8,0x44,0x00,0x04,0xf9,0x44,0x00,0x0a,0xf9,0x44,0x00,0x10,0xf9,0x44,0x00,0x16,0xf9,0x44,0x00};
static const uint8_t mi_rom_0044fac4[]={0x31,0xf9,0x44,0x00,0x37,0xf9,0x44,0x00,0x3d,0xf9,0x44,0x00,0x43,0xf9,0x44,0x00,0x49,0xf9,0x44,0x00,0x4f,0xf9,0x44,0x00,0x55,0xf9,0x44,0x00};
static const uint8_t mi_rom_0044fae0[]={0x70,0xf9,0x44,0x00,0x76,0xf9,0x44,0x00,0x82,0xf9,0x44,0x00,0x7c,0xf9,0x44,0x00};
static const uint8_t mi_rom_0044faf0[]={0xe7,0xf9,0x44,0x00,0xed,0xf9,0x44,0x00,0xf3,0xf9,0x44,0x00,0xf9,0xf9,0x44,0x00,0xff,0xf9,0x44,0x00};
static const uint8_t mi_rom_0044fb04[]={0x16,0xfa,0x44,0x00,0x1c,0xfa,0x44,0x00,0x22,0xfa,0x44,0x00,0x28,0xfa,0x44,0x00};
static const uint8_t mi_rom_0044fbc0[]={0x56,0xfb,0x44,0x00,0x5c,0xfb,0x44,0x00,0x62,0xfb,0x44,0x00,0x68,0xfb,0x44,0x00,0x6e,0xfb,0x44,0x00,0x74,0xfb,0x44,0x00,0x7a,0xfb,0x44,0x00};
static const uint8_t mi_rom_0044fbdc[]={0x91,0xfb,0x44,0x00,0x97,0xfb,0x44,0x00,0x9d,0xfb,0x44,0x00,0xa3,0xfb,0x44,0x00,0xa9,0xfb,0x44,0x00,0xaf,0xfb,0x44,0x00,0xb5,0xfb,0x44,0x00};
static const uint8_t mi_rom_0044fca0[]={0x36,0xfc,0x44,0x00,0x3c,0xfc,0x44,0x00,0x42,0xfc,0x44,0x00,0x48,0xfc,0x44,0x00,0x4e,0xfc,0x44,0x00,0x54,0xfc,0x44,0x00,0x5a,0xfc,0x44,0x00};
static const uint8_t mi_rom_0044fcbc[]={0x71,0xfc,0x44,0x00,0x77,0xfc,0x44,0x00,0x7d,0xfc,0x44,0x00,0x83,0xfc,0x44,0x00,0x89,0xfc,0x44,0x00,0x8f,0xfc,0x44,0x00,0x95,0xfc,0x44,0x00};
static const uint8_t mi_rom_0044fea0[]={0x4a,0xfd,0x44,0x00,0x55,0xfd,0x44,0x00,0x60,0xfd,0x44,0x00,0x6b,0xfd,0x44,0x00};
static const uint8_t mi_rom_0044feb0[]={0x38,0xfe,0x44,0x00,0x4f,0xfe,0x44,0x00,0x66,0xfe,0x44,0x00,0x7d,0xfe,0x44,0x00};
static const uint8_t mi_rom_0045121c[]={0xed,0xfe,0x44,0x00,0x14,0xff,0x44,0x00,0x3b,0xff,0x44,0x00,0x62,0xff,0x44,0x00,0x89,0xff,0x44,0x00,0xb0,0xff,0x44,0x00,0xd7,0xff,0x44,0x00,0xfe,0xff,0x44,0x00,0x25,0x00,0x45,0x00,0x4c,0x00,0x45,0x00,0x73,0x00,0x45,0x00,0x9a,0x00,0x45,0x00,0xc1,0x00,0x45,0x00,0xe8,0x00,0x45,0x00,0x0f,0x01,0x45,0x00,0x36,0x01,0x45,0x00,0x5d,0x01,0x45,0x00,0x84,0x01,0x45,0x00,0xab,0x01,0x45,0x00,0xd2,0x01,0x45,0x00,0xf9,0x01,0x45,0x00,0x20,0x02,0x45,0x00,0x47,0x02,0x45,0x00,0x6e,0x02,0x45,0x00,0x95,0x02,0x45,0x00,0xbc,0x02,0x45,0x00,0xe3,0x02,0x45,0x00,0x0a,0x03,0x45,0x00,0x31,0x03,0x45,0x00,0x58,0x03,0x45,0x00,0x7f,0x03,0x45,0x00,0xa6,0x03,0x45,0x00,0xcd,0x03,0x45,0x00,0xf4,0x03,0x45,0x00,0x1b,0x04,0x45,0x00,0x42,0x04,0x45,0x00,0x69,0x04,0x45,0x00,0x90,0x04,0x45,0x00,0xb7,0x04,0x45,0x00,0xde,0x04,0x45,0x00,0x05,0x05,0x45,0x00,0x2c,0x05,0x45,0x00,0x53,0x05,0x45,0x00,0x7a,0x05,0x45,0x00,0xa1,0x05,0x45,0x00,0xc8,0x05,0x45,0x00,0xef,0x05,0x45,0x00,0x16,0x06,0x45,0x00,0x3d,0x06,0x45,0x00,0x64,0x06,0x45,0x00,0x8b,0x06,0x45,0x00,0xb2,0x06,0x45,0x00,0xd9,0x06,0x45,0x00,0x00,0x07,0x45,0x00,0x27,0x07,0x45,0x00,0x4e,0x07,0x45,0x00,0x75,0x07,0x45,0x00,0x9c,0x07,0x45,0x00,0xc3,0x07,0x45,0x00,0xea,0x07,0x45,0x00,0x11,0x08,0x45,0x00,0x38,0x08,0x45,0x00,0x5f,0x08,0x45,0x00,0x86,0x08,0x45,0x00,0xad,0x08,0x45,0x00,0xd4,0x08,0x45,0x00,0xfb,0x08,0x45,0x00,0x22,0x09,0x45,0x00,0x49,0x09,0x45,0x00,0x70,0x09,0x45,0x00,0x97,0x09,0x45,0x00,0xbe,0x09,0x45,0x00,0xe5,0x09,0x45,0x00,0x0c,0x0a,0x45,0x00,0x33,0x0a,0x45,0x00,0x5a,0x0a,0x45,0x00,0x81,0x0a,0x45,0x00,0xa8,0x0a,0x45,0x00,0xcf,0x0a,0x45,0x00,0xf6,0x0a,0x45,0x00,0x1d,0x0b,0x45,0x00,0x44,0x0b,0x45,0x00,0x6b,0x0b,0x45,0x00,0x92,0x0b,0x45,0x00,0xb9,0x0b,0x45,0x00,0xe0,0x0b,0x45,0x00,0x07,0x0c,0x45,0x00,0x2e,0x0c,0x45,0x00,0x55,0x0c,0x45,0x00,0x7c,0x0c,0x45,0x00,0xa3,0x0c,0x45,0x00,0xca,0x0c,0x45,0x00,0xf1,0x0c,0x45,0x00,0x18,0x0d,0x45,0x00,0x3f,0x0d,0x45,0x00,0x66,0x0d,0x45,0x00,0x8d,0x0d,0x45,0x00,0xb4,0x0d,0x45,0x00,0xdb,0x0d,0x45,0x00,0x02,0x0e,0x45,0x00,0x29,0x0e,0x45,0x00,0x50,0x0e,0x45,0x00,0x77,0x0e,0x45,0x00,0x9e,0x0e,0x45,0x00,0xc5,0x0e,0x45,0x00,0xec,0x0e,0x45,0x00,0x13,0x0f,0x45,0x00,0x3a,0x0f,0x45,0x00,0x61,0x0f,0x45,0x00,0x88,0x0f,0x45,0x00,0xaf,0x0f,0x45,0x00,0xd6,0x0f,0x45,0x00,0xfd,0x0f,0x45,0x00,0x24,0x10,0x45,0x00,0x4b,0x10,0x45,0x00,0x72,0x10,0x45,0x00,0x99,0x10,0x45,0x00,0xc0,0x10,0x45,0x00,0xe7,0x10,0x45,0x00,0x0e,0x11,0x45,0x00,0x35,0x11,0x45,0x00,0x5c,0x11,0x45,0x00,0x83,0x11,0x45,0x00,0xaa,0x11,0x45,0x00,0xcd,0x11,0x45,0x00,0xf0,0x11,0x45,0x00,0x13,0x12,0x45,0x00};
static const uint8_t mi_rom_00451418[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x7e,0x7e,0x3c,0x3d,0x7e,0x7e,0x3e,0x3f,0x7e,0x7e,0x40,0x41,0x7e,0x7e,0x42,0x43,0x7e,0x7e,0x44,0x45,0x7e,0x7e,0x46,0x47,0x7e,0x7e,0x48,0x49,0x7e,0x7e,0x4a,0x4b,0x7e,0x7e,0x4c,0x4d,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x4e,0x4f,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x50,0x51,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x52,0x53,0x7e,0x7e,0x54,0x55,0x7e,0x7e,0x56,0x57,0x7e,0x7e,0x58,0x59,0x7e,0x7e,0x5a,0x5b,0x7e,0x7e,0x5c,0x5d,0x7e,0x7e,0x5e,0x5f,0x7e,0x7e,0x60,0x61,0x7e,0x7e,0x62,0x63,0x7e,0x7e,0x64,0x65,0x7e,0x7e,0x66,0x67,0x7e,0x7e,0x68,0x69,0x7e,0x7e,0x6a,0x6b,0x7e,0x7e,0x6c,0x6d,0x7e,0x7e,0x6e,0x6f,0x7e,0x7e,0x70,0x71,0x7e,0x7e,0x72,0x73,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x74,0x75,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x7e,0x76,0x77,0x78,0x79,0x7a,0x7b,0x7c,0x7d};
static const uint8_t mi_rom_004519b0[]={0x67,0x15,0x45,0x00,0x77,0x15,0x45,0x00,0xa2,0x15,0x45,0x00,0x92,0x15,0x45,0x00};
static const uint8_t mi_rom_004519c0[]={0x4d,0x16,0x45,0x00,0x9e,0x16,0x45,0x00,0x12,0x17,0x45,0x00,0xda,0x16,0x45,0x00};
static const uint8_t mi_rom_004519d0[]={0xa8,0x19,0x45,0x00,0xc3,0x18,0x45,0x00,0x68,0x18,0x45,0x00,0x0d,0x18,0x45,0x00,0xb7,0x17,0x45,0x00,0x15,0x19,0x45,0x00};
static const uint8_t mi_rom_004519e8[]={0x00,0x01,0x05,0x05,0x05,0x05,0x02,0x05,0x05,0x05,0x05,0x05,0x03,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x04};
static const uint8_t mi_rom_00451a04[]={0xce,0x17,0x45,0x00,0xdf,0x17,0x45,0x00,0x01,0x18,0x45,0x00,0xf0,0x17,0x45,0x00};
static const uint8_t mi_rom_00451a14[]={0x24,0x18,0x45,0x00,0x35,0x18,0x45,0x00,0x57,0x18,0x45,0x00,0x46,0x18,0x45,0x00};
static const uint8_t mi_rom_00451a24[]={0x7f,0x18,0x45,0x00,0x90,0x18,0x45,0x00,0xb2,0x18,0x45,0x00,0xa1,0x18,0x45,0x00};
static const uint8_t mi_rom_00451a34[]={0xda,0x18,0x45,0x00,0xeb,0x18,0x45,0x00,0x07,0x19,0x45,0x00,0xf9,0x18,0x45,0x00};
static const uint8_t mi_rom_00451a44[]={0x3e,0x19,0x45,0x00,0x4c,0x19,0x45,0x00,0x68,0x19,0x45,0x00,0x5a,0x19,0x45,0x00};
static const uint8_t mi_rom_004eadc4[]={0xbb,0xad,0x4e,0x00,0xbe,0xad,0x4e,0x00};
static const uint8_t mi_rom_004eadcc[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0044c700u&&width<=116u&&(offset=address-0x0044c700u)<=116u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044c700[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044cd9cu&&width<=32u&&(offset=address-0x0044cd9cu)<=32u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044cd9c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d3d4u&&width<=28u&&(offset=address-0x0044d3d4u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d3d4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d3f0u&&width<=28u&&(offset=address-0x0044d3f0u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d3f0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d40cu&&width<=28u&&(offset=address-0x0044d40cu)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d40c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d428u&&width<=28u&&(offset=address-0x0044d428u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d428[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d444u&&width<=28u&&(offset=address-0x0044d444u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d444[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d460u&&width<=28u&&(offset=address-0x0044d460u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d460[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d47cu&&width<=28u&&(offset=address-0x0044d47cu)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d47c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044d498u&&width<=28u&&(offset=address-0x0044d498u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044d498[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044e5bcu&&width<=20u&&(offset=address-0x0044e5bcu)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044e5bc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044e6e0u&&width<=16u&&(offset=address-0x0044e6e0u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044e6e0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f158u&&width<=16u&&(offset=address-0x0044f158u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f158[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f2acu&&width<=92u&&(offset=address-0x0044f2acu)<=92u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f2ac[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f308u&&width<=28u&&(offset=address-0x0044f308u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f308[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f324u&&width<=16u&&(offset=address-0x0044f324u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f324[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f334u&&width<=20u&&(offset=address-0x0044f334u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f334[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f348u&&width<=16u&&(offset=address-0x0044f348u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f348[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f3b4u&&width<=28u&&(offset=address-0x0044f3b4u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f3b4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f424u&&width<=28u&&(offset=address-0x0044f424u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f424[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f5acu&&width<=92u&&(offset=address-0x0044f5acu)<=92u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f5ac[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f608u&&width<=28u&&(offset=address-0x0044f608u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f608[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f624u&&width<=16u&&(offset=address-0x0044f624u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f624[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f634u&&width<=20u&&(offset=address-0x0044f634u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f634[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f648u&&width<=16u&&(offset=address-0x0044f648u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f648[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f7ecu&&width<=92u&&(offset=address-0x0044f7ecu)<=92u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f7ec[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f848u&&width<=28u&&(offset=address-0x0044f848u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f848[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f864u&&width<=28u&&(offset=address-0x0044f864u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f864[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f880u&&width<=16u&&(offset=address-0x0044f880u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f880[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f890u&&width<=20u&&(offset=address-0x0044f890u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f890[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044f8a4u&&width<=16u&&(offset=address-0x0044f8a4u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044f8a4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fa4cu&&width<=92u&&(offset=address-0x0044fa4cu)<=92u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fa4c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044faa8u&&width<=28u&&(offset=address-0x0044faa8u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044faa8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fac4u&&width<=28u&&(offset=address-0x0044fac4u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fac4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fae0u&&width<=16u&&(offset=address-0x0044fae0u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fae0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044faf0u&&width<=20u&&(offset=address-0x0044faf0u)<=20u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044faf0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fb04u&&width<=16u&&(offset=address-0x0044fb04u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fb04[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fbc0u&&width<=28u&&(offset=address-0x0044fbc0u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fbc0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fbdcu&&width<=28u&&(offset=address-0x0044fbdcu)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fbdc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fca0u&&width<=28u&&(offset=address-0x0044fca0u)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fca0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fcbcu&&width<=28u&&(offset=address-0x0044fcbcu)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fcbc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044fea0u&&width<=16u&&(offset=address-0x0044fea0u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044fea0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044feb0u&&width<=16u&&(offset=address-0x0044feb0u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044feb0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0045121cu&&width<=508u&&(offset=address-0x0045121cu)<=508u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045121c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00451418u&&width<=286u&&(offset=address-0x00451418u)<=286u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00451418[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004519b0u&&width<=16u&&(offset=address-0x004519b0u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004519b0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004519c0u&&width<=16u&&(offset=address-0x004519c0u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004519c0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004519d0u&&width<=24u&&(offset=address-0x004519d0u)<=24u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004519d0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004519e8u&&width<=25u&&(offset=address-0x004519e8u)<=25u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004519e8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00451a04u&&width<=16u&&(offset=address-0x00451a04u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00451a04[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00451a14u&&width<=16u&&(offset=address-0x00451a14u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00451a14[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00451a24u&&width<=16u&&(offset=address-0x00451a24u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00451a24[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00451a34u&&width<=16u&&(offset=address-0x00451a34u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00451a34[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00451a44u&&width<=16u&&(offset=address-0x00451a44u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00451a44[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004eadc4u&&width<=8u&&(offset=address-0x004eadc4u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004eadc4[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004eadccu&&width<=77u&&(offset=address-0x004eadccu)<=77u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004eadcc[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_event_C_00446260(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00446260 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446261 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446262 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 00446266 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00446268 */
    if(mi_signed_less(f->flags))goto mi_00446282;
    /* 0044626a */
    a=f->r[7];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00446270 */
    if(!mi_signed_less(f->flags))goto mi_00446282;
    /* 00446272 */
    f->r[0]=f->r[7];
    /* 00446274 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00446277 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00446279 */
    f->r[6]=(0x00785d74u+f->r[0]*4u);
    /* 00446280 */
    goto mi_00446284;
mi_00446282:;
    /* 00446282 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_00446284:;
    /* 00446284 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446286 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446288 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446289 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044628eu;f->boundary(0x00446289u,0x004b8360u,f);
    /* 0044628e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446291 */
    f->r[1]=0x00000001u;
    /* 00446296 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00446298 */
    if((f->flags&MI_ZF))goto mi_0044629e;
    /* 0044629a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
mi_0044629e:;
    /* 0044629e */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004462a4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004462a6 */
    if(!mi_signed_less(f->flags))goto mi_004462d2;
    /* 004462a8 */
    f->r[6]=f->r[2];
    /* 004462aa */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004462ab */
    a=f->r[6];b=0x000049d8u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 004462b1 */
    f->r[3]=MI_U32((0x00ea61fcu+f->r[6]));
    /* 004462b7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004462b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004462ba */
    if(!(f->flags&MI_ZF))goto mi_004462d2;
    /* 004462bc */
    f->r[0]=0x00000064u;
    /* 004462c1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004462c2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004462c3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004462c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004462cau;f->boundary(0x004462c5u,0x004b8890u,f);
    /* 004462ca */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004462cd */
    f->r[0]=f->r[6];
    /* 004462cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004462d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004462d1 */
    f->r[4]+=4;return;
mi_004462d2:;
    /* 004462d2 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004462d8 */
    f->r[6]=MI_U32((0x00ea61fcu+f->r[2]));
    /* 004462de */
    f->r[2]=(0x00ea61fcu+f->r[2]);
    /* 004462e4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004462e6 */
    if((f->flags&MI_ZF))goto mi_00446304;
    /* 004462e8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004462ea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004462eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004462ec */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 004462f2 */
    f->r[6]=0x00000001u;
    /* 004462f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004462fcu;f->boundary(0x004462f7u,0x004b8890u,f);
    /* 004462fc */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004462ff */
    f->r[0]=f->r[6];
    /* 00446301 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00446302 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446303 */
    f->r[4]+=4;return;
mi_00446304:;
    /* 00446304 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446305 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00446307 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446308 */
    f->r[6]=0x00000001u;
    /* 0044630d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446312u;f->boundary(0x0044630du,0x004cde60u,f);
    /* 00446312 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446315 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00446318 */
    if(!mi_signed_less(f->flags))goto mi_0044632e;
    /* 0044631a */
    f->r[0]=0x00000046u;
    /* 0044631f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446320 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446321 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446326u;f->boundary(0x00446321u,0x004b8890u,f);
    /* 00446326 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446329 */
    f->r[0]=f->r[6];
    /* 0044632b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044632c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044632d */
    f->r[4]+=4;return;
mi_0044632e:;
    /* 0044632e */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 00446331 */
    if(!mi_signed_less(f->flags))goto mi_00446347;
    /* 00446333 */
    f->r[0]=0x00000032u;
    /* 00446338 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446339 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044633a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044633fu;f->boundary(0x0044633au,0x004b8890u,f);
    /* 0044633f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446342 */
    f->r[0]=f->r[6];
    /* 00446344 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00446345 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446346 */
    f->r[4]+=4;return;
mi_00446347:;
    /* 00446347 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 0044634a */
    if(!mi_signed_less(f->flags))goto mi_00446360;
    /* 0044634c */
    f->r[0]=0x00000019u;
    /* 00446351 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446352 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446353 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446358u;f->boundary(0x00446353u,0x004b8890u,f);
    /* 00446358 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044635b */
    f->r[0]=f->r[6];
    /* 0044635d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044635e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044635f */
    f->r[4]+=4;return;
mi_00446360:;
    /* 00446360 */
    a=f->r[0];b=0x0000004bu;t=mi_alu(f,a,b,32,1);
    /* 00446363 */
    if(!mi_signed_less(f->flags))goto mi_00446379;
    /* 00446365 */
    f->r[0]=0x0000000fu;
    /* 0044636a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044636b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044636c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00446371u;f->boundary(0x0044636cu,0x004b8890u,f);
    /* 00446371 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00446374 */
    f->r[0]=f->r[6];
    /* 00446376 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00446377 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446378 */
    f->r[4]+=4;return;
mi_00446379:;
    /* 00446379 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0044637c */
    if(!mi_signed_less(f->flags))goto mi_00446392;
    /* 0044637e */
    f->r[0]=0x0000000au;
    /* 00446383 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446384 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00446385 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044638au;f->boundary(0x00446385u,0x004b8890u,f);
    /* 0044638a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044638d */
    f->r[0]=f->r[6];
    /* 0044638f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00446390 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00446391 */
    f->r[4]+=4;return;
mi_00446392:;
    /* 00446392 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00446394 */
    a=f->r[0];b=0x00000096u;t=mi_alu(f,a,b,32,1);
    /* 00446399 */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044639c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044639d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044639e */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004463a1 */
    f->r[0]=f->r[1];
    /* 004463a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004463a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004463a9u;f->boundary(0x004463a4u,0x004b8890u,f);
    /* 004463a9 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004463ac */
    f->r[0]=f->r[6];
    /* 004463ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004463af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004463b0 */
    f->r[4]+=4;return;
}

void restore_event_C_004487f0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004487f0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004487f1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004487f2 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00000002u+f->r[1])))&0xffffu)<<0);
    /* 004487f6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004487f7 */
    a=(f->r[6]&0xffffu);b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004487fa */
    if(mi_signed_less(f->flags))goto mi_00448809;
    /* 004487fc */
    a=MI_U16((0x00000004u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00448801 */
    if(mi_signed_less(f->flags))goto mi_00448955;
    /* 00448807 */
    goto mi_0044882d;
mi_00448809:;
    /* 00448809 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[1])))&0xffffu)<<0);
    /* 0044880d */
    a=(f->r[2]&0xffffu);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00448810 */
    if(mi_signed_less(f->flags))goto mi_0044882d;
    /* 00448812 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000006u+f->r[1])))&0xffffu)<<0);
    /* 00448816 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00448819 */
    if(mi_signed_less(f->flags))goto mi_0044882d;
    /* 0044881b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0044881e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 00448821 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00448823 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00448824 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00448827 */
    if((f->flags&MI_ZF))goto mi_00448955;
mi_0044882d:;
    /* 0044882d */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 00448832 */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 00448838 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0044883f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00448840 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00448843 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 00448848 */
    if(mi_signed_less(f->flags))goto mi_00448854;
    /* 0044884a */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_00448854:;
    /* 00448854 */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 0044885a */
    f->r[2]=MI_U32((0x00d3b380u));
    /* 00448860 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00448861 */
    f->r[3]=0x0000001fu;
mi_00448866:;
    /* 00448866 */
    f->r[0]=f->r[6];
    /* 00448868 */
    f->r[7]=f->r[6];
    /* 0044886a */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0044886d */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00448870 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 00448873 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00448875 */
    f->r[7]=f->r[2];
    /* 00448877 */
    a=f->r[7];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[7]=t;
    /* 0044887a */
    f->r[5]=f->r[2];
    /* 0044887c */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 0044887f */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 00448882 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00448884 */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 00448886 */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 00448888 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044888a */
    if((f->flags&MI_ZF))goto mi_00448892;
    /* 0044888c */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_00448892:;
    /* 00448892 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00448894 */
    if((f->flags&MI_ZF))goto mi_0044889c;
    /* 00448896 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_0044889c:;
    /* 0044889c */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0044889d */
    if(!(f->flags&MI_ZF))goto mi_00448866;
    /* 0044889f */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004488a5 */
    f->r[0]=f->r[6];
    /* 004488a7 */
    f->r[7]=f->r[6];
    /* 004488a9 */
    a=f->r[6];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004488ac */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[6]);
    /* 004488b2 */
    f->r[6]=f->r[2];
    /* 004488b4 */
    a=f->r[6];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004488ba */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[2]);
    /* 004488c0 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[6]);
    /* 004488c6 */
    f->r[6]=f->r[2];
    /* 004488c8 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004488cd */
    a=f->r[7];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004488d0 */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004488d3 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004488d6 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004488db */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[7]);
    /* 004488e1 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[6]);
    /* 004488e7 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[2]);
    /* 004488ed */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00000002u+f->r[1])))&0xffffu)<<0);
    /* 004488f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004488f2 */
    a=(f->r[6]&0xffffu);b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004488f5 */
    if(!mi_signed_less(f->flags))goto mi_00448915;
    /* 004488f7 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[1])))&0xffffu)<<0);
    /* 004488fb */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00448900 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[1]));
    /* 00448904 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 00448907 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00448909 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044890a */
    a=f->r[7];mi_idiv32(f,a);
    /* 0044890c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044890d */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044890f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00448910 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00448913 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00448914 */
    f->r[4]+=4;return;
mi_00448915:;
    /* 00448915 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[1])))&0xffffu)<<0);
    /* 00448919 */
    a=(f->r[5]&0xffffu);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044891c */
    if(!mi_signed_less(f->flags))goto mi_00448925;
    /* 0044891e */
    f->r[7]=0x00000001u;
    /* 00448923 */
    goto mi_00448933;
mi_00448925:;
    /* 00448925 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00448927 */
    a=MI_U16((0x00000006u+f->r[1]));b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044892b */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044892e */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00448931 */
    f->r[7]=f->r[2];
mi_00448933:;
    /* 00448933 */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00448938 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00448939 */
    a=f->r[7];mi_idiv32(f,a);
    /* 0044893b */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044893e */
    if((f->flags&MI_ZF))goto mi_00448955;
    /* 00448940 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00448941 */
    if((f->flags&MI_ZF))goto mi_0044894e;
    /* 00448943 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000006u+f->r[1])))&0xffffu)<<0);
    /* 00448947 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00448948 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00448949 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044894c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044894d */
    f->r[4]+=4;return;
mi_0044894e:;
    /* 0044894e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044894f */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 00448952 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00448953 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00448954 */
    f->r[4]+=4;return;
mi_00448955:;
    /* 00448955 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[6]&0xffffu));
    /* 00448958 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00448959 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044895a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044895b */
    f->r[4]+=4;return;
}

void restore_event_C_00448a90(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00448a90 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00448a91 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00448a92 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00448a93 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00448a94 */
    f->r[6]=f->r[1];
    /* 00448a96 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00448a97 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 00448a9a */
    f->r[1]=(0x00000004u+f->r[6]);
    /* 00448a9d */
    a=f->r[2];b=0xfffffffbu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00448aa0 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 00448aa3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00448aa8u;f->boundary(0x00448aa3u,0x004487f0u,f);
    /* 00448aa8 */
    f->r[1]=(0x0000000cu+f->r[6]);
    /* 00448aab */
    f->r[4]-=4;MI_U32(f->r[4])=0x00448ab0u;f->boundary(0x00448aabu,0x004487f0u,f);
    /* 00448ab0 */
    f->r[7]=(0x00000014u+f->r[6]);
    /* 00448ab3 */
    f->r[1]=f->r[7];
    /* 00448ab5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00448abau;f->boundary(0x00448ab5u,0x004487f0u,f);
    /* 00448aba */
    f->r[1]=(0x0000001cu+f->r[6]);
    /* 00448abd */
    f->r[4]-=4;MI_U32(f->r[4])=0x00448ac2u;f->boundary(0x00448abdu,0x004487f0u,f);
    /* 00448ac2 */
    f->r[1]=(0x00000058u+f->r[6]);
    /* 00448ac5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00448acau;f->boundary(0x00448ac5u,0x004487f0u,f);
    /* 00448aca */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U8((0x00000003u+f->r[6])))&0xffffu)<<0);
    /* 00448acf */
    a=(f->r[0]&0xffffu);b=MI_U16((0x00000000u+f->r[7]));t=mi_alu(f,a,b,16,0);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 00448ad2 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00448ad4 */
    MI_U8((0x00000057u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00448ad7 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00448adb */
    MI_U16((0x0000002eu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00448adf */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 00448ae2 */
    MI_U16((0x00000034u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00448ae6 */
    MI_U16((0x00000064u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00448aea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[6])))&0xffu)<<0);
    /* 00448aed */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00448af0 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00448af2 */
    MI_U8((0x00000033u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00448af5 */
    MI_U8((0x00000063u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 00448af8 */
    if(!(f->flags&MI_ZF))goto mi_00448b1d;
    /* 00448afa */
    f->r[0]=(0x00000042u+f->r[6]);
    /* 00448afd */
    f->r[1]=0x00000004u;
mi_00448b02:;
    /* 00448b02 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00784c60u)))&0xffffu)<<0);
    /* 00448b09 */
    MI_U16((0xfffffffeu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00448b0d */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00784c62u)))&0xffffu)<<0);
    /* 00448b14 */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00448b17 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00448b1a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00448b1b */
    if(!(f->flags&MI_ZF))goto mi_00448b02;
mi_00448b1d:;
    /* 00448b1d */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 00448b22 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 00448b28 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 00448b2f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00448b30 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00448b33 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 00448b38 */
    if(mi_signed_less(f->flags))goto mi_00448b40;
    /* 00448b3a */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[3]);
mi_00448b40:;
    /* 00448b40 */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 00448b46 */
    f->r[7]=MI_U32((0x00d3b380u));
    /* 00448b4c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_00448b54:;
    /* 00448b54 */
    f->r[0]=f->r[1];
    /* 00448b56 */
    f->r[2]=f->r[1];
    /* 00448b58 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 00448b5b */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00448b5e */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00448b61 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00448b63 */
    f->r[2]=f->r[7];
    /* 00448b65 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 00448b68 */
    f->r[5]=f->r[7];
    /* 00448b6a */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00448b6d */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 00448b70 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00448b72 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 00448b74 */
    a=f->r[7];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[7]=t;
    /* 00448b76 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00448b78 */
    if((f->flags&MI_ZF))goto mi_00448b80;
    /* 00448b7a */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_00448b80:;
    /* 00448b80 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00448b82 */
    if((f->flags&MI_ZF))goto mi_00448b8a;
    /* 00448b84 */
    a=f->r[7];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
mi_00448b8a:;
    /* 00448b8a */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00448b8e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00448b8f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00448b93 */
    if(!(f->flags&MI_ZF))goto mi_00448b54;
    /* 00448b95 */
    f->r[0]=f->r[1];
    /* 00448b97 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 00448b9d */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00448ba2 */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00448ba5 */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00448baa */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[1]);
    /* 00448bb0 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00448bb1 */
    f->r[1]=0x00000005u;
    /* 00448bb6 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[7]);
    /* 00448bbc */
    a=f->r[1];mi_idiv32(f,a);
    /* 00448bbe */
    a=(f->r[2]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00448bc1 */
    MI_U8((0x00000061u+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 00448bc4 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 00448bc9 */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 00448bcf */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 00448bd6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00448bd7 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00448bda */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 00448bdf */
    if(mi_signed_less(f->flags))goto mi_00448be7;
    /* 00448be1 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[3]);
mi_00448be7:;
    /* 00448be7 */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 00448bed */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 00448bf2 */
    f->r[5]=0x0000001fu;
mi_00448bf7:;
    /* 00448bf7 */
    f->r[2]=f->r[1];
    /* 00448bf9 */
    f->r[7]=f->r[1];
    /* 00448bfb */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 00448bfe */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00448c01 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 00448c04 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00448c06 */
    f->r[7]=f->r[0];
    /* 00448c08 */
    a=f->r[7];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[7]=t;
    /* 00448c0b */
    f->r[3]=f->r[0];
    /* 00448c0d */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 00448c10 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 00448c13 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00448c15 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 00448c17 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 00448c19 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00448c1b */
    if((f->flags&MI_ZF))goto mi_00448c23;
    /* 00448c1d */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_00448c23:;
    /* 00448c23 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00448c25 */
    if((f->flags&MI_ZF))goto mi_00448c2c;
    /* 00448c27 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_00448c2c:;
    /* 00448c2c */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00448c2d */
    if(!(f->flags&MI_ZF))goto mi_00448bf7;
    /* 00448c2f */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 00448c35 */
    f->r[2]=f->r[1];
    /* 00448c37 */
    f->r[7]=f->r[1];
    /* 00448c39 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00448c3c */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 00448c42 */
    f->r[1]=f->r[0];
    /* 00448c44 */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00448c4a */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00448c50 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 00448c56 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 00448c5c */
    f->r[1]=f->r[0];
    /* 00448c5e */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 00448c63 */
    a=f->r[2];b=0x0000ffffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00448c69 */
    a=f->r[7];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 00448c6c */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 00448c6f */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00448c72 */
    a=f->r[2];b=0x80000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00448c78 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[7]);
    /* 00448c7e */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[1]);
    /* 00448c84 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 00448c89 */
    if(!(f->flags&MI_SF))goto mi_00448c90;
    /* 00448c8b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00448c8c */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 00448c8f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_00448c90:;
    /* 00448c90 */
    MI_U8((0x00000062u+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 00448c93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00448c94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00448c95 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00448c96 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00448c97 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00448c98 */
    f->r[4]+=4;return;
}

void restore_event_C_004496d0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004496d0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004496d1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004496d2 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 004496d6 */
    f->r[6]=f->r[1];
    /* 004496d8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004496d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004496deu;f->boundary(0x004496d9u,0x0044c0d0u,f);
    /* 004496de */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004496e0 */
    if(!(f->flags&MI_ZF))goto mi_004496e9;
    /* 004496e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004496e3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004496e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004496e6 */
    f->r[4]+=8;return;
mi_004496e9:;
    /* 004496e9 */
    a=MI_U8((0x00000028u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 004496ed */
    if(!(f->flags&MI_ZF))goto mi_004496f6;
    /* 004496ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004496f0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004496f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004496f3 */
    f->r[4]+=8;return;
mi_004496f6:;
    /* 004496f6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 004496f9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004496fb */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004496fd */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449700 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00449701 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[1]&0xffu))&0xffu)<<0);
    /* 00449703 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00449704 */
    f->r[4]+=8;return;
}

void restore_event_C_0044c0d0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c0d0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0044c0d4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044c0d6 */
    if(mi_signed_less(f->flags))goto mi_0044c0f1;
    /* 0044c0d8 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044c0de */
    if(!mi_signed_less(f->flags))goto mi_0044c0f1;
    /* 0044c0e0 */
    f->r[1]=f->r[0];
    /* 0044c0e2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044c0e5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044c0e7 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044c0ee */
    f->r[4]+=8;return;
mi_0044c0f1:;
    /* 0044c0f1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044c0f3 */
    f->r[4]+=8;return;
}

void restore_event_C_0044c430(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c430 */
    f->r[2]=MI_U32((0x005d9a40u));
    /* 0044c436 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0044c438 */
    if(!mi_signed_less(f->flags))goto mi_0044c43f;
    /* 0044c43a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c43c */
    f->r[4]+=8;return;
mi_0044c43f:;
    /* 0044c43f */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0044c443 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044c445 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044c44c;
    /* 0044c447 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c449 */
    f->r[4]+=8;return;
mi_0044c44c:;
    /* 0044c44c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044c44e */
    if(mi_signed_less(f->flags))goto mi_0044c463;
    /* 0044c450 */
    a=f->r[0];b=MI_U32((0x00000000u+f->r[1]));t=mi_alu(f,a,b,32,1);
    /* 0044c452 */
    if(!mi_signed_less(f->flags))goto mi_0044c463;
    /* 0044c454 */
    f->r[2]=f->r[0];
    /* 0044c456 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0044c459 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044c45b */
    f->r[0]=(0x00000004u+f->r[1]+f->r[2]*4u);
    /* 0044c45f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044c461 */
    if(!(f->flags&MI_ZF))goto mi_0044c468;
mi_0044c463:;
    /* 0044c463 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c465 */
    f->r[4]+=8;return;
mi_0044c468:;
    /* 0044c468 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[0])))&0xffu)<<0);
    /* 0044c46b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0078a634u)))&0xffu)<<0);
    /* 0044c470 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0044c473 */
    if(!(f->flags&MI_ZF))goto mi_0044c47d;
    /* 0044c475 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c477 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c47a */
    f->r[4]+=8;return;
mi_0044c47d:;
    /* 0044c47d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c47f */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c482 */
    f->r[4]+=8;return;
}

void restore_event_C_0044c490(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c490 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0044c494 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c495 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c496 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c497 */
    f->r[5]=f->r[1];
    /* 0044c499 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c49a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c49fu;f->boundary(0x0044c49au,0x0044c0d0u,f);
    /* 0044c49f */
    f->r[6]=f->r[0];
    /* 0044c4a1 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044c4a3 */
    if((f->flags&MI_ZF))goto mi_0044c657;
    /* 0044c4a9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c4ab */
    f->r[1]=0x00785d70u;
    /* 0044c4b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c4b5u;f->boundary(0x0044c4b0u,0x00448d10u,f);
    /* 0044c4b5 */
    f->r[3]=f->r[0];
    /* 0044c4b7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0044c4b9 */
    if((f->flags&MI_ZF))goto mi_0044c657;
    /* 0044c4bf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[3]));
    /* 0044c4c2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c4c3 */
    f->r[1]=0x0000001fu;
    /* 0044c4c8 */
    f->r[7]=f->r[3];
    /* 0044c4ca */
    while(f->r[1]){a=MI_U32(f->r[6]);MI_U32(f->r[7])=a;f->r[6]+=(f->flags&0x400u)?0xfffffffcu:4u;f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 0044c4cc */
    f->r[1]=f->r[3];
    /* 0044c4ce */
    MI_U16((0x00000000u+f->r[3]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044c4d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c4d6u;f->boundary(0x0044c4d1u,0x00448a90u,f);
    /* 0044c4d6 */
    f->r[1]=f->r[3];
    /* 0044c4d8 */
    MI_U32((0x00000028u+f->r[3]))=(uint32_t)(0x00000008u);
    /* 0044c4df */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c4e4u;f->boundary(0x0044c4dfu,0x00449390u,f);
    /* 0044c4e4 */
    f->r[2]=(f->r[2]&0xffffff00u)|((((f->r[0]&0xffu))&0xffu)<<0);
    /* 0044c4e6 */
    f->r[1]=f->r[5];
    /* 0044c4e8 */
    MI_U8((0x00000014u+f->r[4]))=(uint8_t)((f->r[2]&0xffu));
    /* 0044c4ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c4f1u;f->boundary(0x0044c4ecu,0x00449390u,f);
    /* 0044c4f1 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000014u+f->r[4])))&0xffu)<<0);
    /* 0044c4f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044c4f6 */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0044c4f8 */
    if(!(f->flags&MI_ZF))goto mi_0044c539;
    /* 0044c4fa */
    a=MI_U16((0x0000001eu+f->r[3]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044c4ff */
    f->r[6]=(0x0000001cu+f->r[3]);
    /* 0044c502 */
    if(!mi_signed_less(f->flags))goto mi_0044c50e;
    /* 0044c504 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044c508 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000006u+f->r[6])))&0xffffu)<<0);
    /* 0044c50c */
    goto mi_0044c528;
mi_0044c50e:;
    /* 0044c50e */
    f->r[1]=f->r[6];
    /* 0044c510 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c515u;f->boundary(0x0044c510u,0x0044c960u,f);
    /* 0044c515 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0044c518 */
    if(!(f->flags&MI_ZF))goto mi_0044c522;
    /* 0044c51a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000002u+f->r[6])))&0xffffu)<<0);
    /* 0044c51e */
    f->r[2]=f->r[0];
    /* 0044c520 */
    goto mi_0044c528;
mi_0044c522:;
    /* 0044c522 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044c525 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0044c528:;
    /* 0044c528 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000001cu+f->r[5])))&0xffffu)<<0);
    /* 0044c52c */
    a=(f->r[1]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044c52f */
    if(mi_signed_less(f->flags))goto mi_0044c539;
    /* 0044c531 */
    a=(f->r[1]&0xffffu);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044c534 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044c539;
    /* 0044c536 */
    MI_U16((0x00000000u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_0044c539:;
    /* 0044c539 */
    f->r[1]=f->r[3];
    /* 0044c53b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c540u;f->boundary(0x0044c53bu,0x004493c0u,f);
    /* 0044c540 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c542 */
    if((f->flags&MI_ZF))goto mi_0044c59e;
    /* 0044c544 */
    f->r[1]=f->r[5];
    /* 0044c546 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c54bu;f->boundary(0x0044c546u,0x004493c0u,f);
    /* 0044c54b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c54d */
    if((f->flags&MI_ZF))goto mi_0044c59e;
    /* 0044c54f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 0044c553 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c554 */
    f->r[1]=f->r[3];
    /* 0044c556 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c55bu;f->boundary(0x0044c556u,0x00449550u,f);
    /* 0044c55b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c55d */
    if((f->flags&MI_ZF))goto mi_0044c59e;
    /* 0044c55f */
    a=MI_U16((0x00000006u+f->r[3]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044c564 */
    f->r[6]=(0x00000004u+f->r[3]);
    /* 0044c567 */
    if(!mi_signed_less(f->flags))goto mi_0044c573;
    /* 0044c569 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044c56d */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000006u+f->r[6])))&0xffffu)<<0);
    /* 0044c571 */
    goto mi_0044c58d;
mi_0044c573:;
    /* 0044c573 */
    f->r[1]=f->r[6];
    /* 0044c575 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c57au;f->boundary(0x0044c575u,0x0044c960u,f);
    /* 0044c57a */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0044c57d */
    if(!(f->flags&MI_ZF))goto mi_0044c587;
    /* 0044c57f */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000002u+f->r[6])))&0xffffu)<<0);
    /* 0044c583 */
    f->r[2]=f->r[0];
    /* 0044c585 */
    goto mi_0044c58d;
mi_0044c587:;
    /* 0044c587 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044c58a */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0044c58d:;
    /* 0044c58d */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[5])))&0xffffu)<<0);
    /* 0044c591 */
    a=(f->r[1]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044c594 */
    if(mi_signed_less(f->flags))goto mi_0044c59e;
    /* 0044c596 */
    a=(f->r[1]&0xffffu);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044c599 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044c59e;
    /* 0044c59b */
    MI_U16((0x00000000u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_0044c59e:;
    /* 0044c59e */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000014u+f->r[4])))&0xffffu)<<0);
    /* 0044c5a3 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[5])))&0xffffu)<<0);
    /* 0044c5a7 */
    MI_U16((0x00000064u+f->r[3]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044c5ab */
    f->r[1]=f->r[5];
    /* 0044c5ad */
    MI_U16((0x00000034u+f->r[3]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044c5b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c5b6u;f->boundary(0x0044c5b1u,0x00449390u,f);
    /* 0044c5b6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c5b8 */
    if((f->flags&MI_ZF))goto mi_0044c5c2;
    /* 0044c5ba */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0000001cu+f->r[5])))&0xffu)<<0);
    /* 0044c5bd */
    MI_U8((0x00000063u+f->r[3]))=(uint8_t)((f->r[2]&0xffu));
    /* 0044c5c0 */
    goto mi_0044c5c6;
mi_0044c5c2:;
    /* 0044c5c2 */
    MI_U8((0x00000063u+f->r[3]))=(uint8_t)(0x00000000u);
mi_0044c5c6:;
    /* 0044c5c6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[5])))&0xffffu)<<0);
    /* 0044c5ca */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000014u+f->r[3]));
    /* 0044c5ce */
    MI_U16((0x00000068u+f->r[3]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044c5d2 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[5])))&0xffffu)<<0);
    /* 0044c5d6 */
    MI_U16((0x00000066u+f->r[3]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044c5da */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000036u+f->r[5])))&0xffu)<<0);
    /* 0044c5dd */
    MI_U8((0x00000037u+f->r[3]))=(uint8_t)((f->r[2]&0xffu));
    /* 0044c5e0 */
    f->r[0]=MI_U32((0x0078d7c0u));
    /* 0044c5e5 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044c5e7 */
    f->r[1]=0x0000000cu;
    /* 0044c5ec */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044c5ed */
    a=f->r[1];mi_idiv32(f,a);
    /* 0044c5ef */
    f->r[0]=0x2aaaaaabu;
    /* 0044c5f4 */
    f->r[1]=f->r[2];
    /* 0044c5f6 */
    a=f->r[0];b=f->r[6];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0044c5f8 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0044c5fa */
    f->r[0]=f->r[2];
    /* 0044c5fc */
    MI_U8((0x00000003u+f->r[3]))=(uint8_t)((f->r[1]&0xffu));
    /* 0044c5ff */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0044c602 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044c604 */
    f->r[0]=MI_U32((0x0078d7c0u));
    /* 0044c609 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044c60f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044c611 */
    if(!mi_signed_less(f->flags))goto mi_0044c614;
    /* 0044c613 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_0044c614:;
    /* 0044c614 */
    f->r[1]=MI_U32((0x0078d7bcu));
    /* 0044c61a */
    f->r[5]=MI_U32((0x007848d8u));
    /* 0044c620 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044c622 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044c624 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044c626 */
    MI_U16((0x00000014u+f->r[3]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044c62a */
    if(!(f->flags&MI_ZF))goto mi_0044c657;
    /* 0044c62c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[3]));
    /* 0044c62f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c630 */
    f->r[1]=0x00785d70u;
    /* 0044c635 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c63au;f->boundary(0x0044c635u,0x0044c430u,f);
    /* 0044c63a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c63c */
    if(!(f->flags&MI_ZF))goto mi_0044c657;
    /* 0044c63e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[3])))&0xffu)<<0);
    /* 0044c641 */
    f->r[1]=f->r[3];
    /* 0044c643 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0044c645 */
    if(!(f->flags&MI_ZF))goto mi_0044c652;
    /* 0044c647 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c64cu;f->boundary(0x0044c647u,0x00451540u,f);
    /* 0044c64c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c64d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044c64e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044c64f */
    f->r[4]+=12;return;
mi_0044c652:;
    /* 0044c652 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c657u;f->boundary(0x0044c652u,0x0044c660u,f);
mi_0044c657:;
    /* 0044c657 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c658 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044c659 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044c65a */
    f->r[4]+=12;return;
}

void restore_event_C_0044c660(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c660 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044c662 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044c665 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044c666 */
    a=f->r[0];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0044c669 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044c6d3;
    /* 0044c66b */
    switch(mi_read((0x0044c700u+f->r[0]*4u),4u)){case 0x0044c672u:goto mi_0044c672;case 0x0044c677u:goto mi_0044c677;case 0x0044c67cu:goto mi_0044c67c;case 0x0044c681u:goto mi_0044c681;case 0x0044c686u:goto mi_0044c686;case 0x0044c68bu:goto mi_0044c68b;case 0x0044c690u:goto mi_0044c690;case 0x0044c695u:goto mi_0044c695;case 0x0044c69au:goto mi_0044c69a;case 0x0044c69fu:goto mi_0044c69f;case 0x0044c6a4u:goto mi_0044c6a4;case 0x0044c6a9u:goto mi_0044c6a9;case 0x0044c6aeu:goto mi_0044c6ae;case 0x0044c6b3u:goto mi_0044c6b3;case 0x0044c6b8u:goto mi_0044c6b8;case 0x0044c6bdu:goto mi_0044c6bd;case 0x0044c6c2u:goto mi_0044c6c2;case 0x0044c6c7u:goto mi_0044c6c7;case 0x0044c6ccu:goto mi_0044c6cc;case 0x0044c6d3u:goto mi_0044c6d3;case 0x0044c6d4u:goto mi_0044c6d4;case 0x0044c6dcu:goto mi_0044c6dc;case 0x0044c6e1u:goto mi_0044c6e1;case 0x0044c6e6u:goto mi_0044c6e6;case 0x0044c6ebu:goto mi_0044c6eb;case 0x0044c6f0u:goto mi_0044c6f0;case 0x0044c6f5u:goto mi_0044c6f5;case 0x0044c6fau:goto mi_0044c6fa;default:abort();}
mi_0044c672:;
    /* 0044c672 */
    f->boundary(0x0044c672u,0x0044cdc0u,f);return;
mi_0044c677:;
    /* 0044c677 */
    f->boundary(0x0044c677u,0x0044c7e0u,f);return;
mi_0044c67c:;
    /* 0044c67c */
    f->boundary(0x0044c67cu,0x0044c8a0u,f);return;
mi_0044c681:;
    /* 0044c681 */
    f->boundary(0x0044c681u,0x0044d780u,f);return;
mi_0044c686:;
    /* 0044c686 */
    f->boundary(0x0044c686u,0x0044d870u,f);return;
mi_0044c68b:;
    /* 0044c68b */
    f->boundary(0x0044c68bu,0x0051f780u,f);return;
mi_0044c690:;
    /* 0044c690 */
    f->boundary(0x0044c690u,0x0051f780u,f);return;
mi_0044c695:;
    /* 0044c695 */
    f->boundary(0x0044c695u,0x0044db20u,f);return;
mi_0044c69a:;
    /* 0044c69a */
    f->boundary(0x0044c69au,0x0044dbd0u,f);return;
mi_0044c69f:;
    /* 0044c69f */
    f->boundary(0x0044c69fu,0x0044dca0u,f);return;
mi_0044c6a4:;
    /* 0044c6a4 */
    f->boundary(0x0044c6a4u,0x0044dd60u,f);return;
mi_0044c6a9:;
    /* 0044c6a9 */
    f->boundary(0x0044c6a9u,0x0044de20u,f);return;
mi_0044c6ae:;
    /* 0044c6ae */
    f->boundary(0x0044c6aeu,0x0044dee0u,f);return;
mi_0044c6b3:;
    /* 0044c6b3 */
    f->boundary(0x0044c6b3u,0x0044df90u,f);return;
mi_0044c6b8:;
    /* 0044c6b8 */
    f->boundary(0x0044c6b8u,0x0044d970u,f);return;
mi_0044c6bd:;
    /* 0044c6bd */
    f->boundary(0x0044c6bdu,0x0044da30u,f);return;
mi_0044c6c2:;
    /* 0044c6c2 */
    f->boundary(0x0044c6c2u,0x0044e2a0u,f);return;
mi_0044c6c7:;
    /* 0044c6c7 */
    f->boundary(0x0044c6c7u,0x0044e5d0u,f);return;
mi_0044c6cc:;
    /* 0044c6cc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c6ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c6d3u;f->boundary(0x0044c6ceu,0x0044e7b0u,f);
mi_0044c6d3:;
    /* 0044c6d3 */
    f->r[4]+=4;return;
mi_0044c6d4:;
    /* 0044c6d4 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c6d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c6dbu;f->boundary(0x0044c6d6u,0x0044e7b0u,f);
    /* 0044c6db */
    f->r[4]+=4;return;
mi_0044c6dc:;
    /* 0044c6dc */
    f->boundary(0x0044c6dcu,0x0044e890u,f);return;
mi_0044c6e1:;
    /* 0044c6e1 */
    f->boundary(0x0044c6e1u,0x0044e000u,f);return;
mi_0044c6e6:;
    /* 0044c6e6 */
    f->boundary(0x0044c6e6u,0x0044e070u,f);return;
mi_0044c6eb:;
    /* 0044c6eb */
    f->boundary(0x0044c6ebu,0x0044e0e0u,f);return;
mi_0044c6f0:;
    /* 0044c6f0 */
    f->boundary(0x0044c6f0u,0x0044e150u,f);return;
mi_0044c6f5:;
    /* 0044c6f5 */
    f->boundary(0x0044c6f5u,0x0044e1c0u,f);return;
mi_0044c6fa:;
    /* 0044c6fa */
    f->boundary(0x0044c6fau,0x0044e230u,f);return;
}

void restore_event_C_0044c7e0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c7e0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044c7e3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c7e4 */
    f->r[6]=f->r[1];
    /* 0044c7e6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c7e8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c7edu;f->boundary(0x0044c7e8u,0x0044d680u,f);
    /* 0044c7ed */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c7ef */
    if((f->flags&MI_ZF))goto mi_0044c895;
    /* 0044c7f5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000034u+f->r[6]));
    /* 0044c7f9 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c7fa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c7fb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c7fc */
    f->r[1]=f->r[6];
    /* 0044c7fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c803u;f->boundary(0x0044c7feu,0x0044c0d0u,f);
    /* 0044c803 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044c806 */
    f->r[1]=f->r[6];
    /* 0044c808 */
    f->r[7]=f->r[0];
    /* 0044c80a */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000bfu);
    /* 0044c811 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000c0u);
    /* 0044c818 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c81du;f->boundary(0x0044c818u,0x0044f660u,f);
    /* 0044c81d */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0044c821 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044c826 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c827 */
    f->r[1]=f->r[6];
    /* 0044c829 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c82eu;f->boundary(0x0044c829u,0x0044e6f0u,f);
    /* 0044c82e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0044c830 */
    if((f->flags&MI_ZF))goto mi_0044c835;
    /* 0044c832 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
mi_0044c835:;
    /* 0044c835 */
    f->r[2]=(0x0000000cu+f->r[4]);
    /* 0044c839 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c83b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c83c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c83e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c83f */
    a=0x00000083u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c844 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044c84e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c853u;f->boundary(0x0044c84eu,0x004e0cf0u,f);
    /* 0044c853 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044c857 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c858 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c85du;f->boundary(0x0044c858u,0x004e96c0u,f);
    /* 0044c85d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044c861 */
    f->r[7]=MI_U32((0x00000028u+f->r[6]));
    /* 0044c864 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c867 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0044c86a */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0044c86e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c86f */
    f->r[1]=f->r[6];
    /* 0044c871 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[7]);
    /* 0044c874 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c879u;f->boundary(0x0044c874u,0x004496d0u,f);
    /* 0044c879 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044c87a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044c87b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c87d */
    if((f->flags&MI_ZF))goto mi_0044c895;
    /* 0044c87f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c881 */
    f->r[1]=f->r[6];
    /* 0044c883 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c888u;f->boundary(0x0044c883u,0x0044fd30u,f);
    /* 0044c888 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044c88c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c88d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c88e */
    f->r[1]=f->r[6];
    /* 0044c890 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c895u;f->boundary(0x0044c890u,0x0044c490u,f);
mi_0044c895:;
    /* 0044c895 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c896 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c899 */
    f->r[4]+=4;return;
}

void restore_event_C_0044c8a0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c8a0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044c8a3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8a4 */
    f->r[6]=f->r[1];
    /* 0044c8a6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c8adu;f->boundary(0x0044c8a8u,0x0044d680u,f);
    /* 0044c8ad */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c8af */
    if((f->flags&MI_ZF))goto mi_0044c953;
    /* 0044c8b5 */
    f->r[1]=f->r[6];
    /* 0044c8b7 */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)(0x000000c3u);
    /* 0044c8be */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)(0x000000c4u);
    /* 0044c8c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c8cau;f->boundary(0x0044c8c5u,0x0044f660u,f);
    /* 0044c8ca */
    MI_U16((0x00000014u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044c8cf */
    f->r[0]=(0x00000004u+f->r[4]);
    /* 0044c8d3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8d4 */
    f->r[1]=f->r[6];
    /* 0044c8d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c8dbu;f->boundary(0x0044c8d6u,0x0044e6f0u,f);
    /* 0044c8db */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044c8e5 */
    f->r[1]=(0x00000004u+f->r[4]);
    /* 0044c8e9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044c8ec */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8ee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8ef */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8f1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8f2 */
    a=0x00000083u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c8f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c8fcu;f->boundary(0x0044c8f7u,0x004e0cf0u,f);
    /* 0044c8fc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044c900 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0044c902 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c903 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c908u;f->boundary(0x0044c903u,0x004e96c0u,f);
    /* 0044c908 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0044c90e */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c911 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044c915 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0044c91b */
    MI_U32((0x00ea5df8u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0044c921 */
    f->r[0]=MI_U32((0x00000028u+f->r[6]));
    /* 0044c924 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c926 */
    f->r[1]=f->r[6];
    /* 0044c928 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044c92b */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0044c92f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044c933 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c934 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c939u;f->boundary(0x0044c934u,0x004496d0u,f);
    /* 0044c939 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c93b */
    if((f->flags&MI_ZF))goto mi_0044c953;
    /* 0044c93d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c93f */
    f->r[1]=f->r[6];
    /* 0044c941 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c946u;f->boundary(0x0044c941u,0x0044fd30u,f);
    /* 0044c946 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044c94a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c94b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c94c */
    f->r[1]=f->r[6];
    /* 0044c94e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c953u;f->boundary(0x0044c94eu,0x0044c490u,f);
mi_0044c953:;
    /* 0044c953 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c954 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c957 */
    f->r[4]+=4;return;
}

void restore_event_C_0044c960(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c960 */
    a=MI_U16((0x00000002u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044c965 */
    if(mi_signed_less(f->flags))goto mi_0044c981;
    /* 0044c967 */
    a=MI_U16((0x00000004u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044c96c */
    if(!mi_signed_less(f->flags))goto mi_0044c974;
    /* 0044c96e */
    f->r[0]=0x00000001u;
    /* 0044c973 */
    f->r[4]+=4;return;
mi_0044c974:;
    /* 0044c974 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044c976 */
    a=MI_U16((0x00000006u+f->r[1]));b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044c97a */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044c97d */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044c980 */
    f->r[4]+=4;return;
mi_0044c981:;
    /* 0044c981 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[1])))&0xffffu)<<0);
    /* 0044c985 */
    a=(f->r[2]&0xffffu);b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044c988 */
    if(mi_signed_less(f->flags))goto mi_0044c99d;
    /* 0044c98a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000006u+f->r[1])))&0xffffu)<<0);
    /* 0044c98e */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044c991 */
    if(mi_signed_less(f->flags))goto mi_0044c99d;
    /* 0044c993 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0044c996 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 0044c999 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044c99b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044c99c */
    f->r[4]+=4;return;
mi_0044c99d:;
    /* 0044c99d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044c99f */
    f->r[4]+=4;return;
}

void restore_event_C_0044c9a0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c9a0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c9a1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c9a2 */
    f->r[7]=f->r[1];
    /* 0044c9a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c9a9u;f->boundary(0x0044c9a4u,0x0044d750u,f);
    /* 0044c9a9 */
    f->r[1]=f->r[7];
    /* 0044c9ab */
    f->r[6]=f->r[0];
    /* 0044c9ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c9b2u;f->boundary(0x0044c9adu,0x0044c110u,f);
    /* 0044c9b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044c9b4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044c9bb;
    /* 0044c9b6 */
    f->r[0]=0x0000000cu;
mi_0044c9bb:;
    /* 0044c9bb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044c9bd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000061u+f->r[7])))&0xffu)<<0);
    /* 0044c9c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044c9c1 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0044c9c3 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0044c9c5 */
    a=f->r[6];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044c9c8 */
    f->r[0]=f->r[6];
    /* 0044c9ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c9cb */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0044c9ce */
    if(!mi_signed_less(f->flags))goto mi_0044c9d5;
    /* 0044c9d0 */
    f->r[0]=0x00000001u;
mi_0044c9d5:;
    /* 0044c9d5 */
    f->r[4]+=4;return;
}

void restore_event_C_0044c9e0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044c9e0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044c9e3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c9e4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c9e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c9e6 */
    f->r[6]=f->r[1];
    /* 0044c9e8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c9e9 */
    a=mi_read((0x00000030u+f->r[6]),2u);b=0x00000008u;t=mi_alu(f,a,b,16,1);
    /* 0044c9ee */
    if((f->flags&MI_ZF))goto mi_0044cd92;
    /* 0044c9f4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044c9f7 */
    f->r[3]=0x00000001u;
    /* 0044c9fc */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c9fe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044ca01 */
    if((f->flags&MI_ZF))goto mi_0044ca2a;
    /* 0044ca03 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0044ca05 */
    if(!(f->flags&MI_ZF))goto mi_0044ca21;
    /* 0044ca07 */
    MI_U16((0x00000012u+f->r[4]))=(uint16_t)(0x00000000u);
mi_0044ca0e:;
    /* 0044ca0e */
    f->r[1]=f->r[6];
    /* 0044ca10 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ca15u;f->boundary(0x0044ca10u,0x0044f660u,f);
    /* 0044ca15 */
    MI_U16((0x00000020u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ca1a */
    f->r[5]=0x0000010au;
    /* 0044ca1f */
    goto mi_0044ca4d;
mi_0044ca21:;
    /* 0044ca21 */
    MI_U16((0x00000012u+f->r[4]))=(uint16_t)(0x00000012u);
    /* 0044ca28 */
    goto mi_0044ca3c;
mi_0044ca2a:;
    /* 0044ca2a */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0044ca2c */
    if(!(f->flags&MI_ZF))goto mi_0044ca35;
    /* 0044ca2e */
    MI_U16((0x00000012u+f->r[4]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0044ca33 */
    goto mi_0044ca0e;
mi_0044ca35:;
    /* 0044ca35 */
    MI_U16((0x00000012u+f->r[4]))=(uint16_t)(0x00000013u);
mi_0044ca3c:;
    /* 0044ca3c */
    f->r[1]=f->r[6];
    /* 0044ca3e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ca43u;f->boundary(0x0044ca3eu,0x0044f660u,f);
    /* 0044ca43 */
    MI_U16((0x00000020u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ca48 */
    f->r[5]=0x00000110u;
mi_0044ca4d:;
    /* 0044ca4d */
    f->r[0]=(0x00000010u+f->r[4]);
    /* 0044ca51 */
    f->r[1]=f->r[6];
    /* 0044ca53 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ca54 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ca59u;f->boundary(0x0044ca54u,0x0044e6f0u,f);
    /* 0044ca59 */
    f->r[1]=f->r[6];
    /* 0044ca5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ca60u;f->boundary(0x0044ca5bu,0x0044d750u,f);
    /* 0044ca60 */
    f->r[7]=f->r[0];
    /* 0044ca62 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0044ca64 */
    if(!(f->flags&MI_ZF))goto mi_0044cb8c;
    /* 0044ca6a */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044ca70 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00000030u+f->r[6]),2u))&0xffffu)<<0);
    /* 0044ca74 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044ca77 */
    if(!(f->flags&MI_ZF))goto mi_0044caa0;
    /* 0044ca79 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000026u+f->r[6]),2u);
    /* 0044ca7d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ca7e */
    f->r[1]=f->r[6];
    /* 0044ca80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ca85u;f->boundary(0x0044ca80u,0x004496d0u,f);
    /* 0044ca85 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ca87 */
    if((f->flags&MI_ZF))goto mi_0044cb7a;
    /* 0044ca8d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ca8e */
    f->r[1]=f->r[6];
    /* 0044ca90 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ca95u;f->boundary(0x0044ca90u,0x0044fd30u,f);
    /* 0044ca95 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000026u+f->r[6]),2u);
    /* 0044ca99 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ca9a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ca9b */
    goto mi_0044cb73;
mi_0044caa0:;
    /* 0044caa0 */
    a=(f->r[0]&0xffffu);b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0044caa4 */
    if((f->flags&MI_ZF))goto mi_0044cab8;
    /* 0044caa6 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000008u);
    /* 0044caac */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0044cab0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044cab1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044cab2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044cab3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044cab4 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cab7 */
    f->r[4]+=4;return;
mi_0044cab8:;
    /* 0044cab8 */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0044cabd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044caf8;
    /* 0044cabf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000052u+f->r[6]),2u);
    /* 0044cac3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cac4 */
    f->r[1]=f->r[6];
    /* 0044cac6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cacbu;f->boundary(0x0044cac6u,0x004496d0u,f);
    /* 0044cacb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cacd */
    if((f->flags&MI_ZF))goto mi_0044cae5;
    /* 0044cacf */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cad1 */
    f->r[1]=f->r[6];
    /* 0044cad3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cad8u;f->boundary(0x0044cad3u,0x0044fd30u,f);
    /* 0044cad8 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000052u+f->r[6]),2u);
    /* 0044cadc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cadd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cade */
    f->r[1]=f->r[6];
    /* 0044cae0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cae5u;f->boundary(0x0044cae0u,0x0044c490u,f);
mi_0044cae5:;
    /* 0044cae5 */
    f->r[1]=f->r[6];
    /* 0044cae7 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000007u);
    /* 0044caed */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044caf2u;f->boundary(0x0044caedu,0x0044c110u,f);
    /* 0044caf2 */
    a=(f->r[0]&0xffffu);b=mi_read((0x0000002eu+f->r[6]),2u);t=mi_alu(f,a,b,16,1);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0044caf6 */
    goto mi_0044cb3b;
mi_0044caf8:;
    /* 0044caf8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044cafb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cafc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb01u;f->boundary(0x0044cafcu,0x00446260u,f);
    /* 0044cb01 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cb04 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044cb06 */
    if((f->flags&MI_ZF))goto mi_0044cb54;
    /* 0044cb08 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0044cb0c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cb0d */
    f->r[1]=f->r[6];
    /* 0044cb0f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb14u;f->boundary(0x0044cb0fu,0x004496d0u,f);
    /* 0044cb14 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cb16 */
    if((f->flags&MI_ZF))goto mi_0044cb2e;
    /* 0044cb18 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cb1a */
    f->r[1]=f->r[6];
    /* 0044cb1c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb21u;f->boundary(0x0044cb1cu,0x0044fd30u,f);
    /* 0044cb21 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0044cb25 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cb26 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cb27 */
    f->r[1]=f->r[6];
    /* 0044cb29 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb2eu;f->boundary(0x0044cb29u,0x0044c490u,f);
mi_0044cb2e:;
    /* 0044cb2e */
    f->r[1]=f->r[6];
    /* 0044cb30 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000006u);
    /* 0044cb36 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb3bu;f->boundary(0x0044cb36u,0x0044c110u,f);
mi_0044cb3b:;
    /* 0044cb3b */
    MI_U16((0x0000002eu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044cb3f */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
mi_0044cb42:;
    /* 0044cb42 */
    if(!mi_signed_less(f->flags))goto mi_0044cd92;
    /* 0044cb48 */
    MI_U16((0x0000002eu+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0044cb4c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044cb4d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044cb4e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044cb4f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044cb50 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cb53 */
    f->r[4]+=4;return;
mi_0044cb54:;
    /* 0044cb54 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000054u+f->r[6]),2u);
    /* 0044cb58 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cb59 */
    f->r[1]=f->r[6];
    /* 0044cb5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb60u;f->boundary(0x0044cb5bu,0x004496d0u,f);
    /* 0044cb60 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cb62 */
    if((f->flags&MI_ZF))goto mi_0044cb7a;
    /* 0044cb64 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cb66 */
    f->r[1]=f->r[6];
    /* 0044cb68 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb6du;f->boundary(0x0044cb68u,0x0044fd30u,f);
    /* 0044cb6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cb6e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000054u+f->r[6]),2u);
    /* 0044cb72 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0044cb73:;
    /* 0044cb73 */
    f->r[1]=f->r[6];
    /* 0044cb75 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cb7au;f->boundary(0x0044cb75u,0x0044c490u,f);
mi_0044cb7a:;
    /* 0044cb7a */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000008u);
    /* 0044cb80 */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0044cb84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044cb85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044cb86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044cb87 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044cb88 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cb8b */
    f->r[4]+=4;return;
mi_0044cb8c:;
    /* 0044cb8c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000030u+f->r[6]),2u);
    /* 0044cb90 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0044cb93 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044cd92;
    /* 0044cb99 */
    switch(mi_read((0x0044cd9cu+f->r[0]*4u),4u)){case 0x0044cba0u:goto mi_0044cba0;case 0x0044cbaau:goto mi_0044cbaa;case 0x0044cbffu:goto mi_0044cbff;case 0x0044cc33u:goto mi_0044cc33;case 0x0044cd92u:goto mi_0044cd92;default:abort();}
mi_0044cba0:;
    /* 0044cba0 */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x0000002eu+f->r[6]))=(uint16_t)(t);
    /* 0044cba4 */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044cba8 */
    goto mi_0044cb42;
mi_0044cbaa:;
    /* 0044cbaa */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x0000002eu+f->r[6]))=(uint16_t)(t);
    /* 0044cbae */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044cbb2 */
    if(!mi_signed_less(f->flags))goto mi_0044cd92;
    /* 0044cbb8 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0044cbbe */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cbc0 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0044cbc6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cbc8 */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044cbce */
    f->r[2]=mi_read((0x00ea3ff4u+f->r[1]),4u);
    /* 0044cbd4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cbd5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cbd7 */
    a=0x0000011fu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0044cbdc:;
    /* 0044cbdc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cbe1u;f->boundary(0x0044cbdcu,0x004e0cf0u,f);
    /* 0044cbe1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044cbe4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cbe5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cbeau;f->boundary(0x0044cbe5u,0x004b8690u,f);
    /* 0044cbea */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cbed */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000008u);
    /* 0044cbf3 */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0044cbf7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044cbf8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044cbf9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044cbfa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044cbfb */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cbfe */
    f->r[4]+=4;return;
mi_0044cbff:;
    /* 0044cbff */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x0000002eu+f->r[6]))=(uint16_t)(t);
    /* 0044cc03 */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044cc07 */
    if(!mi_signed_less(f->flags))goto mi_0044cd92;
    /* 0044cc0d */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0044cc13 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cc15 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0044cc1b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cc1d */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044cc23 */
    f->r[2]=mi_read((0x00ea3ff4u+f->r[1]),4u);
    /* 0044cc29 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cc2a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cc2c */
    a=0x00000120u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cc31 */
    goto mi_0044cbdc;
mi_0044cc33:;
    /* 0044cc33 */
    f->r[1]=f->r[6];
    /* 0044cc35 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cc3au;f->boundary(0x0044cc35u,0x0044c9a0u,f);
    /* 0044cc3a */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044cc3f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000032u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cc42 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cc44 */
    if(!(f->flags&MI_ZF))goto mi_0044cce1;
    /* 0044cc4a */
    a=mi_read((0x00000028u+f->r[6]),1u);b=0x00000011u;t=mi_alu(f,a,b,8,3);
    /* 0044cc4e */
    if(!(f->flags&MI_ZF))goto mi_0044cd92;
    /* 0044cc54 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000014u+f->r[6]),2u);
    /* 0044cc58 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007848d8u),2u);
    /* 0044cc5f */
    f->r[0]=mi_read((0x0078d7bcu),4u);
    /* 0044cc64 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044cc66 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0044cc68 */
    if(!(f->flags&MI_ZF))goto mi_0044cd92;
    /* 0044cc6e */
    f->r[1]=mi_read((0x0078d7c0u),4u);
    /* 0044cc74 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044cc76 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000003u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cc79 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044cc7b */
    if(!(f->flags&MI_ZF))goto mi_0044cd92;
    /* 0044cc81 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x0000002cu+f->r[6]),2u))&0xffffu)<<0);
    /* 0044cc85 */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0044cc88 */
    MI_U16((0x0000002eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044cc8c */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044cc92 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cc95 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cc97 */
    if((f->flags&MI_ZF))goto mi_0044ccaf;
    /* 0044cc99 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cc9c */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cc9e */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cca0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044cca2 */
    a=f->r[2];b=0x00000012u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044cca5 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044cca8 */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044ccad */
    goto mi_0044ccc3;
mi_0044ccaf:;
    /* 0044ccaf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044ccb2 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ccb4 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ccb6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044ccb8 */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0044ccbb */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044ccbe */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
mi_0044ccc3:;
    /* 0044ccc3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044ccc6 */
    f->r[1]=(0x00000010u+f->r[4]);
    /* 0044ccca */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cccc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cccd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cccf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ccd0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ccd1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ccd6u;f->boundary(0x0044ccd1u,0x004e0cf0u,f);
    /* 0044ccd6 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ccd9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044ccda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044ccdb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044ccdc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044ccdd */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cce0 */
    f->r[4]+=4;return;
mi_0044cce1:;
    /* 0044cce1 */
    f->r[1]=f->r[6];
    /* 0044cce3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cce8u;f->boundary(0x0044cce3u,0x0044c110u,f);
    /* 0044cce8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044ccea */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044ccf1;
    /* 0044ccec */
    f->r[0]=0x0000000cu;
mi_0044ccf1:;
    /* 0044ccf1 */
    f->r[1]=f->r[7];
    /* 0044ccf3 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044ccf5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044ccf7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000061u+f->r[6]),1u))&0xffu)<<0);
    /* 0044ccfa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044ccfc */
    if(!(f->flags&MI_ZF))goto mi_0044cd3e;
    /* 0044ccfe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cd01 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cd03 */
    if((f->flags&MI_ZF))goto mi_0044cd1b;
    /* 0044cd05 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cd08 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd0a */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd0c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044cd0e */
    a=f->r[1];b=0x00000012u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044cd11 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044cd14 */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044cd19 */
    goto mi_0044cd2f;
mi_0044cd1b:;
    /* 0044cd1b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cd1e */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd20 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd22 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044cd24 */
    a=f->r[2];b=0x00000012u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044cd27 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044cd2a */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
mi_0044cd2f:;
    /* 0044cd2f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044cd32 */
    f->r[0]=(0x00000010u+f->r[4]);
    /* 0044cd36 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd38 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd39 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd3b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd3c */
    goto mi_0044cd85;
mi_0044cd3e:;
    /* 0044cd3e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044cd40 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00000062u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cd43 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044cd45 */
    if(!(f->flags&MI_ZF))goto mi_0044cd92;
    /* 0044cd47 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cd4a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cd4c */
    if((f->flags&MI_ZF))goto mi_0044cd64;
    /* 0044cd4e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cd51 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd53 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd55 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044cd57 */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0044cd5a */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044cd5d */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044cd62 */
    goto mi_0044cd78;
mi_0044cd64:;
    /* 0044cd64 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cd67 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd69 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044cd6b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044cd6d */
    a=f->r[1];b=0x00000012u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044cd70 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044cd73 */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
mi_0044cd78:;
    /* 0044cd78 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044cd7b */
    f->r[2]=(0x00000010u+f->r[4]);
    /* 0044cd7f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd81 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd82 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd84 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0044cd85:;
    /* 0044cd85 */
    a=0x00000082u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cd8a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cd8fu;f->boundary(0x0044cd8au,0x004e0cf0u,f);
    /* 0044cd8f */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044cd92:;
    /* 0044cd92 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044cd93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044cd94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044cd95 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044cd96 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cd99 */
    f->r[4]+=4;return;
}

void restore_event_C_0044cdc0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044cdc0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044cdc3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cdc4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cdc5 */
    f->r[6]=f->r[1];
    /* 0044cdc7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cdc8 */
    f->r[1]=0x00000025u;
    /* 0044cdcd */
    a=mi_read((0x00000004u+f->r[6]),2u);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044cdd1 */
    if(!(f->flags&MI_ZF))goto mi_0044cde1;
    /* 0044cdd3 */
    f->r[1]=f->r[6];
    /* 0044cdd5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cddau;f->boundary(0x0044cdd5u,0x0044c9e0u,f);
    /* 0044cdda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044cddb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044cddc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044cddd */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cde0 */
    f->r[4]+=4;return;
mi_0044cde1:;
    /* 0044cde1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00000030u+f->r[6]),2u))&0xffffu)<<0);
    /* 0044cde5 */
    a=(f->r[0]&0xffffu);b=0x00000008u;t=mi_alu(f,a,b,16,1);
    /* 0044cde9 */
    if((f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044cdef */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0044cdf3 */
    if((f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044cdf9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cdfc */
    f->r[7]=0x00000082u;
    /* 0044ce01 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ce03 */
    if((f->flags&MI_ZF))goto mi_0044ce56;
    /* 0044ce05 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044ce07 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044ce0a */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044ce0d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044ce9f;
    /* 0044ce13 */
    switch(mi_read((0x0044d3d4u+f->r[0]*4u),4u)){case 0x0044ce1au:goto mi_0044ce1a;case 0x0044ce23u:goto mi_0044ce23;case 0x0044ce31u:goto mi_0044ce31;case 0x0044ce3fu:goto mi_0044ce3f;case 0x0044ce4du:goto mi_0044ce4d;case 0x0044ce9fu:goto mi_0044ce9f;default:abort();}
mi_0044ce1a:;
    /* 0044ce1a */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x00000024u);
    /* 0044ce21 */
    goto mi_0044ce9f;
mi_0044ce23:;
    /* 0044ce23 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x00000036u);
    /* 0044ce2a */
    f->r[7]=0x00000113u;
    /* 0044ce2f */
    goto mi_0044ce9f;
mi_0044ce31:;
    /* 0044ce31 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x00000048u);
    /* 0044ce38 */
    f->r[7]=0x00000114u;
    /* 0044ce3d */
    goto mi_0044ce9f;
mi_0044ce3f:;
    /* 0044ce3f */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x0000005au);
    /* 0044ce46 */
    f->r[7]=0x0000014au;
    /* 0044ce4b */
    goto mi_0044ce9f;
mi_0044ce4d:;
    /* 0044ce4d */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x0000006cu);
    /* 0044ce54 */
    goto mi_0044ce9f;
mi_0044ce56:;
    /* 0044ce56 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044ce58 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044ce5b */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044ce5e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044ce9f;
    /* 0044ce60 */
    switch(mi_read((0x0044d3f0u+f->r[0]*4u),4u)){case 0x0044ce67u:goto mi_0044ce67;case 0x0044ce6eu:goto mi_0044ce6e;case 0x0044ce7cu:goto mi_0044ce7c;case 0x0044ce8au:goto mi_0044ce8a;case 0x0044ce98u:goto mi_0044ce98;case 0x0044ce9fu:goto mi_0044ce9f;default:abort();}
mi_0044ce67:;
    /* 0044ce67 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044ce6c */
    goto mi_0044ce9f;
mi_0044ce6e:;
    /* 0044ce6e */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x00000037u);
    /* 0044ce75 */
    f->r[7]=0x00000113u;
    /* 0044ce7a */
    goto mi_0044ce9f;
mi_0044ce7c:;
    /* 0044ce7c */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x00000049u);
    /* 0044ce83 */
    f->r[7]=0x00000114u;
    /* 0044ce88 */
    goto mi_0044ce9f;
mi_0044ce8a:;
    /* 0044ce8a */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x0000005bu);
    /* 0044ce91 */
    f->r[7]=0x0000014au;
    /* 0044ce96 */
    goto mi_0044ce9f;
mi_0044ce98:;
    /* 0044ce98 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x0000006du);
mi_0044ce9f:;
    /* 0044ce9f */
    f->r[1]=f->r[6];
    /* 0044cea1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cea6u;f->boundary(0x0044cea1u,0x0044f660u,f);
    /* 0044cea6 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ceab */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044ceaf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ceb0 */
    f->r[1]=f->r[6];
    /* 0044ceb2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ceb7u;f->boundary(0x0044ceb2u,0x0044e6f0u,f);
    /* 0044ceb7 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00000030u+f->r[6]),2u))&0xffffu)<<0);
    /* 0044cebb */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0044cebf */
    if((f->flags&MI_ZF))goto mi_0044d357;
    /* 0044cec5 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0044cec9 */
    if((f->flags&MI_ZF))goto mi_0044d357;
    /* 0044cecf */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000032u+f->r[6]),1u))&0xffu)<<0);
    /* 0044ced2 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ced4 */
    if((f->flags&MI_ZF))goto mi_0044d1b0;
    /* 0044ceda */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x0000002eu+f->r[6]))=(uint16_t)(t);
    /* 0044cede */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x0000002eu+f->r[6]),2u))&0xffffu)<<0);
    /* 0044cee2 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044cee5 */
    f->r[3]=0x00000001u;
    /* 0044ceea */
    if(!(f->flags&MI_ZF))goto mi_0044d07d;
    /* 0044cef0 */
    a=(f->r[1]&0xffffu);b=0x00000006u;t=mi_alu(f,a,b,16,1);
    /* 0044cef4 */
    if(!(f->flags&MI_ZF))goto mi_0044cfa1;
    /* 0044cefa */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044cf00 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cf03 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cf05 */
    MI_U16((0x00000018u+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044cf0a */
    if((f->flags&MI_ZF))goto mi_0044cf4a;
    /* 0044cf0c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044cf0e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cf11 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044cf14 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044cf86;
    /* 0044cf16 */
    switch(mi_read((0x0044d40cu+f->r[0]*4u),4u)){case 0x0044cf1du:goto mi_0044cf1d;case 0x0044cf26u:goto mi_0044cf26;case 0x0044cf2fu:goto mi_0044cf2f;case 0x0044cf38u:goto mi_0044cf38;case 0x0044cf41u:goto mi_0044cf41;case 0x0044cf86u:goto mi_0044cf86;default:abort();}
mi_0044cf1d:;
    /* 0044cf1d */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000028u);
    /* 0044cf24 */
    goto mi_0044cf86;
mi_0044cf26:;
    /* 0044cf26 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000003au);
    /* 0044cf2d */
    goto mi_0044cf86;
mi_0044cf2f:;
    /* 0044cf2f */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000004cu);
    /* 0044cf36 */
    goto mi_0044cf86;
mi_0044cf38:;
    /* 0044cf38 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000005eu);
    /* 0044cf3f */
    goto mi_0044cf86;
mi_0044cf41:;
    /* 0044cf41 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000070u);
    /* 0044cf48 */
    goto mi_0044cf86;
mi_0044cf4a:;
    /* 0044cf4a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044cf4c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cf4f */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044cf52 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044cf86;
    /* 0044cf54 */
    switch(mi_read((0x0044d428u+f->r[0]*4u),4u)){case 0x0044cf5bu:goto mi_0044cf5b;case 0x0044cf64u:goto mi_0044cf64;case 0x0044cf6du:goto mi_0044cf6d;case 0x0044cf76u:goto mi_0044cf76;case 0x0044cf7fu:goto mi_0044cf7f;case 0x0044cf86u:goto mi_0044cf86;default:abort();}
mi_0044cf5b:;
    /* 0044cf5b */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000029u);
    /* 0044cf62 */
    goto mi_0044cf86;
mi_0044cf64:;
    /* 0044cf64 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000003bu);
    /* 0044cf6b */
    goto mi_0044cf86;
mi_0044cf6d:;
    /* 0044cf6d */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000004du);
    /* 0044cf74 */
    goto mi_0044cf86;
mi_0044cf76:;
    /* 0044cf76 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000005fu);
    /* 0044cf7d */
    goto mi_0044cf86;
mi_0044cf7f:;
    /* 0044cf7f */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000071u);
mi_0044cf86:;
    /* 0044cf86 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044cf89 */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0044cf8d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cf8f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cf90 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cf92 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cf93 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044cf94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044cf99u;f->boundary(0x0044cf94u,0x004e0cf0u,f);
    /* 0044cf99 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044cf9c */
    goto mi_0044d0b6;
mi_0044cfa1:;
    /* 0044cfa1 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044cfa4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044d0b6;
    /* 0044cfaa */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044cfb0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cfb3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044cfb5 */
    if((f->flags&MI_ZF))goto mi_0044cff5;
    /* 0044cfb7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044cfb9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cfbc */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044cfbf */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d031;
    /* 0044cfc1 */
    switch(mi_read((0x0044d444u+f->r[0]*4u),4u)){case 0x0044cfc8u:goto mi_0044cfc8;case 0x0044cfd1u:goto mi_0044cfd1;case 0x0044cfdau:goto mi_0044cfda;case 0x0044cfe3u:goto mi_0044cfe3;case 0x0044cfecu:goto mi_0044cfec;case 0x0044d031u:goto mi_0044d031;default:abort();}
mi_0044cfc8:;
    /* 0044cfc8 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000002au);
    /* 0044cfcf */
    goto mi_0044d031;
mi_0044cfd1:;
    /* 0044cfd1 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000003cu);
    /* 0044cfd8 */
    goto mi_0044d031;
mi_0044cfda:;
    /* 0044cfda */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000004eu);
    /* 0044cfe1 */
    goto mi_0044d031;
mi_0044cfe3:;
    /* 0044cfe3 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000060u);
    /* 0044cfea */
    goto mi_0044d031;
mi_0044cfec:;
    /* 0044cfec */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000072u);
    /* 0044cff3 */
    goto mi_0044d031;
mi_0044cff5:;
    /* 0044cff5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044cff7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044cffa */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044cffd */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d031;
    /* 0044cfff */
    switch(mi_read((0x0044d460u+f->r[0]*4u),4u)){case 0x0044d006u:goto mi_0044d006;case 0x0044d00fu:goto mi_0044d00f;case 0x0044d018u:goto mi_0044d018;case 0x0044d021u:goto mi_0044d021;case 0x0044d02au:goto mi_0044d02a;case 0x0044d031u:goto mi_0044d031;default:abort();}
mi_0044d006:;
    /* 0044d006 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000002bu);
    /* 0044d00d */
    goto mi_0044d031;
mi_0044d00f:;
    /* 0044d00f */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000003du);
    /* 0044d016 */
    goto mi_0044d031;
mi_0044d018:;
    /* 0044d018 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000004fu);
    /* 0044d01f */
    goto mi_0044d031;
mi_0044d021:;
    /* 0044d021 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000061u);
    /* 0044d028 */
    goto mi_0044d031;
mi_0044d02a:;
    /* 0044d02a */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000073u);
mi_0044d031:;
    /* 0044d031 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044d034 */
    f->r[0]=0x00000018u;
    /* 0044d039 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d03b */
    MI_U16((0x0000002eu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d03f */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d044 */
    f->r[0]=(0x00000010u+f->r[4]);
    /* 0044d048 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0044d04c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d04d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d04f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d050 */
    a=0x00000082u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d055 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d05au;f->boundary(0x0044d055u,0x004e0cf0u,f);
    /* 0044d05a */
    a=0xfffffffdu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d05c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d061u;f->boundary(0x0044d05cu,0x004e96c0u,f);
    /* 0044d061 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0044d066 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d069 */
    f->r[2]=f->r[0];
    /* 0044d06b */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0044d071 */
    MI_U32((0x00ea5df8u+f->r[2]))=(uint32_t)(0x00000003u);
    /* 0044d07b */
    goto mi_0044d0bb;
mi_0044d07d:;
    /* 0044d07d */
    a=(f->r[0]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044d080 */
    if(!(f->flags&MI_ZF))goto mi_0044d0b6;
    /* 0044d082 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044d085 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044d0b6;
    /* 0044d087 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000026u+f->r[6]),2u);
    /* 0044d08b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d08c */
    f->r[1]=f->r[6];
    /* 0044d08e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d093u;f->boundary(0x0044d08eu,0x004496d0u,f);
    /* 0044d093 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d095 */
    if((f->flags&MI_ZF))goto mi_0044d0ac;
    /* 0044d097 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d098 */
    f->r[1]=f->r[6];
    /* 0044d09a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d09fu;f->boundary(0x0044d09au,0x0044fd30u,f);
    /* 0044d09f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000026u+f->r[6]),2u);
    /* 0044d0a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d0a4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d0a5 */
    f->r[1]=f->r[6];
    /* 0044d0a7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d0acu;f->boundary(0x0044d0a7u,0x0044c490u,f);
mi_0044d0ac:;
    /* 0044d0ac */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000004u);
    /* 0044d0b2 */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
mi_0044d0b6:;
    /* 0044d0b6 */
    f->r[0]=mi_read((0x00eaac28u),4u);
mi_0044d0bb:;
    /* 0044d0bb */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000033u+f->r[6]),1u))&0xffu)<<0);
    /* 0044d0be */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d0c0 */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d0c6 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00000004u+f->r[6]),2u))&0xffffu)<<0);
    /* 0044d0ca */
    a=(f->r[1]&0xffffu);b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044d0ce */
    if(!(f->flags&MI_ZF))goto mi_0044d133;
    /* 0044d0d0 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044d0d6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 0044d0da */
    a=mi_read((0x00ea187cu+f->r[0]),4u);b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044d0e0 */
    if(mi_signed_less(f->flags))goto mi_0044d3cc;
    /* 0044d0e6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x0000002eu+f->r[6]),2u))&0xffffu)<<0);
    /* 0044d0ea */
    MI_U8((0x00000033u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0044d0ed */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044d0f3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044d0f6 */
    MI_U16((0x00000018u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d0fb */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044d0fe */
    a=(f->r[1]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0044d101 */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d106 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d10b */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d110 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044d113 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d116 */
    f->r[2]=(0x0000000cu+f->r[4]);
    /* 0044d11a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d11b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d11c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d11e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d11f */
    a=0x00000135u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d124 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d129u;f->boundary(0x0044d124u,0x004e0cf0u,f);
    /* 0044d129 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d12c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d12d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d12e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d12f */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d132 */
    f->r[4]+=4;return;
mi_0044d133:;
    /* 0044d133 */
    a=f->r[0];b=0x00001276u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044d139 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0044d13c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044d13e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 0044d142 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0044d145 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0044d148 */
    f->r[1]=mi_read((0x00ea6160u+f->r[0]*4u),4u);
    /* 0044d14f */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0044d152 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044d154 */
    if(mi_signed_less(f->flags))goto mi_0044d3cc;
    /* 0044d15a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x0000002eu+f->r[6]),2u))&0xffffu)<<0);
    /* 0044d15e */
    MI_U8((0x00000033u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0044d161 */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044d167 */
    MI_U16((0x00000018u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d16c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044d16f */
    MI_U32((0x006057e4u))=(uint32_t)(f->r[1]);
    /* 0044d175 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044d178 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044d17b */
    a=(f->r[1]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0044d17e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044d181 */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d186 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d18b */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d190 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044d194 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d197 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d198 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d199 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d19b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d19c */
    a=0x0000013au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d1a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d1a6u;f->boundary(0x0044d1a1u,0x004e0cf0u,f);
    /* 0044d1a6 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d1a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d1aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d1ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d1ac */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d1af */
    f->r[4]+=4;return;
mi_0044d1b0:;
    /* 0044d1b0 */
    a=mi_read((0x00000028u+f->r[6]),1u);b=0x00000011u;t=mi_alu(f,a,b,8,3);
    /* 0044d1b4 */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d1ba */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000014u+f->r[6]),2u);
    /* 0044d1be */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007848d8u),2u);
    /* 0044d1c5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044d1c7 */
    f->r[0]=mi_read((0x0078d7bcu),4u);
    /* 0044d1cc */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044d1ce */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d1d4 */
    f->r[0]=mi_read((0x0078d7c0u),4u);
    /* 0044d1d9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044d1db */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000003u+f->r[6]),1u))&0xffu)<<0);
    /* 0044d1de */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0044d1e0 */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d1e6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00000004u+f->r[6]),2u))&0xffffu)<<0);
    /* 0044d1ea */
    f->r[3]=0x00000001u;
    /* 0044d1ef */
    a=(f->r[0]&0xffffu);b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044d1f3 */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0044d1f6 */
    if(!(f->flags&MI_ZF))goto mi_0044d212;
    /* 0044d1f8 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0044d1fe */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0044d204 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 0044d208 */
    a=mi_read((0x00ea187cu+f->r[2]),4u);b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044d20e */
    if(mi_signed_less(f->flags))goto mi_0044d23e;
    /* 0044d210 */
    goto mi_0044d23b;
mi_0044d212:;
    /* 0044d212 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0044d218 */
    a=f->r[1];b=0x00001276u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0044d21e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0044d221 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 0044d225 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044d227 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044d22a */
    f->r[2]=mi_read((0x00ea6160u+f->r[1]*4u),4u);
    /* 0044d231 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044d234 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044d237 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044d239 */
    if(mi_signed_less(f->flags))goto mi_0044d23e;
mi_0044d23b:;
    /* 0044d23b */
    MI_U8((0x00000033u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
mi_0044d23e:;
    /* 0044d23e */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x0000002cu+f->r[6]),2u))&0xffffu)<<0);
    /* 0044d242 */
    MI_U16((0x0000002eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044d246 */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044d24c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[6]),1u))&0xffu)<<0);
    /* 0044d24f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d251 */
    if((f->flags&MI_ZF))goto mi_0044d2a4;
    /* 0044d253 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044d255 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044d258 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044d25b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d2ef;
    /* 0044d261 */
    switch(mi_read((0x0044d47cu+f->r[0]*4u),4u)){case 0x0044d268u:goto mi_0044d268;case 0x0044d271u:goto mi_0044d271;case 0x0044d27fu:goto mi_0044d27f;case 0x0044d28du:goto mi_0044d28d;case 0x0044d29bu:goto mi_0044d29b;case 0x0044d2efu:goto mi_0044d2ef;default:abort();}
mi_0044d268:;
    /* 0044d268 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000026u);
    /* 0044d26f */
    goto mi_0044d2ef;
mi_0044d271:;
    /* 0044d271 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000038u);
    /* 0044d278 */
    f->r[7]=0x00000113u;
    /* 0044d27d */
    goto mi_0044d2ef;
mi_0044d27f:;
    /* 0044d27f */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000004au);
    /* 0044d286 */
    f->r[7]=0x00000114u;
    /* 0044d28b */
    goto mi_0044d2ef;
mi_0044d28d:;
    /* 0044d28d */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000005cu);
    /* 0044d294 */
    f->r[7]=0x0000014au;
    /* 0044d299 */
    goto mi_0044d2ef;
mi_0044d29b:;
    /* 0044d29b */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000006eu);
    /* 0044d2a2 */
    goto mi_0044d2ef;
mi_0044d2a4:;
    /* 0044d2a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044d2a6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044d2a9 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044d2ac */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044d2ef;
    /* 0044d2ae */
    switch(mi_read((0x0044d498u+f->r[0]*4u),4u)){case 0x0044d2b5u:goto mi_0044d2b5;case 0x0044d2beu:goto mi_0044d2be;case 0x0044d2ccu:goto mi_0044d2cc;case 0x0044d2dau:goto mi_0044d2da;case 0x0044d2e8u:goto mi_0044d2e8;case 0x0044d2efu:goto mi_0044d2ef;default:abort();}
mi_0044d2b5:;
    /* 0044d2b5 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000027u);
    /* 0044d2bc */
    goto mi_0044d2ef;
mi_0044d2be:;
    /* 0044d2be */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000039u);
    /* 0044d2c5 */
    f->r[7]=0x00000113u;
    /* 0044d2ca */
    goto mi_0044d2ef;
mi_0044d2cc:;
    /* 0044d2cc */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000004bu);
    /* 0044d2d3 */
    f->r[7]=0x00000114u;
    /* 0044d2d8 */
    goto mi_0044d2ef;
mi_0044d2da:;
    /* 0044d2da */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000005du);
    /* 0044d2e1 */
    f->r[7]=0x0000014au;
    /* 0044d2e6 */
    goto mi_0044d2ef;
mi_0044d2e8:;
    /* 0044d2e8 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x0000006fu);
mi_0044d2ef:;
    /* 0044d2ef */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 0044d2f2 */
    f->r[2]=(0x0000000cu+f->r[4]);
    /* 0044d2f6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d2f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d2f9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d2fb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d2fc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d2fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d302u;f->boundary(0x0044d2fdu,0x004e0cf0u,f);
    /* 0044d302 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x007027c0u),1u))&0xffu)<<0);
    /* 0044d307 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d30a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d30c */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d312 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)mi_read((0x00e38ea8u),1u);
    /* 0044d319 */
    a=f->r[1];b=mi_read((0x00579590u),4u);t=mi_alu(f,a,b,32,1);
    /* 0044d31f */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d325 */
    a=mi_read((0x00000002u+f->r[6]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0044d328 */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d32e */
    a=mi_read((0x00000004u+f->r[6]),2u);b=0x0000000du;t=mi_alu(f,a,b,16,1);
    /* 0044d333 */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d339 */
    f->r[0]=mi_read((0x00e38ebcu),4u);
    /* 0044d33e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044d340 */
    if(!(f->flags&MI_ZF))goto mi_0044d3cc;
    /* 0044d346 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d348 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d34du;f->boundary(0x0044d348u,0x00417ae0u,f);
    /* 0044d34d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d350 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d351 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d352 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d353 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d356 */
    f->r[4]+=4;return;
mi_0044d357:;
    /* 0044d357 */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x0000002eu+f->r[6]))=(uint16_t)(t);
    /* 0044d35b */
    a=mi_read((0x0000002eu+f->r[6]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044d360 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044d3cc;
    /* 0044d362 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044d36c */
    a=mi_read((0x00000030u+f->r[6]),2u);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0044d371 */
    if(!(f->flags&MI_ZF))goto mi_0044d394;
    /* 0044d373 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0044d377 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d378 */
    f->r[1]=f->r[6];
    /* 0044d37a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d37fu;f->boundary(0x0044d37au,0x004496d0u,f);
    /* 0044d37f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d381 */
    if((f->flags&MI_ZF))goto mi_0044d3ba;
    /* 0044d383 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d385 */
    f->r[1]=f->r[6];
    /* 0044d387 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d38cu;f->boundary(0x0044d387u,0x0044fd30u,f);
    /* 0044d38c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d38d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0044d391 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d392 */
    goto mi_0044d3b3;
mi_0044d394:;
    /* 0044d394 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000052u+f->r[6]),2u);
    /* 0044d398 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d399 */
    f->r[1]=f->r[6];
    /* 0044d39b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d3a0u;f->boundary(0x0044d39bu,0x004496d0u,f);
    /* 0044d3a0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d3a2 */
    if((f->flags&MI_ZF))goto mi_0044d3ba;
    /* 0044d3a4 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d3a6 */
    f->r[1]=f->r[6];
    /* 0044d3a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d3adu;f->boundary(0x0044d3a8u,0x0044fd30u,f);
    /* 0044d3ad */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000052u+f->r[6]),2u);
    /* 0044d3b1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d3b2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0044d3b3:;
    /* 0044d3b3 */
    f->r[1]=f->r[6];
    /* 0044d3b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d3bau;f->boundary(0x0044d3b5u,0x0044c490u,f);
mi_0044d3ba:;
    /* 0044d3ba */
    f->r[0]=mi_read((0x00000028u+f->r[6]),4u);
    /* 0044d3bd */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000008u);
    /* 0044d3c3 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d3c5 */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0044d3c9 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
mi_0044d3cc:;
    /* 0044d3cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d3cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d3ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044d3cf */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d3d2 */
    f->r[4]+=4;return;
}

void restore_event_C_0044d780(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044d780 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044d783 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d784 */
    f->r[6]=f->r[1];
    /* 0044d786 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d788 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d78du;f->boundary(0x0044d788u,0x0044d680u,f);
    /* 0044d78d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d78f */
    if((f->flags&MI_ZF))goto mi_0044d86b;
    /* 0044d795 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d796 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d797 */
    f->r[7]=MI_U32((0x00000028u+f->r[6]));
    /* 0044d79a */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[6]));
    /* 0044d79e */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0044d7a1 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[7]);
    /* 0044d7a4 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 0044d7a8 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 0044d7ab */
    f->r[0]=(0x00000000u+f->r[5]+f->r[0]*4u);
    /* 0044d7af */
    f->r[1]=MI_U32((0x0077646cu+f->r[0]*4u));
    /* 0044d7b6 */
    a=f->r[1];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 0044d7b9 */
    if(!mi_signed_less(f->flags))goto mi_0044d843;
    /* 0044d7bf */
    a=f->r[1];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0044d7c2 */
    if(mi_signed_less(f->flags))goto mi_0044d7d1;
    /* 0044d7c4 */
    MI_U32((0x0077646cu+f->r[0]*4u))=(uint32_t)(0x00000028u);
    /* 0044d7cf */
    goto mi_0044d7e7;
mi_0044d7d1:;
    /* 0044d7d1 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044d7d3 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0044d7d6 */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d7d9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044d7da */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044d7dd */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044d7e0 */
    MI_U32((0x0077646cu+f->r[0]*4u))=(uint32_t)(f->r[2]);
mi_0044d7e7:;
    /* 0044d7e7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d7e8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d7edu;f->boundary(0x0044d7e8u,0x00444980u,f);
    /* 0044d7ed */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d7f0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044d7f2 */
    if((f->flags&MI_ZF))goto mi_0044d843;
    /* 0044d7f4 */
    f->r[1]=f->r[6];
    /* 0044d7f6 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000cfu);
    /* 0044d7fd */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000d0u);
    /* 0044d804 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d809u;f->boundary(0x0044d804u,0x0044f660u,f);
    /* 0044d809 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d80e */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044d812 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d813 */
    f->r[1]=f->r[6];
    /* 0044d815 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d81au;f->boundary(0x0044d815u,0x0044e6f0u,f);
    /* 0044d81a */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0044d81e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d820 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d821 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d822 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d823 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044d82d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d832u;f->boundary(0x0044d82du,0x004449d0u,f);
    /* 0044d832 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d835 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d836 */
    a=0x00000111u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d83b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d840u;f->boundary(0x0044d83bu,0x004e0cf0u,f);
    /* 0044d840 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044d843:;
    /* 0044d843 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044d847 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d848 */
    f->r[1]=f->r[6];
    /* 0044d84a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d84fu;f->boundary(0x0044d84au,0x004496d0u,f);
    /* 0044d84f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d850 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044d851 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d853 */
    if((f->flags&MI_ZF))goto mi_0044d86b;
    /* 0044d855 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d857 */
    f->r[1]=f->r[6];
    /* 0044d859 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d85eu;f->boundary(0x0044d859u,0x0044fd30u,f);
    /* 0044d85e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d85f */
    f->r[1]=f->r[6];
    /* 0044d861 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044d865 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d866 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d86bu;f->boundary(0x0044d866u,0x0044c490u,f);
mi_0044d86b:;
    /* 0044d86b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d86c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d86f */
    f->r[4]+=4;return;
}

void restore_event_C_0044d870(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044d870 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044d873 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d874 */
    f->r[6]=f->r[1];
    /* 0044d876 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d878 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d87du;f->boundary(0x0044d878u,0x0044d680u,f);
    /* 0044d87d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d87f */
    if((f->flags&MI_ZF))goto mi_0044d95d;
    /* 0044d885 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d886 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d887 */
    f->r[7]=MI_U32((0x00000028u+f->r[6]));
    /* 0044d88a */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[6]));
    /* 0044d88e */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0044d891 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[7]);
    /* 0044d894 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 0044d898 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 0044d89b */
    f->r[1]=(0x00000000u+f->r[5]+f->r[0]*4u);
    /* 0044d89f */
    f->r[0]=MI_U32((0x0077646cu+f->r[1]*4u));
    /* 0044d8a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044d8a8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044d935;
    /* 0044d8ae */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0044d8b1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044d8c0;
    /* 0044d8b3 */
    MI_U32((0x0077646cu+f->r[1]*4u))=(uint32_t)(0x00000019u);
    /* 0044d8be */
    goto mi_0044d8d3;
mi_0044d8c0:;
    /* 0044d8c0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044d8c2 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0044d8c5 */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044d8c8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044d8c9 */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044d8cc */
    MI_U32((0x0077646cu+f->r[1]*4u))=(uint32_t)(f->r[2]);
mi_0044d8d3:;
    /* 0044d8d3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d8d4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d8d9u;f->boundary(0x0044d8d4u,0x005599c0u,f);
    /* 0044d8d9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d8da */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d8dfu;f->boundary(0x0044d8dau,0x00444980u,f);
    /* 0044d8df */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d8e2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044d8e4 */
    if((f->flags&MI_ZF))goto mi_0044d935;
    /* 0044d8e6 */
    f->r[1]=f->r[6];
    /* 0044d8e8 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000d3u);
    /* 0044d8ef */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000d4u);
    /* 0044d8f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d8fbu;f->boundary(0x0044d8f6u,0x0044f660u,f);
    /* 0044d8fb */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d900 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044d904 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d905 */
    f->r[1]=f->r[6];
    /* 0044d907 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d90cu;f->boundary(0x0044d907u,0x0044e6f0u,f);
    /* 0044d90c */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0044d910 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d912 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d913 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d914 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d915 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044d91f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d924u;f->boundary(0x0044d91fu,0x004449d0u,f);
    /* 0044d924 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d927 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d928 */
    a=0x00000112u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d92d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d932u;f->boundary(0x0044d92du,0x004e0cf0u,f);
    /* 0044d932 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044d935:;
    /* 0044d935 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044d939 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d93a */
    f->r[1]=f->r[6];
    /* 0044d93c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d941u;f->boundary(0x0044d93cu,0x004496d0u,f);
    /* 0044d941 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044d942 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044d943 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d945 */
    if((f->flags&MI_ZF))goto mi_0044d95d;
    /* 0044d947 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d949 */
    f->r[1]=f->r[6];
    /* 0044d94b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d950u;f->boundary(0x0044d94bu,0x0044fd30u,f);
    /* 0044d950 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d951 */
    f->r[1]=f->r[6];
    /* 0044d953 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044d957 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d958 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d95du;f->boundary(0x0044d958u,0x0044c490u,f);
mi_0044d95d:;
    /* 0044d95d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044d95e */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044d961 */
    f->r[4]+=4;return;
}

void restore_event_C_0044d970(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044d970 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044d973 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d974 */
    f->r[6]=f->r[1];
    /* 0044d976 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d978 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d97du;f->boundary(0x0044d978u,0x0044d680u,f);
    /* 0044d97d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044d97f */
    if((f->flags&MI_ZF))goto mi_0044da26;
    /* 0044d985 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044d988 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d989 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044d98d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d98e */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044d991 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[6]));
    /* 0044d995 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044d998 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000c7u);
    /* 0044d99f */
    f->r[1]=MI_U32((0x005d2c48u+f->r[7]*8u));
    /* 0044d9a6 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000c8u);
    /* 0044d9ad */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044d9af */
    MI_U32((0x005d2c48u+f->r[7]*8u))=(uint32_t)(f->r[1]);
    /* 0044d9b6 */
    f->r[0]=MI_U32((0x005d2c4cu+f->r[7]*8u));
    /* 0044d9bd */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044d9bf */
    f->r[1]=f->r[6];
    /* 0044d9c1 */
    MI_U32((0x005d2c4cu+f->r[7]*8u))=(uint32_t)(f->r[0]);
    /* 0044d9c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d9cdu;f->boundary(0x0044d9c8u,0x0044f660u,f);
    /* 0044d9cd */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044d9d2 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044d9d6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d9d7 */
    f->r[1]=f->r[6];
    /* 0044d9d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d9deu;f->boundary(0x0044d9d9u,0x0044e6f0u,f);
    /* 0044d9de */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0044d9e2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d9e4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d9e5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d9e6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d9e7 */
    a=0x00000115u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044d9ec */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044d9f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044d9fbu;f->boundary(0x0044d9f6u,0x004e0cf0u,f);
    /* 0044d9fb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044d9ff */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044da02 */
    f->r[1]=f->r[6];
    /* 0044da04 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044da0au;f->boundary(0x0044da05u,0x004496d0u,f);
    /* 0044da0a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044da0b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044da0c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044da0e */
    if((f->flags&MI_ZF))goto mi_0044da26;
    /* 0044da10 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da12 */
    f->r[1]=f->r[6];
    /* 0044da14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044da19u;f->boundary(0x0044da14u,0x0044fd30u,f);
    /* 0044da19 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da1a */
    f->r[1]=f->r[6];
    /* 0044da1c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044da20 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da21 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044da26u;f->boundary(0x0044da21u,0x0044c490u,f);
mi_0044da26:;
    /* 0044da26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044da27 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044da2a */
    f->r[4]+=4;return;
}

void restore_event_C_0044da30(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044da30 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044da33 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da34 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da35 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da36 */
    f->r[6]=f->r[1];
    /* 0044da38 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044da3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044da3fu;f->boundary(0x0044da3au,0x0044d680u,f);
    /* 0044da3f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044da41 */
    if((f->flags&MI_ZF))goto mi_0044db0f;
    /* 0044da47 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044da4a */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[6]));
    /* 0044da4e */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044da52 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044da55 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044da58 */
    f->r[0]=MI_U32((0x005d2c4cu+f->r[7]*8u));
    /* 0044da5f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044da61 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044db0f;
    /* 0044da67 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0044da69 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044da8d;
    /* 0044da6b */
    f->r[1]=MI_U32((0x005d2c48u+f->r[7]*8u));
    /* 0044da72 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044da74 */
    MI_U32((0x005d2c48u+f->r[7]*8u))=(uint32_t)(f->r[1]);
    /* 0044da7b */
    f->r[0]=MI_U32((0x005d2c4cu+f->r[7]*8u));
    /* 0044da82 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044da84 */
    MI_U32((0x005d2c4cu+f->r[7]*8u))=(uint32_t)(f->r[0]);
    /* 0044da8b */
    goto mi_0044daa3;
mi_0044da8d:;
    /* 0044da8d */
    MI_U32((0x005d2c48u+f->r[7]*8u))=(uint32_t)(0x00000002u);
    /* 0044da98 */
    MI_U32((0x005d2c4cu+f->r[7]*8u))=(uint32_t)(0x00000000u);
mi_0044daa3:;
    /* 0044daa3 */
    f->r[1]=f->r[6];
    /* 0044daa5 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000cbu);
    /* 0044daac */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000ccu);
    /* 0044dab3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dab8u;f->boundary(0x0044dab3u,0x0044f660u,f);
    /* 0044dab8 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044dabd */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044dac1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dac2 */
    f->r[1]=f->r[6];
    /* 0044dac4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dac9u;f->boundary(0x0044dac4u,0x0044e6f0u,f);
    /* 0044dac9 */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0044dacd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dacf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dad0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dad1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dad2 */
    a=0x00000116u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dad7 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044dae1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dae6u;f->boundary(0x0044dae1u,0x004e0cf0u,f);
    /* 0044dae6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044daea */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044daed */
    f->r[1]=f->r[6];
    /* 0044daef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044daf0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044daf5u;f->boundary(0x0044daf0u,0x004496d0u,f);
    /* 0044daf5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044daf7 */
    if((f->flags&MI_ZF))goto mi_0044db0f;
    /* 0044daf9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dafb */
    f->r[1]=f->r[6];
    /* 0044dafd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044db02u;f->boundary(0x0044dafdu,0x0044fd30u,f);
    /* 0044db02 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db03 */
    f->r[1]=f->r[6];
    /* 0044db05 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044db09 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db0a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044db0fu;f->boundary(0x0044db0au,0x0044c490u,f);
mi_0044db0f:;
    /* 0044db0f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044db10 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044db11 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044db12 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044db15 */
    f->r[4]+=4;return;
}

void restore_event_C_0044db20(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044db20 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044db23 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db24 */
    f->r[6]=f->r[1];
    /* 0044db26 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044db2du;f->boundary(0x0044db28u,0x0044d680u,f);
    /* 0044db2d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044db2f */
    if((f->flags&MI_ZF))goto mi_0044dbca;
    /* 0044db35 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044db38 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044db3b */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044db3e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044db43 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044db49 */
    a=MI_U16((0x00ea5cecu+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044db51 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044dba4;
    /* 0044db53 */
    f->r[1]=f->r[6];
    /* 0044db55 */
    MI_U16((0x00ea5cf0u+f->r[0]))=(uint16_t)(0x00000030u);
    /* 0044db5e */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044db68 */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)(0x000000dbu);
    /* 0044db6f */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)(0x000000dcu);
    /* 0044db76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044db7bu;f->boundary(0x0044db76u,0x0044f660u,f);
    /* 0044db7b */
    MI_U16((0x00000014u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044db80 */
    f->r[0]=(0x00000004u+f->r[4]);
    /* 0044db84 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db85 */
    f->r[1]=f->r[6];
    /* 0044db87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044db8cu;f->boundary(0x0044db87u,0x0044e6f0u,f);
    /* 0044db8c */
    f->r[1]=(0x00000004u+f->r[4]);
    /* 0044db90 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db92 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db93 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db95 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db97 */
    a=0x00000109u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044db9c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dba1u;f->boundary(0x0044db9cu,0x004e0cf0u,f);
    /* 0044dba1 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044dba4:;
    /* 0044dba4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dba8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dba9 */
    f->r[1]=f->r[6];
    /* 0044dbab */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dbb0u;f->boundary(0x0044dbabu,0x004496d0u,f);
    /* 0044dbb0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dbb2 */
    if((f->flags&MI_ZF))goto mi_0044dbca;
    /* 0044dbb4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dbb6 */
    f->r[1]=f->r[6];
    /* 0044dbb8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dbbdu;f->boundary(0x0044dbb8u,0x0044fd30u,f);
    /* 0044dbbd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dbbe */
    f->r[1]=f->r[6];
    /* 0044dbc0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dbc4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dbc5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dbcau;f->boundary(0x0044dbc5u,0x0044c490u,f);
mi_0044dbca:;
    /* 0044dbca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044dbcb */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044dbce */
    f->r[4]+=4;return;
}

void restore_event_C_0044dbd0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044dbd0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044dbd3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dbd4 */
    f->r[6]=f->r[1];
    /* 0044dbd6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dbd8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dbddu;f->boundary(0x0044dbd8u,0x0044d680u,f);
    /* 0044dbdd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dbdf */
    if((f->flags&MI_ZF))goto mi_0044dc93;
    /* 0044dbe5 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044dbe8 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044dbeb */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044dbee */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044dbf3 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044dbf9 */
    a=MI_U16((0x00ea5ceeu+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044dc01 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044dc6d;
    /* 0044dc03 */
    MI_U16((0x00ea5cf2u+f->r[0]))=(uint16_t)(0x00000030u);
    /* 0044dc0c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00784cb0u)))&0xffu)<<0);
    /* 0044dc11 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0044dc13 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044dc1d */
    if(!(f->flags&MI_ZF))goto mi_0044dc2f;
    /* 0044dc1f */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)(0x000000dfu);
    /* 0044dc26 */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)(0x000000e0u);
    /* 0044dc2d */
    goto mi_0044dc3d;
mi_0044dc2f:;
    /* 0044dc2f */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)(0x000000f3u);
    /* 0044dc36 */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)(0x000000f4u);
mi_0044dc3d:;
    /* 0044dc3d */
    f->r[1]=f->r[6];
    /* 0044dc3f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dc44u;f->boundary(0x0044dc3fu,0x0044f660u,f);
    /* 0044dc44 */
    MI_U16((0x00000014u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044dc49 */
    f->r[0]=(0x00000004u+f->r[4]);
    /* 0044dc4d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc4e */
    f->r[1]=f->r[6];
    /* 0044dc50 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dc55u;f->boundary(0x0044dc50u,0x0044e6f0u,f);
    /* 0044dc55 */
    f->r[1]=(0x00000004u+f->r[4]);
    /* 0044dc59 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc5b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc5c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc5e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc60 */
    a=0x00000108u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc65 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dc6au;f->boundary(0x0044dc65u,0x004e0cf0u,f);
    /* 0044dc6a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044dc6d:;
    /* 0044dc6d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dc71 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc72 */
    f->r[1]=f->r[6];
    /* 0044dc74 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dc79u;f->boundary(0x0044dc74u,0x004496d0u,f);
    /* 0044dc79 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dc7b */
    if((f->flags&MI_ZF))goto mi_0044dc93;
    /* 0044dc7d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc7f */
    f->r[1]=f->r[6];
    /* 0044dc81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dc86u;f->boundary(0x0044dc81u,0x0044fd30u,f);
    /* 0044dc86 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc87 */
    f->r[1]=f->r[6];
    /* 0044dc89 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dc8d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dc8e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dc93u;f->boundary(0x0044dc8eu,0x0044c490u,f);
mi_0044dc93:;
    /* 0044dc93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044dc94 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044dc97 */
    f->r[4]+=4;return;
}

void restore_event_C_0044dca0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044dca0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044dca3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dca4 */
    f->r[6]=f->r[1];
    /* 0044dca6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dca8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dcadu;f->boundary(0x0044dca8u,0x0044d680u,f);
    /* 0044dcad */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dcaf */
    if((f->flags&MI_ZF))goto mi_0044dd59;
    /* 0044dcb5 */
    f->r[0]=MI_U32((0x00000028u+f->r[6]));
    /* 0044dcb8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dcb9 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044dcbb */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044dcbe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dcc3u;f->boundary(0x0044dcbeu,0x004c38f0u,f);
    /* 0044dcc3 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0044dcc9 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0044dccf */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044dcd3 */
    f->r[7]=MI_U32((0x00ea447cu+f->r[1]));
    /* 0044dcd9 */
    f->r[1]=(0x00ea447cu+f->r[1]);
    /* 0044dcdf */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044dce1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044dce2 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044dce4 */
    a=f->r[0];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 0044dce7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044dd33;
    /* 0044dce9 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0044dceb */
    f->r[1]=f->r[6];
    /* 0044dced */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)(0x000000e3u);
    /* 0044dcf4 */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)(0x000000e4u);
    /* 0044dcfb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd00u;f->boundary(0x0044dcfbu,0x0044f660u,f);
    /* 0044dd00 */
    f->r[1]=(0x00000004u+f->r[4]);
    /* 0044dd04 */
    MI_U16((0x00000014u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044dd09 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd0a */
    f->r[1]=f->r[6];
    /* 0044dd0c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd11u;f->boundary(0x0044dd0cu,0x0044e6f0u,f);
    /* 0044dd11 */
    f->r[2]=(0x00000004u+f->r[4]);
    /* 0044dd15 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd17 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd18 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd1a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd1c */
    a=0x0000010bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd21 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044dd2b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd30u;f->boundary(0x0044dd2bu,0x004e0cf0u,f);
    /* 0044dd30 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044dd33:;
    /* 0044dd33 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dd37 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd38 */
    f->r[1]=f->r[6];
    /* 0044dd3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd3fu;f->boundary(0x0044dd3au,0x004496d0u,f);
    /* 0044dd3f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dd41 */
    if((f->flags&MI_ZF))goto mi_0044dd59;
    /* 0044dd43 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd45 */
    f->r[1]=f->r[6];
    /* 0044dd47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd4cu;f->boundary(0x0044dd47u,0x0044fd30u,f);
    /* 0044dd4c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dd50 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd51 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd52 */
    f->r[1]=f->r[6];
    /* 0044dd54 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd59u;f->boundary(0x0044dd54u,0x0044c490u,f);
mi_0044dd59:;
    /* 0044dd59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044dd5a */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044dd5d */
    f->r[4]+=4;return;
}

void restore_event_C_0044dd60(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044dd60 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044dd63 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd64 */
    f->r[6]=f->r[1];
    /* 0044dd66 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd68 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd6du;f->boundary(0x0044dd68u,0x0044d680u,f);
    /* 0044dd6d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dd6f */
    if((f->flags&MI_ZF))goto mi_0044de19;
    /* 0044dd75 */
    f->r[0]=MI_U32((0x00000028u+f->r[6]));
    /* 0044dd78 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dd79 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044dd7b */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044dd7e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dd83u;f->boundary(0x0044dd7eu,0x004c38f0u,f);
    /* 0044dd83 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0044dd89 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0044dd8f */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044dd93 */
    f->r[2]=(0x00ea447cu+f->r[1]);
    /* 0044dd99 */
    f->r[1]=MI_U32((0x00ea447cu+f->r[1]));
    /* 0044dd9f */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044dda1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044dda2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044dda4 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0044dda7 */
    if(mi_signed_less(f->flags))goto mi_0044ddf3;
    /* 0044dda9 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0044ddab */
    f->r[1]=f->r[6];
    /* 0044ddad */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044ddb7 */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)(0x000000e7u);
    /* 0044ddbe */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)(0x000000e8u);
    /* 0044ddc5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ddcau;f->boundary(0x0044ddc5u,0x0044f660u,f);
    /* 0044ddca */
    f->r[1]=(0x00000004u+f->r[4]);
    /* 0044ddce */
    MI_U16((0x00000014u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ddd3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ddd4 */
    f->r[1]=f->r[6];
    /* 0044ddd6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dddbu;f->boundary(0x0044ddd6u,0x0044e6f0u,f);
    /* 0044dddb */
    f->r[2]=(0x00000004u+f->r[4]);
    /* 0044dddf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dde1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dde2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dde4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dde6 */
    a=0x0000010cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ddeb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ddf0u;f->boundary(0x0044ddebu,0x004e0cf0u,f);
    /* 0044ddf0 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044ddf3:;
    /* 0044ddf3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044ddf7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ddf8 */
    f->r[1]=f->r[6];
    /* 0044ddfa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ddffu;f->boundary(0x0044ddfau,0x004496d0u,f);
    /* 0044ddff */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044de01 */
    if((f->flags&MI_ZF))goto mi_0044de19;
    /* 0044de03 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de05 */
    f->r[1]=f->r[6];
    /* 0044de07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044de0cu;f->boundary(0x0044de07u,0x0044fd30u,f);
    /* 0044de0c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044de10 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de11 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de12 */
    f->r[1]=f->r[6];
    /* 0044de14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044de19u;f->boundary(0x0044de14u,0x0044c490u,f);
mi_0044de19:;
    /* 0044de19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044de1a */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044de1d */
    f->r[4]+=4;return;
}

void restore_event_C_0044de20(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044de20 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044de23 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de24 */
    f->r[6]=f->r[1];
    /* 0044de26 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044de2du;f->boundary(0x0044de28u,0x0044d680u,f);
    /* 0044de2d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044de2f */
    if((f->flags&MI_ZF))goto mi_0044ded9;
    /* 0044de35 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044de38 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044de3b */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044de3e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 0044de43 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0044de49 */
    f->r[0]=MI_U32((0x00ea1888u+f->r[0]));
    /* 0044de4f */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,1);
    /* 0044de52 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044de58;
    /* 0044de54 */
    a=0xffffffceu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de56 */
    goto mi_0044de63;
mi_0044de58:;
    /* 0044de58 */
    a=f->r[0];b=0x0000003cu;t=mi_alu(f,a,b,32,1);
    /* 0044de5b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044de61;
    /* 0044de5d */
    a=0xffffffd8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de5f */
    goto mi_0044de63;
mi_0044de61:;
    /* 0044de61 */
    a=0xffffffe7u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0044de63:;
    /* 0044de63 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044de68u;f->boundary(0x0044de63u,0x004e85d0u,f);
    /* 0044de68 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044de6b */
    f->r[1]=f->r[6];
    /* 0044de6d */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044de77 */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)(0x000000ebu);
    /* 0044de7e */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)(0x000000ecu);
    /* 0044de85 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044de8au;f->boundary(0x0044de85u,0x0044f660u,f);
    /* 0044de8a */
    f->r[1]=(0x00000004u+f->r[4]);
    /* 0044de8e */
    MI_U16((0x00000014u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044de93 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044de94 */
    f->r[1]=f->r[6];
    /* 0044de96 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044de9bu;f->boundary(0x0044de96u,0x0044e6f0u,f);
    /* 0044de9b */
    f->r[2]=(0x00000004u+f->r[4]);
    /* 0044de9f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dea1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dea2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dea4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dea6 */
    a=0x0000010du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044deab */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044deb0u;f->boundary(0x0044deabu,0x004e0cf0u,f);
    /* 0044deb0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044deb4 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044deb7 */
    f->r[1]=f->r[6];
    /* 0044deb9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044deba */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044debfu;f->boundary(0x0044debau,0x004496d0u,f);
    /* 0044debf */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dec1 */
    if((f->flags&MI_ZF))goto mi_0044ded9;
    /* 0044dec3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dec5 */
    f->r[1]=f->r[6];
    /* 0044dec7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044deccu;f->boundary(0x0044dec7u,0x0044fd30u,f);
    /* 0044decc */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044ded0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ded1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ded2 */
    f->r[1]=f->r[6];
    /* 0044ded4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ded9u;f->boundary(0x0044ded4u,0x0044c490u,f);
mi_0044ded9:;
    /* 0044ded9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044deda */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044dedd */
    f->r[4]+=4;return;
}

void restore_event_C_0044dee0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044dee0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044dee3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dee4 */
    f->r[6]=f->r[1];
    /* 0044dee6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dee8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044deedu;f->boundary(0x0044dee8u,0x0044d680u,f);
    /* 0044deed */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044deef */
    if((f->flags&MI_ZF))goto mi_0044df80;
    /* 0044def5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044def6 */
    f->r[7]=MI_U32((0x00000028u+f->r[6]));
    /* 0044def9 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0044defc */
    a=0x000000a1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df01 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[7]);
    /* 0044df04 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df09u;f->boundary(0x0044df04u,0x00469f80u,f);
    /* 0044df09 */
    f->r[7]=f->r[0];
    /* 0044df0b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044df0e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0044df10 */
    if((f->flags&MI_ZF))goto mi_0044df59;
    /* 0044df12 */
    f->r[1]=f->r[6];
    /* 0044df14 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044df1e */
    MI_U16((0x0000000au+f->r[4]))=(uint16_t)(0x000000efu);
    /* 0044df25 */
    MI_U16((0x00000008u+f->r[4]))=(uint16_t)(0x000000f0u);
    /* 0044df2c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df31u;f->boundary(0x0044df2cu,0x0044f660u,f);
    /* 0044df31 */
    MI_U16((0x00000018u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044df36 */
    f->r[0]=(0x00000008u+f->r[4]);
    /* 0044df3a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df3b */
    f->r[1]=f->r[6];
    /* 0044df3d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df42u;f->boundary(0x0044df3du,0x0044e6f0u,f);
    /* 0044df42 */
    f->r[1]=(0x00000008u+f->r[4]);
    /* 0044df46 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df48 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df49 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df4a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df4c */
    a=0x0000010eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df51 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df56u;f->boundary(0x0044df51u,0x004e0cf0u,f);
    /* 0044df56 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044df59:;
    /* 0044df59 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044df5d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df5e */
    f->r[1]=f->r[6];
    /* 0044df60 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df65u;f->boundary(0x0044df60u,0x004496d0u,f);
    /* 0044df65 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044df67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044df68 */
    if((f->flags&MI_ZF))goto mi_0044df80;
    /* 0044df6a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df6c */
    f->r[1]=f->r[6];
    /* 0044df6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df73u;f->boundary(0x0044df6eu,0x0044fd30u,f);
    /* 0044df73 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df74 */
    f->r[1]=f->r[6];
    /* 0044df76 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044df7a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df80u;f->boundary(0x0044df7bu,0x0044c490u,f);
mi_0044df80:;
    /* 0044df80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044df81 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044df84 */
    f->r[4]+=4;return;
}

void restore_event_C_0044df90(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044df90 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df91 */
    f->r[6]=f->r[1];
    /* 0044df93 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044df95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044df9au;f->boundary(0x0044df95u,0x0044d680u,f);
    /* 0044df9a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044df9c */
    if((f->flags&MI_ZF))goto mi_0044dff9;
    /* 0044df9e */
    f->r[1]=MI_U32((0x00000028u+f->r[6]));
    /* 0044dfa1 */
    a=0x0000006du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfa3 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 0044dfa6 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0044dfa9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dfaeu;f->boundary(0x0044dfa9u,0x00469f80u,f);
    /* 0044dfae */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044dfb1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044dfb3 */
    if((f->flags&MI_ZF))goto mi_0044dfd3;
    /* 0044dfb5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfb7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfb9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfbc */
    a=0x0000010fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfc1 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044dfcb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dfd0u;f->boundary(0x0044dfcbu,0x004e0cf0u,f);
    /* 0044dfd0 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044dfd3:;
    /* 0044dfd3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dfd7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfd8 */
    f->r[1]=f->r[6];
    /* 0044dfda */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dfdfu;f->boundary(0x0044dfdau,0x004496d0u,f);
    /* 0044dfdf */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044dfe1 */
    if((f->flags&MI_ZF))goto mi_0044dff9;
    /* 0044dfe3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dfe5 */
    f->r[1]=f->r[6];
    /* 0044dfe7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dfecu;f->boundary(0x0044dfe7u,0x0044fd30u,f);
    /* 0044dfec */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044dff0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dff1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044dff2 */
    f->r[1]=f->r[6];
    /* 0044dff4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044dff9u;f->boundary(0x0044dff4u,0x0044c490u,f);
mi_0044dff9:;
    /* 0044dff9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044dffa */
    f->r[4]+=4;return;
}

void restore_event_C_0044e000(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e000 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e001 */
    f->r[6]=f->r[1];
    /* 0044e003 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e005 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e00au;f->boundary(0x0044e005u,0x0044d680u,f);
    /* 0044e00a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e00c */
    if((f->flags&MI_ZF))goto mi_0044e060;
    /* 0044e00e */
    f->r[0]=MI_U32((0x00000028u+f->r[6]));
    /* 0044e011 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e013 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044e016 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e01bu;f->boundary(0x0044e016u,0x004e6eb0u,f);
    /* 0044e01b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e01d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e01f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e021 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e023 */
    a=0x000001e3u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e028 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e032 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e037u;f->boundary(0x0044e032u,0x004e0cf0u,f);
    /* 0044e037 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e03b */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e03e */
    f->r[1]=f->r[6];
    /* 0044e040 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e041 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e046u;f->boundary(0x0044e041u,0x004496d0u,f);
    /* 0044e046 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e048 */
    if((f->flags&MI_ZF))goto mi_0044e060;
    /* 0044e04a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e04c */
    f->r[1]=f->r[6];
    /* 0044e04e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e053u;f->boundary(0x0044e04eu,0x0044fd30u,f);
    /* 0044e053 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e057 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e058 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e059 */
    f->r[1]=f->r[6];
    /* 0044e05b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e060u;f->boundary(0x0044e05bu,0x0044c490u,f);
mi_0044e060:;
    /* 0044e060 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e061 */
    f->r[4]+=4;return;
}

void restore_event_C_0044e070(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e070 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e071 */
    f->r[6]=f->r[1];
    /* 0044e073 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e075 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e07au;f->boundary(0x0044e075u,0x0044d680u,f);
    /* 0044e07a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e07c */
    if((f->flags&MI_ZF))goto mi_0044e0d6;
    /* 0044e07e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044e082 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044e085 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e086 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044e089 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044e08c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e091u;f->boundary(0x0044e08cu,0x004e70b0u,f);
    /* 0044e091 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e093 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e095 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e097 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e099 */
    a=0x000001e4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e09e */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e0a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e0adu;f->boundary(0x0044e0a8u,0x004e0cf0u,f);
    /* 0044e0ad */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e0b1 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e0b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0b5 */
    f->r[1]=f->r[6];
    /* 0044e0b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e0bcu;f->boundary(0x0044e0b7u,0x004496d0u,f);
    /* 0044e0bc */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e0be */
    if((f->flags&MI_ZF))goto mi_0044e0d6;
    /* 0044e0c0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0c2 */
    f->r[1]=f->r[6];
    /* 0044e0c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e0c9u;f->boundary(0x0044e0c4u,0x0044fd30u,f);
    /* 0044e0c9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e0cd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0ce */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0cf */
    f->r[1]=f->r[6];
    /* 0044e0d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e0d6u;f->boundary(0x0044e0d1u,0x0044c490u,f);
mi_0044e0d6:;
    /* 0044e0d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e0d7 */
    f->r[4]+=4;return;
}

void restore_event_C_0044e0e0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e0e0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0e1 */
    f->r[6]=f->r[1];
    /* 0044e0e3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0e5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e0eau;f->boundary(0x0044e0e5u,0x0044d680u,f);
    /* 0044e0ea */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e0ec */
    if((f->flags&MI_ZF))goto mi_0044e140;
    /* 0044e0ee */
    f->r[0]=MI_U32((0x00000028u+f->r[6]));
    /* 0044e0f1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0f3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0f5 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e0f7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0f9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e0fb */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044e0fe */
    a=0x000001e5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e103 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e10d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e112u;f->boundary(0x0044e10du,0x004e0cf0u,f);
    /* 0044e112 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e115 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e11au;f->boundary(0x0044e115u,0x004e6ed0u,f);
    /* 0044e11a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e11e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e11f */
    f->r[1]=f->r[6];
    /* 0044e121 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e126u;f->boundary(0x0044e121u,0x004496d0u,f);
    /* 0044e126 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e128 */
    if((f->flags&MI_ZF))goto mi_0044e140;
    /* 0044e12a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e12c */
    f->r[1]=f->r[6];
    /* 0044e12e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e133u;f->boundary(0x0044e12eu,0x0044fd30u,f);
    /* 0044e133 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e137 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e138 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e139 */
    f->r[1]=f->r[6];
    /* 0044e13b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e140u;f->boundary(0x0044e13bu,0x0044c490u,f);
mi_0044e140:;
    /* 0044e140 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e141 */
    f->r[4]+=4;return;
}

void restore_event_C_0044e150(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e150 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e151 */
    f->r[6]=f->r[1];
    /* 0044e153 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e155 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e15au;f->boundary(0x0044e155u,0x0044d680u,f);
    /* 0044e15a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e15c */
    if((f->flags&MI_ZF))goto mi_0044e1b6;
    /* 0044e15e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044e162 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044e165 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e166 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044e169 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044e16c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e171u;f->boundary(0x0044e16cu,0x004e7200u,f);
    /* 0044e171 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e173 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e175 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e177 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e179 */
    a=0x000001e6u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e17e */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e188 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e18du;f->boundary(0x0044e188u,0x004e0cf0u,f);
    /* 0044e18d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e191 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e194 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e195 */
    f->r[1]=f->r[6];
    /* 0044e197 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e19cu;f->boundary(0x0044e197u,0x004496d0u,f);
    /* 0044e19c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e19e */
    if((f->flags&MI_ZF))goto mi_0044e1b6;
    /* 0044e1a0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1a2 */
    f->r[1]=f->r[6];
    /* 0044e1a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e1a9u;f->boundary(0x0044e1a4u,0x0044fd30u,f);
    /* 0044e1a9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e1ad */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1ae */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1af */
    f->r[1]=f->r[6];
    /* 0044e1b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e1b6u;f->boundary(0x0044e1b1u,0x0044c490u,f);
mi_0044e1b6:;
    /* 0044e1b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e1b7 */
    f->r[4]+=4;return;
}

void restore_event_C_0044e1c0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e1c0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1c1 */
    f->r[6]=f->r[1];
    /* 0044e1c3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e1cau;f->boundary(0x0044e1c5u,0x0044d680u,f);
    /* 0044e1ca */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e1cc */
    if((f->flags&MI_ZF))goto mi_0044e226;
    /* 0044e1ce */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044e1d2 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044e1d5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1d6 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044e1d9 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044e1dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e1e1u;f->boundary(0x0044e1dcu,0x00469c60u,f);
    /* 0044e1e1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1e3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1e5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1e7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1e9 */
    a=0x000001ebu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e1ee */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e1f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e1fdu;f->boundary(0x0044e1f8u,0x004e0cf0u,f);
    /* 0044e1fd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e201 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e204 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e205 */
    f->r[1]=f->r[6];
    /* 0044e207 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e20cu;f->boundary(0x0044e207u,0x004496d0u,f);
    /* 0044e20c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e20e */
    if((f->flags&MI_ZF))goto mi_0044e226;
    /* 0044e210 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e212 */
    f->r[1]=f->r[6];
    /* 0044e214 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e219u;f->boundary(0x0044e214u,0x0044fd30u,f);
    /* 0044e219 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e21d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e21e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e21f */
    f->r[1]=f->r[6];
    /* 0044e221 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e226u;f->boundary(0x0044e221u,0x0044c490u,f);
mi_0044e226:;
    /* 0044e226 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e227 */
    f->r[4]+=4;return;
}

void restore_event_C_0044e230(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e230 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e231 */
    f->r[6]=f->r[1];
    /* 0044e233 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e235 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e23au;f->boundary(0x0044e235u,0x0044d680u,f);
    /* 0044e23a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e23c */
    if((f->flags&MI_ZF))goto mi_0044e296;
    /* 0044e23e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044e242 */
    f->r[2]=MI_U32((0x00000028u+f->r[6]));
    /* 0044e245 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e246 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044e249 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044e24c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e251u;f->boundary(0x0044e24cu,0x004e7ac0u,f);
    /* 0044e251 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e253 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e255 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e257 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e259 */
    a=0x000001f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e25e */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e268 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e26du;f->boundary(0x0044e268u,0x004e0cf0u,f);
    /* 0044e26d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e271 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e274 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e275 */
    f->r[1]=f->r[6];
    /* 0044e277 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e27cu;f->boundary(0x0044e277u,0x004496d0u,f);
    /* 0044e27c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e27e */
    if((f->flags&MI_ZF))goto mi_0044e296;
    /* 0044e280 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e282 */
    f->r[1]=f->r[6];
    /* 0044e284 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e289u;f->boundary(0x0044e284u,0x0044fd30u,f);
    /* 0044e289 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044e28d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e28e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e28f */
    f->r[1]=f->r[6];
    /* 0044e291 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e296u;f->boundary(0x0044e291u,0x0044c490u,f);
mi_0044e296:;
    /* 0044e296 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e297 */
    f->r[4]+=4;return;
}

void restore_event_C_0044e2a0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e2a0 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044e2a3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e2a4 */
    f->r[3]=f->r[1];
    /* 0044e2a6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e2a8 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0044e2ac */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000083u);
    /* 0044e2b4 */
    MI_U8((0x0000000bu+f->r[4]))=(uint8_t)(0x00000000u);
    /* 0044e2b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e2beu;f->boundary(0x0044e2b9u,0x0044d680u,f);
    /* 0044e2be */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e2c0 */
    if((f->flags&MI_ZF))goto mi_0044e5b6;
    /* 0044e2c6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000001cu+f->r[3]),2u);
    /* 0044e2ca */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e2cb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e2cc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e2cd */
    f->r[7]=mi_read((0x00000028u+f->r[3]),4u);
    /* 0044e2d0 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0044e2d3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e2d4 */
    MI_U32((0x00000028u+f->r[3]))=(uint32_t)(f->r[7]);
    /* 0044e2d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e2dcu;f->boundary(0x0044e2d7u,0x004449d0u,f);
    /* 0044e2dc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e2df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044e2e1 */
    if((f->flags&MI_ZF))goto mi_0044e58d;
    /* 0044e2e7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044e2ea */
    f->r[7]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0044e2ed */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0044e2ef */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0044e2f1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044e2f3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[3]),1u))&0xffu)<<0);
    /* 0044e2f6 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0044e2f8 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0044e2fb */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x0077de56u+f->r[7]),2u);
    /* 0044e302 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044e58d;
    /* 0044e308 */
    switch(mi_read((0x0044e5bcu+f->r[0]*4u),4u)){case 0x0044e30fu:goto mi_0044e30f;case 0x0044e361u:goto mi_0044e361;case 0x0044e3b3u:goto mi_0044e3b3;case 0x0044e41cu:goto mi_0044e41c;case 0x0044e465u:goto mi_0044e465;default:abort();}
mi_0044e30f:;
    /* 0044e30f */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 0044e313 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e316 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044e318 */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e31a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0077f760u+f->r[6]),1u))&0xffu)<<0);
    /* 0044e320 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0044e322 */
    if(!(f->flags&MI_ZF))goto mi_0044e32b;
    /* 0044e324 */
    f->r[3]=0x00000005u;
    /* 0044e329 */
    goto mi_0044e338;
mi_0044e32b:;
    /* 0044e32b */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0044e32d */
    if(!(f->flags&MI_ZF))goto mi_0044e58d;
    /* 0044e333 */
    f->r[3]=0x00000006u;
mi_0044e338:;
    /* 0044e338 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0044e33c */
    MI_U16((0x0000001eu+f->r[4]))=(uint16_t)(0x0000007eu);
    /* 0044e343 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x0000007fu);
    /* 0044e34a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e34fu;f->boundary(0x0044e34au,0x0044f660u,f);
    /* 0044e34f */
    MI_U16((0x0000002cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e354 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000152u);
    /* 0044e35c */
    goto mi_0044e4ba;
mi_0044e361:;
    /* 0044e361 */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 0044e365 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e368 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044e36a */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e36c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0077f760u+f->r[6]),1u))&0xffu)<<0);
    /* 0044e372 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0044e374 */
    if(!(f->flags&MI_ZF))goto mi_0044e37d;
    /* 0044e376 */
    f->r[3]=0x00000003u;
    /* 0044e37b */
    goto mi_0044e38a;
mi_0044e37d:;
    /* 0044e37d */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0044e37f */
    if(!(f->flags&MI_ZF))goto mi_0044e58d;
    /* 0044e385 */
    f->r[3]=0x00000004u;
mi_0044e38a:;
    /* 0044e38a */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0044e38e */
    MI_U16((0x0000001eu+f->r[4]))=(uint16_t)(0x00000082u);
    /* 0044e395 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x00000083u);
    /* 0044e39c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e3a1u;f->boundary(0x0044e39cu,0x0044f660u,f);
    /* 0044e3a1 */
    MI_U16((0x0000002cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e3a6 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000153u);
    /* 0044e3ae */
    goto mi_0044e4ba;
mi_0044e3b3:;
    /* 0044e3b3 */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 0044e3b7 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e3ba */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044e3bc */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e3be */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0077f760u+f->r[6]),1u))&0xffu)<<0);
    /* 0044e3c4 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0044e3c6 */
    if(!(f->flags&MI_ZF))goto mi_0044e3e0;
    /* 0044e3c8 */
    f->r[3]=0x00000005u;
    /* 0044e3cd */
    MI_U16((0x0000001eu+f->r[4]))=(uint16_t)(0x00000086u);
    /* 0044e3d4 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x00000087u);
    /* 0044e3db */
    goto mi_0044e4ac;
mi_0044e3e0:;
    /* 0044e3e0 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0044e3e2 */
    if(!(f->flags&MI_ZF))goto mi_0044e3fc;
    /* 0044e3e4 */
    f->r[3]=0x00000003u;
    /* 0044e3e9 */
    MI_U16((0x0000001eu+f->r[4]))=(uint16_t)(0x00000086u);
    /* 0044e3f0 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x00000087u);
    /* 0044e3f7 */
    goto mi_0044e4ac;
mi_0044e3fc:;
    /* 0044e3fc */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0044e3fe */
    if(!(f->flags&MI_ZF))goto mi_0044e58d;
    /* 0044e404 */
    f->r[3]=0x00000001u;
    /* 0044e409 */
    MI_U16((0x0000001eu+f->r[4]))=(uint16_t)(0x00000086u);
    /* 0044e410 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x00000087u);
    /* 0044e417 */
    goto mi_0044e4ac;
mi_0044e41c:;
    /* 0044e41c */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 0044e420 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e423 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044e425 */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e427 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0077f760u+f->r[6]),1u))&0xffu)<<0);
    /* 0044e42d */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0044e42f */
    if(!(f->flags&MI_ZF))goto mi_0044e438;
    /* 0044e431 */
    f->r[3]=0x00000006u;
    /* 0044e436 */
    goto mi_0044e450;
mi_0044e438:;
    /* 0044e438 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0044e43a */
    if(!(f->flags&MI_ZF))goto mi_0044e443;
    /* 0044e43c */
    f->r[3]=0x00000004u;
    /* 0044e441 */
    goto mi_0044e450;
mi_0044e443:;
    /* 0044e443 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0044e445 */
    if(!(f->flags&MI_ZF))goto mi_0044e58d;
    /* 0044e44b */
    f->r[3]=0x00000002u;
mi_0044e450:;
    /* 0044e450 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 0044e455 */
    MI_U16((0x0000001eu+f->r[4]))=(uint16_t)(0x0000008au);
    /* 0044e45c */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x0000008bu);
    /* 0044e463 */
    goto mi_0044e4ac;
mi_0044e465:;
    /* 0044e465 */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 0044e469 */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e46c */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044e46e */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0044e470 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0077f760u+f->r[6]),1u))&0xffu)<<0);
    /* 0044e476 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0044e478 */
    if(!(f->flags&MI_ZF))goto mi_0044e481;
    /* 0044e47a */
    f->r[3]=0x00000006u;
    /* 0044e47f */
    goto mi_0044e499;
mi_0044e481:;
    /* 0044e481 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0044e483 */
    if(!(f->flags&MI_ZF))goto mi_0044e48c;
    /* 0044e485 */
    f->r[3]=0x00000004u;
    /* 0044e48a */
    goto mi_0044e499;
mi_0044e48c:;
    /* 0044e48c */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0044e48e */
    if(!(f->flags&MI_ZF))goto mi_0044e58d;
    /* 0044e494 */
    f->r[3]=0x00000002u;
mi_0044e499:;
    /* 0044e499 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 0044e49e */
    MI_U16((0x0000001eu+f->r[4]))=(uint16_t)(0x0000008eu);
    /* 0044e4a5 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x0000008fu);
mi_0044e4ac:;
    /* 0044e4ac */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0044e4b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e4b5u;f->boundary(0x0044e4b0u,0x0044f660u,f);
    /* 0044e4b5 */
    MI_U16((0x0000002cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
mi_0044e4ba:;
    /* 0044e4ba */
    f->r[1]=(0x0000001cu+f->r[4]);
    /* 0044e4be */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e4bf */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0044e4c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e4c8u;f->boundary(0x0044e4c3u,0x0044e6f0u,f);
    /* 0044e4c8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e4c9 */
    MI_U8((0x0077f760u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0044e4cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e4d4u;f->boundary(0x0044e4cfu,0x004425c0u,f);
    /* 0044e4d4 */
    MI_U16((0x0077f754u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e4db */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000017u+f->r[4]),1u))&0xffu)<<0);
    /* 0044e4df */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0077f760u+f->r[6]),1u))&0xffu)<<0);
    /* 0044e4e5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e4e8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e4ea */
    MI_U8((0x0077de02u+f->r[7]))=(uint8_t)((f->r[2]&0xffu));
    /* 0044e4f0 */
    if((f->flags&MI_ZF))goto mi_0044e4f9;
    /* 0044e4f2 */
    MI_U8((0x0077de05u+f->r[7]))=(uint8_t)(0x00000000u);
mi_0044e4f9:;
    /* 0044e4f9 */
    f->r[6]=0x00000001u;
mi_0044e4fe:;
    /* 0044e4fe */
    MI_U8((0x0077de2au+f->r[7]+f->r[6]*1u))=(uint8_t)(0x00000000u);
    /* 0044e506 */
    MI_U8((0x0077de06u+f->r[7]+f->r[6]*1u))=(uint8_t)(0x00000000u);
    /* 0044e50e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x0077de02u+f->r[7]),1u))&0xffu)<<0);
    /* 0044e514 */
    f->r[0]=f->r[1];
    /* 0044e516 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0044e51b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044e51c */
    if((f->flags&MI_ZF))goto mi_0044e52c;
    /* 0044e51e */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044e521 */
    if((f->flags&MI_ZF))goto mi_0044e52c;
    /* 0044e523 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044e526 */
    if((f->flags&MI_ZF))goto mi_0044e52c;
    /* 0044e528 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e52a */
    if(!(f->flags&MI_ZF))goto mi_0044e558;
mi_0044e52c:;
    /* 0044e52c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e52d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e52e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e533u;f->boundary(0x0044e52eu,0x00443000u,f);
    /* 0044e533 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e536 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044e538 */
    if((f->flags&MI_ZF))goto mi_0044e542;
    /* 0044e53a */
    MI_U8((0x0077de2au+f->r[7]+f->r[6]*1u))=(uint8_t)(0x00000001u);
mi_0044e542:;
    /* 0044e542 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e543 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e544 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e549u;f->boundary(0x0044e544u,0x00443040u,f);
    /* 0044e549 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e54c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044e54e */
    if((f->flags&MI_ZF))goto mi_0044e558;
    /* 0044e550 */
    MI_U8((0x0077de06u+f->r[7]+f->r[6]*1u))=(uint8_t)(0x00000001u);
mi_0044e558:;
    /* 0044e558 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e559 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e55eu;f->boundary(0x0044e559u,0x005599c0u,f);
    /* 0044e55e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e561 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0044e562 */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0044e565 */
    if(mi_signed_less(f->flags))goto mi_0044e4fe;
    /* 0044e567 */
    f->r[1]=mi_read((0x00000018u+f->r[4]),4u);
    /* 0044e56b */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 0044e56f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e571 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e572 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e574 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e576 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e577 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e581 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e586u;f->boundary(0x0044e581u,0x004e0cf0u,f);
    /* 0044e586 */
    f->r[3]=mi_read((0x00000028u+f->r[4]),4u);
    /* 0044e58a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044e58d:;
    /* 0044e58d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[3]),2u);
    /* 0044e591 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e592 */
    f->r[1]=f->r[3];
    /* 0044e594 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e599u;f->boundary(0x0044e594u,0x004496d0u,f);
    /* 0044e599 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044e59a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e59b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e59d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044e59e */
    if((f->flags&MI_ZF))goto mi_0044e5b6;
    /* 0044e5a0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e5a2 */
    f->r[1]=f->r[3];
    /* 0044e5a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e5a9u;f->boundary(0x0044e5a4u,0x0044fd30u,f);
    /* 0044e5a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e5aa */
    f->r[1]=f->r[3];
    /* 0044e5ac */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[3]),2u);
    /* 0044e5b0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e5b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e5b6u;f->boundary(0x0044e5b1u,0x0044c490u,f);
mi_0044e5b6:;
    /* 0044e5b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044e5b7 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e5ba */
    f->r[4]+=4;return;
}

void restore_event_C_0044e5d0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e5d0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044e5d3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e5d4 */
    f->r[6]=f->r[1];
    /* 0044e5d6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e5d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e5ddu;f->boundary(0x0044e5d8u,0x0044d680u,f);
    /* 0044e5dd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e5df */
    if((f->flags&MI_ZF))goto mi_0044e6d8;
    /* 0044e5e5 */
    f->r[2]=mi_read((0x00000028u+f->r[6]),4u);
    /* 0044e5e8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044e5ea */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044e5ed */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e5ee */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0044e5f1 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e5fb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[6]),1u))&0xffu)<<0);
    /* 0044e5fe */
    f->r[7]=0x00000083u;
    /* 0044e603 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044e606 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044e67f;
    /* 0044e608 */
    switch(mi_read((0x0044e6e0u+f->r[0]*4u),4u)){case 0x0044e60fu:goto mi_0044e60f;case 0x0044e630u:goto mi_0044e630;case 0x0044e651u:goto mi_0044e651;case 0x0044e668u:goto mi_0044e668;default:abort();}
mi_0044e60f:;
    /* 0044e60f */
    f->r[1]=f->r[6];
    /* 0044e611 */
    MI_U16((0x0000000au+f->r[4]))=(uint16_t)(0x00000092u);
    /* 0044e618 */
    MI_U16((0x00000008u+f->r[4]))=(uint16_t)(0x00000093u);
    /* 0044e61f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e624u;f->boundary(0x0044e61fu,0x0044f660u,f);
    /* 0044e624 */
    MI_U16((0x00000018u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e629 */
    f->r[7]=0x00000106u;
    /* 0044e62e */
    goto mi_0044e691;
mi_0044e630:;
    /* 0044e630 */
    f->r[1]=f->r[6];
    /* 0044e632 */
    MI_U16((0x0000000au+f->r[4]))=(uint16_t)(0x00000096u);
    /* 0044e639 */
    MI_U16((0x00000008u+f->r[4]))=(uint16_t)(0x00000097u);
    /* 0044e640 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e645u;f->boundary(0x0044e640u,0x0044f660u,f);
    /* 0044e645 */
    MI_U16((0x00000018u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e64a */
    f->r[7]=0x00000107u;
    /* 0044e64f */
    goto mi_0044e691;
mi_0044e651:;
    /* 0044e651 */
    f->r[1]=f->r[6];
    /* 0044e653 */
    MI_U16((0x0000000au+f->r[4]))=(uint16_t)(0x0000009au);
    /* 0044e65a */
    MI_U16((0x00000008u+f->r[4]))=(uint16_t)(0x0000009bu);
    /* 0044e661 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e666u;f->boundary(0x0044e661u,0x0044f660u,f);
    /* 0044e666 */
    goto mi_0044e68c;
mi_0044e668:;
    /* 0044e668 */
    f->r[1]=f->r[6];
    /* 0044e66a */
    MI_U16((0x0000000au+f->r[4]))=(uint16_t)(0x0000009eu);
    /* 0044e671 */
    MI_U16((0x00000008u+f->r[4]))=(uint16_t)(0x0000009fu);
    /* 0044e678 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e67du;f->boundary(0x0044e678u,0x0044f660u,f);
    /* 0044e67d */
    goto mi_0044e68c;
mi_0044e67f:;
    /* 0044e67f */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044e682 */
    MI_U16((0x00000008u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e687 */
    MI_U16((0x0000000au+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
mi_0044e68c:;
    /* 0044e68c */
    MI_U16((0x00000018u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
mi_0044e691:;
    /* 0044e691 */
    f->r[0]=(0x00000008u+f->r[4]);
    /* 0044e695 */
    f->r[1]=f->r[6];
    /* 0044e697 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e698 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e69du;f->boundary(0x0044e698u,0x0044e6f0u,f);
    /* 0044e69d */
    f->r[1]=(0x00000008u+f->r[4]);
    /* 0044e6a1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6a3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6a4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6a6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6a8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e6aeu;f->boundary(0x0044e6a9u,0x004e0cf0u,f);
    /* 0044e6ae */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0044e6b2 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e6b5 */
    f->r[1]=f->r[6];
    /* 0044e6b7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e6bdu;f->boundary(0x0044e6b8u,0x004496d0u,f);
    /* 0044e6bd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e6bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044e6c0 */
    if((f->flags&MI_ZF))goto mi_0044e6d8;
    /* 0044e6c2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6c4 */
    f->r[1]=f->r[6];
    /* 0044e6c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e6cbu;f->boundary(0x0044e6c6u,0x0044fd30u,f);
    /* 0044e6cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6cc */
    f->r[1]=f->r[6];
    /* 0044e6ce */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0044e6d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e6d8u;f->boundary(0x0044e6d3u,0x0044c490u,f);
mi_0044e6d8:;
    /* 0044e6d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e6d9 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e6dc */
    f->r[4]+=4;return;
}

void restore_event_C_0044e6f0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e6f0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6f1 */
    f->r[6]=f->r[1];
    /* 0044e6f3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6f4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 0044e6f8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e6f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e6feu;f->boundary(0x0044e6f9u,0x004449d0u,f);
    /* 0044e6fe */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0044e702 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e705 */
    MI_U16((0x00000006u+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e709 */
    MI_U16((0x00000012u+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e70d */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[6])))&0xffffu)<<0);
    /* 0044e711 */
    MI_U16((0x00000018u+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044e715 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044e719 */
    MI_U16((0x0000001au+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044e71d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000036u+f->r[6])))&0xffu)<<0);
    /* 0044e720 */
    MI_U8((0x0000001du+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 0044e723 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000034u+f->r[6]));
    /* 0044e727 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e728 */
    f->r[1]=f->r[6];
    /* 0044e72a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e72fu;f->boundary(0x0044e72au,0x0044c0d0u,f);
    /* 0044e72f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044e731 */
    if((f->flags&MI_ZF))goto mi_0044e76f;
    /* 0044e733 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000064u+f->r[6])))&0xffffu)<<0);
    /* 0044e737 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044e73a */
    if(mi_signed_less(f->flags))goto mi_0044e740;
    /* 0044e73c */
    MI_U16((0x00000010u+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
mi_0044e740:;
    /* 0044e740 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000063u+f->r[6])))&0xffu)<<0);
    /* 0044e743 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e745 */
    if((f->flags&MI_ZF))goto mi_0044e759;
    /* 0044e747 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0044e74c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e74d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e752u;f->boundary(0x0044e74du,0x004449d0u,f);
    /* 0044e752 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e755 */
    MI_U16((0x00000012u+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
mi_0044e759:;
    /* 0044e759 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000068u+f->r[6])))&0xffffu)<<0);
    /* 0044e75d */
    MI_U16((0x00000018u+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044e761 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000066u+f->r[6])))&0xffffu)<<0);
    /* 0044e765 */
    MI_U16((0x0000001au+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e769 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000037u+f->r[6])))&0xffu)<<0);
    /* 0044e76c */
    MI_U8((0x0000001du+f->r[7]))=(uint8_t)((f->r[1]&0xffu));
mi_0044e76f:;
    /* 0044e76f */
    MI_U16((0x00000004u+f->r[7]))=(uint16_t)(0x0000ffffu);
    /* 0044e775 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[6])))&0xffffu)<<0);
    /* 0044e779 */
    MI_U16((0x00000008u+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044e77d */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044e781 */
    MI_U16((0x0000000au+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e785 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000002cu+f->r[6])))&0xffffu)<<0);
    /* 0044e789 */
    MI_U16((0x0000000cu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044e78d */
    f->r[1]=f->r[6];
    /* 0044e78f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e794u;f->boundary(0x0044e78fu,0x0044d750u,f);
    /* 0044e794 */
    MI_U16((0x0000000eu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e798 */
    MI_U16((0x00000014u+f->r[7]))=(uint16_t)(0x00000000u);
    /* 0044e79e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000036u+f->r[6])))&0xffu)<<0);
    /* 0044e7a1 */
    MI_U16((0x00000016u+f->r[7]))=(uint16_t)(0x00000000u);
    /* 0044e7a7 */
    MI_U8((0x0000001cu+f->r[7]))=(uint8_t)((f->r[2]&0xffu));
    /* 0044e7aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044e7ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044e7ac */
    f->r[4]+=8;return;
}

void restore_event_C_0044e890(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044e890 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044e893 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e894 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e895 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e896 */
    f->r[6]=f->r[1];
    /* 0044e898 */
    f->r[7]=0x00000009u;
    /* 0044e89d */
    a=MI_U16((0x00000030u+f->r[6]));b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0044e8a1 */
    if((f->flags&MI_ZF))goto mi_0044edc1;
    /* 0044e8a7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044e8aa */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x00000108u);
    /* 0044e8b1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e8b3 */
    if(!(f->flags&MI_ZF))goto mi_0044e8bc;
    /* 0044e8b5 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x00000109u);
mi_0044e8bc:;
    /* 0044e8bc */
    f->r[1]=f->r[6];
    /* 0044e8be */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e8c3u;f->boundary(0x0044e8beu,0x0044f660u,f);
    /* 0044e8c3 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e8c8 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044e8cc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e8cd */
    f->r[1]=f->r[6];
    /* 0044e8cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e8d4u;f->boundary(0x0044e8cfu,0x0044e6f0u,f);
    /* 0044e8d4 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000030u+f->r[6])))&0xffffu)<<0);
    /* 0044e8d8 */
    a=(f->r[0]&0xffffu);b=0x00000006u;t=mi_alu(f,a,b,16,1);
    /* 0044e8dc */
    if((f->flags&MI_ZF))goto mi_0044ec17;
    /* 0044e8e2 */
    a=(f->r[0]&0xffffu);b=0x00000007u;t=mi_alu(f,a,b,16,1);
    /* 0044e8e6 */
    if((f->flags&MI_ZF))goto mi_0044ec17;
    /* 0044e8ec */
    a=(f->r[0]&0xffffu);b=0x00000008u;t=mi_alu(f,a,b,16,1);
    /* 0044e8f0 */
    if((f->flags&MI_ZF))goto mi_0044ec17;
    /* 0044e8f6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000032u+f->r[6])))&0xffu)<<0);
    /* 0044e8f9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e8fb */
    if((f->flags&MI_ZF))goto mi_0044eade;
    /* 0044e901 */
    a=MI_U16((0x0000002eu+f->r[6]));b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x0000002eu+f->r[6]))=(uint16_t)(t);
    /* 0044e905 */
    a=MI_U16((0x0000002eu+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044e90a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044edc1;
    /* 0044e910 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044e914 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044e918 */
    a=(f->r[0]&0xffffu);b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044e91c */
    if(!(f->flags&MI_ZF))goto mi_0044e922;
    /* 0044e91e */
    f->r[1]=f->r[7];
    /* 0044e920 */
    goto mi_0044e94a;
mi_0044e922:;
    /* 0044e922 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0044e928 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0044e92b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e92c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e92d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044e932u;f->boundary(0x0044e92du,0x0045aac0u,f);
    /* 0044e932 */
    f->r[1]=f->r[0];
    /* 0044e934 */
    f->r[0]=0x51eb851fu;
    /* 0044e939 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0044e93b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0044e93e */
    f->r[0]=f->r[2];
    /* 0044e940 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044e943 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0044e946 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044e948 */
    f->r[1]=f->r[2];
mi_0044e94a:;
    /* 0044e94a */
    f->r[0]=f->r[7];
    /* 0044e94c */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0044e94e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044e94f */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e952 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044e954 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0044e956 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0044e958 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000030u+f->r[6])))&0xffffu)<<0);
    /* 0044e95c */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e95f */
    a=(f->r[0]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0044e963 */
    if(!(f->flags&MI_ZF))goto mi_0044ea1d;
    /* 0044e969 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e96b */
    if((f->flags&MI_ZF))goto mi_0044e9aa;
    /* 0044e96d */
    a=MI_U16((0x00000004u+f->r[6]));b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044e972 */
    if(!(f->flags&MI_ZF))goto mi_0044e98f;
    /* 0044e974 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044e977 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000005u);
    /* 0044e97e */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e980 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044e982 */
    a=f->r[1];b=0x0000010du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044e988 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044e98d */
    goto mi_0044e9e1;
mi_0044e98f:;
    /* 0044e98f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044e992 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000005u);
    /* 0044e999 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e99b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044e99d */
    a=f->r[2];b=0x0000010bu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044e9a3 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044e9a8 */
    goto mi_0044e9e1;
mi_0044e9aa:;
    /* 0044e9aa */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044e9ac */
    if((f->flags&MI_ZF))goto mi_0044e9c8;
    /* 0044e9ae */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044e9b1 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000007u);
    /* 0044e9b8 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e9ba */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044e9bc */
    a=f->r[0];b=0x0000010fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044e9c1 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044e9c6 */
    goto mi_0044e9e1;
mi_0044e9c8:;
    /* 0044e9c8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044e9cb */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000000u);
    /* 0044e9d2 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044e9d4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044e9d6 */
    a=f->r[1];b=0x00000111u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044e9dc */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
mi_0044e9e1:;
    /* 0044e9e1 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044e9eb */
    f->r[2]=(0x0000000cu+f->r[4]);
    /* 0044e9ef */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044e9f2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e9f4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e9f5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e9f7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e9f8 */
    a=0x00000084u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044e9fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ea02u;f->boundary(0x0044e9fdu,0x004e0cf0u,f);
    /* 0044ea02 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000036u+f->r[4])))&0xffffu)<<0);
    /* 0044ea07 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ea0a */
    a=(f->r[1]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,16,1);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0044ea0d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044ea0f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044ea10 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044ea13 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044ea17 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044ea18 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044ea19 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ea1c */
    f->r[4]+=4;return;
mi_0044ea1d:;
    /* 0044ea1d */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0044ea21 */
    if(!(f->flags&MI_ZF))goto mi_0044edc1;
    /* 0044ea27 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ea29 */
    if((f->flags&MI_ZF))goto mi_0044ea67;
    /* 0044ea2b */
    a=MI_U16((0x00000004u+f->r[6]));b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044ea30 */
    if(!(f->flags&MI_ZF))goto mi_0044ea4d;
    /* 0044ea32 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ea35 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000005u);
    /* 0044ea3c */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ea3e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044ea40 */
    a=f->r[2];b=0x0000010du;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044ea46 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044ea4b */
    goto mi_0044ea9f;
mi_0044ea4d:;
    /* 0044ea4d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ea50 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000005u);
    /* 0044ea57 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ea59 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044ea5b */
    a=f->r[0];b=0x0000010bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044ea60 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ea65 */
    goto mi_0044ea9f;
mi_0044ea67:;
    /* 0044ea67 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ea69 */
    if((f->flags&MI_ZF))goto mi_0044ea86;
    /* 0044ea6b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ea6e */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000007u);
    /* 0044ea75 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ea77 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044ea79 */
    a=f->r[1];b=0x00000113u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044ea7f */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044ea84 */
    goto mi_0044ea9f;
mi_0044ea86:;
    /* 0044ea86 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ea89 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000000u);
    /* 0044ea90 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ea92 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044ea94 */
    a=f->r[2];b=0x00000115u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044ea9a */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
mi_0044ea9f:;
    /* 0044ea9f */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0044eaa9 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044eaad */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044eab0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eab2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eab3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eab5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eab6 */
    a=0x00000084u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eabb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044eac0u;f->boundary(0x0044eabbu,0x004e0cf0u,f);
    /* 0044eac0 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000036u+f->r[4])))&0xffffu)<<0);
    /* 0044eac5 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044eac8 */
    a=(f->r[2]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,16,1);f->r[2]=(f->r[2]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0044eacb */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044eacd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044eace */
    a=f->r[2];b=0xfffffffdu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0044ead1 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044ead4 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044ead8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044ead9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044eada */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044eadd */
    f->r[4]+=4;return;
mi_0044eade:;
    /* 0044eade */
    a=MI_U8((0x00000028u+f->r[6]));b=0x00000011u;t=mi_alu(f,a,b,8,3);
    /* 0044eae2 */
    if(!(f->flags&MI_ZF))goto mi_0044edc1;
    /* 0044eae8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000014u+f->r[6]));
    /* 0044eaec */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007848d8u));
    /* 0044eaf3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044eaf5 */
    f->r[1]=MI_U32((0x0078d7bcu));
    /* 0044eafb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044eafd */
    if(!(f->flags&MI_ZF))goto mi_0044edc1;
    /* 0044eb03 */
    f->r[0]=MI_U32((0x0078d7c0u));
    /* 0044eb08 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044eb0a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000003u+f->r[6])))&0xffu)<<0);
    /* 0044eb0d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0044eb0f */
    if(!(f->flags&MI_ZF))goto mi_0044edc1;
    /* 0044eb15 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044eb19 */
    f->r[3]=0x00000001u;
    /* 0044eb1e */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044eb22 */
    a=(f->r[0]&0xffffu);b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044eb26 */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 0044eb29 */
    if(!(f->flags&MI_ZF))goto mi_0044eb2f;
    /* 0044eb2b */
    f->r[1]=f->r[7];
    /* 0044eb2d */
    goto mi_0044eb57;
mi_0044eb2f:;
    /* 0044eb2f */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0044eb35 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0044eb38 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eb39 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eb3a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044eb3fu;f->boundary(0x0044eb3au,0x0045aac0u,f);
    /* 0044eb3f */
    f->r[1]=f->r[0];
    /* 0044eb41 */
    f->r[0]=0x51eb851fu;
    /* 0044eb46 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0044eb48 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0044eb4b */
    f->r[0]=f->r[2];
    /* 0044eb4d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044eb50 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0044eb53 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044eb55 */
    f->r[1]=f->r[2];
mi_0044eb57:;
    /* 0044eb57 */
    f->r[0]=f->r[7];
    /* 0044eb59 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044eb5a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044eb5c */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0044eb5e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0044eb60 */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044eb63 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0044eb65 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044eba4;
    /* 0044eb67 */
    a=MI_U16((0x00000004u+f->r[6]));b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044eb6c */
    if(!(f->flags&MI_ZF))goto mi_0044eb89;
    /* 0044eb6e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044eb71 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000005u);
    /* 0044eb78 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044eb7a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044eb7c */
    a=f->r[1];b=0x0000010du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044eb82 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044eb87 */
    goto mi_0044ebdf;
mi_0044eb89:;
    /* 0044eb89 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044eb8c */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000005u);
    /* 0044eb93 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044eb95 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044eb97 */
    a=f->r[2];b=0x0000010bu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044eb9d */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044eba2 */
    goto mi_0044ebdf;
mi_0044eba4:;
    /* 0044eba4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044eba6 */
    if((f->flags&MI_ZF))goto mi_0044ebc2;
    /* 0044eba8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ebab */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000007u);
    /* 0044ebb2 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ebb4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044ebb6 */
    a=f->r[0];b=0x0000010fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044ebbb */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ebc0 */
    goto mi_0044ebdf;
mi_0044ebc2:;
    /* 0044ebc2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ebc5 */
    MI_U16((0x00000022u+f->r[4]))=(uint16_t)(0x00000000u);
    /* 0044ebcc */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ebce */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044ebd0 */
    MI_U16((0x0000002eu+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0044ebd4 */
    a=f->r[1];b=0x00000111u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044ebda */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
mi_0044ebdf:;
    /* 0044ebdf */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044ebe5 */
    f->r[2]=(0x0000000cu+f->r[4]);
    /* 0044ebe9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044ebec */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ebee */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ebef */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ebf1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ebf2 */
    a=0x00000084u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ebf7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ebfcu;f->boundary(0x0044ebf7u,0x004e0cf0u,f);
    /* 0044ebfc */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000036u+f->r[4])))&0xffffu)<<0);
    /* 0044ec01 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ec04 */
    a=(f->r[1]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,16,1);f->r[1]=(f->r[1]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 0044ec07 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044ec09 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044ec0a */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044ec0d */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044ec11 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044ec12 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044ec13 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ec16 */
    f->r[4]+=4;return;
mi_0044ec17:;
    /* 0044ec17 */
    f->r[3]=0x00000001u;
    /* 0044ec1c */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[3]);
    /* 0044ec22 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000030u+f->r[6]));
    /* 0044ec26 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044ec29 */
    if((f->flags&MI_ZF))goto mi_0044ece0;
    /* 0044ec2f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044ec30 */
    if((f->flags&MI_ZF))goto mi_0044ec8d;
    /* 0044ec32 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044ec33 */
    if(!(f->flags&MI_ZF))goto mi_0044edb1;
    /* 0044ec39 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ec3c */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 0044ec40 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044ec43 */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ec45 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec47 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec48 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,2);f->r[2]=t;
    /* 0044ec4a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec4c */
    a=f->r[2];b=0x00000115u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044ec52 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec53 */
    a=0x00000084u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec58 */
    MI_U16((0x00000020u+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044ec5d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ec62u;f->boundary(0x0044ec5du,0x004e0cf0u,f);
    /* 0044ec62 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000052u+f->r[6]));
    /* 0044ec66 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ec69 */
    f->r[1]=f->r[6];
    /* 0044ec6b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec6c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ec71u;f->boundary(0x0044ec6cu,0x004496d0u,f);
    /* 0044ec71 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ec73 */
    if((f->flags&MI_ZF))goto mi_0044edb1;
    /* 0044ec79 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec7b */
    f->r[1]=f->r[6];
    /* 0044ec7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ec82u;f->boundary(0x0044ec7du,0x0044fd30u,f);
    /* 0044ec82 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec83 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000052u+f->r[6]));
    /* 0044ec87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec88 */
    goto mi_0044edaa;
mi_0044ec8d:;
    /* 0044ec8d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ec90 */
    f->r[2]=(0x0000000cu+f->r[4]);
    /* 0044ec94 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044ec97 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ec99 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec9b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ec9c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044ec9e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eca0 */
    a=f->r[1];b=0x0000011du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044eca6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eca7 */
    a=0x00000084u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ecac */
    MI_U16((0x00000020u+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044ecb1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ecb6u;f->boundary(0x0044ecb1u,0x004e0cf0u,f);
    /* 0044ecb6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000026u+f->r[6]));
    /* 0044ecba */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ecbd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ecbe */
    f->r[1]=f->r[6];
    /* 0044ecc0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ecc5u;f->boundary(0x0044ecc0u,0x004496d0u,f);
    /* 0044ecc5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ecc7 */
    if((f->flags&MI_ZF))goto mi_0044edb1;
    /* 0044eccd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ecce */
    f->r[1]=f->r[6];
    /* 0044ecd0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ecd5u;f->boundary(0x0044ecd0u,0x0044fd30u,f);
    /* 0044ecd5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000026u+f->r[6]));
    /* 0044ecd9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ecda */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ecdb */
    goto mi_0044edaa;
mi_0044ece0:;
    /* 0044ece0 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[6])))&0xffffu)<<0);
    /* 0044ece4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[6]));
    /* 0044ece8 */
    a=(f->r[1]&0xffffu);b=0x00000024u;t=mi_alu(f,a,b,16,1);
    /* 0044ecec */
    if(!(f->flags&MI_ZF))goto mi_0044ed2e;
    /* 0044ecee */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 0044ecf4 */
    MI_U16((0x00000020u+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ecf9 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0044ecff */
    f->r[2]=MI_U32((0x00ea187cu+f->r[1]));
    /* 0044ed05 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044ed07 */
    MI_U32((0x00ea187cu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0044ed0d */
    f->r[3]=MI_U32((0x00ea5cc8u+f->r[1]));
    /* 0044ed13 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0044ed15 */
    MI_U32((0x00ea5cc8u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 0044ed1b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ed1e */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ed20 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044ed22 */
    a=f->r[0];b=0x00000119u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044ed27 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044ed2c */
    goto mi_0044ed71;
mi_0044ed2e:;
    /* 0044ed2e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044ed31 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed33 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044ed36 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0044ed39 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0044ed3c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed3d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed3e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ed43u;f->boundary(0x0044ed3eu,0x0045afe0u,f);
    /* 0044ed43 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[6])))&0xffu)<<0);
    /* 0044ed46 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ed49 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ed4b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,2);f->r[1]=t;
    /* 0044ed4d */
    a=f->r[1];b=0x00000117u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0044ed53 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044ed58 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0044ed5b */
    f->r[0]=0x51eb851fu;
    /* 0044ed60 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0044ed62 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 0044ed65 */
    f->r[0]=f->r[2];
    /* 0044ed67 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0044ed6a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0044ed6c */
    MI_U16((0x00000020u+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
mi_0044ed71:;
    /* 0044ed71 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0044ed74 */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0044ed78 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed7a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed7b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed7d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed7e */
    a=0x00000084u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ed88u;f->boundary(0x0044ed83u,0x004e0cf0u,f);
    /* 0044ed88 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044ed8c */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044ed8f */
    f->r[1]=f->r[6];
    /* 0044ed91 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ed97u;f->boundary(0x0044ed92u,0x004496d0u,f);
    /* 0044ed97 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044ed99 */
    if((f->flags&MI_ZF))goto mi_0044edb1;
    /* 0044ed9b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ed9d */
    f->r[1]=f->r[6];
    /* 0044ed9f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044eda4u;f->boundary(0x0044ed9fu,0x0044fd30u,f);
    /* 0044eda4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000024u+f->r[6]));
    /* 0044eda8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eda9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0044edaa:;
    /* 0044edaa */
    f->r[1]=f->r[6];
    /* 0044edac */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044edb1u;f->boundary(0x0044edacu,0x0044c490u,f);
mi_0044edb1:;
    /* 0044edb1 */
    f->r[0]=MI_U32((0x00000028u+f->r[6]));
    /* 0044edb4 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0044edb8 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044edba */
    MI_U8((0x00000032u+f->r[6]))=(uint8_t)(0x00000000u);
    /* 0044edbe */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[0]);
mi_0044edc1:;
    /* 0044edc1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044edc2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044edc3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044edc4 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044edc7 */
    f->r[4]+=4;return;
}

void restore_event_C_0044edd0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044edd0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044edd1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044edd2 */
    f->r[6]=MI_U32((0x00000000u+f->r[1]));
    /* 0044edd4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044edd6 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044edd8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044edd9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044ee09;
    /* 0044eddb */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0044eddf */
    f->r[0]=(0x00000036u+f->r[1]);
mi_0044ede2:;
    /* 0044ede2 */
    a=MI_U8((0xffffffd0u+f->r[0]));b=0x00000017u;t=mi_alu(f,a,b,8,1);
    /* 0044ede6 */
    if(!(f->flags&MI_ZF))goto mi_0044ee01;
    /* 0044ede8 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0044edeb */
    if((f->flags&MI_ZF))goto mi_0044ee01;
    /* 0044eded */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[7])))&0xffffu)<<0);
    /* 0044edf1 */
    a=(f->r[5]&0xffffu);b=MI_U16((0xffffffdau+f->r[0]));t=mi_alu(f,a,b,16,1);
    /* 0044edf5 */
    if(!(f->flags&MI_ZF))goto mi_0044ee01;
    /* 0044edf7 */
    f->r[5]=(f->r[5]&0xffff0000u)|(((MI_U16((0x0000000au+f->r[7])))&0xffffu)<<0);
    /* 0044edfb */
    a=(f->r[5]&0xffffu);b=MI_U16((0xffffffd2u+f->r[0]));t=mi_alu(f,a,b,16,1);
    /* 0044edff */
    if((f->flags&MI_ZF))goto mi_0044ee11;
mi_0044ee01:;
    /* 0044ee01 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ee02 */
    a=f->r[0];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044ee05 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0044ee07 */
    if(mi_signed_less(f->flags))goto mi_0044ede2;
mi_0044ee09:;
    /* 0044ee09 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044ee0a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044ee0b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044ee0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044ee0e */
    f->r[4]+=8;return;
mi_0044ee11:;
    /* 0044ee11 */
    f->r[0]=f->r[2];
    /* 0044ee13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044ee14 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044ee17 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044ee19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044ee1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044ee1b */
    f->r[0]=(0x00000004u+f->r[1]+f->r[0]*4u);
    /* 0044ee1f */
    MI_U16((0x00000000u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044ee22 */
    f->r[4]+=8;return;
}

void restore_event_C_0044ee30(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044ee30 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00d3b186u)))&0xffu)<<0);
    /* 0044ee35 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ee36 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 0044ee38 */
    if((f->flags&MI_ZF))goto mi_0044ee94;
    /* 0044ee3a */
    f->r[0]=MI_U32((0x00e951e4u));
    /* 0044ee3f */
    f->r[1]=0x00785d70u;
    /* 0044ee44 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044ee47 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044ee4a */
    MI_U16((0x00e9524eu+f->r[0]))=(uint16_t)(0x00000000u);
    /* 0044ee53 */
    f->r[0]=(0x00e95238u+f->r[0]);
    /* 0044ee59 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ee5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ee5fu;f->boundary(0x0044ee5au,0x0044edd0u,f);
    /* 0044ee5f */
    f->r[6]=f->r[0];
    /* 0044ee61 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044ee63 */
    if((f->flags&MI_ZF))goto mi_0044ee94;
    /* 0044ee65 */
    f->r[1]=f->r[6];
    /* 0044ee67 */
    MI_U16((0x00000030u+f->r[6]))=(uint16_t)(0x00000006u);
    /* 0044ee6d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ee72u;f->boundary(0x0044ee6du,0x0044e890u,f);
    /* 0044ee72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ee77u;f->boundary(0x0044ee72u,0x004c7f30u,f);
    /* 0044ee77 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0044ee7d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[6]));
    /* 0044ee81 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0044ee87 */
    MI_U8((0x00ea6214u+f->r[1]+f->r[2]*1u))=(uint8_t)(0x00000001u);
    /* 0044ee8f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ee94u;f->boundary(0x0044ee8fu,0x005596d0u,f);
mi_0044ee94:;
    /* 0044ee94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044ee95 */
    f->r[4]+=4;return;
}

void restore_event_C_0044eea0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044eea0 */
    a=MI_U8((0x00d3b186u));b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0044eea7 */
    if((f->flags&MI_ZF))goto mi_0044eee4;
    /* 0044eea9 */
    f->r[0]=MI_U32((0x00e951e4u));
    /* 0044eeae */
    f->r[1]=0x00785d70u;
    /* 0044eeb3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044eeb6 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044eeb9 */
    MI_U16((0x00e9524eu+f->r[0]))=(uint16_t)(0x00000000u);
    /* 0044eec2 */
    f->r[0]=(0x00e95238u+f->r[0]);
    /* 0044eec8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044eec9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044eeceu;f->boundary(0x0044eec9u,0x0044edd0u,f);
    /* 0044eece */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044eed0 */
    if((f->flags&MI_ZF))goto mi_0044eee4;
    /* 0044eed2 */
    f->r[1]=f->r[0];
    /* 0044eed4 */
    MI_U16((0x00000030u+f->r[0]))=(uint16_t)(0x00000007u);
    /* 0044eeda */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044eedfu;f->boundary(0x0044eedau,0x0044e890u,f);
    /* 0044eedf */
    f->boundary(0x0044eedfu,0x004c7f30u,f);return;
mi_0044eee4:;
    /* 0044eee4 */
    f->r[4]+=4;return;
}

void restore_event_C_0044eef0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044eef0 */
    a=MI_U8((0x00d3b186u));b=0x00000002u;t=mi_alu(f,a,b,8,3);
    /* 0044eef7 */
    if((f->flags&MI_ZF))goto mi_0044ef56;
    /* 0044eef9 */
    f->r[0]=MI_U32((0x00e951e4u));
    /* 0044eefe */
    f->r[1]=0x00785d70u;
    /* 0044ef03 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044ef06 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044ef09 */
    MI_U16((0x00e9524eu+f->r[0]))=(uint16_t)(0x00000000u);
    /* 0044ef12 */
    f->r[0]=(0x00e95238u+f->r[0]);
    /* 0044ef18 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ef19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ef1eu;f->boundary(0x0044ef19u,0x0044edd0u,f);
    /* 0044ef1e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044ef20 */
    if((f->flags&MI_ZF))goto mi_0044ef56;
    /* 0044ef22 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000030u+f->r[0]));
    /* 0044ef26 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044ef27 */
    if((f->flags&MI_ZF))goto mi_0044ef45;
    /* 0044ef29 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044ef2c */
    if((f->flags&MI_ZF))goto mi_0044ef3d;
    /* 0044ef2e */
    f->r[1]=f->r[0];
    /* 0044ef30 */
    MI_U16((0x00000030u+f->r[0]))=(uint16_t)(0x00000008u);
    /* 0044ef36 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044ef3bu;f->boundary(0x0044ef36u,0x0044e890u,f);
    /* 0044ef3b */
    goto mi_0044ef51;
mi_0044ef3d:;
    /* 0044ef3d */
    MI_U16((0x00000030u+f->r[0]))=(uint16_t)(0x00000004u);
    /* 0044ef43 */
    goto mi_0044ef4b;
mi_0044ef45:;
    /* 0044ef45 */
    MI_U16((0x00000030u+f->r[0]))=(uint16_t)(0x00000002u);
mi_0044ef4b:;
    /* 0044ef4b */
    MI_U16((0x0000002eu+f->r[0]))=(uint16_t)(0x00000001u);
mi_0044ef51:;
    /* 0044ef51 */
    f->boundary(0x0044ef51u,0x004c7f30u,f);return;
mi_0044ef56:;
    /* 0044ef56 */
    f->r[4]+=4;return;
}

void restore_event_C_0044ef60(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044ef60 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000034u+f->r[1]));
    /* 0044ef64 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044ef66 */
    if(mi_signed_less(f->flags))goto mi_0044ef82;
    /* 0044ef68 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044ef6e */
    if(!mi_signed_less(f->flags))goto mi_0044ef82;
    /* 0044ef70 */
    f->r[1]=f->r[0];
    /* 0044ef72 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044ef75 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044ef77 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044ef7e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044ef80 */
    if(!(f->flags&MI_ZF))goto mi_0044ef85;
mi_0044ef82:;
    /* 0044ef82 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ef84 */
    f->r[4]+=4;return;
mi_0044ef85:;
    /* 0044ef85 */
    f->r[0]=MI_U32((0x00000028u+f->r[0]));
    /* 0044ef88 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0044ef8b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ef8d */
    f->r[4]+=4;return;
}

void restore_event_C_0044ef90(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044ef90 */
    f->r[0]=MI_U32((0x00000028u+f->r[1]));
    /* 0044ef93 */
    MI_U8((0x00000032u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0044ef97 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044ef99 */
    MI_U32((0x00000028u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0044ef9c */
    f->r[4]+=4;return;
}

void restore_event_C_0044f120(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f120 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0044f124 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f127 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f150;
    /* 0044f129 */
    switch(mi_read((0x0044f158u+f->r[0]*4u),4u)){case 0x0044f130u:goto mi_0044f130;case 0x0044f138u:goto mi_0044f138;case 0x0044f140u:goto mi_0044f140;case 0x0044f148u:goto mi_0044f148;default:abort();}
mi_0044f130:;
    /* 0044f130 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044f135u;f->boundary(0x0044f130u,0x0044f170u,f);
    /* 0044f135 */
    f->r[4]+=8;return;
mi_0044f138:;
    /* 0044f138 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044f13du;f->boundary(0x0044f138u,0x0044f360u,f);
    /* 0044f13d */
    f->r[4]+=8;return;
mi_0044f140:;
    /* 0044f140 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044f145u;f->boundary(0x0044f140u,0x0044f3d0u,f);
    /* 0044f145 */
    f->r[4]+=8;return;
mi_0044f148:;
    /* 0044f148 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044f14du;f->boundary(0x0044f148u,0x0044f440u,f);
    /* 0044f14d */
    f->r[4]+=8;return;
mi_0044f150:;
    /* 0044f150 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044f153 */
    f->r[4]+=8;return;
}

void restore_event_C_0044f170(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f170 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f172 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f175 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f176 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);
    /* 0044f179 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f2a7;
    /* 0044f17f */
    switch(mi_read((0x0044f2acu+f->r[0]*4u),4u)){case 0x0044f186u:goto mi_0044f186;case 0x0044f1c2u:goto mi_0044f1c2;case 0x0044f1efu:goto mi_0044f1ef;case 0x0044f1f5u:goto mi_0044f1f5;case 0x0044f1fbu:goto mi_0044f1fb;case 0x0044f201u:goto mi_0044f201;case 0x0044f207u:goto mi_0044f207;case 0x0044f20du:goto mi_0044f20d;case 0x0044f213u:goto mi_0044f213;case 0x0044f219u:goto mi_0044f219;case 0x0044f21fu:goto mi_0044f21f;case 0x0044f225u:goto mi_0044f225;case 0x0044f22bu:goto mi_0044f22b;case 0x0044f231u:goto mi_0044f231;case 0x0044f237u:goto mi_0044f237;case 0x0044f23du:goto mi_0044f23d;case 0x0044f26cu:goto mi_0044f26c;case 0x0044f295u:goto mi_0044f295;case 0x0044f29bu:goto mi_0044f29b;case 0x0044f2a1u:goto mi_0044f2a1;case 0x0044f2a7u:goto mi_0044f2a7;default:abort();}
mi_0044f186:;
    /* 0044f186 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f188 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f18b */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f18e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f2a7;
    /* 0044f194 */
    switch(mi_read((0x0044f308u+f->r[0]*4u),4u)){case 0x0044f19bu:goto mi_0044f19b;case 0x0044f1a1u:goto mi_0044f1a1;case 0x0044f1a4u:goto mi_0044f1a4;case 0x0044f1aau:goto mi_0044f1aa;case 0x0044f1b0u:goto mi_0044f1b0;case 0x0044f1b6u:goto mi_0044f1b6;case 0x0044f1bcu:goto mi_0044f1bc;default:abort();}
mi_0044f19b:;
    /* 0044f19b */
    f->r[0]=0x00000008u;
    /* 0044f1a0 */
    f->r[4]+=4;return;
mi_0044f1a1:;
    /* 0044f1a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f1a3 */
    f->r[4]+=4;return;
mi_0044f1a4:;
    /* 0044f1a4 */
    f->r[0]=0x00000004u;
    /* 0044f1a9 */
    f->r[4]+=4;return;
mi_0044f1aa:;
    /* 0044f1aa */
    f->r[0]=0x0000000bu;
    /* 0044f1af */
    f->r[4]+=4;return;
mi_0044f1b0:;
    /* 0044f1b0 */
    f->r[0]=0x0000000eu;
    /* 0044f1b5 */
    f->r[4]+=4;return;
mi_0044f1b6:;
    /* 0044f1b6 */
    f->r[0]=0x00000011u;
    /* 0044f1bb */
    f->r[4]+=4;return;
mi_0044f1bc:;
    /* 0044f1bc */
    f->r[0]=0x00000014u;
    /* 0044f1c1 */
    f->r[4]+=4;return;
mi_0044f1c2:;
    /* 0044f1c2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[1]),2u);
    /* 0044f1c6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f1c7 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f1ca */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f2a7;
    /* 0044f1d0 */
    switch(mi_read((0x0044f324u+f->r[0]*4u),4u)){case 0x0044f1d7u:goto mi_0044f1d7;case 0x0044f1ddu:goto mi_0044f1dd;case 0x0044f1e3u:goto mi_0044f1e3;case 0x0044f1e9u:goto mi_0044f1e9;default:abort();}
mi_0044f1d7:;
    /* 0044f1d7 */
    f->r[0]=0x00000020u;
    /* 0044f1dc */
    f->r[4]+=4;return;
mi_0044f1dd:;
    /* 0044f1dd */
    f->r[0]=0x00000021u;
    /* 0044f1e2 */
    f->r[4]+=4;return;
mi_0044f1e3:;
    /* 0044f1e3 */
    f->r[0]=0x00000032u;
    /* 0044f1e8 */
    f->r[4]+=4;return;
mi_0044f1e9:;
    /* 0044f1e9 */
    f->r[0]=0x00000022u;
    /* 0044f1ee */
    f->r[4]+=4;return;
mi_0044f1ef:;
    /* 0044f1ef */
    f->r[0]=0x0000002bu;
    /* 0044f1f4 */
    f->r[4]+=4;return;
mi_0044f1f5:;
    /* 0044f1f5 */
    f->r[0]=0x0000002cu;
    /* 0044f1fa */
    f->r[4]+=4;return;
mi_0044f1fb:;
    /* 0044f1fb */
    f->r[0]=0x0000002du;
    /* 0044f200 */
    f->r[4]+=4;return;
mi_0044f201:;
    /* 0044f201 */
    f->r[0]=0x00000029u;
    /* 0044f206 */
    f->r[4]+=4;return;
mi_0044f207:;
    /* 0044f207 */
    f->r[0]=0x0000002au;
    /* 0044f20c */
    f->r[4]+=4;return;
mi_0044f20d:;
    /* 0044f20d */
    f->r[0]=0x0000002eu;
    /* 0044f212 */
    f->r[4]+=4;return;
mi_0044f213:;
    /* 0044f213 */
    f->r[0]=0x0000002fu;
    /* 0044f218 */
    f->r[4]+=4;return;
mi_0044f219:;
    /* 0044f219 */
    f->r[0]=0x00000025u;
    /* 0044f21e */
    f->r[4]+=4;return;
mi_0044f21f:;
    /* 0044f21f */
    f->r[0]=0x00000026u;
    /* 0044f224 */
    f->r[4]+=4;return;
mi_0044f225:;
    /* 0044f225 */
    f->r[0]=0x00000027u;
    /* 0044f22a */
    f->r[4]+=4;return;
mi_0044f22b:;
    /* 0044f22b */
    f->r[0]=0x00000028u;
    /* 0044f230 */
    f->r[4]+=4;return;
mi_0044f231:;
    /* 0044f231 */
    f->r[0]=0x00000023u;
    /* 0044f236 */
    f->r[4]+=4;return;
mi_0044f237:;
    /* 0044f237 */
    f->r[0]=0x00000024u;
    /* 0044f23c */
    f->r[4]+=4;return;
mi_0044f23d:;
    /* 0044f23d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f23f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f242 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0044f245 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f2a7;
    /* 0044f247 */
    switch(mi_read((0x0044f334u+f->r[0]*4u),4u)){case 0x0044f24eu:goto mi_0044f24e;case 0x0044f254u:goto mi_0044f254;case 0x0044f25au:goto mi_0044f25a;case 0x0044f260u:goto mi_0044f260;case 0x0044f266u:goto mi_0044f266;default:abort();}
mi_0044f24e:;
    /* 0044f24e */
    f->r[0]=0x00000017u;
    /* 0044f253 */
    f->r[4]+=4;return;
mi_0044f254:;
    /* 0044f254 */
    f->r[0]=0x00000018u;
    /* 0044f259 */
    f->r[4]+=4;return;
mi_0044f25a:;
    /* 0044f25a */
    f->r[0]=0x00000019u;
    /* 0044f25f */
    f->r[4]+=4;return;
mi_0044f260:;
    /* 0044f260 */
    f->r[0]=0x0000001au;
    /* 0044f265 */
    f->r[4]+=4;return;
mi_0044f266:;
    /* 0044f266 */
    f->r[0]=0x0000001bu;
    /* 0044f26b */
    f->r[4]+=4;return;
mi_0044f26c:;
    /* 0044f26c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f26e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f271 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f274 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f2a7;
    /* 0044f276 */
    switch(mi_read((0x0044f348u+f->r[0]*4u),4u)){case 0x0044f27du:goto mi_0044f27d;case 0x0044f283u:goto mi_0044f283;case 0x0044f289u:goto mi_0044f289;case 0x0044f28fu:goto mi_0044f28f;default:abort();}
mi_0044f27d:;
    /* 0044f27d */
    f->r[0]=0x0000001cu;
    /* 0044f282 */
    f->r[4]+=4;return;
mi_0044f283:;
    /* 0044f283 */
    f->r[0]=0x0000001du;
    /* 0044f288 */
    f->r[4]+=4;return;
mi_0044f289:;
    /* 0044f289 */
    f->r[0]=0x0000001eu;
    /* 0044f28e */
    f->r[4]+=4;return;
mi_0044f28f:;
    /* 0044f28f */
    f->r[0]=0x0000001fu;
    /* 0044f294 */
    f->r[4]+=4;return;
mi_0044f295:;
    /* 0044f295 */
    f->r[0]=0x00000030u;
    /* 0044f29a */
    f->r[4]+=4;return;
mi_0044f29b:;
    /* 0044f29b */
    f->r[0]=0x00000031u;
    /* 0044f2a0 */
    f->r[4]+=4;return;
mi_0044f2a1:;
    /* 0044f2a1 */
    f->r[0]=0x00000035u;
    /* 0044f2a6 */
    f->r[4]+=4;return;
mi_0044f2a7:;
    /* 0044f2a7 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044f2aa */
    f->r[4]+=4;return;
}

void restore_event_C_0044f360(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f360 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f362 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f365 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f366 */
    if((f->flags&MI_ZF))goto mi_0044f373;
    /* 0044f368 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044f36b */
    if(!(f->flags&MI_ZF))goto mi_0044f3ae;
    /* 0044f36d */
    f->r[0]=0x00000033u;
    /* 0044f372 */
    f->r[4]+=4;return;
mi_0044f373:;
    /* 0044f373 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f375 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f378 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f37b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f3ae;
    /* 0044f37d */
    switch(mi_read((0x0044f3b4u+f->r[0]*4u),4u)){case 0x0044f384u:goto mi_0044f384;case 0x0044f38au:goto mi_0044f38a;case 0x0044f390u:goto mi_0044f390;case 0x0044f396u:goto mi_0044f396;case 0x0044f39cu:goto mi_0044f39c;case 0x0044f3a2u:goto mi_0044f3a2;case 0x0044f3a8u:goto mi_0044f3a8;default:abort();}
mi_0044f384:;
    /* 0044f384 */
    f->r[0]=0x0000000au;
    /* 0044f389 */
    f->r[4]+=4;return;
mi_0044f38a:;
    /* 0044f38a */
    f->r[0]=0x00000003u;
    /* 0044f38f */
    f->r[4]+=4;return;
mi_0044f390:;
    /* 0044f390 */
    f->r[0]=0x00000007u;
    /* 0044f395 */
    f->r[4]+=4;return;
mi_0044f396:;
    /* 0044f396 */
    f->r[0]=0x0000000du;
    /* 0044f39b */
    f->r[4]+=4;return;
mi_0044f39c:;
    /* 0044f39c */
    f->r[0]=0x00000010u;
    /* 0044f3a1 */
    f->r[4]+=4;return;
mi_0044f3a2:;
    /* 0044f3a2 */
    f->r[0]=0x00000013u;
    /* 0044f3a7 */
    f->r[4]+=4;return;
mi_0044f3a8:;
    /* 0044f3a8 */
    f->r[0]=0x00000016u;
    /* 0044f3ad */
    f->r[4]+=4;return;
mi_0044f3ae:;
    /* 0044f3ae */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044f3b1 */
    f->r[4]+=4;return;
}

void restore_event_C_0044f3d0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f3d0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f3d2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f3d5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f3d6 */
    if((f->flags&MI_ZF))goto mi_0044f3e3;
    /* 0044f3d8 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044f3db */
    if(!(f->flags&MI_ZF))goto mi_0044f41e;
    /* 0044f3dd */
    f->r[0]=0x00000034u;
    /* 0044f3e2 */
    f->r[4]+=4;return;
mi_0044f3e3:;
    /* 0044f3e3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f3e5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f3e8 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f3eb */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f41e;
    /* 0044f3ed */
    switch(mi_read((0x0044f424u+f->r[0]*4u),4u)){case 0x0044f3f4u:goto mi_0044f3f4;case 0x0044f3fau:goto mi_0044f3fa;case 0x0044f400u:goto mi_0044f400;case 0x0044f406u:goto mi_0044f406;case 0x0044f40cu:goto mi_0044f40c;case 0x0044f412u:goto mi_0044f412;case 0x0044f418u:goto mi_0044f418;default:abort();}
mi_0044f3f4:;
    /* 0044f3f4 */
    f->r[0]=0x00000009u;
    /* 0044f3f9 */
    f->r[4]+=4;return;
mi_0044f3fa:;
    /* 0044f3fa */
    f->r[0]=0x00000002u;
    /* 0044f3ff */
    f->r[4]+=4;return;
mi_0044f400:;
    /* 0044f400 */
    f->r[0]=0x00000006u;
    /* 0044f405 */
    f->r[4]+=4;return;
mi_0044f406:;
    /* 0044f406 */
    f->r[0]=0x0000000cu;
    /* 0044f40b */
    f->r[4]+=4;return;
mi_0044f40c:;
    /* 0044f40c */
    f->r[0]=0x0000000fu;
    /* 0044f411 */
    f->r[4]+=4;return;
mi_0044f412:;
    /* 0044f412 */
    f->r[0]=0x00000012u;
    /* 0044f417 */
    f->r[4]+=4;return;
mi_0044f418:;
    /* 0044f418 */
    f->r[0]=0x00000015u;
    /* 0044f41d */
    f->r[4]+=4;return;
mi_0044f41e:;
    /* 0044f41e */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044f421 */
    f->r[4]+=4;return;
}

void restore_event_C_0044f440(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f440 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f442 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 0044f445 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f446 */
    if(!(f->flags&MI_ZF))goto mi_0044f453;
    /* 0044f448 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f44a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000060u+f->r[1])))&0xffu)<<0);
    /* 0044f44d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f44e */
    if((f->flags&MI_ZF))goto mi_0044f45d;
    /* 0044f450 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f451 */
    if((f->flags&MI_ZF))goto mi_0044f457;
mi_0044f453:;
    /* 0044f453 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044f456 */
    f->r[4]+=4;return;
mi_0044f457:;
    /* 0044f457 */
    f->r[0]=0x00000005u;
    /* 0044f45c */
    f->r[4]+=4;return;
mi_0044f45d:;
    /* 0044f45d */
    f->r[0]=0x00000001u;
    /* 0044f462 */
    f->r[4]+=4;return;
}

void restore_event_C_0044f470(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f470 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f472 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f475 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f476 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);
    /* 0044f479 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f5a7;
    /* 0044f47f */
    switch(mi_read((0x0044f5acu+f->r[0]*4u),4u)){case 0x0044f486u:goto mi_0044f486;case 0x0044f4c2u:goto mi_0044f4c2;case 0x0044f4efu:goto mi_0044f4ef;case 0x0044f4f5u:goto mi_0044f4f5;case 0x0044f4fbu:goto mi_0044f4fb;case 0x0044f501u:goto mi_0044f501;case 0x0044f507u:goto mi_0044f507;case 0x0044f50du:goto mi_0044f50d;case 0x0044f513u:goto mi_0044f513;case 0x0044f519u:goto mi_0044f519;case 0x0044f51fu:goto mi_0044f51f;case 0x0044f525u:goto mi_0044f525;case 0x0044f52bu:goto mi_0044f52b;case 0x0044f531u:goto mi_0044f531;case 0x0044f537u:goto mi_0044f537;case 0x0044f53du:goto mi_0044f53d;case 0x0044f56cu:goto mi_0044f56c;case 0x0044f595u:goto mi_0044f595;case 0x0044f59bu:goto mi_0044f59b;case 0x0044f5a1u:goto mi_0044f5a1;case 0x0044f5a7u:goto mi_0044f5a7;default:abort();}
mi_0044f486:;
    /* 0044f486 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f488 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f48b */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f48e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f5a7;
    /* 0044f494 */
    switch(mi_read((0x0044f608u+f->r[0]*4u),4u)){case 0x0044f49bu:goto mi_0044f49b;case 0x0044f4a1u:goto mi_0044f4a1;case 0x0044f4a4u:goto mi_0044f4a4;case 0x0044f4aau:goto mi_0044f4aa;case 0x0044f4b0u:goto mi_0044f4b0;case 0x0044f4b6u:goto mi_0044f4b6;case 0x0044f4bcu:goto mi_0044f4bc;default:abort();}
mi_0044f49b:;
    /* 0044f49b */
    f->r[0]=0x00000002u;
    /* 0044f4a0 */
    f->r[4]+=4;return;
mi_0044f4a1:;
    /* 0044f4a1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f4a3 */
    f->r[4]+=4;return;
mi_0044f4a4:;
    /* 0044f4a4 */
    f->r[0]=0x00000001u;
    /* 0044f4a9 */
    f->r[4]+=4;return;
mi_0044f4aa:;
    /* 0044f4aa */
    f->r[0]=0x00000003u;
    /* 0044f4af */
    f->r[4]+=4;return;
mi_0044f4b0:;
    /* 0044f4b0 */
    f->r[0]=0x00000004u;
    /* 0044f4b5 */
    f->r[4]+=4;return;
mi_0044f4b6:;
    /* 0044f4b6 */
    f->r[0]=0x00000005u;
    /* 0044f4bb */
    f->r[4]+=4;return;
mi_0044f4bc:;
    /* 0044f4bc */
    f->r[0]=0x00000006u;
    /* 0044f4c1 */
    f->r[4]+=4;return;
mi_0044f4c2:;
    /* 0044f4c2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[1]),2u);
    /* 0044f4c6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f4c7 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f4ca */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f5a7;
    /* 0044f4d0 */
    switch(mi_read((0x0044f624u+f->r[0]*4u),4u)){case 0x0044f4d7u:goto mi_0044f4d7;case 0x0044f4ddu:goto mi_0044f4dd;case 0x0044f4e3u:goto mi_0044f4e3;case 0x0044f4e9u:goto mi_0044f4e9;default:abort();}
mi_0044f4d7:;
    /* 0044f4d7 */
    f->r[0]=0x00000010u;
    /* 0044f4dc */
    f->r[4]+=4;return;
mi_0044f4dd:;
    /* 0044f4dd */
    f->r[0]=0x00000011u;
    /* 0044f4e2 */
    f->r[4]+=4;return;
mi_0044f4e3:;
    /* 0044f4e3 */
    f->r[0]=0x00000022u;
    /* 0044f4e8 */
    f->r[4]+=4;return;
mi_0044f4e9:;
    /* 0044f4e9 */
    f->r[0]=0x00000012u;
    /* 0044f4ee */
    f->r[4]+=4;return;
mi_0044f4ef:;
    /* 0044f4ef */
    f->r[0]=0x0000001bu;
    /* 0044f4f4 */
    f->r[4]+=4;return;
mi_0044f4f5:;
    /* 0044f4f5 */
    f->r[0]=0x0000001cu;
    /* 0044f4fa */
    f->r[4]+=4;return;
mi_0044f4fb:;
    /* 0044f4fb */
    f->r[0]=0x0000001du;
    /* 0044f500 */
    f->r[4]+=4;return;
mi_0044f501:;
    /* 0044f501 */
    f->r[0]=0x00000019u;
    /* 0044f506 */
    f->r[4]+=4;return;
mi_0044f507:;
    /* 0044f507 */
    f->r[0]=0x0000001au;
    /* 0044f50c */
    f->r[4]+=4;return;
mi_0044f50d:;
    /* 0044f50d */
    f->r[0]=0x0000001eu;
    /* 0044f512 */
    f->r[4]+=4;return;
mi_0044f513:;
    /* 0044f513 */
    f->r[0]=0x0000001fu;
    /* 0044f518 */
    f->r[4]+=4;return;
mi_0044f519:;
    /* 0044f519 */
    f->r[0]=0x00000015u;
    /* 0044f51e */
    f->r[4]+=4;return;
mi_0044f51f:;
    /* 0044f51f */
    f->r[0]=0x00000016u;
    /* 0044f524 */
    f->r[4]+=4;return;
mi_0044f525:;
    /* 0044f525 */
    f->r[0]=0x00000017u;
    /* 0044f52a */
    f->r[4]+=4;return;
mi_0044f52b:;
    /* 0044f52b */
    f->r[0]=0x00000018u;
    /* 0044f530 */
    f->r[4]+=4;return;
mi_0044f531:;
    /* 0044f531 */
    f->r[0]=0x00000013u;
    /* 0044f536 */
    f->r[4]+=4;return;
mi_0044f537:;
    /* 0044f537 */
    f->r[0]=0x00000014u;
    /* 0044f53c */
    f->r[4]+=4;return;
mi_0044f53d:;
    /* 0044f53d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f53f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f542 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0044f545 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f5a7;
    /* 0044f547 */
    switch(mi_read((0x0044f634u+f->r[0]*4u),4u)){case 0x0044f54eu:goto mi_0044f54e;case 0x0044f554u:goto mi_0044f554;case 0x0044f55au:goto mi_0044f55a;case 0x0044f560u:goto mi_0044f560;case 0x0044f566u:goto mi_0044f566;default:abort();}
mi_0044f54e:;
    /* 0044f54e */
    f->r[0]=0x00000007u;
    /* 0044f553 */
    f->r[4]+=4;return;
mi_0044f554:;
    /* 0044f554 */
    f->r[0]=0x00000008u;
    /* 0044f559 */
    f->r[4]+=4;return;
mi_0044f55a:;
    /* 0044f55a */
    f->r[0]=0x00000009u;
    /* 0044f55f */
    f->r[4]+=4;return;
mi_0044f560:;
    /* 0044f560 */
    f->r[0]=0x0000000au;
    /* 0044f565 */
    f->r[4]+=4;return;
mi_0044f566:;
    /* 0044f566 */
    f->r[0]=0x0000000bu;
    /* 0044f56b */
    f->r[4]+=4;return;
mi_0044f56c:;
    /* 0044f56c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f56e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f571 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f574 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f5a7;
    /* 0044f576 */
    switch(mi_read((0x0044f648u+f->r[0]*4u),4u)){case 0x0044f57du:goto mi_0044f57d;case 0x0044f583u:goto mi_0044f583;case 0x0044f589u:goto mi_0044f589;case 0x0044f58fu:goto mi_0044f58f;default:abort();}
mi_0044f57d:;
    /* 0044f57d */
    f->r[0]=0x0000000cu;
    /* 0044f582 */
    f->r[4]+=4;return;
mi_0044f583:;
    /* 0044f583 */
    f->r[0]=0x0000000du;
    /* 0044f588 */
    f->r[4]+=4;return;
mi_0044f589:;
    /* 0044f589 */
    f->r[0]=0x0000000eu;
    /* 0044f58e */
    f->r[4]+=4;return;
mi_0044f58f:;
    /* 0044f58f */
    f->r[0]=0x0000000fu;
    /* 0044f594 */
    f->r[4]+=4;return;
mi_0044f595:;
    /* 0044f595 */
    f->r[0]=0x00000020u;
    /* 0044f59a */
    f->r[4]+=4;return;
mi_0044f59b:;
    /* 0044f59b */
    f->r[0]=0x00000021u;
    /* 0044f5a0 */
    f->r[4]+=4;return;
mi_0044f5a1:;
    /* 0044f5a1 */
    f->r[0]=0x00000023u;
    /* 0044f5a6 */
    f->r[4]+=4;return;
mi_0044f5a7:;
    /* 0044f5a7 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044f5aa */
    f->r[4]+=4;return;
}

void restore_event_C_0044f660(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f660 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f662 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f665 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f666 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);
    /* 0044f669 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f7e7;
    /* 0044f66f */
    switch(mi_read((0x0044f7ecu+f->r[0]*4u),4u)){case 0x0044f676u:goto mi_0044f676;case 0x0044f6fbu:goto mi_0044f6fb;case 0x0044f728u:goto mi_0044f728;case 0x0044f72eu:goto mi_0044f72e;case 0x0044f734u:goto mi_0044f734;case 0x0044f73au:goto mi_0044f73a;case 0x0044f740u:goto mi_0044f740;case 0x0044f746u:goto mi_0044f746;case 0x0044f74cu:goto mi_0044f74c;case 0x0044f752u:goto mi_0044f752;case 0x0044f758u:goto mi_0044f758;case 0x0044f75eu:goto mi_0044f75e;case 0x0044f764u:goto mi_0044f764;case 0x0044f76au:goto mi_0044f76a;case 0x0044f770u:goto mi_0044f770;case 0x0044f776u:goto mi_0044f776;case 0x0044f7a5u:goto mi_0044f7a5;case 0x0044f7ceu:goto mi_0044f7ce;case 0x0044f7d4u:goto mi_0044f7d4;case 0x0044f7dau:goto mi_0044f7da;case 0x0044f7e7u:goto mi_0044f7e7;default:abort();}
mi_0044f676:;
    /* 0044f676 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f679 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044f67b */
    if((f->flags&MI_ZF))goto mi_0044f6bc;
    /* 0044f67d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f67f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f682 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f685 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f7e7;
    /* 0044f68b */
    switch(mi_read((0x0044f848u+f->r[0]*4u),4u)){case 0x0044f692u:goto mi_0044f692;case 0x0044f698u:goto mi_0044f698;case 0x0044f69eu:goto mi_0044f69e;case 0x0044f6a4u:goto mi_0044f6a4;case 0x0044f6aau:goto mi_0044f6aa;case 0x0044f6b0u:goto mi_0044f6b0;case 0x0044f6b6u:goto mi_0044f6b6;default:abort();}
mi_0044f692:;
    /* 0044f692 */
    f->r[0]=0x00000034u;
    /* 0044f697 */
    f->r[4]+=4;return;
mi_0044f698:;
    /* 0044f698 */
    f->r[0]=0x00000010u;
    /* 0044f69d */
    f->r[4]+=4;return;
mi_0044f69e:;
    /* 0044f69e */
    f->r[0]=0x00000022u;
    /* 0044f6a3 */
    f->r[4]+=4;return;
mi_0044f6a4:;
    /* 0044f6a4 */
    f->r[0]=0x00000046u;
    /* 0044f6a9 */
    f->r[4]+=4;return;
mi_0044f6aa:;
    /* 0044f6aa */
    f->r[0]=0x00000058u;
    /* 0044f6af */
    f->r[4]+=4;return;
mi_0044f6b0:;
    /* 0044f6b0 */
    f->r[0]=0x0000006au;
    /* 0044f6b5 */
    f->r[4]+=4;return;
mi_0044f6b6:;
    /* 0044f6b6 */
    f->r[0]=0x0000007cu;
    /* 0044f6bb */
    f->r[4]+=4;return;
mi_0044f6bc:;
    /* 0044f6bc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f6be */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f6c1 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f6c4 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f7e7;
    /* 0044f6ca */
    switch(mi_read((0x0044f864u+f->r[0]*4u),4u)){case 0x0044f6d1u:goto mi_0044f6d1;case 0x0044f6d7u:goto mi_0044f6d7;case 0x0044f6ddu:goto mi_0044f6dd;case 0x0044f6e3u:goto mi_0044f6e3;case 0x0044f6e9u:goto mi_0044f6e9;case 0x0044f6efu:goto mi_0044f6ef;case 0x0044f6f5u:goto mi_0044f6f5;default:abort();}
mi_0044f6d1:;
    /* 0044f6d1 */
    f->r[0]=0x00000035u;
    /* 0044f6d6 */
    f->r[4]+=4;return;
mi_0044f6d7:;
    /* 0044f6d7 */
    f->r[0]=0x00000011u;
    /* 0044f6dc */
    f->r[4]+=4;return;
mi_0044f6dd:;
    /* 0044f6dd */
    f->r[0]=0x00000023u;
    /* 0044f6e2 */
    f->r[4]+=4;return;
mi_0044f6e3:;
    /* 0044f6e3 */
    f->r[0]=0x00000047u;
    /* 0044f6e8 */
    f->r[4]+=4;return;
mi_0044f6e9:;
    /* 0044f6e9 */
    f->r[0]=0x00000059u;
    /* 0044f6ee */
    f->r[4]+=4;return;
mi_0044f6ef:;
    /* 0044f6ef */
    f->r[0]=0x0000006bu;
    /* 0044f6f4 */
    f->r[4]+=4;return;
mi_0044f6f5:;
    /* 0044f6f5 */
    f->r[0]=0x0000007du;
    /* 0044f6fa */
    f->r[4]+=4;return;
mi_0044f6fb:;
    /* 0044f6fb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[1]),2u);
    /* 0044f6ff */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f700 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f703 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f7e7;
    /* 0044f709 */
    switch(mi_read((0x0044f880u+f->r[0]*4u),4u)){case 0x0044f710u:goto mi_0044f710;case 0x0044f716u:goto mi_0044f716;case 0x0044f71cu:goto mi_0044f71c;case 0x0044f722u:goto mi_0044f722;default:abort();}
mi_0044f710:;
    /* 0044f710 */
    f->r[0]=0x000000beu;
    /* 0044f715 */
    f->r[4]+=4;return;
mi_0044f716:;
    /* 0044f716 */
    f->r[0]=0x000000b4u;
    /* 0044f71b */
    f->r[4]+=4;return;
mi_0044f71c:;
    /* 0044f71c */
    f->r[0]=0x00000107u;
    /* 0044f721 */
    f->r[4]+=4;return;
mi_0044f722:;
    /* 0044f722 */
    f->r[0]=0x000000aau;
    /* 0044f727 */
    f->r[4]+=4;return;
mi_0044f728:;
    /* 0044f728 */
    f->r[0]=0x000000dau;
    /* 0044f72d */
    f->r[4]+=4;return;
mi_0044f72e:;
    /* 0044f72e */
    f->r[0]=0x000000deu;
    /* 0044f733 */
    f->r[4]+=4;return;
mi_0044f734:;
    /* 0044f734 */
    f->r[0]=0x000000e2u;
    /* 0044f739 */
    f->r[4]+=4;return;
mi_0044f73a:;
    /* 0044f73a */
    f->r[0]=0x000000e6u;
    /* 0044f73f */
    f->r[4]+=4;return;
mi_0044f740:;
    /* 0044f740 */
    f->r[0]=0x000000eau;
    /* 0044f745 */
    f->r[4]+=4;return;
mi_0044f746:;
    /* 0044f746 */
    f->r[0]=0x000000eeu;
    /* 0044f74b */
    f->r[4]+=4;return;
mi_0044f74c:;
    /* 0044f74c */
    f->r[0]=0x000000f2u;
    /* 0044f751 */
    f->r[4]+=4;return;
mi_0044f752:;
    /* 0044f752 */
    f->r[0]=0x000000d2u;
    /* 0044f757 */
    f->r[4]+=4;return;
mi_0044f758:;
    /* 0044f758 */
    f->r[0]=0x000000d6u;
    /* 0044f75d */
    f->r[4]+=4;return;
mi_0044f75e:;
    /* 0044f75e */
    f->r[0]=0x000000cau;
    /* 0044f763 */
    f->r[4]+=4;return;
mi_0044f764:;
    /* 0044f764 */
    f->r[0]=0x000000ceu;
    /* 0044f769 */
    f->r[4]+=4;return;
mi_0044f76a:;
    /* 0044f76a */
    f->r[0]=0x000000c2u;
    /* 0044f76f */
    f->r[4]+=4;return;
mi_0044f770:;
    /* 0044f770 */
    f->r[0]=0x000000c6u;
    /* 0044f775 */
    f->r[4]+=4;return;
mi_0044f776:;
    /* 0044f776 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f778 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f77b */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0044f77e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f7e7;
    /* 0044f780 */
    switch(mi_read((0x0044f890u+f->r[0]*4u),4u)){case 0x0044f787u:goto mi_0044f787;case 0x0044f78du:goto mi_0044f78d;case 0x0044f793u:goto mi_0044f793;case 0x0044f799u:goto mi_0044f799;case 0x0044f79fu:goto mi_0044f79f;default:abort();}
mi_0044f787:;
    /* 0044f787 */
    f->r[0]=0x00000081u;
    /* 0044f78c */
    f->r[4]+=4;return;
mi_0044f78d:;
    /* 0044f78d */
    f->r[0]=0x00000085u;
    /* 0044f792 */
    f->r[4]+=4;return;
mi_0044f793:;
    /* 0044f793 */
    f->r[0]=0x00000089u;
    /* 0044f798 */
    f->r[4]+=4;return;
mi_0044f799:;
    /* 0044f799 */
    f->r[0]=0x0000008du;
    /* 0044f79e */
    f->r[4]+=4;return;
mi_0044f79f:;
    /* 0044f79f */
    f->r[0]=0x00000091u;
    /* 0044f7a4 */
    f->r[4]+=4;return;
mi_0044f7a5:;
    /* 0044f7a5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f7a7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f7aa */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f7ad */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044f7e7;
    /* 0044f7af */
    switch(mi_read((0x0044f8a4u+f->r[0]*4u),4u)){case 0x0044f7b6u:goto mi_0044f7b6;case 0x0044f7bcu:goto mi_0044f7bc;case 0x0044f7c2u:goto mi_0044f7c2;case 0x0044f7c8u:goto mi_0044f7c8;default:abort();}
mi_0044f7b6:;
    /* 0044f7b6 */
    f->r[0]=0x00000095u;
    /* 0044f7bb */
    f->r[4]+=4;return;
mi_0044f7bc:;
    /* 0044f7bc */
    f->r[0]=0x00000099u;
    /* 0044f7c1 */
    f->r[4]+=4;return;
mi_0044f7c2:;
    /* 0044f7c2 */
    f->r[0]=0x0000009du;
    /* 0044f7c7 */
    f->r[4]+=4;return;
mi_0044f7c8:;
    /* 0044f7c8 */
    f->r[0]=0x000000a1u;
    /* 0044f7cd */
    f->r[4]+=4;return;
mi_0044f7ce:;
    /* 0044f7ce */
    f->r[0]=0x000000fau;
    /* 0044f7d3 */
    f->r[4]+=4;return;
mi_0044f7d4:;
    /* 0044f7d4 */
    f->r[0]=0x000000feu;
    /* 0044f7d9 */
    f->r[4]+=4;return;
mi_0044f7da:;
    /* 0044f7da */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f7dd */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044f7df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044f7e1 */
    a=f->r[0];b=0x00000125u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044f7e6 */
    f->r[4]+=4;return;
mi_0044f7e7:;
    /* 0044f7e7 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044f7ea */
    f->r[4]+=4;return;
}

void restore_event_C_0044f8c0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044f8c0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f8c2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f8c5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f8c6 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);
    /* 0044f8c9 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fa47;
    /* 0044f8cf */
    switch(mi_read((0x0044fa4cu+f->r[0]*4u),4u)){case 0x0044f8d6u:goto mi_0044f8d6;case 0x0044f95bu:goto mi_0044f95b;case 0x0044f988u:goto mi_0044f988;case 0x0044f98eu:goto mi_0044f98e;case 0x0044f994u:goto mi_0044f994;case 0x0044f99au:goto mi_0044f99a;case 0x0044f9a0u:goto mi_0044f9a0;case 0x0044f9a6u:goto mi_0044f9a6;case 0x0044f9acu:goto mi_0044f9ac;case 0x0044f9b2u:goto mi_0044f9b2;case 0x0044f9b8u:goto mi_0044f9b8;case 0x0044f9beu:goto mi_0044f9be;case 0x0044f9c4u:goto mi_0044f9c4;case 0x0044f9cau:goto mi_0044f9ca;case 0x0044f9d0u:goto mi_0044f9d0;case 0x0044f9d6u:goto mi_0044f9d6;case 0x0044fa05u:goto mi_0044fa05;case 0x0044fa2eu:goto mi_0044fa2e;case 0x0044fa34u:goto mi_0044fa34;case 0x0044fa3au:goto mi_0044fa3a;case 0x0044fa47u:goto mi_0044fa47;default:abort();}
mi_0044f8d6:;
    /* 0044f8d6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f8d9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044f8db */
    if((f->flags&MI_ZF))goto mi_0044f91c;
    /* 0044f8dd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f8df */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f8e2 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f8e5 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fa47;
    /* 0044f8eb */
    switch(mi_read((0x0044faa8u+f->r[0]*4u),4u)){case 0x0044f8f2u:goto mi_0044f8f2;case 0x0044f8f8u:goto mi_0044f8f8;case 0x0044f8feu:goto mi_0044f8fe;case 0x0044f904u:goto mi_0044f904;case 0x0044f90au:goto mi_0044f90a;case 0x0044f910u:goto mi_0044f910;case 0x0044f916u:goto mi_0044f916;default:abort();}
mi_0044f8f2:;
    /* 0044f8f2 */
    f->r[0]=0x0000002eu;
    /* 0044f8f7 */
    f->r[4]+=4;return;
mi_0044f8f8:;
    /* 0044f8f8 */
    f->r[0]=0x00000008u;
    /* 0044f8fd */
    f->r[4]+=4;return;
mi_0044f8fe:;
    /* 0044f8fe */
    f->r[0]=0x0000001au;
    /* 0044f903 */
    f->r[4]+=4;return;
mi_0044f904:;
    /* 0044f904 */
    f->r[0]=0x00000040u;
    /* 0044f909 */
    f->r[4]+=4;return;
mi_0044f90a:;
    /* 0044f90a */
    f->r[0]=0x00000052u;
    /* 0044f90f */
    f->r[4]+=4;return;
mi_0044f910:;
    /* 0044f910 */
    f->r[0]=0x00000064u;
    /* 0044f915 */
    f->r[4]+=4;return;
mi_0044f916:;
    /* 0044f916 */
    f->r[0]=0x00000076u;
    /* 0044f91b */
    f->r[4]+=4;return;
mi_0044f91c:;
    /* 0044f91c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f91e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f921 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044f924 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fa47;
    /* 0044f92a */
    switch(mi_read((0x0044fac4u+f->r[0]*4u),4u)){case 0x0044f931u:goto mi_0044f931;case 0x0044f937u:goto mi_0044f937;case 0x0044f93du:goto mi_0044f93d;case 0x0044f943u:goto mi_0044f943;case 0x0044f949u:goto mi_0044f949;case 0x0044f94fu:goto mi_0044f94f;case 0x0044f955u:goto mi_0044f955;default:abort();}
mi_0044f931:;
    /* 0044f931 */
    f->r[0]=0x0000002fu;
    /* 0044f936 */
    f->r[4]+=4;return;
mi_0044f937:;
    /* 0044f937 */
    f->r[0]=0x00000009u;
    /* 0044f93c */
    f->r[4]+=4;return;
mi_0044f93d:;
    /* 0044f93d */
    f->r[0]=0x0000001bu;
    /* 0044f942 */
    f->r[4]+=4;return;
mi_0044f943:;
    /* 0044f943 */
    f->r[0]=0x00000041u;
    /* 0044f948 */
    f->r[4]+=4;return;
mi_0044f949:;
    /* 0044f949 */
    f->r[0]=0x00000053u;
    /* 0044f94e */
    f->r[4]+=4;return;
mi_0044f94f:;
    /* 0044f94f */
    f->r[0]=0x00000065u;
    /* 0044f954 */
    f->r[4]+=4;return;
mi_0044f955:;
    /* 0044f955 */
    f->r[0]=0x00000077u;
    /* 0044f95a */
    f->r[4]+=4;return;
mi_0044f95b:;
    /* 0044f95b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[1]),2u);
    /* 0044f95f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044f960 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044f963 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fa47;
    /* 0044f969 */
    switch(mi_read((0x0044fae0u+f->r[0]*4u),4u)){case 0x0044f970u:goto mi_0044f970;case 0x0044f976u:goto mi_0044f976;case 0x0044f97cu:goto mi_0044f97c;case 0x0044f982u:goto mi_0044f982;default:abort();}
mi_0044f970:;
    /* 0044f970 */
    f->r[0]=0x000000bdu;
    /* 0044f975 */
    f->r[4]+=4;return;
mi_0044f976:;
    /* 0044f976 */
    f->r[0]=0x000000b3u;
    /* 0044f97b */
    f->r[4]+=4;return;
mi_0044f97c:;
    /* 0044f97c */
    f->r[0]=0x00000106u;
    /* 0044f981 */
    f->r[4]+=4;return;
mi_0044f982:;
    /* 0044f982 */
    f->r[0]=0x000000a9u;
    /* 0044f987 */
    f->r[4]+=4;return;
mi_0044f988:;
    /* 0044f988 */
    f->r[0]=0x000000d9u;
    /* 0044f98d */
    f->r[4]+=4;return;
mi_0044f98e:;
    /* 0044f98e */
    f->r[0]=0x000000ddu;
    /* 0044f993 */
    f->r[4]+=4;return;
mi_0044f994:;
    /* 0044f994 */
    f->r[0]=0x000000e1u;
    /* 0044f999 */
    f->r[4]+=4;return;
mi_0044f99a:;
    /* 0044f99a */
    f->r[0]=0x000000e5u;
    /* 0044f99f */
    f->r[4]+=4;return;
mi_0044f9a0:;
    /* 0044f9a0 */
    f->r[0]=0x000000e9u;
    /* 0044f9a5 */
    f->r[4]+=4;return;
mi_0044f9a6:;
    /* 0044f9a6 */
    f->r[0]=0x000000edu;
    /* 0044f9ab */
    f->r[4]+=4;return;
mi_0044f9ac:;
    /* 0044f9ac */
    f->r[0]=0x000000f1u;
    /* 0044f9b1 */
    f->r[4]+=4;return;
mi_0044f9b2:;
    /* 0044f9b2 */
    f->r[0]=0x000000d1u;
    /* 0044f9b7 */
    f->r[4]+=4;return;
mi_0044f9b8:;
    /* 0044f9b8 */
    f->r[0]=0x000000d5u;
    /* 0044f9bd */
    f->r[4]+=4;return;
mi_0044f9be:;
    /* 0044f9be */
    f->r[0]=0x000000c9u;
    /* 0044f9c3 */
    f->r[4]+=4;return;
mi_0044f9c4:;
    /* 0044f9c4 */
    f->r[0]=0x000000cdu;
    /* 0044f9c9 */
    f->r[4]+=4;return;
mi_0044f9ca:;
    /* 0044f9ca */
    f->r[0]=0x000000c1u;
    /* 0044f9cf */
    f->r[4]+=4;return;
mi_0044f9d0:;
    /* 0044f9d0 */
    f->r[0]=0x000000c5u;
    /* 0044f9d5 */
    f->r[4]+=4;return;
mi_0044f9d6:;
    /* 0044f9d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044f9d8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044f9db */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0044f9de */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fa47;
    /* 0044f9e0 */
    switch(mi_read((0x0044faf0u+f->r[0]*4u),4u)){case 0x0044f9e7u:goto mi_0044f9e7;case 0x0044f9edu:goto mi_0044f9ed;case 0x0044f9f3u:goto mi_0044f9f3;case 0x0044f9f9u:goto mi_0044f9f9;case 0x0044f9ffu:goto mi_0044f9ff;default:abort();}
mi_0044f9e7:;
    /* 0044f9e7 */
    f->r[0]=0x00000080u;
    /* 0044f9ec */
    f->r[4]+=4;return;
mi_0044f9ed:;
    /* 0044f9ed */
    f->r[0]=0x00000084u;
    /* 0044f9f2 */
    f->r[4]+=4;return;
mi_0044f9f3:;
    /* 0044f9f3 */
    f->r[0]=0x00000088u;
    /* 0044f9f8 */
    f->r[4]+=4;return;
mi_0044f9f9:;
    /* 0044f9f9 */
    f->r[0]=0x0000008cu;
    /* 0044f9fe */
    f->r[4]+=4;return;
mi_0044f9ff:;
    /* 0044f9ff */
    f->r[0]=0x00000090u;
    /* 0044fa04 */
    f->r[4]+=4;return;
mi_0044fa05:;
    /* 0044fa05 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fa07 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fa0a */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044fa0d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fa47;
    /* 0044fa0f */
    switch(mi_read((0x0044fb04u+f->r[0]*4u),4u)){case 0x0044fa16u:goto mi_0044fa16;case 0x0044fa1cu:goto mi_0044fa1c;case 0x0044fa22u:goto mi_0044fa22;case 0x0044fa28u:goto mi_0044fa28;default:abort();}
mi_0044fa16:;
    /* 0044fa16 */
    f->r[0]=0x00000094u;
    /* 0044fa1b */
    f->r[4]+=4;return;
mi_0044fa1c:;
    /* 0044fa1c */
    f->r[0]=0x00000098u;
    /* 0044fa21 */
    f->r[4]+=4;return;
mi_0044fa22:;
    /* 0044fa22 */
    f->r[0]=0x0000009cu;
    /* 0044fa27 */
    f->r[4]+=4;return;
mi_0044fa28:;
    /* 0044fa28 */
    f->r[0]=0x000000a0u;
    /* 0044fa2d */
    f->r[4]+=4;return;
mi_0044fa2e:;
    /* 0044fa2e */
    f->r[0]=0x000000f9u;
    /* 0044fa33 */
    f->r[4]+=4;return;
mi_0044fa34:;
    /* 0044fa34 */
    f->r[0]=0x000000fdu;
    /* 0044fa39 */
    f->r[4]+=4;return;
mi_0044fa3a:;
    /* 0044fa3a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fa3d */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044fa3f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044fa41 */
    a=f->r[0];b=0x0000011fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044fa46 */
    f->r[4]+=4;return;
mi_0044fa47:;
    /* 0044fa47 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044fa4a */
    f->r[4]+=4;return;
}

void restore_event_C_0044fb20(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044fb20 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fb22 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fb25 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044fb26 */
    if((f->flags&MI_ZF))goto mi_0044fb3e;
    /* 0044fb28 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044fb2b */
    if(!(f->flags&MI_ZF))goto mi_0044fbbb;
    /* 0044fb31 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fb34 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044fb36 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044fb38 */
    a=f->r[0];b=0x00000123u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044fb3d */
    f->r[4]+=4;return;
mi_0044fb3e:;
    /* 0044fb3e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fb41 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044fb43 */
    if((f->flags&MI_ZF))goto mi_0044fb80;
    /* 0044fb45 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fb47 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fb4a */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044fb4d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fbbb;
    /* 0044fb4f */
    switch(mi_read((0x0044fbc0u+f->r[0]*4u),4u)){case 0x0044fb56u:goto mi_0044fb56;case 0x0044fb5cu:goto mi_0044fb5c;case 0x0044fb62u:goto mi_0044fb62;case 0x0044fb68u:goto mi_0044fb68;case 0x0044fb6eu:goto mi_0044fb6e;case 0x0044fb74u:goto mi_0044fb74;case 0x0044fb7au:goto mi_0044fb7a;default:abort();}
mi_0044fb56:;
    /* 0044fb56 */
    f->r[0]=0x00000032u;
    /* 0044fb5b */
    f->r[4]+=4;return;
mi_0044fb5c:;
    /* 0044fb5c */
    f->r[0]=0x0000000cu;
    /* 0044fb61 */
    f->r[4]+=4;return;
mi_0044fb62:;
    /* 0044fb62 */
    f->r[0]=0x0000001eu;
    /* 0044fb67 */
    f->r[4]+=4;return;
mi_0044fb68:;
    /* 0044fb68 */
    f->r[0]=0x00000044u;
    /* 0044fb6d */
    f->r[4]+=4;return;
mi_0044fb6e:;
    /* 0044fb6e */
    f->r[0]=0x00000056u;
    /* 0044fb73 */
    f->r[4]+=4;return;
mi_0044fb74:;
    /* 0044fb74 */
    f->r[0]=0x00000068u;
    /* 0044fb79 */
    f->r[4]+=4;return;
mi_0044fb7a:;
    /* 0044fb7a */
    f->r[0]=0x0000007au;
    /* 0044fb7f */
    f->r[4]+=4;return;
mi_0044fb80:;
    /* 0044fb80 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fb82 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fb85 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044fb88 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fbbb;
    /* 0044fb8a */
    switch(mi_read((0x0044fbdcu+f->r[0]*4u),4u)){case 0x0044fb91u:goto mi_0044fb91;case 0x0044fb97u:goto mi_0044fb97;case 0x0044fb9du:goto mi_0044fb9d;case 0x0044fba3u:goto mi_0044fba3;case 0x0044fba9u:goto mi_0044fba9;case 0x0044fbafu:goto mi_0044fbaf;case 0x0044fbb5u:goto mi_0044fbb5;default:abort();}
mi_0044fb91:;
    /* 0044fb91 */
    f->r[0]=0x00000033u;
    /* 0044fb96 */
    f->r[4]+=4;return;
mi_0044fb97:;
    /* 0044fb97 */
    f->r[0]=0x0000000du;
    /* 0044fb9c */
    f->r[4]+=4;return;
mi_0044fb9d:;
    /* 0044fb9d */
    f->r[0]=0x0000001fu;
    /* 0044fba2 */
    f->r[4]+=4;return;
mi_0044fba3:;
    /* 0044fba3 */
    f->r[0]=0x00000045u;
    /* 0044fba8 */
    f->r[4]+=4;return;
mi_0044fba9:;
    /* 0044fba9 */
    f->r[0]=0x00000057u;
    /* 0044fbae */
    f->r[4]+=4;return;
mi_0044fbaf:;
    /* 0044fbaf */
    f->r[0]=0x00000069u;
    /* 0044fbb4 */
    f->r[4]+=4;return;
mi_0044fbb5:;
    /* 0044fbb5 */
    f->r[0]=0x0000007bu;
    /* 0044fbba */
    f->r[4]+=4;return;
mi_0044fbbb:;
    /* 0044fbbb */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044fbbe */
    f->r[4]+=4;return;
}

void restore_event_C_0044fc00(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044fc00 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fc02 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000002u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fc05 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044fc06 */
    if((f->flags&MI_ZF))goto mi_0044fc1e;
    /* 0044fc08 */
    a=f->r[0];b=0x00000016u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044fc0b */
    if(!(f->flags&MI_ZF))goto mi_0044fc9b;
    /* 0044fc11 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fc14 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044fc16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044fc18 */
    a=f->r[0];b=0x00000121u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044fc1d */
    f->r[4]+=4;return;
mi_0044fc1e:;
    /* 0044fc1e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000056u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fc21 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044fc23 */
    if((f->flags&MI_ZF))goto mi_0044fc60;
    /* 0044fc25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fc27 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fc2a */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044fc2d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fc9b;
    /* 0044fc2f */
    switch(mi_read((0x0044fca0u+f->r[0]*4u),4u)){case 0x0044fc36u:goto mi_0044fc36;case 0x0044fc3cu:goto mi_0044fc3c;case 0x0044fc42u:goto mi_0044fc42;case 0x0044fc48u:goto mi_0044fc48;case 0x0044fc4eu:goto mi_0044fc4e;case 0x0044fc54u:goto mi_0044fc54;case 0x0044fc5au:goto mi_0044fc5a;default:abort();}
mi_0044fc36:;
    /* 0044fc36 */
    f->r[0]=0x00000030u;
    /* 0044fc3b */
    f->r[4]+=4;return;
mi_0044fc3c:;
    /* 0044fc3c */
    f->r[0]=0x0000000au;
    /* 0044fc41 */
    f->r[4]+=4;return;
mi_0044fc42:;
    /* 0044fc42 */
    f->r[0]=0x0000001cu;
    /* 0044fc47 */
    f->r[4]+=4;return;
mi_0044fc48:;
    /* 0044fc48 */
    f->r[0]=0x00000042u;
    /* 0044fc4d */
    f->r[4]+=4;return;
mi_0044fc4e:;
    /* 0044fc4e */
    f->r[0]=0x00000054u;
    /* 0044fc53 */
    f->r[4]+=4;return;
mi_0044fc54:;
    /* 0044fc54 */
    f->r[0]=0x00000066u;
    /* 0044fc59 */
    f->r[4]+=4;return;
mi_0044fc5a:;
    /* 0044fc5a */
    f->r[0]=0x00000078u;
    /* 0044fc5f */
    f->r[4]+=4;return;
mi_0044fc60:;
    /* 0044fc60 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fc62 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 0044fc65 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044fc68 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fc9b;
    /* 0044fc6a */
    switch(mi_read((0x0044fcbcu+f->r[0]*4u),4u)){case 0x0044fc71u:goto mi_0044fc71;case 0x0044fc77u:goto mi_0044fc77;case 0x0044fc7du:goto mi_0044fc7d;case 0x0044fc83u:goto mi_0044fc83;case 0x0044fc89u:goto mi_0044fc89;case 0x0044fc8fu:goto mi_0044fc8f;case 0x0044fc95u:goto mi_0044fc95;default:abort();}
mi_0044fc71:;
    /* 0044fc71 */
    f->r[0]=0x00000031u;
    /* 0044fc76 */
    f->r[4]+=4;return;
mi_0044fc77:;
    /* 0044fc77 */
    f->r[0]=0x0000000bu;
    /* 0044fc7c */
    f->r[4]+=4;return;
mi_0044fc7d:;
    /* 0044fc7d */
    f->r[0]=0x0000001du;
    /* 0044fc82 */
    f->r[4]+=4;return;
mi_0044fc83:;
    /* 0044fc83 */
    f->r[0]=0x00000043u;
    /* 0044fc88 */
    f->r[4]+=4;return;
mi_0044fc89:;
    /* 0044fc89 */
    f->r[0]=0x00000055u;
    /* 0044fc8e */
    f->r[4]+=4;return;
mi_0044fc8f:;
    /* 0044fc8f */
    f->r[0]=0x00000067u;
    /* 0044fc94 */
    f->r[4]+=4;return;
mi_0044fc95:;
    /* 0044fc95 */
    f->r[0]=0x00000079u;
    /* 0044fc9a */
    f->r[4]+=4;return;
mi_0044fc9b:;
    /* 0044fc9b */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044fc9e */
    f->r[4]+=4;return;
}

void restore_event_C_0044fce0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044fce0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fce2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 0044fce5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044fce6 */
    if(!(f->flags&MI_ZF))goto mi_0044fd11;
    /* 0044fce8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[1])))&0xffu)<<0);
    /* 0044fceb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044fced */
    if((f->flags&MI_ZF))goto mi_0044fd06;
    /* 0044fcef */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fcf1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000060u+f->r[1])))&0xffu)<<0);
    /* 0044fcf4 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044fcf5 */
    if((f->flags&MI_ZF))goto mi_0044fd00;
    /* 0044fcf7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044fcf8 */
    if(!(f->flags&MI_ZF))goto mi_0044fd11;
    /* 0044fcfa */
    f->r[0]=0x00000020u;
    /* 0044fcff */
    f->r[4]+=4;return;
mi_0044fd00:;
    /* 0044fd00 */
    f->r[0]=0x0000000eu;
    /* 0044fd05 */
    f->r[4]+=4;return;
mi_0044fd06:;
    /* 0044fd06 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fd08 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000060u+f->r[1])))&0xffu)<<0);
    /* 0044fd0b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044fd0c */
    if((f->flags&MI_ZF))goto mi_0044fd1b;
    /* 0044fd0e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044fd0f */
    if((f->flags&MI_ZF))goto mi_0044fd15;
mi_0044fd11:;
    /* 0044fd11 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044fd14 */
    f->r[4]+=4;return;
mi_0044fd15:;
    /* 0044fd15 */
    f->r[0]=0x00000021u;
    /* 0044fd1a */
    f->r[4]+=4;return;
mi_0044fd1b:;
    /* 0044fd1b */
    f->r[0]=0x0000000fu;
    /* 0044fd20 */
    f->r[4]+=4;return;
}

void restore_event_C_0044fd30(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044fd30 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fd31 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fd32 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fd33 */
    f->r[6]=f->r[1];
    /* 0044fd35 */
    f->r[1]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0044fd39 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fd3a */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044fd3d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044fe94;
    /* 0044fd43 */
    switch(mi_read((0x0044fea0u+f->r[1]*4u),4u)){case 0x0044fd4au:goto mi_0044fd4a;case 0x0044fd55u:goto mi_0044fd55;case 0x0044fd60u:goto mi_0044fd60;case 0x0044fd6bu:goto mi_0044fd6b;default:abort();}
mi_0044fd4a:;
    /* 0044fd4a */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0044fd4e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fd50 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0000006au+f->r[6]),1u))&0xffu)<<0);
    /* 0044fd53 */
    goto mi_0044fd74;
mi_0044fd55:;
    /* 0044fd55 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000026u+f->r[6]),2u);
    /* 0044fd59 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fd5b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0000006bu+f->r[6]),1u))&0xffu)<<0);
    /* 0044fd5e */
    goto mi_0044fd74;
mi_0044fd60:;
    /* 0044fd60 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000052u+f->r[6]),2u);
    /* 0044fd64 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fd66 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0000006cu+f->r[6]),1u))&0xffu)<<0);
    /* 0044fd69 */
    goto mi_0044fd74;
mi_0044fd6b:;
    /* 0044fd6b */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000054u+f->r[6]),2u);
    /* 0044fd6f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fd71 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0000006du+f->r[6]),1u))&0xffu)<<0);
mi_0044fd74:;
    /* 0044fd74 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044fd76 */
    if(mi_signed_less(f->flags))goto mi_0044fd82;
    /* 0044fd78 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0044fd7b */
    if(!mi_signed_less(f->flags))goto mi_0044fd82;
    /* 0044fd7d */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044fd80 */
    if(!(f->flags&MI_ZF))goto mi_0044fde1;
mi_0044fd82:;
    /* 0044fd82 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0044fd84 */
    if(mi_signed_less(f->flags))goto mi_0044fe94;
    /* 0044fd8a */
    a=f->r[7];b=mi_read((0x00785d70u),4u);t=mi_alu(f,a,b,32,1);
    /* 0044fd90 */
    if(!mi_signed_less(f->flags))goto mi_0044fe94;
    /* 0044fd96 */
    f->r[0]=f->r[7];
    /* 0044fd98 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0044fd9b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044fd9d */
    f->r[5]=(0x00785d74u+f->r[0]*4u);
    /* 0044fda4 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0044fda6 */
    if((f->flags&MI_ZF))goto mi_0044fe94;
    /* 0044fdac */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fdad */
    f->r[1]=f->r[6];
    /* 0044fdaf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fdb4u;f->boundary(0x0044fdafu,0x0044f120u,f);
    /* 0044fdb4 */
    f->r[1]=f->r[5];
    /* 0044fdb6 */
    f->r[3]=f->r[0];
    /* 0044fdb8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fdbdu;f->boundary(0x0044fdb8u,0x0044f470u,f);
    /* 0044fdbd */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0044fdbf */
    if(mi_signed_less(f->flags))goto mi_0044fe20;
    /* 0044fdc1 */
    a=f->r[3];b=0x00000036u;t=mi_alu(f,a,b,32,1);
    /* 0044fdc4 */
    if(!mi_signed_less(f->flags))goto mi_0044fe20;
    /* 0044fdc6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044fdc8 */
    if(mi_signed_less(f->flags))goto mi_0044fe20;
    /* 0044fdca */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0044fdcd */
    if(!mi_signed_less(f->flags))goto mi_0044fe20;
    /* 0044fdcf */
    f->r[1]=(0x00000000u+f->r[3]+f->r[3]*8u);
    /* 0044fdd2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044fdd4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007854a8u+f->r[0]+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0044fddb */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0044fddf */
    f->r[0]=f->r[2];
mi_0044fde1:;
    /* 0044fde1 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044fde4 */
    if(mi_signed_less(f->flags))goto mi_0044fe28;
    /* 0044fde6 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0044fde9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0044fe28;
    /* 0044fdeb */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0044fded */
    if(mi_signed_less(f->flags))goto mi_0044fe28;
    /* 0044fdef */
    a=f->r[7];b=mi_read((0x00785d70u),4u);t=mi_alu(f,a,b,32,1);
    /* 0044fdf5 */
    if(!mi_signed_less(f->flags))goto mi_0044fe28;
    /* 0044fdf7 */
    f->r[2]=f->r[7];
    /* 0044fdf9 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0044fdfc */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044fdfe */
    f->r[7]=(0x00785d74u+f->r[2]*4u);
    /* 0044fe05 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0044fe07 */
    if((f->flags&MI_ZF))goto mi_0044fe28;
    /* 0044fe09 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe0a */
    f->r[1]=f->r[7];
    /* 0044fe0c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe11u;f->boundary(0x0044fe0cu,0x0044f660u,f);
    /* 0044fe11 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe12 */
    f->r[1]=f->r[7];
    /* 0044fe14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe19u;f->boundary(0x0044fe14u,0x0044fec0u,f);
    /* 0044fe19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044fe1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044fe1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044fe1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044fe1d */
    f->r[4]+=8;return;
mi_0044fe20:;
    /* 0044fe20 */
    f->r[1]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0044fe24 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044fe26 */
    goto mi_0044fe31;
mi_0044fe28:;
    /* 0044fe28 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044fe2a */
    if(mi_signed_less(f->flags))goto mi_0044fe94;
    /* 0044fe2c */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0044fe2f */
    if(!mi_signed_less(f->flags))goto mi_0044fe94;
mi_0044fe31:;
    /* 0044fe31 */
    switch(mi_read((0x0044feb0u+f->r[1]*4u),4u)){case 0x0044fe38u:goto mi_0044fe38;case 0x0044fe4fu:goto mi_0044fe4f;case 0x0044fe66u:goto mi_0044fe66;case 0x0044fe7du:goto mi_0044fe7d;default:abort();}
mi_0044fe38:;
    /* 0044fe38 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe39 */
    f->r[1]=f->r[6];
    /* 0044fe3b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe40u;f->boundary(0x0044fe3bu,0x0044f8c0u,f);
    /* 0044fe40 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe41 */
    f->r[1]=f->r[6];
    /* 0044fe43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe48u;f->boundary(0x0044fe43u,0x0044fec0u,f);
    /* 0044fe48 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044fe49 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044fe4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044fe4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044fe4c */
    f->r[4]+=8;return;
mi_0044fe4f:;
    /* 0044fe4f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe50 */
    f->r[1]=f->r[6];
    /* 0044fe52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe57u;f->boundary(0x0044fe52u,0x0044fb20u,f);
    /* 0044fe57 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe58 */
    f->r[1]=f->r[6];
    /* 0044fe5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe5fu;f->boundary(0x0044fe5au,0x0044fec0u,f);
    /* 0044fe5f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044fe60 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044fe61 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044fe62 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044fe63 */
    f->r[4]+=8;return;
mi_0044fe66:;
    /* 0044fe66 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe67 */
    f->r[1]=f->r[6];
    /* 0044fe69 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe6eu;f->boundary(0x0044fe69u,0x0044fc00u,f);
    /* 0044fe6e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe6f */
    f->r[1]=f->r[6];
    /* 0044fe71 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe76u;f->boundary(0x0044fe71u,0x0044fec0u,f);
    /* 0044fe76 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044fe77 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044fe78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044fe79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044fe7a */
    f->r[4]+=8;return;
mi_0044fe7d:;
    /* 0044fe7d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe7e */
    f->r[1]=f->r[6];
    /* 0044fe80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe85u;f->boundary(0x0044fe80u,0x0044fce0u,f);
    /* 0044fe85 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044fe86 */
    f->r[1]=f->r[6];
    /* 0044fe88 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044fe8du;f->boundary(0x0044fe88u,0x0044fec0u,f);
    /* 0044fe8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044fe8e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044fe8f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044fe90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044fe91 */
    f->r[4]+=8;return;
mi_0044fe94:;
    /* 0044fe94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044fe95 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044fe96 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044fe97 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044fe9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044fe9b */
    f->r[4]+=8;return;
}

void restore_event_C_0044fec0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044fec0 */
    f->r[0]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0044fec4 */
    f->r[1]=0x00000003u;
    /* 0044fec9 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044feca */
    a=f->r[1];mi_idiv32(f,a);
    /* 0044fecc */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0044fed0 */
    a=f->r[0];b=0xfffffff8u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044fed3 */
    a=f->r[0];b=0x0000011du;t=mi_alu(f,a,b,32,1);
    /* 0044fed8 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00451213;
    /* 0044fede */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044fee0 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00451418u+f->r[0]),1u))&0xffu)<<0);
    /* 0044fee6 */
    switch(mi_read((0x0045121cu+f->r[1]*4u),4u)){case 0x0044feedu:goto mi_0044feed;case 0x0044ff14u:goto mi_0044ff14;case 0x0044ff3bu:goto mi_0044ff3b;case 0x0044ff62u:goto mi_0044ff62;case 0x0044ff89u:goto mi_0044ff89;case 0x0044ffb0u:goto mi_0044ffb0;case 0x0044ffd7u:goto mi_0044ffd7;case 0x0044fffeu:goto mi_0044fffe;case 0x00450025u:goto mi_00450025;case 0x0045004cu:goto mi_0045004c;case 0x00450073u:goto mi_00450073;case 0x0045009au:goto mi_0045009a;case 0x004500c1u:goto mi_004500c1;case 0x004500e8u:goto mi_004500e8;case 0x0045010fu:goto mi_0045010f;case 0x00450136u:goto mi_00450136;case 0x0045015du:goto mi_0045015d;case 0x00450184u:goto mi_00450184;case 0x004501abu:goto mi_004501ab;case 0x004501d2u:goto mi_004501d2;case 0x004501f9u:goto mi_004501f9;case 0x00450220u:goto mi_00450220;case 0x00450247u:goto mi_00450247;case 0x0045026eu:goto mi_0045026e;case 0x00450295u:goto mi_00450295;case 0x004502bcu:goto mi_004502bc;case 0x004502e3u:goto mi_004502e3;case 0x0045030au:goto mi_0045030a;case 0x00450331u:goto mi_00450331;case 0x00450358u:goto mi_00450358;case 0x0045037fu:goto mi_0045037f;case 0x004503a6u:goto mi_004503a6;case 0x004503cdu:goto mi_004503cd;case 0x004503f4u:goto mi_004503f4;case 0x0045041bu:goto mi_0045041b;case 0x00450442u:goto mi_00450442;case 0x00450469u:goto mi_00450469;case 0x00450490u:goto mi_00450490;case 0x004504b7u:goto mi_004504b7;case 0x004504deu:goto mi_004504de;case 0x00450505u:goto mi_00450505;case 0x0045052cu:goto mi_0045052c;case 0x00450553u:goto mi_00450553;case 0x0045057au:goto mi_0045057a;case 0x004505a1u:goto mi_004505a1;case 0x004505c8u:goto mi_004505c8;case 0x004505efu:goto mi_004505ef;case 0x00450616u:goto mi_00450616;case 0x0045063du:goto mi_0045063d;case 0x00450664u:goto mi_00450664;case 0x0045068bu:goto mi_0045068b;case 0x004506b2u:goto mi_004506b2;case 0x004506d9u:goto mi_004506d9;case 0x00450700u:goto mi_00450700;case 0x00450727u:goto mi_00450727;case 0x0045074eu:goto mi_0045074e;case 0x00450775u:goto mi_00450775;case 0x0045079cu:goto mi_0045079c;case 0x004507c3u:goto mi_004507c3;case 0x004507eau:goto mi_004507ea;case 0x00450811u:goto mi_00450811;case 0x00450838u:goto mi_00450838;case 0x0045085fu:goto mi_0045085f;case 0x00450886u:goto mi_00450886;case 0x004508adu:goto mi_004508ad;case 0x004508d4u:goto mi_004508d4;case 0x004508fbu:goto mi_004508fb;case 0x00450922u:goto mi_00450922;case 0x00450949u:goto mi_00450949;case 0x00450970u:goto mi_00450970;case 0x00450997u:goto mi_00450997;case 0x004509beu:goto mi_004509be;case 0x004509e5u:goto mi_004509e5;case 0x00450a0cu:goto mi_00450a0c;case 0x00450a33u:goto mi_00450a33;case 0x00450a5au:goto mi_00450a5a;case 0x00450a81u:goto mi_00450a81;case 0x00450aa8u:goto mi_00450aa8;case 0x00450acfu:goto mi_00450acf;case 0x00450af6u:goto mi_00450af6;case 0x00450b1du:goto mi_00450b1d;case 0x00450b44u:goto mi_00450b44;case 0x00450b6bu:goto mi_00450b6b;case 0x00450b92u:goto mi_00450b92;case 0x00450bb9u:goto mi_00450bb9;case 0x00450be0u:goto mi_00450be0;case 0x00450c07u:goto mi_00450c07;case 0x00450c2eu:goto mi_00450c2e;case 0x00450c55u:goto mi_00450c55;case 0x00450c7cu:goto mi_00450c7c;case 0x00450ca3u:goto mi_00450ca3;case 0x00450ccau:goto mi_00450cca;case 0x00450cf1u:goto mi_00450cf1;case 0x00450d18u:goto mi_00450d18;case 0x00450d3fu:goto mi_00450d3f;case 0x00450d66u:goto mi_00450d66;case 0x00450d8du:goto mi_00450d8d;case 0x00450db4u:goto mi_00450db4;case 0x00450ddbu:goto mi_00450ddb;case 0x00450e02u:goto mi_00450e02;case 0x00450e29u:goto mi_00450e29;case 0x00450e50u:goto mi_00450e50;case 0x00450e77u:goto mi_00450e77;case 0x00450e9eu:goto mi_00450e9e;case 0x00450ec5u:goto mi_00450ec5;case 0x00450eecu:goto mi_00450eec;case 0x00450f13u:goto mi_00450f13;case 0x00450f3au:goto mi_00450f3a;case 0x00450f61u:goto mi_00450f61;case 0x00450f88u:goto mi_00450f88;case 0x00450fafu:goto mi_00450faf;case 0x00450fd6u:goto mi_00450fd6;case 0x00450ffdu:goto mi_00450ffd;case 0x00451024u:goto mi_00451024;case 0x0045104bu:goto mi_0045104b;case 0x00451072u:goto mi_00451072;case 0x00451099u:goto mi_00451099;case 0x004510c0u:goto mi_004510c0;case 0x004510e7u:goto mi_004510e7;case 0x0045110eu:goto mi_0045110e;case 0x00451135u:goto mi_00451135;case 0x0045115cu:goto mi_0045115c;case 0x00451183u:goto mi_00451183;case 0x004511aau:goto mi_004511aa;case 0x004511cdu:goto mi_004511cd;case 0x004511f0u:goto mi_004511f0;case 0x00451213u:goto mi_00451213;default:abort();}
mi_0044feed:;
    /* 0044feed */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044fef0 */
    if((f->flags&MI_ZF))goto mi_0044ff0c;
    /* 0044fef2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044fef3 */
    if((f->flags&MI_ZF))goto mi_0044ff04;
    /* 0044fef5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044fef6 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0044fefc */
    f->r[0]=0x0000012bu;
    /* 0044ff01 */
    f->r[4]+=12;return;
mi_0044ff04:;
    /* 0044ff04 */
    f->r[0]=0x0000012au;
    /* 0044ff09 */
    f->r[4]+=12;return;
mi_0044ff0c:;
    /* 0044ff0c */
    f->r[0]=0x00000008u;
    /* 0044ff11 */
    f->r[4]+=12;return;
mi_0044ff14:;
    /* 0044ff14 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044ff17 */
    if((f->flags&MI_ZF))goto mi_0044ff33;
    /* 0044ff19 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff1a */
    if((f->flags&MI_ZF))goto mi_0044ff2b;
    /* 0044ff1c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff1d */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0044ff23 */
    f->r[0]=0x0000012du;
    /* 0044ff28 */
    f->r[4]+=12;return;
mi_0044ff2b:;
    /* 0044ff2b */
    f->r[0]=0x0000012cu;
    /* 0044ff30 */
    f->r[4]+=12;return;
mi_0044ff33:;
    /* 0044ff33 */
    f->r[0]=0x00000009u;
    /* 0044ff38 */
    f->r[4]+=12;return;
mi_0044ff3b:;
    /* 0044ff3b */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044ff3e */
    if((f->flags&MI_ZF))goto mi_0044ff5a;
    /* 0044ff40 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff41 */
    if((f->flags&MI_ZF))goto mi_0044ff52;
    /* 0044ff43 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff44 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0044ff4a */
    f->r[0]=0x0000012fu;
    /* 0044ff4f */
    f->r[4]+=12;return;
mi_0044ff52:;
    /* 0044ff52 */
    f->r[0]=0x0000012eu;
    /* 0044ff57 */
    f->r[4]+=12;return;
mi_0044ff5a:;
    /* 0044ff5a */
    f->r[0]=0x0000000au;
    /* 0044ff5f */
    f->r[4]+=12;return;
mi_0044ff62:;
    /* 0044ff62 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044ff65 */
    if((f->flags&MI_ZF))goto mi_0044ff81;
    /* 0044ff67 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff68 */
    if((f->flags&MI_ZF))goto mi_0044ff79;
    /* 0044ff6a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff6b */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0044ff71 */
    f->r[0]=0x00000131u;
    /* 0044ff76 */
    f->r[4]+=12;return;
mi_0044ff79:;
    /* 0044ff79 */
    f->r[0]=0x00000130u;
    /* 0044ff7e */
    f->r[4]+=12;return;
mi_0044ff81:;
    /* 0044ff81 */
    f->r[0]=0x0000000bu;
    /* 0044ff86 */
    f->r[4]+=12;return;
mi_0044ff89:;
    /* 0044ff89 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044ff8c */
    if((f->flags&MI_ZF))goto mi_0044ffa8;
    /* 0044ff8e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff8f */
    if((f->flags&MI_ZF))goto mi_0044ffa0;
    /* 0044ff91 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ff92 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0044ff98 */
    f->r[0]=0x00000133u;
    /* 0044ff9d */
    f->r[4]+=12;return;
mi_0044ffa0:;
    /* 0044ffa0 */
    f->r[0]=0x00000132u;
    /* 0044ffa5 */
    f->r[4]+=12;return;
mi_0044ffa8:;
    /* 0044ffa8 */
    f->r[0]=0x0000000cu;
    /* 0044ffad */
    f->r[4]+=12;return;
mi_0044ffb0:;
    /* 0044ffb0 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044ffb3 */
    if((f->flags&MI_ZF))goto mi_0044ffcf;
    /* 0044ffb5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ffb6 */
    if((f->flags&MI_ZF))goto mi_0044ffc7;
    /* 0044ffb8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ffb9 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0044ffbf */
    f->r[0]=0x00000135u;
    /* 0044ffc4 */
    f->r[4]+=12;return;
mi_0044ffc7:;
    /* 0044ffc7 */
    f->r[0]=0x00000134u;
    /* 0044ffcc */
    f->r[4]+=12;return;
mi_0044ffcf:;
    /* 0044ffcf */
    f->r[0]=0x0000000du;
    /* 0044ffd4 */
    f->r[4]+=12;return;
mi_0044ffd7:;
    /* 0044ffd7 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044ffda */
    if((f->flags&MI_ZF))goto mi_0044fff6;
    /* 0044ffdc */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ffdd */
    if((f->flags&MI_ZF))goto mi_0044ffee;
    /* 0044ffdf */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0044ffe0 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0044ffe6 */
    f->r[0]=0x00000137u;
    /* 0044ffeb */
    f->r[4]+=12;return;
mi_0044ffee:;
    /* 0044ffee */
    f->r[0]=0x00000136u;
    /* 0044fff3 */
    f->r[4]+=12;return;
mi_0044fff6:;
    /* 0044fff6 */
    f->r[0]=0x0000000eu;
    /* 0044fffb */
    f->r[4]+=12;return;
mi_0044fffe:;
    /* 0044fffe */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450001 */
    if((f->flags&MI_ZF))goto mi_0045001d;
    /* 00450003 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450004 */
    if((f->flags&MI_ZF))goto mi_00450015;
    /* 00450006 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450007 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045000d */
    f->r[0]=0x00000139u;
    /* 00450012 */
    f->r[4]+=12;return;
mi_00450015:;
    /* 00450015 */
    f->r[0]=0x00000138u;
    /* 0045001a */
    f->r[4]+=12;return;
mi_0045001d:;
    /* 0045001d */
    f->r[0]=0x0000000fu;
    /* 00450022 */
    f->r[4]+=12;return;
mi_00450025:;
    /* 00450025 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450028 */
    if((f->flags&MI_ZF))goto mi_00450044;
    /* 0045002a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045002b */
    if((f->flags&MI_ZF))goto mi_0045003c;
    /* 0045002d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045002e */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450034 */
    f->r[0]=0x0000013bu;
    /* 00450039 */
    f->r[4]+=12;return;
mi_0045003c:;
    /* 0045003c */
    f->r[0]=0x0000013au;
    /* 00450041 */
    f->r[4]+=12;return;
mi_00450044:;
    /* 00450044 */
    f->r[0]=0x00000010u;
    /* 00450049 */
    f->r[4]+=12;return;
mi_0045004c:;
    /* 0045004c */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045004f */
    if((f->flags&MI_ZF))goto mi_0045006b;
    /* 00450051 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450052 */
    if((f->flags&MI_ZF))goto mi_00450063;
    /* 00450054 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450055 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045005b */
    f->r[0]=0x0000013du;
    /* 00450060 */
    f->r[4]+=12;return;
mi_00450063:;
    /* 00450063 */
    f->r[0]=0x0000013cu;
    /* 00450068 */
    f->r[4]+=12;return;
mi_0045006b:;
    /* 0045006b */
    f->r[0]=0x00000011u;
    /* 00450070 */
    f->r[4]+=12;return;
mi_00450073:;
    /* 00450073 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450076 */
    if((f->flags&MI_ZF))goto mi_00450092;
    /* 00450078 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450079 */
    if((f->flags&MI_ZF))goto mi_0045008a;
    /* 0045007b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045007c */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450082 */
    f->r[0]=0x0000013fu;
    /* 00450087 */
    f->r[4]+=12;return;
mi_0045008a:;
    /* 0045008a */
    f->r[0]=0x0000013eu;
    /* 0045008f */
    f->r[4]+=12;return;
mi_00450092:;
    /* 00450092 */
    f->r[0]=0x0000001au;
    /* 00450097 */
    f->r[4]+=12;return;
mi_0045009a:;
    /* 0045009a */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045009d */
    if((f->flags&MI_ZF))goto mi_004500b9;
    /* 0045009f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004500a0 */
    if((f->flags&MI_ZF))goto mi_004500b1;
    /* 004500a2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004500a3 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004500a9 */
    f->r[0]=0x00000141u;
    /* 004500ae */
    f->r[4]+=12;return;
mi_004500b1:;
    /* 004500b1 */
    f->r[0]=0x00000140u;
    /* 004500b6 */
    f->r[4]+=12;return;
mi_004500b9:;
    /* 004500b9 */
    f->r[0]=0x0000001bu;
    /* 004500be */
    f->r[4]+=12;return;
mi_004500c1:;
    /* 004500c1 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004500c4 */
    if((f->flags&MI_ZF))goto mi_004500e0;
    /* 004500c6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004500c7 */
    if((f->flags&MI_ZF))goto mi_004500d8;
    /* 004500c9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004500ca */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004500d0 */
    f->r[0]=0x00000143u;
    /* 004500d5 */
    f->r[4]+=12;return;
mi_004500d8:;
    /* 004500d8 */
    f->r[0]=0x00000142u;
    /* 004500dd */
    f->r[4]+=12;return;
mi_004500e0:;
    /* 004500e0 */
    f->r[0]=0x0000001cu;
    /* 004500e5 */
    f->r[4]+=12;return;
mi_004500e8:;
    /* 004500e8 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004500eb */
    if((f->flags&MI_ZF))goto mi_00450107;
    /* 004500ed */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004500ee */
    if((f->flags&MI_ZF))goto mi_004500ff;
    /* 004500f0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004500f1 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004500f7 */
    f->r[0]=0x00000145u;
    /* 004500fc */
    f->r[4]+=12;return;
mi_004500ff:;
    /* 004500ff */
    f->r[0]=0x00000144u;
    /* 00450104 */
    f->r[4]+=12;return;
mi_00450107:;
    /* 00450107 */
    f->r[0]=0x0000001du;
    /* 0045010c */
    f->r[4]+=12;return;
mi_0045010f:;
    /* 0045010f */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450112 */
    if((f->flags&MI_ZF))goto mi_0045012e;
    /* 00450114 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450115 */
    if((f->flags&MI_ZF))goto mi_00450126;
    /* 00450117 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450118 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045011e */
    f->r[0]=0x00000147u;
    /* 00450123 */
    f->r[4]+=12;return;
mi_00450126:;
    /* 00450126 */
    f->r[0]=0x00000146u;
    /* 0045012b */
    f->r[4]+=12;return;
mi_0045012e:;
    /* 0045012e */
    f->r[0]=0x0000001eu;
    /* 00450133 */
    f->r[4]+=12;return;
mi_00450136:;
    /* 00450136 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450139 */
    if((f->flags&MI_ZF))goto mi_00450155;
    /* 0045013b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045013c */
    if((f->flags&MI_ZF))goto mi_0045014d;
    /* 0045013e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045013f */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450145 */
    f->r[0]=0x00000149u;
    /* 0045014a */
    f->r[4]+=12;return;
mi_0045014d:;
    /* 0045014d */
    f->r[0]=0x00000148u;
    /* 00450152 */
    f->r[4]+=12;return;
mi_00450155:;
    /* 00450155 */
    f->r[0]=0x0000001fu;
    /* 0045015a */
    f->r[4]+=12;return;
mi_0045015d:;
    /* 0045015d */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450160 */
    if((f->flags&MI_ZF))goto mi_0045017c;
    /* 00450162 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450163 */
    if((f->flags&MI_ZF))goto mi_00450174;
    /* 00450165 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450166 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045016c */
    f->r[0]=0x0000014bu;
    /* 00450171 */
    f->r[4]+=12;return;
mi_00450174:;
    /* 00450174 */
    f->r[0]=0x0000014au;
    /* 00450179 */
    f->r[4]+=12;return;
mi_0045017c:;
    /* 0045017c */
    f->r[0]=0x00000020u;
    /* 00450181 */
    f->r[4]+=12;return;
mi_00450184:;
    /* 00450184 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450187 */
    if((f->flags&MI_ZF))goto mi_004501a3;
    /* 00450189 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045018a */
    if((f->flags&MI_ZF))goto mi_0045019b;
    /* 0045018c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045018d */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450193 */
    f->r[0]=0x0000014du;
    /* 00450198 */
    f->r[4]+=12;return;
mi_0045019b:;
    /* 0045019b */
    f->r[0]=0x0000014cu;
    /* 004501a0 */
    f->r[4]+=12;return;
mi_004501a3:;
    /* 004501a3 */
    f->r[0]=0x00000021u;
    /* 004501a8 */
    f->r[4]+=12;return;
mi_004501ab:;
    /* 004501ab */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004501ae */
    if((f->flags&MI_ZF))goto mi_004501ca;
    /* 004501b0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004501b1 */
    if((f->flags&MI_ZF))goto mi_004501c2;
    /* 004501b3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004501b4 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004501ba */
    f->r[0]=0x0000014fu;
    /* 004501bf */
    f->r[4]+=12;return;
mi_004501c2:;
    /* 004501c2 */
    f->r[0]=0x0000014eu;
    /* 004501c7 */
    f->r[4]+=12;return;
mi_004501ca:;
    /* 004501ca */
    f->r[0]=0x00000022u;
    /* 004501cf */
    f->r[4]+=12;return;
mi_004501d2:;
    /* 004501d2 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004501d5 */
    if((f->flags&MI_ZF))goto mi_004501f1;
    /* 004501d7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004501d8 */
    if((f->flags&MI_ZF))goto mi_004501e9;
    /* 004501da */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004501db */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004501e1 */
    f->r[0]=0x00000151u;
    /* 004501e6 */
    f->r[4]+=12;return;
mi_004501e9:;
    /* 004501e9 */
    f->r[0]=0x00000150u;
    /* 004501ee */
    f->r[4]+=12;return;
mi_004501f1:;
    /* 004501f1 */
    f->r[0]=0x00000023u;
    /* 004501f6 */
    f->r[4]+=12;return;
mi_004501f9:;
    /* 004501f9 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004501fc */
    if((f->flags&MI_ZF))goto mi_00450218;
    /* 004501fe */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004501ff */
    if((f->flags&MI_ZF))goto mi_00450210;
    /* 00450201 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450202 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450208 */
    f->r[0]=0x00000153u;
    /* 0045020d */
    f->r[4]+=12;return;
mi_00450210:;
    /* 00450210 */
    f->r[0]=0x00000152u;
    /* 00450215 */
    f->r[4]+=12;return;
mi_00450218:;
    /* 00450218 */
    f->r[0]=0x0000002eu;
    /* 0045021d */
    f->r[4]+=12;return;
mi_00450220:;
    /* 00450220 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450223 */
    if((f->flags&MI_ZF))goto mi_0045023f;
    /* 00450225 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450226 */
    if((f->flags&MI_ZF))goto mi_00450237;
    /* 00450228 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450229 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045022f */
    f->r[0]=0x00000155u;
    /* 00450234 */
    f->r[4]+=12;return;
mi_00450237:;
    /* 00450237 */
    f->r[0]=0x00000154u;
    /* 0045023c */
    f->r[4]+=12;return;
mi_0045023f:;
    /* 0045023f */
    f->r[0]=0x0000002fu;
    /* 00450244 */
    f->r[4]+=12;return;
mi_00450247:;
    /* 00450247 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045024a */
    if((f->flags&MI_ZF))goto mi_00450266;
    /* 0045024c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045024d */
    if((f->flags&MI_ZF))goto mi_0045025e;
    /* 0045024f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450250 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450256 */
    f->r[0]=0x00000157u;
    /* 0045025b */
    f->r[4]+=12;return;
mi_0045025e:;
    /* 0045025e */
    f->r[0]=0x00000156u;
    /* 00450263 */
    f->r[4]+=12;return;
mi_00450266:;
    /* 00450266 */
    f->r[0]=0x00000030u;
    /* 0045026b */
    f->r[4]+=12;return;
mi_0045026e:;
    /* 0045026e */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450271 */
    if((f->flags&MI_ZF))goto mi_0045028d;
    /* 00450273 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450274 */
    if((f->flags&MI_ZF))goto mi_00450285;
    /* 00450276 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450277 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045027d */
    f->r[0]=0x00000159u;
    /* 00450282 */
    f->r[4]+=12;return;
mi_00450285:;
    /* 00450285 */
    f->r[0]=0x00000158u;
    /* 0045028a */
    f->r[4]+=12;return;
mi_0045028d:;
    /* 0045028d */
    f->r[0]=0x00000031u;
    /* 00450292 */
    f->r[4]+=12;return;
mi_00450295:;
    /* 00450295 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450298 */
    if((f->flags&MI_ZF))goto mi_004502b4;
    /* 0045029a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045029b */
    if((f->flags&MI_ZF))goto mi_004502ac;
    /* 0045029d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045029e */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004502a4 */
    f->r[0]=0x0000015bu;
    /* 004502a9 */
    f->r[4]+=12;return;
mi_004502ac:;
    /* 004502ac */
    f->r[0]=0x0000015au;
    /* 004502b1 */
    f->r[4]+=12;return;
mi_004502b4:;
    /* 004502b4 */
    f->r[0]=0x00000032u;
    /* 004502b9 */
    f->r[4]+=12;return;
mi_004502bc:;
    /* 004502bc */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004502bf */
    if((f->flags&MI_ZF))goto mi_004502db;
    /* 004502c1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004502c2 */
    if((f->flags&MI_ZF))goto mi_004502d3;
    /* 004502c4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004502c5 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004502cb */
    f->r[0]=0x0000015du;
    /* 004502d0 */
    f->r[4]+=12;return;
mi_004502d3:;
    /* 004502d3 */
    f->r[0]=0x0000015cu;
    /* 004502d8 */
    f->r[4]+=12;return;
mi_004502db:;
    /* 004502db */
    f->r[0]=0x00000033u;
    /* 004502e0 */
    f->r[4]+=12;return;
mi_004502e3:;
    /* 004502e3 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004502e6 */
    if((f->flags&MI_ZF))goto mi_00450302;
    /* 004502e8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004502e9 */
    if((f->flags&MI_ZF))goto mi_004502fa;
    /* 004502eb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004502ec */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004502f2 */
    f->r[0]=0x0000015fu;
    /* 004502f7 */
    f->r[4]+=12;return;
mi_004502fa:;
    /* 004502fa */
    f->r[0]=0x0000015eu;
    /* 004502ff */
    f->r[4]+=12;return;
mi_00450302:;
    /* 00450302 */
    f->r[0]=0x00000034u;
    /* 00450307 */
    f->r[4]+=12;return;
mi_0045030a:;
    /* 0045030a */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045030d */
    if((f->flags&MI_ZF))goto mi_00450329;
    /* 0045030f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450310 */
    if((f->flags&MI_ZF))goto mi_00450321;
    /* 00450312 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450313 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450319 */
    f->r[0]=0x00000161u;
    /* 0045031e */
    f->r[4]+=12;return;
mi_00450321:;
    /* 00450321 */
    f->r[0]=0x00000160u;
    /* 00450326 */
    f->r[4]+=12;return;
mi_00450329:;
    /* 00450329 */
    f->r[0]=0x00000035u;
    /* 0045032e */
    f->r[4]+=12;return;
mi_00450331:;
    /* 00450331 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450334 */
    if((f->flags&MI_ZF))goto mi_00450350;
    /* 00450336 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450337 */
    if((f->flags&MI_ZF))goto mi_00450348;
    /* 00450339 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045033a */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450340 */
    f->r[0]=0x00000163u;
    /* 00450345 */
    f->r[4]+=12;return;
mi_00450348:;
    /* 00450348 */
    f->r[0]=0x00000162u;
    /* 0045034d */
    f->r[4]+=12;return;
mi_00450350:;
    /* 00450350 */
    f->r[0]=0x00000040u;
    /* 00450355 */
    f->r[4]+=12;return;
mi_00450358:;
    /* 00450358 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045035b */
    if((f->flags&MI_ZF))goto mi_00450377;
    /* 0045035d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045035e */
    if((f->flags&MI_ZF))goto mi_0045036f;
    /* 00450360 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450361 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450367 */
    f->r[0]=0x00000165u;
    /* 0045036c */
    f->r[4]+=12;return;
mi_0045036f:;
    /* 0045036f */
    f->r[0]=0x00000164u;
    /* 00450374 */
    f->r[4]+=12;return;
mi_00450377:;
    /* 00450377 */
    f->r[0]=0x00000041u;
    /* 0045037c */
    f->r[4]+=12;return;
mi_0045037f:;
    /* 0045037f */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450382 */
    if((f->flags&MI_ZF))goto mi_0045039e;
    /* 00450384 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450385 */
    if((f->flags&MI_ZF))goto mi_00450396;
    /* 00450387 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450388 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045038e */
    f->r[0]=0x00000167u;
    /* 00450393 */
    f->r[4]+=12;return;
mi_00450396:;
    /* 00450396 */
    f->r[0]=0x00000166u;
    /* 0045039b */
    f->r[4]+=12;return;
mi_0045039e:;
    /* 0045039e */
    f->r[0]=0x00000042u;
    /* 004503a3 */
    f->r[4]+=12;return;
mi_004503a6:;
    /* 004503a6 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004503a9 */
    if((f->flags&MI_ZF))goto mi_004503c5;
    /* 004503ab */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004503ac */
    if((f->flags&MI_ZF))goto mi_004503bd;
    /* 004503ae */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004503af */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004503b5 */
    f->r[0]=0x00000169u;
    /* 004503ba */
    f->r[4]+=12;return;
mi_004503bd:;
    /* 004503bd */
    f->r[0]=0x00000168u;
    /* 004503c2 */
    f->r[4]+=12;return;
mi_004503c5:;
    /* 004503c5 */
    f->r[0]=0x00000043u;
    /* 004503ca */
    f->r[4]+=12;return;
mi_004503cd:;
    /* 004503cd */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004503d0 */
    if((f->flags&MI_ZF))goto mi_004503ec;
    /* 004503d2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004503d3 */
    if((f->flags&MI_ZF))goto mi_004503e4;
    /* 004503d5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004503d6 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004503dc */
    f->r[0]=0x0000016bu;
    /* 004503e1 */
    f->r[4]+=12;return;
mi_004503e4:;
    /* 004503e4 */
    f->r[0]=0x0000016au;
    /* 004503e9 */
    f->r[4]+=12;return;
mi_004503ec:;
    /* 004503ec */
    f->r[0]=0x00000044u;
    /* 004503f1 */
    f->r[4]+=12;return;
mi_004503f4:;
    /* 004503f4 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004503f7 */
    if((f->flags&MI_ZF))goto mi_00450413;
    /* 004503f9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004503fa */
    if((f->flags&MI_ZF))goto mi_0045040b;
    /* 004503fc */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004503fd */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450403 */
    f->r[0]=0x0000016du;
    /* 00450408 */
    f->r[4]+=12;return;
mi_0045040b:;
    /* 0045040b */
    f->r[0]=0x0000016cu;
    /* 00450410 */
    f->r[4]+=12;return;
mi_00450413:;
    /* 00450413 */
    f->r[0]=0x00000045u;
    /* 00450418 */
    f->r[4]+=12;return;
mi_0045041b:;
    /* 0045041b */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045041e */
    if((f->flags&MI_ZF))goto mi_0045043a;
    /* 00450420 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450421 */
    if((f->flags&MI_ZF))goto mi_00450432;
    /* 00450423 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450424 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045042a */
    f->r[0]=0x0000016fu;
    /* 0045042f */
    f->r[4]+=12;return;
mi_00450432:;
    /* 00450432 */
    f->r[0]=0x0000016eu;
    /* 00450437 */
    f->r[4]+=12;return;
mi_0045043a:;
    /* 0045043a */
    f->r[0]=0x00000046u;
    /* 0045043f */
    f->r[4]+=12;return;
mi_00450442:;
    /* 00450442 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450445 */
    if((f->flags&MI_ZF))goto mi_00450461;
    /* 00450447 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450448 */
    if((f->flags&MI_ZF))goto mi_00450459;
    /* 0045044a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045044b */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450451 */
    f->r[0]=0x00000171u;
    /* 00450456 */
    f->r[4]+=12;return;
mi_00450459:;
    /* 00450459 */
    f->r[0]=0x00000170u;
    /* 0045045e */
    f->r[4]+=12;return;
mi_00450461:;
    /* 00450461 */
    f->r[0]=0x00000047u;
    /* 00450466 */
    f->r[4]+=12;return;
mi_00450469:;
    /* 00450469 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045046c */
    if((f->flags&MI_ZF))goto mi_00450488;
    /* 0045046e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045046f */
    if((f->flags&MI_ZF))goto mi_00450480;
    /* 00450471 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450472 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450478 */
    f->r[0]=0x00000173u;
    /* 0045047d */
    f->r[4]+=12;return;
mi_00450480:;
    /* 00450480 */
    f->r[0]=0x00000172u;
    /* 00450485 */
    f->r[4]+=12;return;
mi_00450488:;
    /* 00450488 */
    f->r[0]=0x00000052u;
    /* 0045048d */
    f->r[4]+=12;return;
mi_00450490:;
    /* 00450490 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450493 */
    if((f->flags&MI_ZF))goto mi_004504af;
    /* 00450495 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450496 */
    if((f->flags&MI_ZF))goto mi_004504a7;
    /* 00450498 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450499 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045049f */
    f->r[0]=0x00000175u;
    /* 004504a4 */
    f->r[4]+=12;return;
mi_004504a7:;
    /* 004504a7 */
    f->r[0]=0x00000174u;
    /* 004504ac */
    f->r[4]+=12;return;
mi_004504af:;
    /* 004504af */
    f->r[0]=0x00000053u;
    /* 004504b4 */
    f->r[4]+=12;return;
mi_004504b7:;
    /* 004504b7 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004504ba */
    if((f->flags&MI_ZF))goto mi_004504d6;
    /* 004504bc */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004504bd */
    if((f->flags&MI_ZF))goto mi_004504ce;
    /* 004504bf */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004504c0 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004504c6 */
    f->r[0]=0x00000177u;
    /* 004504cb */
    f->r[4]+=12;return;
mi_004504ce:;
    /* 004504ce */
    f->r[0]=0x00000176u;
    /* 004504d3 */
    f->r[4]+=12;return;
mi_004504d6:;
    /* 004504d6 */
    f->r[0]=0x00000054u;
    /* 004504db */
    f->r[4]+=12;return;
mi_004504de:;
    /* 004504de */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004504e1 */
    if((f->flags&MI_ZF))goto mi_004504fd;
    /* 004504e3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004504e4 */
    if((f->flags&MI_ZF))goto mi_004504f5;
    /* 004504e6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004504e7 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004504ed */
    f->r[0]=0x00000179u;
    /* 004504f2 */
    f->r[4]+=12;return;
mi_004504f5:;
    /* 004504f5 */
    f->r[0]=0x00000178u;
    /* 004504fa */
    f->r[4]+=12;return;
mi_004504fd:;
    /* 004504fd */
    f->r[0]=0x00000055u;
    /* 00450502 */
    f->r[4]+=12;return;
mi_00450505:;
    /* 00450505 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450508 */
    if((f->flags&MI_ZF))goto mi_00450524;
    /* 0045050a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045050b */
    if((f->flags&MI_ZF))goto mi_0045051c;
    /* 0045050d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045050e */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450514 */
    f->r[0]=0x0000017bu;
    /* 00450519 */
    f->r[4]+=12;return;
mi_0045051c:;
    /* 0045051c */
    f->r[0]=0x0000017au;
    /* 00450521 */
    f->r[4]+=12;return;
mi_00450524:;
    /* 00450524 */
    f->r[0]=0x00000056u;
    /* 00450529 */
    f->r[4]+=12;return;
mi_0045052c:;
    /* 0045052c */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045052f */
    if((f->flags&MI_ZF))goto mi_0045054b;
    /* 00450531 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450532 */
    if((f->flags&MI_ZF))goto mi_00450543;
    /* 00450534 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450535 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045053b */
    f->r[0]=0x0000017du;
    /* 00450540 */
    f->r[4]+=12;return;
mi_00450543:;
    /* 00450543 */
    f->r[0]=0x0000017cu;
    /* 00450548 */
    f->r[4]+=12;return;
mi_0045054b:;
    /* 0045054b */
    f->r[0]=0x00000057u;
    /* 00450550 */
    f->r[4]+=12;return;
mi_00450553:;
    /* 00450553 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450556 */
    if((f->flags&MI_ZF))goto mi_00450572;
    /* 00450558 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450559 */
    if((f->flags&MI_ZF))goto mi_0045056a;
    /* 0045055b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045055c */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450562 */
    f->r[0]=0x0000017fu;
    /* 00450567 */
    f->r[4]+=12;return;
mi_0045056a:;
    /* 0045056a */
    f->r[0]=0x0000017eu;
    /* 0045056f */
    f->r[4]+=12;return;
mi_00450572:;
    /* 00450572 */
    f->r[0]=0x00000058u;
    /* 00450577 */
    f->r[4]+=12;return;
mi_0045057a:;
    /* 0045057a */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045057d */
    if((f->flags&MI_ZF))goto mi_00450599;
    /* 0045057f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450580 */
    if((f->flags&MI_ZF))goto mi_00450591;
    /* 00450582 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450583 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450589 */
    f->r[0]=0x00000181u;
    /* 0045058e */
    f->r[4]+=12;return;
mi_00450591:;
    /* 00450591 */
    f->r[0]=0x00000180u;
    /* 00450596 */
    f->r[4]+=12;return;
mi_00450599:;
    /* 00450599 */
    f->r[0]=0x00000059u;
    /* 0045059e */
    f->r[4]+=12;return;
mi_004505a1:;
    /* 004505a1 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004505a4 */
    if((f->flags&MI_ZF))goto mi_004505c0;
    /* 004505a6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004505a7 */
    if((f->flags&MI_ZF))goto mi_004505b8;
    /* 004505a9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004505aa */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004505b0 */
    f->r[0]=0x00000183u;
    /* 004505b5 */
    f->r[4]+=12;return;
mi_004505b8:;
    /* 004505b8 */
    f->r[0]=0x00000182u;
    /* 004505bd */
    f->r[4]+=12;return;
mi_004505c0:;
    /* 004505c0 */
    f->r[0]=0x00000064u;
    /* 004505c5 */
    f->r[4]+=12;return;
mi_004505c8:;
    /* 004505c8 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004505cb */
    if((f->flags&MI_ZF))goto mi_004505e7;
    /* 004505cd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004505ce */
    if((f->flags&MI_ZF))goto mi_004505df;
    /* 004505d0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004505d1 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004505d7 */
    f->r[0]=0x00000185u;
    /* 004505dc */
    f->r[4]+=12;return;
mi_004505df:;
    /* 004505df */
    f->r[0]=0x00000184u;
    /* 004505e4 */
    f->r[4]+=12;return;
mi_004505e7:;
    /* 004505e7 */
    f->r[0]=0x00000065u;
    /* 004505ec */
    f->r[4]+=12;return;
mi_004505ef:;
    /* 004505ef */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004505f2 */
    if((f->flags&MI_ZF))goto mi_0045060e;
    /* 004505f4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004505f5 */
    if((f->flags&MI_ZF))goto mi_00450606;
    /* 004505f7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004505f8 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004505fe */
    f->r[0]=0x00000187u;
    /* 00450603 */
    f->r[4]+=12;return;
mi_00450606:;
    /* 00450606 */
    f->r[0]=0x00000186u;
    /* 0045060b */
    f->r[4]+=12;return;
mi_0045060e:;
    /* 0045060e */
    f->r[0]=0x00000066u;
    /* 00450613 */
    f->r[4]+=12;return;
mi_00450616:;
    /* 00450616 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450619 */
    if((f->flags&MI_ZF))goto mi_00450635;
    /* 0045061b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045061c */
    if((f->flags&MI_ZF))goto mi_0045062d;
    /* 0045061e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045061f */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450625 */
    f->r[0]=0x00000189u;
    /* 0045062a */
    f->r[4]+=12;return;
mi_0045062d:;
    /* 0045062d */
    f->r[0]=0x00000188u;
    /* 00450632 */
    f->r[4]+=12;return;
mi_00450635:;
    /* 00450635 */
    f->r[0]=0x00000067u;
    /* 0045063a */
    f->r[4]+=12;return;
mi_0045063d:;
    /* 0045063d */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450640 */
    if((f->flags&MI_ZF))goto mi_0045065c;
    /* 00450642 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450643 */
    if((f->flags&MI_ZF))goto mi_00450654;
    /* 00450645 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450646 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045064c */
    f->r[0]=0x0000018bu;
    /* 00450651 */
    f->r[4]+=12;return;
mi_00450654:;
    /* 00450654 */
    f->r[0]=0x0000018au;
    /* 00450659 */
    f->r[4]+=12;return;
mi_0045065c:;
    /* 0045065c */
    f->r[0]=0x00000068u;
    /* 00450661 */
    f->r[4]+=12;return;
mi_00450664:;
    /* 00450664 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450667 */
    if((f->flags&MI_ZF))goto mi_00450683;
    /* 00450669 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045066a */
    if((f->flags&MI_ZF))goto mi_0045067b;
    /* 0045066c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045066d */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450673 */
    f->r[0]=0x0000018du;
    /* 00450678 */
    f->r[4]+=12;return;
mi_0045067b:;
    /* 0045067b */
    f->r[0]=0x0000018cu;
    /* 00450680 */
    f->r[4]+=12;return;
mi_00450683:;
    /* 00450683 */
    f->r[0]=0x00000069u;
    /* 00450688 */
    f->r[4]+=12;return;
mi_0045068b:;
    /* 0045068b */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045068e */
    if((f->flags&MI_ZF))goto mi_004506aa;
    /* 00450690 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450691 */
    if((f->flags&MI_ZF))goto mi_004506a2;
    /* 00450693 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450694 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045069a */
    f->r[0]=0x0000018fu;
    /* 0045069f */
    f->r[4]+=12;return;
mi_004506a2:;
    /* 004506a2 */
    f->r[0]=0x0000018eu;
    /* 004506a7 */
    f->r[4]+=12;return;
mi_004506aa:;
    /* 004506aa */
    f->r[0]=0x0000006au;
    /* 004506af */
    f->r[4]+=12;return;
mi_004506b2:;
    /* 004506b2 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004506b5 */
    if((f->flags&MI_ZF))goto mi_004506d1;
    /* 004506b7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004506b8 */
    if((f->flags&MI_ZF))goto mi_004506c9;
    /* 004506ba */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004506bb */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004506c1 */
    f->r[0]=0x00000191u;
    /* 004506c6 */
    f->r[4]+=12;return;
mi_004506c9:;
    /* 004506c9 */
    f->r[0]=0x00000190u;
    /* 004506ce */
    f->r[4]+=12;return;
mi_004506d1:;
    /* 004506d1 */
    f->r[0]=0x0000006bu;
    /* 004506d6 */
    f->r[4]+=12;return;
mi_004506d9:;
    /* 004506d9 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004506dc */
    if((f->flags&MI_ZF))goto mi_004506f8;
    /* 004506de */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004506df */
    if((f->flags&MI_ZF))goto mi_004506f0;
    /* 004506e1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004506e2 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004506e8 */
    f->r[0]=0x00000193u;
    /* 004506ed */
    f->r[4]+=12;return;
mi_004506f0:;
    /* 004506f0 */
    f->r[0]=0x00000192u;
    /* 004506f5 */
    f->r[4]+=12;return;
mi_004506f8:;
    /* 004506f8 */
    f->r[0]=0x00000076u;
    /* 004506fd */
    f->r[4]+=12;return;
mi_00450700:;
    /* 00450700 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450703 */
    if((f->flags&MI_ZF))goto mi_0045071f;
    /* 00450705 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450706 */
    if((f->flags&MI_ZF))goto mi_00450717;
    /* 00450708 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450709 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045070f */
    f->r[0]=0x00000195u;
    /* 00450714 */
    f->r[4]+=12;return;
mi_00450717:;
    /* 00450717 */
    f->r[0]=0x00000194u;
    /* 0045071c */
    f->r[4]+=12;return;
mi_0045071f:;
    /* 0045071f */
    f->r[0]=0x00000077u;
    /* 00450724 */
    f->r[4]+=12;return;
mi_00450727:;
    /* 00450727 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045072a */
    if((f->flags&MI_ZF))goto mi_00450746;
    /* 0045072c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045072d */
    if((f->flags&MI_ZF))goto mi_0045073e;
    /* 0045072f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450730 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450736 */
    f->r[0]=0x00000197u;
    /* 0045073b */
    f->r[4]+=12;return;
mi_0045073e:;
    /* 0045073e */
    f->r[0]=0x00000196u;
    /* 00450743 */
    f->r[4]+=12;return;
mi_00450746:;
    /* 00450746 */
    f->r[0]=0x00000078u;
    /* 0045074b */
    f->r[4]+=12;return;
mi_0045074e:;
    /* 0045074e */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450751 */
    if((f->flags&MI_ZF))goto mi_0045076d;
    /* 00450753 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450754 */
    if((f->flags&MI_ZF))goto mi_00450765;
    /* 00450756 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450757 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045075d */
    f->r[0]=0x00000199u;
    /* 00450762 */
    f->r[4]+=12;return;
mi_00450765:;
    /* 00450765 */
    f->r[0]=0x00000198u;
    /* 0045076a */
    f->r[4]+=12;return;
mi_0045076d:;
    /* 0045076d */
    f->r[0]=0x00000079u;
    /* 00450772 */
    f->r[4]+=12;return;
mi_00450775:;
    /* 00450775 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450778 */
    if((f->flags&MI_ZF))goto mi_00450794;
    /* 0045077a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045077b */
    if((f->flags&MI_ZF))goto mi_0045078c;
    /* 0045077d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045077e */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450784 */
    f->r[0]=0x0000019bu;
    /* 00450789 */
    f->r[4]+=12;return;
mi_0045078c:;
    /* 0045078c */
    f->r[0]=0x0000019au;
    /* 00450791 */
    f->r[4]+=12;return;
mi_00450794:;
    /* 00450794 */
    f->r[0]=0x0000007au;
    /* 00450799 */
    f->r[4]+=12;return;
mi_0045079c:;
    /* 0045079c */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045079f */
    if((f->flags&MI_ZF))goto mi_004507bb;
    /* 004507a1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004507a2 */
    if((f->flags&MI_ZF))goto mi_004507b3;
    /* 004507a4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004507a5 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004507ab */
    f->r[0]=0x0000019du;
    /* 004507b0 */
    f->r[4]+=12;return;
mi_004507b3:;
    /* 004507b3 */
    f->r[0]=0x0000019cu;
    /* 004507b8 */
    f->r[4]+=12;return;
mi_004507bb:;
    /* 004507bb */
    f->r[0]=0x0000007bu;
    /* 004507c0 */
    f->r[4]+=12;return;
mi_004507c3:;
    /* 004507c3 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004507c6 */
    if((f->flags&MI_ZF))goto mi_004507e2;
    /* 004507c8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004507c9 */
    if((f->flags&MI_ZF))goto mi_004507da;
    /* 004507cb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004507cc */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004507d2 */
    f->r[0]=0x0000019fu;
    /* 004507d7 */
    f->r[4]+=12;return;
mi_004507da:;
    /* 004507da */
    f->r[0]=0x0000019eu;
    /* 004507df */
    f->r[4]+=12;return;
mi_004507e2:;
    /* 004507e2 */
    f->r[0]=0x0000007cu;
    /* 004507e7 */
    f->r[4]+=12;return;
mi_004507ea:;
    /* 004507ea */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004507ed */
    if((f->flags&MI_ZF))goto mi_00450809;
    /* 004507ef */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004507f0 */
    if((f->flags&MI_ZF))goto mi_00450801;
    /* 004507f2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004507f3 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004507f9 */
    f->r[0]=0x000001a1u;
    /* 004507fe */
    f->r[4]+=12;return;
mi_00450801:;
    /* 00450801 */
    f->r[0]=0x000001a0u;
    /* 00450806 */
    f->r[4]+=12;return;
mi_00450809:;
    /* 00450809 */
    f->r[0]=0x0000007du;
    /* 0045080e */
    f->r[4]+=12;return;
mi_00450811:;
    /* 00450811 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450814 */
    if((f->flags&MI_ZF))goto mi_00450830;
    /* 00450816 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450817 */
    if((f->flags&MI_ZF))goto mi_00450828;
    /* 00450819 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045081a */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450820 */
    f->r[0]=0x000001a3u;
    /* 00450825 */
    f->r[4]+=12;return;
mi_00450828:;
    /* 00450828 */
    f->r[0]=0x000001a2u;
    /* 0045082d */
    f->r[4]+=12;return;
mi_00450830:;
    /* 00450830 */
    f->r[0]=0x00000080u;
    /* 00450835 */
    f->r[4]+=12;return;
mi_00450838:;
    /* 00450838 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045083b */
    if((f->flags&MI_ZF))goto mi_00450857;
    /* 0045083d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045083e */
    if((f->flags&MI_ZF))goto mi_0045084f;
    /* 00450840 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450841 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450847 */
    f->r[0]=0x000001a5u;
    /* 0045084c */
    f->r[4]+=12;return;
mi_0045084f:;
    /* 0045084f */
    f->r[0]=0x000001a4u;
    /* 00450854 */
    f->r[4]+=12;return;
mi_00450857:;
    /* 00450857 */
    f->r[0]=0x00000081u;
    /* 0045085c */
    f->r[4]+=12;return;
mi_0045085f:;
    /* 0045085f */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450862 */
    if((f->flags&MI_ZF))goto mi_0045087e;
    /* 00450864 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450865 */
    if((f->flags&MI_ZF))goto mi_00450876;
    /* 00450867 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450868 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045086e */
    f->r[0]=0x000001a7u;
    /* 00450873 */
    f->r[4]+=12;return;
mi_00450876:;
    /* 00450876 */
    f->r[0]=0x000001a6u;
    /* 0045087b */
    f->r[4]+=12;return;
mi_0045087e:;
    /* 0045087e */
    f->r[0]=0x00000084u;
    /* 00450883 */
    f->r[4]+=12;return;
mi_00450886:;
    /* 00450886 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450889 */
    if((f->flags&MI_ZF))goto mi_004508a5;
    /* 0045088b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045088c */
    if((f->flags&MI_ZF))goto mi_0045089d;
    /* 0045088e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045088f */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450895 */
    f->r[0]=0x000001a9u;
    /* 0045089a */
    f->r[4]+=12;return;
mi_0045089d:;
    /* 0045089d */
    f->r[0]=0x000001a8u;
    /* 004508a2 */
    f->r[4]+=12;return;
mi_004508a5:;
    /* 004508a5 */
    f->r[0]=0x00000085u;
    /* 004508aa */
    f->r[4]+=12;return;
mi_004508ad:;
    /* 004508ad */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004508b0 */
    if((f->flags&MI_ZF))goto mi_004508cc;
    /* 004508b2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004508b3 */
    if((f->flags&MI_ZF))goto mi_004508c4;
    /* 004508b5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004508b6 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004508bc */
    f->r[0]=0x000001abu;
    /* 004508c1 */
    f->r[4]+=12;return;
mi_004508c4:;
    /* 004508c4 */
    f->r[0]=0x000001aau;
    /* 004508c9 */
    f->r[4]+=12;return;
mi_004508cc:;
    /* 004508cc */
    f->r[0]=0x00000088u;
    /* 004508d1 */
    f->r[4]+=12;return;
mi_004508d4:;
    /* 004508d4 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004508d7 */
    if((f->flags&MI_ZF))goto mi_004508f3;
    /* 004508d9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004508da */
    if((f->flags&MI_ZF))goto mi_004508eb;
    /* 004508dc */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004508dd */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004508e3 */
    f->r[0]=0x000001adu;
    /* 004508e8 */
    f->r[4]+=12;return;
mi_004508eb:;
    /* 004508eb */
    f->r[0]=0x000001acu;
    /* 004508f0 */
    f->r[4]+=12;return;
mi_004508f3:;
    /* 004508f3 */
    f->r[0]=0x00000089u;
    /* 004508f8 */
    f->r[4]+=12;return;
mi_004508fb:;
    /* 004508fb */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004508fe */
    if((f->flags&MI_ZF))goto mi_0045091a;
    /* 00450900 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450901 */
    if((f->flags&MI_ZF))goto mi_00450912;
    /* 00450903 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450904 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045090a */
    f->r[0]=0x000001afu;
    /* 0045090f */
    f->r[4]+=12;return;
mi_00450912:;
    /* 00450912 */
    f->r[0]=0x000001aeu;
    /* 00450917 */
    f->r[4]+=12;return;
mi_0045091a:;
    /* 0045091a */
    f->r[0]=0x0000008cu;
    /* 0045091f */
    f->r[4]+=12;return;
mi_00450922:;
    /* 00450922 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450925 */
    if((f->flags&MI_ZF))goto mi_00450941;
    /* 00450927 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450928 */
    if((f->flags&MI_ZF))goto mi_00450939;
    /* 0045092a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045092b */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450931 */
    f->r[0]=0x000001b1u;
    /* 00450936 */
    f->r[4]+=12;return;
mi_00450939:;
    /* 00450939 */
    f->r[0]=0x000001b0u;
    /* 0045093e */
    f->r[4]+=12;return;
mi_00450941:;
    /* 00450941 */
    f->r[0]=0x0000008du;
    /* 00450946 */
    f->r[4]+=12;return;
mi_00450949:;
    /* 00450949 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045094c */
    if((f->flags&MI_ZF))goto mi_00450968;
    /* 0045094e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045094f */
    if((f->flags&MI_ZF))goto mi_00450960;
    /* 00450951 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450952 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450958 */
    f->r[0]=0x000001b3u;
    /* 0045095d */
    f->r[4]+=12;return;
mi_00450960:;
    /* 00450960 */
    f->r[0]=0x000001b2u;
    /* 00450965 */
    f->r[4]+=12;return;
mi_00450968:;
    /* 00450968 */
    f->r[0]=0x00000090u;
    /* 0045096d */
    f->r[4]+=12;return;
mi_00450970:;
    /* 00450970 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450973 */
    if((f->flags&MI_ZF))goto mi_0045098f;
    /* 00450975 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450976 */
    if((f->flags&MI_ZF))goto mi_00450987;
    /* 00450978 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450979 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045097f */
    f->r[0]=0x000001b5u;
    /* 00450984 */
    f->r[4]+=12;return;
mi_00450987:;
    /* 00450987 */
    f->r[0]=0x000001b4u;
    /* 0045098c */
    f->r[4]+=12;return;
mi_0045098f:;
    /* 0045098f */
    f->r[0]=0x00000091u;
    /* 00450994 */
    f->r[4]+=12;return;
mi_00450997:;
    /* 00450997 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045099a */
    if((f->flags&MI_ZF))goto mi_004509b6;
    /* 0045099c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045099d */
    if((f->flags&MI_ZF))goto mi_004509ae;
    /* 0045099f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004509a0 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004509a6 */
    f->r[0]=0x000001b7u;
    /* 004509ab */
    f->r[4]+=12;return;
mi_004509ae:;
    /* 004509ae */
    f->r[0]=0x000001b6u;
    /* 004509b3 */
    f->r[4]+=12;return;
mi_004509b6:;
    /* 004509b6 */
    f->r[0]=0x00000094u;
    /* 004509bb */
    f->r[4]+=12;return;
mi_004509be:;
    /* 004509be */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004509c1 */
    if((f->flags&MI_ZF))goto mi_004509dd;
    /* 004509c3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004509c4 */
    if((f->flags&MI_ZF))goto mi_004509d5;
    /* 004509c6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004509c7 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004509cd */
    f->r[0]=0x000001b9u;
    /* 004509d2 */
    f->r[4]+=12;return;
mi_004509d5:;
    /* 004509d5 */
    f->r[0]=0x000001b8u;
    /* 004509da */
    f->r[4]+=12;return;
mi_004509dd:;
    /* 004509dd */
    f->r[0]=0x00000095u;
    /* 004509e2 */
    f->r[4]+=12;return;
mi_004509e5:;
    /* 004509e5 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004509e8 */
    if((f->flags&MI_ZF))goto mi_00450a04;
    /* 004509ea */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004509eb */
    if((f->flags&MI_ZF))goto mi_004509fc;
    /* 004509ed */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004509ee */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004509f4 */
    f->r[0]=0x000001bbu;
    /* 004509f9 */
    f->r[4]+=12;return;
mi_004509fc:;
    /* 004509fc */
    f->r[0]=0x000001bau;
    /* 00450a01 */
    f->r[4]+=12;return;
mi_00450a04:;
    /* 00450a04 */
    f->r[0]=0x00000098u;
    /* 00450a09 */
    f->r[4]+=12;return;
mi_00450a0c:;
    /* 00450a0c */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450a0f */
    if((f->flags&MI_ZF))goto mi_00450a2b;
    /* 00450a11 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a12 */
    if((f->flags&MI_ZF))goto mi_00450a23;
    /* 00450a14 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a15 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450a1b */
    f->r[0]=0x000001bdu;
    /* 00450a20 */
    f->r[4]+=12;return;
mi_00450a23:;
    /* 00450a23 */
    f->r[0]=0x000001bcu;
    /* 00450a28 */
    f->r[4]+=12;return;
mi_00450a2b:;
    /* 00450a2b */
    f->r[0]=0x00000099u;
    /* 00450a30 */
    f->r[4]+=12;return;
mi_00450a33:;
    /* 00450a33 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450a36 */
    if((f->flags&MI_ZF))goto mi_00450a52;
    /* 00450a38 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a39 */
    if((f->flags&MI_ZF))goto mi_00450a4a;
    /* 00450a3b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a3c */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450a42 */
    f->r[0]=0x000001bfu;
    /* 00450a47 */
    f->r[4]+=12;return;
mi_00450a4a:;
    /* 00450a4a */
    f->r[0]=0x000001beu;
    /* 00450a4f */
    f->r[4]+=12;return;
mi_00450a52:;
    /* 00450a52 */
    f->r[0]=0x0000009cu;
    /* 00450a57 */
    f->r[4]+=12;return;
mi_00450a5a:;
    /* 00450a5a */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450a5d */
    if((f->flags&MI_ZF))goto mi_00450a79;
    /* 00450a5f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a60 */
    if((f->flags&MI_ZF))goto mi_00450a71;
    /* 00450a62 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a63 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450a69 */
    f->r[0]=0x000001c1u;
    /* 00450a6e */
    f->r[4]+=12;return;
mi_00450a71:;
    /* 00450a71 */
    f->r[0]=0x000001c0u;
    /* 00450a76 */
    f->r[4]+=12;return;
mi_00450a79:;
    /* 00450a79 */
    f->r[0]=0x0000009du;
    /* 00450a7e */
    f->r[4]+=12;return;
mi_00450a81:;
    /* 00450a81 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450a84 */
    if((f->flags&MI_ZF))goto mi_00450aa0;
    /* 00450a86 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a87 */
    if((f->flags&MI_ZF))goto mi_00450a98;
    /* 00450a89 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450a8a */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450a90 */
    f->r[0]=0x000001c3u;
    /* 00450a95 */
    f->r[4]+=12;return;
mi_00450a98:;
    /* 00450a98 */
    f->r[0]=0x000001c2u;
    /* 00450a9d */
    f->r[4]+=12;return;
mi_00450aa0:;
    /* 00450aa0 */
    f->r[0]=0x000000a0u;
    /* 00450aa5 */
    f->r[4]+=12;return;
mi_00450aa8:;
    /* 00450aa8 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450aab */
    if((f->flags&MI_ZF))goto mi_00450ac7;
    /* 00450aad */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450aae */
    if((f->flags&MI_ZF))goto mi_00450abf;
    /* 00450ab0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ab1 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450ab7 */
    f->r[0]=0x000001c5u;
    /* 00450abc */
    f->r[4]+=12;return;
mi_00450abf:;
    /* 00450abf */
    f->r[0]=0x000001c4u;
    /* 00450ac4 */
    f->r[4]+=12;return;
mi_00450ac7:;
    /* 00450ac7 */
    f->r[0]=0x000000a1u;
    /* 00450acc */
    f->r[4]+=12;return;
mi_00450acf:;
    /* 00450acf */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450ad2 */
    if((f->flags&MI_ZF))goto mi_00450aee;
    /* 00450ad4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ad5 */
    if((f->flags&MI_ZF))goto mi_00450ae6;
    /* 00450ad7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ad8 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450ade */
    f->r[0]=0x000001c7u;
    /* 00450ae3 */
    f->r[4]+=12;return;
mi_00450ae6:;
    /* 00450ae6 */
    f->r[0]=0x000001c6u;
    /* 00450aeb */
    f->r[4]+=12;return;
mi_00450aee:;
    /* 00450aee */
    f->r[0]=0x000000a9u;
    /* 00450af3 */
    f->r[4]+=12;return;
mi_00450af6:;
    /* 00450af6 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450af9 */
    if((f->flags&MI_ZF))goto mi_00450b15;
    /* 00450afb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450afc */
    if((f->flags&MI_ZF))goto mi_00450b0d;
    /* 00450afe */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450aff */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450b05 */
    f->r[0]=0x000001c9u;
    /* 00450b0a */
    f->r[4]+=12;return;
mi_00450b0d:;
    /* 00450b0d */
    f->r[0]=0x000001c8u;
    /* 00450b12 */
    f->r[4]+=12;return;
mi_00450b15:;
    /* 00450b15 */
    f->r[0]=0x000000aau;
    /* 00450b1a */
    f->r[4]+=12;return;
mi_00450b1d:;
    /* 00450b1d */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450b20 */
    if((f->flags&MI_ZF))goto mi_00450b3c;
    /* 00450b22 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b23 */
    if((f->flags&MI_ZF))goto mi_00450b34;
    /* 00450b25 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b26 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450b2c */
    f->r[0]=0x000001cbu;
    /* 00450b31 */
    f->r[4]+=12;return;
mi_00450b34:;
    /* 00450b34 */
    f->r[0]=0x000001cau;
    /* 00450b39 */
    f->r[4]+=12;return;
mi_00450b3c:;
    /* 00450b3c */
    f->r[0]=0x000000b3u;
    /* 00450b41 */
    f->r[4]+=12;return;
mi_00450b44:;
    /* 00450b44 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450b47 */
    if((f->flags&MI_ZF))goto mi_00450b63;
    /* 00450b49 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b4a */
    if((f->flags&MI_ZF))goto mi_00450b5b;
    /* 00450b4c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b4d */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450b53 */
    f->r[0]=0x000001cdu;
    /* 00450b58 */
    f->r[4]+=12;return;
mi_00450b5b:;
    /* 00450b5b */
    f->r[0]=0x000001ccu;
    /* 00450b60 */
    f->r[4]+=12;return;
mi_00450b63:;
    /* 00450b63 */
    f->r[0]=0x000000b4u;
    /* 00450b68 */
    f->r[4]+=12;return;
mi_00450b6b:;
    /* 00450b6b */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450b6e */
    if((f->flags&MI_ZF))goto mi_00450b8a;
    /* 00450b70 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b71 */
    if((f->flags&MI_ZF))goto mi_00450b82;
    /* 00450b73 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b74 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450b7a */
    f->r[0]=0x000001cfu;
    /* 00450b7f */
    f->r[4]+=12;return;
mi_00450b82:;
    /* 00450b82 */
    f->r[0]=0x000001ceu;
    /* 00450b87 */
    f->r[4]+=12;return;
mi_00450b8a:;
    /* 00450b8a */
    f->r[0]=0x000000bdu;
    /* 00450b8f */
    f->r[4]+=12;return;
mi_00450b92:;
    /* 00450b92 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450b95 */
    if((f->flags&MI_ZF))goto mi_00450bb1;
    /* 00450b97 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b98 */
    if((f->flags&MI_ZF))goto mi_00450ba9;
    /* 00450b9a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450b9b */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450ba1 */
    f->r[0]=0x000001d1u;
    /* 00450ba6 */
    f->r[4]+=12;return;
mi_00450ba9:;
    /* 00450ba9 */
    f->r[0]=0x000001d0u;
    /* 00450bae */
    f->r[4]+=12;return;
mi_00450bb1:;
    /* 00450bb1 */
    f->r[0]=0x000000beu;
    /* 00450bb6 */
    f->r[4]+=12;return;
mi_00450bb9:;
    /* 00450bb9 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450bbc */
    if((f->flags&MI_ZF))goto mi_00450bd8;
    /* 00450bbe */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450bbf */
    if((f->flags&MI_ZF))goto mi_00450bd0;
    /* 00450bc1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450bc2 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450bc8 */
    f->r[0]=0x000001d3u;
    /* 00450bcd */
    f->r[4]+=12;return;
mi_00450bd0:;
    /* 00450bd0 */
    f->r[0]=0x000001d2u;
    /* 00450bd5 */
    f->r[4]+=12;return;
mi_00450bd8:;
    /* 00450bd8 */
    f->r[0]=0x000000c1u;
    /* 00450bdd */
    f->r[4]+=12;return;
mi_00450be0:;
    /* 00450be0 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450be3 */
    if((f->flags&MI_ZF))goto mi_00450bff;
    /* 00450be5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450be6 */
    if((f->flags&MI_ZF))goto mi_00450bf7;
    /* 00450be8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450be9 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450bef */
    f->r[0]=0x000001d5u;
    /* 00450bf4 */
    f->r[4]+=12;return;
mi_00450bf7:;
    /* 00450bf7 */
    f->r[0]=0x000001d4u;
    /* 00450bfc */
    f->r[4]+=12;return;
mi_00450bff:;
    /* 00450bff */
    f->r[0]=0x000000c2u;
    /* 00450c04 */
    f->r[4]+=12;return;
mi_00450c07:;
    /* 00450c07 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450c0a */
    if((f->flags&MI_ZF))goto mi_00450c26;
    /* 00450c0c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c0d */
    if((f->flags&MI_ZF))goto mi_00450c1e;
    /* 00450c0f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c10 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450c16 */
    f->r[0]=0x000001d7u;
    /* 00450c1b */
    f->r[4]+=12;return;
mi_00450c1e:;
    /* 00450c1e */
    f->r[0]=0x000001d6u;
    /* 00450c23 */
    f->r[4]+=12;return;
mi_00450c26:;
    /* 00450c26 */
    f->r[0]=0x000000c5u;
    /* 00450c2b */
    f->r[4]+=12;return;
mi_00450c2e:;
    /* 00450c2e */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450c31 */
    if((f->flags&MI_ZF))goto mi_00450c4d;
    /* 00450c33 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c34 */
    if((f->flags&MI_ZF))goto mi_00450c45;
    /* 00450c36 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c37 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450c3d */
    f->r[0]=0x000001d9u;
    /* 00450c42 */
    f->r[4]+=12;return;
mi_00450c45:;
    /* 00450c45 */
    f->r[0]=0x000001d8u;
    /* 00450c4a */
    f->r[4]+=12;return;
mi_00450c4d:;
    /* 00450c4d */
    f->r[0]=0x000000c6u;
    /* 00450c52 */
    f->r[4]+=12;return;
mi_00450c55:;
    /* 00450c55 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450c58 */
    if((f->flags&MI_ZF))goto mi_00450c74;
    /* 00450c5a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c5b */
    if((f->flags&MI_ZF))goto mi_00450c6c;
    /* 00450c5d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c5e */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450c64 */
    f->r[0]=0x000001dbu;
    /* 00450c69 */
    f->r[4]+=12;return;
mi_00450c6c:;
    /* 00450c6c */
    f->r[0]=0x000001dau;
    /* 00450c71 */
    f->r[4]+=12;return;
mi_00450c74:;
    /* 00450c74 */
    f->r[0]=0x000000c9u;
    /* 00450c79 */
    f->r[4]+=12;return;
mi_00450c7c:;
    /* 00450c7c */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450c7f */
    if((f->flags&MI_ZF))goto mi_00450c9b;
    /* 00450c81 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c82 */
    if((f->flags&MI_ZF))goto mi_00450c93;
    /* 00450c84 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450c85 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450c8b */
    f->r[0]=0x000001ddu;
    /* 00450c90 */
    f->r[4]+=12;return;
mi_00450c93:;
    /* 00450c93 */
    f->r[0]=0x000001dcu;
    /* 00450c98 */
    f->r[4]+=12;return;
mi_00450c9b:;
    /* 00450c9b */
    f->r[0]=0x000000cau;
    /* 00450ca0 */
    f->r[4]+=12;return;
mi_00450ca3:;
    /* 00450ca3 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450ca6 */
    if((f->flags&MI_ZF))goto mi_00450cc2;
    /* 00450ca8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ca9 */
    if((f->flags&MI_ZF))goto mi_00450cba;
    /* 00450cab */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450cac */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450cb2 */
    f->r[0]=0x000001dfu;
    /* 00450cb7 */
    f->r[4]+=12;return;
mi_00450cba:;
    /* 00450cba */
    f->r[0]=0x000001deu;
    /* 00450cbf */
    f->r[4]+=12;return;
mi_00450cc2:;
    /* 00450cc2 */
    f->r[0]=0x000000cdu;
    /* 00450cc7 */
    f->r[4]+=12;return;
mi_00450cca:;
    /* 00450cca */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450ccd */
    if((f->flags&MI_ZF))goto mi_00450ce9;
    /* 00450ccf */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450cd0 */
    if((f->flags&MI_ZF))goto mi_00450ce1;
    /* 00450cd2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450cd3 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450cd9 */
    f->r[0]=0x000001e1u;
    /* 00450cde */
    f->r[4]+=12;return;
mi_00450ce1:;
    /* 00450ce1 */
    f->r[0]=0x000001e0u;
    /* 00450ce6 */
    f->r[4]+=12;return;
mi_00450ce9:;
    /* 00450ce9 */
    f->r[0]=0x000000ceu;
    /* 00450cee */
    f->r[4]+=12;return;
mi_00450cf1:;
    /* 00450cf1 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450cf4 */
    if((f->flags&MI_ZF))goto mi_00450d10;
    /* 00450cf6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450cf7 */
    if((f->flags&MI_ZF))goto mi_00450d08;
    /* 00450cf9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450cfa */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450d00 */
    f->r[0]=0x000001e3u;
    /* 00450d05 */
    f->r[4]+=12;return;
mi_00450d08:;
    /* 00450d08 */
    f->r[0]=0x000001e2u;
    /* 00450d0d */
    f->r[4]+=12;return;
mi_00450d10:;
    /* 00450d10 */
    f->r[0]=0x000000d1u;
    /* 00450d15 */
    f->r[4]+=12;return;
mi_00450d18:;
    /* 00450d18 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450d1b */
    if((f->flags&MI_ZF))goto mi_00450d37;
    /* 00450d1d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d1e */
    if((f->flags&MI_ZF))goto mi_00450d2f;
    /* 00450d20 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d21 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450d27 */
    f->r[0]=0x000001e5u;
    /* 00450d2c */
    f->r[4]+=12;return;
mi_00450d2f:;
    /* 00450d2f */
    f->r[0]=0x000001e4u;
    /* 00450d34 */
    f->r[4]+=12;return;
mi_00450d37:;
    /* 00450d37 */
    f->r[0]=0x000000d2u;
    /* 00450d3c */
    f->r[4]+=12;return;
mi_00450d3f:;
    /* 00450d3f */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450d42 */
    if((f->flags&MI_ZF))goto mi_00450d5e;
    /* 00450d44 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d45 */
    if((f->flags&MI_ZF))goto mi_00450d56;
    /* 00450d47 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d48 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450d4e */
    f->r[0]=0x000001e7u;
    /* 00450d53 */
    f->r[4]+=12;return;
mi_00450d56:;
    /* 00450d56 */
    f->r[0]=0x000001e6u;
    /* 00450d5b */
    f->r[4]+=12;return;
mi_00450d5e:;
    /* 00450d5e */
    f->r[0]=0x000000d5u;
    /* 00450d63 */
    f->r[4]+=12;return;
mi_00450d66:;
    /* 00450d66 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450d69 */
    if((f->flags&MI_ZF))goto mi_00450d85;
    /* 00450d6b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d6c */
    if((f->flags&MI_ZF))goto mi_00450d7d;
    /* 00450d6e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d6f */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450d75 */
    f->r[0]=0x000001e9u;
    /* 00450d7a */
    f->r[4]+=12;return;
mi_00450d7d:;
    /* 00450d7d */
    f->r[0]=0x000001e8u;
    /* 00450d82 */
    f->r[4]+=12;return;
mi_00450d85:;
    /* 00450d85 */
    f->r[0]=0x000000d6u;
    /* 00450d8a */
    f->r[4]+=12;return;
mi_00450d8d:;
    /* 00450d8d */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450d90 */
    if((f->flags&MI_ZF))goto mi_00450dac;
    /* 00450d92 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d93 */
    if((f->flags&MI_ZF))goto mi_00450da4;
    /* 00450d95 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450d96 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450d9c */
    f->r[0]=0x000001ebu;
    /* 00450da1 */
    f->r[4]+=12;return;
mi_00450da4:;
    /* 00450da4 */
    f->r[0]=0x000001eau;
    /* 00450da9 */
    f->r[4]+=12;return;
mi_00450dac:;
    /* 00450dac */
    f->r[0]=0x000000d9u;
    /* 00450db1 */
    f->r[4]+=12;return;
mi_00450db4:;
    /* 00450db4 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450db7 */
    if((f->flags&MI_ZF))goto mi_00450dd3;
    /* 00450db9 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450dba */
    if((f->flags&MI_ZF))goto mi_00450dcb;
    /* 00450dbc */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450dbd */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450dc3 */
    f->r[0]=0x000001edu;
    /* 00450dc8 */
    f->r[4]+=12;return;
mi_00450dcb:;
    /* 00450dcb */
    f->r[0]=0x000001ecu;
    /* 00450dd0 */
    f->r[4]+=12;return;
mi_00450dd3:;
    /* 00450dd3 */
    f->r[0]=0x000000dau;
    /* 00450dd8 */
    f->r[4]+=12;return;
mi_00450ddb:;
    /* 00450ddb */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450dde */
    if((f->flags&MI_ZF))goto mi_00450dfa;
    /* 00450de0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450de1 */
    if((f->flags&MI_ZF))goto mi_00450df2;
    /* 00450de3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450de4 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450dea */
    f->r[0]=0x000001efu;
    /* 00450def */
    f->r[4]+=12;return;
mi_00450df2:;
    /* 00450df2 */
    f->r[0]=0x000001eeu;
    /* 00450df7 */
    f->r[4]+=12;return;
mi_00450dfa:;
    /* 00450dfa */
    f->r[0]=0x000000ddu;
    /* 00450dff */
    f->r[4]+=12;return;
mi_00450e02:;
    /* 00450e02 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450e05 */
    if((f->flags&MI_ZF))goto mi_00450e21;
    /* 00450e07 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e08 */
    if((f->flags&MI_ZF))goto mi_00450e19;
    /* 00450e0a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e0b */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450e11 */
    f->r[0]=0x000001f1u;
    /* 00450e16 */
    f->r[4]+=12;return;
mi_00450e19:;
    /* 00450e19 */
    f->r[0]=0x000001f0u;
    /* 00450e1e */
    f->r[4]+=12;return;
mi_00450e21:;
    /* 00450e21 */
    f->r[0]=0x000000deu;
    /* 00450e26 */
    f->r[4]+=12;return;
mi_00450e29:;
    /* 00450e29 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450e2c */
    if((f->flags&MI_ZF))goto mi_00450e48;
    /* 00450e2e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e2f */
    if((f->flags&MI_ZF))goto mi_00450e40;
    /* 00450e31 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e32 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450e38 */
    f->r[0]=0x000001f3u;
    /* 00450e3d */
    f->r[4]+=12;return;
mi_00450e40:;
    /* 00450e40 */
    f->r[0]=0x000001f2u;
    /* 00450e45 */
    f->r[4]+=12;return;
mi_00450e48:;
    /* 00450e48 */
    f->r[0]=0x000000e1u;
    /* 00450e4d */
    f->r[4]+=12;return;
mi_00450e50:;
    /* 00450e50 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450e53 */
    if((f->flags&MI_ZF))goto mi_00450e6f;
    /* 00450e55 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e56 */
    if((f->flags&MI_ZF))goto mi_00450e67;
    /* 00450e58 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e59 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450e5f */
    f->r[0]=0x000001f5u;
    /* 00450e64 */
    f->r[4]+=12;return;
mi_00450e67:;
    /* 00450e67 */
    f->r[0]=0x000001f4u;
    /* 00450e6c */
    f->r[4]+=12;return;
mi_00450e6f:;
    /* 00450e6f */
    f->r[0]=0x000000e2u;
    /* 00450e74 */
    f->r[4]+=12;return;
mi_00450e77:;
    /* 00450e77 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450e7a */
    if((f->flags&MI_ZF))goto mi_00450e96;
    /* 00450e7c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e7d */
    if((f->flags&MI_ZF))goto mi_00450e8e;
    /* 00450e7f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450e80 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450e86 */
    f->r[0]=0x000001f7u;
    /* 00450e8b */
    f->r[4]+=12;return;
mi_00450e8e:;
    /* 00450e8e */
    f->r[0]=0x000001f6u;
    /* 00450e93 */
    f->r[4]+=12;return;
mi_00450e96:;
    /* 00450e96 */
    f->r[0]=0x000000e5u;
    /* 00450e9b */
    f->r[4]+=12;return;
mi_00450e9e:;
    /* 00450e9e */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450ea1 */
    if((f->flags&MI_ZF))goto mi_00450ebd;
    /* 00450ea3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ea4 */
    if((f->flags&MI_ZF))goto mi_00450eb5;
    /* 00450ea6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ea7 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450ead */
    f->r[0]=0x000001f9u;
    /* 00450eb2 */
    f->r[4]+=12;return;
mi_00450eb5:;
    /* 00450eb5 */
    f->r[0]=0x000001f8u;
    /* 00450eba */
    f->r[4]+=12;return;
mi_00450ebd:;
    /* 00450ebd */
    f->r[0]=0x000000e6u;
    /* 00450ec2 */
    f->r[4]+=12;return;
mi_00450ec5:;
    /* 00450ec5 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450ec8 */
    if((f->flags&MI_ZF))goto mi_00450ee4;
    /* 00450eca */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ecb */
    if((f->flags&MI_ZF))goto mi_00450edc;
    /* 00450ecd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ece */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450ed4 */
    f->r[0]=0x000001fbu;
    /* 00450ed9 */
    f->r[4]+=12;return;
mi_00450edc:;
    /* 00450edc */
    f->r[0]=0x000001fau;
    /* 00450ee1 */
    f->r[4]+=12;return;
mi_00450ee4:;
    /* 00450ee4 */
    f->r[0]=0x000000e9u;
    /* 00450ee9 */
    f->r[4]+=12;return;
mi_00450eec:;
    /* 00450eec */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450eef */
    if((f->flags&MI_ZF))goto mi_00450f0b;
    /* 00450ef1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ef2 */
    if((f->flags&MI_ZF))goto mi_00450f03;
    /* 00450ef4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450ef5 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450efb */
    f->r[0]=0x000001fdu;
    /* 00450f00 */
    f->r[4]+=12;return;
mi_00450f03:;
    /* 00450f03 */
    f->r[0]=0x000001fcu;
    /* 00450f08 */
    f->r[4]+=12;return;
mi_00450f0b:;
    /* 00450f0b */
    f->r[0]=0x000000eau;
    /* 00450f10 */
    f->r[4]+=12;return;
mi_00450f13:;
    /* 00450f13 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450f16 */
    if((f->flags&MI_ZF))goto mi_00450f32;
    /* 00450f18 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f19 */
    if((f->flags&MI_ZF))goto mi_00450f2a;
    /* 00450f1b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f1c */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450f22 */
    f->r[0]=0x000001ffu;
    /* 00450f27 */
    f->r[4]+=12;return;
mi_00450f2a:;
    /* 00450f2a */
    f->r[0]=0x000001feu;
    /* 00450f2f */
    f->r[4]+=12;return;
mi_00450f32:;
    /* 00450f32 */
    f->r[0]=0x000000edu;
    /* 00450f37 */
    f->r[4]+=12;return;
mi_00450f3a:;
    /* 00450f3a */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450f3d */
    if((f->flags&MI_ZF))goto mi_00450f59;
    /* 00450f3f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f40 */
    if((f->flags&MI_ZF))goto mi_00450f51;
    /* 00450f42 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f43 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450f49 */
    f->r[0]=0x00000201u;
    /* 00450f4e */
    f->r[4]+=12;return;
mi_00450f51:;
    /* 00450f51 */
    f->r[0]=0x00000200u;
    /* 00450f56 */
    f->r[4]+=12;return;
mi_00450f59:;
    /* 00450f59 */
    f->r[0]=0x000000eeu;
    /* 00450f5e */
    f->r[4]+=12;return;
mi_00450f61:;
    /* 00450f61 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450f64 */
    if((f->flags&MI_ZF))goto mi_00450f80;
    /* 00450f66 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f67 */
    if((f->flags&MI_ZF))goto mi_00450f78;
    /* 00450f69 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f6a */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450f70 */
    f->r[0]=0x00000203u;
    /* 00450f75 */
    f->r[4]+=12;return;
mi_00450f78:;
    /* 00450f78 */
    f->r[0]=0x00000202u;
    /* 00450f7d */
    f->r[4]+=12;return;
mi_00450f80:;
    /* 00450f80 */
    f->r[0]=0x000000f1u;
    /* 00450f85 */
    f->r[4]+=12;return;
mi_00450f88:;
    /* 00450f88 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450f8b */
    if((f->flags&MI_ZF))goto mi_00450fa7;
    /* 00450f8d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f8e */
    if((f->flags&MI_ZF))goto mi_00450f9f;
    /* 00450f90 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450f91 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450f97 */
    f->r[0]=0x00000205u;
    /* 00450f9c */
    f->r[4]+=12;return;
mi_00450f9f:;
    /* 00450f9f */
    f->r[0]=0x00000204u;
    /* 00450fa4 */
    f->r[4]+=12;return;
mi_00450fa7:;
    /* 00450fa7 */
    f->r[0]=0x000000f2u;
    /* 00450fac */
    f->r[4]+=12;return;
mi_00450faf:;
    /* 00450faf */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450fb2 */
    if((f->flags&MI_ZF))goto mi_00450fce;
    /* 00450fb4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450fb5 */
    if((f->flags&MI_ZF))goto mi_00450fc6;
    /* 00450fb7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450fb8 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450fbe */
    f->r[0]=0x00000207u;
    /* 00450fc3 */
    f->r[4]+=12;return;
mi_00450fc6:;
    /* 00450fc6 */
    f->r[0]=0x00000206u;
    /* 00450fcb */
    f->r[4]+=12;return;
mi_00450fce:;
    /* 00450fce */
    f->r[0]=0x000000f5u;
    /* 00450fd3 */
    f->r[4]+=12;return;
mi_00450fd6:;
    /* 00450fd6 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00450fd9 */
    if((f->flags&MI_ZF))goto mi_00450ff5;
    /* 00450fdb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450fdc */
    if((f->flags&MI_ZF))goto mi_00450fed;
    /* 00450fde */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00450fdf */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00450fe5 */
    f->r[0]=0x00000209u;
    /* 00450fea */
    f->r[4]+=12;return;
mi_00450fed:;
    /* 00450fed */
    f->r[0]=0x00000208u;
    /* 00450ff2 */
    f->r[4]+=12;return;
mi_00450ff5:;
    /* 00450ff5 */
    f->r[0]=0x000000f6u;
    /* 00450ffa */
    f->r[4]+=12;return;
mi_00450ffd:;
    /* 00450ffd */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00451000 */
    if((f->flags&MI_ZF))goto mi_0045101c;
    /* 00451002 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451003 */
    if((f->flags&MI_ZF))goto mi_00451014;
    /* 00451005 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451006 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045100c */
    f->r[0]=0x0000020bu;
    /* 00451011 */
    f->r[4]+=12;return;
mi_00451014:;
    /* 00451014 */
    f->r[0]=0x0000020au;
    /* 00451019 */
    f->r[4]+=12;return;
mi_0045101c:;
    /* 0045101c */
    f->r[0]=0x000000f9u;
    /* 00451021 */
    f->r[4]+=12;return;
mi_00451024:;
    /* 00451024 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00451027 */
    if((f->flags&MI_ZF))goto mi_00451043;
    /* 00451029 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045102a */
    if((f->flags&MI_ZF))goto mi_0045103b;
    /* 0045102c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045102d */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00451033 */
    f->r[0]=0x0000020du;
    /* 00451038 */
    f->r[4]+=12;return;
mi_0045103b:;
    /* 0045103b */
    f->r[0]=0x0000020cu;
    /* 00451040 */
    f->r[4]+=12;return;
mi_00451043:;
    /* 00451043 */
    f->r[0]=0x000000fau;
    /* 00451048 */
    f->r[4]+=12;return;
mi_0045104b:;
    /* 0045104b */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045104e */
    if((f->flags&MI_ZF))goto mi_0045106a;
    /* 00451050 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451051 */
    if((f->flags&MI_ZF))goto mi_00451062;
    /* 00451053 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451054 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045105a */
    f->r[0]=0x0000020fu;
    /* 0045105f */
    f->r[4]+=12;return;
mi_00451062:;
    /* 00451062 */
    f->r[0]=0x0000020eu;
    /* 00451067 */
    f->r[4]+=12;return;
mi_0045106a:;
    /* 0045106a */
    f->r[0]=0x000000fdu;
    /* 0045106f */
    f->r[4]+=12;return;
mi_00451072:;
    /* 00451072 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00451075 */
    if((f->flags&MI_ZF))goto mi_00451091;
    /* 00451077 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451078 */
    if((f->flags&MI_ZF))goto mi_00451089;
    /* 0045107a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045107b */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00451081 */
    f->r[0]=0x00000211u;
    /* 00451086 */
    f->r[4]+=12;return;
mi_00451089:;
    /* 00451089 */
    f->r[0]=0x00000210u;
    /* 0045108e */
    f->r[4]+=12;return;
mi_00451091:;
    /* 00451091 */
    f->r[0]=0x000000feu;
    /* 00451096 */
    f->r[4]+=12;return;
mi_00451099:;
    /* 00451099 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045109c */
    if((f->flags&MI_ZF))goto mi_004510b8;
    /* 0045109e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045109f */
    if((f->flags&MI_ZF))goto mi_004510b0;
    /* 004510a1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004510a2 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004510a8 */
    f->r[0]=0x00000213u;
    /* 004510ad */
    f->r[4]+=12;return;
mi_004510b0:;
    /* 004510b0 */
    f->r[0]=0x00000212u;
    /* 004510b5 */
    f->r[4]+=12;return;
mi_004510b8:;
    /* 004510b8 */
    f->r[0]=0x00000106u;
    /* 004510bd */
    f->r[4]+=12;return;
mi_004510c0:;
    /* 004510c0 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004510c3 */
    if((f->flags&MI_ZF))goto mi_004510df;
    /* 004510c5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004510c6 */
    if((f->flags&MI_ZF))goto mi_004510d7;
    /* 004510c8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004510c9 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004510cf */
    f->r[0]=0x00000215u;
    /* 004510d4 */
    f->r[4]+=12;return;
mi_004510d7:;
    /* 004510d7 */
    f->r[0]=0x00000214u;
    /* 004510dc */
    f->r[4]+=12;return;
mi_004510df:;
    /* 004510df */
    f->r[0]=0x00000107u;
    /* 004510e4 */
    f->r[4]+=12;return;
mi_004510e7:;
    /* 004510e7 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004510ea */
    if((f->flags&MI_ZF))goto mi_00451106;
    /* 004510ec */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004510ed */
    if((f->flags&MI_ZF))goto mi_004510fe;
    /* 004510ef */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004510f0 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004510f6 */
    f->r[0]=0x00000217u;
    /* 004510fb */
    f->r[4]+=12;return;
mi_004510fe:;
    /* 004510fe */
    f->r[0]=0x00000216u;
    /* 00451103 */
    f->r[4]+=12;return;
mi_00451106:;
    /* 00451106 */
    f->r[0]=0x0000011eu;
    /* 0045110b */
    f->r[4]+=12;return;
mi_0045110e:;
    /* 0045110e */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00451111 */
    if((f->flags&MI_ZF))goto mi_0045112d;
    /* 00451113 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451114 */
    if((f->flags&MI_ZF))goto mi_00451125;
    /* 00451116 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451117 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045111d */
    f->r[0]=0x00000219u;
    /* 00451122 */
    f->r[4]+=12;return;
mi_00451125:;
    /* 00451125 */
    f->r[0]=0x00000218u;
    /* 0045112a */
    f->r[4]+=12;return;
mi_0045112d:;
    /* 0045112d */
    f->r[0]=0x0000011fu;
    /* 00451132 */
    f->r[4]+=12;return;
mi_00451135:;
    /* 00451135 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00451138 */
    if((f->flags&MI_ZF))goto mi_00451154;
    /* 0045113a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045113b */
    if((f->flags&MI_ZF))goto mi_0045114c;
    /* 0045113d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045113e */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00451144 */
    f->r[0]=0x0000021bu;
    /* 00451149 */
    f->r[4]+=12;return;
mi_0045114c:;
    /* 0045114c */
    f->r[0]=0x0000021au;
    /* 00451151 */
    f->r[4]+=12;return;
mi_00451154:;
    /* 00451154 */
    f->r[0]=0x00000120u;
    /* 00451159 */
    f->r[4]+=12;return;
mi_0045115c:;
    /* 0045115c */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0045115f */
    if((f->flags&MI_ZF))goto mi_0045117b;
    /* 00451161 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451162 */
    if((f->flags&MI_ZF))goto mi_00451173;
    /* 00451164 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451165 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 0045116b */
    f->r[0]=0x0000021du;
    /* 00451170 */
    f->r[4]+=12;return;
mi_00451173:;
    /* 00451173 */
    f->r[0]=0x0000021cu;
    /* 00451178 */
    f->r[4]+=12;return;
mi_0045117b:;
    /* 0045117b */
    f->r[0]=0x00000121u;
    /* 00451180 */
    f->r[4]+=12;return;
mi_00451183:;
    /* 00451183 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00451186 */
    if((f->flags&MI_ZF))goto mi_004511a2;
    /* 00451188 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00451189 */
    if((f->flags&MI_ZF))goto mi_0045119a;
    /* 0045118b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045118c */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 00451192 */
    f->r[0]=0x0000021fu;
    /* 00451197 */
    f->r[4]+=12;return;
mi_0045119a:;
    /* 0045119a */
    f->r[0]=0x0000021eu;
    /* 0045119f */
    f->r[4]+=12;return;
mi_004511a2:;
    /* 004511a2 */
    f->r[0]=0x00000122u;
    /* 004511a7 */
    f->r[4]+=12;return;
mi_004511aa:;
    /* 004511aa */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004511ad */
    if((f->flags&MI_ZF))goto mi_004511c5;
    /* 004511af */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004511b0 */
    if((f->flags&MI_ZF))goto mi_004511bd;
    /* 004511b2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004511b3 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004511b5 */
    f->r[0]=0x00000221u;
    /* 004511ba */
    f->r[4]+=12;return;
mi_004511bd:;
    /* 004511bd */
    f->r[0]=0x00000220u;
    /* 004511c2 */
    f->r[4]+=12;return;
mi_004511c5:;
    /* 004511c5 */
    f->r[0]=0x00000123u;
    /* 004511ca */
    f->r[4]+=12;return;
mi_004511cd:;
    /* 004511cd */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004511d0 */
    if((f->flags&MI_ZF))goto mi_004511e8;
    /* 004511d2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004511d3 */
    if((f->flags&MI_ZF))goto mi_004511e0;
    /* 004511d5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004511d6 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004511d8 */
    f->r[0]=0x00000223u;
    /* 004511dd */
    f->r[4]+=12;return;
mi_004511e0:;
    /* 004511e0 */
    f->r[0]=0x00000222u;
    /* 004511e5 */
    f->r[4]+=12;return;
mi_004511e8:;
    /* 004511e8 */
    f->r[0]=0x00000124u;
    /* 004511ed */
    f->r[4]+=12;return;
mi_004511f0:;
    /* 004511f0 */
    a=f->r[2];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004511f3 */
    if((f->flags&MI_ZF))goto mi_0045120b;
    /* 004511f5 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004511f6 */
    if((f->flags&MI_ZF))goto mi_00451203;
    /* 004511f8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004511f9 */
    if(!(f->flags&MI_ZF))goto mi_00451213;
    /* 004511fb */
    f->r[0]=0x00000225u;
    /* 00451200 */
    f->r[4]+=12;return;
mi_00451203:;
    /* 00451203 */
    f->r[0]=0x00000224u;
    /* 00451208 */
    f->r[4]+=12;return;
mi_0045120b:;
    /* 0045120b */
    f->r[0]=0x00000125u;
    /* 00451210 */
    f->r[4]+=12;return;
mi_00451213:;
    /* 00451213 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 00451216 */
    f->r[4]+=12;return;
}

void restore_event_C_00451540(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00451540 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00451543 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451544 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451545 */
    f->r[6]=f->r[1];
    /* 00451547 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451548 */
    f->r[0]=mi_read((0x00000028u+f->r[6]),4u);
    /* 0045154b */
    a=f->r[0];t=a>>2u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>1u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0045154e */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00451550 */
    if(!(f->flags&MI_ZF))goto mi_004519a8;
    /* 00451556 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[6]),2u);
    /* 0045155a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045155b */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0045155e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004515c0;
    /* 00451560 */
    switch(mi_read((0x004519b0u+f->r[0]*4u),4u)){case 0x00451567u:goto mi_00451567;case 0x00451577u:goto mi_00451577;case 0x00451592u:goto mi_00451592;case 0x004515a2u:goto mi_004515a2;default:abort();}
mi_00451567:;
    /* 00451567 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000b6u);
    /* 0045156e */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x000000beu);
    /* 00451575 */
    goto mi_004515c0;
mi_00451577:;
    /* 00451577 */
    f->r[1]=f->r[6];
    /* 00451579 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000acu);
    /* 00451580 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451585u;f->boundary(0x00451580u,0x0044ef60u,f);
    /* 00451585 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00451587 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 00451589 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0045158b */
    a=f->r[0];b=0x000000b4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00451590 */
    goto mi_004515bb;
mi_00451592:;
    /* 00451592 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000ffu);
    /* 00451599 */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)(0x00000107u);
    /* 004515a0 */
    goto mi_004515c0;
mi_004515a2:;
    /* 004515a2 */
    f->r[1]=f->r[6];
    /* 004515a4 */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)(0x000000a2u);
    /* 004515ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004515b0u;f->boundary(0x004515abu,0x0044ef60u,f);
    /* 004515b0 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004515b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004515b4 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004515b6 */
    a=f->r[0];b=0x000000aau;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004515bb:;
    /* 004515bb */
    MI_U16((0x0000001cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
mi_004515c0:;
    /* 004515c0 */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 004515c4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004515c5 */
    f->r[1]=f->r[6];
    /* 004515c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004515ccu;f->boundary(0x004515c7u,0x0044e6f0u,f);
    /* 004515cc */
    f->r[1]=f->r[6];
    /* 004515ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004515d3u;f->boundary(0x004515ceu,0x0044d700u,f);
    /* 004515d3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004515d5 */
    if((f->flags&MI_ZF))goto mi_004519a8;
    /* 004515db */
    f->r[1]=f->r[6];
    /* 004515dd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004515e2u;f->boundary(0x004515ddu,0x0044ef60u,f);
    /* 004515e2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004515e4 */
    if((f->flags&MI_ZF))goto mi_00451625;
    /* 004515e6 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 004515ec */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004515f2 */
    a=mi_read((0x00ea5b48u+f->r[2]),4u);b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004515f9 */
    if(mi_signed_less(f->flags))goto mi_00451625;
    /* 004515fb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004515fd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004515ff */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451601 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451603 */
    a=0x00000078u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451605 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0045160f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451614u;f->boundary(0x0045160fu,0x004e0cf0u,f);
    /* 00451614 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00451617 */
    f->r[1]=f->r[6];
    /* 00451619 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045161eu;f->boundary(0x00451619u,0x0044ef90u,f);
    /* 0045161e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0045161f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00451620 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00451621 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00451624 */
    f->r[4]+=4;return;
mi_00451625:;
    /* 00451625 */
    f->r[1]=f->r[6];
    /* 00451627 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045162cu;f->boundary(0x00451627u,0x0044d750u,f);
    /* 0045162c */
    f->r[7]=f->r[0];
    /* 0045162e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00451630 */
    if(!(f->flags&MI_ZF))goto mi_0045179d;
    /* 00451636 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[6]),2u);
    /* 0045163a */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045163c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045163d */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00451640 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0045174a;
    /* 00451646 */
    switch(mi_read((0x004519c0u+f->r[0]*4u),4u)){case 0x0045164du:goto mi_0045164d;case 0x0045169eu:goto mi_0045169e;case 0x004516dau:goto mi_004516da;case 0x00451712u:goto mi_00451712;default:abort();}
mi_0045164d:;
    /* 0045164d */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00000038u+f->r[6]),2u))&0xffffu)<<0);
    /* 00451651 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 00451654 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00451657 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000001cu+f->r[6]),2u);
    /* 0045165b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045165c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045165d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 00451661 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x007848fcu),2u);
    /* 00451668 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451669 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045166a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045166b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451670u;f->boundary(0x0045166bu,0x00446a30u,f);
    /* 00451670 */
    f->r[7]=f->r[0];
    /* 00451672 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00451675 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00451677 */
    if((f->flags&MI_ZF))goto mi_0045174c;
    /* 0045167d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045167f */
    f->r[1]=f->r[6];
    /* 00451681 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0045168b */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0045168d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451692u;f->boundary(0x0045168du,0x0044fd30u,f);
    /* 00451692 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000bcu);
    /* 00451699 */
    goto mi_0045174c;
mi_0045169e:;
    /* 0045169e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 004516a1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000038u+f->r[6]),2u);
    /* 004516a5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000001cu+f->r[6]),2u);
    /* 004516a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516aa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516ab */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 004516af */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516b0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516b1 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004516b8u;f->boundary(0x004516b3u,0x00446a30u,f);
    /* 004516b8 */
    f->r[7]=f->r[0];
    /* 004516ba */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004516bd */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004516bf */
    if((f->flags&MI_ZF))goto mi_0045174c;
    /* 004516c5 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 004516cf */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004516d1 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000b2u);
    /* 004516d8 */
    goto mi_0045174c;
mi_004516da:;
    /* 004516da */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 004516dd */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000038u+f->r[6]),2u);
    /* 004516e1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000001cu+f->r[6]),2u);
    /* 004516e5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516e6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516e7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 004516eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516ec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516ed */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004516ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004516f4u;f->boundary(0x004516efu,0x00446a30u,f);
    /* 004516f4 */
    f->r[7]=f->r[0];
    /* 004516f6 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004516f9 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004516fb */
    if((f->flags&MI_ZF))goto mi_0045174c;
    /* 004516fd */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 00451707 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00451709 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000105u);
    /* 00451710 */
    goto mi_0045174c;
mi_00451712:;
    /* 00451712 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[6]),2u);
    /* 00451715 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000038u+f->r[6]),2u);
    /* 00451719 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000001cu+f->r[6]),2u);
    /* 0045171d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045171e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045171f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x0000000cu+f->r[6]),2u);
    /* 00451723 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451724 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451725 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451727 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045172cu;f->boundary(0x00451727u,0x00446a30u,f);
    /* 0045172c */
    f->r[7]=f->r[0];
    /* 0045172e */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00451731 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00451733 */
    if((f->flags&MI_ZF))goto mi_0045174c;
    /* 00451735 */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 0045173f */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00451741 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000a8u);
    /* 00451748 */
    goto mi_0045174c;
mi_0045174a:;
    /* 0045174a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0045174c:;
    /* 0045174c */
    f->r[1]=mi_read((0x00000028u+f->r[6]),4u);
    /* 0045174f */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 00451752 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00451754 */
    MI_U32((0x00000028u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00451757 */
    if((f->flags&MI_ZF))goto mi_00451780;
    /* 00451759 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00775900u),2u);
    /* 00451760 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 00451764 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451766 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451767 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451768 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451769 */
    a=0x00000085u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045176e */
    MI_U32((0x00e950dcu))=(uint32_t)(0x00000001u);
    /* 00451778 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045177du;f->boundary(0x00451778u,0x004e0cf0u,f);
    /* 0045177d */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00451780:;
    /* 00451780 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451782 */
    f->r[1]=f->r[6];
    /* 00451784 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451789u;f->boundary(0x00451784u,0x0044fd30u,f);
    /* 00451789 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000024u+f->r[6]),2u);
    /* 0045178d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045178e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045178f */
    f->r[1]=f->r[6];
    /* 00451791 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451796u;f->boundary(0x00451791u,0x0044c490u,f);
    /* 00451796 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00451797 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00451798 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00451799 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045179c */
    f->r[4]+=4;return;
mi_0045179d:;
    /* 0045179d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0045179f */
    a=f->r[7];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 004517a2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00451915;
    /* 004517a8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004517aa */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x004519e8u+f->r[7]),1u))&0xffu)<<0);
    /* 004517b0 */
    switch(mi_read((0x004519d0u+f->r[0]*4u),4u)){case 0x004517b7u:goto mi_004517b7;case 0x0045180du:goto mi_0045180d;case 0x00451868u:goto mi_00451868;case 0x004518c3u:goto mi_004518c3;case 0x00451915u:goto mi_00451915;case 0x004519a8u:goto mi_004519a8;default:abort();}
mi_004517b7:;
    /* 004517b7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[6]),2u);
    /* 004517bb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004517bc */
    f->r[1]=(f->r[1]&0xffffff00u)|(((0x00000002u)&0xffu)<<0);
    /* 004517be */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004517c1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00451974;
    /* 004517c7 */
    switch(mi_read((0x00451a04u+f->r[0]*4u),4u)){case 0x004517ceu:goto mi_004517ce;case 0x004517dfu:goto mi_004517df;case 0x004517f0u:goto mi_004517f0;case 0x00451801u:goto mi_00451801;default:abort();}
mi_004517ce:;
    /* 004517ce */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000b8u);
    /* 004517d5 */
    f->r[3]=0x00000077u;
    /* 004517da */
    goto mi_00451974;
mi_004517df:;
    /* 004517df */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000aeu);
    /* 004517e6 */
    f->r[3]=0x00000081u;
    /* 004517eb */
    goto mi_00451974;
mi_004517f0:;
    /* 004517f0 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000101u);
    /* 004517f7 */
    f->r[3]=0x00000077u;
    /* 004517fc */
    goto mi_00451974;
mi_00451801:;
    /* 00451801 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000a4u);
    /* 00451808 */
    goto mi_0045196f;
mi_0045180d:;
    /* 0045180d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[6]),2u);
    /* 00451811 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00451812 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((0x00000004u)&0xffu)<<0);
    /* 00451814 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00451817 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00451974;
    /* 0045181d */
    switch(mi_read((0x00451a14u+f->r[0]*4u),4u)){case 0x00451824u:goto mi_00451824;case 0x00451835u:goto mi_00451835;case 0x00451846u:goto mi_00451846;case 0x00451857u:goto mi_00451857;default:abort();}
mi_00451824:;
    /* 00451824 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000b9u);
    /* 0045182b */
    f->r[3]=0x00000076u;
    /* 00451830 */
    goto mi_00451974;
mi_00451835:;
    /* 00451835 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000afu);
    /* 0045183c */
    f->r[3]=0x00000080u;
    /* 00451841 */
    goto mi_00451974;
mi_00451846:;
    /* 00451846 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000102u);
    /* 0045184d */
    f->r[3]=0x00000076u;
    /* 00451852 */
    goto mi_00451974;
mi_00451857:;
    /* 00451857 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000a5u);
    /* 0045185e */
    f->r[3]=0x0000007du;
    /* 00451863 */
    goto mi_00451974;
mi_00451868:;
    /* 00451868 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[6]),2u);
    /* 0045186c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0045186d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((0x00000008u)&0xffu)<<0);
    /* 0045186f */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00451872 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00451974;
    /* 00451878 */
    switch(mi_read((0x00451a24u+f->r[0]*4u),4u)){case 0x0045187fu:goto mi_0045187f;case 0x00451890u:goto mi_00451890;case 0x004518a1u:goto mi_004518a1;case 0x004518b2u:goto mi_004518b2;default:abort();}
mi_0045187f:;
    /* 0045187f */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000bau);
    /* 00451886 */
    f->r[3]=0x00000071u;
    /* 0045188b */
    goto mi_00451974;
mi_00451890:;
    /* 00451890 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000b0u);
    /* 00451897 */
    f->r[3]=0x0000007fu;
    /* 0045189c */
    goto mi_00451974;
mi_004518a1:;
    /* 004518a1 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000103u);
    /* 004518a8 */
    f->r[3]=0x00000071u;
    /* 004518ad */
    goto mi_00451974;
mi_004518b2:;
    /* 004518b2 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000a6u);
    /* 004518b9 */
    f->r[3]=0x0000007cu;
    /* 004518be */
    goto mi_00451974;
mi_004518c3:;
    /* 004518c3 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[6]),2u);
    /* 004518c7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004518c8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((0x00000010u)&0xffu)<<0);
    /* 004518ca */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004518cd */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00451974;
    /* 004518d3 */
    switch(mi_read((0x00451a34u+f->r[0]*4u),4u)){case 0x004518dau:goto mi_004518da;case 0x004518ebu:goto mi_004518eb;case 0x004518f9u:goto mi_004518f9;case 0x00451907u:goto mi_00451907;default:abort();}
mi_004518da:;
    /* 004518da */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000bbu);
    /* 004518e1 */
    f->r[3]=0x00000071u;
    /* 004518e6 */
    goto mi_00451974;
mi_004518eb:;
    /* 004518eb */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000b1u);
    /* 004518f2 */
    f->r[3]=0x0000007fu;
    /* 004518f7 */
    goto mi_00451974;
mi_004518f9:;
    /* 004518f9 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000104u);
    /* 00451900 */
    f->r[3]=0x00000071u;
    /* 00451905 */
    goto mi_00451974;
mi_00451907:;
    /* 00451907 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000a7u);
    /* 0045190e */
    f->r[3]=0x0000007cu;
    /* 00451913 */
    goto mi_00451974;
mi_00451915:;
    /* 00451915 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451917 */
    f->r[1]=f->r[6];
    /* 00451919 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045191eu;f->boundary(0x00451919u,0x0044d680u,f);
    /* 0045191e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00451920 */
    if((f->flags&MI_ZF))goto mi_004519a8;
    /* 00451926 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00451929 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004519a8;
    /* 0045192b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000004u+f->r[6]),2u);
    /* 0045192f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00451930 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00451932 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00451935 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00451974;
    /* 00451937 */
    switch(mi_read((0x00451a44u+f->r[0]*4u),4u)){case 0x0045193eu:goto mi_0045193e;case 0x0045194cu:goto mi_0045194c;case 0x0045195au:goto mi_0045195a;case 0x00451968u:goto mi_00451968;default:abort();}
mi_0045193e:;
    /* 0045193e */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000b7u);
    /* 00451945 */
    f->r[3]=0x00000077u;
    /* 0045194a */
    goto mi_00451974;
mi_0045194c:;
    /* 0045194c */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000adu);
    /* 00451953 */
    f->r[3]=0x00000081u;
    /* 00451958 */
    goto mi_00451974;
mi_0045195a:;
    /* 0045195a */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x00000100u);
    /* 00451961 */
    f->r[3]=0x00000077u;
    /* 00451966 */
    goto mi_00451974;
mi_00451968:;
    /* 00451968 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)(0x000000a3u);
mi_0045196f:;
    /* 0045196f */
    f->r[3]=0x0000007eu;
mi_00451974:;
    /* 00451974 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000057u+f->r[6]),1u))&0xffu)<<0);
    /* 00451977 */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00451979 */
    if(!(f->flags&MI_ZF))goto mi_004519a8;
    /* 0045197b */
    a=(f->r[0]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045197d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045197f */
    f->r[1]=f->r[6];
    /* 00451981 */
    MI_U8((0x00000057u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 00451984 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00451989u;f->boundary(0x00451984u,0x0044d680u,f);
    /* 00451989 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045198b */
    f->r[1]=(0x0000000cu+f->r[4]);
    /* 0045198f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451991 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 00451993 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451994 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451996 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00451998 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0045199a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045199b */
    MI_U32((0x00e950dcu))=(uint32_t)(f->r[0]);
    /* 004519a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004519a5u;f->boundary(0x004519a0u,0x004e0cf0u,f);
    /* 004519a5 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004519a8:;
    /* 004519a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004519a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004519aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004519ab */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004519ae */
    f->r[4]+=4;return;
}

void restore_event_C_004583f0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004583f0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004583f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004583f2 */
    f->r[7]=MI_U32((0x0078d790u));
    /* 004583f8 */
    f->r[0]=0x00000001u;
    /* 004583fd */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004583ff */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00458401 */
    MI_U32((0x0078d83cu))=(uint32_t)(f->r[6]);
    /* 00458407 */
    if(mi_signed_less(f->flags))goto mi_00458436;
    /* 00458409 */
    f->r[2]=0x0078e178u;
    /* 0045840e */
    f->r[1]=0x00799ae5u;
mi_00458413:;
    /* 00458413 */
    a=MI_U8((0xfffffffbu+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00458417 */
    if(!(f->flags&MI_ZF))goto mi_00458425;
    /* 00458419 */
    a=MI_U8((0x00000000u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0045841c */
    if((f->flags&MI_ZF))goto mi_00458425;
    /* 0045841e */
    MI_U16((0x00000000u+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00458421 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00458422 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_00458425:;
    /* 00458425 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00458426 */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045842c */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0045842e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00458413;
    /* 00458430 */
    MI_U32((0x0078d83cu))=(uint32_t)(f->r[6]);
mi_00458436:;
    /* 00458436 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00458437 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00458438 */
    f->r[4]+=4;return;
}

void restore_event_C_0045aac0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0045aac0 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 0045aac4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045aac6 */
    a=0x000f4240u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045aacb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045aacc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045aad1u;f->boundary(0x0045aaccu,0x0045afe0u,f);
    /* 0045aad1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045aad4 */
    f->r[4]+=4;return;
}

void restore_event_C_00469f80(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00469f80 */
    f->r[2]=MI_U32((0x0078d790u));
    /* 00469f86 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469f87 */
    f->r[3]=0x00000001u;
    /* 00469f8c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469f8d */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00469f8f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469f90 */
    f->r[0]=f->r[3];
    /* 00469f92 */
    if(mi_signed_less(f->flags))goto mi_00469fb4;
    /* 00469f94 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 00469f98 */
    f->r[1]=0x00799af0u;
mi_00469f9d:;
    /* 00469f9d */
    a=MI_U8((0xfffffff0u+f->r[1]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00469fa0 */
    if(!(f->flags&MI_ZF))goto mi_00469fa9;
    /* 00469fa2 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[1]));
    /* 00469fa5 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00469fa7 */
    if((f->flags&MI_ZF))goto mi_00469fba;
mi_00469fa9:;
    /* 00469fa9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00469faa */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00469fb0 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00469fb2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00469f9d;
mi_00469fb4:;
    /* 00469fb4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00469fb5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00469fb6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00469fb8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00469fb9 */
    f->r[4]+=4;return;
mi_00469fba:;
    /* 00469fba */
    f->r[6]=f->r[0];
    /* 00469fbc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00469fbe */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00469fc1 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00469fc3 */
    MI_U32((0x00bc9694u))=(uint32_t)(0x00000000u);
    /* 00469fcd */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00469fd0 */
    f->r[7]=MI_U32((0x007999e4u+f->r[6]));
    /* 00469fd6 */
    MI_U8((0x007999d8u+f->r[6]))=(uint8_t)(0x00000004u);
    /* 00469fdd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 00469fe3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 00469fea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469feb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469fec */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 00469ff3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469ff4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469ff5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469ff6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00469ffbu;f->boundary(0x00469ff6u,0x0047c2b0u,f);
    /* 00469ffb */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00469ffd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046a002u;f->boundary(0x00469ffdu,0x0053b090u,f);
    /* 0046a002 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046a009 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046a010 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046a012 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a013 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 0046a019 */
    f->r[3]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 0046a01c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046a01e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a01f */
    f->r[3]=(0x00000000u+f->r[2]+f->r[0]*1u);
    /* 0046a022 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a023 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0046a025 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a026 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a027 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046a02cu;f->boundary(0x0046a027u,0x00516f90u,f);
    /* 0046a02c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 0046a033 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 0046a03a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0046a03c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[6])))&0xffu)<<0);
    /* 0046a042 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 0046a045 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0046a047 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a048 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[0]*1u);
    /* 0046a04b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a04c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0046a04e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a04f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0046a050 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0046a055u;f->boundary(0x0046a050u,0x005173a0u,f);
    /* 0046a055 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0046a058 */
    f->r[0]=f->r[7];
    /* 0046a05a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0046a05b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0046a05c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0046a05d */
    f->r[4]+=4;return;
}

void restore_event_C_004b1df0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b1df0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004b1df4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b1df5 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b1df8 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b1dfb */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b1dfd */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b1e00 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd652u+f->r[6]));
    /* 004b1e07 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[6])))&0xffu)<<0);
    /* 004b1e0d */
    MI_U8((0x00bcd631u+f->r[6]))=(uint8_t)(0x0000000fu);
    /* 004b1e14 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004b1e16 */
    f->r[0]=f->r[1];
    /* 004b1e18 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b1e1b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b1e1d */
    MI_U8((0x00bcd63eu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004b1e24 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b1e27 */
    MI_U8((0x00bcd641u+f->r[6]))=(uint8_t)(0x000000ffu);
    /* 004b1e2e */
    MI_U8((0x00bcd640u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 004b1e35 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00799a0cu+f->r[0])))&0xffu)<<0);
    /* 004b1e3b */
    f->r[1]=f->r[2];
    /* 004b1e3d */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b1e40 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004b1e46 */
    a=(f->r[2]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004b1e49 */
    MI_U8((0x00799a0cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004b1e4f */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b1e58;
    /* 004b1e51 */
    MI_U8((0x00799a0cu+f->r[0]))=(uint8_t)(0x00000000u);
mi_004b1e58:;
    /* 004b1e58 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004b1e5f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004b1e66 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b1e68 */
    if(!(f->flags&MI_ZF))goto mi_004b1e6f;
    /* 004b1e6a */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b1e6d */
    goto mi_004b1e8b;
mi_004b1e6f:;
    /* 004b1e6f */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004b1e72 */
    if(!(f->flags&MI_ZF))goto mi_004b1e79;
    /* 004b1e74 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b1e77 */
    goto mi_004b1e8b;
mi_004b1e79:;
    /* 004b1e79 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004b1e7c */
    if(!(f->flags&MI_ZF))goto mi_004b1e83;
    /* 004b1e7e */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b1e81 */
    goto mi_004b1e8b;
mi_004b1e83:;
    /* 004b1e83 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004b1e86 */
    if(!(f->flags&MI_ZF))goto mi_004b1e8b;
    /* 004b1e88 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
mi_004b1e8b:;
    /* 004b1e8b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004b1e8d */
    if(!mi_signed_less(f->flags))goto mi_004b1e91;
    /* 004b1e8f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_004b1e91:;
    /* 004b1e91 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b1e93 */
    if(!mi_signed_less(f->flags))goto mi_004b1e97;
    /* 004b1e95 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004b1e97:;
    /* 004b1e97 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004b1e9d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004b1e9f */
    if(mi_signed_less(f->flags))goto mi_004b1ea4;
    /* 004b1ea1 */
    f->r[2]=(0xffffffffu+f->r[1]);
mi_004b1ea4:;
    /* 004b1ea4 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 004b1eaa */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004b1eac */
    if(mi_signed_less(f->flags))goto mi_004b1eb1;
    /* 004b1eae */
    f->r[0]=(0xffffffffu+f->r[1]);
mi_004b1eb1:;
    /* 004b1eb1 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b1eb3 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b1eb5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b1eb6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b1eb7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b1ebcu;f->boundary(0x004b1eb7u,0x0047fe60u,f);
    /* 004b1ebc */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b1ebf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b1ec1 */
    if((f->flags&MI_ZF))goto mi_004b1ee0;
    /* 004b1ec3 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00d3b4e4u)))&0xffffu)<<0);
    /* 004b1ec9 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00d3b4e0u)))&0xffffu)<<0);
    /* 004b1ed0 */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b1ed7 */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b1ede */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b1edf */
    f->r[4]+=4;return;
mi_004b1ee0:;
    /* 004b1ee0 */
    MI_U8((0x00bcd63eu+f->r[6]))=(uint8_t)(0x00000001u);
    /* 004b1ee7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b1ee8 */
    f->r[4]+=4;return;
}

void restore_event_C_004b8330(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b8330 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004b8334 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b8337 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b833a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b833c */
    f->r[0]=0x66666667u;
    /* 004b8341 */
    f->r[1]=MI_U32((0x00bcd720u+f->r[1]*4u));
    /* 004b8348 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004b834a */
    f->r[0]=f->r[2];
    /* 004b834c */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004b834f */
    f->r[2]=f->r[0];
    /* 004b8351 */
    a=f->r[2];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004b8354 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b8356 */
    f->r[4]+=4;return;
}

void restore_event_C_004b8360(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b8360 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004b8364 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b8366 */
    if(!mi_signed_less(f->flags))goto mi_004b836b;
    /* 004b8368 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b836a */
    f->r[4]+=4;return;
mi_004b836b:;
    /* 004b836b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b836c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b836d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b836e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b836f */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004b8371 */
    f->r[6]=0x00bcba9au;
    /* 004b8376 */
    f->r[3]=0x00000001u;
    /* 004b837b */
    f->r[5]=0x0000000du;
mi_004b8380:;
    /* 004b8380 */
    a=MI_U8((0xffffffbeu+f->r[6]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b8383 */
    if(!(f->flags&MI_ZF))goto mi_004b83eb;
    /* 004b8385 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004b8388 */
    if((f->flags&MI_ZF))goto mi_004b83eb;
    /* 004b838a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000025u+f->r[6])))&0xffu)<<0);
    /* 004b838d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b838f */
    if((f->flags&MI_ZF))goto mi_004b83eb;
    /* 004b8391 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffe6u+f->r[6])))&0xffu)<<0);
    /* 004b8394 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8396 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004b83eb;
    /* 004b8398 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000040u+f->r[6]));
    /* 004b839c */
    a=f->r[1];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004b83a0 */
    if(!(f->flags&MI_ZF))goto mi_004b83eb;
    /* 004b83a2 */
    a=MI_U16((0xffffffc2u+f->r[6]));b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004b83a6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000038u+f->r[6]));
    /* 004b83aa */
    if(!(f->flags&MI_ZF))goto mi_004b83cd;
    /* 004b83ac */
    a=f->r[1];b=0x000000c8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b83b2 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004b83b7 */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004b83ba */
    f->r[0]=0x51eb851fu;
    /* 004b83bf */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004b83c1 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004b83c4 */
    f->r[0]=f->r[2];
    /* 004b83c6 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004b83c9 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b83cb */
    goto mi_004b83e9;
mi_004b83cd:;
    /* 004b83cd */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b83d0 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004b83d5 */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004b83d8 */
    f->r[0]=0x51eb851fu;
    /* 004b83dd */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004b83df */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004b83e2 */
    f->r[1]=f->r[2];
    /* 004b83e4 */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004b83e7 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_004b83e9:;
    /* 004b83e9 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004b83eb:;
    /* 004b83eb */
    a=f->r[6];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b83f1 */
    a=f->r[6];b=0x00bcd62au;t=mi_alu(f,a,b,32,1);
    /* 004b83f7 */
    if(mi_signed_less(f->flags))goto mi_004b8380;
    /* 004b83f9 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b83fd */
    f->r[5]=f->r[7];
    /* 004b83ff */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b8401 */
    if((f->flags&MI_ZF))goto mi_004b8405;
    /* 004b8403 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[7]);
mi_004b8405:;
    /* 004b8405 */
    f->r[6]=0x00bcd786u;
mi_004b840a:;
    /* 004b840a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[6])))&0xffu)<<0);
    /* 004b840d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b840f */
    if((f->flags&MI_ZF))goto mi_004b843c;
    /* 004b8411 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b8414 */
    if(!(f->flags&MI_ZF))goto mi_004b843c;
    /* 004b8416 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004b841b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b841d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000005u+f->r[6])))&0xffu)<<0);
    /* 004b8420 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b8422 */
    if(!(f->flags&MI_ZF))goto mi_004b843c;
    /* 004b8424 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000128u+f->r[6]));
    /* 004b842b */
    a=f->r[0];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004b842f */
    if(!(f->flags&MI_ZF))goto mi_004b843c;
    /* 004b8431 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8432 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b8437u;f->boundary(0x004b8432u,0x004b8330u,f);
    /* 004b8437 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b843a */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004b843c:;
    /* 004b843c */
    a=f->r[6];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b8442 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004b8443 */
    a=f->r[6];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004b8449 */
    if(mi_signed_less(f->flags))goto mi_004b840a;
    /* 004b844b */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004b844f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b8451 */
    if((f->flags&MI_ZF))goto mi_004b8459;
    /* 004b8453 */
    f->r[1]=f->r[7];
    /* 004b8455 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004b8457 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_004b8459:;
    /* 004b8459 */
    f->r[0]=f->r[7];
    /* 004b845b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b845c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b845d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b845e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b845f */
    f->r[4]+=4;return;
}

void restore_event_C_004b8690(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b8690 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004b8693 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8694 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8695 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8696 */
    f->r[3]=0x00000001u;
    /* 004b869b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b869c */
    f->r[5]=f->r[3];
    /* 004b869e */
    f->r[7]=0x00bcbadau;
mi_004b86a3:;
    /* 004b86a3 */
    a=MI_U8((0xffffff7eu+f->r[7]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b86a9 */
    if(!(f->flags&MI_ZF))goto mi_004b87b6;
    /* 004b86af */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffc0u+f->r[7])))&0xffu)<<0);
    /* 004b86b2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b86b4 */
    if((f->flags&MI_ZF))goto mi_004b87b6;
    /* 004b86ba */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 004b86bd */
    a=f->r[0];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004b86c1 */
    if(!(f->flags&MI_ZF))goto mi_004b87b6;
    /* 004b86c7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b86c8 */
    MI_U16((0x00000000u+f->r[7]))=(uint16_t)(0x0000ffffu);
    /* 004b86cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b86d2u;f->boundary(0x004b86cdu,0x0040b5e0u,f);
    /* 004b86d2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b86d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b86d7 */
    if((f->flags&MI_ZF))goto mi_004b8778;
    /* 004b86dd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b86de */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b86e3u;f->boundary(0x004b86deu,0x004b8460u,f);
    /* 004b86e3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b86e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b86e8 */
    if((f->flags&MI_ZF))goto mi_004b87b6;
    /* 004b86ee */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b86f1 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b86f4 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b86f6 */
    f->r[0]=(0x00000014u+f->r[4]);
    /* 004b86fa */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b86fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b86fe */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00bcd60cu+f->r[6])))&0xffffu)<<0);
    /* 004b8705 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffacu+f->r[7]));
    /* 004b8709 */
    MI_U16((0x00bcd624u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b8710 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00bcd60eu+f->r[6])))&0xffffu)<<0);
    /* 004b8717 */
    MI_U16((0x00bcd626u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b871e */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 004b8722 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffaeu+f->r[7]));
    /* 004b8726 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8727 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8728 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b872f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b8736 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8737 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8738 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8739 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b873eu;f->boundary(0x004b8739u,0x0040c180u,f);
    /* 004b873e */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 004b8743 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000002cu+f->r[4])))&0xffffu)<<0);
    /* 004b8748 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0xffffffacu+f->r[7])))&0xffffu)<<0);
    /* 004b874c */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b8753 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0xffffffaeu+f->r[7])))&0xffffu)<<0);
    /* 004b8757 */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b875e */
    MI_U16((0x00bcd628u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b8765 */
    MI_U16((0x00bcd62au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b876c */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b876f */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000013u);
    /* 004b8776 */
    goto mi_004b87b6;
mi_004b8778:;
    /* 004b8778 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b877a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffa6u+f->r[7])))&0xffu)<<0);
    /* 004b877d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b877f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b87b6;
    /* 004b8781 */
    f->r[2]=(0xffffff86u+f->r[7]);
    /* 004b8784 */
    f->r[6]=f->r[0];
mi_004b8786:;
    /* 004b8786 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004b8789 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b878b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b87b0;
    /* 004b878d */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b8790 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b8793 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b8795 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b8798 */
    a=MI_U8((0x00bcd606u+f->r[0]));b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b879e */
    if(!(f->flags&MI_ZF))goto mi_004b87b0;
    /* 004b87a0 */
    a=MI_U8((0x00bcd630u+f->r[0]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b87a7 */
    if((f->flags&MI_ZF))goto mi_004b87b0;
    /* 004b87a9 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000010u);
mi_004b87b0:;
    /* 004b87b0 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b87b3 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004b87b4 */
    if(!(f->flags&MI_ZF))goto mi_004b8786;
mi_004b87b6:;
    /* 004b87b6 */
    a=f->r[7];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004b87bc */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004b87bd */
    a=f->r[7];b=0x00bcd66au;t=mi_alu(f,a,b,32,1);
    /* 004b87c3 */
    if(mi_signed_less(f->flags))goto mi_004b86a3;
    /* 004b87c9 */
    f->r[7]=MI_U32((0x00d3b4e4u));
    /* 004b87cf */
    f->r[5]=MI_U32((0x00d3b4e0u));
    /* 004b87d5 */
    f->r[6]=0x00bcd786u;
mi_004b87da:;
    /* 004b87da */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[6])))&0xffu)<<0);
    /* 004b87dd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b87df */
    if((f->flags&MI_ZF))goto mi_004b8876;
    /* 004b87e5 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b87e8 */
    if(!(f->flags&MI_ZF))goto mi_004b8876;
    /* 004b87ee */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000128u+f->r[6]));
    /* 004b87f5 */
    a=f->r[1];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004b87f9 */
    if(!(f->flags&MI_ZF))goto mi_004b8876;
    /* 004b87fb */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004b8800 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b8802 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000005u+f->r[6])))&0xffu)<<0);
    /* 004b8805 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b8807 */
    if(!(f->flags&MI_ZF))goto mi_004b8876;
    /* 004b8809 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000000au+f->r[6])))&0xffffu)<<0);
    /* 004b880d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b880f */
    MI_U16((0x00000022u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b8813 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[6])))&0xffffu)<<0);
    /* 004b8817 */
    MI_U16((0x00000024u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b881b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000050u+f->r[6]));
    /* 004b881f */
    f->r[0]=f->r[1];
    /* 004b8821 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b8824 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b8826 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b8829 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x007999dbu+f->r[0])))&0xffu)<<0);
    /* 004b882f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[0]));
    /* 004b8836 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8837 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8838 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[0]));
    /* 004b883f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8840 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b8845u;f->boundary(0x004b8840u,0x004806c0u,f);
    /* 004b8845 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00d3b4e4u)))&0xffffu)<<0);
    /* 004b884b */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00d3b4e0u)))&0xffffu)<<0);
    /* 004b8852 */
    MI_U16((0x0000001au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b8856 */
    MI_U16((0x0000001cu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004b885a */
    MI_U16((0x00000128u+f->r[6]))=(uint16_t)(0x0000ffffu);
    /* 004b8863 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8866 */
    MI_U32((0x00d3b4e4u))=(uint32_t)(f->r[7]);
    /* 004b886c */
    MI_U32((0x00d3b4e0u))=(uint32_t)(f->r[5]);
    /* 004b8872 */
    MI_U8((0x0000002eu+f->r[6]))=(uint8_t)(0x00000013u);
mi_004b8876:;
    /* 004b8876 */
    a=f->r[6];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b887c */
    a=f->r[6];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004b8882 */
    if(mi_signed_less(f->flags))goto mi_004b87da;
    /* 004b8888 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b8889 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b888a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b888b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b888c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b888f */
    f->r[4]+=4;return;
}

void restore_event_C_004b8890(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b8890 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004b8893 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8894 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b8895 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b8899 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b889a */
    f->r[0]=(0x00000014u+f->r[4]);
    /* 004b889e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b889f */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 004b88a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b88a4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b88a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b88a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b88abu;f->boundary(0x004b88a6u,0x004b8360u,f);
    /* 004b88ab */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b88ae */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004b88b6 */
    f->r[6]=0x00bcba9au;
mi_004b88bb:;
    /* 004b88bb */
    a=MI_U8((0xffffffbeu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b88bf */
    if(!(f->flags&MI_ZF))goto mi_004b89c3;
    /* 004b88c5 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004b88c8 */
    if((f->flags&MI_ZF))goto mi_004b89c3;
    /* 004b88ce */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000040u+f->r[6]));
    /* 004b88d2 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b88d4 */
    if(!(f->flags&MI_ZF))goto mi_004b89c3;
    /* 004b88da */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b88dc */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004b88de */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffe6u+f->r[6])))&0xffu)<<0);
    /* 004b88e1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b88e3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b8915;
    /* 004b88e5 */
    f->r[2]=(0xffffffc6u+f->r[6]);
    /* 004b88e8 */
    f->r[7]=f->r[0];
mi_004b88ea:;
    /* 004b88ea */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004b88ed */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b88ef */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b890f;
    /* 004b88f1 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b88f4 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b88f7 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b88f9 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b88fc */
    a=MI_U8((0x00bcd606u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b8903 */
    if(!(f->flags&MI_ZF))goto mi_004b890f;
    /* 004b8905 */
    a=MI_U8((0x00bcd630u+f->r[0]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b890c */
    if((f->flags&MI_ZF))goto mi_004b890f;
    /* 004b890e */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
mi_004b890f:;
    /* 004b890f */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b8912 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004b8913 */
    if(!(f->flags&MI_ZF))goto mi_004b88ea;
mi_004b8915:;
    /* 004b8915 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004b8919 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b891a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b891b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b8920u;f->boundary(0x004b891bu,0x004cde40u,f);
    /* 004b8920 */
    f->r[3]=f->r[0];
    /* 004b8922 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8925 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b8927 */
    if(mi_signed_less(f->flags))goto mi_004b897b;
    /* 004b8929 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004b892d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b892e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b8933u;f->boundary(0x004b892eu,0x0040b5e0u,f);
    /* 004b8933 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8936 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b8938 */
    if((f->flags&MI_ZF))goto mi_004b8971;
    /* 004b893a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b893b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b8940u;f->boundary(0x004b893bu,0x004b8460u,f);
    /* 004b8940 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8943 */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[0]);
    /* 004b8948 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b894a */
    if((f->flags&MI_ZF))goto mi_004b8971;
    /* 004b894c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b894f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b8952 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b8954 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b8957 */
    MI_U32((0x00bcd720u+f->r[1]))=(uint32_t)(0x00000000u);
    /* 004b8961 */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000004u);
    /* 004b8968 */
    MI_U16((0x00bcd72au+f->r[1]))=(uint16_t)(0x0000ffffu);
mi_004b8971:;
    /* 004b8971 */
    MI_U8((0xffffffc1u+f->r[6]))=(uint8_t)(0x00000001u);
    /* 004b8975 */
    MI_U16((0x00000040u+f->r[6]))=(uint16_t)(0x0000ffffu);
mi_004b897b:;
    /* 004b897b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b897d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffe6u+f->r[6])))&0xffu)<<0);
    /* 004b8980 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004b8982 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b89bf;
    /* 004b8984 */
    f->r[2]=(0xffffffc6u+f->r[6]);
    /* 004b8987 */
    f->r[7]=f->r[0];
mi_004b8989:;
    /* 004b8989 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004b898c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b898e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004b89b9;
    /* 004b8990 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b8993 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b8996 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004b8998 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004b899b */
    a=MI_U8((0x00bcd606u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b89a2 */
    if(!(f->flags&MI_ZF))goto mi_004b89b9;
    /* 004b89a4 */
    a=MI_U8((0x00bcd630u+f->r[0]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b89ab */
    if((f->flags&MI_ZF))goto mi_004b89b9;
    /* 004b89ad */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004b89af */
    if((f->flags&MI_ZF))goto mi_004b89b9;
    /* 004b89b1 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004b89b2 */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
mi_004b89b9:;
    /* 004b89b9 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b89bc */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004b89bd */
    if(!(f->flags&MI_ZF))goto mi_004b8989;
mi_004b89bf:;
    /* 004b89bf */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
mi_004b89c3:;
    /* 004b89c3 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004b89c7 */
    a=f->r[6];b=0x00000090u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b89cd */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004b89ce */
    a=f->r[6];b=0x00bcd62au;t=mi_alu(f,a,b,32,1);
    /* 004b89d4 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004b89d8 */
    if(mi_signed_less(f->flags))goto mi_004b88bb;
    /* 004b89de */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004b89e2 */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004b89e6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b89e7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004b89ea */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b89ec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b89ed */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b89f2u;f->boundary(0x004b89edu,0x004cde40u,f);
    /* 004b89f2 */
    f->r[3]=MI_U32((0x00eaac28u));
    /* 004b89f8 */
    f->r[6]=0x00000001u;
    /* 004b89fd */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8a00 */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[6]);
    /* 004b8a06 */
    f->r[1]=0x00bcd8a4u;
mi_004b8a0b:;
    /* 004b8a0b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffee6u+f->r[1])))&0xffu)<<0);
    /* 004b8a11 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b8a13 */
    if((f->flags&MI_ZF))goto mi_004b8a52;
    /* 004b8a15 */
    a=MI_U8((0xfffffee2u+f->r[1]));b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b8a1c */
    if(!(f->flags&MI_ZF))goto mi_004b8a52;
    /* 004b8a1e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004b8a20 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffee7u+f->r[1])))&0xffu)<<0);
    /* 004b8a26 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004b8a28 */
    if(!(f->flags&MI_ZF))goto mi_004b8a52;
    /* 004b8a2a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000au+f->r[1]));
    /* 004b8a2e */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004b8a30 */
    if(!(f->flags&MI_ZF))goto mi_004b8a52;
    /* 004b8a32 */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 004b8a34 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b8a36 */
    if(mi_signed_less(f->flags))goto mi_004b8a3d;
    /* 004b8a38 */
    a=f->r[7];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004b8a3b */
    if(!(f->flags&MI_ZF))goto mi_004b8a6f;
mi_004b8a3d:;
    /* 004b8a3d */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(0x00000000u);
    /* 004b8a43 */
    MI_U8((0xffffff10u+f->r[1]))=(uint8_t)(0x00000004u);
    /* 004b8a4a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004b8a4c */
    MI_U16((0x0000000au+f->r[1]))=(uint16_t)(0x0000ffffu);
mi_004b8a52:;
    /* 004b8a52 */
    a=f->r[1];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b8a58 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004b8a59 */
    a=f->r[1];b=0x00c8ae60u;t=mi_alu(f,a,b,32,1);
    /* 004b8a5f */
    if(mi_signed_less(f->flags))goto mi_004b8a0b;
    /* 004b8a61 */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[6]);
    /* 004b8a67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b8a68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b8a69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b8a6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b8a6b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8a6e */
    f->r[4]+=4;return;
mi_004b8a6f:;
    /* 004b8a6f */
    f->r[1]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004b8a72 */
    MI_U32((0x00d056f8u))=(uint32_t)(f->r[6]);
    /* 004b8a78 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004b8a7b */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004b8a7d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b8a7e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b8a7f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b8a80 */
    f->r[2]=MI_U32((0x00bcd720u+f->r[1]*4u));
    /* 004b8a87 */
    f->r[1]=(0x00bcd720u+f->r[1]*4u);
    /* 004b8a8e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004b8a90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b8a91 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004b8a93 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b8a96 */
    f->r[4]+=4;return;
}

void restore_event_C_004bd5e0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bd5e0 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004bd5e4 */
    f->r[2]=MI_U32((0x0000001cu+f->r[1]));
    /* 004bd5e7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bd5e9 */
    MI_U32((0x0000001cu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004bd5ec */
    f->r[0]=f->r[2];
    /* 004bd5ee */
    if((f->flags&MI_SF))goto mi_004bd600;
    /* 004bd5f0 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bd5f3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bd602;
    /* 004bd5f5 */
    f->r[0]=0x00000064u;
    /* 004bd5fa */
    MI_U32((0x0000001cu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004bd5fd */
    f->r[4]+=8;return;
mi_004bd600:;
    /* 004bd600 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004bd602:;
    /* 004bd602 */
    MI_U32((0x0000001cu+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004bd605 */
    f->r[4]+=8;return;
}

void restore_event_C_004bed00(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bed00 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bed01 */
    f->r[6]=f->r[1];
    /* 004bed03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bed08u;f->boundary(0x004bed03u,0x00562348u,f);
    /* 004bed08 */
    f->r[1]=f->r[0];
    /* 004bed0a */
    a=f->r[1];b=0x80000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bed10 */
    if(!(f->flags&MI_SF))goto mi_004bed17;
    /* 004bed12 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004bed13 */
    a=f->r[1];b=0xfffffff8u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 004bed16 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
mi_004bed17:;
    /* 004bed17 */
    f->r[0]=0xfffffffdu;
    /* 004bed1c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004bed1e */
    f->r[1]=(0x00032c40u+f->r[6]);
    /* 004bed24 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004bed27 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bed29 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bed2a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bed2fu;f->boundary(0x004bed2au,0x004bd5e0u,f);
    /* 004bed2f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bed30 */
    f->r[4]+=4;return;
}

void restore_event_C_004bed40(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bed40 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bed41 */
    f->r[6]=f->r[1];
    /* 004bed43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bed48u;f->boundary(0x004bed43u,0x00562348u,f);
    /* 004bed48 */
    a=f->r[0];b=0x80000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bed4d */
    if(!(f->flags&MI_SF))goto mi_004bed54;
    /* 004bed4f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bed50 */
    a=f->r[0];b=0xfffffff8u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004bed53 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
mi_004bed54:;
    /* 004bed54 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bed57 */
    f->r[1]=(0x00032c40u+f->r[6]);
    /* 004bed5d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004bed60 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bed62 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bed63 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bed68u;f->boundary(0x004bed63u,0x004bd5e0u,f);
    /* 004bed68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bed69 */
    f->r[4]+=4;return;
}

void restore_event_C_004e6eb0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e6eb0 */
    a=0xfffffff1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6eb2 */
    MI_U32((0x0078d79cu))=(uint32_t)(0xffffffffu);
    /* 004e6ebc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e6ec1u;f->boundary(0x004e6ebcu,0x004e8e80u,f);
    /* 004e6ec1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004e6ec2 */
    f->r[4]+=4;return;
}

void restore_event_C_004e6ed0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e6ed0 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004e6ed6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6ed7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6ed8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6ed9 */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004e6edf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6ee0 */
    f->r[7]=MI_U32((0x00d3b380u));
    /* 004e6ee6 */
    f->r[5]=0x00bcd786u;
mi_004e6eeb:;
    /* 004e6eeb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[5])))&0xffu)<<0);
    /* 004e6eee */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e6ef0 */
    if((f->flags&MI_ZF))goto mi_004e702b;
    /* 004e6ef6 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004e6efb */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004e6f02 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e6f03 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e6f06 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004e6f0b */
    if(mi_signed_less(f->flags))goto mi_004e6f17;
    /* 004e6f0d */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004e6f17:;
    /* 004e6f17 */
    f->r[2]=0x0000001fu;
mi_004e6f1c:;
    /* 004e6f1c */
    f->r[0]=f->r[6];
    /* 004e6f1e */
    f->r[1]=f->r[6];
    /* 004e6f20 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e6f23 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e6f26 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e6f29 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e6f2b */
    f->r[1]=f->r[7];
    /* 004e6f2d */
    a=f->r[1];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004e6f30 */
    f->r[3]=f->r[7];
    /* 004e6f32 */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e6f35 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004e6f38 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e6f3a */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 004e6f3c */
    a=f->r[7];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[7]=t;
    /* 004e6f3e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e6f40 */
    if((f->flags&MI_ZF))goto mi_004e6f48;
    /* 004e6f42 */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_004e6f48:;
    /* 004e6f48 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e6f4a */
    if((f->flags&MI_ZF))goto mi_004e6f52;
    /* 004e6f4c */
    a=f->r[7];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[7]=t;
mi_004e6f52:;
    /* 004e6f52 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004e6f53 */
    if(!(f->flags&MI_ZF))goto mi_004e6f1c;
    /* 004e6f55 */
    f->r[2]=f->r[6];
    /* 004e6f57 */
    f->r[0]=f->r[6];
    /* 004e6f59 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e6f5c */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e6f61 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004e6f67 */
    f->r[2]=f->r[7];
    /* 004e6f69 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e6f6f */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004e6f74 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004e6f7a */
    f->r[2]=f->r[7];
    /* 004e6f7c */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e6f7f */
    f->r[3]=0x00000064u;
    /* 004e6f84 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004e6f8a */
    f->r[2]=f->r[7];
    /* 004e6f8c */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e6f8f */
    f->r[1]=f->r[6];
    /* 004e6f91 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[2]);
    /* 004e6f97 */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e6f9a */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e6f9b */
    a=f->r[3];mi_idiv32(f,a);
    /* 004e6f9d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000002eu+f->r[5])))&0xffu)<<0);
    /* 004e6fa0 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[7]);
    /* 004e6fa6 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004e6fa8 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004e6fae */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[1]);
    /* 004e6fb4 */
    f->r[3]=f->r[2];
    /* 004e6fb6 */
    if(!(f->flags&MI_ZF))goto mi_004e6fbf;
    /* 004e6fb8 */
    f->r[0]=0x00000019u;
    /* 004e6fbd */
    goto mi_004e7023;
mi_004e6fbf:;
    /* 004e6fbf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[5])))&0xffu)<<0);
    /* 004e6fc2 */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004e6fc4 */
    if((f->flags&MI_ZF))goto mi_004e701e;
    /* 004e6fc6 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004e6fc8 */
    if((f->flags&MI_ZF))goto mi_004e701e;
    /* 004e6fca */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004e6fcc */
    if((f->flags&MI_ZF))goto mi_004e701e;
    /* 004e6fce */
    a=(f->r[0]&0xffu);b=0x0000002bu;t=mi_alu(f,a,b,8,1);
    /* 004e6fd0 */
    if((f->flags&MI_CF))goto mi_004e6fd6;
    /* 004e6fd2 */
    a=(f->r[0]&0xffu);b=0x00000039u;t=mi_alu(f,a,b,8,1);
    /* 004e6fd4 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004e701e;
mi_004e6fd6:;
    /* 004e6fd6 */
    a=(f->r[0]&0xffu);b=0x00000063u;t=mi_alu(f,a,b,8,1);
    /* 004e6fd8 */
    if((f->flags&MI_ZF))goto mi_004e701e;
    /* 004e6fda */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e6fdf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6fe0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e6fe5u;f->boundary(0x004e6fe0u,0x004ea9c0u,f);
    /* 004e6fe5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e6fe8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e6fea */
    if((f->flags&MI_ZF))goto mi_004e6ff3;
    /* 004e6fec */
    f->r[0]=0x00000032u;
    /* 004e6ff1 */
    goto mi_004e700a;
mi_004e6ff3:;
    /* 004e6ff3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e6ff5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[5])))&0xffu)<<0);
    /* 004e6ff8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6ff9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e6ffeu;f->boundary(0x004e6ff9u,0x004eada0u,f);
    /* 004e6ffe */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7001 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e7003 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004e7005 */
    a=(f->r[0]&0xffu);b=0x000000e7u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e7007 */
    a=f->r[0];b=0x0000004bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004e700a:;
    /* 004e700a */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004e7010 */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004e7016 */
    f->r[7]=MI_U32((0x00d3b380u));
    /* 004e701c */
    goto mi_004e7023;
mi_004e701e:;
    /* 004e701e */
    f->r[0]=0x00000032u;
mi_004e7023:;
    /* 004e7023 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e7025 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e702b;
    /* 004e7027 */
    MI_U8((0x0000002eu+f->r[5]))=(uint8_t)(0x00000004u);
mi_004e702b:;
    /* 004e702b */
    a=f->r[5];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004e7031 */
    a=f->r[5];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004e7037 */
    if(mi_signed_less(f->flags))goto mi_004e6eeb;
    /* 004e703d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e703f */
    f->r[1]=0x00e92bf8u;
    /* 004e7044 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7049u;f->boundary(0x004e7044u,0x0055f5a1u,f);
    /* 004e7049 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e704a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e704b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e704c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e704d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e704f */
    if((f->flags&MI_ZF))goto mi_004e705d;
    /* 004e7051 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7053 */
    f->r[1]=0x00e92bf8u;
    /* 004e7058 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e705du;f->boundary(0x004e7058u,0x0055f561u,f);
mi_004e705d:;
    /* 004e705d */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e705f */
    f->r[1]=0x00e92bf8u;
    /* 004e7064 */
    MI_U32((0x00f55b94u))=(uint32_t)(0x00000001u);
    /* 004e706e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7073u;f->boundary(0x004e706eu,0x0055f5a1u,f);
    /* 004e7073 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7075 */
    if((f->flags&MI_ZF))goto mi_004e7083;
    /* 004e7077 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7079 */
    f->r[1]=0x00e92bf8u;
    /* 004e707e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7083u;f->boundary(0x004e707eu,0x0055f561u,f);
mi_004e7083:;
    /* 004e7083 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7085 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7087 */
    a=0x005c92f8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e708c */
    f->r[1]=0x00e92bf8u;
    /* 004e7091 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7096u;f->boundary(0x004e7091u,0x0055f449u,f);
    /* 004e7096 */
    a=0xfffffff1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7098 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e709du;f->boundary(0x004e7098u,0x004e8e80u,f);
    /* 004e709d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e70a0 */
    MI_U32((0x0078d7a0u))=(uint32_t)(0x00000198u);
    /* 004e70aa */
    f->r[4]+=4;return;
}

void restore_event_C_004e70b0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e70b0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004e70b4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e70b5 */
    f->r[0]=f->r[1];
    /* 004e70b7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e70b8 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e70bb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e70bd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e70be */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e70bf */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004e70c1 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e70c4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e70c6 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e70c9 */
    MI_U32((0x0078d7a8u))=(uint32_t)(f->r[0]);
    /* 004e70ce */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004e70d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e70d5u;f->boundary(0x004e70d0u,0x004583f0u,f);
    /* 004e70d5 */
    f->r[3]=MI_U32((0x0078d83cu));
    /* 004e70db */
    MI_U32((0x0078d840u))=(uint32_t)(f->r[5]);
    /* 004e70e1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004e70e3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7119;
    /* 004e70e5 */
    f->r[2]=0x0078e178u;
    /* 004e70ea */
    MI_U32((0x0078d840u))=(uint32_t)(f->r[3]);
mi_004e70f0:;
    /* 004e70f0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004e70f3 */
    f->r[0]=f->r[1];
    /* 004e70f5 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e70f8 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e70fb */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e70fd */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7100 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[0]));
    /* 004e7107 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004e7109 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e710d;
    /* 004e710b */
    f->r[6]=f->r[1];
mi_004e710d:;
    /* 004e710d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[0]));
    /* 004e7114 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004e7116 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004e7117 */
    if(!(f->flags&MI_ZF))goto mi_004e70f0;
mi_004e7119:;
    /* 004e7119 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e711c */
    if(mi_signed_less(f->flags))goto mi_004e719b;
    /* 004e711e */
    f->r[1]=f->r[7];
    /* 004e7120 */
    f->r[0]=0x51eb851fu;
    /* 004e7125 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e7128 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e712a */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004e712c */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004e712f */
    f->r[0]=f->r[2];
    /* 004e7131 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e7134 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e7136 */
    f->r[7]=f->r[2];
mi_004e7138:;
    /* 004e7138 */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e713a */
    if(!mi_signed_less(f->flags))goto mi_004e719b;
    /* 004e713c */
    f->r[0]=MI_U32((0x0078d83cu));
    /* 004e7141 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004e7143 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7145 */
    MI_U32((0x0078d840u))=(uint32_t)(f->r[2]);
    /* 004e714b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7195;
mi_004e714d:;
    /* 004e714d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0078e178u+f->r[2]*2u));
    /* 004e7155 */
    f->r[0]=f->r[1];
    /* 004e7157 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004e7158 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e715b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e715d */
    MI_U32((0x0078d840u))=(uint32_t)(f->r[2]);
    /* 004e7163 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7166 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[0]));
    /* 004e716d */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004e716f */
    if(!(f->flags&MI_ZF))goto mi_004e718d;
    /* 004e7171 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[0]));
    /* 004e7178 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004e717a */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e717c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e718d;
    /* 004e717e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e717f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7184u;f->boundary(0x004e717fu,0x004e7300u,f);
    /* 004e7184 */
    f->r[2]=MI_U32((0x0078d840u));
    /* 004e718a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e718d:;
    /* 004e718d */
    a=f->r[2];b=MI_U32((0x0078d83cu));t=mi_alu(f,a,b,32,1);
    /* 004e7193 */
    if(mi_signed_less(f->flags))goto mi_004e714d;
mi_004e7195:;
    /* 004e7195 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e7196 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e7199 */
    if(!mi_signed_less(f->flags))goto mi_004e7138;
mi_004e719b:;
    /* 004e719b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e719d */
    f->r[1]=0x00e92bf8u;
    /* 004e71a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e71a7u;f->boundary(0x004e71a2u,0x0055f5a1u,f);
    /* 004e71a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e71a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e71a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e71aa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e71ab */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e71ad */
    if((f->flags&MI_ZF))goto mi_004e71bb;
    /* 004e71af */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e71b1 */
    f->r[1]=0x00e92bf8u;
    /* 004e71b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e71bbu;f->boundary(0x004e71b6u,0x0055f561u,f);
mi_004e71bb:;
    /* 004e71bb */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e71bd */
    f->r[1]=0x00e92bf8u;
    /* 004e71c2 */
    MI_U32((0x00f55b94u))=(uint32_t)(0x00000001u);
    /* 004e71cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e71d1u;f->boundary(0x004e71ccu,0x0055f5a1u,f);
    /* 004e71d1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e71d3 */
    if((f->flags&MI_ZF))goto mi_004e71e1;
    /* 004e71d5 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e71d7 */
    f->r[1]=0x00e92bf8u;
    /* 004e71dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e71e1u;f->boundary(0x004e71dcu,0x0055f561u,f);
mi_004e71e1:;
    /* 004e71e1 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e71e3 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e71e5 */
    a=0x005e4940u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e71ea */
    f->r[1]=0x00e92bf8u;
    /* 004e71ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e71f4u;f->boundary(0x004e71efu,0x0055f449u,f);
    /* 004e71f4 */
    a=0xfffffff1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e71f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e71fbu;f->boundary(0x004e71f6u,0x004e8e80u,f);
    /* 004e71fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004e71fc */
    f->r[4]+=4;return;
}

void restore_event_C_004e7200(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e7200 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7201 */
    f->r[5]=MI_U32((0x00000008u+f->r[4]));
    /* 004e7205 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7206 */
    a=f->r[5];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004e7209 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e720a */
    if(!mi_signed_less(f->flags))goto mi_004e7213;
    /* 004e720c */
    f->r[5]=0x00000003u;
    /* 004e7211 */
    goto mi_004e721d;
mi_004e7213:;
    /* 004e7213 */
    a=f->r[5];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004e7216 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e721d;
    /* 004e7218 */
    f->r[5]=0x00000009u;
mi_004e721d:;
    /* 004e721d */
    f->r[0]=MI_U32((0x0078d790u));
    /* 004e7222 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e7225 */
    if(mi_signed_less(f->flags))goto mi_004e7279;
    /* 004e7227 */
    f->r[1]=0x00799bc8u;
    /* 004e722c */
    f->r[6]=f->r[0];
mi_004e722e:;
    /* 004e722e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffff1du+f->r[1])))&0xffu)<<0);
    /* 004e7234 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e7236 */
    if((f->flags&MI_ZF))goto mi_004e7270;
    /* 004e7238 */
    f->r[7]=MI_U32((0x0000000cu+f->r[1]));
    /* 004e723b */
    f->r[0]=0x00001388u;
    /* 004e7240 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e7241 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e7242 */
    a=f->r[7];mi_idiv32(f,a);
    /* 004e7244 */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 004e7246 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e7248 */
    f->r[0]=0x00001388u;
    /* 004e724d */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004e724f */
    f->r[7]=MI_U32((0x0000000cu+f->r[1]));
    /* 004e7252 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e7253 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e7254 */
    a=f->r[7];mi_idiv32(f,a);
    /* 004e7256 */
    f->r[2]=MI_U32((0x00000014u+f->r[1]));
    /* 004e7259 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e725b */
    MI_U32((0x00000014u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004e725e */
    f->r[0]=MI_U32((0x0000000cu+f->r[1]));
    /* 004e7261 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004e7263 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffff8fu+f->r[1])))&0xffu)<<0);
    /* 004e7266 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e7269 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004e726b */
    if(!mi_signed_less(f->flags))goto mi_004e7270;
    /* 004e726d */
    MI_U8((0xffffff8fu+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
mi_004e7270:;
    /* 004e7270 */
    a=f->r[1];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e7276 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e7277 */
    if(!(f->flags&MI_ZF))goto mi_004e722e;
mi_004e7279:;
    /* 004e7279 */
    f->r[7]=0x00000001u;
    /* 004e727e */
    f->r[6]=0x00bcd786u;
mi_004e7283:;
    /* 004e7283 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[6])))&0xffu)<<0);
    /* 004e7286 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e7288 */
    if((f->flags&MI_ZF))goto mi_004e72ca;
    /* 004e728a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004e728c */
    a=(f->r[0]&0xffu);b=0x0000005fu;t=mi_alu(f,a,b,8,1);
    /* 004e728e */
    if(!(f->flags&MI_ZF))goto mi_004e7298;
    /* 004e7290 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7291 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7296u;f->boundary(0x004e7291u,0x004b5420u,f);
    /* 004e7296 */
    goto mi_004e72c7;
mi_004e7298:;
    /* 004e7298 */
    a=(f->r[0]&0xffu);b=0x00000019u;t=mi_alu(f,a,b,8,1);
    /* 004e729a */
    if(!(f->flags&MI_ZF))goto mi_004e72ca;
    /* 004e729c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000002eu+f->r[6])))&0xffu)<<0);
    /* 004e729f */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004e72a1 */
    if((f->flags&MI_ZF))goto mi_004e72a7;
    /* 004e72a3 */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004e72a5 */
    if(!(f->flags&MI_ZF))goto mi_004e72ca;
mi_004e72a7:;
    /* 004e72a7 */
    MI_U16((0x0000002cu+f->r[6]))=(uint16_t)(0x00000000u);
    /* 004e72ad */
    MI_U8((0x0000002eu+f->r[6]))=(uint8_t)(0x0000000du);
    /* 004e72b1 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000022u+f->r[6])))&0xffffu)<<0);
    /* 004e72b5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e72b6 */
    MI_U16((0x0000001au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004e72ba */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[6])))&0xffffu)<<0);
    /* 004e72be */
    MI_U16((0x0000001cu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004e72c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e72c7u;f->boundary(0x004e72c2u,0x004b3250u,f);
mi_004e72c7:;
    /* 004e72c7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e72ca:;
    /* 004e72ca */
    a=f->r[6];b=0x00000184u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e72d0 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e72d1 */
    a=f->r[6];b=0x00c8ad42u;t=mi_alu(f,a,b,32,1);
    /* 004e72d7 */
    if(mi_signed_less(f->flags))goto mi_004e7283;
    /* 004e72d9 */
    f->r[0]=f->r[5];
    /* 004e72db */
    a=0xfffffff1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e72dd */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e72e0 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e72e2 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e72e5 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e72e8 */
    MI_U32((0x0078d7b0u))=(uint32_t)(f->r[0]);
    /* 004e72ed */
    MI_U32((0x0078d7acu))=(uint32_t)(f->r[0]);
    /* 004e72f2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e72f7u;f->boundary(0x004e72f2u,0x004e8e80u,f);
    /* 004e72f7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e72fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e72fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e72fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e72fd */
    f->r[4]+=4;return;
}

void restore_event_C_004e7300(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e7300 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7301 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004e7306 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004e730c */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004e7313 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e7314 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e7317 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004e731c */
    if(mi_signed_less(f->flags))goto mi_004e7328;
    /* 004e731e */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004e7328:;
    /* 004e7328 */
    f->r[2]=MI_U32((0x00d3b384u));
    /* 004e732e */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004e7334 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7335 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7336 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7337 */
    f->r[7]=0x0000001fu;
mi_004e733c:;
    /* 004e733c */
    f->r[0]=f->r[2];
    /* 004e733e */
    f->r[6]=f->r[2];
    /* 004e7340 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e7343 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e7346 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004e7349 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e734b */
    f->r[6]=f->r[1];
    /* 004e734d */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004e7350 */
    f->r[3]=f->r[1];
    /* 004e7352 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004e7355 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004e7358 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e735a */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 004e735c */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004e735e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7360 */
    if((f->flags&MI_ZF))goto mi_004e7368;
    /* 004e7362 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_004e7368:;
    /* 004e7368 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e736a */
    if((f->flags&MI_ZF))goto mi_004e7372;
    /* 004e736c */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004e7372:;
    /* 004e7372 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e7373 */
    if(!(f->flags&MI_ZF))goto mi_004e733c;
    /* 004e7375 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 004e737b */
    f->r[0]=f->r[2];
    /* 004e737d */
    f->r[6]=f->r[2];
    /* 004e737f */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7382 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004e7388 */
    f->r[2]=f->r[1];
    /* 004e738a */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7390 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004e7396 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004e739c */
    f->r[2]=f->r[1];
    /* 004e739e */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e73a3 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e73a6 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e73a9 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004e73ae */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004e73b4 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004e73ba */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e73bb */
    f->r[1]=0x00000064u;
    /* 004e73c0 */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004e73c3 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004e73c5 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004e73cb */
    a=f->r[2];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 004e73ce */
    if(!mi_signed_less(f->flags))goto mi_004e73fe;
    /* 004e73d0 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004e73d4 */
    f->r[2]=(0x00000014u+f->r[4]);
    /* 004e73d8 */
    f->r[0]=(0x0000000cu+f->r[4]);
    /* 004e73dc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e73dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e73de */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e73df */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e73e4u;f->boundary(0x004e73dfu,0x004e75a0u,f);
    /* 004e73e4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e73e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e73e9 */
    if((f->flags&MI_ZF))goto mi_004e73fe;
    /* 004e73eb */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004e73ef */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004e73f3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e73f4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e73f5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e73f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e73fbu;f->boundary(0x004e73f6u,0x004e7410u,f);
    /* 004e73fb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e73fe:;
    /* 004e73fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e73ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e7400 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e7401 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004e7402 */
    f->r[4]+=4;return;
}

void restore_event_C_004e7410(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e7410 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 004e7414 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004e7418 */
    a=f->r[4];b=0x000001f8u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e741e */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e7424 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7425 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7426 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7427 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7428 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e742a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e742b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e742c */
    a=0x0000006au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e742e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e742f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7434u;f->boundary(0x004e742fu,0x004b4800u,f);
    /* 004e7434 */
    f->r[5]=f->r[0];
    /* 004e7436 */
    f->r[0]=MI_U32((0x00000220u+f->r[4]));
    /* 004e743d */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 004e7443 */
    f->r[1]=f->r[0];
    /* 004e7445 */
    f->r[6]=(0x00000000u+f->r[5]+f->r[5]*2u);
    /* 004e7449 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e744c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e744f */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e7451 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e7454 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e7457 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 004e745e */
    MI_U16((0x00bcd654u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004e7465 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e7467 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004e746c */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 004e7473 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e7474 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e7477 */
    MI_U16((0x00799a06u+f->r[1]*8u))=(uint16_t)((f->r[5]&0xffffu));
    /* 004e747f */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004e7484 */
    if(mi_signed_less(f->flags))goto mi_004e7490;
    /* 004e7486 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004e7490:;
    /* 004e7490 */
    f->r[2]=MI_U32((0x00d3b384u));
    /* 004e7496 */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004e749c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004e74a4:;
    /* 004e74a4 */
    f->r[0]=f->r[2];
    /* 004e74a6 */
    f->r[7]=f->r[2];
    /* 004e74a8 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e74ab */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e74ae */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004e74b1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e74b3 */
    f->r[7]=f->r[1];
    /* 004e74b5 */
    a=f->r[7];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[7]=t;
    /* 004e74b8 */
    f->r[3]=f->r[1];
    /* 004e74ba */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004e74bd */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004e74c0 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004e74c2 */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 004e74c4 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004e74c6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e74c8 */
    if((f->flags&MI_ZF))goto mi_004e74d0;
    /* 004e74ca */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_004e74d0:;
    /* 004e74d0 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004e74d2 */
    if((f->flags&MI_ZF))goto mi_004e74da;
    /* 004e74d4 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004e74da:;
    /* 004e74da */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004e74de */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e74df */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e74e3 */
    if(!(f->flags&MI_ZF))goto mi_004e74a4;
    /* 004e74e5 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 004e74eb */
    f->r[0]=f->r[2];
    /* 004e74ed */
    f->r[7]=f->r[2];
    /* 004e74ef */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e74f2 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004e74f8 */
    f->r[2]=f->r[1];
    /* 004e74fa */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7500 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004e7506 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004e750c */
    f->r[2]=f->r[1];
    /* 004e750e */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e7513 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7516 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e7519 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004e751e */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004e7524 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004e752a */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e752b */
    f->r[1]=0x00000096u;
    /* 004e7530 */
    a=f->r[7];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 004e7533 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004e7535 */
    f->r[0]=MI_U32((0x00000214u+f->r[4]));
    /* 004e753c */
    f->r[1]=MI_U32((0x00000210u+f->r[4]));
    /* 004e7543 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[7]);
    /* 004e7549 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004e7550 */
    f->r[2]=MI_U32((0x0000020cu+f->r[4]));
    /* 004e7557 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7558 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7559 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e755a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e755b */
    f->r[2]=(0x00000024u+f->r[4]);
    /* 004e755f */
    a=0x005e4958u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7564 */
    MI_U8((0x00bcd65cu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004e756b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e756c */
    MI_U8((0x00bcd66au+f->r[6]))=(uint8_t)(0x00000006u);
    /* 004e7573 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7578u;f->boundary(0x004e7573u,0x00561a36u,f);
    /* 004e7578 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e757b */
    f->r[0]=(0x00000014u+f->r[4]);
    /* 004e757f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7580 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7586u;f->boundary(0x004e7580u,0x0056f0f8u,f);
    /* 004e7586 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e7587 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e7588 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e7589 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e758a */
    a=f->r[4];b=0x000001f8u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7590 */
    f->r[4]+=4;return;
}

void restore_event_C_004e75a0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e75a0 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004e75a5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e75a8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75a9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75aa */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75ac */
    f->r[7]=MI_U32((0x00bca2d0u));
    /* 004e75b2 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e75b4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e75b8;
    /* 004e75b6 */
    f->r[7]=f->r[0];
mi_004e75b8:;
    /* 004e75b8 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004e75bc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75be */
    f->r[6]=f->r[0];
    /* 004e75c0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75c1 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e75c4 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e75c6 */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 004e75ca */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e75cd */
    f->r[1]=(0x00000018u+f->r[4]);
    /* 004e75d1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75d2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004e75d9 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004e75e0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75e1 */
    a=0x00040000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75e6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75e8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75ea */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e75ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e75f1u;f->boundary(0x004e75ecu,0x004eb800u,f);
    /* 004e75f1 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e75f4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e75f6 */
    if((f->flags&MI_ZF))goto mi_004e7633;
    /* 004e75f8 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004e75ff */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004e7603 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 004e7607 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004e760e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004e7610 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e7612 */
    f->r[2]=f->r[1];
    /* 004e7614 */
    a=f->r[2];b=f->r[1];t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e7617 */
    f->r[1]=f->r[0];
    /* 004e7619 */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e761c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e761e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004e7622 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004e7626 */
    __asm { fsqrt }
    /* 004e7628 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e762du;f->boundary(0x004e7628u,0x0056133cu,f);
    /* 004e762d */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e7631 */
    goto mi_004e7637;
mi_004e7633:;
    /* 004e7633 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[7]);
mi_004e7637:;
    /* 004e7637 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004e763e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7640 */
    f->r[2]=(0x0000001cu+f->r[4]);
    /* 004e7644 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7645 */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 004e7649 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e764a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e764b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004e7652 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7654 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7656 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7658 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7659 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e765a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e765fu;f->boundary(0x004e765au,0x004eb800u,f);
    /* 004e765f */
    f->r[3]=MI_U32((0x0000003cu+f->r[4]));
    /* 004e7663 */
    f->r[5]=MI_U32((0x00000040u+f->r[4]));
    /* 004e7667 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e766a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e766c */
    if((f->flags&MI_ZF))goto mi_004e769d;
    /* 004e766e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004e7675 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004e767c */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004e767e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e7680 */
    f->r[2]=f->r[1];
    /* 004e7682 */
    a=f->r[2];b=f->r[1];t=mi_imul(f,a,b);f->r[2]=t;
    /* 004e7685 */
    f->r[1]=f->r[0];
    /* 004e7687 */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004e768a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e768c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004e7690 */
    p=(0x0000001cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004e7694 */
    __asm { fsqrt }
    /* 004e7696 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e769bu;f->boundary(0x004e7696u,0x0056133cu,f);
    /* 004e769b */
    goto mi_004e769f;
mi_004e769d:;
    /* 004e769d */
    f->r[0]=f->r[7];
mi_004e769f:;
    /* 004e769f */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 004e76a3 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e76a5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e76c6;
    /* 004e76a7 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e76a9 */
    if(!mi_signed_less(f->flags))goto mi_004e76c4;
    /* 004e76ab */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004e76af */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004e76b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e76b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e76b5 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 004e76b7 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e76b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e76ba */
    f->r[0]=0x00000001u;
    /* 004e76bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e76c0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e76c3 */
    f->r[4]+=4;return;
mi_004e76c4:;
    /* 004e76c4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
mi_004e76c6:;
    /* 004e76c6 */
    if(!mi_signed_less(f->flags))goto mi_004e76ed;
    /* 004e76c8 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e76ca */
    if(!mi_signed_less(f->flags))goto mi_004e76ed;
    /* 004e76cc */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004e76d0 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004e76d4 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004e76d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e76d9 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 004e76db */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004e76df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e76e0 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e76e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e76e3 */
    f->r[0]=0x00000001u;
    /* 004e76e8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e76e9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e76ec */
    f->r[4]+=4;return;
mi_004e76ed:;
    /* 004e76ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e76ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e76ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e76f0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e76f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e76f3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e76f6 */
    f->r[4]+=4;return;
}

void restore_event_C_004e7ac0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e7ac0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ac1 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004e7ac5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ac6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ac7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ac8 */
    MI_U32((0x0078d798u))=(uint32_t)(f->r[0]);
    /* 004e7acd */
    f->r[0]=MI_U32((0x0078d790u));
    /* 004e7ad2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ad3 */
    f->r[6]=0x00000001u;
    /* 004e7ad8 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004e7ada */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004e7adc */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004e7ade */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004e7ae2 */
    if(mi_signed_less(f->flags))goto mi_004e7c7c;
mi_004e7ae8:;
    /* 004e7ae8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7ae9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7aeeu;f->boundary(0x004e7ae9u,0x004e7c90u,f);
    /* 004e7aee */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7af1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e7af3 */
    if((f->flags&MI_ZF))goto mi_004e7af6;
    /* 004e7af5 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
mi_004e7af6:;
    /* 004e7af6 */
    f->r[3]=MI_U32((0x0078d790u));
    /* 004e7afc */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e7afd */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004e7aff */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7ae8;
    /* 004e7b01 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004e7b03 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7c7c;
    /* 004e7b09 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004e7b0e */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004e7b14 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004e7b1b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e7b1c */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e7b1f */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004e7b24 */
    if(mi_signed_less(f->flags))goto mi_004e7b2c;
    /* 004e7b26 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[5]);
mi_004e7b2c:;
    /* 004e7b2c */
    f->r[2]=MI_U32((0x00d3b384u));
    /* 004e7b32 */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004e7b38 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004e7b40:;
    /* 004e7b40 */
    f->r[0]=f->r[2];
    /* 004e7b42 */
    f->r[6]=f->r[2];
    /* 004e7b44 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004e7b47 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e7b4a */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004e7b4d */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e7b4f */
    f->r[6]=f->r[1];
    /* 004e7b51 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004e7b54 */
    f->r[5]=f->r[1];
    /* 004e7b56 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004e7b59 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004e7b5c */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e7b5e */
    a=f->r[2];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[2]=t;
    /* 004e7b60 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004e7b62 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7b64 */
    if((f->flags&MI_ZF))goto mi_004e7b6c;
    /* 004e7b66 */
    a=f->r[2];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[2]=t;
mi_004e7b6c:;
    /* 004e7b6c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004e7b6e */
    if((f->flags&MI_ZF))goto mi_004e7b76;
    /* 004e7b70 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004e7b76:;
    /* 004e7b76 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004e7b7a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004e7b7b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e7b7f */
    if(!(f->flags&MI_ZF))goto mi_004e7b40;
    /* 004e7b81 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[2]);
    /* 004e7b87 */
    f->r[0]=f->r[2];
    /* 004e7b89 */
    f->r[6]=f->r[2];
    /* 004e7b8b */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7b8e */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004e7b94 */
    f->r[2]=f->r[1];
    /* 004e7b96 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7b9c */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004e7ba1 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004e7ba7 */
    f->r[2]=f->r[1];
    /* 004e7ba9 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004e7bac */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004e7bb1 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004e7bb7 */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004e7bba */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e7bbb */
    a=f->r[7];mi_idiv32(f,a);
    /* 004e7bbd */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004e7bc3 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004e7bc9 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004e7bcc */
    f->r[7]=0x00000001u;
    /* 004e7bd1 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004e7bd7 */
    f->r[6]=f->r[2];
    /* 004e7bd9 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004e7bda */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004e7bdc */
    if(mi_signed_less(f->flags))goto mi_004e7c7c;
    /* 004e7be2 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
mi_004e7be6:;
    /* 004e7be6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7be7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7becu;f->boundary(0x004e7be7u,0x004e7c90u,f);
    /* 004e7bec */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7bef */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e7bf1 */
    if((f->flags&MI_ZF))goto mi_004e7bf8;
    /* 004e7bf3 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004e7bf4 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004e7bf6 */
    if((f->flags&MI_ZF))goto mi_004e7c08;
mi_004e7bf8:;
    /* 004e7bf8 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 004e7bfd */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e7bfe */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e7c00 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7be6;
    /* 004e7c02 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e7c03 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e7c04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e7c05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e7c06 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004e7c07 */
    f->r[4]+=4;return;
mi_004e7c08:;
    /* 004e7c08 */
    f->r[0]=f->r[7];
    /* 004e7c0a */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c0c */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7c0f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e7c11 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c13 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7c16 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a00u+f->r[0]));
    /* 004e7c1d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999feu+f->r[0]));
    /* 004e7c24 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c25 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c26 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7c2bu;f->boundary(0x004e7c26u,0x0047fe60u,f);
    /* 004e7c2b */
    f->r[0]=MI_U32((0x00d3b4e0u));
    /* 004e7c30 */
    f->r[1]=MI_U32((0x00d3b4e4u));
    /* 004e7c36 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004e7c3c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c3e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c3f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c40 */
    a=0x0000006du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c42 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7c48u;f->boundary(0x004e7c43u,0x004b4800u,f);
    /* 004e7c48 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e7c4b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7c4c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e7c4f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e7c51 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004e7c54 */
    MI_U16((0x00bcd652u+f->r[6]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004e7c5b */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000009u);
    /* 004e7c62 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)(0x00000000u);
    /* 004e7c6b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7c70u;f->boundary(0x004e7c6bu,0x004b1df0u,f);
    /* 004e7c70 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7c73 */
    MI_U16((0x00bcd63cu+f->r[6]))=(uint16_t)(0x00000000u);
mi_004e7c7c:;
    /* 004e7c7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e7c7d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e7c7e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e7c7f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e7c80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004e7c81 */
    f->r[4]+=4;return;
}

void restore_event_C_004e7c90(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e7c90 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004e7c94 */
    f->r[0]=f->r[1];
    /* 004e7c96 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7c99 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e7c9b */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e7c9e */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e8u+f->r[0])))&0xffffu)<<0);
    /* 004e7ca5 */
    a=(f->r[1]&0xffffu);b=0x0000003cu;t=mi_alu(f,a,b,16,1);
    /* 004e7ca9 */
    if(mi_signed_less(f->flags))goto mi_004e7cb1;
    /* 004e7cab */
    a=(f->r[1]&0xffffu);b=0x00000040u;t=mi_alu(f,a,b,16,1);
    /* 004e7caf */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e7ce1;
mi_004e7cb1:;
    /* 004e7cb1 */
    a=(f->r[1]&0xffffu);b=0x0000002fu;t=mi_alu(f,a,b,16,1);
    /* 004e7cb5 */
    if((f->flags&MI_ZF))goto mi_004e7ce1;
    /* 004e7cb7 */
    a=(f->r[1]&0xffffu);b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004e7cbc */
    if((f->flags&MI_ZF))goto mi_004e7ce1;
    /* 004e7cbe */
    a=(f->r[1]&0xffffu);b=0x000000deu;t=mi_alu(f,a,b,16,1);
    /* 004e7cc3 */
    if((f->flags&MI_ZF))goto mi_004e7ce1;
    /* 004e7cc5 */
    a=(f->r[1]&0xffffu);b=0x000000e5u;t=mi_alu(f,a,b,16,1);
    /* 004e7cca */
    if((f->flags&MI_ZF))goto mi_004e7ce1;
    /* 004e7ccc */
    a=(f->r[1]&0xffffu);b=0x000000eau;t=mi_alu(f,a,b,16,1);
    /* 004e7cd1 */
    if((f->flags&MI_ZF))goto mi_004e7ce1;
    /* 004e7cd3 */
    a=(f->r[1]&0xffffu);b=0x000000ebu;t=mi_alu(f,a,b,16,1);
    /* 004e7cd8 */
    if((f->flags&MI_ZF))goto mi_004e7ce1;
    /* 004e7cda */
    a=(f->r[1]&0xffffu);b=0x000000ecu;t=mi_alu(f,a,b,16,1);
    /* 004e7cdf */
    if(!(f->flags&MI_ZF))goto mi_004e7d04;
mi_004e7ce1:;
    /* 004e7ce1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a00u+f->r[0]));
    /* 004e7ce8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999feu+f->r[0]));
    /* 004e7cef */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7cf1 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7cf3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7cf4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e7cf5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e7cfau;f->boundary(0x004e7cf5u,0x0047fe60u,f);
    /* 004e7cfa */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e7cfd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e7cff */
    if((f->flags&MI_ZF))goto mi_004e7d04;
    /* 004e7d01 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e7d03 */
    f->r[4]+=4;return;
mi_004e7d04:;
    /* 004e7d04 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e7d06 */
    f->r[4]+=4;return;
}

void restore_event_C_004e85d0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e85d0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e85d5 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004e85d9 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e85df */
    f->r[2]=MI_U32((0x00ea1888u+f->r[0]));
    /* 004e85e5 */
    f->r[0]=(0x00ea1888u+f->r[0]);
    /* 004e85eb */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e85ed */
    f->r[1]=f->r[2];
    /* 004e85ef */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004e85f2 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004e85f4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e85fc;
    /* 004e85f6 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000064u);
mi_004e85fc:;
    /* 004e85fc */
    a=MI_U32((0x00000000u+f->r[0]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 004e85ff */
    if(!mi_signed_less(f->flags))goto mi_004e8607;
    /* 004e8601 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
mi_004e8607:;
    /* 004e8607 */
    f->r[4]+=4;return;
}

void restore_event_C_004e8e80(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e8e80 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[4])))&0xffu)<<0);
    /* 004e8e84 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e8e85 */
    f->r[0]=0x00799ba2u;
    /* 004e8e8a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
mi_004e8e8c:;
    /* 004e8e8c */
    a=MI_U8((0xffffff3eu+f->r[0]));b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004e8e92 */
    if(!(f->flags&MI_ZF))goto mi_004e8eb1;
    /* 004e8e94 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0xffffff43u+f->r[0])))&0xffu)<<0);
    /* 004e8e9a */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e8e9c */
    if((f->flags&MI_ZF))goto mi_004e8eb1;
    /* 004e8e9e */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 004e8ea0 */
    a=(f->r[3]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,0);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e8ea2 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 004e8ea4 */
    if(!(f->flags&MI_SF))goto mi_004e8ea9;
    /* 004e8ea6 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000000u);
mi_004e8ea9:;
    /* 004e8ea9 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000064u;t=mi_alu(f,a,b,8,1);
    /* 004e8eac */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e8eb1;
    /* 004e8eae */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000064u);
mi_004e8eb1:;
    /* 004e8eb1 */
    a=f->r[0];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e8eb6 */
    a=f->r[0];b=0x0089b79au;t=mi_alu(f,a,b,32,1);
    /* 004e8ebb */
    if(mi_signed_less(f->flags))goto mi_004e8e8c;
    /* 004e8ebd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e8ebe */
    f->r[4]+=4;return;
}

void restore_event_C_004eada0(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004eada0 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004eada4 */
    a=f->r[0];b=0xffffffe7u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004eada7 */
    a=f->r[0];b=0x0000004cu;t=mi_alu(f,a,b,32,1);
    /* 004eadaa */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004eadbe;
    /* 004eadac */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004eadae */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004eadccu+f->r[0]),1u))&0xffu)<<0);
    /* 004eadb4 */
    switch(mi_read((0x004eadc4u+f->r[1]*4u),4u)){case 0x004eadbbu:goto mi_004eadbb;case 0x004eadbeu:goto mi_004eadbe;default:abort();}
mi_004eadbb:;
    /* 004eadbb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004eadbd */
    f->r[4]+=4;return;
mi_004eadbe:;
    /* 004eadbe */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004eadc0 */
    f->r[4]+=4;return;
}

void restore_event_C_004eb800(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004eb800 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004eb803 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004eb807 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eb808 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 004eb80c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eb80d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004eb810 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eb811 */
    f->r[6]=MI_U32((0x00000020u+f->r[4]));
    /* 004eb815 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004eb816 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004eb819 */
    f->r[7]=0x00000001u;
    /* 004eb81e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004eb826 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004eb829 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004eb82f */
    f->r[5]=(0x00000000u+f->r[1]+f->r[2]*4u);
    /* 004eb832 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004eb836 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 004eb83a */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004eb83c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004eb840 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004eb844 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 004eb846 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[0]);
mi_004eb848:;
    /* 004eb848 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004eb84a */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 004eb850 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004eb858 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004eb930;
mi_004eb85e:;
    /* 004eb85e */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 004eb860 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004eb862 */
    if(mi_signed_less(f->flags))goto mi_004eb90d;
    /* 004eb868 */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 004eb86a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb86c */
    if(mi_signed_less(f->flags))goto mi_004eb90d;
    /* 004eb872 */
    a=f->r[1];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004eb878 */
    if(!mi_signed_less(f->flags))goto mi_004eb90d;
    /* 004eb87e */
    a=f->r[0];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004eb884 */
    if(!mi_signed_less(f->flags))goto mi_004eb90d;
    /* 004eb88a */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004eb88c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004eb88e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004eb8ee;
    /* 004eb890 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004eb896 */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 004eb89a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[2]*2u));
mi_004eb8a2:;
    /* 004eb8a2 */
    f->r[0]=f->r[2];
    /* 004eb8a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb8a6 */
    if((f->flags&MI_ZF))goto mi_004eb8de;
    /* 004eb8a8 */
    f->r[6]=MI_U32((0x00000000u+f->r[7]));
mi_004eb8aa:;
    /* 004eb8aa */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004eb8ad */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004eb8af */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004eb8b2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004eb8b4 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004eb8b7 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[1])))&0xffu)<<0);
    /* 004eb8bd */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004eb8bf */
    if(!(f->flags&MI_ZF))goto mi_004eb8cb;
    /* 004eb8c1 */
    a=MI_U32((0x0000003cu+f->r[4]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004eb8c5 */
    if(!(f->flags&MI_ZF))goto mi_004ebc78;
mi_004eb8cb:;
    /* 004eb8cb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[1]));
    /* 004eb8d2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb8d4 */
    if(!(f->flags&MI_ZF))goto mi_004eb8aa;
    /* 004eb8d6 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004eb8da */
    f->r[3]=MI_U32((0x00000034u+f->r[4]));
mi_004eb8de:;
    /* 004eb8de */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004eb8df */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004eb8e2 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004eb8e4 */
    if(mi_signed_less(f->flags))goto mi_004eb8a2;
    /* 004eb8e6 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004eb8ea */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
mi_004eb8ee:;
    /* 004eb8ee */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004eb8f3 */
    f->r[0]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004eb8fa */
    a=f->r[0];b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004eb8ff */
    if(!(f->flags&MI_ZF))goto mi_004eb90d;
    /* 004eb901 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004eb905 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb907 */
    if(!(f->flags&MI_ZF))goto mi_004ebc80;
mi_004eb90d:;
    /* 004eb90d */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004eb911 */
    f->r[1]=MI_U32((0x00bc9768u));
    /* 004eb917 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004eb918 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004eb919 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 004eb91f */
    f->r[5]=MI_U32((0x00000000u+f->r[2]));
    /* 004eb921 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004eb922 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004eb924 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004eb928 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 004eb92a */
    if(mi_signed_less(f->flags))goto mi_004eb85e;
mi_004eb930:;
    /* 004eb930 */
    f->r[5]=MI_U32((0x00bc9768u));
    /* 004eb936 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004eb93e */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004eb93f */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 004eb945 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 004eb947 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004eb948 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004eb94a */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 004eb94c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004eba29;
mi_004eb952:;
    /* 004eb952 */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 004eb954 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb956 */
    if(mi_signed_less(f->flags))goto mi_004eba01;
    /* 004eb95c */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004eb95e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004eb960 */
    if(mi_signed_less(f->flags))goto mi_004eba01;
    /* 004eb966 */
    a=f->r[0];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004eb96c */
    if(!mi_signed_less(f->flags))goto mi_004eba01;
    /* 004eb972 */
    a=f->r[1];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004eb978 */
    if(!mi_signed_less(f->flags))goto mi_004eba01;
    /* 004eb97e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004eb980 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004eb982 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004eb9e2;
    /* 004eb984 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004eb98a */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 004eb98e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[2]*2u));
mi_004eb996:;
    /* 004eb996 */
    f->r[0]=f->r[2];
    /* 004eb998 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb99a */
    if((f->flags&MI_ZF))goto mi_004eb9d2;
    /* 004eb99c */
    f->r[6]=MI_U32((0x00000000u+f->r[7]));
mi_004eb99e:;
    /* 004eb99e */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004eb9a1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004eb9a3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004eb9a6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004eb9a8 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004eb9ab */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[1])))&0xffu)<<0);
    /* 004eb9b1 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004eb9b3 */
    if(!(f->flags&MI_ZF))goto mi_004eb9bf;
    /* 004eb9b5 */
    a=MI_U32((0x0000003cu+f->r[4]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004eb9b9 */
    if(!(f->flags&MI_ZF))goto mi_004ebc78;
mi_004eb9bf:;
    /* 004eb9bf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[1]));
    /* 004eb9c6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb9c8 */
    if(!(f->flags&MI_ZF))goto mi_004eb99e;
    /* 004eb9ca */
    f->r[3]=MI_U32((0x00000034u+f->r[4]));
    /* 004eb9ce */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
mi_004eb9d2:;
    /* 004eb9d2 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004eb9d3 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004eb9d6 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004eb9d8 */
    if(mi_signed_less(f->flags))goto mi_004eb996;
    /* 004eb9da */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004eb9de */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
mi_004eb9e2:;
    /* 004eb9e2 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004eb9e7 */
    f->r[0]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004eb9ee */
    a=f->r[0];b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004eb9f3 */
    if(!(f->flags&MI_ZF))goto mi_004eba01;
    /* 004eb9f5 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004eb9f9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eb9fb */
    if(!(f->flags&MI_ZF))goto mi_004ebc80;
mi_004eba01:;
    /* 004eba01 */
    f->r[1]=MI_U32((0x00bc9768u));
    /* 004eba07 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004eba0b */
    a=f->r[1];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004eba11 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004eba12 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 004eba18 */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 004eba1a */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004eba1b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004eba1d */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004eba21 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[5]);
    /* 004eba23 */
    if(mi_signed_less(f->flags))goto mi_004eb952;
mi_004eba29:;
    /* 004eba29 */
    f->r[5]=MI_U32((0x00bc9768u));
    /* 004eba2f */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004eba37 */
    a=f->r[5];b=0x000000e4u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004eba3d */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 004eba43 */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004eba45 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004eba46 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004eba48 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[1]);
    /* 004eba4a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ebb22;
mi_004eba50:;
    /* 004eba50 */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 004eba52 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eba54 */
    if(mi_signed_less(f->flags))goto mi_004ebaff;
    /* 004eba5a */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004eba5c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004eba5e */
    if(mi_signed_less(f->flags))goto mi_004ebaff;
    /* 004eba64 */
    a=f->r[0];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004eba6a */
    if(!mi_signed_less(f->flags))goto mi_004ebaff;
    /* 004eba70 */
    a=f->r[1];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004eba76 */
    if(!mi_signed_less(f->flags))goto mi_004ebaff;
    /* 004eba7c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004eba7e */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004eba80 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ebae0;
    /* 004eba82 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004eba88 */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 004eba8c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[2]*2u));
mi_004eba94:;
    /* 004eba94 */
    f->r[0]=f->r[2];
    /* 004eba96 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004eba98 */
    if((f->flags&MI_ZF))goto mi_004ebad0;
    /* 004eba9a */
    f->r[6]=MI_U32((0x00000000u+f->r[7]));
mi_004eba9c:;
    /* 004eba9c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004eba9f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ebaa1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ebaa4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ebaa6 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ebaa9 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[1])))&0xffu)<<0);
    /* 004ebaaf */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004ebab1 */
    if(!(f->flags&MI_ZF))goto mi_004ebabd;
    /* 004ebab3 */
    a=MI_U32((0x0000003cu+f->r[4]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004ebab7 */
    if(!(f->flags&MI_ZF))goto mi_004ebc78;
mi_004ebabd:;
    /* 004ebabd */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[1]));
    /* 004ebac4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ebac6 */
    if(!(f->flags&MI_ZF))goto mi_004eba9c;
    /* 004ebac8 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004ebacc */
    f->r[3]=MI_U32((0x00000034u+f->r[4]));
mi_004ebad0:;
    /* 004ebad0 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004ebad1 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ebad4 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004ebad6 */
    if(mi_signed_less(f->flags))goto mi_004eba94;
    /* 004ebad8 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004ebadc */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
mi_004ebae0:;
    /* 004ebae0 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004ebae5 */
    f->r[0]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004ebaec */
    a=f->r[0];b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004ebaf1 */
    if(!(f->flags&MI_ZF))goto mi_004ebaff;
    /* 004ebaf3 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004ebaf7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ebaf9 */
    if(!(f->flags&MI_ZF))goto mi_004ebc80;
mi_004ebaff:;
    /* 004ebaff */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004ebb03 */
    f->r[1]=MI_U32((0x00bc9768u));
    /* 004ebb09 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ebb0a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ebb0b */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 004ebb11 */
    f->r[5]=MI_U32((0x00000000u+f->r[2]));
    /* 004ebb13 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004ebb14 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004ebb16 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ebb1a */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 004ebb1c */
    if(mi_signed_less(f->flags))goto mi_004eba50;
mi_004ebb22:;
    /* 004ebb22 */
    f->r[5]=MI_U32((0x00bc9768u));
    /* 004ebb28 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004ebb30 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004ebb31 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 004ebb37 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 004ebb39 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ebb3a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004ebb3c */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 004ebb3e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ebc1b;
mi_004ebb44:;
    /* 004ebb44 */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 004ebb46 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ebb48 */
    if(mi_signed_less(f->flags))goto mi_004ebbf3;
    /* 004ebb4e */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004ebb50 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004ebb52 */
    if(mi_signed_less(f->flags))goto mi_004ebbf3;
    /* 004ebb58 */
    a=f->r[0];b=MI_U32((0x00bca2d0u));t=mi_alu(f,a,b,32,1);
    /* 004ebb5e */
    if(!mi_signed_less(f->flags))goto mi_004ebbf3;
    /* 004ebb64 */
    a=f->r[1];b=MI_U32((0x00bca2d4u));t=mi_alu(f,a,b,32,1);
    /* 004ebb6a */
    if(!mi_signed_less(f->flags))goto mi_004ebbf3;
    /* 004ebb70 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004ebb72 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004ebb74 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ebbd4;
    /* 004ebb76 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 004ebb7c */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 004ebb80 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[2]*2u));
mi_004ebb88:;
    /* 004ebb88 */
    f->r[0]=f->r[2];
    /* 004ebb8a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ebb8c */
    if((f->flags&MI_ZF))goto mi_004ebbc4;
    /* 004ebb8e */
    f->r[6]=MI_U32((0x00000000u+f->r[7]));
mi_004ebb90:;
    /* 004ebb90 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004ebb93 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004ebb95 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ebb98 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ebb9a */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004ebb9d */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[1])))&0xffu)<<0);
    /* 004ebba3 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004ebba5 */
    if(!(f->flags&MI_ZF))goto mi_004ebbb1;
    /* 004ebba7 */
    a=MI_U32((0x0000003cu+f->r[4]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004ebbab */
    if(!(f->flags&MI_ZF))goto mi_004ebc78;
mi_004ebbb1:;
    /* 004ebbb1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[1]));
    /* 004ebbb8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ebbba */
    if(!(f->flags&MI_ZF))goto mi_004ebb90;
    /* 004ebbbc */
    f->r[3]=MI_U32((0x00000034u+f->r[4]));
    /* 004ebbc0 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
mi_004ebbc4:;
    /* 004ebbc4 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004ebbc5 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ebbc8 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004ebbca */
    if(mi_signed_less(f->flags))goto mi_004ebb88;
    /* 004ebbcc */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004ebbd0 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
mi_004ebbd4:;
    /* 004ebbd4 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 004ebbd9 */
    f->r[0]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004ebbe0 */
    a=f->r[0];b=0x00080000u;t=mi_alu(f,a,b,32,3);
    /* 004ebbe5 */
    if(!(f->flags&MI_ZF))goto mi_004ebbf3;
    /* 004ebbe7 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004ebbeb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ebbed */
    if(!(f->flags&MI_ZF))goto mi_004ebc80;
mi_004ebbf3:;
    /* 004ebbf3 */
    f->r[1]=MI_U32((0x00bc9768u));
    /* 004ebbf9 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004ebbfd */
    a=f->r[1];b=0x000000e4u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004ebc03 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ebc04 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[1]);
    /* 004ebc0a */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 004ebc0c */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004ebc0d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004ebc0f */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ebc13 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[5]);
    /* 004ebc15 */
    if(mi_signed_less(f->flags))goto mi_004ebb44;
mi_004ebc1b:;
    /* 004ebc1b */
    f->r[5]=MI_U32((0x00bc9768u));
    /* 004ebc21 */
    f->r[0]=MI_U32((0x00000038u+f->r[4]));
    /* 004ebc25 */
    a=f->r[5];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004ebc2b */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[5]);
    /* 004ebc31 */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004ebc33 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ebc34 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004ebc36 */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[1]);
    /* 004ebc38 */
    if((f->flags&MI_ZF))goto mi_004ebc76;
    /* 004ebc3a */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004ebc3e */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 004ebc42 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 004ebc44 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004ebc45 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004ebc48 */
    a=f->r[5];b=0x000000e5u;t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004ebc4e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ebc4f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004ebc53 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 004ebc55 */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004ebc57 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ebc58 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004ebc5c */
    MI_U32((0x00000000u+f->r[3]))=(uint32_t)(f->r[1]);
    /* 004ebc5e */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 004ebc62 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004ebc63 */
    a=f->r[0];b=0x000000e4u;t=mi_alu(f,a,b,32,1);
    /* 004ebc68 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004ebc6c */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004ebc70 */
    if(mi_signed_less(f->flags))goto mi_004eb848;
mi_004ebc76:;
    /* 004ebc76 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004ebc78:;
    /* 004ebc78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ebc79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ebc7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ebc7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ebc7c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ebc7f */
    f->r[4]+=4;return;
mi_004ebc80:;
    /* 004ebc80 */
    f->r[0]=(0x00000001u+f->r[6]);
    /* 004ebc83 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ebc84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ebc85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ebc86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ebc87 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ebc8a */
    f->r[4]+=4;return;
}

void restore_event_C_00562348(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00562348 */
    f->r[0]=MI_U32((0x00604950u));
    /* 0056234d */
    a=f->r[0];b=0x000343fdu;t=mi_imul(f,a,b);f->r[0]=t;
    /* 00562353 */
    a=f->r[0];b=0x00269ec3u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00562358 */
    MI_U32((0x00604950u))=(uint32_t)(f->r[0]);
    /* 0056235d */
    a=f->r[0];b=0x00000010u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00562360 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00562365 */
    f->r[4]+=4;return;
}

void restore_event_C_0044ba90(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044ba90 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ba91 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044ba92 */
    f->r[7]=f->r[1];
    /* 0044ba94 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044ba96 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0044ba98 */
    MI_U8((0x000048b5u+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 0044ba9e */
    MI_U8((0x000048b4u+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 0044baa4 */
    MI_U8((0x0078a634u))=(uint8_t)((f->r[0]&0xffu));
    /* 0044baa9 */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 0044baab */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044baad */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044bad1;
    /* 0044baaf */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044bab0 */
    f->r[5]=(0x00000004u+f->r[7]);
mi_0044bab3:;
    /* 0044bab3 */
    a=MI_U8((0x00000028u+f->r[5]));b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0044bab7 */
    if(!(f->flags&MI_ZF))goto mi_0044bac6;
    /* 0044bab9 */
    f->r[1]=f->r[5];
    /* 0044babb */
    MI_U32((0x005d9a40u))=(uint32_t)(f->r[6]);
    /* 0044bac1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044bac6u;f->boundary(0x0044bac1u,0x0044c660u,f);
mi_0044bac6:;
    /* 0044bac6 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 0044bac8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0044bac9 */
    a=f->r[5];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0044bacc */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044bace */
    if(mi_signed_less(f->flags))goto mi_0044bab3;
    /* 0044bad0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_0044bad1:;
    /* 0044bad1 */
    f->r[1]=f->r[7];
    /* 0044bad3 */
    MI_U32((0x005d9a40u))=(uint32_t)(0xffffffffu);
    /* 0044badd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044bae2u;f->boundary(0x0044baddu,0x0044bc20u,f);
    /* 0044bae2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044bae3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044bae4 */
    f->r[4]+=4;return;
}

void restore_event_C_0044bc20(struct restore_event_actions_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044bc20 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044bc21 */
    f->r[3]=f->r[1];
    /* 0044bc23 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044bc24 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0044bc26 */
    a=MI_U32((0x00000000u+f->r[3]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 0044bc29 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044bc90;
    /* 0044bc2b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044bc2c */
    f->r[6]=(0x0000002cu+f->r[3]);
mi_0044bc2f:;
    /* 0044bc2f */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 0044bc31 */
    f->r[1]=f->r[0];
    /* 0044bc33 */
    a=f->r[1];t=a>>2u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>1u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 0044bc36 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0044bc39 */
    if((f->flags&MI_ZF))goto mi_0044bc85;
    /* 0044bc3b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0000000au+f->r[6])))&0xffu)<<0);
    /* 0044bc3e */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044bc40 */
    if(!(f->flags&MI_ZF))goto mi_0044bc85;
    /* 0044bc42 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,3);
    /* 0044bc44 */
    if((f->flags&MI_ZF))goto mi_0044bc66;
    /* 0044bc46 */
    a=(f->r[0]&0xffu);b=0x000000fbu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044bc48 */
    f->r[1]=(0xffffffd8u+f->r[6]);
    /* 0044bc4b */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044bc4d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044bc52u;f->boundary(0x0044bc4du,0x00448a90u,f);
    /* 0044bc52 */
    f->r[2]=MI_U32((0x0078d7bcu));
    /* 0044bc58 */
    f->r[1]=MI_U32((0x007848d8u));
    /* 0044bc5e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044bc60 */
    a=MI_U16((0xffffffecu+f->r[6]));b=(f->r[2]&0xffffu);t=mi_alu(f,a,b,16,0);MI_U16((0xffffffecu+f->r[6]))=(uint16_t)(t);
    /* 0044bc64 */
    goto mi_0044bc85;
mi_0044bc66:;
    /* 0044bc66 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 0044bc69 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0044bc6b */
    if((f->flags&MI_ZF))goto mi_0044bc85;
    /* 0044bc6d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffdbu+f->r[6])))&0xffu)<<0);
    /* 0044bc70 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044bc72 */
    MI_U8((0xffffffdbu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0044bc75 */
    if(!(f->flags&MI_ZF))goto mi_0044bc85;
    /* 0044bc77 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 0044bc79 */
    f->r[1]=(0xffffffd8u+f->r[6]);
    /* 0044bc7c */
    a=(f->r[0]&0xffu);b=0x000000fbu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044bc7e */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0044bc80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044bc85u;f->boundary(0x0044bc80u,0x00448a90u,f);
mi_0044bc85:;
    /* 0044bc85 */
    f->r[0]=MI_U32((0x00000000u+f->r[3]));
    /* 0044bc87 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0044bc88 */
    a=f->r[6];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0044bc8b */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044bc8d */
    if(mi_signed_less(f->flags))goto mi_0044bc2f;
    /* 0044bc8f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0044bc90:;
    /* 0044bc90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044bc91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044bc92 */
    f->r[4]+=4;return;
}
