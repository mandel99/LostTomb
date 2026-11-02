/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "actor_mission_init.h"
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
static __inline uint32_t mi_alu(struct restore_actor_init_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_actor_init_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_actor_init_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static __inline void mi_idiv32(struct restore_actor_init_frame *f,uint32_t divisor_bits)
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


void restore_actor_init_C_004bb090(struct restore_actor_init_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bb090 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb091 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb092 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb097u;f->boundary(0x004bb092u,0x004b4760u,f);
    /* 004bb097 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb09cu;f->boundary(0x004bb097u,0x004b8c90u,f);
    /* 004bb09c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb09e */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0a0 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0a2 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0a4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb0abu;f->boundary(0x004bb0a6u,0x004b4800u,f);
    /* 004bb0ab */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb0ae */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0b0 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb0b3 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0b5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb0b7 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0b9 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0bb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0bd */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x00000001u);
    /* 004bb0c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb0cau;f->boundary(0x004bb0c5u,0x004b4800u,f);
    /* 004bb0ca */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb0cd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0cf */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb0d2 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0d4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb0d6 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0d8 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0da */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0dc */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x00000002u);
    /* 004bb0e4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb0e9u;f->boundary(0x004bb0e4u,0x004b4800u,f);
    /* 004bb0e9 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb0ec */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0ee */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb0f1 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0f3 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb0f5 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0f7 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0f9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb0fb */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x00000003u);
    /* 004bb103 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb108u;f->boundary(0x004bb103u,0x004b4800u,f);
    /* 004bb108 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb10b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb10d */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb10f */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb111 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb113 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb115 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb11au;f->boundary(0x004bb115u,0x004b4800u,f);
    /* 004bb11a */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb11d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb11f */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb122 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb124 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb126 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb128 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb12a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb12c */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x0000000cu);
    /* 004bb134 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb139u;f->boundary(0x004bb134u,0x004b4800u,f);
    /* 004bb139 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb13c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb13e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb141 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb143 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb145 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb147 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb149 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb14b */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x0000000du);
    /* 004bb153 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb158u;f->boundary(0x004bb153u,0x004b4800u,f);
    /* 004bb158 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb15b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb15d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb160 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb162 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb164 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb166 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb168 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb16a */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x0000000eu);
    /* 004bb172 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb177u;f->boundary(0x004bb172u,0x004b4800u,f);
    /* 004bb177 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb17a */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb17d */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb180 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb182 */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x0000000fu);
    /* 004bb18a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb18c */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb18e */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb190 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb192 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb194 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb199u;f->boundary(0x004bb194u,0x004b4800u,f);
    /* 004bb199 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb19c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb19e */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb1a1 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1a3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb1a5 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1a7 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1a9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1ab */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x00000010u);
    /* 004bb1b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb1b8u;f->boundary(0x004bb1b3u,0x004b4800u,f);
    /* 004bb1b8 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb1bb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1bd */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb1c0 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1c2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb1c4 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1c6 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1c8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1ca */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x00000011u);
    /* 004bb1d2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb1d7u;f->boundary(0x004bb1d2u,0x004b4800u,f);
    /* 004bb1d7 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb1da */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1dc */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb1df */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1e1 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb1e3 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1e5 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1e7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb1e9 */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x00000012u);
    /* 004bb1f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb1f6u;f->boundary(0x004bb1f1u,0x004b4800u,f);
    /* 004bb1f6 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb1f9 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb1fc */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb1ff */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb201 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb203 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb205 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb207 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb209 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb20b */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x00000013u);
    /* 004bb213 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb218u;f->boundary(0x004bb213u,0x004b4800u,f);
    /* 004bb218 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb21b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb21d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb220 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb222 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb224 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb226 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb228 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb22a */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x00000014u);
    /* 004bb232 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb237u;f->boundary(0x004bb232u,0x004b4800u,f);
    /* 004bb237 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb23a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb23c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb23f */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb241 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb243 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb245 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb247 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb249 */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x00000015u);
    /* 004bb251 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb256u;f->boundary(0x004bb251u,0x004b4800u,f);
    /* 004bb256 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb259 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb25b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb25e */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb260 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb262 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb264 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb266 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb268 */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x00000016u);
    /* 004bb270 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb275u;f->boundary(0x004bb270u,0x004b4800u,f);
    /* 004bb275 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb278 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb27b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb27e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb280 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb282 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb284 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb286 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb288 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb28a */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x00000017u);
    /* 004bb292 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb297u;f->boundary(0x004bb292u,0x004b4800u,f);
    /* 004bb297 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb29a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb29c */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb29f */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2a1 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb2a3 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2a5 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2a7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2a9 */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x00000018u);
    /* 004bb2b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb2b6u;f->boundary(0x004bb2b1u,0x004b4800u,f);
    /* 004bb2b6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb2b9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2bb */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb2be */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2c0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb2c2 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2c4 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2c6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2c8 */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x00000019u);
    /* 004bb2d0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb2d5u;f->boundary(0x004bb2d0u,0x004b4800u,f);
    /* 004bb2d5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb2d8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2da */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb2dd */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2df */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb2e1 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2e3 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2e5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2e7 */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x0000001au);
    /* 004bb2ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb2f4u;f->boundary(0x004bb2efu,0x004b4800u,f);
    /* 004bb2f4 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb2f7 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb2fa */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb2fd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb2ff */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb301 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb303 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb305 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb307 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb309 */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x0000001bu);
    /* 004bb311 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb316u;f->boundary(0x004bb311u,0x004b4800u,f);
    /* 004bb316 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb319 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb31b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb31e */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb320 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb322 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb324 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb326 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb328 */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x0000001cu);
    /* 004bb330 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb335u;f->boundary(0x004bb330u,0x004b4800u,f);
    /* 004bb335 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb338 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb33a */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb33d */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb33f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb341 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb343 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb345 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb347 */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x0000001du);
    /* 004bb34f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb354u;f->boundary(0x004bb34fu,0x004b4800u,f);
    /* 004bb354 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb357 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb359 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb35c */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb35e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb360 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb362 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb364 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb366 */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x0000001eu);
    /* 004bb36e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb373u;f->boundary(0x004bb36eu,0x004b4800u,f);
    /* 004bb373 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb376 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb379 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb37c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb37e */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb380 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb382 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb384 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb386 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb388 */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)(0x0000001fu);
    /* 004bb390 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb395u;f->boundary(0x004bb390u,0x004b4800u,f);
    /* 004bb395 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb398 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb39b */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb39e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb3a0 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004bb3a2 */
    MI_U8((0x00bcd603u+f->r[2]*4u))=(uint8_t)(0x00000020u);
mi_004bb3aa:;
    /* 004bb3aa */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb3ac */
    f->r[2]=(f->r[2]&0xffffff00u)|((((f->r[3]&0xffu))&0xffu)<<0);
    /* 004bb3ae */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb3b0 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb3b2 */
    a=(f->r[2]&0xffu);b=0x00000032u;t=mi_alu(f,a,b,8,0);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004bb3b5 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb3b7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb3b9 */
    MI_U8((0x0000001bu+f->r[4]))=(uint8_t)((f->r[2]&0xffu));
    /* 004bb3bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb3c2u;f->boundary(0x004bb3bdu,0x004b4800u,f);
    /* 004bb3c2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0000001bu+f->r[4])))&0xffu)<<0);
    /* 004bb3c6 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb3c9 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb3cc */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb3cf */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb3d1 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004bb3d2 */
    a=f->r[3];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004bb3d5 */
    MI_U8((0x00bcd603u+f->r[1]*4u))=(uint8_t)((f->r[2]&0xffu));
    /* 004bb3dc */
    if(mi_signed_less(f->flags))goto mi_004bb3aa;
    /* 004bb3de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004bb3df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004bb3e0 */
    f->r[4]+=4;return;
}

void restore_actor_init_C_004bb3f0(struct restore_actor_init_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bb3f0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004bb3f3 */
    f->r[0]=MI_U32((0x00784cb0u));
    /* 004bb3f8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb3f9 */
    f->r[1]=f->r[0];
    /* 004bb3fb */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bb401 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004bb404 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb407 */
    f->r[6]=MI_U32((0x005e4c20u+f->r[2]));
    /* 004bb40d */
    f->r[2]=MI_U32((0x005e4c28u+f->r[2]));
    /* 004bb413 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004bb417 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004bb41b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784b94u));
    /* 004bb422 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 004bb425 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004bb428 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004bb42b */
    f->r[2]=MI_U32((0x005e4cb0u+f->r[1]));
    /* 004bb431 */
    f->r[1]=MI_U32((0x005e4cb8u+f->r[1]));
    /* 004bb437 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004bb43b */
    f->r[1]=MI_U32((0x00605a3cu));
    /* 004bb441 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004bb443 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004bb447 */
    if(!(f->flags&MI_ZF))goto mi_004bbc06;
    /* 004bb44d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb44e */
    f->r[3]=0x00784b64u;
    /* 004bb453 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb454 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb455 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
mi_004bb459:;
    /* 004bb459 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 004bb45c */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004bb45f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bb827;
    /* 004bb465 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[3]));
    /* 004bb469 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb46a */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb46f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784b94u));
    /* 004bb476 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 004bb479 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 004bb47c */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004bb480 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb481 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb483 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb486 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bb489 */
    f->r[1]=MI_U32((0x005e4cdcu+f->r[0]));
    /* 004bb48f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb490 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb491 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb496u;f->boundary(0x004bb491u,0x004b6ff0u,f);
    /* 004bb496 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb499 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bb49d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bb49f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bb822;
    /* 004bb4a5 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 004bb4a8 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[3])))&0xffffu)<<0);
    /* 004bb4ac */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004bb4af */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004bb4b7 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bb4ba */
    MI_U16((0x00bcba02u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004bb4c1 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 004bb4c5 */
    MI_U16((0x00bcba04u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004bb4cc */
    MI_U8((0x00bcba3cu+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004bb4d3 */
    MI_U8((0x00bcba31u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 004bb4da */
    MI_U16((0x00bcba16u+f->r[0]))=(uint16_t)(0x00000018u);
    /* 004bb4e3 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004bb4e5 */
    MI_U8((0x00bcb9f1u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004bb4eb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bb822;
    /* 004bb4f1 */
    f->r[5]=MI_U32((0x00d3b1c4u));
    /* 004bb4f7 */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004bb4fd */
    f->r[1]=MI_U32((0x00d3b380u));
mi_004bb503:;
    /* 004bb503 */
    f->r[0]=MI_U32((0x00d3b374u));
    /* 004bb508 */
    MI_U32((0x00d3b1c8u+f->r[5]*4u))=(uint32_t)(f->r[0]);
    /* 004bb50f */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004bb510 */
    a=f->r[5];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bb513 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[5]);
    /* 004bb519 */
    if(mi_signed_less(f->flags))goto mi_004bb525;
    /* 004bb51b */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004bb525:;
    /* 004bb525 */
    f->r[7]=0x0000001fu;
mi_004bb52a:;
    /* 004bb52a */
    f->r[0]=f->r[6];
    /* 004bb52c */
    f->r[2]=f->r[6];
    /* 004bb52e */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004bb531 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb534 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb537 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004bb539 */
    f->r[2]=f->r[1];
    /* 004bb53b */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004bb53e */
    f->r[5]=f->r[1];
    /* 004bb540 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb543 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004bb546 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bb548 */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 004bb54a */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004bb54c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bb54e */
    if((f->flags&MI_ZF))goto mi_004bb556;
    /* 004bb550 */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_004bb556:;
    /* 004bb556 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004bb558 */
    if((f->flags&MI_ZF))goto mi_004bb560;
    /* 004bb55a */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004bb560:;
    /* 004bb560 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004bb561 */
    if(!(f->flags&MI_ZF))goto mi_004bb52a;
    /* 004bb563 */
    f->r[0]=f->r[6];
    /* 004bb565 */
    f->r[2]=f->r[6];
    /* 004bb567 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004bb56b */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb570 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb573 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004bb578 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[2]);
    /* 004bb57e */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004bb584 */
    f->r[0]=f->r[1];
    /* 004bb586 */
    f->r[2]=f->r[1];
    /* 004bb588 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004bb58e */
    a=f->r[6];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bb591 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb596 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb599 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bb59c */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[6]);
    /* 004bb5a2 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[0]);
    /* 004bb5a7 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004bb5ad */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004bb5b3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004bb5b5:;
    /* 004bb5b5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[3]));
    /* 004bb5b9 */
    f->r[0]=MI_U32((0x005cd97cu+f->r[7]*8u));
    /* 004bb5c0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb5c2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bb5c4 */
    f->r[1]=MI_U32((0x005cd978u+f->r[7]*8u));
    /* 004bb5cb */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004bb5ce */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004bb5d1 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb5d4 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004bb5d7 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004bb5dd */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[3]));
    /* 004bb5e0 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004bb5e2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bb5e4 */
    f->r[2]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004bb5eb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb5ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb5f1u;f->boundary(0x004bb5ecu,0x004ec410u,f);
    /* 004bb5f1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb5f4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bb5f6 */
    if(!(f->flags&MI_ZF))goto mi_004bb606;
    /* 004bb5f8 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004bb5f9 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004bb5fc */
    if(mi_signed_less(f->flags))goto mi_004bb600;
    /* 004bb5fe */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_004bb600:;
    /* 004bb600 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004bb601 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004bb604 */
    if(mi_signed_less(f->flags))goto mi_004bb5b5;
mi_004bb606:;
    /* 004bb606 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004bb60b */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004bb611 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004bb618 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bb619 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bb61c */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004bb621 */
    if(mi_signed_less(f->flags))goto mi_004bb62d;
    /* 004bb623 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004bb62d:;
    /* 004bb62d */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 004bb633 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 004bb638 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004bb640:;
    /* 004bb640 */
    f->r[2]=f->r[1];
    /* 004bb642 */
    f->r[6]=f->r[1];
    /* 004bb644 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004bb647 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb64a */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bb64d */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bb64f */
    f->r[6]=f->r[0];
    /* 004bb651 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004bb654 */
    f->r[5]=f->r[0];
    /* 004bb656 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bb659 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004bb65c */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004bb65e */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004bb660 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 004bb662 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004bb664 */
    if((f->flags&MI_ZF))goto mi_004bb66c;
    /* 004bb666 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004bb66c:;
    /* 004bb66c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004bb66e */
    if((f->flags&MI_ZF))goto mi_004bb675;
    /* 004bb670 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004bb675:;
    /* 004bb675 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004bb679 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004bb67a */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004bb67e */
    if(!(f->flags&MI_ZF))goto mi_004bb640;
    /* 004bb680 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 004bb686 */
    f->r[2]=f->r[1];
    /* 004bb688 */
    f->r[6]=f->r[1];
    /* 004bb68a */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bb68d */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 004bb693 */
    f->r[1]=f->r[0];
    /* 004bb695 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb69b */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bb6a1 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 004bb6a7 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 004bb6ad */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb6b0 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 004bb6b5 */
    f->r[1]=f->r[0];
    /* 004bb6b7 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb6ba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb6bb */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bb6be */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[3]));
    /* 004bb6c2 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 004bb6c7 */
    f->r[0]=MI_U32((0x005cd97cu+f->r[7]*8u));
    /* 004bb6ce */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[1]);
    /* 004bb6d4 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bb6d6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[3]));
    /* 004bb6d9 */
    f->r[2]=MI_U32((0x005cd978u+f->r[7]*8u));
    /* 004bb6e0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb6e1 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 004bb6e5 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bb6e7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb6e8 */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bb6eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb6ec */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb6ee */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004bb6f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb6f9u;f->boundary(0x004bb6f4u,0x004b4800u,f);
    /* 004bb6f9 */
    f->r[5]=MI_U32((0x00d3b1c4u));
    /* 004bb6ff */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 004bb705 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb708 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000002cu+f->r[4])))&0xffffu)<<0);
    /* 004bb70d */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004bb710 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004bb712 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb715 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004bb718 */
    MI_U32((0x00d3b1c8u+f->r[5]*4u))=(uint32_t)(f->r[2]);
    /* 004bb71f */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004bb720 */
    MI_U8((0x00bcd630u+f->r[7]))=(uint8_t)(0x00000018u);
    /* 004bb727 */
    MI_U16((0x00bcd658u+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004bb72e */
    a=f->r[5];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bb731 */
    MI_U16((0x00bcd62eu+f->r[7]))=(uint16_t)(0x00000000u);
    /* 004bb73a */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[5]);
    /* 004bb740 */
    if(mi_signed_less(f->flags))goto mi_004bb74a;
    /* 004bb742 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004bb744 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[5]);
mi_004bb74a:;
    /* 004bb74a */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004bb750 */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004bb756 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004bb75e:;
    /* 004bb75e */
    f->r[0]=f->r[6];
    /* 004bb760 */
    f->r[2]=f->r[6];
    /* 004bb762 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004bb765 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb768 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb76b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004bb76d */
    f->r[2]=f->r[1];
    /* 004bb76f */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004bb772 */
    f->r[3]=f->r[1];
    /* 004bb774 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb777 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004bb77a */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bb77c */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 004bb77e */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004bb780 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bb782 */
    if((f->flags&MI_ZF))goto mi_004bb78a;
    /* 004bb784 */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_004bb78a:;
    /* 004bb78a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004bb78c */
    if((f->flags&MI_ZF))goto mi_004bb794;
    /* 004bb78e */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004bb794:;
    /* 004bb794 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004bb798 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bb799 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bb79d */
    if(!(f->flags&MI_ZF))goto mi_004bb75e;
    /* 004bb79f */
    f->r[2]=f->r[6];
    /* 004bb7a1 */
    f->r[0]=f->r[6];
    /* 004bb7a3 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb7a6 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb7ab */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[2]);
    /* 004bb7b1 */
    f->r[2]=f->r[6];
    /* 004bb7b3 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb7b6 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004bb7bb */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004bb7c1 */
    f->r[2]=f->r[1];
    /* 004bb7c3 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb7c9 */
    f->r[3]=0x00000019u;
    /* 004bb7ce */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004bb7d4 */
    f->r[2]=f->r[1];
    /* 004bb7d6 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb7d9 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004bb7df */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004bb7e5 */
    f->r[2]=f->r[1];
    /* 004bb7e7 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb7ea */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004bb7f0 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[2]);
    /* 004bb7f6 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004bb7f7 */
    a=f->r[3];mi_idiv32(f,a);
    /* 004bb7f9 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004bb7fd */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 004bb801 */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bb804 */
    MI_U16((0x00bcd62eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004bb80b */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004bb80f */
    MI_U8((0x00bcd65au+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 004bb815 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bb816 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004bb818 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bb81c */
    if(mi_signed_less(f->flags))goto mi_004bb503;
mi_004bb822:;
    /* 004bb822 */
    f->r[0]=MI_U32((0x00784cb0u));
mi_004bb827:;
    /* 004bb827 */
    a=f->r[3];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004bb82a */
    a=f->r[3];b=0x00784b6cu;t=mi_alu(f,a,b,32,1);
    /* 004bb830 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004bb834 */
    if(mi_signed_less(f->flags))goto mi_004bb459;
    /* 004bb83a */
    f->r[3]=0x00784cc0u;
    /* 004bb83f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
mi_004bb843:;
    /* 004bb843 */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004bb845 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004bb847 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bbbf0;
    /* 004bb84d */
    f->r[2]=MI_U32((0x00000010u+f->r[3]));
    /* 004bb850 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb855 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb856 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 004bb85a */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb85d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb85e */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bb861 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb863 */
    f->r[1]=MI_U32((0x005e4c4cu+f->r[0]));
    /* 004bb869 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb86a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb86b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb870u;f->boundary(0x004bb86bu,0x004b6ff0u,f);
    /* 004bb870 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb873 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bb877 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bb879 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bbbeb;
    /* 004bb87f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[3])))&0xffffu)<<0);
    /* 004bb882 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000010u+f->r[3])))&0xffffu)<<0);
    /* 004bb886 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004bb889 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004bb891 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004bb894 */
    MI_U16((0x00bcba02u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004bb89b */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004bb89f */
    MI_U16((0x00bcba04u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004bb8a6 */
    MI_U8((0x00bcba3cu+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004bb8ad */
    MI_U8((0x00bcba31u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 004bb8b4 */
    MI_U16((0x00bcba16u+f->r[0]))=(uint16_t)(0x00000018u);
    /* 004bb8bd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004bb8bf */
    MI_U8((0x00bcb9f1u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 004bb8c5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bbbeb;
    /* 004bb8cb */
    f->r[5]=MI_U32((0x00d3b1c4u));
    /* 004bb8d1 */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004bb8d7 */
    f->r[1]=MI_U32((0x00d3b380u));
mi_004bb8dd:;
    /* 004bb8dd */
    f->r[0]=MI_U32((0x00d3b374u));
    /* 004bb8e2 */
    MI_U32((0x00d3b1c8u+f->r[5]*4u))=(uint32_t)(f->r[0]);
    /* 004bb8e9 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004bb8ea */
    a=f->r[5];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bb8ed */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[5]);
    /* 004bb8f3 */
    if(mi_signed_less(f->flags))goto mi_004bb8ff;
    /* 004bb8f5 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004bb8ff:;
    /* 004bb8ff */
    f->r[7]=0x0000001fu;
mi_004bb904:;
    /* 004bb904 */
    f->r[0]=f->r[6];
    /* 004bb906 */
    f->r[2]=f->r[6];
    /* 004bb908 */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004bb90b */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb90e */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb911 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004bb913 */
    f->r[2]=f->r[1];
    /* 004bb915 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004bb918 */
    f->r[5]=f->r[1];
    /* 004bb91a */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb91d */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004bb920 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bb922 */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 004bb924 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004bb926 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bb928 */
    if((f->flags&MI_ZF))goto mi_004bb930;
    /* 004bb92a */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_004bb930:;
    /* 004bb930 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004bb932 */
    if((f->flags&MI_ZF))goto mi_004bb93a;
    /* 004bb934 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004bb93a:;
    /* 004bb93a */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004bb93b */
    if(!(f->flags&MI_ZF))goto mi_004bb904;
    /* 004bb93d */
    f->r[0]=f->r[6];
    /* 004bb93f */
    f->r[2]=f->r[6];
    /* 004bb941 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004bb945 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb94a */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb94d */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004bb952 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[2]);
    /* 004bb958 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004bb95e */
    f->r[0]=f->r[1];
    /* 004bb960 */
    f->r[2]=f->r[1];
    /* 004bb962 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004bb968 */
    a=f->r[6];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bb96b */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bb970 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bb973 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bb976 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[6]);
    /* 004bb97c */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[0]);
    /* 004bb981 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004bb987 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[1]);
    /* 004bb98d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004bb98f:;
    /* 004bb98f */
    f->r[0]=MI_U32((0x005cdb7cu+f->r[7]*8u));
    /* 004bb996 */
    f->r[1]=MI_U32((0x00000010u+f->r[3]));
    /* 004bb999 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bb99b */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 004bb99d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb99f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004bb9a2 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004bb9a5 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bb9a8 */
    f->r[0]=MI_U32((0x005cdb78u+f->r[7]*8u));
    /* 004bb9af */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 004bb9b2 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004bb9b8 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bb9ba */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bb9bc */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004bb9c3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bb9c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bb9c9u;f->boundary(0x004bb9c4u,0x004ec410u,f);
    /* 004bb9c9 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bb9cc */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bb9ce */
    if(!(f->flags&MI_ZF))goto mi_004bb9de;
    /* 004bb9d0 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004bb9d1 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004bb9d4 */
    if(mi_signed_less(f->flags))goto mi_004bb9d8;
    /* 004bb9d6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_004bb9d8:;
    /* 004bb9d8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004bb9d9 */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004bb9dc */
    if(mi_signed_less(f->flags))goto mi_004bb98f;
mi_004bb9de:;
    /* 004bb9de */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004bb9e3 */
    f->r[2]=MI_U32((0x00d3b374u));
    /* 004bb9e9 */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 004bb9f0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bb9f1 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bb9f4 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004bb9f9 */
    if(mi_signed_less(f->flags))goto mi_004bba05;
    /* 004bb9fb */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004bba05:;
    /* 004bba05 */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 004bba0b */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 004bba10 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004bba18:;
    /* 004bba18 */
    f->r[2]=f->r[1];
    /* 004bba1a */
    f->r[6]=f->r[1];
    /* 004bba1c */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004bba1f */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bba22 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bba25 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bba27 */
    f->r[6]=f->r[0];
    /* 004bba29 */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004bba2c */
    f->r[5]=f->r[0];
    /* 004bba2e */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bba31 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004bba34 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004bba36 */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004bba38 */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 004bba3a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004bba3c */
    if((f->flags&MI_ZF))goto mi_004bba44;
    /* 004bba3e */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004bba44:;
    /* 004bba44 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004bba46 */
    if((f->flags&MI_ZF))goto mi_004bba4d;
    /* 004bba48 */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004bba4d:;
    /* 004bba4d */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004bba51 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004bba52 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004bba56 */
    if(!(f->flags&MI_ZF))goto mi_004bba18;
    /* 004bba58 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 004bba5e */
    f->r[2]=f->r[1];
    /* 004bba60 */
    f->r[6]=f->r[1];
    /* 004bba62 */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bba65 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 004bba6b */
    f->r[1]=f->r[0];
    /* 004bba6d */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bba73 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bba79 */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 004bba7f */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 004bba84 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 004bba8a */
    f->r[1]=f->r[0];
    /* 004bba8c */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bba8f */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bba92 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 004bba97 */
    f->r[0]=MI_U32((0x00000010u+f->r[3]));
    /* 004bba9a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bba9b */
    f->r[2]=MI_U32((0x005cdb7cu+f->r[7]*8u));
    /* 004bbaa2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bbaa4 */
    f->r[0]=MI_U32((0x005cdb78u+f->r[7]*8u));
    /* 004bbaab */
    f->r[7]=MI_U32((0x00000000u+f->r[3]));
    /* 004bbaad */
    a=f->r[1];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bbab0 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[1]);
    /* 004bbab6 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004bbaba */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004bbabc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbabd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbabe */
    a=f->r[6];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bbac1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbac2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbac4 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[6]);
    /* 004bbaca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bbacfu;f->boundary(0x004bbacau,0x004b4800u,f);
    /* 004bbacf */
    f->r[5]=MI_U32((0x00d3b1c4u));
    /* 004bbad5 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004bbad8 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x0000002cu+f->r[4])))&0xffffu)<<0);
    /* 004bbadd */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bbae0 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004bbae3 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004bbae5 */
    f->r[0]=MI_U32((0x00d3b374u));
    /* 004bbaea */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004bbaed */
    MI_U32((0x00d3b1c8u+f->r[5]*4u))=(uint32_t)(f->r[0]);
    /* 004bbaf4 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004bbaf5 */
    MI_U8((0x00bcd630u+f->r[7]))=(uint8_t)(0x00000018u);
    /* 004bbafc */
    a=f->r[5];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bbaff */
    MI_U16((0x00bcd658u+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004bbb06 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[5]);
    /* 004bbb0c */
    if(mi_signed_less(f->flags))goto mi_004bbb16;
    /* 004bbb0e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004bbb10 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[5]);
mi_004bbb16:;
    /* 004bbb16 */
    f->r[6]=MI_U32((0x00d3b384u));
    /* 004bbb1c */
    f->r[1]=MI_U32((0x00d3b380u));
    /* 004bbb22 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004bbb2a:;
    /* 004bbb2a */
    f->r[0]=f->r[6];
    /* 004bbb2c */
    f->r[2]=f->r[6];
    /* 004bbb2e */
    a=f->r[0];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004bbb31 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bbb34 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbb37 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004bbb39 */
    f->r[2]=f->r[1];
    /* 004bbb3b */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004bbb3e */
    f->r[3]=f->r[1];
    /* 004bbb40 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbb43 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 004bbb46 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bbb48 */
    a=f->r[6];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[6]=t;
    /* 004bbb4a */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004bbb4c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bbb4e */
    if((f->flags&MI_ZF))goto mi_004bbb56;
    /* 004bbb50 */
    a=f->r[6];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[6]=t;
mi_004bbb56:;
    /* 004bbb56 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004bbb58 */
    if((f->flags&MI_ZF))goto mi_004bbb60;
    /* 004bbb5a */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004bbb60:;
    /* 004bbb60 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004bbb64 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bbb65 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bbb69 */
    if(!(f->flags&MI_ZF))goto mi_004bbb2a;
    /* 004bbb6b */
    f->r[2]=f->r[6];
    /* 004bbb6d */
    f->r[0]=f->r[6];
    /* 004bbb6f */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbb72 */
    a=f->r[0];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bbb77 */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[2]);
    /* 004bbb7d */
    f->r[2]=f->r[6];
    /* 004bbb7f */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbb82 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[0]);
    /* 004bbb87 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[2]);
    /* 004bbb8d */
    f->r[2]=f->r[1];
    /* 004bbb8f */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbb95 */
    f->r[3]=0x00000019u;
    /* 004bbb9a */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[2]);
    /* 004bbba0 */
    f->r[2]=f->r[1];
    /* 004bbba2 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbba5 */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[1]);
    /* 004bbbab */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004bbbb1 */
    f->r[2]=f->r[1];
    /* 004bbbb3 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbbb6 */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[6]);
    /* 004bbbbc */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[2]);
    /* 004bbbc2 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004bbbc3 */
    a=f->r[3];mi_idiv32(f,a);
    /* 004bbbc5 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004bbbc9 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 004bbbcd */
    MI_U16((0x00bcd62eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004bbbd4 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 004bbbd8 */
    MI_U8((0x00bcd65au+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 004bbbde */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bbbdf */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004bbbe1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bbbe5 */
    if(mi_signed_less(f->flags))goto mi_004bb8dd;
mi_004bbbeb:;
    /* 004bbbeb */
    f->r[0]=MI_U32((0x00784cb0u));
mi_004bbbf0:;
    /* 004bbbf0 */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004bbbf3 */
    a=f->r[3];b=0x00784cd0u;t=mi_alu(f,a,b,32,1);
    /* 004bbbf9 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004bbbfd */
    if(mi_signed_less(f->flags))goto mi_004bb843;
    /* 004bbc03 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bbc04 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004bbc05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_004bbc06:;
    /* 004bbc06 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bbc07 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bbc0a */
    f->r[4]+=4;return;
}

void restore_actor_init_C_004bbc10(struct restore_actor_init_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bbc10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbc11 */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004bbc16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bbc18 */
    if(!(f->flags&MI_ZF))goto mi_004bbd3d;
    /* 004bbc1e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbc1f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbc20 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbc21 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbc22 */
    f->r[7]=0x00784b74u;
mi_004bbc27:;
    /* 004bbc27 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[7])))&0xffffu)<<0);
    /* 004bbc2a */
    a=(f->r[3]&0xffffu);b=(f->r[3]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 004bbc2d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004bbd2a;
    /* 004bbc33 */
    f->r[0]=MI_U32((0x00d3b1c4u));
    /* 004bbc38 */
    f->r[1]=MI_U32((0x00d3b374u));
    /* 004bbc3e */
    MI_U32((0x00d3b1c8u+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004bbc45 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bbc46 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 004bbc49 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(f->r[0]);
    /* 004bbc4e */
    if(mi_signed_less(f->flags))goto mi_004bbc5a;
    /* 004bbc50 */
    MI_U32((0x00d3b1c4u))=(uint32_t)(0x00000000u);
mi_004bbc5a:;
    /* 004bbc5a */
    f->r[1]=MI_U32((0x00d3b384u));
    /* 004bbc60 */
    f->r[0]=MI_U32((0x00d3b380u));
    /* 004bbc65 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001fu);
mi_004bbc6d:;
    /* 004bbc6d */
    f->r[2]=f->r[1];
    /* 004bbc6f */
    f->r[6]=f->r[1];
    /* 004bbc71 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 004bbc74 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbc77 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bbc7a */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004bbc7c */
    f->r[6]=f->r[0];
    /* 004bbc7e */
    a=f->r[6];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[6]=t;
    /* 004bbc81 */
    f->r[5]=f->r[0];
    /* 004bbc83 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004bbc86 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 004bbc89 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004bbc8b */
    a=f->r[1];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[1]=t;
    /* 004bbc8d */
    a=f->r[0];t=a>>1u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF|MI_OF))|((a>>0u)&1u)|mi_szp(t,32)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;
    /* 004bbc8f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004bbc91 */
    if((f->flags&MI_ZF))goto mi_004bbc99;
    /* 004bbc93 */
    a=f->r[1];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
mi_004bbc99:;
    /* 004bbc99 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004bbc9b */
    if((f->flags&MI_ZF))goto mi_004bbca2;
    /* 004bbc9d */
    a=f->r[0];b=0x40000000u;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004bbca2:;
    /* 004bbca2 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004bbca6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004bbca7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004bbcab */
    if(!(f->flags&MI_ZF))goto mi_004bbc6d;
    /* 004bbcad */
    f->r[2]=f->r[1];
    /* 004bbcaf */
    MI_U32((0x00d3b384u))=(uint32_t)(f->r[1]);
    /* 004bbcb5 */
    a=f->r[2];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbcbb */
    MI_U32((0x00d3b380u))=(uint32_t)(f->r[0]);
    /* 004bbcc0 */
    MI_U32((0x00d3b370u))=(uint32_t)(f->r[2]);
    /* 004bbcc6 */
    f->r[2]=f->r[1];
    /* 004bbcc8 */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbccb */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bbcce */
    MI_U32((0x00d3b374u))=(uint32_t)(f->r[2]);
    /* 004bbcd4 */
    MI_U32((0x00d3b378u))=(uint32_t)(f->r[1]);
    /* 004bbcda */
    f->r[1]=f->r[0];
    /* 004bbcdc */
    f->r[2]=f->r[0];
    /* 004bbcde */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004bbce1 */
    a=f->r[1];b=0x00007fffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004bbce7 */
    MI_U32((0x00d3b36cu))=(uint32_t)(f->r[0]);
    /* 004bbcec */
    MI_U32((0x00d3b364u))=(uint32_t)(f->r[1]);
    /* 004bbcf2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000010u+f->r[7]));
    /* 004bbcf6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[3]&0xffffu);
    /* 004bbcf9 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbcfb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbcfc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbcfd */
    a=f->r[2];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004bbd00 */
    a=0x0000005fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbd02 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bbd04 */
    MI_U32((0x00d3b368u))=(uint32_t)(f->r[2]);
    /* 004bbd0a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bbd0fu;f->boundary(0x004bbd0au,0x004b4800u,f);
    /* 004bbd0f */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bbd12 */
    f->r[6]=f->r[0];
    /* 004bbd14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bbd19u;f->boundary(0x004bbd14u,0x00562348u,f);
    /* 004bbd19 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 004bbd1c */
    a=(f->r[0]&0xffu);b=0x0000001fu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004bbd1e */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004bbd21 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004bbd23 */
    MI_U8((0x00bcd5f9u+f->r[2]*4u))=(uint8_t)((f->r[0]&0xffu));
mi_004bbd2a:;
    /* 004bbd2a */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004bbd2d */
    a=f->r[7];b=0x00784b84u;t=mi_alu(f,a,b,32,1);
    /* 004bbd33 */
    if(mi_signed_less(f->flags))goto mi_004bbc27;
    /* 004bbd39 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004bbd3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004bbd3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004bbd3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_004bbd3d:;
    /* 004bbd3d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004bbd3e */
    f->r[4]+=4;return;
}
