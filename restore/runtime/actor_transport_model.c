#include "city_supply.h"
#include "industry_logistics.h"
#include "granary.h"
#include "storage_transactions.h"
#include "storage_trade.h"
#include "storage_delivery.h"
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_transport.h"
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
static __inline uint32_t mi_alu(struct restore_transport_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_transport_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_transport_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static __inline void mi_idiv32(struct restore_transport_frame *f,uint32_t divisor_bits)
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


static const uint8_t mi_rom_0045efc8[]={0xc9,0xeb,0x45,0x00,0xd0,0xea,0x45,0x00,0xd0,0xea,0x45,0x00,0xd0,0xea,0x45,0x00,0xd0,0xea,0x45,0x00,0xd0,0xea,0x45,0x00,0xd3,0xeb,0x45,0x00,0x42,0xeb,0x45,0x00,0x4f,0xeb,0x45,0x00,0x1b,0xeb,0x45,0x00,0x35,0xeb,0x45,0x00,0xe7,0xeb,0x45,0x00,0x5c,0xeb,0x45,0x00,0xbf,0xeb,0x45,0x00,0xeb,0xea,0x45,0x00,0x97,0xeb,0x45,0x00,0x69,0xeb,0x45,0x00,0xd0,0xea,0x45,0x00,0x0e,0xeb,0x45,0x00,0x8d,0xeb,0x45,0x00,0x76,0xeb,0x45,0x00,0x83,0xeb,0x45,0x00,0x01,0xeb,0x45,0x00,0x28,0xeb,0x45,0x00,0xd0,0xea,0x45,0x00,0xd0,0xea,0x45,0x00,0xb5,0xeb,0x45,0x00,0xa1,0xeb,0x45,0x00,0xab,0xeb,0x45,0x00,0xdd,0xeb,0x45,0x00,0xf4,0xea,0x45,0x00,0xf1,0xeb,0x45,0x00,0xfb,0xeb,0x45,0x00};
static const uint8_t mi_rom_0045f04c[]={0x77,0xec,0x45,0x00,0x4c,0xee,0x45,0x00};
static const uint8_t mi_rom_0045f054[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
static const uint8_t mi_rom_0045f120[]={0x0c,0xed,0x45,0x00,0x13,0xed,0x45,0x00,0x1a,0xed,0x45,0x00,0x21,0xed,0x45,0x00};
static const uint8_t mi_rom_0045f130[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0045f1d0[]={0x8a,0xed,0x45,0x00,0xd5,0xed,0x45,0x00,0xdc,0xed,0x45,0x00,0xe3,0xed,0x45,0x00};
static const uint8_t mi_rom_0045f1e0[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0045f280[]={0x8a,0xed,0x45,0x00,0xd5,0xed,0x45,0x00,0xdc,0xed,0x45,0x00,0xe3,0xed,0x45,0x00};
static const uint8_t mi_rom_0045f290[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0045f330[]={0xae,0xed,0x45,0x00,0xb5,0xed,0x45,0x00,0xbc,0xed,0x45,0x00,0xc3,0xed,0x45,0x00};
static const uint8_t mi_rom_0045f340[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0045f3e0[]={0xee,0xee,0x45,0x00,0xa8,0xef,0x45,0x00,0x02,0xef,0x45,0x00,0x0c,0xef,0x45,0x00,0x16,0xef,0x45,0x00,0x20,0xef,0x45,0x00,0x2a,0xef,0x45,0x00,0x38,0xef,0x45,0x00,0x3f,0xef,0x45,0x00,0x46,0xef,0x45,0x00,0x4d,0xef,0x45,0x00,0x54,0xef,0x45,0x00,0x31,0xef,0x45,0x00,0x77,0xef,0x45,0x00,0x5b,0xef,0x45,0x00,0x62,0xef,0x45,0x00,0x69,0xef,0x45,0x00,0x70,0xef,0x45,0x00,0xa8,0xef,0x45,0x00,0xf8,0xee,0x45,0x00,0x7e,0xef,0x45,0x00,0x85,0xef,0x45,0x00,0x8c,0xef,0x45,0x00,0x93,0xef,0x45,0x00,0x9a,0xef,0x45,0x00,0xa1,0xef,0x45,0x00};
static const uint8_t mi_rom_0045f654[]={0x18,0xf5,0x45,0x00,0x6e,0xf4,0x45,0x00,0x6e,0xf4,0x45,0x00,0x6e,0xf4,0x45,0x00,0x6e,0xf4,0x45,0x00,0x6e,0xf4,0x45,0x00,0x1f,0xf5,0x45,0x00,0xc4,0xf4,0x45,0x00,0xcb,0xf4,0x45,0x00,0xa9,0xf4,0x45,0x00,0xbd,0xf4,0x45,0x00,0x2d,0xf5,0x45,0x00,0xd2,0xf4,0x45,0x00,0x11,0xf5,0x45,0x00,0x81,0xf4,0x45,0x00,0xf5,0xf4,0x45,0x00,0xd9,0xf4,0x45,0x00,0x6e,0xf4,0x45,0x00,0x9f,0xf4,0x45,0x00,0xee,0xf4,0x45,0x00,0xe0,0xf4,0x45,0x00,0xe7,0xf4,0x45,0x00,0x95,0xf4,0x45,0x00,0xb3,0xf4,0x45,0x00,0x6e,0xf4,0x45,0x00,0x6e,0xf4,0x45,0x00,0x0a,0xf5,0x45,0x00,0xfc,0xf4,0x45,0x00,0x03,0xf5,0x45,0x00,0x26,0xf5,0x45,0x00,0x8b,0xf4,0x45,0x00,0x34,0xf5,0x45,0x00,0x3b,0xf5,0x45,0x00};
static const uint8_t mi_rom_0045f6d8[]={0x9a,0xf5,0x45,0x00,0xec,0xf5,0x45,0x00};
static const uint8_t mi_rom_0045f6e0[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
static const uint8_t mi_rom_0045fad0[]={0x79,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0x80,0xf8,0x45,0x00,0x25,0xf8,0x45,0x00,0x2c,0xf8,0x45,0x00,0x07,0xf8,0x45,0x00,0x1b,0xf8,0x45,0x00,0x8e,0xf8,0x45,0x00,0x33,0xf8,0x45,0x00,0x72,0xf8,0x45,0x00,0xdf,0xf7,0x45,0x00,0x56,0xf8,0x45,0x00,0x3a,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0xfd,0xf7,0x45,0x00,0x4f,0xf8,0x45,0x00,0x41,0xf8,0x45,0x00,0x48,0xf8,0x45,0x00,0xf3,0xf7,0x45,0x00,0x11,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0xa3,0xf8,0x45,0x00,0x6b,0xf8,0x45,0x00,0x5d,0xf8,0x45,0x00,0x64,0xf8,0x45,0x00,0x87,0xf8,0x45,0x00,0xe9,0xf7,0x45,0x00,0x95,0xf8,0x45,0x00,0x9c,0xf8,0x45,0x00};
static const uint8_t mi_rom_0045fb54[]={0x1b,0xf9,0x45,0x00,0x22,0xf9,0x45,0x00,0x29,0xf9,0x45,0x00,0x30,0xf9,0x45,0x00};
static const uint8_t mi_rom_0045fb64[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0045fc04[]={0x6a,0xf9,0x45,0x00,0x71,0xf9,0x45,0x00,0x78,0xf9,0x45,0x00,0x7f,0xf9,0x45,0x00};
static const uint8_t mi_rom_0045fc14[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0045fcb4[]={0xcf,0xf9,0x45,0x00,0xd6,0xf9,0x45,0x00,0xdd,0xf9,0x45,0x00,0xe4,0xf9,0x45,0x00};
static const uint8_t mi_rom_0045fcc4[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0045fd64[]={0x1e,0xfa,0x45,0x00,0x25,0xfa,0x45,0x00,0x2c,0xfa,0x45,0x00,0x33,0xfa,0x45,0x00};
static const uint8_t mi_rom_0045fd74[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_00460a78[]={0xde,0x08,0x46,0x00,0xe5,0x08,0x46,0x00,0xec,0x08,0x46,0x00,0xf3,0x08,0x46,0x00};
static const uint8_t mi_rom_00460a88[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_00460b28[]={0x24,0x09,0x46,0x00,0x2b,0x09,0x46,0x00,0x32,0x09,0x46,0x00,0x39,0x09,0x46,0x00};
static const uint8_t mi_rom_00460b38[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_00460bd8[]={0x60,0x09,0x46,0x00,0x67,0x09,0x46,0x00,0x6e,0x09,0x46,0x00,0x75,0x09,0x46,0x00};
static const uint8_t mi_rom_00460be8[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_00460c88[]={0xbe,0x09,0x46,0x00,0xc5,0x09,0x46,0x00,0xcc,0x09,0x46,0x00,0xd3,0x09,0x46,0x00};
static const uint8_t mi_rom_00460c98[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_00460d38[]={0x04,0x0a,0x46,0x00,0x0b,0x0a,0x46,0x00,0x12,0x0a,0x46,0x00,0x19,0x0a,0x46,0x00};
static const uint8_t mi_rom_00460d48[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_00460de8[]={0x40,0x0a,0x46,0x00,0x47,0x0a,0x46,0x00,0x4e,0x0a,0x46,0x00,0x55,0x0a,0x46,0x00};
static const uint8_t mi_rom_00460df8[]={0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x01,0x03,0x03,0x03,0x00,0x03,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x02,0x02,0x02};
static const uint8_t mi_rom_0048b158[]={0x44,0xaf,0x48,0x00,0x4c,0xb0,0x48,0x00};
static const uint8_t mi_rom_0048b160[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
static const uint8_t mi_rom_0048b22c[]={0x66,0xaf,0x48,0x00,0x20,0xb0,0x48,0x00,0x7a,0xaf,0x48,0x00,0x84,0xaf,0x48,0x00,0x8e,0xaf,0x48,0x00,0x98,0xaf,0x48,0x00,0xa2,0xaf,0x48,0x00,0xb0,0xaf,0x48,0x00,0xb7,0xaf,0x48,0x00,0xbe,0xaf,0x48,0x00,0xc5,0xaf,0x48,0x00,0xcc,0xaf,0x48,0x00,0xa9,0xaf,0x48,0x00,0xef,0xaf,0x48,0x00,0xd3,0xaf,0x48,0x00,0xda,0xaf,0x48,0x00,0xe1,0xaf,0x48,0x00,0xe8,0xaf,0x48,0x00,0x20,0xb0,0x48,0x00,0x70,0xaf,0x48,0x00,0xf6,0xaf,0x48,0x00,0xfd,0xaf,0x48,0x00,0x04,0xb0,0x48,0x00,0x0b,0xb0,0x48,0x00,0x12,0xb0,0x48,0x00,0x19,0xb0,0x48,0x00};
static const uint8_t mi_rom_0048b294[]={0x09,0xb1,0x48,0x00,0x00,0xb1,0x48,0x00,0x12,0xb1,0x48,0x00,0x1b,0xb1,0x48,0x00};
static const uint8_t mi_rom_0048b2a4[]={0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x03,0x01};
static const uint8_t mi_rom_0048b408[]={0x35,0xb3,0x48,0x00,0x3b,0xb3,0x48,0x00,0x41,0xb3,0x48,0x00,0x47,0xb3,0x48,0x00,0x4d,0xb3,0x48,0x00,0x53,0xb3,0x48,0x00,0x59,0xb3,0x48,0x00,0x5f,0xb3,0x48,0x00,0x65,0xb3,0x48,0x00,0x6b,0xb3,0x48,0x00,0x71,0xb3,0x48,0x00,0x77,0xb3,0x48,0x00,0x7d,0xb3,0x48,0x00,0x83,0xb3,0x48,0x00,0x89,0xb3,0x48,0x00,0x8f,0xb3,0x48,0x00,0x9b,0xb3,0x48,0x00,0xa1,0xb3,0x48,0x00,0xa7,0xb3,0x48,0x00,0xad,0xb3,0x48,0x00,0xb3,0xb3,0x48,0x00,0xb9,0xb3,0x48,0x00,0xbf,0xb3,0x48,0x00,0xc5,0xb3,0x48,0x00,0xcb,0xb3,0x48,0x00,0xd1,0xb3,0x48,0x00,0x01,0xb4,0x48,0x00,0xdd,0xb3,0x48,0x00,0xe3,0xb3,0x48,0x00,0xe9,0xb3,0x48,0x00,0xfb,0xb3,0x48,0x00,0x95,0xb3,0x48,0x00,0xf5,0xb3,0x48,0x00,0xef,0xb3,0x48,0x00,0xd7,0xb3,0x48,0x00};
static const uint8_t mi_rom_0048b5d8[]={0x08,0xb5,0x48,0x00,0x03,0xb5,0x48,0x00};
static const uint8_t mi_rom_0048b5e0[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048b5ec[]={0xc6,0xb5,0x48,0x00,0x75,0xb5,0x48,0x00};
static const uint8_t mi_rom_0048b5f4[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048b600[]={0xa7,0xb5,0x48,0x00,0xa3,0xb5,0x48,0x00};
static const uint8_t mi_rom_0048b608[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048bf30[]={0x1c,0xb8,0x48,0x00,0x03,0xb8,0x48,0x00,0x35,0xb8,0x48,0x00,0x4e,0xb8,0x48,0x00};
static const uint8_t mi_rom_0048bf40[]={0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x03,0x01};
static const uint8_t mi_rom_0048bfbc[]={0x06,0xbf,0x48,0x00,0xd5,0xbe,0x48,0x00};
static const uint8_t mi_rom_0048bfc4[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048bfd0[]={0x06,0xbf,0x48,0x00,0x02,0xbf,0x48,0x00};
static const uint8_t mi_rom_0048bfd8[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048ccec[]={0xf4,0xca,0x48,0x00,0xfc,0xcb,0x48,0x00};
static const uint8_t mi_rom_0048ccf4[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00,0x00,0x00};
static const uint8_t mi_rom_0048cdc0[]={0x16,0xcb,0x48,0x00,0xd0,0xcb,0x48,0x00,0x2a,0xcb,0x48,0x00,0x34,0xcb,0x48,0x00,0x3e,0xcb,0x48,0x00,0x48,0xcb,0x48,0x00,0x52,0xcb,0x48,0x00,0x60,0xcb,0x48,0x00,0x67,0xcb,0x48,0x00,0x6e,0xcb,0x48,0x00,0x75,0xcb,0x48,0x00,0x7c,0xcb,0x48,0x00,0x59,0xcb,0x48,0x00,0x9f,0xcb,0x48,0x00,0x83,0xcb,0x48,0x00,0x8a,0xcb,0x48,0x00,0x91,0xcb,0x48,0x00,0x98,0xcb,0x48,0x00,0xd0,0xcb,0x48,0x00,0x20,0xcb,0x48,0x00,0xa6,0xcb,0x48,0x00,0xad,0xcb,0x48,0x00,0xb4,0xcb,0x48,0x00,0xbb,0xcb,0x48,0x00,0xc2,0xcb,0x48,0x00,0xc9,0xcb,0x48,0x00};
static const uint8_t mi_rom_0048dde8[]={0xf7,0xd4,0x48,0x00,0xb1,0xd4,0x48,0x00,0x36,0xd5,0x48,0x00,0x60,0xd5,0x48,0x00};
static const uint8_t mi_rom_0048ddf8[]={0x00,0x01,0x01,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x03,0x01};
static const uint8_t mi_rom_0048de74[]={0xef,0xd8,0x48,0x00,0xde,0xd8,0x48,0x00};
static const uint8_t mi_rom_0048de7c[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048de88[]={0xdd,0xda,0x48,0x00,0xcc,0xda,0x48,0x00};
static const uint8_t mi_rom_0048de90[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048de9c[]={0xb8,0xdd,0x48,0x00,0x87,0xdd,0x48,0x00};
static const uint8_t mi_rom_0048dea4[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0048deb0[]={0xb8,0xdd,0x48,0x00,0xb4,0xdd,0x48,0x00};
static const uint8_t mi_rom_0048deb8[]={0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0045efc8u&&width<=132u&&(offset=address-0x0045efc8u)<=132u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045efc8[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f04cu&&width<=8u&&(offset=address-0x0045f04cu)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f04c[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f054u&&width<=202u&&(offset=address-0x0045f054u)<=202u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f054[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f120u&&width<=16u&&(offset=address-0x0045f120u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f120[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f130u&&width<=158u&&(offset=address-0x0045f130u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f130[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f1d0u&&width<=16u&&(offset=address-0x0045f1d0u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f1d0[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f1e0u&&width<=158u&&(offset=address-0x0045f1e0u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f1e0[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f280u&&width<=16u&&(offset=address-0x0045f280u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f280[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f290u&&width<=158u&&(offset=address-0x0045f290u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f290[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f330u&&width<=16u&&(offset=address-0x0045f330u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f330[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f340u&&width<=158u&&(offset=address-0x0045f340u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f340[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f3e0u&&width<=104u&&(offset=address-0x0045f3e0u)<=104u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f3e0[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f654u&&width<=132u&&(offset=address-0x0045f654u)<=132u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f654[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f6d8u&&width<=8u&&(offset=address-0x0045f6d8u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f6d8[offset+k]<<(k*8u);return result;}
 if(address>=0x0045f6e0u&&width<=202u&&(offset=address-0x0045f6e0u)<=202u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045f6e0[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fad0u&&width<=132u&&(offset=address-0x0045fad0u)<=132u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fad0[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fb54u&&width<=16u&&(offset=address-0x0045fb54u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fb54[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fb64u&&width<=158u&&(offset=address-0x0045fb64u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fb64[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fc04u&&width<=16u&&(offset=address-0x0045fc04u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fc04[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fc14u&&width<=158u&&(offset=address-0x0045fc14u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fc14[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fcb4u&&width<=16u&&(offset=address-0x0045fcb4u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fcb4[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fcc4u&&width<=158u&&(offset=address-0x0045fcc4u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fcc4[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fd64u&&width<=16u&&(offset=address-0x0045fd64u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fd64[offset+k]<<(k*8u);return result;}
 if(address>=0x0045fd74u&&width<=158u&&(offset=address-0x0045fd74u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0045fd74[offset+k]<<(k*8u);return result;}
 if(address>=0x00460a78u&&width<=16u&&(offset=address-0x00460a78u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460a78[offset+k]<<(k*8u);return result;}
 if(address>=0x00460a88u&&width<=158u&&(offset=address-0x00460a88u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460a88[offset+k]<<(k*8u);return result;}
 if(address>=0x00460b28u&&width<=16u&&(offset=address-0x00460b28u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460b28[offset+k]<<(k*8u);return result;}
 if(address>=0x00460b38u&&width<=158u&&(offset=address-0x00460b38u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460b38[offset+k]<<(k*8u);return result;}
 if(address>=0x00460bd8u&&width<=16u&&(offset=address-0x00460bd8u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460bd8[offset+k]<<(k*8u);return result;}
 if(address>=0x00460be8u&&width<=158u&&(offset=address-0x00460be8u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460be8[offset+k]<<(k*8u);return result;}
 if(address>=0x00460c88u&&width<=16u&&(offset=address-0x00460c88u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460c88[offset+k]<<(k*8u);return result;}
 if(address>=0x00460c98u&&width<=158u&&(offset=address-0x00460c98u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460c98[offset+k]<<(k*8u);return result;}
 if(address>=0x00460d38u&&width<=16u&&(offset=address-0x00460d38u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460d38[offset+k]<<(k*8u);return result;}
 if(address>=0x00460d48u&&width<=158u&&(offset=address-0x00460d48u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460d48[offset+k]<<(k*8u);return result;}
 if(address>=0x00460de8u&&width<=16u&&(offset=address-0x00460de8u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460de8[offset+k]<<(k*8u);return result;}
 if(address>=0x00460df8u&&width<=158u&&(offset=address-0x00460df8u)<=158u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00460df8[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b158u&&width<=8u&&(offset=address-0x0048b158u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b158[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b160u&&width<=202u&&(offset=address-0x0048b160u)<=202u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b160[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b22cu&&width<=104u&&(offset=address-0x0048b22cu)<=104u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b22c[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b294u&&width<=16u&&(offset=address-0x0048b294u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b294[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b2a4u&&width<=123u&&(offset=address-0x0048b2a4u)<=123u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b2a4[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b408u&&width<=140u&&(offset=address-0x0048b408u)<=140u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b408[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b5d8u&&width<=8u&&(offset=address-0x0048b5d8u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b5d8[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b5e0u&&width<=12u&&(offset=address-0x0048b5e0u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b5e0[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b5ecu&&width<=8u&&(offset=address-0x0048b5ecu)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b5ec[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b5f4u&&width<=12u&&(offset=address-0x0048b5f4u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b5f4[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b600u&&width<=8u&&(offset=address-0x0048b600u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b600[offset+k]<<(k*8u);return result;}
 if(address>=0x0048b608u&&width<=12u&&(offset=address-0x0048b608u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048b608[offset+k]<<(k*8u);return result;}
 if(address>=0x0048bf30u&&width<=16u&&(offset=address-0x0048bf30u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048bf30[offset+k]<<(k*8u);return result;}
 if(address>=0x0048bf40u&&width<=123u&&(offset=address-0x0048bf40u)<=123u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048bf40[offset+k]<<(k*8u);return result;}
 if(address>=0x0048bfbcu&&width<=8u&&(offset=address-0x0048bfbcu)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048bfbc[offset+k]<<(k*8u);return result;}
 if(address>=0x0048bfc4u&&width<=12u&&(offset=address-0x0048bfc4u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048bfc4[offset+k]<<(k*8u);return result;}
 if(address>=0x0048bfd0u&&width<=8u&&(offset=address-0x0048bfd0u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048bfd0[offset+k]<<(k*8u);return result;}
 if(address>=0x0048bfd8u&&width<=12u&&(offset=address-0x0048bfd8u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048bfd8[offset+k]<<(k*8u);return result;}
 if(address>=0x0048ccecu&&width<=8u&&(offset=address-0x0048ccecu)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048ccec[offset+k]<<(k*8u);return result;}
 if(address>=0x0048ccf4u&&width<=202u&&(offset=address-0x0048ccf4u)<=202u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048ccf4[offset+k]<<(k*8u);return result;}
 if(address>=0x0048cdc0u&&width<=104u&&(offset=address-0x0048cdc0u)<=104u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048cdc0[offset+k]<<(k*8u);return result;}
 if(address>=0x0048dde8u&&width<=16u&&(offset=address-0x0048dde8u)<=16u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048dde8[offset+k]<<(k*8u);return result;}
 if(address>=0x0048ddf8u&&width<=123u&&(offset=address-0x0048ddf8u)<=123u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048ddf8[offset+k]<<(k*8u);return result;}
 if(address>=0x0048de74u&&width<=8u&&(offset=address-0x0048de74u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048de74[offset+k]<<(k*8u);return result;}
 if(address>=0x0048de7cu&&width<=12u&&(offset=address-0x0048de7cu)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048de7c[offset+k]<<(k*8u);return result;}
 if(address>=0x0048de88u&&width<=8u&&(offset=address-0x0048de88u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048de88[offset+k]<<(k*8u);return result;}
 if(address>=0x0048de90u&&width<=12u&&(offset=address-0x0048de90u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048de90[offset+k]<<(k*8u);return result;}
 if(address>=0x0048de9cu&&width<=8u&&(offset=address-0x0048de9cu)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048de9c[offset+k]<<(k*8u);return result;}
 if(address>=0x0048dea4u&&width<=12u&&(offset=address-0x0048dea4u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048dea4[offset+k]<<(k*8u);return result;}
 if(address>=0x0048deb0u&&width<=8u&&(offset=address-0x0048deb0u)<=8u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048deb0[offset+k]<<(k*8u);return result;}
 if(address>=0x0048deb8u&&width<=12u&&(offset=address-0x0048deb8u)<=12u-width){result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0048deb8[offset+k]<<(k*8u);return result;}
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_transport_C_0045acf0(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_storage_delivery_destination((int32_t)MI_U32(sp+4u),(uint32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(uint32_t)MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_transport_C_0045b880(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];restore_storage_rank_gathering((int32_t)MI_U32(sp+4u),(int32_t *)(uintptr_t)MI_U32(sp+8u),(int32_t *)(uintptr_t)MI_U32(sp+12u),(int32_t *)(uintptr_t)MI_U32(sp+16u),(int32_t *)(uintptr_t)MI_U32(sp+20u),(int32_t *)(uintptr_t)MI_U32(sp+24u),(int32_t *)(uintptr_t)MI_U32(sp+28u));f->r[4]=sp+4u;
}

void restore_transport_C_0045d510(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_resource_amount((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u));f->r[4]=sp+4u;
}

void restore_transport_C_0045d9e0(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_food_delivery_destination((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_transport_C_0045dcc0(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_granary_gather_destination((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(uint32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_transport_C_0045de80(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_granary_find_gather_source((int32_t)MI_U32(sp+4u));f->r[4]=sp+4u;
}

void restore_transport_C_0045e150(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_granary_take_gathered((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u));f->r[4]=sp+4u;
}

void restore_transport_C_0045e360(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_granary_withdraw((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_transport_C_0045eab0(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_industry_delivery_destination((uint32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(uint32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_transport_C_0045f450(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_industry_producer_destination((uint32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(uint32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_transport_C_0045f7b0(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_industry_city_destination((uint32_t)MI_U32(sp+4u),(uint32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(uint32_t)MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_transport_C_00460890(struct restore_transport_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_industry_deliver((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_transport_C_0048ae50(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048ae50 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae51 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae52 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae53 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 0048ae57 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 0048ae5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048ae5d */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 0048ae60 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae61 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048ae64 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048ae66 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048ae69 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 0048ae70 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd603u+f->r[6])))&0xffu)<<0);
    /* 0048ae76 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 0048ae7d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae7e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae7f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048ae85u;f->boundary(0x0048ae80u,0x0045f7b0u,f);
    /* 0048ae85 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048ae88 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048ae8a */
    if(!(f->flags&MI_ZF))goto mi_0048aee4;
    /* 0048ae8c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd603u+f->r[6])))&0xffu)<<0);
    /* 0048ae92 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae93 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae94 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ae95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048ae9au;f->boundary(0x0048ae95u,0x0045eab0u,f);
    /* 0048ae9a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048ae9d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048ae9f */
    if(!(f->flags&MI_ZF))goto mi_0048aee4;
    /* 0048aea1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048aea3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aea4 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd603u+f->r[6])))&0xffu)<<0);
    /* 0048aeaa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aeab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aeac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aead */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048aeb2u;f->boundary(0x0048aeadu,0x0045acf0u,f);
    /* 0048aeb2 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048aeb5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048aeb7 */
    if(!(f->flags&MI_ZF))goto mi_0048aee4;
    /* 0048aeb9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048aebb */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aebc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00bcd603u+f->r[6])))&0xffu)<<0);
    /* 0048aec2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aec3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aec4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aec5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048aecau;f->boundary(0x0048aec5u,0x0045d9e0u,f);
    /* 0048aeca */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048aecd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048aecf */
    if(!(f->flags&MI_ZF))goto mi_0048aee4;
    /* 0048aed1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd603u+f->r[6])))&0xffu)<<0);
    /* 0048aed7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aed8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aed9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aeda */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aedc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048aee1u;f->boundary(0x0048aedcu,0x0045d9e0u,f);
    /* 0048aee1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048aee4:;
    /* 0048aee4 */
    MI_U16((0x00bcd656u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048aeeb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048aeec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048aeed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048aeee */
    f->r[4]+=4;return;
}

void restore_transport_C_0048aef0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048aef0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aef1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048aef2 */
    f->r[7]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0048aef6 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 0048aef9 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048aefc */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048aefe */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048af01 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[6]),2u);
    /* 0048af08 */
    f->r[1]=f->r[0];
    /* 0048af0a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048af0d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048af0f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 0048af17 */
    a=(f->r[1]&0xffffu);b=0x0000006eu;t=mi_alu(f,a,b,16,1);
    /* 0048af1b */
    if(mi_signed_less(f->flags))goto mi_0048af23;
    /* 0048af1d */
    a=(f->r[1]&0xffffu);b=0x00000072u;t=mi_alu(f,a,b,16,1);
    /* 0048af21 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048af44;
mi_0048af23:;
    /* 0048af23 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0048af26 */
    a=f->r[1];b=0xffffffe0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048af29 */
    a=f->r[1];b=0x000000c9u;t=mi_alu(f,a,b,32,1);
    /* 0048af2f */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b04c;
    /* 0048af35 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048af37 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048b160u+f->r[1]),1u))&0xffu)<<0);
    /* 0048af3d */
    switch(mi_read((0x0048b158u+f->r[2]*4u),4u)){case 0x0048af44u:goto mi_0048af44;case 0x0048b04cu:goto mi_0048b04c;default:abort();}
mi_0048af44:;
    /* 0048af44 */
    f->r[1]=f->r[0];
    /* 0048af46 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048af49 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048af4b */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048af4e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799aa6u+f->r[1]),2u);
    /* 0048af55 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048af56 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0048af59 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b020;
    /* 0048af5f */
    switch(mi_read((0x0048b22cu+f->r[0]*4u),4u)){case 0x0048af66u:goto mi_0048af66;case 0x0048af70u:goto mi_0048af70;case 0x0048af7au:goto mi_0048af7a;case 0x0048af84u:goto mi_0048af84;case 0x0048af8eu:goto mi_0048af8e;case 0x0048af98u:goto mi_0048af98;case 0x0048afa2u:goto mi_0048afa2;case 0x0048afa9u:goto mi_0048afa9;case 0x0048afb0u:goto mi_0048afb0;case 0x0048afb7u:goto mi_0048afb7;case 0x0048afbeu:goto mi_0048afbe;case 0x0048afc5u:goto mi_0048afc5;case 0x0048afccu:goto mi_0048afcc;case 0x0048afd3u:goto mi_0048afd3;case 0x0048afdau:goto mi_0048afda;case 0x0048afe1u:goto mi_0048afe1;case 0x0048afe8u:goto mi_0048afe8;case 0x0048afefu:goto mi_0048afef;case 0x0048aff6u:goto mi_0048aff6;case 0x0048affdu:goto mi_0048affd;case 0x0048b004u:goto mi_0048b004;case 0x0048b00bu:goto mi_0048b00b;case 0x0048b012u:goto mi_0048b012;case 0x0048b019u:goto mi_0048b019;case 0x0048b020u:goto mi_0048b020;default:abort();}
mi_0048af66:;
    /* 0048af66 */
    f->r[0]=0x00000010u;
    /* 0048af6b */
    goto mi_0048b023;
mi_0048af70:;
    /* 0048af70 */
    f->r[0]=0x00000020u;
    /* 0048af75 */
    goto mi_0048b023;
mi_0048af7a:;
    /* 0048af7a */
    f->r[0]=0x00000018u;
    /* 0048af7f */
    goto mi_0048b023;
mi_0048af84:;
    /* 0048af84 */
    f->r[0]=0x00000014u;
    /* 0048af89 */
    goto mi_0048b023;
mi_0048af8e:;
    /* 0048af8e */
    f->r[0]=0x0000000bu;
    /* 0048af93 */
    goto mi_0048b023;
mi_0048af98:;
    /* 0048af98 */
    f->r[0]=0x00000019u;
    /* 0048af9d */
    goto mi_0048b023;
mi_0048afa2:;
    /* 0048afa2 */
    f->r[0]=0x0000000cu;
    /* 0048afa7 */
    goto mi_0048b023;
mi_0048afa9:;
    /* 0048afa9 */
    f->r[0]=0x00000009u;
    /* 0048afae */
    goto mi_0048b023;
mi_0048afb0:;
    /* 0048afb0 */
    f->r[0]=0x0000000au;
    /* 0048afb5 */
    goto mi_0048b023;
mi_0048afb7:;
    /* 0048afb7 */
    f->r[0]=0x0000000eu;
    /* 0048afbc */
    goto mi_0048b023;
mi_0048afbe:;
    /* 0048afbe */
    f->r[0]=0x00000012u;
    /* 0048afc3 */
    goto mi_0048b023;
mi_0048afc5:;
    /* 0048afc5 */
    f->r[0]=0x00000016u;
    /* 0048afca */
    goto mi_0048b023;
mi_0048afcc:;
    /* 0048afcc */
    f->r[0]=0x00000017u;
    /* 0048afd1 */
    goto mi_0048b023;
mi_0048afd3:;
    /* 0048afd3 */
    f->r[0]=0x00000015u;
    /* 0048afd8 */
    goto mi_0048b023;
mi_0048afda:;
    /* 0048afda */
    f->r[0]=0x00000011u;
    /* 0048afdf */
    goto mi_0048b023;
mi_0048afe1:;
    /* 0048afe1 */
    f->r[0]=0x0000001du;
    /* 0048afe6 */
    goto mi_0048b023;
mi_0048afe8:;
    /* 0048afe8 */
    f->r[0]=0x0000001eu;
    /* 0048afed */
    goto mi_0048b023;
mi_0048afef:;
    /* 0048afef */
    f->r[0]=0x0000001cu;
    /* 0048aff4 */
    goto mi_0048b023;
mi_0048aff6:;
    /* 0048aff6 */
    f->r[0]=0x00000002u;
    /* 0048affb */
    goto mi_0048b023;
mi_0048affd:;
    /* 0048affd */
    f->r[0]=0x00000008u;
    /* 0048b002 */
    goto mi_0048b023;
mi_0048b004:;
    /* 0048b004 */
    f->r[0]=0x0000001fu;
    /* 0048b009 */
    goto mi_0048b023;
mi_0048b00b:;
    /* 0048b00b */
    f->r[0]=0x0000000du;
    /* 0048b010 */
    goto mi_0048b023;
mi_0048b012:;
    /* 0048b012 */
    f->r[0]=0x00000021u;
    /* 0048b017 */
    goto mi_0048b023;
mi_0048b019:;
    /* 0048b019 */
    f->r[0]=0x00000022u;
    /* 0048b01e */
    goto mi_0048b023;
mi_0048b020:;
    /* 0048b020 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0048b023:;
    /* 0048b023 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x000000ccu;t=mi_alu(f,a,b,16,1);
    /* 0048b02c */
    if(!(f->flags&MI_ZF))goto mi_0048b043;
    /* 0048b02e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048b030 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[6]),1u))&0xffu)<<0);
    /* 0048b036 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048b038 */
    if(!(f->flags&MI_ZF))goto mi_0048b043;
    /* 0048b03a */
    a=mi_read((0x00799adeu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adeu+f->r[1]))=(uint16_t)(t);
    /* 0048b041 */
    goto mi_0048b075;
mi_0048b043:;
    /* 0048b043 */
    a=mi_read((0x00799adcu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adcu+f->r[1]))=(uint16_t)(t);
    /* 0048b04a */
    goto mi_0048b075;
mi_0048b04c:;
    /* 0048b04c */
    f->r[1]=f->r[0];
    /* 0048b04e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b051 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b053 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b056 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0048b05e */
    if(!(f->flags&MI_ZF))goto mi_0048b075;
    /* 0048b060 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd66cu+f->r[6]),2u))&0xffffu)<<0);
    /* 0048b067 */
    a=mi_read((0x00799adcu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adcu+f->r[1]))=(uint16_t)(t);
    /* 0048b06e */
    a=mi_read((0x00799adeu+f->r[1]),2u);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799adeu+f->r[1]))=(uint16_t)(t);
mi_0048b075:;
    /* 0048b075 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd66cu+f->r[6]),2u))&0xffffu)<<0);
    /* 0048b07c */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0048b080 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0048b083 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048b085 */
    if(!(f->flags&MI_ZF))goto mi_0048b0bd;
    /* 0048b087 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048b089 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x0000000fu);
    /* 0048b090 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048b097 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd624u+f->r[6]),2u))&0xffffu)<<0);
    /* 0048b09e */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048b0a5 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00bcd626u+f->r[6]),2u))&0xffffu)<<0);
    /* 0048b0ac */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048b0b3 */
    MI_U16((0x00bcd66cu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048b0ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048b0bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048b0bc */
    f->r[4]+=4;return;
mi_0048b0bd:;
    /* 0048b0bd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048b0bf */
    MI_U16((0x00bcd66cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048b0c6 */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0048b0ca */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b0cb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b0cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b0d1u;f->boundary(0x0048b0ccu,0x0048ae50u,f);
    /* 0048b0d1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048b0d3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b0d6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048b0d8 */
    if((f->flags&MI_ZF))goto mi_0048b147;
    /* 0048b0da */
    f->r[2]=f->r[0];
    /* 0048b0dc */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048b0df */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048b0e1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[2]*8u),2u);
    /* 0048b0e9 */
    a=f->r[0];b=0xffffffb9u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048b0ec */
    a=f->r[0];b=0x0000007au;t=mi_alu(f,a,b,32,1);
    /* 0048b0ef */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b11b;
    /* 0048b0f1 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048b0f3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048b2a4u+f->r[0]),1u))&0xffu)<<0);
    /* 0048b0f9 */
    switch(mi_read((0x0048b294u+f->r[2]*4u),4u)){case 0x0048b100u:goto mi_0048b100;case 0x0048b109u:goto mi_0048b109;case 0x0048b112u:goto mi_0048b112;case 0x0048b11bu:goto mi_0048b11b;default:abort();}
mi_0048b100:;
    /* 0048b100 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000009u);
    /* 0048b107 */
    goto mi_0048b122;
mi_0048b109:;
    /* 0048b109 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x0000000au);
    /* 0048b110 */
    goto mi_0048b122;
mi_0048b112:;
    /* 0048b112 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000010u);
    /* 0048b119 */
    goto mi_0048b122;
mi_0048b11b:;
    /* 0048b11b */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x0000000bu);
mi_0048b122:;
    /* 0048b122 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00d3b4e4u),2u))&0xffffu)<<0);
    /* 0048b128 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048b12f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00d3b4e0u),2u))&0xffffu)<<0);
    /* 0048b136 */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048b13d */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048b144 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048b145 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048b146 */
    f->r[4]+=4;return;
mi_0048b147:;
    /* 0048b147 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 0048b14e */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048b155 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048b156 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048b157 */
    f->r[4]+=4;return;
}

void restore_transport_C_0048b320(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048b320 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0048b324 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048b325 */
    a=f->r[0];b=0x00000022u;t=mi_alu(f,a,b,32,1);
    /* 0048b328 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b401;
    /* 0048b32e */
    switch(mi_read((0x0048b408u+f->r[0]*4u),4u)){case 0x0048b335u:goto mi_0048b335;case 0x0048b33bu:goto mi_0048b33b;case 0x0048b341u:goto mi_0048b341;case 0x0048b347u:goto mi_0048b347;case 0x0048b34du:goto mi_0048b34d;case 0x0048b353u:goto mi_0048b353;case 0x0048b359u:goto mi_0048b359;case 0x0048b35fu:goto mi_0048b35f;case 0x0048b365u:goto mi_0048b365;case 0x0048b36bu:goto mi_0048b36b;case 0x0048b371u:goto mi_0048b371;case 0x0048b377u:goto mi_0048b377;case 0x0048b37du:goto mi_0048b37d;case 0x0048b383u:goto mi_0048b383;case 0x0048b389u:goto mi_0048b389;case 0x0048b38fu:goto mi_0048b38f;case 0x0048b395u:goto mi_0048b395;case 0x0048b39bu:goto mi_0048b39b;case 0x0048b3a1u:goto mi_0048b3a1;case 0x0048b3a7u:goto mi_0048b3a7;case 0x0048b3adu:goto mi_0048b3ad;case 0x0048b3b3u:goto mi_0048b3b3;case 0x0048b3b9u:goto mi_0048b3b9;case 0x0048b3bfu:goto mi_0048b3bf;case 0x0048b3c5u:goto mi_0048b3c5;case 0x0048b3cbu:goto mi_0048b3cb;case 0x0048b3d1u:goto mi_0048b3d1;case 0x0048b3d7u:goto mi_0048b3d7;case 0x0048b3ddu:goto mi_0048b3dd;case 0x0048b3e3u:goto mi_0048b3e3;case 0x0048b3e9u:goto mi_0048b3e9;case 0x0048b3efu:goto mi_0048b3ef;case 0x0048b3f5u:goto mi_0048b3f5;case 0x0048b3fbu:goto mi_0048b3fb;case 0x0048b401u:goto mi_0048b401;default:abort();}
mi_0048b335:;
    /* 0048b335 */
    f->r[0]=0x00000075u;
    /* 0048b33a */
    f->r[4]+=4;return;
mi_0048b33b:;
    /* 0048b33b */
    f->r[0]=0x00000076u;
    /* 0048b340 */
    f->r[4]+=4;return;
mi_0048b341:;
    /* 0048b341 */
    f->r[0]=0x00000077u;
    /* 0048b346 */
    f->r[4]+=4;return;
mi_0048b347:;
    /* 0048b347 */
    f->r[0]=0x00000078u;
    /* 0048b34c */
    f->r[4]+=4;return;
mi_0048b34d:;
    /* 0048b34d */
    f->r[0]=0x00000079u;
    /* 0048b352 */
    f->r[4]+=4;return;
mi_0048b353:;
    /* 0048b353 */
    f->r[0]=0x0000007au;
    /* 0048b358 */
    f->r[4]+=4;return;
mi_0048b359:;
    /* 0048b359 */
    f->r[0]=0x0000007bu;
    /* 0048b35e */
    f->r[4]+=4;return;
mi_0048b35f:;
    /* 0048b35f */
    f->r[0]=0x0000007cu;
    /* 0048b364 */
    f->r[4]+=4;return;
mi_0048b365:;
    /* 0048b365 */
    f->r[0]=0x0000007du;
    /* 0048b36a */
    f->r[4]+=4;return;
mi_0048b36b:;
    /* 0048b36b */
    f->r[0]=0x0000007eu;
    /* 0048b370 */
    f->r[4]+=4;return;
mi_0048b371:;
    /* 0048b371 */
    f->r[0]=0x0000007fu;
    /* 0048b376 */
    f->r[4]+=4;return;
mi_0048b377:;
    /* 0048b377 */
    f->r[0]=0x00000080u;
    /* 0048b37c */
    f->r[4]+=4;return;
mi_0048b37d:;
    /* 0048b37d */
    f->r[0]=0x00000081u;
    /* 0048b382 */
    f->r[4]+=4;return;
mi_0048b383:;
    /* 0048b383 */
    f->r[0]=0x00000082u;
    /* 0048b388 */
    f->r[4]+=4;return;
mi_0048b389:;
    /* 0048b389 */
    f->r[0]=0x00000083u;
    /* 0048b38e */
    f->r[4]+=4;return;
mi_0048b38f:;
    /* 0048b38f */
    f->r[0]=0x00000084u;
    /* 0048b394 */
    f->r[4]+=4;return;
mi_0048b395:;
    /* 0048b395 */
    f->r[0]=0x000002a3u;
    /* 0048b39a */
    f->r[4]+=4;return;
mi_0048b39b:;
    /* 0048b39b */
    f->r[0]=0x00000085u;
    /* 0048b3a0 */
    f->r[4]+=4;return;
mi_0048b3a1:;
    /* 0048b3a1 */
    f->r[0]=0x00000086u;
    /* 0048b3a6 */
    f->r[4]+=4;return;
mi_0048b3a7:;
    /* 0048b3a7 */
    f->r[0]=0x00000087u;
    /* 0048b3ac */
    f->r[4]+=4;return;
mi_0048b3ad:;
    /* 0048b3ad */
    f->r[0]=0x00000088u;
    /* 0048b3b2 */
    f->r[4]+=4;return;
mi_0048b3b3:;
    /* 0048b3b3 */
    f->r[0]=0x00000089u;
    /* 0048b3b8 */
    f->r[4]+=4;return;
mi_0048b3b9:;
    /* 0048b3b9 */
    f->r[0]=0x0000008au;
    /* 0048b3be */
    f->r[4]+=4;return;
mi_0048b3bf:;
    /* 0048b3bf */
    f->r[0]=0x0000008bu;
    /* 0048b3c4 */
    f->r[4]+=4;return;
mi_0048b3c5:;
    /* 0048b3c5 */
    f->r[0]=0x0000008cu;
    /* 0048b3ca */
    f->r[4]+=4;return;
mi_0048b3cb:;
    /* 0048b3cb */
    f->r[0]=0x0000008du;
    /* 0048b3d0 */
    f->r[4]+=4;return;
mi_0048b3d1:;
    /* 0048b3d1 */
    f->r[0]=0x0000008eu;
    /* 0048b3d6 */
    f->r[4]+=4;return;
mi_0048b3d7:;
    /* 0048b3d7 */
    f->r[0]=0x000002a6u;
    /* 0048b3dc */
    f->r[4]+=4;return;
mi_0048b3dd:;
    /* 0048b3dd */
    f->r[0]=0x00000090u;
    /* 0048b3e2 */
    f->r[4]+=4;return;
mi_0048b3e3:;
    /* 0048b3e3 */
    f->r[0]=0x00000092u;
    /* 0048b3e8 */
    f->r[4]+=4;return;
mi_0048b3e9:;
    /* 0048b3e9 */
    f->r[0]=0x00000093u;
    /* 0048b3ee */
    f->r[4]+=4;return;
mi_0048b3ef:;
    /* 0048b3ef */
    f->r[0]=0x000002a5u;
    /* 0048b3f4 */
    f->r[4]+=4;return;
mi_0048b3f5:;
    /* 0048b3f5 */
    f->r[0]=0x000002a4u;
    /* 0048b3fa */
    f->r[4]+=4;return;
mi_0048b3fb:;
    /* 0048b3fb */
    f->r[0]=0x000002a2u;
    /* 0048b400 */
    f->r[4]+=4;return;
mi_0048b401:;
    /* 0048b401 */
    f->r[0]=0x00000074u;
    /* 0048b406 */
    f->r[4]+=4;return;
}

void restore_transport_C_0048b4a0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048b4a0 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 0048b4a4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b4a5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b4a6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b4a7 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b4aa */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048b4ad */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048b4af */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048b4b2 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd66cu+f->r[6]),2u))&0xffffu)<<0);
    /* 0048b4b9 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0048b4bc */
    if(!(f->flags&MI_ZF))goto mi_0048b4c3;
    /* 0048b4be */
    a=f->r[7];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[7]=t;
    /* 0048b4c1 */
    goto mi_0048b4df;
mi_0048b4c3:;
    /* 0048b4c3 */
    a=(f->r[0]&0xffffu);b=0x000000c8u;t=mi_alu(f,a,b,16,1);
    /* 0048b4c7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0048b4cd;
    /* 0048b4c9 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0048b4cb */
    goto mi_0048b4df;
mi_0048b4cd:;
    /* 0048b4cd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048b4cf */
    a=(f->r[0]&0xffffu);b=0x00000190u;t=mi_alu(f,a,b,16,1);
    /* 0048b4d3 */
    t=(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0048b4d6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0048b4d7 */
    a=f->r[1];b=0xfffffff8u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0048b4da */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b4dd */
    f->r[7]=f->r[1];
mi_0048b4df:;
    /* 0048b4df */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[6]),1u))&0xffu)<<0);
    /* 0048b4e5 */
    f->r[0]=f->r[2];
    /* 0048b4e7 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0048b4ec */
    f->r[5]=(0xffffffe8u+f->r[0]);
    /* 0048b4ef */
    a=f->r[5];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048b4f2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b503;
    /* 0048b4f4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048b4f6 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x0048b5e0u+f->r[5]),1u))&0xffu)<<0);
    /* 0048b4fc */
    switch(mi_read((0x0048b5d8u+f->r[1]*4u),4u)){case 0x0048b503u:goto mi_0048b503;case 0x0048b508u:goto mi_0048b508;default:abort();}
mi_0048b503:;
    /* 0048b503 */
    a=(f->r[2]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048b506 */
    if(!(f->flags&MI_ZF))goto mi_0048b50f;
mi_0048b508:;
    /* 0048b508 */
    a=f->r[7];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0048b50b */
    if((f->flags&MI_ZF))goto mi_0048b50f;
    /* 0048b50d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0048b50f:;
    /* 0048b50f */
    a=mi_read((0x00bcd602u+f->r[6]),1u);b=0x00000026u;t=mi_alu(f,a,b,8,1);
    /* 0048b516 */
    if(!(f->flags&MI_ZF))goto mi_0048b541;
    /* 0048b518 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0048b51e */
    a=(f->r[1]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 0048b521 */
    if((f->flags&MI_ZF))goto mi_0048b561;
    /* 0048b523 */
    a=(f->r[1]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048b526 */
    if((f->flags&MI_ZF))goto mi_0048b561;
    /* 0048b528 */
    a=(f->r[1]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 0048b52b */
    if(!(f->flags&MI_ZF))goto mi_0048b53d;
    /* 0048b52d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd608u+f->r[6]),1u))&0xffu)<<0);
    /* 0048b533 */
    a=(f->r[1]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 0048b536 */
    if((f->flags&MI_ZF))goto mi_0048b561;
    /* 0048b538 */
    a=(f->r[1]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048b53b */
    if((f->flags&MI_ZF))goto mi_0048b561;
mi_0048b53d:;
    /* 0048b53d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0048b53f */
    goto mi_0048b545;
mi_0048b541:;
    /* 0048b541 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0048b543 */
    if(mi_signed_less(f->flags))goto mi_0048b561;
mi_0048b545:;
    /* 0048b545 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b546 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b54bu;f->boundary(0x0048b546u,0x0048b320u,f);
    /* 0048b54b */
    f->r[2]=mi_read((0x00f55bb4u+f->r[0]*8u),4u);
    /* 0048b552 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b555 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048b557 */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0048b55d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048b55e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048b55f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048b560 */
    f->r[4]+=4;return;
mi_0048b561:;
    /* 0048b561 */
    a=f->r[5];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048b564 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b575;
    /* 0048b566 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048b568 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x0048b5f4u+f->r[5]),1u))&0xffu)<<0);
    /* 0048b56e */
    switch(mi_read((0x0048b5ecu+f->r[0]*4u),4u)){case 0x0048b575u:goto mi_0048b575;case 0x0048b5c6u:goto mi_0048b5c6;default:abort();}
mi_0048b575:;
    /* 0048b575 */
    a=(f->r[2]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048b578 */
    if((f->flags&MI_ZF))goto mi_0048b5c6;
    /* 0048b57a */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048b57c */
    if(!(f->flags&MI_ZF))goto mi_0048b5b6;
    /* 0048b57e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[6]),1u))&0xffu)<<0);
    /* 0048b584 */
    f->r[1]=f->r[0];
    /* 0048b586 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0048b58c */
    a=f->r[1];b=0xffffffe8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b58f */
    a=f->r[1];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048b592 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b5a3;
    /* 0048b594 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048b596 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048b608u+f->r[1]),1u))&0xffu)<<0);
    /* 0048b59c */
    switch(mi_read((0x0048b600u+f->r[2]*4u),4u)){case 0x0048b5a3u:goto mi_0048b5a3;case 0x0048b5a7u:goto mi_0048b5a7;default:abort();}
mi_0048b5a3:;
    /* 0048b5a3 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048b5a5 */
    if(!(f->flags&MI_ZF))goto mi_0048b5b6;
mi_0048b5a7:;
    /* 0048b5a7 */
    f->r[0]=mi_read((0x00f5603cu),4u);
    /* 0048b5ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048b5ad */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0048b5b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048b5b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048b5b5 */
    f->r[4]+=4;return;
mi_0048b5b6:;
    /* 0048b5b6 */
    f->r[1]=mi_read((0x00f55f54u),4u);
    /* 0048b5bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048b5bd */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0048b5c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048b5c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048b5c5 */
    f->r[4]+=4;return;
mi_0048b5c6:;
    /* 0048b5c6 */
    f->r[2]=mi_read((0x00f5603cu),4u);
    /* 0048b5cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048b5cd */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0048b5d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048b5d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048b5d5 */
    f->r[4]+=4;return;
}

void restore_transport_C_0048b620(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048b620 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b625 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048b627 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b628 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b629 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b62c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b62d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b630 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b632 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b633 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b636 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[1]),1u))&0xffu)<<0);
    /* 0048b63c */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read((0x00bcd5f9u+f->r[1]),1u))&0xffu)<<0);
    /* 0048b642 */
    f->r[6]=(0x00000000u+f->r[2]*8u);
    /* 0048b649 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0048b64b */
    a=(f->r[3]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0048b64d */
    f->r[2]=(f->r[2]&0xffffff00u)|((((f->r[3]&0xffu))&0xffu)<<0);
    /* 0048b64f */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 0048b655 */
    f->r[7]=mi_read((0x007090ecu+f->r[6]*8u),4u);
    /* 0048b65c */
    a=(f->r[2]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048b65f */
    if((f->flags&MI_CF))goto mi_0048b668;
    /* 0048b661 */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)(0x00000000u);
mi_0048b668:;
    /* 0048b668 */
    MI_U32((0x00bcd778u+f->r[1]))=(uint32_t)(0x00000000u);
    /* 0048b672 */
    f->r[6]=mi_read((0x00bcd618u+f->r[1]),4u);
    /* 0048b678 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048b67a */
    MI_U8((0x00bcd66au+f->r[1]))=(uint8_t)(0x00000001u);
    /* 0048b681 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00a76aecu+f->r[6]),1u))&0xffu)<<0);
    /* 0048b687 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0048b689 */
    f->r[5]=f->r[2];
    /* 0048b68b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[1]),1u))&0xffu)<<0);
    /* 0048b691 */
    a=(f->r[2]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0048b694 */
    if(!(f->flags&MI_ZF))goto mi_0048b6c9;
    /* 0048b696 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b697 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b69cu;f->boundary(0x0048b697u,0x004b17d0u,f);
mi_0048b69c:;
    /* 0048b69c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048b69f:;
    /* 0048b69f */
    f->r[0]=mi_read((0x00d056f8u),4u);
mi_0048b6a4:;
    /* 0048b6a4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b6a7 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b6aa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b6ac */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b6af */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[1]),1u))&0xffu)<<0);
    /* 0048b6b5 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048b6b7 */
    if(mi_signed_less(f->flags))goto mi_0048be2a;
    /* 0048b6bd */
    f->r[6]=(uint32_t)(int32_t)(int8_t)mi_read((0x00bcd60au+f->r[1]),1u);
    /* 0048b6c4 */
    goto mi_0048be2d;
mi_0048b6c9:;
    /* 0048b6c9 */
    a=(f->r[2]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0048b6cc */
    if(!(f->flags&MI_ZF))goto mi_0048b6d6;
    /* 0048b6ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b6cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b6d4u;f->boundary(0x0048b6cfu,0x004b1d50u,f);
    /* 0048b6d4 */
    goto mi_0048b69c;
mi_0048b6d6:;
    /* 0048b6d6 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048b6d9 */
    if(!(f->flags&MI_ZF))goto mi_0048b8db;
    /* 0048b6df */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[1]),2u);
    /* 0048b6e6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b6e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b6ecu;f->boundary(0x0048b6e7u,0x0048b4a0u,f);
    /* 0048b6ec */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00ee0c50u+f->r[6]),1u))&0xffu)<<0);
    /* 0048b6f2 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b6f7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b6fa */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048b6fc */
    if(mi_signed_less(f->flags))goto mi_0048b703;
    /* 0048b6fe */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0048b701 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048b713;
mi_0048b703:;
    /* 0048b703 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b706 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b709 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b70b */
    MI_U8((0x00bcd606u+f->r[1]*4u))=(uint8_t)(0x00000002u);
mi_0048b713:;
    /* 0048b713 */
    f->r[2]=f->r[7];
    /* 0048b715 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048b718 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048b71a */
    a=f->r[2];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048b71d */
    a=mi_read((0x007999d8u+f->r[2]),1u);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0048b724 */
    if(!(f->flags&MI_ZF))goto mi_0048b73c;
    /* 0048b726 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a02u+f->r[2]),2u);
    /* 0048b72d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048b72f */
    if((f->flags&MI_ZF))goto mi_0048b74c;
    /* 0048b731 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8eu+f->r[2]),2u);
    /* 0048b738 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048b73a */
    if((f->flags&MI_ZF))goto mi_0048b74c;
mi_0048b73c:;
    /* 0048b73c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b73f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b742 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b744 */
    MI_U8((0x00bcd606u+f->r[1]*4u))=(uint8_t)(0x00000002u);
mi_0048b74c:;
    /* 0048b74c */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b74f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b752 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b754 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b757 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 0048b75e */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 0048b766 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048b8cf;
    /* 0048b76c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048b76e */
    MI_U32((0x00d05628u))=(uint32_t)(0x00000000u);
    /* 0048b778 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00799a1bu+f->r[2]),1u))&0xffu)<<0);
    /* 0048b77e */
    f->r[6]=f->r[1];
    /* 0048b780 */
    f->r[1]=mi_read((0x00eaac28u),4u);
    /* 0048b786 */
    f->r[2]=f->r[1];
    /* 0048b788 */
    a=f->r[2];b=0x000024ecu;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0048b78e */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048b790 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00ea4096u+f->r[2]*2u),2u))&0xffffu)<<0);
    /* 0048b798 */
    a=(f->r[2]&0xffffu);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0048b79c */
    if((f->flags&MI_ZF))goto mi_0048b7a4;
    /* 0048b79e */
    a=(f->r[2]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 0048b7a2 */
    if(!(f->flags&MI_ZF))goto mi_0048b7a6;
mi_0048b7a4:;
    /* 0048b7a4 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
mi_0048b7a6:;
    /* 0048b7a6 */
    a=f->r[1];b=0x00001276u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 0048b7ac */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b7ae */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0048b7b0 */
    a=mi_read((0x00ea42f8u+f->r[1]*4u),4u);b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048b7b7 */
    if(!(f->flags&MI_ZF))goto mi_0048b7cb;
    /* 0048b7b9 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048b7bb */
    if(!(f->flags&MI_ZF))goto mi_0048b7cb;
    /* 0048b7bd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b7be */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b7c3u;f->boundary(0x0048b7beu,0x004b3250u,f);
    /* 0048b7c3 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b7c8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048b7cb:;
    /* 0048b7cb */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b7cc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b7cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b7d2u;f->boundary(0x0048b7cdu,0x0048ae50u,f);
    /* 0048b7d2 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b7d5 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048b7d7 */
    if((f->flags&MI_ZF))goto mi_0048b894;
    /* 0048b7dd */
    f->r[1]=f->r[0];
    /* 0048b7df */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b7e2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b7e4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[1]*8u),2u);
    /* 0048b7ec */
    a=f->r[0];b=0xffffffb9u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048b7ef */
    a=f->r[0];b=0x0000007au;t=mi_alu(f,a,b,32,1);
    /* 0048b7f2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048b84e;
    /* 0048b7f4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048b7f6 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048bf40u+f->r[0]),1u))&0xffu)<<0);
    /* 0048b7fc */
    switch(mi_read((0x0048bf30u+f->r[2]*4u),4u)){case 0x0048b803u:goto mi_0048b803;case 0x0048b81cu:goto mi_0048b81c;case 0x0048b835u:goto mi_0048b835;case 0x0048b84eu:goto mi_0048b84e;default:abort();}
mi_0048b803:;
    /* 0048b803 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b808 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b80b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b80e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b810 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b813 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000009u);
    /* 0048b81a */
    goto mi_0048b865;
mi_0048b81c:;
    /* 0048b81c */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b821 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b824 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b827 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b829 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b82c */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000au);
    /* 0048b833 */
    goto mi_0048b865;
mi_0048b835:;
    /* 0048b835 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b83a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b83d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b840 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b842 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b845 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000010u);
    /* 0048b84c */
    goto mi_0048b865;
mi_0048b84e:;
    /* 0048b84e */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b853 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b856 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b859 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b85b */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b85e */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000bu);
mi_0048b865:;
    /* 0048b865 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00d3b4e4u),2u))&0xffffu)<<0);
    /* 0048b86c */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)((f->r[6]&0xffffu));
    /* 0048b873 */
    MI_U16((0x00bcd61cu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048b87a */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00d3b4e0u),2u))&0xffffu)<<0);
    /* 0048b881 */
    MI_U16((0x00bcd61eu+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048b888 */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0048b88f */
    goto mi_0048b6a4;
mi_0048b894:;
    /* 0048b894 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b899 */
    f->r[2]=mi_read((0x00d05628u),4u);
    /* 0048b89f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b8a2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b8a5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b8a7 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b8aa */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048b8ac */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)((f->r[6]&0xffffu));
    /* 0048b8b3 */
    if((f->flags&MI_ZF))goto mi_0048b8c8;
    /* 0048b8b5 */
    MI_U8((0x00bcd65eu+f->r[1]))=(uint8_t)(0x00000002u);
    /* 0048b8bc */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0048b8c3 */
    goto mi_0048b6a4;
mi_0048b8c8:;
    /* 0048b8c8 */
    MI_U8((0x00bcd65eu+f->r[1]))=(uint8_t)(0x00000001u);
mi_0048b8cf:;
    /* 0048b8cf */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0048b8d6 */
    goto mi_0048b6a4;
mi_0048b8db:;
    /* 0048b8db */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048b8de */
    if(!(f->flags&MI_ZF))goto mi_0048b990;
    /* 0048b8e4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b8e5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b8eau;f->boundary(0x0048b8e5u,0x0048b4a0u,f);
    /* 0048b8ea */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b8ef */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b8f0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b8f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b8f6u;f->boundary(0x0048b8f1u,0x004a5da0u,f);
    /* 0048b8f6 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b8fb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b8fe */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b901 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b904 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b906 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b909 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[1]),1u))&0xffu)<<0);
    /* 0048b90f */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048b912 */
    if(!(f->flags&MI_ZF))goto mi_0048b91d;
    /* 0048b914 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000cu);
    /* 0048b91b */
    goto mi_0048b968;
mi_0048b91d:;
    /* 0048b91d */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048b920 */
    if(!(f->flags&MI_ZF))goto mi_0048b95c;
    /* 0048b922 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b923 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b928u;f->boundary(0x0048b923u,0x004b3250u,f);
    /* 0048b928 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b92d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b930 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b933 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b936 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b938 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b93b */
    f->r[2]=mi_read((0x00bcd618u+f->r[1]),4u);
    /* 0048b941 */
    a=mi_read((0x00ee0c50u+f->r[2]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0048b948 */
    if((f->flags&MI_ZF))goto mi_0048b951;
mi_0048b94a:;
    /* 0048b94a */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
mi_0048b951:;
    /* 0048b951 */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048b95a */
    goto mi_0048b968;
mi_0048b95c:;
    /* 0048b95c */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048b95f */
    if(!(f->flags&MI_ZF))goto mi_0048b968;
    /* 0048b961 */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000002u);
mi_0048b968:;
    /* 0048b968 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[1]),2u);
    /* 0048b96f */
    f->r[6]=f->r[2];
    /* 0048b971 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048b974 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048b976 */
    a=mi_read((0x007999d8u+f->r[6]*8u),1u);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0048b97e */
    if((f->flags&MI_ZF))goto mi_0048b6a4;
    /* 0048b984 */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000002u);
    /* 0048b98b */
    goto mi_0048b6a4;
mi_0048b990:;
    /* 0048b990 */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048b993 */
    if(!(f->flags&MI_ZF))goto mi_0048ba12;
    /* 0048b995 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b996 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b99bu;f->boundary(0x0048b996u,0x0048b4a0u,f);
    /* 0048b99b */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b9a0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b9a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b9a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b9a7u;f->boundary(0x0048b9a2u,0x004a5da0u,f);
    /* 0048b9a7 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b9ac */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b9af */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b9b2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b9b5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b9b7 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b9ba */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[1]),1u))&0xffu)<<0);
    /* 0048b9c0 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048b9c3 */
    if(!(f->flags&MI_ZF))goto mi_0048b9ce;
    /* 0048b9c5 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000du);
    /* 0048b9cc */
    goto mi_0048b968;
mi_0048b9ce:;
    /* 0048b9ce */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048b9d1 */
    if(!(f->flags&MI_ZF))goto mi_0048ba04;
    /* 0048b9d3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048b9d4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048b9d9u;f->boundary(0x0048b9d4u,0x004b3250u,f);
    /* 0048b9d9 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048b9de */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048b9e1 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048b9e4 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b9e7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048b9e9 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048b9ec */
    f->r[2]=mi_read((0x00bcd618u+f->r[1]),4u);
    /* 0048b9f2 */
    a=mi_read((0x00ee0c50u+f->r[2]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0048b9f9 */
    if((f->flags&MI_ZF))goto mi_0048b951;
    /* 0048b9ff */
    goto mi_0048b94a;
mi_0048ba04:;
    /* 0048ba04 */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048ba07 */
    if(!(f->flags&MI_ZF))goto mi_0048b968;
    /* 0048ba0d */
    goto mi_0048b94a;
mi_0048ba12:;
    /* 0048ba12 */
    a=(f->r[2]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 0048ba15 */
    if(!(f->flags&MI_ZF))goto mi_0048ba53;
    /* 0048ba17 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ba18 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048ba1du;f->boundary(0x0048ba18u,0x0048b4a0u,f);
    /* 0048ba1d */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048ba22 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ba23 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ba24 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048ba29u;f->boundary(0x0048ba24u,0x004a5da0u,f);
    /* 0048ba29 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048ba2e */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048ba31 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048ba34 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048ba37 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048ba39 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048ba3c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[1]),1u))&0xffu)<<0);
    /* 0048ba42 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048ba45 */
    if(!(f->flags&MI_ZF))goto mi_0048ba98;
    /* 0048ba47 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000eu);
    /* 0048ba4e */
    goto mi_0048b6a4;
mi_0048ba53:;
    /* 0048ba53 */
    a=(f->r[2]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 0048ba56 */
    if(!(f->flags&MI_ZF))goto mi_0048bade;
    /* 0048ba5c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ba5d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048ba62u;f->boundary(0x0048ba5du,0x0048b4a0u,f);
    /* 0048ba62 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048ba67 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ba68 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ba69 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048ba6eu;f->boundary(0x0048ba69u,0x004a5da0u,f);
    /* 0048ba6e */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048ba73 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048ba76 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048ba79 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048ba7c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048ba7e */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048ba81 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[1]),1u))&0xffu)<<0);
    /* 0048ba87 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048ba8a */
    if(!(f->flags&MI_ZF))goto mi_0048ba98;
    /* 0048ba8c */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000011u);
    /* 0048ba93 */
    goto mi_0048b6a4;
mi_0048ba98:;
    /* 0048ba98 */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048ba9b */
    if(!(f->flags&MI_ZF))goto mi_0048bd5f;
    /* 0048baa1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048baa2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048baa7u;f->boundary(0x0048baa2u,0x004b3250u,f);
    /* 0048baa7 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048baac */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048baaf */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bab2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bab5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bab7 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048baba */
    f->r[2]=mi_read((0x00bcd618u+f->r[1]),4u);
    /* 0048bac0 */
    a=mi_read((0x00ee0c50u+f->r[2]),1u);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0048bac7 */
    if((f->flags&MI_ZF))goto mi_0048bad0;
    /* 0048bac9 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
mi_0048bad0:;
    /* 0048bad0 */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048bad9 */
    goto mi_0048b6a4;
mi_0048bade:;
    /* 0048bade */
    a=(f->r[2]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048bae1 */
    if(!(f->flags&MI_ZF))goto mi_0048bb81;
    /* 0048bae7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bae8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048baedu;f->boundary(0x0048bae8u,0x0048b4a0u,f);
    /* 0048baed */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048baf2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048baf5 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048baf8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bafb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bafd */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bb00 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 0048bb07 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=0x0000000au;t=mi_alu(f,a,b,16,1);
    /* 0048bb0f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048bb6c;
    /* 0048bb11 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[1]),2u);
    /* 0048bb18 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bb1eu;f->boundary(0x0048bb19u,0x00475b50u,f);
    /* 0048bb1e */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048bb24 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048bb26 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048bb29 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048bb2c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048bb2e */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048bb31 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[0]),2u);
    /* 0048bb38 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048bb3e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb3f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[0]),2u);
    /* 0048bb46 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb47 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048bb49 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd607u+f->r[0]),1u))&0xffu)<<0);
    /* 0048bb4f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb50 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb51 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bb56u;f->boundary(0x0048bb51u,0x0045b2a0u,f);
    /* 0048bb56 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048bb5c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb5d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb5e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb5f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bb64u;f->boundary(0x0048bb5fu,0x0048aef0u,f);
    /* 0048bb64 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048bb67:;
    /* 0048bb67 */
    f->r[0]=mi_read((0x00d056f8u),4u);
mi_0048bb6c:;
    /* 0048bb6c */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bb6f */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048bb72 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048bb74 */
    MI_U8((0x00bcd5f9u+f->r[2]*4u))=(uint8_t)(0x00000000u);
    /* 0048bb7c */
    goto mi_0048b6a4;
mi_0048bb81:;
    /* 0048bb81 */
    a=(f->r[2]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 0048bb84 */
    if(!(f->flags&MI_ZF))goto mi_0048bbe3;
    /* 0048bb86 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bb87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bb8cu;f->boundary(0x0048bb87u,0x0048b4a0u,f);
    /* 0048bb8c */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bb91 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bb94 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bb97 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bb9a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bb9c */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bb9f */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 0048bba6 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0048bbae */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048bb6c;
    /* 0048bbb0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[1]),2u);
    /* 0048bbb7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbb8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048bbba */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[1]),2u);
    /* 0048bbc1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]),1u))&0xffu)<<0);
    /* 0048bbc7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbc9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbca */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbcb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bbd0u;f->boundary(0x0048bbcbu,0x0045e220u,f);
    /* 0048bbd0 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048bbd6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbd7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbd8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbd9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bbdeu;f->boundary(0x0048bbd9u,0x0048aef0u,f);
    /* 0048bbde */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bbe1 */
    goto mi_0048bb67;
mi_0048bbe3:;
    /* 0048bbe3 */
    a=(f->r[2]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 0048bbe6 */
    if(!(f->flags&MI_ZF))goto mi_0048bc36;
    /* 0048bbe8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bbe9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bbeeu;f->boundary(0x0048bbe9u,0x0048b4a0u,f);
    /* 0048bbee */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bbf3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bbf6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bbf9 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bbfc */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bbfe */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bc01 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 0048bc08 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0048bc10 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048bb6c;
    /* 0048bc16 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[1]),2u);
    /* 0048bc1d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048bc1f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]),1u))&0xffu)<<0);
    /* 0048bc25 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc26 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc27 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[1]),2u);
    /* 0048bc2e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc2f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bc34u;f->boundary(0x0048bc2fu,0x0045fe20u,f);
    /* 0048bc34 */
    goto mi_0048bc87;
mi_0048bc36:;
    /* 0048bc36 */
    a=(f->r[2]&0xffu);b=0x00000011u;t=mi_alu(f,a,b,8,1);
    /* 0048bc39 */
    if(!(f->flags&MI_ZF))goto mi_0048bc9d;
    /* 0048bc3b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc3c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bc41u;f->boundary(0x0048bc3cu,0x0048b4a0u,f);
    /* 0048bc41 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bc46 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bc49 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bc4c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bc4f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bc51 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bc54 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 0048bc5b */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0048bc63 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048bb6c;
    /* 0048bc69 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[1]),2u);
    /* 0048bc70 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048bc72 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]),1u))&0xffu)<<0);
    /* 0048bc78 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc79 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc7a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[1]),2u);
    /* 0048bc81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bc87u;f->boundary(0x0048bc82u,0x00460890u,f);
mi_0048bc87:;
    /* 0048bc87 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048bc8d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc8e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc8f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bc90 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bc95u;f->boundary(0x0048bc90u,0x0048aef0u,f);
    /* 0048bc95 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bc98 */
    goto mi_0048bb67;
mi_0048bc9d:;
    /* 0048bc9d */
    a=(f->r[2]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 0048bca0 */
    if(!(f->flags&MI_ZF))goto mi_0048bd74;
    /* 0048bca6 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[1]),2u);
    /* 0048bcad */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bcae */
    f->r[1]=0x00bca350u;
    /* 0048bcb3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bcb8u;f->boundary(0x0048bcb3u,0x00488030u,f);
    /* 0048bcb8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048bcba */
    if((f->flags&MI_ZF))goto mi_0048bd07;
    /* 0048bcbc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bcbd */
    f->r[1]=0x00bca350u;
    /* 0048bcc2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bcc7u;f->boundary(0x0048bcc2u,0x004894a0u,f);
    /* 0048bcc7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048bcc9 */
    if((f->flags&MI_ZF))goto mi_0048bd07;
    /* 0048bccb */
    f->r[0]=mi_read((0x00d386d4u),4u);
    /* 0048bcd0 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0048bcd3 */
    if((f->flags&MI_ZF))goto mi_0048bcdf;
    /* 0048bcd5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0048bcd8 */
    if((f->flags&MI_ZF))goto mi_0048bcdf;
    /* 0048bcda */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0048bcdd */
    if(!(f->flags&MI_ZF))goto mi_0048bd07;
mi_0048bcdf:;
    /* 0048bcdf */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bce4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bce5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bceau;f->boundary(0x0048bce5u,0x004b3250u,f);
    /* 0048bcea */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bcef */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bcf2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bcf5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048bcf6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bcf9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bcfb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048bcfc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048bcfd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048bcfe */
    MI_U8((0x00bcd606u+f->r[1]*4u))=(uint8_t)(0x00000002u);
    /* 0048bd06 */
    f->r[4]+=4;return;
mi_0048bd07:;
    /* 0048bd07 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048bd0d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bd0e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bd13u;f->boundary(0x0048bd0eu,0x0048b4a0u,f);
    /* 0048bd13 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bd18 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bd19 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bd1a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bd1fu;f->boundary(0x0048bd1au,0x004a5da0u,f);
    /* 0048bd1f */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bd24 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bd27 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bd2a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bd2d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bd2f */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bd32 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[1]),1u))&0xffu)<<0);
    /* 0048bd38 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048bd3b */
    if(!(f->flags&MI_ZF))goto mi_0048bd4f;
    /* 0048bd3d */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 0048bd43 */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000002u);
    /* 0048bd4a */
    goto mi_0048b6a4;
mi_0048bd4f:;
    /* 0048bd4f */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048bd52 */
    if(!(f->flags&MI_ZF))goto mi_0048bd5f;
    /* 0048bd54 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bd55 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bd5au;f->boundary(0x0048bd55u,0x004b3250u,f);
    /* 0048bd5a */
    goto mi_0048b69c;
mi_0048bd5f:;
    /* 0048bd5f */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048bd62 */
    if(!(f->flags&MI_ZF))goto mi_0048b6a4;
    /* 0048bd68 */
    MI_U8((0x00bcd606u+f->r[1]))=(uint8_t)(0x00000002u);
    /* 0048bd6f */
    goto mi_0048b6a4;
mi_0048bd74:;
    /* 0048bd74 */
    a=(f->r[2]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 0048bd77 */
    if(!(f->flags&MI_ZF))goto mi_0048bdf8;
    /* 0048bd79 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bd7a */
    MI_U8((0x00bcd5f9u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0048bd81 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bd86u;f->boundary(0x0048bd81u,0x0048b4a0u,f);
    /* 0048bd86 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bd8b */
    f->r[2]=mi_read((0x00e94e00u),4u);
    /* 0048bd91 */
    f->r[6]=mi_read((0x00f55e44u),4u);
    /* 0048bd97 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bd9a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bd9d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bda0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bda2 */
    f->r[2]=(0x00000001u+f->r[6]+f->r[2]*1u);
    /* 0048bda6 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bda9 */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0048bdaf */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(t);
    /* 0048bdb6 */
    a=mi_read((0x00bcd62eu+f->r[1]),2u);b=0x00000064u;t=mi_alu(f,a,b,16,1);
    /* 0048bdbe */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048b6a4;
    /* 0048bdc4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048bdc5 */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048bdce */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048bdd3u;f->boundary(0x0048bdceu,0x004a35e0u,f);
    /* 0048bdd3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048bdd6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048bdd8 */
    if(!(f->flags&MI_ZF))goto mi_0048b69f;
    /* 0048bdde */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048bde3 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048bde6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048bde9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048bdeb */
    MI_U8((0x00bcd630u+f->r[1]*4u))=(uint8_t)(0x00000008u);
    /* 0048bdf3 */
    goto mi_0048b6a4;
mi_0048bdf8:;
    /* 0048bdf8 */
    a=(f->r[2]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0048bdfb */
    if(!(f->flags&MI_ZF))goto mi_0048be1e;
    /* 0048bdfd */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048bdff */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0048be05 */
    f->r[6]=mi_read((0x00bcd778u+f->r[1]),4u);
    /* 0048be0b */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048be0d */
    if((f->flags&MI_ZF))goto mi_0048b6a4;
    /* 0048be13 */
    MI_U32((0x00bcd778u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0048be19 */
    goto mi_0048b6a4;
mi_0048be1e:;
    /* 0048be1e */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
    /* 0048be25 */
    goto mi_0048b6a4;
mi_0048be2a:;
    /* 0048be2a */
    f->r[6]=(uint32_t)(int32_t)(int8_t)(f->r[0]&0xffu);
mi_0048be2d:;
    /* 0048be2d */
    a=f->r[6];b=mi_read((0x00bca2e0u),4u);t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0048be33 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0048be35 */
    MI_U32((0x00e94e00u))=(uint32_t)(f->r[6]);
    /* 0048be3b */
    if(!mi_signed_less(f->flags))goto mi_0048be46;
    /* 0048be3d */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048be40 */
    MI_U32((0x00e94e00u))=(uint32_t)(f->r[6]);
mi_0048be46:;
    /* 0048be46 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[1]),1u))&0xffu)<<0);
    /* 0048be4c */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0048be4e */
    if(!(f->flags&MI_ZF))goto mi_0048be7d;
    /* 0048be50 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd62eu+f->r[1]),2u);
    /* 0048be57 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0048be58 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048be5a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048be5c */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0048be5e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x005d048cu+f->r[0]),1u))&0xffu)<<0);
    /* 0048be64 */
    f->r[0]=mi_read((0x00f55e4cu),4u);
    /* 0048be69 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048be6b */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0048be71 */
    MI_U32((0x00bcd778u+f->r[1]))=(uint32_t)(0x00000000u);
    /* 0048be7b */
    goto mi_0048be9a;
mi_0048be7d:;
    /* 0048be7d */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0048be7f */
    if((f->flags&MI_ZF))goto mi_0048be9a;
    /* 0048be81 */
    f->r[2]=mi_read((0x00f55e44u),4u);
    /* 0048be87 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048be89 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd5f9u+f->r[1]),1u))&0xffu)<<0);
    /* 0048be8f */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*8u);
    /* 0048be92 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048be94 */
    MI_U32((0x00bcd5fcu+f->r[1]))=(uint32_t)(f->r[0]);
mi_0048be9a:;
    /* 0048be9a */
    f->r[0]=mi_read((0x00bcd778u+f->r[1]),4u);
    /* 0048bea0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048bea2 */
    if((f->flags&MI_ZF))goto mi_0048bf2b;
    /* 0048bea8 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048beaa */
    MI_U32((0x00bcd778u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 0048beb0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]),1u))&0xffu)<<0);
    /* 0048beb6 */
    f->r[2]=f->r[0];
    /* 0048beb8 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0048bebe */
    f->r[7]=(0xffffffe8u+f->r[2]);
    /* 0048bec1 */
    a=f->r[7];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048bec4 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048bed5;
    /* 0048bec6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048bec8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048bfc4u+f->r[7]),1u))&0xffu)<<0);
    /* 0048bece */
    switch(mi_read((0x0048bfbcu+f->r[2]*4u),4u)){case 0x0048bed5u:goto mi_0048bed5;case 0x0048bf06u:goto mi_0048bf06;default:abort();}
mi_0048bed5:;
    /* 0048bed5 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048bed7 */
    if((f->flags&MI_ZF))goto mi_0048bf06;
    /* 0048bed9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048bedb */
    if(!(f->flags&MI_ZF))goto mi_0048bf11;
    /* 0048bedd */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[1]),1u))&0xffu)<<0);
    /* 0048bee3 */
    f->r[2]=f->r[0];
    /* 0048bee5 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0048beeb */
    f->r[7]=(0xffffffe8u+f->r[2]);
    /* 0048beee */
    a=f->r[7];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048bef1 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048bf02;
    /* 0048bef3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048bef5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048bfd8u+f->r[7]),1u))&0xffu)<<0);
    /* 0048befb */
    switch(mi_read((0x0048bfd0u+f->r[2]*4u),4u)){case 0x0048bf02u:goto mi_0048bf02;case 0x0048bf06u:goto mi_0048bf06;default:abort();}
mi_0048bf02:;
    /* 0048bf02 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048bf04 */
    if(!(f->flags&MI_ZF))goto mi_0048bf11;
mi_0048bf06:;
    /* 0048bf06 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048bf09 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0048bf0c */
    if(mi_signed_less(f->flags))goto mi_0048bf11;
    /* 0048bf0e */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
mi_0048bf11:;
    /* 0048bf11 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x005dc23cu+f->r[6]*2u),1u))&0xffu)<<0);
    /* 0048bf18 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x005dc23du+f->r[6]*2u),1u))&0xffu)<<0);
    /* 0048bf1f */
    MI_U8((0x00bcd664u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 0048bf25 */
    MI_U8((0x00bcd665u+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
mi_0048bf2b:;
    /* 0048bf2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048bf2c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048bf2d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048bf2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048bf2f */
    f->r[4]+=4;return;
}

void restore_transport_C_0048caa0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048caa0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048caa1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048caa2 */
    f->r[7]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0048caa6 */
    f->r[6]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 0048caa9 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048caac */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048caae */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048cab1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[6]),2u);
    /* 0048cab8 */
    f->r[1]=f->r[0];
    /* 0048caba */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048cabd */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048cabf */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[1]*8u),2u))&0xffffu)<<0);
    /* 0048cac7 */
    a=(f->r[1]&0xffffu);b=0x0000006eu;t=mi_alu(f,a,b,16,1);
    /* 0048cacb */
    if(mi_signed_less(f->flags))goto mi_0048cad3;
    /* 0048cacd */
    a=(f->r[1]&0xffffu);b=0x00000072u;t=mi_alu(f,a,b,16,1);
    /* 0048cad1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048caf4;
mi_0048cad3:;
    /* 0048cad3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 0048cad6 */
    a=f->r[1];b=0xffffffe0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048cad9 */
    a=f->r[1];b=0x000000c9u;t=mi_alu(f,a,b,32,1);
    /* 0048cadf */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048cbfc;
    /* 0048cae5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048cae7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048ccf4u+f->r[1]),1u))&0xffu)<<0);
    /* 0048caed */
    switch(mi_read((0x0048ccecu+f->r[2]*4u),4u)){case 0x0048caf4u:goto mi_0048caf4;case 0x0048cbfcu:goto mi_0048cbfc;default:abort();}
mi_0048caf4:;
    /* 0048caf4 */
    f->r[1]=f->r[0];
    /* 0048caf6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048caf9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048cafb */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048cafe */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799aa6u+f->r[1]),2u);
    /* 0048cb05 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0048cb06 */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0048cb09 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048cbd0;
    /* 0048cb0f */
    switch(mi_read((0x0048cdc0u+f->r[0]*4u),4u)){case 0x0048cb16u:goto mi_0048cb16;case 0x0048cb20u:goto mi_0048cb20;case 0x0048cb2au:goto mi_0048cb2a;case 0x0048cb34u:goto mi_0048cb34;case 0x0048cb3eu:goto mi_0048cb3e;case 0x0048cb48u:goto mi_0048cb48;case 0x0048cb52u:goto mi_0048cb52;case 0x0048cb59u:goto mi_0048cb59;case 0x0048cb60u:goto mi_0048cb60;case 0x0048cb67u:goto mi_0048cb67;case 0x0048cb6eu:goto mi_0048cb6e;case 0x0048cb75u:goto mi_0048cb75;case 0x0048cb7cu:goto mi_0048cb7c;case 0x0048cb83u:goto mi_0048cb83;case 0x0048cb8au:goto mi_0048cb8a;case 0x0048cb91u:goto mi_0048cb91;case 0x0048cb98u:goto mi_0048cb98;case 0x0048cb9fu:goto mi_0048cb9f;case 0x0048cba6u:goto mi_0048cba6;case 0x0048cbadu:goto mi_0048cbad;case 0x0048cbb4u:goto mi_0048cbb4;case 0x0048cbbbu:goto mi_0048cbbb;case 0x0048cbc2u:goto mi_0048cbc2;case 0x0048cbc9u:goto mi_0048cbc9;case 0x0048cbd0u:goto mi_0048cbd0;default:abort();}
mi_0048cb16:;
    /* 0048cb16 */
    f->r[0]=0x00000010u;
    /* 0048cb1b */
    goto mi_0048cbd3;
mi_0048cb20:;
    /* 0048cb20 */
    f->r[0]=0x00000020u;
    /* 0048cb25 */
    goto mi_0048cbd3;
mi_0048cb2a:;
    /* 0048cb2a */
    f->r[0]=0x00000018u;
    /* 0048cb2f */
    goto mi_0048cbd3;
mi_0048cb34:;
    /* 0048cb34 */
    f->r[0]=0x00000014u;
    /* 0048cb39 */
    goto mi_0048cbd3;
mi_0048cb3e:;
    /* 0048cb3e */
    f->r[0]=0x0000000bu;
    /* 0048cb43 */
    goto mi_0048cbd3;
mi_0048cb48:;
    /* 0048cb48 */
    f->r[0]=0x00000019u;
    /* 0048cb4d */
    goto mi_0048cbd3;
mi_0048cb52:;
    /* 0048cb52 */
    f->r[0]=0x0000000cu;
    /* 0048cb57 */
    goto mi_0048cbd3;
mi_0048cb59:;
    /* 0048cb59 */
    f->r[0]=0x00000009u;
    /* 0048cb5e */
    goto mi_0048cbd3;
mi_0048cb60:;
    /* 0048cb60 */
    f->r[0]=0x0000000au;
    /* 0048cb65 */
    goto mi_0048cbd3;
mi_0048cb67:;
    /* 0048cb67 */
    f->r[0]=0x0000000eu;
    /* 0048cb6c */
    goto mi_0048cbd3;
mi_0048cb6e:;
    /* 0048cb6e */
    f->r[0]=0x00000012u;
    /* 0048cb73 */
    goto mi_0048cbd3;
mi_0048cb75:;
    /* 0048cb75 */
    f->r[0]=0x00000016u;
    /* 0048cb7a */
    goto mi_0048cbd3;
mi_0048cb7c:;
    /* 0048cb7c */
    f->r[0]=0x00000017u;
    /* 0048cb81 */
    goto mi_0048cbd3;
mi_0048cb83:;
    /* 0048cb83 */
    f->r[0]=0x00000015u;
    /* 0048cb88 */
    goto mi_0048cbd3;
mi_0048cb8a:;
    /* 0048cb8a */
    f->r[0]=0x00000011u;
    /* 0048cb8f */
    goto mi_0048cbd3;
mi_0048cb91:;
    /* 0048cb91 */
    f->r[0]=0x0000001du;
    /* 0048cb96 */
    goto mi_0048cbd3;
mi_0048cb98:;
    /* 0048cb98 */
    f->r[0]=0x0000001eu;
    /* 0048cb9d */
    goto mi_0048cbd3;
mi_0048cb9f:;
    /* 0048cb9f */
    f->r[0]=0x0000001cu;
    /* 0048cba4 */
    goto mi_0048cbd3;
mi_0048cba6:;
    /* 0048cba6 */
    f->r[0]=0x00000002u;
    /* 0048cbab */
    goto mi_0048cbd3;
mi_0048cbad:;
    /* 0048cbad */
    f->r[0]=0x00000008u;
    /* 0048cbb2 */
    goto mi_0048cbd3;
mi_0048cbb4:;
    /* 0048cbb4 */
    f->r[0]=0x0000001fu;
    /* 0048cbb9 */
    goto mi_0048cbd3;
mi_0048cbbb:;
    /* 0048cbbb */
    f->r[0]=0x0000000du;
    /* 0048cbc0 */
    goto mi_0048cbd3;
mi_0048cbc2:;
    /* 0048cbc2 */
    f->r[0]=0x00000021u;
    /* 0048cbc7 */
    goto mi_0048cbd3;
mi_0048cbc9:;
    /* 0048cbc9 */
    f->r[0]=0x00000022u;
    /* 0048cbce */
    goto mi_0048cbd3;
mi_0048cbd0:;
    /* 0048cbd0 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0048cbd3:;
    /* 0048cbd3 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x000000ccu;t=mi_alu(f,a,b,16,1);
    /* 0048cbdc */
    if(!(f->flags&MI_ZF))goto mi_0048cbf3;
    /* 0048cbde */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048cbe0 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[6]),1u))&0xffu)<<0);
    /* 0048cbe6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0048cbe8 */
    if(!(f->flags&MI_ZF))goto mi_0048cbf3;
    /* 0048cbea */
    a=mi_read((0x00799adeu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adeu+f->r[1]))=(uint16_t)(t);
    /* 0048cbf1 */
    goto mi_0048cc25;
mi_0048cbf3:;
    /* 0048cbf3 */
    a=mi_read((0x00799adcu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adcu+f->r[1]))=(uint16_t)(t);
    /* 0048cbfa */
    goto mi_0048cc25;
mi_0048cbfc:;
    /* 0048cbfc */
    f->r[1]=f->r[0];
    /* 0048cbfe */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048cc01 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048cc03 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048cc06 */
    a=mi_read((0x007999e8u+f->r[1]),2u);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0048cc0e */
    if(!(f->flags&MI_ZF))goto mi_0048cc25;
    /* 0048cc10 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd66cu+f->r[6]),2u))&0xffffu)<<0);
    /* 0048cc17 */
    a=mi_read((0x00799adcu+f->r[1]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00799adcu+f->r[1]))=(uint16_t)(t);
    /* 0048cc1e */
    a=mi_read((0x00799adeu+f->r[1]),2u);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,1);MI_U16((0x00799adeu+f->r[1]))=(uint16_t)(t);
mi_0048cc25:;
    /* 0048cc25 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd66cu+f->r[6]),2u))&0xffffu)<<0);
    /* 0048cc2c */
    f->r[1]=mi_read((0x0000000cu+f->r[4]),4u);
    /* 0048cc30 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0048cc33 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048cc35 */
    if(!(f->flags&MI_ZF))goto mi_0048cc86;
    /* 0048cc37 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00000018u+f->r[4]),1u))&0xffu)<<0);
    /* 0048cc3b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048cc3d */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0048cc3f */
    if((f->flags&MI_ZF))goto mi_0048cc52;
    /* 0048cc41 */
    MI_U8((0x00bcd606u+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0048cc48 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048cc4f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048cc50 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048cc51 */
    f->r[4]+=4;return;
mi_0048cc52:;
    /* 0048cc52 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x0000000bu);
    /* 0048cc59 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048cc60 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00bcd624u+f->r[6]),2u))&0xffffu)<<0);
    /* 0048cc67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048cc68 */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048cc6f */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00bcd626u+f->r[6]),2u))&0xffffu)<<0);
    /* 0048cc76 */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048cc7d */
    MI_U16((0x00bcd66cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048cc84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048cc85 */
    f->r[4]+=4;return;
mi_0048cc86:;
    /* 0048cc86 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048cc88 */
    MI_U16((0x00bcd66cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048cc8f */
    f->r[0]=mi_read((0x00000014u+f->r[4]),4u);
    /* 0048cc93 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cc94 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cc95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048cc9au;f->boundary(0x0048cc95u,0x0048ae50u,f);
    /* 0048cc9a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048cc9d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048cc9f */
    if((f->flags&MI_ZF))goto mi_0048ccd0;
    /* 0048cca1 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00d3b4e4u),2u))&0xffffu)<<0);
    /* 0048cca8 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00d3b4e0u),2u))&0xffffu)<<0);
    /* 0048ccaf */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000009u);
    /* 0048ccb6 */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0048ccbf */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048ccc6 */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048cccd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048ccce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048cccf */
    f->r[4]+=4;return;
mi_0048ccd0:;
    /* 0048ccd0 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x00000008u);
    /* 0048ccd7 */
    MI_U8((0x00bcd65eu+f->r[6]))=(uint8_t)(0x00000002u);
    /* 0048ccde */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0048cce7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048cce8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048cce9 */
    f->r[4]+=4;return;
}

void restore_transport_C_0048ce30(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0048ce30 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048ce36 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0048ce39 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048ce3b */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048ce3e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ce3f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048ce42 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048ce44 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ce45 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048ce48 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ce49 */
    f->r[3]=0x00000001u;
    /* 0048ce4e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd602u+f->r[0]),1u))&0xffu)<<0);
    /* 0048ce54 */
    MI_U8((0x00bcd66au+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 0048ce5a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ce5b */
    f->r[6]=(0x00000000u+f->r[2]*8u);
    /* 0048ce62 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0048ce64 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd5f9u+f->r[0]),1u))&0xffu)<<0);
    /* 0048ce6a */
    a=(f->r[2]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0048ce6c */
    f->r[7]=mi_read((0x007090ecu+f->r[6]*8u),4u);
    /* 0048ce73 */
    a=(f->r[2]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048ce76 */
    MI_U8((0x00bcd5f9u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0048ce7c */
    if((f->flags&MI_CF))goto mi_0048ce85;
    /* 0048ce7e */
    MI_U8((0x00bcd5f9u+f->r[0]))=(uint8_t)(0x00000000u);
mi_0048ce85:;
    /* 0048ce85 */
    MI_U32((0x00bcd778u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0048ce8f */
    f->r[6]=mi_read((0x00bcd618u+f->r[0]),4u);
    /* 0048ce95 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048ce97 */
    f->r[5]=0x00000004u;
    /* 0048ce9c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00a76aecu+f->r[6]),1u))&0xffu)<<0);
    /* 0048cea2 */
    f->r[6]=f->r[2];
    /* 0048cea4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[0]),1u))&0xffu)<<0);
    /* 0048ceaa */
    a=(f->r[2]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0048cead */
    if(!(f->flags&MI_ZF))goto mi_0048ceba;
    /* 0048ceaf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048ceb0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048ceb5u;f->boundary(0x0048ceb0u,0x004b17d0u,f);
    /* 0048ceb5 */
    goto mi_0048da45;
mi_0048ceba:;
    /* 0048ceba */
    a=(f->r[2]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0048cebd */
    if(!(f->flags&MI_ZF))goto mi_0048ceca;
    /* 0048cebf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cec0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048cec5u;f->boundary(0x0048cec0u,0x004b1d50u,f);
    /* 0048cec5 */
    goto mi_0048da45;
mi_0048ceca:;
    /* 0048ceca */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048cecd */
    if(!(f->flags&MI_ZF))goto mi_0048d3cd;
    /* 0048ced3 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[0]),2u);
    /* 0048ceda */
    f->r[7]=f->r[5];
    /* 0048cedc */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0048cedf */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0048cee1 */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0048cee4 */
    a=mi_read((0x007999d8u+f->r[7]),1u);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0048ceea */
    if(!(f->flags&MI_ZF))goto mi_0048cf0d;
    /* 0048ceec */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a02u+f->r[7]),2u);
    /* 0048cef3 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048cef5 */
    if((f->flags&MI_ZF))goto mi_0048cf14;
    /* 0048cef7 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a04u+f->r[7]),2u);
    /* 0048cefe */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048cf00 */
    if((f->flags&MI_ZF))goto mi_0048cf14;
    /* 0048cf02 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8eu+f->r[7]),2u);
    /* 0048cf09 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048cf0b */
    if((f->flags&MI_ZF))goto mi_0048cf14;
mi_0048cf0d:;
    /* 0048cf0d */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
mi_0048cf14:;
    /* 0048cf14 */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(t);
    /* 0048cf1b */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=0x00000002u;t=mi_alu(f,a,b,16,1);
    /* 0048cf23 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048d3b2;
    /* 0048cf29 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999e8u+f->r[7]),2u))&0xffffu)<<0);
    /* 0048cf30 */
    a=(f->r[2]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0048cf34 */
    if((f->flags&MI_ZF))goto mi_0048d1b1;
    /* 0048cf3a */
    a=(f->r[2]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    /* 0048cf3f */
    if((f->flags&MI_ZF))goto mi_0048d1b1;
    /* 0048cf45 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048cf4b */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048cf4d */
    if(!(f->flags&MI_ZF))goto mi_0048d05a;
    /* 0048cf53 */
    f->r[0]=0x00000064u;
    /* 0048cf58 */
    f->r[1]=(0x0000001cu+f->r[4]);
    /* 0048cf5c */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048cf60 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0048cf64 */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 0048cf68 */
    f->r[2]=(0x00000020u+f->r[4]);
    /* 0048cf6c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cf6d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cf6e */
    f->r[0]=(0x0000002cu+f->r[4]);
    /* 0048cf72 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cf73 */
    f->r[1]=(0x0000001cu+f->r[4]);
    /* 0048cf77 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cf78 */
    f->r[2]=(0x00000024u+f->r[4]);
    /* 0048cf7c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cf7d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0048cf7f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cf80 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cf81 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0048cf85 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0048cf89 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0048cf8d */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0048cf91 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048cf96u;f->boundary(0x0048cf91u,0x0045b880u,f);
    /* 0048cf96 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00799a8eu+f->r[7]),2u);
    /* 0048cf9d */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048cfa3 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048cfa6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0048cfa8 */
    if(!(f->flags&MI_ZF))goto mi_0048cfb4;
    /* 0048cfaa */
    f->r[0]=mi_read((0x00000010u+f->r[4]),4u);
    /* 0048cfae */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048cfb0 */
    f->r[3]=f->r[0];
    /* 0048cfb2 */
    if(!(f->flags&MI_ZF))goto mi_0048cfb8;
mi_0048cfb4:;
    /* 0048cfb4 */
    f->r[3]=mi_read((0x00000014u+f->r[4]),4u);
mi_0048cfb8:;
    /* 0048cfb8 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0048cfba */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cfbb */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cfbc */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048cfbd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048cfc2u;f->boundary(0x0048cfbdu,0x0045bb50u,f);
    /* 0048cfc2 */
    f->r[6]=f->r[0];
    /* 0048cfc4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048cfc7 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0048cfc9 */
    if((f->flags&MI_ZF))goto mi_0048d03d;
    /* 0048cfcb */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048cfd1 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048cfd4 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048cfd7 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048cfd9 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048cfdc */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048cfde */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048cfe1 */
    a=mi_read((0x007999e8u+f->r[0]*8u),2u);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0048cfea */
    MI_U8((0x00bcd672u+f->r[2]))=(uint8_t)((f->r[3]&0xffu));
    /* 0048cff0 */
    MI_U16((0x00bcd66cu+f->r[2]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0048cff7 */
    MI_U16((0x00bcd656u+f->r[2]))=(uint16_t)((f->r[6]&0xffffu));
    /* 0048cffe */
    if(!(f->flags&MI_ZF))goto mi_0048d009;
    /* 0048d000 */
    MI_U8((0x00bcd630u+f->r[2]))=(uint8_t)(0x0000000fu);
    /* 0048d007 */
    goto mi_0048d010;
mi_0048d009:;
    /* 0048d009 */
    MI_U8((0x00bcd630u+f->r[2]))=(uint8_t)(0x00000012u);
mi_0048d010:;
    /* 0048d010 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00d3b4e4u),2u))&0xffffu)<<0);
    /* 0048d016 */
    MI_U16((0x00bcd62eu+f->r[2]))=(uint16_t)((f->r[7]&0xffffu));
    /* 0048d01d */
    MI_U16((0x00bcd61cu+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d024 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00d3b4e0u),2u))&0xffffu)<<0);
    /* 0048d02a */
    MI_U16((0x00bcd61eu+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d031 */
    MI_U8((0x00bcd66au+f->r[2]))=(uint8_t)(0x00000003u);
    /* 0048d038 */
    goto mi_0048d3b2;
mi_0048d03d:;
    /* 0048d03d */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d042 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048d043 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048d044 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048d045 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d048 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048d049 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d04c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d04e */
    MI_U8((0x00bcd606u+f->r[1]*4u))=(uint8_t)(0x00000002u);
    /* 0048d056 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d059 */
    f->r[4]+=4;return;
mi_0048d05a:;
    /* 0048d05a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60eu+f->r[0]),2u);
    /* 0048d061 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd60cu+f->r[0]),2u);
    /* 0048d068 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0048d06e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d06f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d070 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d071 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d072 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d077u;f->boundary(0x0048d072u,0x0045f7b0u,f);
    /* 0048d077 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d07a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d07c */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d082 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d087 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d089 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d08a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d08d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d090 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d092 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0048d099 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d09a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d09b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d0a0u;f->boundary(0x0048d09bu,0x0045eab0u,f);
    /* 0048d0a0 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d0a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d0a5 */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d0ab */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d0b0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d0b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d0b3 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d0b6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d0b9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d0bb */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0048d0c2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d0c3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d0c4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d0c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d0cbu;f->boundary(0x0048d0c6u,0x0045d9e0u,f);
    /* 0048d0cb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d0ce */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d0d0 */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d0d6 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d0db */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d0dd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d0de */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d0e1 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d0e4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d0e6 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0048d0ed */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d0ee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d0ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d0f4u;f->boundary(0x0048d0efu,0x0045dcc0u,f);
    /* 0048d0f4 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d0f7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d0f9 */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d0ff */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d104 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d106 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d107 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d10a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d10d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d10f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0048d116 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d117 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d118 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d119 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d11eu;f->boundary(0x0048d119u,0x0045acf0u,f);
    /* 0048d11e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d121 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d123 */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d129 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d12e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d130 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d131 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d134 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d137 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d139 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0048d140 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d141 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d142 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d147u;f->boundary(0x0048d142u,0x0045f450u,f);
    /* 0048d147 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d14a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d14c */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d152 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d158 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d159 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d15eu;f->boundary(0x0048d159u,0x0048b4a0u,f);
    /* 0048d15e */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d164 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d166 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a99u+f->r[7]),1u))&0xffu)<<0);
    /* 0048d16c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d16f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d172 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d175 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048d178 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d17a */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048d17d */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d180 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048d182 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d184 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d18a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007900c2u+f->r[2]+f->r[6]*4u),1u))&0xffu)<<0);
    /* 0048d191 */
    a=(f->r[2]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0048d194 */
    if(!(f->flags&MI_ZF))goto mi_0048d1a2;
    /* 0048d196 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0048d19d */
    goto mi_0048d3b2;
mi_0048d1a2:;
    /* 0048d1a2 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0048d1a5 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0048d1ac */
    goto mi_0048d3b2;
mi_0048d1b1:;
    /* 0048d1b1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d1b7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048d1b9 */
    if(!(f->flags&MI_ZF))goto mi_0048d294;
    /* 0048d1bf */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d1c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d1c5u;f->boundary(0x0048d1c0u,0x0045de80u,f);
    /* 0048d1c5 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0048d1c7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d1ca */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048d1cc */
    if((f->flags&MI_ZF))goto mi_0048d213;
    /* 0048d1ce */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d1d4 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d1d7 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d1da */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048d1dc */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d1df */
    MI_U16((0x00bcd656u+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
mi_0048d1e6:;
    /* 0048d1e6 */
    MI_U8((0x00bcd630u+f->r[2]))=(uint8_t)(0x00000012u);
mi_0048d1ed:;
    /* 0048d1ed */
    MI_U16((0x00bcd62eu+f->r[2]))=(uint16_t)((f->r[6]&0xffffu));
mi_0048d1f4:;
    /* 0048d1f4 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00d3b4e4u),2u))&0xffffu)<<0);
    /* 0048d1fa */
    MI_U16((0x00bcd61cu+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d201 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00d3b4e0u),2u))&0xffffu)<<0);
    /* 0048d207 */
    MI_U16((0x00bcd61eu+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d20e */
    goto mi_0048d3b2;
mi_0048d213:;
    /* 0048d213 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d218 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d21a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d21b */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d21e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d221 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d223 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[1]*4u),1u))&0xffu)<<0);
    /* 0048d22a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d22b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d22c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d231u;f->boundary(0x0048d22cu,0x0045bb50u,f);
    /* 0048d231 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d234 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048d236 */
    if((f->flags&MI_ZF))goto mi_0048d26b;
    /* 0048d238 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d23e */
    f->r[7]=f->r[0];
    /* 0048d240 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0048d243 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d246 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0048d248 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d24b */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048d24d */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d250 */
    a=mi_read((0x007999e8u+f->r[7]*8u),2u);b=0x00000048u;t=mi_alu(f,a,b,16,1);
    /* 0048d259 */
    MI_U16((0x00bcd656u+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d260 */
    if(!(f->flags&MI_ZF))goto mi_0048d1e6;
    /* 0048d262 */
    MI_U8((0x00bcd630u+f->r[2]))=(uint8_t)(0x00000014u);
    /* 0048d269 */
    goto mi_0048d1ed;
mi_0048d26b:;
    /* 0048d26b */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d270 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d271 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d274 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d277 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d279 */
    MI_U8((0x00bcd630u+f->r[1]*4u))=(uint8_t)(0x00000008u);
    /* 0048d281 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d286u;f->boundary(0x0048d281u,0x0048b4a0u,f);
    /* 0048d286 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d28c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d28f */
    goto mi_0048d3b2;
mi_0048d294:;
    /* 0048d294 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0048d299 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d29a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d29b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d29c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d29e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d2a3u;f->boundary(0x0048d29eu,0x0045d9e0u,f);
    /* 0048d2a3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d2a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d2a8 */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d2aa */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d2af */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048d2b1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2b2 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d2b5 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d2b8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048d2ba */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 0048d2c1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2c2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2c3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d2cau;f->boundary(0x0048d2c5u,0x0045acf0u,f);
    /* 0048d2ca */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d2cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d2cf */
    if(!(f->flags&MI_ZF))goto mi_0048d2f7;
    /* 0048d2d1 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d2d6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048d2d8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2d9 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0048d2dc */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d2df */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048d2e1 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[2]*4u),1u))&0xffu)<<0);
    /* 0048d2e8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2e9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2ea */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d2eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d2f0u;f->boundary(0x0048d2ebu,0x0045d9e0u,f);
    /* 0048d2f0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d2f3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d2f5 */
    if((f->flags&MI_ZF))goto mi_0048d324;
mi_0048d2f7:;
    /* 0048d2f7 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d2fd */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d300 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d303 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048d305 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d308 */
    MI_U16((0x00bcd656u+f->r[2]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d30f */
    MI_U8((0x00bcd630u+f->r[2]))=(uint8_t)(0x00000009u);
    /* 0048d316 */
    MI_U16((0x00bcd62eu+f->r[2]))=(uint16_t)(0x00000000u);
    /* 0048d31f */
    goto mi_0048d1f4;
mi_0048d324:;
    /* 0048d324 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d32a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d32b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d330u;f->boundary(0x0048d32bu,0x0048b4a0u,f);
    /* 0048d330 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d336 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d338 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00799a99u+f->r[7]),1u))&0xffu)<<0);
    /* 0048d33e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d341 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d344 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d347 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048d34a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d34c */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048d34f */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d352 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048d354 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d356 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d35c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x007900c2u+f->r[2]+f->r[6]*4u),1u))&0xffu)<<0);
    /* 0048d363 */
    a=(f->r[2]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0048d366 */
    if(!(f->flags&MI_ZF))goto mi_0048d371;
    /* 0048d368 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0048d36f */
    goto mi_0048d3b2;
mi_0048d371:;
    /* 0048d371 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0048d374 */
    if(!(f->flags&MI_ZF))goto mi_0048d37f;
    /* 0048d376 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0048d37d */
    goto mi_0048d3b2;
mi_0048d37f:;
    /* 0048d37f */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999deu+f->r[7]),2u))&0xffffu)<<0);
    /* 0048d386 */
    MI_U16((0x00bcd656u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0048d38d */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 0048d394 */
    MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(0x00000000u);
    /* 0048d39d */
    MI_U16((0x00bcd61cu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0048d3a4 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x007999e0u+f->r[7]),2u))&0xffffu)<<0);
    /* 0048d3ab */
    MI_U16((0x00bcd61eu+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
mi_0048d3b2:;
    /* 0048d3b2 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d3b5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d3b6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d3b9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d3bb */
    MI_U8((0x00bcd5f9u+f->r[0]*4u))=(uint8_t)(0x00000000u);
    /* 0048d3c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d3c8u;f->boundary(0x0048d3c3u,0x0048b4a0u,f);
    /* 0048d3c8 */
    goto mi_0048da45;
mi_0048d3cd:;
    /* 0048d3cd */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048d3d0 */
    if(!(f->flags&MI_ZF))goto mi_0048d42e;
    /* 0048d3d2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d3d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d3d8u;f->boundary(0x0048d3d3u,0x0048b4a0u,f);
    /* 0048d3d8 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d3de */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d3df */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d3e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d3e5u;f->boundary(0x0048d3e0u,0x004a5da0u,f);
    /* 0048d3e5 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d3eb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d3ee */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d3f1 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d3f4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d3f6 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d3f9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d3ff */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048d402 */
    if(!(f->flags&MI_ZF))goto mi_0048d410;
    /* 0048d404 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000au);
    /* 0048d40b */
    goto mi_0048da4e;
mi_0048d410:;
    /* 0048d410 */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048d413 */
    if((f->flags&MI_ZF))goto mi_0048da3f;
mi_0048d419:;
    /* 0048d419 */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048d41c */
    if(!(f->flags&MI_ZF))goto mi_0048da4e;
    /* 0048d422 */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 0048d429 */
    goto mi_0048da4e;
mi_0048d42e:;
    /* 0048d42e */
    a=(f->r[2]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048d431 */
    if(!(f->flags&MI_ZF))goto mi_0048d5b6;
    /* 0048d437 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[0]),2u);
    /* 0048d43e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d43f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d444u;f->boundary(0x0048d43fu,0x00475b50u,f);
    /* 0048d444 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048d44a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d44b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d450u;f->boundary(0x0048d44bu,0x0048b4a0u,f);
    /* 0048d450 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d456 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d459 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d45c */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d45f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d461 */
    a=mi_read((0x00bcd62eu+f->r[0]*4u),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[0]*4u))=(uint16_t)(t);
    /* 0048d469 */
    a=mi_read((0x00bcd62eu+f->r[0]*4u),2u);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048d471 */
    f->r[0]=(0x00bcd62eu+f->r[0]*4u);
    /* 0048d478 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048d5a1;
    /* 0048d47e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d47f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d484u;f->boundary(0x0048d47fu,0x0048b4a0u,f);
    /* 0048d484 */
    f->r[1]=f->r[7];
    /* 0048d486 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d489 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d48c */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d48e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[1]*8u),2u);
    /* 0048d496 */
    a=f->r[0];b=0xffffffb9u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d499 */
    a=f->r[0];b=0x0000007au;t=mi_alu(f,a,b,32,1);
    /* 0048d49c */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048d560;
    /* 0048d4a2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d4a4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048ddf8u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d4aa */
    switch(mi_read((0x0048dde8u+f->r[2]*4u),4u)){case 0x0048d4b1u:goto mi_0048d4b1;case 0x0048d4f7u:goto mi_0048d4f7;case 0x0048d536u:goto mi_0048d536;case 0x0048d560u:goto mi_0048d560;default:abort();}
mi_0048d4b1:;
    /* 0048d4b1 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d4b7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d4b9 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d4bc */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d4bf */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d4c1 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d4c4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[0]),2u);
    /* 0048d4cb */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d4d1 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0048d4d6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4d7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4d8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4d9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4da */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d4dfu;f->boundary(0x0048d4dau,0x0045b2a0u,f);
    /* 0048d4df */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d4e5 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4e7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4e8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4e9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d4ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d4efu;f->boundary(0x0048d4eau,0x0048caa0u,f);
    /* 0048d4ef */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d4f2 */
    goto mi_0048d59b;
mi_0048d4f7:;
    /* 0048d4f7 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d4fd */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d500 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d503 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d505 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048d507 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d50a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[0]),2u);
    /* 0048d511 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d517 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d518 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d51a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d51b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d51c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d521u;f->boundary(0x0048d51cu,0x0045e220u,f);
    /* 0048d521 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048d527 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d529 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d52a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d52b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d52c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d531u;f->boundary(0x0048d52cu,0x0048caa0u,f);
    /* 0048d531 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d534 */
    goto mi_0048d59b;
mi_0048d536:;
    /* 0048d536 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d53c */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d53f */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d542 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d544 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048d546 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d549 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[0]),2u);
    /* 0048d550 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d556 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d557 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d558 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d559 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d55eu;f->boundary(0x0048d559u,0x00460890u,f);
    /* 0048d55e */
    goto mi_0048d588;
mi_0048d560:;
    /* 0048d560 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d566 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d568 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d56b */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d56e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d570 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d573 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d579 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[0]),2u);
    /* 0048d580 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d581 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d582 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d583 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d588u;f->boundary(0x0048d583u,0x0045fe20u,f);
mi_0048d588:;
    /* 0048d588 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d58e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d590 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d591 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d592 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d593 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d598u;f->boundary(0x0048d593u,0x0048caa0u,f);
    /* 0048d598 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048d59b:;
    /* 0048d59b */
    f->r[1]=mi_read((0x00d056f8u),4u);
mi_0048d5a1:;
    /* 0048d5a1 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d5a4 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0048d5a7 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048d5a9 */
    MI_U8((0x00bcd5f9u+f->r[2]*4u))=(uint8_t)(0x00000000u);
    /* 0048d5b1 */
    goto mi_0048da4e;
mi_0048d5b6:;
    /* 0048d5b6 */
    a=(f->r[2]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 0048d5b9 */
    if(!(f->flags&MI_ZF))goto mi_0048d5fc;
    /* 0048d5bb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d5bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d5c1u;f->boundary(0x0048d5bcu,0x0048b4a0u,f);
    /* 0048d5c1 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d5c6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d5c7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d5c8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d5cdu;f->boundary(0x0048d5c8u,0x004a5da0u,f);
    /* 0048d5cd */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d5d3 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d5d6 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d5d9 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d5dc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d5de */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d5e1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d5e7 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048d5ea */
    if(!(f->flags&MI_ZF))goto mi_0048da36;
    /* 0048d5f0 */
    MI_U8((0x00bcd606u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 0048d5f7 */
    goto mi_0048da4e;
mi_0048d5fc:;
    /* 0048d5fc */
    a=(f->r[2]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048d5ff */
    if(!(f->flags&MI_ZF))goto mi_0048d643;
    /* 0048d601 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d602 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d607u;f->boundary(0x0048d602u,0x0048b4a0u,f);
    /* 0048d607 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d60d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d60e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d60f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d614u;f->boundary(0x0048d60fu,0x004a5da0u,f);
    /* 0048d614 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d61a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d61d */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d620 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d623 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d625 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d628 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d62e */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048d631 */
    if(!(f->flags&MI_ZF))goto mi_0048da36;
    /* 0048d637 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x0000000du);
    /* 0048d63e */
    goto mi_0048da4e;
mi_0048d643:;
    /* 0048d643 */
    a=(f->r[2]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 0048d646 */
    if(!(f->flags&MI_ZF))goto mi_0048d6e5;
    /* 0048d64c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[0]),2u);
    /* 0048d653 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[0]),2u);
    /* 0048d65a */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(t);
    /* 0048d661 */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048d668 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048d6d0;
    /* 0048d66a */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d66d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d66e */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048d671 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048d673 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d674 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048d677 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d67cu;f->boundary(0x0048d677u,0x0045e150u,f);
    /* 0048d67c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0078d99cu),1u))&0xffu)<<0);
    /* 0048d682 */
    MI_U16((0x00bcd66cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d689 */
    MI_U8((0x00bcd603u+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 0048d68f */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048d695 */
    MI_U8((0x00bcd630u+f->r[6]))=(uint8_t)(0x0000000eu);
    /* 0048d69c */
    MI_U16((0x00bcd62eu+f->r[6]))=(uint16_t)(0x00000000u);
    /* 0048d6a5 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd624u+f->r[6]),2u))&0xffffu)<<0);
    /* 0048d6ac */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d6ad */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048d6b4 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read((0x00bcd626u+f->r[6]),2u))&0xffffu)<<0);
    /* 0048d6bb */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0048d6c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d6c7u;f->boundary(0x0048d6c2u,0x004b3250u,f);
    /* 0048d6c7 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d6cd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048d6d0:;
    /* 0048d6d0 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d6d3 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d6d6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d6d8 */
    MI_U8((0x00bcd5f9u+f->r[0]*4u))=(uint8_t)(0x00000000u);
    /* 0048d6e0 */
    goto mi_0048da4e;
mi_0048d6e5:;
    /* 0048d6e5 */
    a=(f->r[2]&0xffu);b=0x00000012u;t=mi_alu(f,a,b,8,1);
    /* 0048d6e8 */
    if(!(f->flags&MI_ZF))goto mi_0048d72c;
    /* 0048d6ea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d6eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d6f0u;f->boundary(0x0048d6ebu,0x0048b4a0u,f);
    /* 0048d6f0 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d6f6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d6f7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d6f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d6fdu;f->boundary(0x0048d6f8u,0x004a5da0u,f);
    /* 0048d6fd */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d703 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d706 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d709 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d70c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d70e */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d711 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d717 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048d71a */
    if(!(f->flags&MI_ZF))goto mi_0048da36;
    /* 0048d720 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000013u);
    /* 0048d727 */
    goto mi_0048da4e;
mi_0048d72c:;
    /* 0048d72c */
    a=(f->r[2]&0xffu);b=0x00000013u;t=mi_alu(f,a,b,8,1);
    /* 0048d72f */
    if(!(f->flags&MI_ZF))goto mi_0048d84f;
    /* 0048d735 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d736 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d73bu;f->boundary(0x0048d736u,0x0048b4a0u,f);
    /* 0048d73b */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d741 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d744 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d747 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d74a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d74c */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d74f */
    f->r[6]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[0]),2u);
    /* 0048d756 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[0]),2u);
    /* 0048d75d */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(t);
    /* 0048d764 */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048d76b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048d5a1;
    /* 0048d771 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048d773 */
    a=0x00000190u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d778 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d77e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d77f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d780 */
    f->r[2]=mi_read((0x00eaac28u),4u);
    /* 0048d786 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d787 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d78cu;f->boundary(0x0048d787u,0x0045e360u,f);
    /* 0048d78c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d78f */
    a=f->r[0];b=0x00000190u;t=mi_alu(f,a,b,32,1);
    /* 0048d794 */
    if(!(f->flags&MI_ZF))goto mi_0048d7b2;
    /* 0048d796 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048d79c */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048d79f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d7a2 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d7a4 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d7a7 */
    MI_U16((0x00bcd66cu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048d7b0 */
    goto mi_0048d7e1;
mi_0048d7b2:;
    /* 0048d7b2 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048d7b8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d7ba */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048d7bd */
    if(!(f->flags&MI_ZF))goto mi_0048d7d2;
    /* 0048d7bf */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d7c2 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d7c4 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d7c7 */
    MI_U16((0x00bcd66cu+f->r[1]))=(uint16_t)(0x00000190u);
    /* 0048d7d0 */
    goto mi_0048d7e1;
mi_0048d7d2:;
    /* 0048d7d2 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d7d5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d7d7 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d7da */
    MI_U16((0x00bcd66cu+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
mi_0048d7e1:;
    /* 0048d7e1 */
    a=mi_read((0x00bcd66cu+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0048d7e9 */
    if(!(f->flags&MI_ZF))goto mi_0048d807;
    /* 0048d7eb */
    MI_U8((0x00bcd603u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0048d7f2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d7f3 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
    /* 0048d7fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d7ffu;f->boundary(0x0048d7fau,0x004b3250u,f);
    /* 0048d7ff */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d802 */
    goto mi_0048d59b;
mi_0048d807:;
    /* 0048d807 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[1]),1u))&0xffu)<<0);
    /* 0048d80d */
    MI_U8((0x00bcd603u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 0048d813 */
    f->r[0]=f->r[6];
    /* 0048d815 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d818 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d81a */
    a=mi_read((0x007999e8u+f->r[0]*8u),2u);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    /* 0048d823 */
    if(!(f->flags&MI_ZF))goto mi_0048d83a;
    /* 0048d825 */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000eu);
    /* 0048d82c */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048d835 */
    goto mi_0048db78;
mi_0048d83a:;
    /* 0048d83a */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000011u);
    /* 0048d841 */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048d84a */
    goto mi_0048db78;
mi_0048d84f:;
    /* 0048d84f */
    a=(f->r[2]&0xffu);b=0x00000014u;t=mi_alu(f,a,b,8,1);
    /* 0048d852 */
    if(!(f->flags&MI_ZF))goto mi_0048d895;
    /* 0048d854 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d855 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d85au;f->boundary(0x0048d855u,0x0048b4a0u,f);
    /* 0048d85a */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d85f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d860 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d861 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d866u;f->boundary(0x0048d861u,0x004a5da0u,f);
    /* 0048d866 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d86c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d86f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d872 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d875 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d877 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d87a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d880 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048d883 */
    if(!(f->flags&MI_ZF))goto mi_0048da36;
    /* 0048d889 */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000015u);
    /* 0048d890 */
    goto mi_0048da4e;
mi_0048d895:;
    /* 0048d895 */
    a=(f->r[2]&0xffu);b=0x00000015u;t=mi_alu(f,a,b,8,1);
    /* 0048d898 */
    if(!(f->flags&MI_ZF))goto mi_0048d994;
    /* 0048d89e */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[0]),2u);
    /* 0048d8a5 */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(t);
    /* 0048d8ac */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048d8b3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048d5a1;
    /* 0048d8b9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d8bf */
    f->r[1]=f->r[0];
    /* 0048d8c1 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0048d8c7 */
    f->r[6]=(0xffffffe8u+f->r[1]);
    /* 0048d8ca */
    a=f->r[6];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048d8cd */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048d8de;
    /* 0048d8cf */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d8d1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048de7cu+f->r[6]),1u))&0xffu)<<0);
    /* 0048d8d7 */
    switch(mi_read((0x0048de74u+f->r[2]*4u),4u)){case 0x0048d8deu:goto mi_0048d8de;case 0x0048d8efu:goto mi_0048d8ef;default:abort();}
mi_0048d8de:;
    /* 0048d8de */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048d8e0 */
    if((f->flags&MI_ZF))goto mi_0048d8ef;
    /* 0048d8e2 */
    a=(f->r[0]&0xffu);b=0x0000001cu;t=mi_alu(f,a,b,8,1);
    /* 0048d8e4 */
    if((f->flags&MI_ZF))goto mi_0048d8ef;
    /* 0048d8e6 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048d8e8 */
    f->r[6]=0x00000190u;
    /* 0048d8ed */
    if(!(f->flags&MI_ZF))goto mi_0048d8f4;
mi_0048d8ef:;
    /* 0048d8ef */
    f->r[6]=0x00000064u;
mi_0048d8f4:;
    /* 0048d8f4 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0048d8f9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d8fa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d8fb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d8fc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d8fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d902u;f->boundary(0x0048d8fdu,0x0045b570u,f);
    /* 0048d902 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d905 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048d907 */
    if(!(f->flags&MI_ZF))goto mi_0048d925;
    /* 0048d909 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048d90f */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048d912 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d915 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d917 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d91a */
    MI_U16((0x00bcd66cu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048d923 */
    goto mi_0048d94d;
mi_0048d925:;
    /* 0048d925 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048d92b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048d92d */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048d930 */
    if(!(f->flags&MI_ZF))goto mi_0048d93c;
    /* 0048d932 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d935 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d937 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d93a */
    goto mi_0048d946;
mi_0048d93c:;
    /* 0048d93c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d93f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048d941 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048d944 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
mi_0048d946:;
    /* 0048d946 */
    MI_U16((0x00bcd66cu+f->r[1]))=(uint16_t)((f->r[6]&0xffffu));
mi_0048d94d:;
    /* 0048d94d */
    a=mi_read((0x00bcd66cu+f->r[1]),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0048d955 */
    if(!(f->flags&MI_ZF))goto mi_0048d973;
    /* 0048d957 */
    MI_U8((0x00bcd603u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0048d95e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d95f */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
    /* 0048d966 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d96bu;f->boundary(0x0048d966u,0x004b3250u,f);
    /* 0048d96b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d96e */
    goto mi_0048d59b;
mi_0048d973:;
    /* 0048d973 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[1]),1u))&0xffu)<<0);
    /* 0048d979 */
    MI_U8((0x00bcd603u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 0048d97f */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x0000000eu);
    /* 0048d986 */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)(0x00000000u);
    /* 0048d98f */
    goto mi_0048db78;
mi_0048d994:;
    /* 0048d994 */
    a=(f->r[2]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 0048d997 */
    if(!(f->flags&MI_ZF))goto mi_0048d9ef;
    /* 0048d999 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[0]),2u);
    /* 0048d9a0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d9a6u;f->boundary(0x0048d9a1u,0x0048b4a0u,f);
    /* 0048d9a6 */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048d9ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9ac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d9b2u;f->boundary(0x0048d9adu,0x004a5da0u,f);
    /* 0048d9b2 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048d9b8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048d9bb */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048d9be */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d9c1 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048d9c3 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048d9c6 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d9cc */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048d9cf */
    if(!(f->flags&MI_ZF))goto mi_0048da36;
    /* 0048d9d1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[0]),2u);
    /* 0048d9d8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048d9da */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9db */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048d9e1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9e3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9e4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9e5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048d9eau;f->boundary(0x0048d9e5u,0x0045e220u,f);
    /* 0048d9ea */
    goto mi_0048dc07;
mi_0048d9ef:;
    /* 0048d9ef */
    a=(f->r[2]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 0048d9f2 */
    if(!(f->flags&MI_ZF))goto mi_0048da73;
    /* 0048d9f4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048d9f5 */
    MI_U8((0x00bcd66au+f->r[0]))=(uint8_t)(0x00000003u);
    /* 0048d9fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048da01u;f->boundary(0x0048d9fcu,0x0048b4a0u,f);
    /* 0048da01 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048da07 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048da08 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048da09 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048da0eu;f->boundary(0x0048da09u,0x004a5da0u,f);
    /* 0048da0e */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048da14 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048da17 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048da1a */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048da1d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048da1f */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048da22 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048da28 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048da2b */
    if(!(f->flags&MI_ZF))goto mi_0048da36;
    /* 0048da2d */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000010u);
    /* 0048da34 */
    goto mi_0048da4e;
mi_0048da36:;
    /* 0048da36 */
    a=(f->r[2]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 0048da39 */
    if(!(f->flags&MI_ZF))goto mi_0048d419;
mi_0048da3f:;
    /* 0048da3f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048da40 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048da45u;f->boundary(0x0048da40u,0x004b3250u,f);
mi_0048da45:;
    /* 0048da45 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0048da48:;
    /* 0048da48 */
    f->r[1]=mi_read((0x00d056f8u),4u);
mi_0048da4e:;
    /* 0048da4e */
    f->r[6]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048da51 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048da54 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0048da56 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0048da59 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[6]),1u))&0xffu)<<0);
    /* 0048da5f */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048da61 */
    if(mi_signed_less(f->flags))goto mi_0048dcda;
    /* 0048da67 */
    f->r[1]=(uint32_t)(int32_t)(int8_t)mi_read((0x00bcd60au+f->r[6]),1u);
    /* 0048da6e */
    goto mi_0048dcdd;
mi_0048da73:;
    /* 0048da73 */
    a=(f->r[2]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 0048da76 */
    if(!(f->flags&MI_ZF))goto mi_0048dba2;
    /* 0048da7c */
    MI_U8((0x00bcd66au+f->r[0]))=(uint8_t)(0x00000003u);
    /* 0048da83 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd656u+f->r[0]),2u);
    /* 0048da8a */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(t);
    /* 0048da91 */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048da98 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048d5a1;
    /* 0048da9e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0048daa0 */
    MI_U16((0x00bcd66cu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0048daa7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[0]),1u))&0xffu)<<0);
    /* 0048daad */
    f->r[1]=f->r[0];
    /* 0048daaf */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0048dab5 */
    f->r[6]=(0xffffffe8u+f->r[1]);
    /* 0048dab8 */
    a=f->r[6];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048dabb */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048dacc;
    /* 0048dabd */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048dabf */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048de90u+f->r[6]),1u))&0xffu)<<0);
    /* 0048dac5 */
    switch(mi_read((0x0048de88u+f->r[2]*4u),4u)){case 0x0048daccu:goto mi_0048dacc;case 0x0048daddu:goto mi_0048dadd;default:abort();}
mi_0048dacc:;
    /* 0048dacc */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048dace */
    if((f->flags&MI_ZF))goto mi_0048dadd;
    /* 0048dad0 */
    a=(f->r[0]&0xffu);b=0x0000001cu;t=mi_alu(f,a,b,8,1);
    /* 0048dad2 */
    if((f->flags&MI_ZF))goto mi_0048dadd;
    /* 0048dad4 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 0048dad6 */
    f->r[6]=0x00000190u;
    /* 0048dadb */
    if(!(f->flags&MI_ZF))goto mi_0048dae2;
mi_0048dadd:;
    /* 0048dadd */
    f->r[6]=0x00000064u;
mi_0048dae2:;
    /* 0048dae2 */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0048dae7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dae8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dae9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048daea */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048daeb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048daf0u;f->boundary(0x0048daebu,0x0045b570u,f);
    /* 0048daf0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048daf3 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0048daf5 */
    if(!(f->flags&MI_ZF))goto mi_0048db11;
    /* 0048daf7 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048dafd */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048db00 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048db03 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048db05 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048db08 */
    MI_U16((0x00bcd66cu+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0048db0f */
    goto mi_0048db39;
mi_0048db11:;
    /* 0048db11 */
    f->r[2]=mi_read((0x00d056f8u),4u);
    /* 0048db17 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0048db19 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0048db1c */
    if(!(f->flags&MI_ZF))goto mi_0048db28;
    /* 0048db1e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048db21 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048db23 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048db26 */
    goto mi_0048db32;
mi_0048db28:;
    /* 0048db28 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048db2b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048db2d */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0048db30 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
mi_0048db32:;
    /* 0048db32 */
    MI_U16((0x00bcd66cu+f->r[1]))=(uint16_t)((f->r[6]&0xffffu));
mi_0048db39:;
    /* 0048db39 */
    a=mi_read((0x00bcd66cu+f->r[1]),2u);b=(f->r[5]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 0048db40 */
    if(!(f->flags&MI_ZF))goto mi_0048db5e;
    /* 0048db42 */
    MI_U8((0x00bcd603u+f->r[1]))=(uint8_t)(0x00000000u);
    /* 0048db49 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048db4a */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000008u);
    /* 0048db51 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048db56u;f->boundary(0x0048db51u,0x004b3250u,f);
    /* 0048db56 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048db59 */
    goto mi_0048d59b;
mi_0048db5e:;
    /* 0048db5e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[1]),1u))&0xffu)<<0);
    /* 0048db64 */
    MI_U8((0x00bcd603u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 0048db6a */
    MI_U8((0x00bcd630u+f->r[1]))=(uint8_t)(0x00000011u);
    /* 0048db71 */
    MI_U16((0x00bcd62eu+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
mi_0048db78:;
    /* 0048db78 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd624u+f->r[1]),2u))&0xffffu)<<0);
    /* 0048db7f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048db80 */
    MI_U16((0x00bcd61cu+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048db87 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read((0x00bcd626u+f->r[1]),2u))&0xffffu)<<0);
    /* 0048db8e */
    MI_U16((0x00bcd61eu+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0048db95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048db9au;f->boundary(0x0048db95u,0x004b3250u,f);
    /* 0048db9a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048db9d */
    goto mi_0048d59b;
mi_0048dba2:;
    /* 0048dba2 */
    a=(f->r[2]&0xffu);b=0x00000011u;t=mi_alu(f,a,b,8,1);
    /* 0048dba5 */
    if(!(f->flags&MI_ZF))goto mi_0048dc1e;
    /* 0048dba7 */
    MI_U8((0x00bcd66au+f->r[0]))=(uint8_t)(0x00000003u);
    /* 0048dbae */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dbaf */
    f->r[5]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd652u+f->r[0]),2u);
    /* 0048dbb6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048dbbbu;f->boundary(0x0048dbb6u,0x0048b4a0u,f);
    /* 0048dbbb */
    f->r[0]=mi_read((0x00d056f8u),4u);
    /* 0048dbc0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dbc1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dbc2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048dbc7u;f->boundary(0x0048dbc2u,0x004a5da0u,f);
    /* 0048dbc7 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048dbcd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048dbd0 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048dbd3 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048dbd6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048dbd8 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048dbdb */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd609u+f->r[0]),1u))&0xffu)<<0);
    /* 0048dbe1 */
    a=(f->r[2]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 0048dbe4 */
    if(!(f->flags&MI_ZF))goto mi_0048da36;
    /* 0048dbea */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd66cu+f->r[0]),2u);
    /* 0048dbf1 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048dbf3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dbf4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[0]),1u))&0xffu)<<0);
    /* 0048dbfa */
    f->r[0]=mi_read((0x00eaac28u),4u);
    /* 0048dbff */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc00 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc01 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc02 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048dc07u;f->boundary(0x0048dc02u,0x0045b2a0u,f);
mi_0048dc07:;
    /* 0048dc07 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048dc0d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc0e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc0f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc10 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048dc16u;f->boundary(0x0048dc11u,0x0048caa0u,f);
    /* 0048dc16 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048dc19 */
    goto mi_0048da48;
mi_0048dc1e:;
    /* 0048dc1e */
    a=(f->r[2]&0xffu);b=0x00000007u;t=mi_alu(f,a,b,8,1);
    /* 0048dc21 */
    if(!(f->flags&MI_ZF))goto mi_0048dca8;
    /* 0048dc27 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc28 */
    MI_U8((0x00bcd5f9u+f->r[0]))=(uint8_t)(0x00000000u);
    /* 0048dc2f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048dc34u;f->boundary(0x0048dc2fu,0x0048b4a0u,f);
    /* 0048dc34 */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048dc3a */
    f->r[2]=mi_read((0x00e94e00u),4u);
    /* 0048dc40 */
    f->r[6]=mi_read((0x00f55e44u),4u);
    /* 0048dc46 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048dc49 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048dc4c */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048dc4f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048dc51 */
    f->r[2]=(0x00000001u+f->r[6]+f->r[2]*1u);
    /* 0048dc55 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048dc58 */
    MI_U32((0x00bcd5fcu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0048dc5e */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(t);
    /* 0048dc65 */
    a=mi_read((0x00bcd62eu+f->r[0]),2u);b=0x00000064u;t=mi_alu(f,a,b,16,1);
    /* 0048dc6d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0048da4e;
    /* 0048dc73 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048dc74 */
    MI_U16((0x00bcd62eu+f->r[0]))=(uint16_t)(0x00000000u);
    /* 0048dc7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0048dc82u;f->boundary(0x0048dc7du,0x004a35e0u,f);
    /* 0048dc82 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048dc85 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048dc87 */
    if(!(f->flags&MI_ZF))goto mi_0048da48;
    /* 0048dc8d */
    f->r[1]=mi_read((0x00d056f8u),4u);
    /* 0048dc93 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0048dc96 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0048dc99 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048dc9b */
    MI_U8((0x00bcd630u+f->r[0]*4u))=(uint8_t)(0x00000008u);
    /* 0048dca3 */
    goto mi_0048da4e;
mi_0048dca8:;
    /* 0048dca8 */
    a=(f->r[2]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0048dcab */
    if(!(f->flags&MI_ZF))goto mi_0048dcce;
    /* 0048dcad */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048dcaf */
    MI_U32((0x00bcd5fcu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0048dcb5 */
    f->r[6]=mi_read((0x00bcd778u+f->r[0]),4u);
    /* 0048dcbb */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0048dcbd */
    if((f->flags&MI_ZF))goto mi_0048da4e;
    /* 0048dcc3 */
    MI_U32((0x00bcd778u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0048dcc9 */
    goto mi_0048da4e;
mi_0048dcce:;
    /* 0048dcce */
    MI_U8((0x00bcd630u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0048dcd5 */
    goto mi_0048da4e;
mi_0048dcda:;
    /* 0048dcda */
    f->r[1]=(uint32_t)(int32_t)(int8_t)(f->r[0]&0xffu);
mi_0048dcdd:;
    /* 0048dcdd */
    a=f->r[1];b=mi_read((0x00bca2e0u),4u);t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0048dce3 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0048dce5 */
    MI_U32((0x00e94e00u))=(uint32_t)(f->r[1]);
    /* 0048dceb */
    if(!mi_signed_less(f->flags))goto mi_0048dcf6;
    /* 0048dced */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0048dcf0 */
    MI_U32((0x00e94e00u))=(uint32_t)(f->r[1]);
mi_0048dcf6:;
    /* 0048dcf6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd630u+f->r[6]),1u))&0xffu)<<0);
    /* 0048dcfc */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0048dcfe */
    if(!(f->flags&MI_ZF))goto mi_0048dd2d;
    /* 0048dd00 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x00bcd62eu+f->r[6]),2u);
    /* 0048dd07 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0048dd08 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0048dd0a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048dd0c */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0048dd0e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x005d048cu+f->r[0]),1u))&0xffu)<<0);
    /* 0048dd14 */
    f->r[0]=mi_read((0x00f55e4cu),4u);
    /* 0048dd19 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0048dd1b */
    MI_U32((0x00bcd5fcu+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0048dd21 */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 0048dd2b */
    goto mi_0048dd4a;
mi_0048dd2d:;
    /* 0048dd2d */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0048dd2f */
    if((f->flags&MI_ZF))goto mi_0048dd4a;
    /* 0048dd31 */
    f->r[2]=mi_read((0x00f55e44u),4u);
    /* 0048dd37 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0048dd39 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd5f9u+f->r[6]),1u))&0xffu)<<0);
    /* 0048dd3f */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*8u);
    /* 0048dd42 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048dd44 */
    MI_U32((0x00bcd5fcu+f->r[6]))=(uint32_t)(f->r[0]);
mi_0048dd4a:;
    /* 0048dd4a */
    f->r[0]=mi_read((0x00bcd778u+f->r[6]),4u);
    /* 0048dd50 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0048dd52 */
    if((f->flags&MI_ZF))goto mi_0048dddd;
    /* 0048dd58 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0048dd5a */
    f->r[7]=f->r[1];
    /* 0048dd5c */
    MI_U32((0x00bcd778u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0048dd62 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd603u+f->r[6]),1u))&0xffu)<<0);
    /* 0048dd68 */
    f->r[1]=f->r[0];
    /* 0048dd6a */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0048dd70 */
    f->r[2]=(0xffffffe8u+f->r[1]);
    /* 0048dd73 */
    a=f->r[2];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048dd76 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048dd87;
    /* 0048dd78 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0048dd7a */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x0048dea4u+f->r[2]),1u))&0xffu)<<0);
    /* 0048dd80 */
    switch(mi_read((0x0048de9cu+f->r[1]*4u),4u)){case 0x0048dd87u:goto mi_0048dd87;case 0x0048ddb8u:goto mi_0048ddb8;default:abort();}
mi_0048dd87:;
    /* 0048dd87 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048dd89 */
    if((f->flags&MI_ZF))goto mi_0048ddb8;
    /* 0048dd8b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0048dd8d */
    if(!(f->flags&MI_ZF))goto mi_0048ddc3;
    /* 0048dd8f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00bcd672u+f->r[6]),1u))&0xffu)<<0);
    /* 0048dd95 */
    f->r[2]=f->r[0];
    /* 0048dd97 */
    a=f->r[2];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0048dd9d */
    f->r[1]=(0xffffffe8u+f->r[2]);
    /* 0048dda0 */
    a=f->r[1];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0048dda3 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0048ddb4;
    /* 0048dda5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0048dda7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x0048deb8u+f->r[1]),1u))&0xffu)<<0);
    /* 0048ddad */
    switch(mi_read((0x0048deb0u+f->r[2]*4u),4u)){case 0x0048ddb4u:goto mi_0048ddb4;case 0x0048ddb8u:goto mi_0048ddb8;default:abort();}
mi_0048ddb4:;
    /* 0048ddb4 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 0048ddb6 */
    if(!(f->flags&MI_ZF))goto mi_0048ddc3;
mi_0048ddb8:;
    /* 0048ddb8 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0048ddbb */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0048ddbe */
    if(mi_signed_less(f->flags))goto mi_0048ddc3;
    /* 0048ddc0 */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[7]=t;
mi_0048ddc3:;
    /* 0048ddc3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x005dc23cu+f->r[7]*2u),1u))&0xffu)<<0);
    /* 0048ddca */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x005dc23du+f->r[7]*2u),1u))&0xffu)<<0);
    /* 0048ddd1 */
    MI_U8((0x00bcd664u+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0048ddd7 */
    MI_U8((0x00bcd665u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
mi_0048dddd:;
    /* 0048dddd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048ddde */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0048dddf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0048dde0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0048dde1 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0048dde4 */
    f->r[4]+=4;return;
}

void restore_transport_C_004b28f0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b28f0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b28f1 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004b28f5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b28f6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b28f7 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004b28f9 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004b28fc */
    f->r[1]=(0x00000008u+f->r[4]);
    /* 004b2900 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b2903 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004b2905 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2906 */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004b2909 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b290a */
    MI_U16((0x00bcd632u+f->r[6]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004b2911 */
    MI_U16((0x00bcd634u+f->r[6]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004b2918 */
    MI_U16((0x00bcd636u+f->r[6]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004b291f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2924u;f->boundary(0x004b291fu,0x00489970u,f);
    /* 004b2924 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2927 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b2929 */
    if(!(f->flags&MI_ZF))goto mi_004b2931;
    /* 004b292b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b292c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b292e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b292f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b2930 */
    f->r[4]+=4;return;
mi_004b2931:;
    /* 004b2931 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd66eu+f->r[6])))&0xffu)<<0);
    /* 004b2937 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2938 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b293a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b293b */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004b293d */
    if((f->flags&MI_ZF))goto mi_004b2aa4;
    /* 004b2943 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004b294a */
    if(!(f->flags&MI_ZF))goto mi_004b2963;
    /* 004b294c */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c8eu));
    /* 004b2953 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c8cu));
    /* 004b295a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b295b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b295c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2961u;f->boundary(0x004b295cu,0x0051d160u,f);
    /* 004b2961 */
    goto mi_004b2993;
mi_004b2963:;
    /* 004b2963 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b2965 */
    if(!(f->flags&MI_ZF))goto mi_004b297e;
    /* 004b2967 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b296e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2975 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2976 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2977 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b297cu;f->boundary(0x004b2977u,0x0051d160u,f);
    /* 004b297c */
    goto mi_004b2993;
mi_004b297e:;
    /* 004b297e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2985 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b298c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b298d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b298e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2993u;f->boundary(0x004b298eu,0x0051ce90u,f);
mi_004b2993:;
    /* 004b2993 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd66eu+f->r[6])))&0xffu)<<0);
    /* 004b2999 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b299c */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b299e */
    if(!(f->flags&MI_ZF))goto mi_004b29d8;
    /* 004b29a0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61eu+f->r[6]));
    /* 004b29a7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[6]));
    /* 004b29ae */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b29b5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b29b7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b29b8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b29bf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b29c0 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b29c4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b29c5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b29c6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b29c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b29ccu;f->boundary(0x004b29c7u,0x0051dc10u,f);
    /* 004b29cc */
    f->r[5]=f->r[0];
    /* 004b29ce */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b29d1 */
    f->r[7]=f->r[5];
    /* 004b29d3 */
    goto mi_004b2ceb;
mi_004b29d8:;
    /* 004b29d8 */
    a=MI_U8((0x00bcd602u+f->r[6]));b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004b29df */
    if(!(f->flags&MI_ZF))goto mi_004b2a6d;
    /* 004b29e5 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[6])))&0xffu)<<0);
    /* 004b29eb */
    f->r[0]=0x00000009u;
    /* 004b29f0 */
    a=(f->r[1]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004b29f2 */
    if((f->flags&MI_ZF))goto mi_004b29fc;
    /* 004b29f4 */
    a=MI_U32((0x00bcd754u+f->r[6]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004b29fa */
    if(!(f->flags&MI_ZF))goto mi_004b2a6d;
mi_004b29fc:;
    /* 004b29fc */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61eu+f->r[6]));
    /* 004b2a03 */
    f->r[1]=MI_U32((0x00bcd738u+f->r[6]));
    /* 004b2a09 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[6]));
    /* 004b2a10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a11 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a12 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2a19 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2a20 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a21 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2a25 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a26 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2a2du;f->boundary(0x004b2a28u,0x0051d980u,f);
    /* 004b2a2d */
    f->r[5]=f->r[0];
    /* 004b2a2f */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2a32 */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004b2a34 */
    if(!(f->flags&MI_ZF))goto mi_004b2a9d;
    /* 004b2a36 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61eu+f->r[6]));
    /* 004b2a3d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[6]));
    /* 004b2a44 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2a4b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a4c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a4d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2a54 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a55 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2a59 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a5a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a5b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a5c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2a61u;f->boundary(0x004b2a5cu,0x0051dc10u,f);
    /* 004b2a61 */
    f->r[5]=f->r[0];
    /* 004b2a63 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2a66 */
    f->r[7]=f->r[5];
    /* 004b2a68 */
    goto mi_004b2ceb;
mi_004b2a6d:;
    /* 004b2a6d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61eu+f->r[6]));
    /* 004b2a74 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[6]));
    /* 004b2a7b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2a82 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a83 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a84 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2a8b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a8c */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2a90 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a91 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a92 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2a93 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2a98u;f->boundary(0x004b2a93u,0x0051dc10u,f);
    /* 004b2a98 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2a9b */
    f->r[5]=f->r[0];
mi_004b2a9d:;
    /* 004b2a9d */
    f->r[7]=f->r[5];
    /* 004b2a9f */
    goto mi_004b2ceb;
mi_004b2aa4:;
    /* 004b2aa4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd66au+f->r[6])))&0xffu)<<0);
    /* 004b2aaa */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004b2aac */
    if(!(f->flags&MI_ZF))goto mi_004b2b00;
    /* 004b2aae */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2ab2 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2ab6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2abd */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2ac4 */
    a=0x000186a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2ac9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2aca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2acb */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2acc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2acd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2ace */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2ad3u;f->boundary(0x004b2aceu,0x0051a9d0u,f);
    /* 004b2ad3 */
    f->r[7]=f->r[0];
    /* 004b2ad5 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2ad8 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004b2ada */
    if(!(f->flags&MI_ZF))goto mi_004b2c2b;
    /* 004b2ae0 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2ae4 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2aeb */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2af2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2af3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2af4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2af5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2af6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2afbu;f->boundary(0x004b2af6u,0x0051b220u,f);
    /* 004b2afb */
    goto mi_004b2c1e;
mi_004b2b00:;
    /* 004b2b00 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004b2b02 */
    if(!(f->flags&MI_ZF))goto mi_004b2b2a;
    /* 004b2b04 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2b08 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2b0c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2b13 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2b1a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b1b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b1c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b1d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b1e */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b2b20 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2b25u;f->boundary(0x004b2b20u,0x0051a220u,f);
    /* 004b2b25 */
    goto mi_004b2c1e;
mi_004b2b2a:;
    /* 004b2b2a */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004b2b2c */
    if(!(f->flags&MI_ZF))goto mi_004b2b52;
mi_004b2b2e:;
    /* 004b2b2e */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2b32 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2b36 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2b3d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2b44 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b45 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b46 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b47 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b48 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2b4du;f->boundary(0x004b2b48u,0x00518b30u,f);
    /* 004b2b4d */
    goto mi_004b2c1e;
mi_004b2b52:;
    /* 004b2b52 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004b2b54 */
    if(!(f->flags&MI_ZF))goto mi_004b2ba3;
    /* 004b2b56 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2b5a */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2b5e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2b65 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2b6c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b6d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b6e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b6f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2b75u;f->boundary(0x004b2b70u,0x00519e30u,f);
    /* 004b2b75 */
    f->r[7]=f->r[0];
    /* 004b2b77 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2b7a */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b2b7c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004b2b7e */
    if(!(f->flags&MI_ZF))goto mi_004b2c2b;
    /* 004b2b84 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2b88 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004b2b8a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2b91 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2b98 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b99 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b9a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b9b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2b9c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2ba1u;f->boundary(0x004b2b9cu,0x005190a0u,f);
    /* 004b2ba1 */
    goto mi_004b2c1e;
mi_004b2ba3:;
    /* 004b2ba3 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004b2ba5 */
    if(!(f->flags&MI_ZF))goto mi_004b2bca;
    /* 004b2ba7 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2bab */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2baf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2bb6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2bbd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bbe */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bbf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bc0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bc1 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,5);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004b2bc3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2bc8u;f->boundary(0x004b2bc3u,0x00519e30u,f);
    /* 004b2bc8 */
    goto mi_004b2c1e;
mi_004b2bca:;
    /* 004b2bca */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b2bcc */
    if((f->flags&MI_ZF))goto mi_004b2b2e;
    /* 004b2bd2 */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004b2bd4 */
    if(!(f->flags&MI_ZF))goto mi_004b2bf7;
    /* 004b2bd6 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2bda */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2bde */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2be5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2bec */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bed */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bef */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2bf0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2bf5u;f->boundary(0x004b2bf0u,0x00518dd0u,f);
    /* 004b2bf5 */
    goto mi_004b2c1e;
mi_004b2bf7:;
    /* 004b2bf7 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004b2bf9 */
    if(!(f->flags&MI_ZF))goto mi_004b2d16;
    /* 004b2bff */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004b2c03 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004b2c07 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2c0e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2c15 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c16 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c17 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c18 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c19 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2c1eu;f->boundary(0x004b2c19u,0x0051d6d0u,f);
mi_004b2c1e:;
    /* 004b2c1e */
    f->r[7]=f->r[0];
    /* 004b2c20 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2c23 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004b2c25 */
    if((f->flags&MI_ZF))goto mi_004b2d16;
mi_004b2c2b:;
    /* 004b2c2b */
    f->r[5]=MI_U32((0xfffffffcu+f->r[5]+f->r[7]*4u));
    /* 004b2c2f */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004b2c35 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004b2c37 */
    f->r[1]=0x000000e4u;
    /* 004b2c3c */
    f->r[0]=f->r[5];
    /* 004b2c3e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004b2c3f */
    a=f->r[1];mi_idiv32(f,a);
    /* 004b2c41 */
    f->r[0]=0x8fb823efu;
    /* 004b2c46 */
    MI_U16((0x00bcd61cu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b2c4d */
    a=f->r[0];b=f->r[5];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004b2c4f */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b2c51 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004b2c54 */
    f->r[0]=f->r[2];
    /* 004b2c56 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004b2c59 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b2c5b */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004b2c5d */
    MI_U16((0x00bcd61eu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004b2c64 */
    if(!(f->flags&MI_ZF))goto mi_004b2cbe;
    /* 004b2c66 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2c6d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 004b2c70 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[6]));
    /* 004b2c77 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c78 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c79 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2c80 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004b2c84 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c85 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c86 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c87 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2c89 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2c8eu;f->boundary(0x004b2c89u,0x0051b400u,f);
    /* 004b2c8e */
    f->r[5]=f->r[0];
    /* 004b2c90 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2c93 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004b2c95 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004b2cef;
    /* 004b2c97 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61eu+f->r[6]));
    /* 004b2c9e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[6]));
    /* 004b2ca5 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2cac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cad */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cae */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2cb5 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004b2cb9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cbb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cbc */
    goto mi_004b2cdf;
mi_004b2cbe:;
    /* 004b2cbe */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd61cu+f->r[6]));
    /* 004b2cc5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60eu+f->r[6]));
    /* 004b2ccc */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[2]&0xffffu);
    /* 004b2ccf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cd0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cd1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd60cu+f->r[6]));
    /* 004b2cd8 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004b2cdc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cdd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cde */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004b2cdf:;
    /* 004b2cdf */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2ce1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2ce6u;f->boundary(0x004b2ce1u,0x0051b400u,f);
    /* 004b2ce6 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2ce9 */
    f->r[5]=f->r[0];
mi_004b2ceb:;
    /* 004b2ceb */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004b2ced */
    if((f->flags&MI_ZF))goto mi_004b2d16;
mi_004b2cef:;
    /* 004b2cef */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 004b2cf3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004b2cf4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004b2cf9u;f->boundary(0x004b2cf4u,0x004b3250u,f);
    /* 004b2cf9 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004b2cfd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004b2d00 */
    MI_U16((0x00bcd632u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004b2d07 */
    MI_U16((0x00bcd636u+f->r[6]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004b2d0e */
    MI_U16((0x00d04e58u+f->r[0]*2u))=(uint16_t)((f->r[3]&0xffffu));
mi_004b2d16:;
    /* 004b2d16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004b2d17 */
    f->r[0]=f->r[7];
    /* 004b2d19 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004b2d1a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004b2d1b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004b2d1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004b2d1d */
    f->r[4]+=4;return;
}

void restore_transport_C_00518dd0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00518dd0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518dd1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518dd2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518dd3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518dd4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518dd6 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518ddb */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518de0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518de5u;f->boundary(0x00518de0u,0x0040f8f0u,f);
    /* 00518de5 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00518de9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518dec */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00518def */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00518df2 */
    f->r[1]=MI_U32((0x00eadfd4u));
    /* 00518df8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00518dfb */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 00518e00 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00518e03 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00518e07 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00518e09 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00518e0a */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00518e0f */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 00518e15 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00518e20 */
    f->r[1]=MI_U32((0x00eae004u));
    /* 00518e26 */
    f->r[0]=0x00000001u;
    /* 00518e2b */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 00518e31 */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
    /* 00518e3b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00518e40:;
    /* 00518e40 */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 00518e46 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00518e4a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00518e4c */
    f->r[7]=MI_U32((0x00f203a0u+f->r[1]*4u));
    /* 00518e53 */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 00518e56 */
    if((f->flags&MI_ZF))goto mi_00518e71;
    /* 00518e58 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00518e5c */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
mi_00518e5f:;
    /* 00518e5f */
    a=MI_U32((0x00000000u+f->r[2]));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00518e61 */
    if((f->flags&MI_ZF))goto mi_0051908c;
    /* 00518e67 */
    f->r[6]=f->r[1];
    /* 00518e69 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00518e6a */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00518e6d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00518e6f */
    if(!(f->flags&MI_ZF))goto mi_00518e5f;
mi_00518e71:;
    /* 00518e71 */
    f->r[3]=MI_U32((0x00eed760u+f->r[7]*4u));
    /* 00518e78 */
    f->r[5]=(0x00eed760u+f->r[7]*4u);
    /* 00518e7f */
    f->r[6]=(0xffffff1cu+f->r[7]);
    /* 00518e85 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00518e86 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00518e8d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518e8f */
    if(!(f->flags&MI_ZF))goto mi_00518efa;
    /* 00518e91 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518e92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518e97u;f->boundary(0x00518e92u,0x0051aff0u,f);
    /* 00518e97 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518e9a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518e9c */
    if((f->flags&MI_ZF))goto mi_00518ef5;
    /* 00518e9e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0b6cu+f->r[7])))&0xffu)<<0);
    /* 00518ea4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518ea6 */
    if(!mi_signed_less(f->flags))goto mi_00518ebc;
    /* 00518ea8 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518eaa */
    if((f->flags&MI_ZF))goto mi_00518ebc;
    /* 00518eac */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518eae */
    if(!(f->flags&MI_ZF))goto mi_00518ef5;
    /* 00518eb0 */
    f->r[1]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 00518eb7 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518eba */
    if((f->flags&MI_ZF))goto mi_00518ef5;
mi_00518ebc:;
    /* 00518ebc */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00518ebe */
    if((f->flags&MI_ZF))goto mi_00518ef5;
    /* 00518ec0 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00518ec2 */
    if((f->flags&MI_ZF))goto mi_00518ef5;
    /* 00518ec4 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518ec6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518ecc;
    /* 00518ec8 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 00518eca */
    if(!(f->flags&MI_ZF))goto mi_00518ef5;
mi_00518ecc:;
    /* 00518ecc */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518ed3 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00518ed8 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518edf */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518ee0 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518ee5 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518eea */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518efa;
    /* 00518eec */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518eee */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518ef3 */
    goto mi_00518efa;
mi_00518ef5:;
    /* 00518ef5 */
    f->r[0]=MI_U32((0x00eae008u));
mi_00518efa:;
    /* 00518efa */
    f->r[6]=(0x00000001u+f->r[7]);
    /* 00518efd */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00518f04 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518f06 */
    if(!(f->flags&MI_ZF))goto mi_00518f71;
    /* 00518f08 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518f09 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518f0eu;f->boundary(0x00518f09u,0x0051aff0u,f);
    /* 00518f0e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518f11 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518f13 */
    if((f->flags&MI_ZF))goto mi_00518f6c;
    /* 00518f15 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c51u+f->r[7])))&0xffu)<<0);
    /* 00518f1b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518f1d */
    if(!mi_signed_less(f->flags))goto mi_00518f33;
    /* 00518f1f */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518f21 */
    if((f->flags&MI_ZF))goto mi_00518f33;
    /* 00518f23 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518f25 */
    if(!(f->flags&MI_ZF))goto mi_00518f6c;
    /* 00518f27 */
    f->r[1]=MI_U32((0x00af5990u+f->r[7]*4u));
    /* 00518f2e */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518f31 */
    if((f->flags&MI_ZF))goto mi_00518f6c;
mi_00518f33:;
    /* 00518f33 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00518f35 */
    if((f->flags&MI_ZF))goto mi_00518f6c;
    /* 00518f37 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00518f39 */
    if((f->flags&MI_ZF))goto mi_00518f6c;
    /* 00518f3b */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518f3d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518f43;
    /* 00518f3f */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 00518f41 */
    if(!(f->flags&MI_ZF))goto mi_00518f6c;
mi_00518f43:;
    /* 00518f43 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518f4a */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00518f4f */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518f56 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518f57 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518f5c */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518f61 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518f71;
    /* 00518f63 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518f65 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518f6a */
    goto mi_00518f71;
mi_00518f6c:;
    /* 00518f6c */
    f->r[0]=MI_U32((0x00eae008u));
mi_00518f71:;
    /* 00518f71 */
    f->r[6]=(0x000000e4u+f->r[7]);
    /* 00518f77 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00518f7e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518f80 */
    if(!(f->flags&MI_ZF))goto mi_00518feb;
    /* 00518f82 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518f83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518f88u;f->boundary(0x00518f83u,0x0051aff0u,f);
    /* 00518f88 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518f8b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00518f8d */
    if((f->flags&MI_ZF))goto mi_00518fe6;
    /* 00518f8f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0d34u+f->r[7])))&0xffu)<<0);
    /* 00518f95 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00518f97 */
    if(!mi_signed_less(f->flags))goto mi_00518fad;
    /* 00518f99 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00518f9b */
    if((f->flags&MI_ZF))goto mi_00518fad;
    /* 00518f9d */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00518f9f */
    if(!(f->flags&MI_ZF))goto mi_00518fe6;
    /* 00518fa1 */
    f->r[1]=MI_U32((0x00af5d1cu+f->r[7]*4u));
    /* 00518fa8 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00518fab */
    if((f->flags&MI_ZF))goto mi_00518fe6;
mi_00518fad:;
    /* 00518fad */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00518faf */
    if((f->flags&MI_ZF))goto mi_00518fe6;
    /* 00518fb1 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00518fb3 */
    if((f->flags&MI_ZF))goto mi_00518fe6;
    /* 00518fb5 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00518fb7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518fbd;
    /* 00518fb9 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 00518fbb */
    if(!(f->flags&MI_ZF))goto mi_00518fe6;
mi_00518fbd:;
    /* 00518fbd */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 00518fc4 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00518fc9 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00518fd0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00518fd1 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00518fd6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518fdb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00518feb;
    /* 00518fdd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00518fdf */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00518fe4 */
    goto mi_00518feb;
mi_00518fe6:;
    /* 00518fe6 */
    f->r[0]=MI_U32((0x00eae008u));
mi_00518feb:;
    /* 00518feb */
    f->r[1]=MI_U32((0xfffffffcu+f->r[5]));
    /* 00518fee */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00518ff0 */
    if(!(f->flags&MI_ZF))goto mi_0051905a;
    /* 00518ff2 */
    f->r[6]=(0xffffffffu+f->r[7]);
    /* 00518ff5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00518ff6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00518ffbu;f->boundary(0x00518ff6u,0x0051aff0u,f);
    /* 00518ffb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00518ffe */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00519000 */
    if((f->flags&MI_ZF))goto mi_00519055;
    /* 00519002 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c4fu+f->r[7])))&0xffu)<<0);
    /* 00519008 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051900a */
    if(!mi_signed_less(f->flags))goto mi_00519020;
    /* 0051900c */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051900e */
    if((f->flags&MI_ZF))goto mi_00519020;
    /* 00519010 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00519012 */
    if(!(f->flags&MI_ZF))goto mi_00519055;
    /* 00519014 */
    f->r[1]=MI_U32((0x00af5988u+f->r[7]*4u));
    /* 0051901b */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051901e */
    if((f->flags&MI_ZF))goto mi_00519055;
mi_00519020:;
    /* 00519020 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00519022 */
    if((f->flags&MI_ZF))goto mi_00519055;
    /* 00519024 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00519026 */
    if((f->flags&MI_ZF))goto mi_00519055;
    /* 00519028 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051902a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519030;
    /* 0051902c */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051902e */
    if(!(f->flags&MI_ZF))goto mi_00519055;
mi_00519030:;
    /* 00519030 */
    MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(f->r[3]);
    /* 00519033 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00519038 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051903f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519040 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519045 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051904a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051905a;
    /* 0051904c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051904e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519053 */
    goto mi_0051905a;
mi_00519055:;
    /* 00519055 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051905a:;
    /* 0051905a */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 00519060 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00519061 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519067 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051906d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519079;
    /* 0051906f */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
mi_00519079:;
    /* 00519079 */
    a=MI_U32((0x00eae00cu));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051907f */
    if(!(f->flags&MI_ZF))goto mi_00518e40;
mi_00519085:;
    /* 00519085 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00519086 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00519087 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00519088 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051908a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051908b */
    f->r[4]+=4;return;
mi_0051908c:;
    /* 0051908c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051908e */
    if(mi_signed_less(f->flags))goto mi_00519085;
    /* 00519090 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00519091 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00519092 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00519093 */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 00519096 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00519097 */
    f->r[4]+=4;return;
}

void restore_transport_C_005190a0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005190a0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005190a1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005190a2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005190a3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005190a4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005190a6 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005190ab */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005190b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005190b5u;f->boundary(0x005190b0u,0x0040f8f0u,f);
    /* 005190b5 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 005190b9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005190bc */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005190be */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051937f;
    /* 005190c4 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 005190c8 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_005190cb:;
    /* 005190cb */
    f->r[6]=MI_U32((0x00000000u+f->r[1]));
    /* 005190cd */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005190d0 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 005190d7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005190d8 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005190d9 */
    if(!(f->flags&MI_ZF))goto mi_005190cb;
    /* 005190db */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 005190df */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 005190e2 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 005190e5 */
    f->r[1]=MI_U32((0x00eadfd4u));
    /* 005190eb */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005190ee */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 005190f3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 005190f6 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 005190fa */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005190fc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 005190fd */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00519102 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 00519108 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00519113 */
    f->r[1]=MI_U32((0x00eae004u));
    /* 00519119 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051911f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00519121 */
    f->r[0]=0x00000001u;
    /* 00519126 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051912c */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519131 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
mi_00519134:;
    /* 00519134 */
    f->r[7]=MI_U32((0x00f203a0u+f->r[1]*4u));
    /* 0051913b */
    f->r[5]=MI_U32((0x00eed760u+f->r[7]*4u));
    /* 00519142 */
    f->r[1]=(0x00eed760u+f->r[7]*4u);
    /* 00519149 */
    f->r[6]=(0xffffff1cu+f->r[7]);
    /* 0051914f */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00519153 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00519154 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051915b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051915d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005191d2;
    /* 0051915f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519160 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00519165u;f->boundary(0x00519160u,0x0051aff0u,f);
    /* 00519165 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00519168 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051916a */
    if((f->flags&MI_ZF))goto mi_005191cd;
    /* 0051916c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0b6cu+f->r[7])))&0xffu)<<0);
    /* 00519172 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519174 */
    if(!mi_signed_less(f->flags))goto mi_0051918a;
    /* 00519176 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 00519178 */
    if((f->flags&MI_ZF))goto mi_0051918a;
    /* 0051917a */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051917c */
    if(!(f->flags&MI_ZF))goto mi_005191cd;
    /* 0051917e */
    f->r[1]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 00519185 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00519188 */
    if((f->flags&MI_ZF))goto mi_005191cd;
mi_0051918a:;
    /* 0051918a */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051918c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005191cd;
    /* 0051918e */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00519190 */
    if((f->flags&MI_ZF))goto mi_005191cd;
    /* 00519192 */
    f->r[0]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00519199 */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0051919c */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0051919e */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[5]);
    /* 005191a5 */
    if(!(f->flags&MI_SF))goto mi_00519377;
    /* 005191ab */
    f->r[0]=MI_U32((0x00eae008u));
    /* 005191b0 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 005191b7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005191b8 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005191bd */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005191c2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005191d2;
    /* 005191c4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005191c6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005191cb */
    goto mi_005191d2;
mi_005191cd:;
    /* 005191cd */
    f->r[0]=MI_U32((0x00eae008u));
mi_005191d2:;
    /* 005191d2 */
    f->r[6]=(0x00000001u+f->r[7]);
    /* 005191d5 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 005191dc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005191de */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00519253;
    /* 005191e0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005191e1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005191e6u;f->boundary(0x005191e1u,0x0051aff0u,f);
    /* 005191e6 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005191e9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005191eb */
    if((f->flags&MI_ZF))goto mi_0051924e;
    /* 005191ed */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c51u+f->r[7])))&0xffu)<<0);
    /* 005191f3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005191f5 */
    if(!mi_signed_less(f->flags))goto mi_0051920b;
    /* 005191f7 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 005191f9 */
    if((f->flags&MI_ZF))goto mi_0051920b;
    /* 005191fb */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 005191fd */
    if(!(f->flags&MI_ZF))goto mi_0051924e;
    /* 005191ff */
    f->r[1]=MI_U32((0x00af5990u+f->r[7]*4u));
    /* 00519206 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00519209 */
    if((f->flags&MI_ZF))goto mi_0051924e;
mi_0051920b:;
    /* 0051920b */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051920d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051924e;
    /* 0051920f */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00519211 */
    if((f->flags&MI_ZF))goto mi_0051924e;
    /* 00519213 */
    f->r[0]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051921a */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0051921d */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0051921f */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[5]);
    /* 00519226 */
    if(!(f->flags&MI_SF))goto mi_00519377;
    /* 0051922c */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00519231 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519238 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519239 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051923e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519243 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519253;
    /* 00519245 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519247 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051924c */
    goto mi_00519253;
mi_0051924e:;
    /* 0051924e */
    f->r[0]=MI_U32((0x00eae008u));
mi_00519253:;
    /* 00519253 */
    f->r[6]=(0x000000e4u+f->r[7]);
    /* 00519259 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00519260 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00519262 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005192d7;
    /* 00519264 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519265 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051926au;f->boundary(0x00519265u,0x0051aff0u,f);
    /* 0051926a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051926d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051926f */
    if((f->flags&MI_ZF))goto mi_005192d2;
    /* 00519271 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0d34u+f->r[7])))&0xffu)<<0);
    /* 00519277 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519279 */
    if(!mi_signed_less(f->flags))goto mi_0051928f;
    /* 0051927b */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051927d */
    if((f->flags&MI_ZF))goto mi_0051928f;
    /* 0051927f */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00519281 */
    if(!(f->flags&MI_ZF))goto mi_005192d2;
    /* 00519283 */
    f->r[1]=MI_U32((0x00af5d1cu+f->r[7]*4u));
    /* 0051928a */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051928d */
    if((f->flags&MI_ZF))goto mi_005192d2;
mi_0051928f:;
    /* 0051928f */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00519291 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005192d2;
    /* 00519293 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00519295 */
    if((f->flags&MI_ZF))goto mi_005192d2;
    /* 00519297 */
    f->r[0]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051929e */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 005192a1 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 005192a3 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[5]);
    /* 005192aa */
    if(!(f->flags&MI_SF))goto mi_00519377;
    /* 005192b0 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 005192b5 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 005192bc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005192bd */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 005192c2 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005192c7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005192d7;
    /* 005192c9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005192cb */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 005192d0 */
    goto mi_005192d7;
mi_005192d2:;
    /* 005192d2 */
    f->r[0]=MI_U32((0x00eae008u));
mi_005192d7:;
    /* 005192d7 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 005192db */
    f->r[1]=MI_U32((0xfffffffcu+f->r[2]));
    /* 005192de */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005192e0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051934e;
    /* 005192e2 */
    f->r[6]=(0xffffffffu+f->r[7]);
    /* 005192e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005192e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005192ebu;f->boundary(0x005192e6u,0x0051aff0u,f);
    /* 005192eb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005192ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005192f0 */
    if((f->flags&MI_ZF))goto mi_00519349;
    /* 005192f2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c4fu+f->r[7])))&0xffu)<<0);
    /* 005192f8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005192fa */
    if(!mi_signed_less(f->flags))goto mi_00519310;
    /* 005192fc */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 005192fe */
    if((f->flags&MI_ZF))goto mi_00519310;
    /* 00519300 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 00519302 */
    if(!(f->flags&MI_ZF))goto mi_00519349;
    /* 00519304 */
    f->r[1]=MI_U32((0x00af5988u+f->r[7]*4u));
    /* 0051930b */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051930e */
    if((f->flags&MI_ZF))goto mi_00519349;
mi_00519310:;
    /* 00519310 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 00519312 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00519349;
    /* 00519314 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 00519316 */
    if((f->flags&MI_ZF))goto mi_00519349;
    /* 00519318 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0051931c */
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 0051931f */
    a=f->r[3];b=MI_U32((0xfffffffcu+f->r[0]));t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00519322 */
    if(!(f->flags&MI_SF))goto mi_00519377;
    /* 00519324 */
    MI_U32((0xfffffffcu+f->r[0]))=(uint32_t)(f->r[5]);
    /* 00519327 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051932c */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519333 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519334 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519339 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051933e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051934e;
    /* 00519340 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519342 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519347 */
    goto mi_0051934e;
mi_00519349:;
    /* 00519349 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051934e:;
    /* 0051934e */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 00519354 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00519355 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051935b */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 00519361 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051936b;
    /* 00519363 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00519365 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
mi_0051936b:;
    /* 0051936b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051936d */
    if(!(f->flags&MI_ZF))goto mi_00519134;
    /* 00519373 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00519375 */
    if(mi_signed_less(f->flags))goto mi_0051937f;
mi_00519377:;
    /* 00519377 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00519378 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00519379 */
    f->r[0]=(0x00000001u+f->r[3]);
    /* 0051937c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051937d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051937e */
    f->r[4]+=4;return;
mi_0051937f:;
    /* 0051937f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00519380 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00519381 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00519382 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519384 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00519385 */
    f->r[4]+=4;return;
}

void restore_transport_C_00519e30(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00519e30 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519e31 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519e32 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519e33 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519e34 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519e36 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519e3b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00519e40 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00519e45u;f->boundary(0x00519e40u,0x0040f8f0u,f);
    /* 00519e45 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00519e49 */
    f->r[3]=MI_U32((0x0000002cu+f->r[4]));
    /* 00519e4d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00519e50 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00519e53 */
    f->r[5]=(0xffffffffu+f->r[3]);
    /* 00519e56 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00519e59 */
    f->r[1]=MI_U32((0x00eadfd4u));
    /* 00519e5f */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00519e62 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 00519e67 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00519e6a */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00519e6e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00519e70 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00519e71 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 00519e76 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 00519e7c */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 00519e87 */
    f->r[1]=MI_U32((0x00eae004u));
    /* 00519e8d */
    f->r[0]=0x00000001u;
    /* 00519e92 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 00519e98 */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
    /* 00519ea2 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519ea7:;
    /* 00519ea7 */
    f->r[2]=MI_U32((0x00eae00cu));
    /* 00519ead */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 00519eaf */
    f->r[1]=MI_U32((0x00f203a0u+f->r[2]*4u));
    /* 00519eb6 */
    f->r[2]=f->r[5];
    /* 00519eb8 */
    if((f->flags&MI_ZF))goto mi_00519ed3;
    /* 00519eba */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 00519ebe */
    f->r[6]=(0x00000000u+f->r[6]+f->r[2]*4u);
mi_00519ec1:;
    /* 00519ec1 */
    a=MI_U32((0x00000000u+f->r[6]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00519ec3 */
    if((f->flags&MI_ZF))goto mi_0051a021;
    /* 00519ec9 */
    f->r[7]=f->r[2];
    /* 00519ecb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00519ecc */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00519ecf */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00519ed1 */
    if(!(f->flags&MI_ZF))goto mi_00519ec1;
mi_00519ed3:;
    /* 00519ed3 */
    f->r[7]=MI_U32((0x00eed760u+f->r[1]*4u));
    /* 00519eda */
    f->r[6]=(0xffffff1cu+f->r[1]);
    /* 00519ee0 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00519ee1 */
    f->r[2]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00519ee8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519eea */
    if(!(f->flags&MI_ZF))goto mi_00519f22;
    /* 00519eec */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ee0b6cu+f->r[1])))&0xffu)<<0);
    /* 00519ef2 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519ef4 */
    if(mi_signed_less(f->flags))goto mi_00519f22;
    /* 00519ef6 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00519ef9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00519f22;
    /* 00519efb */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519f02 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00519f07 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519f0e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519f0f */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519f14 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519f19 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519f22;
    /* 00519f1b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519f1d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519f22:;
    /* 00519f22 */
    f->r[6]=(0x00000001u+f->r[1]);
    /* 00519f25 */
    f->r[2]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00519f2c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519f2e */
    if(!(f->flags&MI_ZF))goto mi_00519f66;
    /* 00519f30 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ee0c51u+f->r[1])))&0xffu)<<0);
    /* 00519f36 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519f38 */
    if(mi_signed_less(f->flags))goto mi_00519f66;
    /* 00519f3a */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00519f3d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00519f66;
    /* 00519f3f */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519f46 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00519f4b */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519f52 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519f53 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519f58 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519f5d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519f66;
    /* 00519f5f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519f61 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519f66:;
    /* 00519f66 */
    f->r[6]=(0x000000e4u+f->r[1]);
    /* 00519f6c */
    f->r[2]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 00519f73 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519f75 */
    if(!(f->flags&MI_ZF))goto mi_00519fad;
    /* 00519f77 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ee0d34u+f->r[1])))&0xffu)<<0);
    /* 00519f7d */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519f7f */
    if(mi_signed_less(f->flags))goto mi_00519fad;
    /* 00519f81 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00519f84 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00519fad;
    /* 00519f86 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 00519f8d */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00519f92 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00519f99 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519f9a */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519f9f */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519fa4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519fad;
    /* 00519fa6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519fa8 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519fad:;
    /* 00519fad */
    f->r[2]=MI_U32((0x00eed75cu+f->r[1]*4u));
    /* 00519fb4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00519fb6 */
    if(!(f->flags&MI_ZF))goto mi_00519fef;
    /* 00519fb8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ee0c4fu+f->r[1])))&0xffu)<<0);
    /* 00519fbe */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00519fc0 */
    if(mi_signed_less(f->flags))goto mi_00519fef;
    /* 00519fc2 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00519fc5 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00519fef;
    /* 00519fc7 */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[7]);
    /* 00519fce */
    f->r[0]=MI_U32((0x00eae008u));
    /* 00519fd3 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00519fd4 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 00519fdb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00519fdc */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519fe1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 00519fe6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00519fef;
    /* 00519fe8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00519fea */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_00519fef:;
    /* 00519fef */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 00519ff5 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00519ff6 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 00519ffc */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051a002 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a00e;
    /* 0051a004 */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
mi_0051a00e:;
    /* 0051a00e */
    a=MI_U32((0x00eae00cu));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a014 */
    if(!(f->flags&MI_ZF))goto mi_00519ea7;
mi_0051a01a:;
    /* 0051a01a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051a01b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051a01c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051a01d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a01f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051a020 */
    f->r[4]+=4;return;
mi_0051a021:;
    /* 0051a021 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a023 */
    if(mi_signed_less(f->flags))goto mi_0051a01a;
    /* 0051a025 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051a026 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051a027 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051a028 */
    f->r[0]=(0x00000001u+f->r[2]);
    /* 0051a02b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051a02c */
    f->r[4]+=4;return;
}

void restore_transport_C_0051a220(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051a220 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a221 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a222 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a223 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a224 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a226 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a22b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a230 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a235u;f->boundary(0x0051a230u,0x0040f8f0u,f);
    /* 0051a235 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051a239 */
    f->r[3]=MI_U32((0x0000002cu+f->r[4]));
    /* 0051a23d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a240 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051a243 */
    f->r[5]=(0xffffffffu+f->r[3]);
    /* 0051a246 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051a249 */
    f->r[1]=MI_U32((0x00eadfd4u));
    /* 0051a24f */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051a252 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0051a257 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051a25a */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0051a25e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051a260 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051a261 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051a266 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 0051a26c */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051a277 */
    f->r[1]=MI_U32((0x00eae004u));
    /* 0051a27d */
    f->r[0]=0x00000001u;
    /* 0051a282 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051a288 */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
    /* 0051a292 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a297:;
    /* 0051a297 */
    f->r[2]=MI_U32((0x00eae00cu));
    /* 0051a29d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0051a29f */
    f->r[1]=MI_U32((0x00f203a0u+f->r[2]*4u));
    /* 0051a2a6 */
    f->r[2]=f->r[5];
    /* 0051a2a8 */
    if((f->flags&MI_ZF))goto mi_0051a2c3;
    /* 0051a2aa */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 0051a2ae */
    f->r[6]=(0x00000000u+f->r[6]+f->r[2]*4u);
mi_0051a2b1:;
    /* 0051a2b1 */
    a=MI_U32((0x00000000u+f->r[6]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051a2b3 */
    if((f->flags&MI_ZF))goto mi_0051a411;
    /* 0051a2b9 */
    f->r[7]=f->r[2];
    /* 0051a2bb */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051a2bc */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051a2bf */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0051a2c1 */
    if(!(f->flags&MI_ZF))goto mi_0051a2b1;
mi_0051a2c3:;
    /* 0051a2c3 */
    f->r[7]=MI_U32((0x00eed760u+f->r[1]*4u));
    /* 0051a2ca */
    f->r[6]=(0xffffff1cu+f->r[1]);
    /* 0051a2d0 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051a2d1 */
    f->r[2]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051a2d8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a2da */
    if(!(f->flags&MI_ZF))goto mi_0051a312;
    /* 0051a2dc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ebaa3cu+f->r[1])))&0xffu)<<0);
    /* 0051a2e2 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a2e4 */
    if(mi_signed_less(f->flags))goto mi_0051a312;
    /* 0051a2e6 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a2e9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a312;
    /* 0051a2eb */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0051a2f2 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051a2f7 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a2fe */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a2ff */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a304 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a309 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a312;
    /* 0051a30b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a30d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a312:;
    /* 0051a312 */
    f->r[6]=(0x00000001u+f->r[1]);
    /* 0051a315 */
    f->r[2]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051a31c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a31e */
    if(!(f->flags&MI_ZF))goto mi_0051a356;
    /* 0051a320 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ebab21u+f->r[1])))&0xffu)<<0);
    /* 0051a326 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a328 */
    if(mi_signed_less(f->flags))goto mi_0051a356;
    /* 0051a32a */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a32d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a356;
    /* 0051a32f */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0051a336 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051a33b */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a342 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a343 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a348 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a34d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a356;
    /* 0051a34f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a351 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a356:;
    /* 0051a356 */
    f->r[6]=(0x000000e4u+f->r[1]);
    /* 0051a35c */
    f->r[2]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051a363 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a365 */
    if(!(f->flags&MI_ZF))goto mi_0051a39d;
    /* 0051a367 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ebac04u+f->r[1])))&0xffu)<<0);
    /* 0051a36d */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a36f */
    if(mi_signed_less(f->flags))goto mi_0051a39d;
    /* 0051a371 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a374 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a39d;
    /* 0051a376 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[7]);
    /* 0051a37d */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051a382 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051a389 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a38a */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a38f */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a394 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a39d;
    /* 0051a396 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a398 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a39d:;
    /* 0051a39d */
    f->r[2]=MI_U32((0x00eed75cu+f->r[1]*4u));
    /* 0051a3a4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a3a6 */
    if(!(f->flags&MI_ZF))goto mi_0051a3df;
    /* 0051a3a8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ebab1fu+f->r[1])))&0xffu)<<0);
    /* 0051a3ae */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051a3b0 */
    if(mi_signed_less(f->flags))goto mi_0051a3df;
    /* 0051a3b2 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051a3b5 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051a3df;
    /* 0051a3b7 */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[7]);
    /* 0051a3be */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051a3c3 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051a3c4 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0051a3cb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051a3cc */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a3d1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051a3d6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a3df;
    /* 0051a3d8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a3da */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051a3df:;
    /* 0051a3df */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051a3e5 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051a3e6 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051a3ec */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051a3f2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051a3fe;
    /* 0051a3f4 */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
mi_0051a3fe:;
    /* 0051a3fe */
    a=MI_U32((0x00eae00cu));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051a404 */
    if(!(f->flags&MI_ZF))goto mi_0051a297;
mi_0051a40a:;
    /* 0051a40a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051a40b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051a40c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051a40d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051a40f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051a410 */
    f->r[4]+=4;return;
mi_0051a411:;
    /* 0051a411 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051a413 */
    if(mi_signed_less(f->flags))goto mi_0051a40a;
    /* 0051a415 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051a416 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051a417 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051a418 */
    f->r[0]=(0x00000001u+f->r[2]);
    /* 0051a41b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051a41c */
    f->r[4]+=4;return;
}

void restore_transport_C_0051a9d0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051a9d0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9d1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9d2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9d3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9d4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0051a9d6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9d7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9d8 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9dd */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051a9e2 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0051a9e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051a9ebu;f->boundary(0x0051a9e6u,0x0040f8f0u,f);
    /* 0051a9eb */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 0051a9ef */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 0051a9f3 */
    f->r[7]=MI_U32((0x00eadfd4u));
    /* 0051a9f9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051a9fc */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051a9ff */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0051aa03 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051aa06 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051aa09 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0051aa0e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051aa11 */
    f->r[2]=MI_U32((0x00eadfd8u));
    /* 0051aa17 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051aa19 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 0051aa1d */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0051aa1e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051aa1f */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051aa24 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[7]);
    /* 0051aa2a */
    MI_U32((0x00eadfd8u))=(uint32_t)(f->r[2]);
    /* 0051aa30 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051aa3b */
    f->r[1]=MI_U32((0x00eae004u));
    /* 0051aa41 */
    f->r[0]=0x00000001u;
    /* 0051aa46 */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0051aa48 */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051aa4e */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[6]);
    /* 0051aa54 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051aa59 */
    if(!(f->flags&MI_ZF))goto mi_0051ad6c;
mi_0051aa5f:;
    /* 0051aa5f */
    f->r[6]=MI_U32((0x00eae00cu));
    /* 0051aa65 */
    f->r[3]=f->r[6];
    /* 0051aa67 */
    f->r[2]=MI_U32((0x00f203a0u+f->r[3]*4u));
    /* 0051aa6e */
    f->r[1]=MI_U32((0x00eed760u+f->r[2]*4u));
    /* 0051aa75 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0051aa79 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0051aa7b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ab1b;
    /* 0051aa81 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0051aa83 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051aab8;
    /* 0051aa85 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051aa86 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051aa88 */
    if(!mi_signed_less(f->flags))goto mi_0051ab17;
    /* 0051aa8e */
    f->r[7]=(0x00f203a0u+f->r[6]*4u);
mi_0051aa95:;
    /* 0051aa95 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 0051aa97 */
    f->r[2]=MI_U32((0x00eed760u+f->r[2]*4u));
    /* 0051aa9e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051aaa0 */
    if(!mi_signed_less(f->flags))goto mi_0051aaae;
    /* 0051aaa2 */
    f->r[5]=MI_U32((0x00000010u+f->r[4]));
    /* 0051aaa6 */
    f->r[3]=f->r[6];
    /* 0051aaa8 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051aaaa */
    f->r[1]=f->r[2];
    /* 0051aaac */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ab17;
mi_0051aaae:;
    /* 0051aaae */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051aaaf */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051aab2 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051aab4 */
    if(mi_signed_less(f->flags))goto mi_0051aa95;
    /* 0051aab6 */
    goto mi_0051ab17;
mi_0051aab8:;
    /* 0051aab8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051aab9 */
    a=f->r[6];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051aabf */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051aaed;
    /* 0051aac1 */
    f->r[7]=(0x00f203a0u+f->r[6]*4u);
mi_0051aac8:;
    /* 0051aac8 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 0051aaca */
    f->r[2]=MI_U32((0x00eed760u+f->r[2]*4u));
    /* 0051aad1 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051aad3 */
    if(!mi_signed_less(f->flags))goto mi_0051aae1;
    /* 0051aad5 */
    f->r[5]=MI_U32((0x00000010u+f->r[4]));
    /* 0051aad9 */
    f->r[3]=f->r[6];
    /* 0051aadb */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051aadd */
    f->r[1]=f->r[2];
    /* 0051aadf */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ab17;
mi_0051aae1:;
    /* 0051aae1 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051aae4 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051aae5 */
    a=f->r[7];b=0x00f52fdcu;t=mi_alu(f,a,b,32,1);
    /* 0051aaeb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051aac8;
mi_0051aaed:;
    /* 0051aaed */
    a=f->r[1];b=MI_U32((0x00000010u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0051aaf1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ab17;
    /* 0051aaf3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0051aaf5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051aaf7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ab17;
    /* 0051aaf9 */
    f->r[7]=0x00f203a0u;
mi_0051aafe:;
    /* 0051aafe */
    f->r[6]=MI_U32((0x00000000u+f->r[7]));
    /* 0051ab00 */
    f->r[6]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051ab07 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051ab09 */
    if(!mi_signed_less(f->flags))goto mi_0051ab0f;
    /* 0051ab0b */
    f->r[3]=f->r[2];
    /* 0051ab0d */
    f->r[1]=f->r[6];
mi_0051ab0f:;
    /* 0051ab0f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051ab10 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0051ab13 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051ab15 */
    if(mi_signed_less(f->flags))goto mi_0051aafe;
mi_0051ab17:;
    /* 0051ab17 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0051ab1b:;
    /* 0051ab1b */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051ab21 */
    f->r[5]=MI_U32((0x00f203a0u+f->r[3]*4u));
    /* 0051ab28 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051ab2a */
    if((f->flags&MI_ZF))goto mi_0051ab3a;
    /* 0051ab2c */
    f->r[1]=MI_U32((0x00f203a0u+f->r[1]*4u));
    /* 0051ab33 */
    MI_U32((0x00f203a0u+f->r[3]*4u))=(uint32_t)(f->r[1]);
mi_0051ab3a:;
    /* 0051ab3a */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0051ab3e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051ab40 */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 0051ab43 */
    if((f->flags&MI_ZF))goto mi_0051ab5e;
    /* 0051ab45 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0051ab49 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
mi_0051ab4c:;
    /* 0051ab4c */
    a=MI_U32((0x00000000u+f->r[2]));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0051ab4e */
    if((f->flags&MI_ZF))goto mi_0051af82;
    /* 0051ab54 */
    f->r[6]=f->r[1];
    /* 0051ab56 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051ab57 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0051ab5a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0051ab5c */
    if(!(f->flags&MI_ZF))goto mi_0051ab4c;
mi_0051ab5e:;
    /* 0051ab5e */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 0051ab62 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 0051ab66 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051ab67 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0051ab69 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0051ab6d */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0051ad64;
    /* 0051ab73 */
    f->r[3]=MI_U32((0x00eed760u+f->r[5]*4u));
    /* 0051ab7a */
    f->r[6]=(0xffffff1cu+f->r[5]);
    /* 0051ab80 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0051ab81 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051ab88 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051ab8a */
    if(!(f->flags&MI_ZF))goto mi_0051abed;
    /* 0051ab8c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ab8d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ab92u;f->boundary(0x0051ab8du,0x0051afa0u,f);
    /* 0051ab92 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051ab95 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051ab97 */
    if((f->flags&MI_ZF))goto mi_0051abe8;
    /* 0051ab99 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec754cu+f->r[5])))&0xffu)<<0);
    /* 0051ab9f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051aba1 */
    if(mi_signed_less(f->flags))goto mi_0051abe8;
    /* 0051aba3 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051aba5 */
    if(!mi_signed_less(f->flags))goto mi_0051abe8;
    /* 0051aba7 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051aba9 */
    if(!(f->flags&MI_ZF))goto mi_0051abb2;
    /* 0051abab */
    f->r[0]=0x00000028u;
    /* 0051abb0 */
    goto mi_0051abbd;
mi_0051abb2:;
    /* 0051abb2 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051abb4 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051abb6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051abb8 */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051abba */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051abbd:;
    /* 0051abbd */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051abbf */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051abc6 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051abcb */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051abd2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051abd3 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051abd8 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051abdd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051abed;
    /* 0051abdf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051abe1 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051abe6 */
    goto mi_0051abed;
mi_0051abe8:;
    /* 0051abe8 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051abed:;
    /* 0051abed */
    f->r[6]=(0x00000001u+f->r[5]);
    /* 0051abf0 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051abf7 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051abf9 */
    if(!(f->flags&MI_ZF))goto mi_0051ac5c;
    /* 0051abfb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051abfc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ac01u;f->boundary(0x0051abfcu,0x0051afa0u,f);
    /* 0051ac01 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051ac04 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051ac06 */
    if((f->flags&MI_ZF))goto mi_0051ac57;
    /* 0051ac08 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec7631u+f->r[5])))&0xffu)<<0);
    /* 0051ac0e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051ac10 */
    if(mi_signed_less(f->flags))goto mi_0051ac57;
    /* 0051ac12 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051ac14 */
    if(!mi_signed_less(f->flags))goto mi_0051ac57;
    /* 0051ac16 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051ac18 */
    if(!(f->flags&MI_ZF))goto mi_0051ac21;
    /* 0051ac1a */
    f->r[0]=0x00000028u;
    /* 0051ac1f */
    goto mi_0051ac2c;
mi_0051ac21:;
    /* 0051ac21 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ac23 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ac25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051ac27 */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ac29 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051ac2c:;
    /* 0051ac2c */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051ac2e */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051ac35 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051ac3a */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051ac41 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051ac42 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051ac47 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ac4c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ac5c;
    /* 0051ac4e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051ac50 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ac55 */
    goto mi_0051ac5c;
mi_0051ac57:;
    /* 0051ac57 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051ac5c:;
    /* 0051ac5c */
    f->r[6]=(0x000000e4u+f->r[5]);
    /* 0051ac62 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051ac69 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051ac6b */
    if(!(f->flags&MI_ZF))goto mi_0051acce;
    /* 0051ac6d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ac6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ac73u;f->boundary(0x0051ac6eu,0x0051afa0u,f);
    /* 0051ac73 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051ac76 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051ac78 */
    if((f->flags&MI_ZF))goto mi_0051acc9;
    /* 0051ac7a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec7714u+f->r[5])))&0xffu)<<0);
    /* 0051ac80 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051ac82 */
    if(mi_signed_less(f->flags))goto mi_0051acc9;
    /* 0051ac84 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051ac86 */
    if(!mi_signed_less(f->flags))goto mi_0051acc9;
    /* 0051ac88 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051ac8a */
    if(!(f->flags&MI_ZF))goto mi_0051ac93;
    /* 0051ac8c */
    f->r[0]=0x00000028u;
    /* 0051ac91 */
    goto mi_0051ac9e;
mi_0051ac93:;
    /* 0051ac93 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ac95 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ac97 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051ac99 */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ac9b */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051ac9e:;
    /* 0051ac9e */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051aca0 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 0051aca7 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051acac */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051acb3 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051acb4 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051acb9 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051acbe */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051acce;
    /* 0051acc0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051acc2 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051acc7 */
    goto mi_0051acce;
mi_0051acc9:;
    /* 0051acc9 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051acce:;
    /* 0051acce */
    f->r[1]=MI_U32((0x00eed75cu+f->r[5]*4u));
    /* 0051acd5 */
    f->r[6]=(0x00eed75cu+f->r[5]*4u);
    /* 0051acdc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051acde */
    if(!(f->flags&MI_ZF))goto mi_0051ad3f;
    /* 0051ace0 */
    f->r[7]=(0xffffffffu+f->r[5]);
    /* 0051ace3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ace4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ace9u;f->boundary(0x0051ace4u,0x0051afa0u,f);
    /* 0051ace9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051acec */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051acee */
    if((f->flags&MI_ZF))goto mi_0051ad3a;
    /* 0051acf0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec762fu+f->r[5])))&0xffu)<<0);
    /* 0051acf6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051acf8 */
    if(mi_signed_less(f->flags))goto mi_0051ad3a;
    /* 0051acfa */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0051acfc */
    if(!mi_signed_less(f->flags))goto mi_0051ad3a;
    /* 0051acfe */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0051ad00 */
    if(!(f->flags&MI_ZF))goto mi_0051ad09;
    /* 0051ad02 */
    f->r[0]=0x00000028u;
    /* 0051ad07 */
    goto mi_0051ad14;
mi_0051ad09:;
    /* 0051ad09 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ad0b */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ad0d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0051ad0f */
    a=(f->r[0]&0xffu);b=0x000000f6u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0051ad11 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0051ad14:;
    /* 0051ad14 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051ad16 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0051ad18 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051ad1d */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051ad24 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051ad25 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051ad2a */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ad2f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ad3f;
    /* 0051ad31 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051ad33 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ad38 */
    goto mi_0051ad3f;
mi_0051ad3a:;
    /* 0051ad3a */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051ad3f:;
    /* 0051ad3f */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051ad45 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051ad46 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051ad4c */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051ad52 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ad5c;
    /* 0051ad54 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051ad56 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
mi_0051ad5c:;
    /* 0051ad5c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051ad5e */
    if(!(f->flags&MI_ZF))goto mi_0051aa5f;
mi_0051ad64:;
    /* 0051ad64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051ad65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051ad66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051ad67 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051ad69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051ad6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0051ad6b */
    f->r[4]+=4;return;
mi_0051ad6c:;
    /* 0051ad6c */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0051ad70 */
    goto mi_0051ad7a;
mi_0051ad72:;
    /* 0051ad72 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0051ad76 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
mi_0051ad7a:;
    /* 0051ad7a */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051ad80 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051ad82 */
    f->r[6]=MI_U32((0x00f203a0u+f->r[1]*4u));
    /* 0051ad89 */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 0051ad8c */
    if((f->flags&MI_ZF))goto mi_0051ada7;
    /* 0051ad8e */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0051ad92 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
mi_0051ad95:;
    /* 0051ad95 */
    a=MI_U32((0x00000000u+f->r[2]));b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0051ad97 */
    if((f->flags&MI_ZF))goto mi_0051af82;
    /* 0051ad9d */
    f->r[7]=f->r[1];
    /* 0051ad9f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051ada0 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0051ada3 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0051ada5 */
    if(!(f->flags&MI_ZF))goto mi_0051ad95;
mi_0051ada7:;
    /* 0051ada7 */
    f->r[5]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051adae */
    f->r[7]=(0xffffff1cu+f->r[6]);
    /* 0051adb4 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0051adb5 */
    f->r[1]=MI_U32((0x00eed760u+f->r[7]*4u));
    /* 0051adbc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051adbe */
    if(!(f->flags&MI_ZF))goto mi_0051ae19;
    /* 0051adc0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051adc1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051adc6u;f->boundary(0x0051adc1u,0x0051afa0u,f);
    /* 0051adc6 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051adc9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051adcb */
    if((f->flags&MI_ZF))goto mi_0051ae14;
    /* 0051adcd */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec754cu+f->r[6])))&0xffu)<<0);
    /* 0051add3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051add5 */
    if((f->flags&MI_ZF))goto mi_0051adeb;
    /* 0051add7 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051add9 */
    if((f->flags&MI_ZF))goto mi_0051adeb;
    /* 0051addb */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051addd */
    if(!(f->flags&MI_ZF))goto mi_0051ae14;
    /* 0051addf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[7]*2u));
    /* 0051ade7 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0051ade9 */
    if(!(f->flags&MI_ZF))goto mi_0051ae14;
mi_0051adeb:;
    /* 0051adeb */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 0051adf2 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051adf7 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051adfe */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051adff */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051ae04 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ae09 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ae19;
    /* 0051ae0b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051ae0d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ae12 */
    goto mi_0051ae19;
mi_0051ae14:;
    /* 0051ae14 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051ae19:;
    /* 0051ae19 */
    f->r[7]=(0x00000001u+f->r[6]);
    /* 0051ae1c */
    f->r[1]=MI_U32((0x00eed760u+f->r[7]*4u));
    /* 0051ae23 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051ae25 */
    if(!(f->flags&MI_ZF))goto mi_0051ae80;
    /* 0051ae27 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ae28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ae2du;f->boundary(0x0051ae28u,0x0051afa0u,f);
    /* 0051ae2d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051ae30 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051ae32 */
    if((f->flags&MI_ZF))goto mi_0051ae7b;
    /* 0051ae34 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec7631u+f->r[6])))&0xffu)<<0);
    /* 0051ae3a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051ae3c */
    if((f->flags&MI_ZF))goto mi_0051ae52;
    /* 0051ae3e */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051ae40 */
    if((f->flags&MI_ZF))goto mi_0051ae52;
    /* 0051ae42 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051ae44 */
    if(!(f->flags&MI_ZF))goto mi_0051ae7b;
    /* 0051ae46 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6feu+f->r[6]*2u));
    /* 0051ae4e */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0051ae50 */
    if(!(f->flags&MI_ZF))goto mi_0051ae7b;
mi_0051ae52:;
    /* 0051ae52 */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 0051ae59 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051ae5e */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051ae65 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051ae66 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051ae6b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ae70 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051ae80;
    /* 0051ae72 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051ae74 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051ae79 */
    goto mi_0051ae80;
mi_0051ae7b:;
    /* 0051ae7b */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051ae80:;
    /* 0051ae80 */
    f->r[7]=(0x000000e4u+f->r[6]);
    /* 0051ae86 */
    f->r[1]=MI_U32((0x00eed760u+f->r[7]*4u));
    /* 0051ae8d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051ae8f */
    if(!(f->flags&MI_ZF))goto mi_0051aeea;
    /* 0051ae91 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051ae92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051ae97u;f->boundary(0x0051ae92u,0x0051afa0u,f);
    /* 0051ae97 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051ae9a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051ae9c */
    if((f->flags&MI_ZF))goto mi_0051aee5;
    /* 0051ae9e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec7714u+f->r[6])))&0xffu)<<0);
    /* 0051aea4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051aea6 */
    if((f->flags&MI_ZF))goto mi_0051aebc;
    /* 0051aea8 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051aeaa */
    if((f->flags&MI_ZF))goto mi_0051aebc;
    /* 0051aeac */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051aeae */
    if(!(f->flags&MI_ZF))goto mi_0051aee5;
    /* 0051aeb0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e8c4u+f->r[6]*2u));
    /* 0051aeb8 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0051aeba */
    if(!(f->flags&MI_ZF))goto mi_0051aee5;
mi_0051aebc:;
    /* 0051aebc */
    MI_U32((0x00eed760u+f->r[7]*4u))=(uint32_t)(f->r[5]);
    /* 0051aec3 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051aec8 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[7]);
    /* 0051aecf */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051aed0 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051aed5 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051aeda */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051aeea;
    /* 0051aedc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051aede */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051aee3 */
    goto mi_0051aeea;
mi_0051aee5:;
    /* 0051aee5 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051aeea:;
    /* 0051aeea */
    f->r[1]=MI_U32((0x00eed75cu+f->r[6]*4u));
    /* 0051aef1 */
    f->r[7]=(0x00eed75cu+f->r[6]*4u);
    /* 0051aef8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051aefa */
    if(!(f->flags&MI_ZF))goto mi_0051af55;
    /* 0051aefc */
    f->r[3]=(0xffffffffu+f->r[6]);
    /* 0051aeff */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051af00 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051af05u;f->boundary(0x0051af00u,0x0051afa0u,f);
    /* 0051af05 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051af08 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051af0a */
    if((f->flags&MI_ZF))goto mi_0051af50;
    /* 0051af0c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ec762fu+f->r[6])))&0xffu)<<0);
    /* 0051af12 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051af14 */
    if((f->flags&MI_ZF))goto mi_0051af2c;
    /* 0051af16 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0051af18 */
    if((f->flags&MI_ZF))goto mi_0051af2c;
    /* 0051af1a */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0051af1c */
    if(!(f->flags&MI_ZF))goto mi_0051af50;
    /* 0051af1e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fau+f->r[6]*2u));
    /* 0051af26 */
    a=f->r[0];b=MI_U32((0x00000028u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0051af2a */
    if(!(f->flags&MI_ZF))goto mi_0051af50;
mi_0051af2c:;
    /* 0051af2c */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 0051af2e */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051af33 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[3]);
    /* 0051af3a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051af3b */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051af40 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051af45 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051af55;
    /* 0051af47 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051af49 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051af4e */
    goto mi_0051af55;
mi_0051af50:;
    /* 0051af50 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051af55:;
    /* 0051af55 */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051af5b */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051af5c */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051af62 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051af68 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051af72;
    /* 0051af6a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051af6c */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
mi_0051af72:;
    /* 0051af72 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051af74 */
    if(!(f->flags&MI_ZF))goto mi_0051ad72;
    /* 0051af7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051af7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051af7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051af7d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051af7f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051af80 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0051af81 */
    f->r[4]+=4;return;
mi_0051af82:;
    /* 0051af82 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051af84 */
    if(mi_signed_less(f->flags))goto mi_0051ad64;
    /* 0051af8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051af8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051af8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051af8d */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 0051af90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051af91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0051af92 */
    f->r[4]+=4;return;
}

void restore_transport_C_0051b220(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051b220 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b221 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b222 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b223 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b224 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b226 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b22b */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051b230 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051b235u;f->boundary(0x0051b230u,0x0040f8f0u,f);
    /* 0051b235 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051b239 */
    f->r[7]=MI_U32((0x0000002cu+f->r[4]));
    /* 0051b23d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051b240 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051b243 */
    f->r[3]=(0xffffffffu+f->r[7]);
    /* 0051b246 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051b249 */
    f->r[1]=MI_U32((0x00eadfd4u));
    /* 0051b24f */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051b252 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0051b257 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051b25a */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0051b25e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051b260 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051b261 */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051b266 */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 0051b26c */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051b277 */
    f->r[1]=MI_U32((0x00eae004u));
    /* 0051b27d */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051b283 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051b285 */
    f->r[0]=0x00000001u;
    /* 0051b28a */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051b290 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b295:;
    /* 0051b295 */
    f->r[1]=MI_U32((0x00f203a0u+f->r[1]*4u));
    /* 0051b29c */
    f->r[2]=f->r[3];
    /* 0051b29e */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0051b2a0 */
    if((f->flags&MI_ZF))goto mi_0051b2bb;
    /* 0051b2a2 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 0051b2a6 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[2]*4u);
mi_0051b2a9:;
    /* 0051b2a9 */
    a=MI_U32((0x00000000u+f->r[6]));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0051b2ab */
    if((f->flags&MI_ZF))goto mi_0051b3ec;
    /* 0051b2b1 */
    f->r[5]=f->r[2];
    /* 0051b2b3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0051b2b4 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0051b2b7 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051b2b9 */
    if(!(f->flags&MI_ZF))goto mi_0051b2a9;
mi_0051b2bb:;
    /* 0051b2bb */
    f->r[6]=MI_U32((0x00eed760u+f->r[1]*4u));
    /* 0051b2c2 */
    f->r[2]=(0xffffff1cu+f->r[1]);
    /* 0051b2c8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0051b2c9 */
    f->r[5]=MI_U32((0x00eed760u+f->r[2]*4u));
    /* 0051b2d0 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051b2d2 */
    if(!(f->flags&MI_ZF))goto mi_0051b304;
    /* 0051b2d4 */
    a=MI_U8((0x00ec754cu+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0051b2db */
    if(mi_signed_less(f->flags))goto mi_0051b304;
    /* 0051b2dd */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051b2e4 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051b2e9 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051b2f0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b2f1 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b2f6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b2fb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b304;
    /* 0051b2fd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b2ff */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b304:;
    /* 0051b304 */
    f->r[2]=(0x00000001u+f->r[1]);
    /* 0051b307 */
    f->r[5]=MI_U32((0x00eed760u+f->r[2]*4u));
    /* 0051b30e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051b310 */
    if(!(f->flags&MI_ZF))goto mi_0051b342;
    /* 0051b312 */
    a=MI_U8((0x00ec7631u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0051b319 */
    if(mi_signed_less(f->flags))goto mi_0051b342;
    /* 0051b31b */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051b322 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051b327 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051b32e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b32f */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b334 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b339 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b342;
    /* 0051b33b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b33d */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b342:;
    /* 0051b342 */
    f->r[2]=(0x000000e4u+f->r[1]);
    /* 0051b348 */
    f->r[5]=MI_U32((0x00eed760u+f->r[2]*4u));
    /* 0051b34f */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0051b351 */
    if(!(f->flags&MI_ZF))goto mi_0051b383;
    /* 0051b353 */
    a=MI_U8((0x00ec7714u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0051b35a */
    if(mi_signed_less(f->flags))goto mi_0051b383;
    /* 0051b35c */
    MI_U32((0x00eed760u+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0051b363 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051b368 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0051b36f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b370 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b375 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b37a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b383;
    /* 0051b37c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b37e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b383:;
    /* 0051b383 */
    f->r[2]=MI_U32((0x00eed75cu+f->r[1]*4u));
    /* 0051b38a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051b38c */
    if(!(f->flags&MI_ZF))goto mi_0051b3c0;
    /* 0051b38e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ec762fu+f->r[1])))&0xffu)<<0);
    /* 0051b394 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051b396 */
    if(mi_signed_less(f->flags))goto mi_0051b3c0;
    /* 0051b398 */
    MI_U32((0x00eed75cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0051b39f */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051b3a4 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051b3a5 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 0051b3ac */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051b3ad */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b3b2 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051b3b7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b3c0;
    /* 0051b3b9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b3bb */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051b3c0:;
    /* 0051b3c0 */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051b3c6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051b3c7 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051b3cd */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051b3d3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051b3dd;
    /* 0051b3d5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0051b3d7 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
mi_0051b3dd:;
    /* 0051b3dd */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051b3df */
    if(!(f->flags&MI_ZF))goto mi_0051b295;
mi_0051b3e5:;
    /* 0051b3e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051b3e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051b3e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051b3e8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051b3ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051b3eb */
    f->r[4]+=4;return;
mi_0051b3ec:;
    /* 0051b3ec */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051b3ee */
    if(mi_signed_less(f->flags))goto mi_0051b3e5;
    /* 0051b3f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051b3f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051b3f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051b3f3 */
    f->r[0]=(0x00000001u+f->r[2]);
    /* 0051b3f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051b3f7 */
    f->r[4]+=4;return;
}

void restore_transport_C_0051d6d0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0051d6d0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d6d1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d6d2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d6d3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d6d4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d6d6 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d6db */
    a=0x00eed760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d6e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d6e5u;f->boundary(0x0051d6e0u,0x0040f8f0u,f);
    /* 0051d6e5 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0051d6e9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d6ec */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0051d6ef */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0051d6f2 */
    f->r[1]=MI_U32((0x00eadfd4u));
    /* 0051d6f8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0051d6fb */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0051d700 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0051d703 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0051d707 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0051d709 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051d70a */
    MI_U32((0x00eae004u))=(uint32_t)(f->r[0]);
    /* 0051d70f */
    MI_U32((0x00eadfd4u))=(uint32_t)(f->r[1]);
    /* 0051d715 */
    MI_U32((0x00eed760u+f->r[0]*4u))=(uint32_t)(0x00000001u);
    /* 0051d720 */
    f->r[1]=MI_U32((0x00eae004u));
    /* 0051d726 */
    f->r[0]=0x00000001u;
    /* 0051d72b */
    MI_U32((0x00f203a0u))=(uint32_t)(f->r[1]);
    /* 0051d731 */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
    /* 0051d73b */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
mi_0051d740:;
    /* 0051d740 */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051d746 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0051d74a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0051d74c */
    f->r[7]=MI_U32((0x00f203a0u+f->r[1]*4u));
    /* 0051d753 */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 0051d756 */
    if((f->flags&MI_ZF))goto mi_0051d771;
    /* 0051d758 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0051d75c */
    f->r[2]=(0x00000000u+f->r[2]+f->r[1]*4u);
mi_0051d75f:;
    /* 0051d75f */
    a=MI_U32((0x00000000u+f->r[2]));b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0051d761 */
    if((f->flags&MI_ZF))goto mi_0051d96c;
    /* 0051d767 */
    f->r[6]=f->r[1];
    /* 0051d769 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051d76a */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0051d76d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0051d76f */
    if(!(f->flags&MI_ZF))goto mi_0051d75f;
mi_0051d771:;
    /* 0051d771 */
    f->r[3]=MI_U32((0x00eed760u+f->r[7]*4u));
    /* 0051d778 */
    f->r[5]=(0x00eed760u+f->r[7]*4u);
    /* 0051d77f */
    f->r[6]=(0xffffff1cu+f->r[7]);
    /* 0051d785 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0051d786 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051d78d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d78f */
    if(!(f->flags&MI_ZF))goto mi_0051d7f2;
    /* 0051d791 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d792 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d797u;f->boundary(0x0051d792u,0x0051aff0u,f);
    /* 0051d797 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d79a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d79c */
    if((f->flags&MI_ZF))goto mi_0051d7ed;
    /* 0051d79e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0b6cu+f->r[7])))&0xffu)<<0);
    /* 0051d7a4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d7a6 */
    if(!mi_signed_less(f->flags))goto mi_0051d7bc;
    /* 0051d7a8 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d7aa */
    if((f->flags&MI_ZF))goto mi_0051d7bc;
    /* 0051d7ac */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d7ae */
    if(!(f->flags&MI_ZF))goto mi_0051d7ed;
    /* 0051d7b0 */
    f->r[1]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 0051d7b7 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d7ba */
    if((f->flags&MI_ZF))goto mi_0051d7ed;
mi_0051d7bc:;
    /* 0051d7bc */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d7be */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d7c4;
    /* 0051d7c0 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d7c2 */
    if(!(f->flags&MI_ZF))goto mi_0051d7ed;
mi_0051d7c4:;
    /* 0051d7c4 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0051d7cb */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051d7d0 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d7d7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d7d8 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d7dd */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d7e2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d7f2;
    /* 0051d7e4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d7e6 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d7eb */
    goto mi_0051d7f2;
mi_0051d7ed:;
    /* 0051d7ed */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051d7f2:;
    /* 0051d7f2 */
    f->r[6]=(0x00000001u+f->r[7]);
    /* 0051d7f5 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051d7fc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d7fe */
    if(!(f->flags&MI_ZF))goto mi_0051d861;
    /* 0051d800 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d801 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d806u;f->boundary(0x0051d801u,0x0051aff0u,f);
    /* 0051d806 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d809 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d80b */
    if((f->flags&MI_ZF))goto mi_0051d85c;
    /* 0051d80d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c51u+f->r[7])))&0xffu)<<0);
    /* 0051d813 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d815 */
    if(!mi_signed_less(f->flags))goto mi_0051d82b;
    /* 0051d817 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d819 */
    if((f->flags&MI_ZF))goto mi_0051d82b;
    /* 0051d81b */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d81d */
    if(!(f->flags&MI_ZF))goto mi_0051d85c;
    /* 0051d81f */
    f->r[1]=MI_U32((0x00af5990u+f->r[7]*4u));
    /* 0051d826 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d829 */
    if((f->flags&MI_ZF))goto mi_0051d85c;
mi_0051d82b:;
    /* 0051d82b */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d82d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d833;
    /* 0051d82f */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d831 */
    if(!(f->flags&MI_ZF))goto mi_0051d85c;
mi_0051d833:;
    /* 0051d833 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0051d83a */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051d83f */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d846 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d847 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d84c */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d851 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d861;
    /* 0051d853 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d855 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d85a */
    goto mi_0051d861;
mi_0051d85c:;
    /* 0051d85c */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051d861:;
    /* 0051d861 */
    f->r[6]=(0x000000e4u+f->r[7]);
    /* 0051d867 */
    f->r[1]=MI_U32((0x00eed760u+f->r[6]*4u));
    /* 0051d86e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d870 */
    if(!(f->flags&MI_ZF))goto mi_0051d8d3;
    /* 0051d872 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d873 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d878u;f->boundary(0x0051d873u,0x0051aff0u,f);
    /* 0051d878 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d87b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d87d */
    if((f->flags&MI_ZF))goto mi_0051d8ce;
    /* 0051d87f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0d34u+f->r[7])))&0xffu)<<0);
    /* 0051d885 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d887 */
    if(!mi_signed_less(f->flags))goto mi_0051d89d;
    /* 0051d889 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d88b */
    if((f->flags&MI_ZF))goto mi_0051d89d;
    /* 0051d88d */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d88f */
    if(!(f->flags&MI_ZF))goto mi_0051d8ce;
    /* 0051d891 */
    f->r[1]=MI_U32((0x00af5d1cu+f->r[7]*4u));
    /* 0051d898 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d89b */
    if((f->flags&MI_ZF))goto mi_0051d8ce;
mi_0051d89d:;
    /* 0051d89d */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d89f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d8a5;
    /* 0051d8a1 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d8a3 */
    if(!(f->flags&MI_ZF))goto mi_0051d8ce;
mi_0051d8a5:;
    /* 0051d8a5 */
    MI_U32((0x00eed760u+f->r[6]*4u))=(uint32_t)(f->r[3]);
    /* 0051d8ac */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051d8b1 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d8b8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d8b9 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d8be */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d8c3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d8d3;
    /* 0051d8c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d8c7 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d8cc */
    goto mi_0051d8d3;
mi_0051d8ce:;
    /* 0051d8ce */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051d8d3:;
    /* 0051d8d3 */
    f->r[1]=MI_U32((0xfffffffcu+f->r[5]));
    /* 0051d8d6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d8d8 */
    if(!(f->flags&MI_ZF))goto mi_0051d93a;
    /* 0051d8da */
    f->r[6]=(0xffffffffu+f->r[7]);
    /* 0051d8dd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0051d8de */
    f->r[4]-=4;MI_U32(f->r[4])=0x0051d8e3u;f->boundary(0x0051d8deu,0x0051aff0u,f);
    /* 0051d8e3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0051d8e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0051d8e8 */
    if((f->flags&MI_ZF))goto mi_0051d935;
    /* 0051d8ea */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ee0c4fu+f->r[7])))&0xffu)<<0);
    /* 0051d8f0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0051d8f2 */
    if(!mi_signed_less(f->flags))goto mi_0051d908;
    /* 0051d8f4 */
    a=(f->r[0]&0xffu);b=0x000000fdu;t=mi_alu(f,a,b,8,1);
    /* 0051d8f6 */
    if((f->flags&MI_ZF))goto mi_0051d908;
    /* 0051d8f8 */
    a=(f->r[0]&0xffu);b=0x000000ffu;t=mi_alu(f,a,b,8,1);
    /* 0051d8fa */
    if(!(f->flags&MI_ZF))goto mi_0051d935;
    /* 0051d8fc */
    f->r[1]=MI_U32((0x00af5988u+f->r[7]*4u));
    /* 0051d903 */
    a=((f->r[1]>>8)&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 0051d906 */
    if((f->flags&MI_ZF))goto mi_0051d935;
mi_0051d908:;
    /* 0051d908 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0051d90a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d910;
    /* 0051d90c */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0051d90e */
    if(!(f->flags&MI_ZF))goto mi_0051d935;
mi_0051d910:;
    /* 0051d910 */
    MI_U32((0xfffffffcu+f->r[5]))=(uint32_t)(f->r[3]);
    /* 0051d913 */
    f->r[0]=MI_U32((0x00eae008u));
    /* 0051d918 */
    MI_U32((0x00f203a0u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 0051d91f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0051d920 */
    a=f->r[0];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d925 */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d92a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d93a;
    /* 0051d92c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d92e */
    MI_U32((0x00eae008u))=(uint32_t)(f->r[0]);
    /* 0051d933 */
    goto mi_0051d93a;
mi_0051d935:;
    /* 0051d935 */
    f->r[0]=MI_U32((0x00eae008u));
mi_0051d93a:;
    /* 0051d93a */
    f->r[1]=MI_U32((0x00eae00cu));
    /* 0051d940 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0051d941 */
    a=f->r[1];b=0x0000cb0fu;t=mi_alu(f,a,b,32,1);
    /* 0051d947 */
    MI_U32((0x00eae00cu))=(uint32_t)(f->r[1]);
    /* 0051d94d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0051d959;
    /* 0051d94f */
    MI_U32((0x00eae00cu))=(uint32_t)(0x00000000u);
mi_0051d959:;
    /* 0051d959 */
    a=MI_U32((0x00eae00cu));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0051d95f */
    if(!(f->flags&MI_ZF))goto mi_0051d740;
mi_0051d965:;
    /* 0051d965 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051d966 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051d967 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051d968 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0051d96a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051d96b */
    f->r[4]+=4;return;
mi_0051d96c:;
    /* 0051d96c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0051d96e */
    if(mi_signed_less(f->flags))goto mi_0051d965;
    /* 0051d970 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0051d971 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0051d972 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0051d973 */
    f->r[0]=(0x00000001u+f->r[1]);
    /* 0051d976 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0051d977 */
    f->r[4]+=4;return;
}

#pragma code_seg(".zactors")

/* Whole bazaar food/goods purchases and carrier creation.
 * Jump tables are immutable C data; all calls enter explicit C providers. */
/* Complete native 00496c70: 114 instructions. */
static const uint8_t mi_rom_00496da8[]={0x84,0x6c,0x49,0x00,0x92,0x6c,0x49,0x00,0x99,0x6c,0x49,0x00,0xa0,0x6c,0x49,0x00,0xa7,0x6c,0x49,0x00,0x8b,0x6c,0x49,0x00,0xb5,0x6c,0x49,0x00,0xae,0x6c,0x49,0x00,0xca,0x6c,0x49,0x00,0xd1,0x6c,0x49,0x00,0xbc,0x6c,0x49,0x00,0xc3,0x6c,0x49,0x00};
static __inline uint32_t mi_read_00496c70(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00496da8u&&width<=48u&&(offset=address-0x00496da8u)<=48u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00496da8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_transport_C_00496c70(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[0]=mi_read_00496c70((0x00000010u+f->r[4]),4u);
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00496cd8;
    switch(mi_read_00496c70((0x00496da8u+f->r[0]*4u),4u)){case 0x00496c84u:goto mi_00496c84;case 0x00496c8bu:goto mi_00496c8b;case 0x00496c92u:goto mi_00496c92;case 0x00496c99u:goto mi_00496c99;case 0x00496ca0u:goto mi_00496ca0;case 0x00496ca7u:goto mi_00496ca7;case 0x00496caeu:goto mi_00496cae;case 0x00496cb5u:goto mi_00496cb5;case 0x00496cbcu:goto mi_00496cbc;case 0x00496cc3u:goto mi_00496cc3;case 0x00496ccau:goto mi_00496cca;case 0x00496cd1u:goto mi_00496cd1;default:abort();}
mi_00496c84:;
    f->r[3]=0x00000001u;
    goto mi_00496cdb;
mi_00496c8b:;
    f->r[3]=0x00000006u;
    goto mi_00496cdb;
mi_00496c92:;
    f->r[3]=0x00000002u;
    goto mi_00496cdb;
mi_00496c99:;
    f->r[3]=0x00000003u;
    goto mi_00496cdb;
mi_00496ca0:;
    f->r[3]=0x00000004u;
    goto mi_00496cdb;
mi_00496ca7:;
    f->r[3]=0x00000005u;
    goto mi_00496cdb;
mi_00496cae:;
    f->r[3]=0x00000008u;
    goto mi_00496cdb;
mi_00496cb5:;
    f->r[3]=0x00000007u;
    goto mi_00496cdb;
mi_00496cbc:;
    f->r[3]=0x00000011u;
    goto mi_00496cdb;
mi_00496cc3:;
    f->r[3]=0x00000013u;
    goto mi_00496cdb;
mi_00496cca:;
    f->r[3]=0x0000000fu;
    goto mi_00496cdb;
mi_00496cd1:;
    f->r[3]=0x0000000du;
    goto mi_00496cdb;
mi_00496cd8:;
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
mi_00496cdb:;
    f->r[1]=mi_read_00496c70((0x00eaac28u),4u);
    a=f->r[1];b=0x00001276u;t=mi_imul(f,a,b);f->r[1]=t;
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    f->r[2]=mi_read_00496c70((0x00ea42f8u+f->r[1]*4u),4u);
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    if((f->flags&MI_ZF))goto mi_00496cfb;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
mi_00496cfb:;
    f->r[5]=mi_read_00496c70((0x0000001cu+f->r[4]),4u);
    f->r[2]=f->r[5];
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    f->r[1]=(f->r[1]&0xffff0000u)|(((mi_read_00496c70((0x007999e8u+f->r[2]*8u),2u))&0xffffu)<<0);
    a=(f->r[1]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    if((f->flags&MI_ZF))goto mi_00496d34;
    a=(f->r[1]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    if((f->flags&MI_ZF))goto mi_00496d34;
    f->r[1]=mi_read_00496c70((0x00000018u+f->r[4]),4u);
    f->r[6]=mi_read_00496c70((0x00000014u+f->r[4]),4u);
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[0]=mi_read_00496c70((0x00000028u+f->r[4]),4u);
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496d32u;f->boundary(0x00496d2du,0x00496460u,f);
    goto mi_00496d4b;
mi_00496d34:;
    f->r[2]=mi_read_00496c70((0x00000024u+f->r[4]),4u);
    f->r[6]=mi_read_00496c70((0x00000014u+f->r[4]),4u);
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[0]=mi_read_00496c70((0x0000001cu+f->r[4]),4u);
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496d4bu;f->boundary(0x00496d46u,0x00496550u,f);
mi_00496d4b:;
    f->r[7]=f->r[0];
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00496d5b;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
mi_00496d5b:;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496d61u;f->boundary(0x00496d5cu,0x00496640u,f);
    f->r[1]=mi_read_00496c70((0x0000001cu+f->r[4]),4u);
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496d70u;f->boundary(0x00496d6bu,0x004968a0u,f);
    f->r[0]=mi_read_00496c70((0x00000044u+f->r[4]),4u);
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*2u);
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    f->r[1]=(0x00000000u+f->r[7]+f->r[7]*4u);
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    f->r[2]=mi_read_00496c70((0x00bcd6b4u+f->r[6]*4u),4u);
    f->r[0]=(0x00bcd6b4u+f->r[6]*4u);
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    f->r[0]=0x00000001u;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
}

/* Complete native 00496de0: 95 instructions. */
static const uint8_t mi_rom_00496ee4[]={0xf4,0x6d,0x49,0x00,0x02,0x6e,0x49,0x00,0x09,0x6e,0x49,0x00,0x10,0x6e,0x49,0x00,0x17,0x6e,0x49,0x00,0xfb,0x6d,0x49,0x00,0x25,0x6e,0x49,0x00,0x1e,0x6e,0x49,0x00,0x3a,0x6e,0x49,0x00,0x41,0x6e,0x49,0x00,0x2c,0x6e,0x49,0x00,0x33,0x6e,0x49,0x00};
static __inline uint32_t mi_read_00496de0(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00496ee4u&&width<=48u&&(offset=address-0x00496ee4u)<=48u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00496ee4[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_transport_C_00496de0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[0]=mi_read_00496de0((0x00000010u+f->r[4]),4u);
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00496e48;
    switch(mi_read_00496de0((0x00496ee4u+f->r[0]*4u),4u)){case 0x00496df4u:goto mi_00496df4;case 0x00496dfbu:goto mi_00496dfb;case 0x00496e02u:goto mi_00496e02;case 0x00496e09u:goto mi_00496e09;case 0x00496e10u:goto mi_00496e10;case 0x00496e17u:goto mi_00496e17;case 0x00496e1eu:goto mi_00496e1e;case 0x00496e25u:goto mi_00496e25;case 0x00496e2cu:goto mi_00496e2c;case 0x00496e33u:goto mi_00496e33;case 0x00496e3au:goto mi_00496e3a;case 0x00496e41u:goto mi_00496e41;default:abort();}
mi_00496df4:;
    f->r[3]=0x00000001u;
    goto mi_00496e4b;
mi_00496dfb:;
    f->r[3]=0x00000006u;
    goto mi_00496e4b;
mi_00496e02:;
    f->r[3]=0x00000002u;
    goto mi_00496e4b;
mi_00496e09:;
    f->r[3]=0x00000003u;
    goto mi_00496e4b;
mi_00496e10:;
    f->r[3]=0x00000004u;
    goto mi_00496e4b;
mi_00496e17:;
    f->r[3]=0x00000005u;
    goto mi_00496e4b;
mi_00496e1e:;
    f->r[3]=0x00000008u;
    goto mi_00496e4b;
mi_00496e25:;
    f->r[3]=0x00000007u;
    goto mi_00496e4b;
mi_00496e2c:;
    f->r[3]=0x00000011u;
    goto mi_00496e4b;
mi_00496e33:;
    f->r[3]=0x00000013u;
    goto mi_00496e4b;
mi_00496e3a:;
    f->r[3]=0x0000000fu;
    goto mi_00496e4b;
mi_00496e41:;
    f->r[3]=0x0000000du;
    goto mi_00496e4b;
mi_00496e48:;
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
mi_00496e4b:;
    f->r[1]=mi_read_00496de0((0x00eaac28u),4u);
    a=f->r[1];b=0x00001276u;t=mi_imul(f,a,b);f->r[1]=t;
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    f->r[2]=mi_read_00496de0((0x00ea42f8u+f->r[1]*4u),4u);
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    if((f->flags&MI_ZF))goto mi_00496e6b;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
mi_00496e6b:;
    f->r[2]=mi_read_00496de0((0x00000024u+f->r[4]),4u);
    f->r[5]=mi_read_00496de0((0x0000001cu+f->r[4]),4u);
    f->r[6]=mi_read_00496de0((0x00000014u+f->r[4]),4u);
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[0]=mi_read_00496de0((0x0000001cu+f->r[4]),4u);
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496e86u;f->boundary(0x00496e81u,0x004963b0u,f);
    f->r[7]=f->r[0];
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00496e96;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
mi_00496e96:;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496e9cu;f->boundary(0x00496e97u,0x00496640u,f);
    f->r[1]=mi_read_00496de0((0x0000001cu+f->r[4]),4u);
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496eabu;f->boundary(0x00496ea6u,0x00496660u,f);
    f->r[0]=mi_read_00496de0((0x00000044u+f->r[4]),4u);
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*2u);
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    f->r[1]=(0x00000000u+f->r[7]+f->r[7]*4u);
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    f->r[2]=mi_read_00496de0((0x00bcd6b4u+f->r[6]*4u),4u);
    f->r[0]=(0x00bcd6b4u+f->r[6]*4u);
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    f->r[0]=0x00000001u;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
}

/* Complete native 00496640: 8 instructions. */
/* Native assertion: a valid buyer has no predecessor of its own. The
 * nonzero-link path is the original assertion loop, not a missing return. */
void restore_transport_C_00496640(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=MI_U16((0x00bcd660u+f->r[1]*4u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    if((f->flags&MI_ZF))goto mi_00496659;
mi_00496657:;
    goto mi_00496657;
mi_00496659:;
    f->r[4]+=4;return;
}

/* Complete native 00496460: 78 instructions. */
void restore_transport_C_00496460(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496470u;f->boundary(0x0049646bu,0x0045d510u,f);
    f->r[6]=f->r[0];
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    f->r[2]=f->r[0];
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*4u);
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a2eu+f->r[1]*2u));
    f->r[1]=0x00000320u;
    if((f->flags&MI_ZF))goto mi_0049649d;
    f->r[1]=0x00000258u;
mi_0049649d:;
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004964ad;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    f->r[4]+=4;return;
mi_004964ad:;
    f->r[0]=0x51eb851fu;
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    f->r[0]=f->r[2];
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    a=f->r[6];b=0x00000320u;t=mi_alu(f,a,b,32,1);
    f->r[1]=(0x00000001u+f->r[2]+f->r[0]*1u);
    if(mi_signed_less(f->flags))goto mi_004964cf;
    f->r[0]=0x00000008u;
    goto mi_0049653c;
mi_004964cf:;
    a=f->r[6];b=0x000002bcu;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_004964de;
    f->r[0]=0x00000007u;
    goto mi_0049653c;
mi_004964de:;
    a=f->r[6];b=0x00000258u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_004964ed;
    f->r[0]=0x00000006u;
    goto mi_0049653c;
mi_004964ed:;
    a=f->r[6];b=0x000001f4u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_004964fc;
    f->r[0]=0x00000005u;
    goto mi_0049653c;
mi_004964fc:;
    a=f->r[6];b=0x00000190u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_0049650b;
    f->r[0]=0x00000004u;
    goto mi_0049653c;
mi_0049650b:;
    a=f->r[6];b=0x0000012cu;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_0049651a;
    f->r[0]=0x00000003u;
    goto mi_0049653c;
mi_0049651a:;
    a=f->r[6];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_00496529;
    f->r[0]=0x00000002u;
    goto mi_0049653c;
mi_00496529:;
    a=f->r[6];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_00496535;
    f->r[0]=0x00000001u;
    goto mi_0049653c;
mi_00496535:;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    t=(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
mi_0049653c:;
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496542;
    f->r[0]=f->r[1];
mi_00496542:;
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,3);f->r[0]=t;
    f->r[4]+=4;return;
}

/* Complete native 00496550: 76 instructions. */
void restore_transport_C_00496550(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    f->r[1]=f->r[0];
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*4u);
    f->r[1]=0x00000320u;
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a2eu+f->r[2]*2u));
    if((f->flags&MI_ZF))goto mi_00496578;
    f->r[1]=0x00000258u;
mi_00496578:;
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00496587;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    f->r[4]+=4;return;
mi_00496587:;
    f->r[0]=0x51eb851fu;
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    f->r[0]=f->r[2];
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    f->r[1]=(0x00000001u+f->r[2]+f->r[0]*1u);
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    f->r[2]=f->r[0];
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00799a2eu+f->r[2]*2u)))&0xffffu)<<0);
    a=(f->r[0]&0xffffu);b=0x00000320u;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_004965c1;
    f->r[0]=0x00000008u;
    goto mi_00496626;
mi_004965c1:;
    a=(f->r[0]&0xffffu);b=0x000002bcu;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_004965ce;
    f->r[0]=0x00000007u;
    goto mi_00496626;
mi_004965ce:;
    a=(f->r[0]&0xffffu);b=0x00000258u;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_004965db;
    f->r[0]=0x00000006u;
    goto mi_00496626;
mi_004965db:;
    a=(f->r[0]&0xffffu);b=0x000001f4u;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_004965e8;
    f->r[0]=0x00000005u;
    goto mi_00496626;
mi_004965e8:;
    a=(f->r[0]&0xffffu);b=0x00000190u;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_004965f5;
    f->r[0]=0x00000004u;
    goto mi_00496626;
mi_004965f5:;
    a=(f->r[0]&0xffffu);b=0x0000012cu;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_00496602;
    f->r[0]=0x00000003u;
    goto mi_00496626;
mi_00496602:;
    a=(f->r[0]&0xffffu);b=0x000000c8u;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_0049660f;
    f->r[0]=0x00000002u;
    goto mi_00496626;
mi_0049660f:;
    a=(f->r[0]&0xffffu);b=0x00000064u;t=mi_alu(f,a,b,16,1);
    if(mi_signed_less(f->flags))goto mi_0049661c;
    f->r[0]=0x00000001u;
    goto mi_00496626;
mi_0049661c:;
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    t=(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    f->r[0]=f->r[2];
mi_00496626:;
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0049662c;
    f->r[0]=f->r[1];
mi_0049662c:;
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,3);f->r[0]=t;
    f->r[4]+=4;return;
}

/* Complete native 004963b0: 58 instructions. */
void restore_transport_C_004963b0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    f->r[1]=f->r[0];
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*4u);
    f->r[1]=0x00000190u;
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a2eu+f->r[0]*2u));
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004963de;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    f->r[4]+=4;return;
mi_004963de:;
    f->r[0]=0x51eb851fu;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    f->r[0]=f->r[2];
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    f->r[6]=(0x00000001u+f->r[2]+f->r[0]*1u);
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496401u;f->boundary(0x004963fcu,0x0045d510u,f);
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[0];b=0x00000190u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_00496412;
    f->r[0]=0x00000004u;
    goto mi_00496443;
mi_00496412:;
    a=f->r[0];b=0x0000012cu;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_00496420;
    f->r[0]=0x00000003u;
    goto mi_00496443;
mi_00496420:;
    a=f->r[0];b=0x000000c8u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_0049642e;
    f->r[0]=0x00000002u;
    goto mi_00496443;
mi_0049642e:;
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    if(mi_signed_less(f->flags))goto mi_0049643a;
    f->r[0]=0x00000001u;
    goto mi_00496443;
mi_0049643a:;
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    t=(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    f->r[0]=f->r[1];
mi_00496443:;
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496449;
    f->r[0]=f->r[6];
mi_00496449:;
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,3);f->r[0]=t;
    f->r[4]+=4;return;
}

/* Complete native 004968a0: 242 instructions. */
static const uint8_t mi_rom_00496c18[]={0xb7,0x68,0x49,0x00,0xc2,0x68,0x49,0x00,0xc9,0x68,0x49,0x00,0xd0,0x68,0x49,0x00,0xd7,0x68,0x49,0x00,0xbb,0x68,0x49,0x00,0xe5,0x68,0x49,0x00,0xde,0x68,0x49,0x00,0x08,0x69,0x49,0x00,0x08,0x69,0x49,0x00,0x08,0x69,0x49,0x00,0x08,0x69,0x49,0x00,0x01,0x69,0x49,0x00,0x08,0x69,0x49,0x00,0xfa,0x68,0x49,0x00,0x08,0x69,0x49,0x00,0xec,0x68,0x49,0x00,0x08,0x69,0x49,0x00,0xf3,0x68,0x49,0x00};
static __inline uint32_t mi_read_004968a0(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00496c18u&&width<=76u&&(offset=address-0x00496c18u)<=76u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00496c18[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_transport_C_004968a0(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[2]=mi_read_004968a0((0x00000004u+f->r[4]),4u);
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[0]=(0xffffffffu+f->r[2]);
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00496908;
    switch(mi_read_004968a0((0x00496c18u+f->r[0]*4u),4u)){case 0x004968b7u:goto mi_004968b7;case 0x004968bbu:goto mi_004968bb;case 0x004968c2u:goto mi_004968c2;case 0x004968c9u:goto mi_004968c9;case 0x004968d0u:goto mi_004968d0;case 0x004968d7u:goto mi_004968d7;case 0x004968deu:goto mi_004968de;case 0x004968e5u:goto mi_004968e5;case 0x004968ecu:goto mi_004968ec;case 0x004968f3u:goto mi_004968f3;case 0x004968fau:goto mi_004968fa;case 0x00496901u:goto mi_00496901;case 0x00496908u:goto mi_00496908;default:abort();}
mi_004968b7:;
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    goto mi_0049690b;
mi_004968bb:;
    f->r[3]=0x00000005u;
    goto mi_0049690b;
mi_004968c2:;
    f->r[3]=0x00000001u;
    goto mi_0049690b;
mi_004968c9:;
    f->r[3]=0x00000002u;
    goto mi_0049690b;
mi_004968d0:;
    f->r[3]=0x00000003u;
    goto mi_0049690b;
mi_004968d7:;
    f->r[3]=0x00000004u;
    goto mi_0049690b;
mi_004968de:;
    f->r[3]=0x00000007u;
    goto mi_0049690b;
mi_004968e5:;
    f->r[3]=0x00000006u;
    goto mi_0049690b;
mi_004968ec:;
    f->r[3]=0x0000000au;
    goto mi_0049690b;
mi_004968f3:;
    f->r[3]=0x0000000bu;
    goto mi_0049690b;
mi_004968fa:;
    f->r[3]=0x00000008u;
    goto mi_0049690b;
mi_00496901:;
    f->r[3]=0x00000009u;
    goto mi_0049690b;
mi_00496908:;
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
mi_0049690b:;
    f->r[1]=mi_read_004968a0((0x00000024u+f->r[4]),4u);
    f->r[0]=f->r[1];
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_004968a0((0x007999e8u+f->r[0]*8u),2u))&0xffffu)<<0);
    a=(f->r[0]&0xffffu);b=0x00000047u;t=mi_alu(f,a,b,16,1);
    if((f->flags&MI_ZF))goto mi_00496948;
    a=(f->r[0]&0xffffu);b=0x000000bfu;t=mi_alu(f,a,b,16,1);
    if((f->flags&MI_ZF))goto mi_00496948;
    f->r[7]=mi_read_004968a0((0x00000018u+f->r[4]),4u);
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*4u);
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[1]=mi_read_004968a0((0x00eaac28u),4u);
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496946u;f->boundary(0x00496941u,0x0045b570u,f);
    goto mi_00496964;
mi_00496948:;
    f->r[7]=mi_read_004968a0((0x00000018u+f->r[4]),4u);
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*4u);
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[1]=mi_read_004968a0((0x00eaac28u),4u);
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496964u;f->boundary(0x0049695fu,0x0045e360u,f);
mi_00496964:;
    f->r[0]=mi_read_004968a0((0x0000002cu+f->r[4]),4u);
    f->r[1]=mi_read_004968a0((0x00eaac28u),4u);
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496996u;f->boundary(0x00496991u,0x004b4800u,f);
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x0000003cu+f->r[4]),2u))&0xffffu)<<0);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496c0c;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_004968a0((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x004969efu;f->boundary(0x004969eau,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496c0c;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_004968a0((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496a43u;f->boundary(0x00496a3eu,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496c0c;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_004968a0((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496a97u;f->boundary(0x00496a92u,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496c0c;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_004968a0((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496aebu;f->boundary(0x00496ae6u,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496c0c;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_004968a0((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496b3fu;f->boundary(0x00496b3au,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496c0c;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_004968a0((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496b93u;f->boundary(0x00496b8eu,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496c0c;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_004968a0((0x00eaac28u),4u);
    f->r[7]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_004968a0((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496be3u;f->boundary(0x00496bdeu,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[7]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_004968a0((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
mi_00496c0c:;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    f->r[0]=0x00000001u;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
}

/* Complete native 00496660: 138 instructions. */
static const uint8_t mi_rom_0049684c[]={0x77,0x66,0x49,0x00,0x82,0x66,0x49,0x00,0x89,0x66,0x49,0x00,0x90,0x66,0x49,0x00,0x97,0x66,0x49,0x00,0x7b,0x66,0x49,0x00,0xa5,0x66,0x49,0x00,0x9e,0x66,0x49,0x00,0xc8,0x66,0x49,0x00,0xc8,0x66,0x49,0x00,0xc8,0x66,0x49,0x00,0xc8,0x66,0x49,0x00,0xc1,0x66,0x49,0x00,0xc8,0x66,0x49,0x00,0xba,0x66,0x49,0x00,0xc8,0x66,0x49,0x00,0xac,0x66,0x49,0x00,0xc8,0x66,0x49,0x00,0xb3,0x66,0x49,0x00};
static __inline uint32_t mi_read_00496660(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0049684cu&&width<=76u&&(offset=address-0x0049684cu)<=76u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0049684c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_transport_C_00496660(struct restore_transport_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    f->r[1]=mi_read_00496660((0x00000004u+f->r[4]),4u);
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[0]=(0xffffffffu+f->r[1]);
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004966c8;
    switch(mi_read_00496660((0x0049684cu+f->r[0]*4u),4u)){case 0x00496677u:goto mi_00496677;case 0x0049667bu:goto mi_0049667b;case 0x00496682u:goto mi_00496682;case 0x00496689u:goto mi_00496689;case 0x00496690u:goto mi_00496690;case 0x00496697u:goto mi_00496697;case 0x0049669eu:goto mi_0049669e;case 0x004966a5u:goto mi_004966a5;case 0x004966acu:goto mi_004966ac;case 0x004966b3u:goto mi_004966b3;case 0x004966bau:goto mi_004966ba;case 0x004966c1u:goto mi_004966c1;case 0x004966c8u:goto mi_004966c8;default:abort();}
mi_00496677:;
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    goto mi_004966cb;
mi_0049667b:;
    f->r[3]=0x00000005u;
    goto mi_004966cb;
mi_00496682:;
    f->r[3]=0x00000001u;
    goto mi_004966cb;
mi_00496689:;
    f->r[3]=0x00000002u;
    goto mi_004966cb;
mi_00496690:;
    f->r[3]=0x00000003u;
    goto mi_004966cb;
mi_00496697:;
    f->r[3]=0x00000004u;
    goto mi_004966cb;
mi_0049669e:;
    f->r[3]=0x00000007u;
    goto mi_004966cb;
mi_004966a5:;
    f->r[3]=0x00000006u;
    goto mi_004966cb;
mi_004966ac:;
    f->r[3]=0x0000000au;
    goto mi_004966cb;
mi_004966b3:;
    f->r[3]=0x0000000bu;
    goto mi_004966cb;
mi_004966ba:;
    f->r[3]=0x00000008u;
    goto mi_004966cb;
mi_004966c1:;
    f->r[3]=0x00000009u;
    goto mi_004966cb;
mi_004966c8:;
    a=f->r[3];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[3]=t;
mi_004966cb:;
    f->r[7]=mi_read_00496660((0x00000018u+f->r[4]),4u);
    f->r[2]=mi_read_00496660((0x00eaac28u),4u);
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*4u);
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[1]=mi_read_00496660((0x0000002cu+f->r[4]),4u);
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x004966ebu;f->boundary(0x004966e6u,0x0045b570u,f);
    f->r[0]=mi_read_00496660((0x0000002cu+f->r[4]),4u);
    f->r[2]=mi_read_00496660((0x00eaac28u),4u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60eu+f->r[6]),2u);
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60cu+f->r[6]),2u);
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x0049671au;f->boundary(0x00496715u,0x004b4800u,f);
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_00496660((0x0000004cu+f->r[4]),2u))&0xffffu)<<0);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_00496660((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496840;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_00496660((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496773u;f->boundary(0x0049676eu,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_00496660((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496840;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_00496660((0x00eaac28u),4u);
    f->r[5]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x004967c7u;f->boundary(0x004967c2u,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[5]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read_00496660((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00496840;
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60cu+f->r[6]),2u);
    f->r[2]=mi_read_00496660((0x00eaac28u),4u);
    f->r[7]=f->r[0];
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_00496660((0x00bcd60eu+f->r[6]),2u);
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    a=0x00000042u;f->r[4]-=4;MI_U32(f->r[4])=a;
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    f->r[4]-=4;MI_U32(f->r[4])=0x00496817u;f->boundary(0x00496812u,0x004b4800u,f);
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    MI_U16((0x00bcd660u+f->r[1]))=(uint16_t)((f->r[7]&0xffffu));
    MI_U8((0x00bcd672u+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_00496660((0x00bcd652u+f->r[6]),2u))&0xffffu)<<0);
    MI_U16((0x00bcd652u+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
mi_00496840:;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    f->r[0]=0x00000001u;
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    f->r[4]+=4;return;
}
