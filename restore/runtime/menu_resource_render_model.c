/* Whole UIstate14/15 resource renderers and their owned integer providers.
 * Pinned original SHA fde7cd65...d33c8d, exact cache bytes independently checked.
 * Only exact Ghidra instruction items are translated: 52EC50 includes its
 * shared52E8C0 tail and enters at52EC50, never through a disassembled gap.
 * Six native tail jumps bind genuine52F850 with no extra CALL/return slot.
 * Preserve native widths, mutable record/list reloads, scratch and write order.
 * MUL40FB4E is explicit unsigned64 product, full EDX:EAX+CF/OF.
 * Pixel/file-backed layout fidelity remains beyond translation/compile;
 * no controlled production seams or invented bounds. */
#include "menu_resource_render.h"
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
static __inline uint32_t mi_alu(struct restore_resource_render_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_resource_render_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_resource_render_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}
static __inline void mi_idiv32(struct restore_resource_render_frame *f,uint32_t divisor_bits)
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
void restore_resource_render_C_0040fb10(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_0040fb10;
mi_0040fb10:;
    /* 0040fb10 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040fb11 */
    f->r[5]=f->r[4];
    /* 0040fb13 */
    f->r[0]=MI_U32((0x00000008u+f->r[5]));
    /* 0040fb16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040fb18 */
    if(mi_signed_less(f->flags))goto mi_0040fb5d;
    /* 0040fb1a */
    a=f->r[0];b=MI_U32((0x00e38d9cu));t=mi_alu(f,a,b,32,1);
    /* 0040fb20 */
    if(!mi_signed_less(f->flags))goto mi_0040fb5d;
    /* 0040fb22 */
    f->r[0]=MI_U32((0x0000000cu+f->r[5]));
    /* 0040fb25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040fb27 */
    if(mi_signed_less(f->flags))goto mi_0040fb5d;
    /* 0040fb29 */
    f->r[1]=MI_U32((0x00e38d98u));
    /* 0040fb2f */
    a=f->r[1];b=0xffffffe0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040fb32 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040fb34 */
    if(!mi_signed_less(f->flags))goto mi_0040fb5d;
    /* 0040fb36 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040fb37 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040fb38 */
    f->r[7]=MI_U32((0x00e92e18u));
    /* 0040fb3e */
    f->r[0]=MI_U32((0x00000008u+f->r[5]));
    /* 0040fb41 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040fb43 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0040fb45 */
    f->r[0]=MI_U32((0x0000000cu+f->r[5]));
    /* 0040fb48 */
    f->r[2]=MI_U32((0x00e38d94u));
    /* 0040fb4e */
    a=f->r[0];b=f->r[2];{uint64_t product=(uint64_t)a*b;f->r[0]=(uint32_t)product;f->r[2]=(uint32_t)(product>>32);f->flags&=~(MI_CF|MI_OF);if(f->r[2])f->flags|=MI_CF|MI_OF;}
    /* 0040fb50 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0040fb52 */
    f->r[3]=MI_U32((0x00d3b4f0u));
    /* 0040fb58 */
    MI_U16((0x00000000u+f->r[7]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0040fb5b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040fb5c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0040fb5d:;
    /* 0040fb5d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040fb5e */
    f->r[4]+=4;return;
}

void restore_resource_render_C_00444980(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_00444980;
mi_00444980:;
    /* 00444980 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00444981 */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 00444985 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00444987 */
    f->r[0]=0x0077de04u;
mi_0044498c:;
    /* 0044498c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffffcu+f->r[0])))&0xffu)<<0);
    /* 0044498f */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00444991 */
    if((f->flags&MI_ZF))goto mi_0044499b;
    /* 00444993 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00444995 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 00444997 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00444999 */
    if((f->flags&MI_ZF))goto mi_004449aa;
mi_0044499b:;
    /* 0044499b */
    a=f->r[0];b=0x0000006au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044499e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044499f */
    a=f->r[0];b=0x0077f746u;t=mi_alu(f,a,b,32,1);
    /* 004449a4 */
    if(mi_signed_less(f->flags))goto mi_0044498c;
    /* 004449a6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004449a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004449a9 */
    f->r[4]+=4;return;
mi_004449aa:;
    /* 004449aa */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004449ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004449ae */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004449b1 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004449b3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004449b5 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004449b7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077de05u+f->r[0]*2u)))&0xffu)<<0);
    /* 004449be */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004449c0 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004449c3 */
    f->r[0]=f->r[1];
    /* 004449c5 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004449d0(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004449d0;
mi_004449d0:;
    /* 004449d0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004449d1 */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 004449d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004449d7 */
    f->r[1]=0x0077de04u;
mi_004449dc:;
    /* 004449dc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffffcu+f->r[1])))&0xffu)<<0);
    /* 004449df */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004449e1 */
    if((f->flags&MI_ZF))goto mi_004449eb;
    /* 004449e3 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004449e5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 004449e7 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004449e9 */
    if((f->flags&MI_ZF))goto mi_004449f9;
mi_004449eb:;
    /* 004449eb */
    a=f->r[1];b=0x0000006au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004449ee */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004449ef */
    a=f->r[1];b=0x0077f746u;t=mi_alu(f,a,b,32,1);
    /* 004449f5 */
    if(mi_signed_less(f->flags))goto mi_004449dc;
    /* 004449f7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004449f9:;
    /* 004449f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004449fa */
    f->r[4]+=4;return;
}

void restore_resource_render_C_00447430(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_00447430;
mi_00447430:;
    /* 00447430 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447431 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447432 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00447433 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00447435 */
    f->r[0]=0x0077f748u;
mi_0044743a:;
    /* 0044743a */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000001u+f->r[0])))&0xffu)<<0);
    /* 0044743d */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044743f */
    if((f->flags&MI_ZF))goto mi_0044744d;
    /* 00447441 */
    a=MI_U8((0x00000000u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00447444 */
    if(!(f->flags&MI_ZF))goto mi_0044744d;
    /* 00447446 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000018u+f->r[0])))&0xffu)<<0);
    /* 00447449 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044744b */
    if((f->flags&MI_ZF))goto mi_0044745f;
mi_0044744d:;
    /* 0044744d */
    a=f->r[0];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00447450 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00447451 */
    a=f->r[0];b=0x007843d8u;t=mi_alu(f,a,b,32,1);
    /* 00447456 */
    if(mi_signed_less(f->flags))goto mi_0044743a;
mi_00447458:;
    /* 00447458 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00447459 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044745a */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 0044745d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044745e */
    f->r[4]+=4;return;
mi_0044745f:;
    /* 0044745f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00447461 */
    f->r[7]=f->r[1];
    /* 00447463 */
    if(mi_signed_less(f->flags))goto mi_00447458;
    /* 00447465 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00447469 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044746c */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0044746f */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00447471 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00447473 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00447475 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0077de56u+f->r[1]*2u));
    /* 0044747d */
    f->r[1]=0x007770f2u;
mi_00447482:;
    /* 00447482 */
    a=MI_U8((0xffffffffu+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00447486 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044748b;
    /* 00447488 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)(0x00000000u);
mi_0044748b:;
    /* 0044748b */
    a=MI_U8((0x00000000u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0044748e */
    if((f->flags&MI_ZF))goto mi_004474ac;
    /* 00447490 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffau+f->r[1]));
    /* 00447494 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00447496 */
    if((f->flags&MI_ZF))goto mi_004474a0;
    /* 00447498 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffcu+f->r[1]));
    /* 0044749c */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0044749e */
    if(!(f->flags&MI_ZF))goto mi_004474ac;
mi_004474a0:;
    /* 004474a0 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004474a2 */
    if((f->flags&MI_ZF))goto mi_004474c2;
    /* 004474a4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffcu+f->r[1]));
    /* 004474a8 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004474aa */
    if((f->flags&MI_ZF))goto mi_004474c2;
mi_004474ac:;
    /* 004474ac */
    a=f->r[1];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004474b2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004474b3 */
    a=f->r[1];b=0x0077b03au;t=mi_alu(f,a,b,32,1);
    /* 004474b9 */
    if(mi_signed_less(f->flags))goto mi_00447482;
    /* 004474bb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004474bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004474bd */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004474c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004474c1 */
    f->r[4]+=4;return;
mi_004474c2:;
    /* 004474c2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004474c5 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004474c8 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770ecu+f->r[1]*4u));
    /* 004474d0 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004474d2 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004474d6 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004474d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004474da */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004474db */
    MI_U8((0x00000000u+f->r[2]))=(uint8_t)((f->r[1]&0xffu));
    /* 004474dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004474de */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004477d0(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004477d0;
mi_004477d0:;
    /* 004477d0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004477d2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 004477d4 */
    f->r[1]=MI_U32((0x00000134u+f->r[1]));
    /* 004477da */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 004477dd */
    f->r[0]=0x51eb851fu;
    /* 004477e2 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004477e4 */
    a=f->r[2];b=0x00000007u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004477e7 */
    f->r[1]=f->r[2];
    /* 004477e9 */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 004477ec */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004477ee */
    f->r[0]=f->r[2];
    /* 004477f0 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004477f3 */
    if(!mi_signed_less(f->flags))goto mi_004477fa;
    /* 004477f5 */
    f->r[0]=0x00000001u;
mi_004477fa:;
    /* 004477fa */
    f->r[4]+=4;return;
}

void restore_resource_render_C_00449370(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_00449370;
mi_00449370:;
    /* 00449370 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00449372 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449375 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x005d9c18u+f->r[0])))&0xffu)<<0);
    /* 0044937b */
    f->r[4]+=4;return;
}

void restore_resource_render_C_00449380(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_00449380;
mi_00449380:;
    /* 00449380 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00449382 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449385 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x005d9c38u+f->r[0])))&0xffu)<<0);
    /* 0044938b */
    f->r[4]+=4;return;
}

void restore_resource_render_C_00449390(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_00449390;
mi_00449390:;
    /* 00449390 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449391 */
    f->r[6]=f->r[1];
    /* 00449393 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449398u;f->boundary(0x00449393u,0x00449370u,f);
    /* 00449398 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044939a */
    if((f->flags&MI_ZF))goto mi_004493aa;
    /* 0044939c */
    f->r[1]=f->r[6];
    /* 0044939e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004493a3u;f->boundary(0x0044939eu,0x00449380u,f);
    /* 004493a3 */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004493a5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004493a7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004493a8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004493a9 */
    f->r[4]+=4;return;
mi_004493aa:;
    /* 004493aa */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004493ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004493ad */
    f->r[4]+=4;return;
}

void restore_resource_render_C_00449750(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_00449750;
mi_00449750:;
    /* 00449750 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00449754 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449755 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 00449759 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044975a */
    f->r[5]=MI_U32((0x0000000cu+f->r[4]));
    /* 0044975e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044975f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449760 */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 00449764 */
    MI_U32((0x00000000u+f->r[5]))=(uint32_t)(0xffffffffu);
    /* 0044976b */
    f->r[6]=f->r[1];
    /* 0044976d */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 00449773 */
    MI_U8((0x00000000u+f->r[3]))=(uint8_t)(0x00000000u);
    /* 00449776 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0044977c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[6])))&0xffu)<<0);
    /* 0044977f */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00449781 */
    if((f->flags&MI_ZF))goto mi_0044978c;
    /* 00449783 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00449784 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00449785 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00449786 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449788 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00449789 */
    f->r[4]+=20;return;
mi_0044978c:;
    /* 0044978c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000032u+f->r[6])))&0xffu)<<0);
    /* 0044978f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449791 */
    if(!(f->flags&MI_ZF))goto mi_0044979c;
    /* 00449793 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00449794 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00449795 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00449796 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449798 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00449799 */
    f->r[4]+=20;return;
mi_0044979c:;
    /* 0044979c */
    a=MI_U16((0x00000004u+f->r[6]));b=0x00000025u;t=mi_alu(f,a,b,16,1);
    /* 004497a1 */
    if((f->flags&MI_ZF))goto mi_004497ac;
mi_004497a3:;
    /* 004497a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004497a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004497a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004497a6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004497a8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004497a9 */
    f->r[4]+=20;return;
mi_004497ac:;
    /* 004497ac */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 004497b0 */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 004497b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004497b5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004497b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004497bbu;f->boundary(0x004497b6u,0x00447430u,f);
    /* 004497bb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 004497bf */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004497c2 */
    f->r[1]=f->r[6];
    /* 004497c4 */
    MI_U32((0x00000000u+f->r[5]))=(uint32_t)(f->r[0]);
    /* 004497c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004497ccu;f->boundary(0x004497c7u,0x0044c110u,f);
    /* 004497cc */
    f->r[1]=f->r[0];
    /* 004497ce */
    a=f->r[1];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004497d1 */
    if(!mi_signed_less(f->flags))goto mi_004497d8;
    /* 004497d3 */
    f->r[1]=0x00000001u;
mi_004497d8:;
    /* 004497d8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000030u+f->r[6]));
    /* 004497dc */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004497df */
    if((f->flags&MI_ZF))goto mi_004497fc;
    /* 004497e1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004497a3;
    /* 004497e3 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004497e6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004497a3;
    /* 004497e8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000002eu+f->r[6]));
    /* 004497ec */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004497ef */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004497f2 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004497f5 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004497f6 */
    a=f->r[1];mi_idiv32(f,a);
    /* 004497f8 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004497fa */
    goto mi_00449815;
mi_004497fc:;
    /* 004497fc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000002eu+f->r[6]));
    /* 00449800 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00449803 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00449806 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00449809 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044980a */
    a=f->r[1];mi_idiv32(f,a);
    /* 0044980c */
    f->r[1]=0x00000064u;
    /* 00449811 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449813 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
mi_00449815:;
    /* 00449815 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 00449817 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449819 */
    if(!mi_signed_less(f->flags))goto mi_00449823;
    /* 0044981b */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 00449821 */
    goto mi_0044982e;
mi_00449823:;
    /* 00449823 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 00449826 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044982e;
    /* 00449828 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000064u);
mi_0044982e:;
    /* 0044982e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000014u+f->r[4])))&0xffu)<<0);
    /* 00449832 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449834 */
    if((f->flags&MI_ZF))goto mi_00449841;
    /* 00449836 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 00449838 */
    f->r[2]=0x00000064u;
    /* 0044983d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0044983f */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[2]);
mi_00449841:;
    /* 00449841 */
    f->r[0]=MI_U32((0x00000000u+f->r[5]));
    /* 00449844 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00449848 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00449849 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044984a */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044984d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044984e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00449851 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f0u+f->r[0]*4u)))&0xffu)<<0);
    /* 00449858 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0044985a */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0044985d */
    t=((f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449860 */
    MI_U8((0x00000000u+f->r[3]))=(uint8_t)((f->r[1]&0xffu));
    /* 00449862 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0x00000000u);
    /* 00449868 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00449869 */
    f->r[4]+=20;return;
}

void restore_resource_render_C_0044c110(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_0044c110;
mi_0044c110:;
    /* 0044c110 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c111 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c112 */
    f->r[6]=f->r[1];
    /* 0044c114 */
    a=MI_U8((0x00000002u+f->r[6]));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0044c118 */
    if(!(f->flags&MI_ZF))goto mi_0044c158;
    /* 0044c11a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000058u+f->r[6]));
    /* 0044c11e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044c120 */
    if(mi_signed_less(f->flags))goto mi_0044c190;
    /* 0044c122 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 0044c125 */
    if(!mi_signed_less(f->flags))goto mi_0044c190;
    /* 0044c127 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044c12a */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044c12d */
    f->r[1]=(0x00776fb0u+f->r[0]*4u);
    /* 0044c134 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[0]*4u)))&0xffu)<<0);
    /* 0044c13b */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0044c13d */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044c146;
    /* 0044c13f */
    MI_U8((0x00000142u+f->r[1]))=(uint8_t)(0x00000000u);
mi_0044c146:;
    /* 0044c146 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000142u+f->r[1])))&0xffu)<<0);
    /* 0044c14c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c14e */
    if((f->flags&MI_ZF))goto mi_0044c190;
    /* 0044c150 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c155u;f->boundary(0x0044c150u,0x004477d0u,f);
    /* 0044c155 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c156 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0044c157 */
    f->r[4]+=4;return;
mi_0044c158:;
    /* 0044c158 */
    f->r[1]=f->r[6];
    /* 0044c15a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c15fu;f->boundary(0x0044c15au,0x00449390u,f);
    /* 0044c15f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044c161 */
    if((f->flags&MI_ZF))goto mi_0044c190;
    /* 0044c163 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000001cu+f->r[6]));
    /* 0044c167 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c168 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c16du;f->boundary(0x0044c168u,0x004449d0u,f);
    /* 0044c16d */
    f->r[1]=(0x0000000bu+f->r[4]);
    /* 0044c171 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c172 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044c173 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c178u;f->boundary(0x0044c173u,0x00447430u,f);
    /* 0044c178 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 0044c17b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044c17e */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044c181 */
    f->r[1]=(0x00776fb0u+f->r[2]*4u);
    /* 0044c188 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044c18du;f->boundary(0x0044c188u,0x004477d0u,f);
    /* 0044c18d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c18e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0044c18f */
    f->r[4]+=4;return;
mi_0044c190:;
    /* 0044c190 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0044c192 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044c193 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0044c194 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_0044d750(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_0044d750;
mi_0044d750:;
    /* 0044d750 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000014u+f->r[1]));
    /* 0044d754 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007848d8u));
    /* 0044d75b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044d75d */
    f->r[2]=MI_U32((0x0078d7bcu));
    /* 0044d763 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044d765 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000002cu+f->r[1]));
    /* 0044d769 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044d76c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 0044d76f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0044d771 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000003u+f->r[1])))&0xffu)<<0);
    /* 0044d774 */
    f->r[1]=MI_U32((0x0078d7c0u));
    /* 0044d77a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0044d77c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044d77e */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd3e0(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd3e0;
mi_004cd3e0:;
    /* 004cd3e0 */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004cd3e4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd3e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd3e6 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd3ea */
    f->r[0]=0x00000001u;
    /* 004cd3ef */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004cd3f1 */
    MI_U32((0x00d3b4dcu))=(uint32_t)(f->r[0]);
    /* 004cd3f6 */
    MI_U32((0x00d3b4d8u))=(uint32_t)(f->r[0]);
    /* 004cd3fb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd403;
    /* 004cd3fd */
    f->r[3]=f->r[2];
    /* 004cd3ff */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004cd401 */
    goto mi_004cd407;
mi_004cd403:;
    /* 004cd403 */
    f->r[3]=f->r[6];
    /* 004cd405 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[3]=t;
mi_004cd407:;
    /* 004cd407 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004cd40b */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004cd40f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004cd411 */
    MI_U32((0x00d3b4d0u))=(uint32_t)(f->r[3]);
    /* 004cd417 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd418 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd420;
    /* 004cd41a */
    f->r[7]=f->r[0];
    /* 004cd41c */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004cd41e */
    goto mi_004cd424;
mi_004cd420:;
    /* 004cd420 */
    f->r[7]=f->r[1];
    /* 004cd422 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
mi_004cd424:;
    /* 004cd424 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004cd426 */
    MI_U32((0x00d3b4d4u))=(uint32_t)(f->r[7]);
    /* 004cd42c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd432;
    /* 004cd42e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004cd430 */
    goto mi_004cd434;
mi_004cd432:;
    /* 004cd432 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
mi_004cd434:;
    /* 004cd434 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd44d;
    /* 004cd436 */
    f->r[3]=f->r[1];
    /* 004cd438 */
    f->r[7]=f->r[6];
    /* 004cd43a */
    MI_U32((0x00d3b4ccu))=(uint32_t)(f->r[3]);
    /* 004cd440 */
    MI_U32((0x00d3b4c4u))=(uint32_t)(f->r[0]);
    /* 004cd445 */
    MI_U32((0x00d3b4c8u))=(uint32_t)(f->r[7]);
    /* 004cd44b */
    goto mi_004cd466;
mi_004cd44d:;
    /* 004cd44d */
    f->r[3]=f->r[0];
    /* 004cd44f */
    f->r[0]=f->r[1];
    /* 004cd451 */
    f->r[7]=f->r[2];
    /* 004cd453 */
    MI_U32((0x00d3b4ccu))=(uint32_t)(f->r[3]);
    /* 004cd459 */
    MI_U32((0x00d3b4c4u))=(uint32_t)(f->r[0]);
    /* 004cd45e */
    MI_U32((0x00d3b4c8u))=(uint32_t)(f->r[7]);
    /* 004cd464 */
    f->r[2]=f->r[6];
mi_004cd466:;
    /* 004cd466 */
    a=f->r[1];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 004cd469 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004cd46b */
    MI_U32((0x00d3b4c0u))=(uint32_t)(f->r[2]);
    /* 004cd471 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd472 */
    if(!mi_signed_less(f->flags))goto mi_004cd47a;
    /* 004cd474 */
    MI_U32((0x00d3b4d8u))=(uint32_t)(f->r[1]);
mi_004cd47a:;
    /* 004cd47a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd47b */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004cd47d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd47e */
    if(!mi_signed_less(f->flags))goto mi_004cd486;
    /* 004cd480 */
    MI_U32((0x00d3b4dcu))=(uint32_t)(f->r[1]);
mi_004cd486:;
    /* 004cd486 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd490(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd490;
mi_004cd490:;
    /* 004cd490 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004cd494 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd496 */
    f->r[0]=MI_U32((0x00d3b4bcu));
    /* 004cd49b */
    if(!(f->flags&MI_ZF))goto mi_004cd4b7;
    /* 004cd49d */
    f->r[1]=MI_U32((0x00d3b4d4u));
    /* 004cd4a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd4a5 */
    if(mi_signed_less(f->flags))goto mi_004cd4c7;
    /* 004cd4a7 */
    a=f->r[1];b=MI_U32((0x00d3b4d0u));t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004cd4ad */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004cd4b0 */
    MI_U32((0x00d3b4bcu))=(uint32_t)(f->r[2]);
    /* 004cd4b6 */
    f->r[4]+=4;return;
mi_004cd4b7:;
    /* 004cd4b7 */
    f->r[1]=MI_U32((0x00d3b4d0u));
    /* 004cd4bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd4bf */
    if(mi_signed_less(f->flags))goto mi_004cd4c7;
    /* 004cd4c1 */
    a=f->r[1];b=MI_U32((0x00d3b4d4u));t=mi_alu(f,a,b,32,1);f->r[1]=t;
mi_004cd4c7:;
    /* 004cd4c7 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 004cd4ca */
    MI_U32((0x00d3b4bcu))=(uint32_t)(f->r[2]);
    /* 004cd4d0 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd4e0(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd4e0;
mi_004cd4e0:;
    /* 004cd4e0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004cd4e4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004cd4e6 */
    if(mi_signed_less(f->flags))goto mi_004cd50f;
    /* 004cd4e8 */
    a=f->r[1];b=MI_U32((0x00e38d9cu));t=mi_alu(f,a,b,32,1);
    /* 004cd4ee */
    if(!mi_signed_less(f->flags))goto mi_004cd50f;
    /* 004cd4f0 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004cd4f4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd4f6 */
    if(mi_signed_less(f->flags))goto mi_004cd50f;
    /* 004cd4f8 */
    a=f->r[0];b=MI_U32((0x00e38d98u));t=mi_alu(f,a,b,32,1);
    /* 004cd4fe */
    if(!mi_signed_less(f->flags))goto mi_004cd50f;
    /* 004cd500 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004cd504 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd505 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd506 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd507 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd50cu;f->boundary(0x004cd507u,0x0040fa40u,f);
    /* 004cd50c */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004cd50f:;
    /* 004cd50f */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd510(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd510;
mi_004cd510:;
    /* 004cd510 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004cd514 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004cd518 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd519 */
    f->r[3]=MI_U32((0x00000008u+f->r[4]));
    /* 004cd51d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd51e */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd522 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd523 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd524 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd525 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd526 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd527 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd528 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd52du;f->boundary(0x004cd528u,0x004cd3e0u,f);
    /* 004cd52d */
    f->r[6]=MI_U32((0x00d3b4ccu));
    /* 004cd533 */
    f->r[7]=MI_U32((0x00d3b4c8u));
    /* 004cd539 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd53c */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004cd53e */
    if(!(f->flags&MI_ZF))goto mi_004cd56a;
    /* 004cd540 */
    a=f->r[6];b=MI_U32((0x00d3b4c4u));t=mi_alu(f,a,b,32,1);
    /* 004cd546 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd67b;
    /* 004cd54c */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
mi_004cd550:;
    /* 004cd550 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd551 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd552 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd553 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd558u;f->boundary(0x004cd553u,0x004cd4e0u,f);
    /* 004cd558 */
    f->r[0]=MI_U32((0x00d3b4c4u));
    /* 004cd55d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd560 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004cd561 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd563 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd550;
    /* 004cd565 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd566 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd567 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd568 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd569 */
    f->r[4]+=4;return;
mi_004cd56a:;
    /* 004cd56a */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 004cd56e */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd572 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd574 */
    if(!(f->flags&MI_ZF))goto mi_004cd5a0;
    /* 004cd576 */
    a=f->r[7];b=MI_U32((0x00d3b4c0u));t=mi_alu(f,a,b,32,1);
    /* 004cd57c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd67b;
    /* 004cd582 */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
mi_004cd586:;
    /* 004cd586 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd587 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd588 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd589 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd58eu;f->boundary(0x004cd589u,0x004cd4e0u,f);
    /* 004cd58e */
    f->r[0]=MI_U32((0x00d3b4c0u));
    /* 004cd593 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd596 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004cd597 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd599 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd586;
    /* 004cd59b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd59c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd59d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd59e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd59f */
    f->r[4]+=4;return;
mi_004cd5a0:;
    /* 004cd5a0 */
    f->r[0]=MI_U32((0x00d3b4d4u));
    /* 004cd5a5 */
    f->r[1]=MI_U32((0x00d3b4d0u));
    /* 004cd5ab */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004cd5ad */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd61a;
    /* 004cd5af */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*1u);
    /* 004cd5b2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004cd5b4 */
    f->r[0]=MI_U32((0x00d3b4c4u));
    /* 004cd5b9 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd5bb */
    MI_U32((0x00d3b4bcu))=(uint32_t)(f->r[2]);
    /* 004cd5c1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd67b;
    /* 004cd5c7 */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 004cd5cb */
    f->r[5]=0x00000001u;
mi_004cd5d0:;
    /* 004cd5d0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd5d1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd5d2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd5d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd5d8u;f->boundary(0x004cd5d3u,0x004cd4e0u,f);
    /* 004cd5d8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd5d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd5deu;f->boundary(0x004cd5d9u,0x004cd490u,f);
    /* 004cd5de */
    f->r[1]=MI_U32((0x00d3b4d4u));
    /* 004cd5e4 */
    f->r[0]=MI_U32((0x00d3b4bcu));
    /* 004cd5e9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd5ec */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004cd5ed */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004cd5ef */
    MI_U32((0x00d3b4d4u))=(uint32_t)(f->r[1]);
    /* 004cd5f5 */
    if(mi_signed_less(f->flags))goto mi_004cd60b;
    /* 004cd5f7 */
    f->r[1]=MI_U32((0x00d3b4d0u));
    /* 004cd5fd */
    f->r[0]=MI_U32((0x00d3b4d8u));
    /* 004cd602 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004cd603 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004cd605 */
    MI_U32((0x00d3b4d0u))=(uint32_t)(f->r[1]);
mi_004cd60b:;
    /* 004cd60b */
    f->r[0]=MI_U32((0x00d3b4c4u));
    /* 004cd610 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004cd611 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd613 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd5d0;
    /* 004cd615 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd616 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd617 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd618 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd619 */
    f->r[4]+=4;return;
mi_004cd61a:;
    /* 004cd61a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cd61c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd61e */
    MI_U32((0x00d3b4bcu))=(uint32_t)(f->r[0]);
    /* 004cd623 */
    f->r[0]=MI_U32((0x00d3b4c0u));
    /* 004cd628 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd62a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd67b;
    /* 004cd62c */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 004cd630 */
    f->r[5]=0x00000001u;
mi_004cd635:;
    /* 004cd635 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd636 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd637 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd638 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd63du;f->boundary(0x004cd638u,0x004cd4e0u,f);
    /* 004cd63d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd63f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd644u;f->boundary(0x004cd63fu,0x004cd490u,f);
    /* 004cd644 */
    f->r[1]=MI_U32((0x00d3b4d0u));
    /* 004cd64a */
    f->r[0]=MI_U32((0x00d3b4bcu));
    /* 004cd64f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd652 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004cd653 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004cd655 */
    MI_U32((0x00d3b4d0u))=(uint32_t)(f->r[1]);
    /* 004cd65b */
    if(mi_signed_less(f->flags))goto mi_004cd671;
    /* 004cd65d */
    f->r[1]=MI_U32((0x00d3b4d4u));
    /* 004cd663 */
    f->r[0]=MI_U32((0x00d3b4dcu));
    /* 004cd668 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004cd669 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004cd66b */
    MI_U32((0x00d3b4d4u))=(uint32_t)(f->r[1]);
mi_004cd671:;
    /* 004cd671 */
    f->r[0]=MI_U32((0x00d3b4c0u));
    /* 004cd676 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004cd677 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd679 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cd635;
mi_004cd67b:;
    /* 004cd67b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd67c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd67d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd67e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd67f */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd710(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd710;
mi_004cd710:;
    /* 004cd710 */
    f->r[0]=MI_U32((0x00d3b4f4u));
    /* 004cd715 */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 004cd719 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd71a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd71b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd71c */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd720 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd721 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd725 */
    MI_U32((0x00d3b4f0u))=(uint32_t)(f->r[0]);
    /* 004cd72a */
    MI_U32((0x00d3b4e8u))=(uint32_t)(0x00000000u);
    /* 004cd734 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[1]*1u);
    /* 004cd737 */
    f->r[6]=f->r[7];
    /* 004cd739 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd73b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004cd73f */
    if(!mi_signed_less(f->flags))goto mi_004cd767;
mi_004cd741:;
    /* 004cd741 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd742 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd743 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd748u;f->boundary(0x004cd743u,0x004cdc30u,f);
    /* 004cd748 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd74b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd74d */
    if((f->flags&MI_ZF))goto mi_004cd759;
    /* 004cd74f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd750 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd751 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd756u;f->boundary(0x004cd751u,0x0040fb10u,f);
    /* 004cd756 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004cd759:;
    /* 004cd759 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd75eu;f->boundary(0x004cd759u,0x004cd850u,f);
    /* 004cd75e */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd762 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004cd763 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd765 */
    if(mi_signed_less(f->flags))goto mi_004cd741;
mi_004cd767:;
    /* 004cd767 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd769 */
    f->r[5]=f->r[7];
    /* 004cd76b */
    if(!mi_signed_less(f->flags))goto mi_004cd79b;
    /* 004cd76d */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd771 */
    f->r[6]=(0xffffffffu+f->r[3]+f->r[2]*1u);
mi_004cd775:;
    /* 004cd775 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd776 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd777 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd77cu;f->boundary(0x004cd777u,0x004cdc30u,f);
    /* 004cd77c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd77f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd781 */
    if((f->flags&MI_ZF))goto mi_004cd78d;
    /* 004cd783 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd784 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd785 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd78au;f->boundary(0x004cd785u,0x0040fb10u,f);
    /* 004cd78a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004cd78d:;
    /* 004cd78d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd792u;f->boundary(0x004cd78du,0x004cd850u,f);
    /* 004cd792 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd796 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004cd797 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd799 */
    if(mi_signed_less(f->flags))goto mi_004cd775;
mi_004cd79b:;
    /* 004cd79b */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd79f */
    f->r[6]=f->r[3];
    /* 004cd7a1 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cd7a3 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd7a5 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004cd7a9 */
    if(!mi_signed_less(f->flags))goto mi_004cd7d1;
mi_004cd7ab:;
    /* 004cd7ab */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7ac */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd7b2u;f->boundary(0x004cd7adu,0x004cdc30u,f);
    /* 004cd7b2 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd7b5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd7b7 */
    if((f->flags&MI_ZF))goto mi_004cd7c3;
    /* 004cd7b9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7ba */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7bb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd7c0u;f->boundary(0x004cd7bbu,0x0040fb10u,f);
    /* 004cd7c0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004cd7c3:;
    /* 004cd7c3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd7c8u;f->boundary(0x004cd7c3u,0x004cd850u,f);
    /* 004cd7c8 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd7cc */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004cd7cd */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd7cf */
    if(mi_signed_less(f->flags))goto mi_004cd7ab;
mi_004cd7d1:;
    /* 004cd7d1 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd7d3 */
    f->r[6]=f->r[3];
    /* 004cd7d5 */
    if(!mi_signed_less(f->flags))goto mi_004cd80a;
    /* 004cd7d7 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004cd7db */
    f->r[5]=(0xffffffffu+f->r[1]+f->r[5]*1u);
mi_004cd7df:;
    /* 004cd7df */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7e0 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7e1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd7e6u;f->boundary(0x004cd7e1u,0x004cdc30u,f);
    /* 004cd7e6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd7e9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004cd7eb */
    if((f->flags&MI_ZF))goto mi_004cd7fc;
    /* 004cd7ed */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd7f1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7f2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004cd7f3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd7f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd7f9u;f->boundary(0x004cd7f4u,0x0040fb10u,f);
    /* 004cd7f9 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004cd7fc:;
    /* 004cd7fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd801u;f->boundary(0x004cd7fcu,0x004cd850u,f);
    /* 004cd801 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd805 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004cd806 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd808 */
    if(mi_signed_less(f->flags))goto mi_004cd7df;
mi_004cd80a:;
    /* 004cd80a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd80b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd80c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd80d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd80e */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd810(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd810;
mi_004cd810:;
    /* 004cd810 */
    f->r[0]=MI_U32((0x00e36c84u));
    /* 004cd815 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004cd816 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 004cd819 */
    MI_U32((0x00e36c84u))=(uint32_t)(f->r[0]);
    /* 004cd81e */
    if(mi_signed_less(f->flags))goto mi_004cd84e;
    /* 004cd820 */
    f->r[0]=MI_U32((0x00d3b4f4u));
    /* 004cd825 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004cd827 */
    a=f->r[0];b=0x0000f800u;t=mi_alu(f,a,b,32,1);
    /* 004cd82c */
    MI_U32((0x00e36c84u))=(uint32_t)(f->r[1]);
    /* 004cd832 */
    if(!(f->flags&MI_ZF))goto mi_004cd83b;
    /* 004cd834 */
    MI_U32((0x00d3b4f4u))=(uint32_t)(f->r[1]);
    /* 004cd83a */
    f->r[4]+=4;return;
mi_004cd83b:;
    /* 004cd83b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004cd83d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004cd83f */
    a=f->r[0];b=0xfffff801u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004cd844 */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cd849 */
    MI_U32((0x00d3b4f4u))=(uint32_t)(f->r[0]);
mi_004cd84e:;
    /* 004cd84e */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd850(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd850;
mi_004cd850:;
    /* 004cd850 */
    f->r[0]=MI_U32((0x00d3b4e8u));
    /* 004cd855 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004cd856 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004cd859 */
    MI_U32((0x00d3b4e8u))=(uint32_t)(f->r[0]);
    /* 004cd85e */
    if(mi_signed_less(f->flags))goto mi_004cd88e;
    /* 004cd860 */
    f->r[0]=MI_U32((0x00d3b4f0u));
    /* 004cd865 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004cd867 */
    a=f->r[0];b=0x0000f800u;t=mi_alu(f,a,b,32,1);
    /* 004cd86c */
    MI_U32((0x00d3b4e8u))=(uint32_t)(f->r[1]);
    /* 004cd872 */
    if(!(f->flags&MI_ZF))goto mi_004cd87b;
    /* 004cd874 */
    MI_U32((0x00d3b4f0u))=(uint32_t)(f->r[1]);
    /* 004cd87a */
    f->r[4]+=4;return;
mi_004cd87b:;
    /* 004cd87b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004cd87d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004cd87f */
    a=f->r[0];b=0xfffff801u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004cd884 */
    a=f->r[0];b=0x0000ffffu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cd889 */
    MI_U32((0x00d3b4f0u))=(uint32_t)(f->r[0]);
mi_004cd88e:;
    /* 004cd88e */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd890(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd890;
mi_004cd890:;
    /* 004cd890 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd891 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd892 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd893 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd894 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd898 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004cd89a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd8a4;
    /* 004cd89c */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004cd8a0 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004cd8a2 */
    goto mi_004cd8b8;
mi_004cd8a4:;
    /* 004cd8a4 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004cd8a8 */
    f->r[0]=MI_U32((0x00e38d9cu));
    /* 004cd8ad */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*1u);
    /* 004cd8b0 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd8b2 */
    if(mi_signed_less(f->flags))goto mi_004cd8b8;
    /* 004cd8b4 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd8b6 */
    f->r[1]=f->r[0];
mi_004cd8b8:;
    /* 004cd8b8 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 004cd8bc */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004cd8be */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd8c8;
    /* 004cd8c0 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd8c4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004cd8c6 */
    goto mi_004cd8dc;
mi_004cd8c8:;
    /* 004cd8c8 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd8cc */
    f->r[0]=MI_U32((0x00e38d98u));
    /* 004cd8d1 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 004cd8d4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd8d6 */
    if(mi_signed_less(f->flags))goto mi_004cd8dc;
    /* 004cd8d8 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd8da */
    f->r[5]=f->r[0];
mi_004cd8dc:;
    /* 004cd8dc */
    f->r[3]=(0xffffffffu+f->r[7]+f->r[1]*1u);
    /* 004cd8e0 */
    a=0x00008410u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8e6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8e7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8e8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd8eeu;f->boundary(0x004cd8e9u,0x004cd510u,f);
    /* 004cd8ee */
    f->r[5]=(0xffffffffu+f->r[6]+f->r[5]*1u);
    /* 004cd8f2 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8f7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8f8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8f9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8fa */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd8fb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd900u;f->boundary(0x004cd8fbu,0x004cd510u,f);
    /* 004cd900 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd905 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd906 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd907 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd908 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd909 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd90eu;f->boundary(0x004cd909u,0x004cd510u,f);
    /* 004cd90e */
    a=0x00008410u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd913 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd914 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd915 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd916 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd917 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd91cu;f->boundary(0x004cd917u,0x004cd510u,f);
    /* 004cd91c */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd91f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd920 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd921 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd922 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd923 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd930(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd930;
mi_004cd930:;
    /* 004cd930 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd931 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd932 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd933 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd934 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004cd938 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004cd93a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd944;
    /* 004cd93c */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004cd940 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 004cd942 */
    goto mi_004cd958;
mi_004cd944:;
    /* 004cd944 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004cd948 */
    f->r[0]=MI_U32((0x00e38d9cu));
    /* 004cd94d */
    f->r[2]=(0x00000000u+f->r[7]+f->r[1]*1u);
    /* 004cd950 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd952 */
    if(mi_signed_less(f->flags))goto mi_004cd958;
    /* 004cd954 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd956 */
    f->r[1]=f->r[0];
mi_004cd958:;
    /* 004cd958 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 004cd95c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004cd95e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd968;
    /* 004cd960 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd964 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004cd966 */
    goto mi_004cd97c;
mi_004cd968:;
    /* 004cd968 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004cd96c */
    f->r[0]=MI_U32((0x00e38d98u));
    /* 004cd971 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[5]*1u);
    /* 004cd974 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd976 */
    if(mi_signed_less(f->flags))goto mi_004cd97c;
    /* 004cd978 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd97a */
    f->r[5]=f->r[0];
mi_004cd97c:;
    /* 004cd97c */
    f->r[3]=(0xffffffffu+f->r[7]+f->r[1]*1u);
    /* 004cd980 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd985 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd986 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd987 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd988 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd989 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd98eu;f->boundary(0x004cd989u,0x004cd510u,f);
    /* 004cd98e */
    f->r[5]=(0xffffffffu+f->r[6]+f->r[5]*1u);
    /* 004cd992 */
    a=0x00008410u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd997 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd998 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd999 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd99a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd99b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd9a0u;f->boundary(0x004cd99bu,0x004cd510u,f);
    /* 004cd9a0 */
    a=0x00008410u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9a6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9a7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9a8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd9aeu;f->boundary(0x004cd9a9u,0x004cd510u,f);
    /* 004cd9ae */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9b3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9b4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9b5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9b6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cd9bcu;f->boundary(0x004cd9b7u,0x004cd510u,f);
    /* 004cd9bc */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cd9bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cd9c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cd9c1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004cd9c2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cd9c3 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cd9d0(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cd9d0;
mi_004cd9d0:;
    /* 004cd9d0 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004cd9d4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9d5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9d6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cd9d7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004cd9d9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cda3a;
    /* 004cd9db */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004cd9df */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004cd9e1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cda3a;
    /* 004cd9e3 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 004cd9e7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004cd9e9 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cd9ed;
    /* 004cd9eb */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_004cd9ed:;
    /* 004cd9ed */
    f->r[0]=MI_U32((0x00e38d9cu));
    /* 004cd9f2 */
    f->r[6]=(0x00000000u+f->r[3]+f->r[2]*1u);
    /* 004cd9f5 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cd9f7 */
    if(mi_signed_less(f->flags))goto mi_004cd9fd;
    /* 004cd9f9 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cd9fb */
    f->r[2]=f->r[0];
mi_004cd9fd:;
    /* 004cd9fd */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004cda01 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004cda03 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004cda07;
    /* 004cda05 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004cda07:;
    /* 004cda07 */
    f->r[0]=MI_U32((0x00e38d98u));
    /* 004cda0c */
    f->r[7]=(0x00000000u+f->r[6]+f->r[1]*1u);
    /* 004cda0f */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004cda11 */
    if(mi_signed_less(f->flags))goto mi_004cda17;
    /* 004cda13 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004cda15 */
    f->r[1]=f->r[0];
mi_004cda17:;
    /* 004cda17 */
    f->r[7]=(0x00000000u+f->r[6]+f->r[1]*1u);
    /* 004cda1a */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004cda1c */
    if(!mi_signed_less(f->flags))goto mi_004cda3a;
    /* 004cda1e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cda1f */
    f->r[5]=(0xffffffffu+f->r[3]+f->r[2]*1u);
mi_004cda23:;
    /* 004cda23 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004cda27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cda28 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cda29 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cda2a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cda2b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cda2c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cda31u;f->boundary(0x004cda2cu,0x004cd510u,f);
    /* 004cda31 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cda34 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004cda35 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004cda37 */
    if(mi_signed_less(f->flags))goto mi_004cda23;
    /* 004cda39 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_004cda3a:;
    /* 004cda3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004cda3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cda3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cda3d */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004cdc30(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_004cdc30;
mi_004cdc30:;
    /* 004cdc30 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004cdc34 */
    f->r[1]=MI_U32((0x00d3b55cu));
    /* 004cdc3a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004cdc3c */
    if(!mi_signed_less(f->flags))goto mi_004cdc41;
    /* 004cdc3e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004cdc40 */
    f->r[4]+=4;return;
mi_004cdc41:;
    /* 004cdc41 */
    a=f->r[0];b=MI_U32((0x00d3b558u));t=mi_alu(f,a,b,32,1);
    /* 004cdc47 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cdc4c;
    /* 004cdc49 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004cdc4b */
    f->r[4]+=4;return;
mi_004cdc4c:;
    /* 004cdc4c */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004cdc50 */
    f->r[1]=MI_U32((0x00d3b554u));
    /* 004cdc56 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004cdc58 */
    if(!mi_signed_less(f->flags))goto mi_004cdc5d;
    /* 004cdc5a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004cdc5c */
    f->r[4]+=4;return;
mi_004cdc5d:;
    /* 004cdc5d */
    f->r[2]=MI_U32((0x00d3b550u));
    /* 004cdc63 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004cdc65 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004cdc67 */
    t=((f->flags&MI_ZF)||mi_signed_less(f->flags))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004cdc6a */
    f->r[0]=f->r[1];
    /* 004cdc6c */
    f->r[4]+=4;return;
}

void restore_resource_render_C_0052ec50(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_0052ec50;
mi_0052e8c0:;
    /* 0052e8c0 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052e8c5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8c6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8c7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8c8 */
    f->r[7]=(0xffffff98u+f->r[0]);
    /* 0052e8cb */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8cd */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8d2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8d3 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052e8ddu;f->boundary(0x0052e8d8u,0x004cd930u,f);
    /* 0052e8dd */
    f->r[6]=MI_U32((0x007056c8u));
    /* 0052e8e3 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052e8e9 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052e8ef */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0052e8f0 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052e8f5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8f6 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[6]=t;
    /* 0052e8f8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,2);f->r[6]=t;
    /* 0052e8fa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e8fb */
    a=f->r[6];b=0xffff0800u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0052e901 */
    f->r[5]=(0x00000004u+f->r[7]);
    /* 0052e904 */
    a=f->r[6];b=0x0000f800u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0052e90a */
    MI_U32((0x00e20778u))=(uint32_t)(0x00000000u);
    /* 0052e914 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e915 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e916 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e918 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e919 */
    a=0x0000019du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e91e */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e920 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e922 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052e927u;f->boundary(0x0052e922u,0x004d6160u,f);
    /* 0052e927 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052e92d */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052e933 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052e938 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e939 */
    f->r[1]=MI_U32((0x00e20778u));
    /* 0052e93f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e940 */
    f->r[2]=MI_U32((0x005d2a30u));
    /* 0052e946 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e947 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e948 */
    a=f->r[1];b=0x0000019du;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052e94e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e94f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e950 */
    a=0x007027d4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e955 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e957 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e958 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052e95du;f->boundary(0x0052e958u,0x004d6860u,f);
    /* 0052e95d */
    a=f->r[4];b=0x00000058u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052e960 */
    f->r[0]=(0x00000019u+f->r[7]);
    /* 0052e963 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e965 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e96a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e96b */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e970 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052e975u;f->boundary(0x0052e970u,0x004cd930u,f);
    /* 0052e975 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 0052e97a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052e97d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0052e980 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0052e983 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f0u+f->r[1]*4u)))&0xffu)<<0);
    /* 0052e98a */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0052e98c */
    if(!(f->flags&MI_ZF))goto mi_0052e9b9;
    /* 0052e98e */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052e994 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052e999 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052e99f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9a0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9a1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9a3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9a4 */
    f->r[2]=(0x0000001du+f->r[7]);
    /* 0052e9a7 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9ac */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9ad */
    a=0x0000019du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9b2 */
    a=0x000000abu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9b7 */
    goto mi_0052ea11;
mi_0052e9b9:;
    /* 0052e9b9 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0052e9bb */
    if(!(f->flags&MI_ZF))goto mi_0052e9e8;
    /* 0052e9bd */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052e9c2 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052e9c8 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052e9ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9cf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9d0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9d2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9d3 */
    f->r[0]=(0x0000001du+f->r[7]);
    /* 0052e9d6 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9db */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9dc */
    a=0x0000019du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9e1 */
    a=0x000000aau;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9e6 */
    goto mi_0052ea11;
mi_0052e9e8:;
    /* 0052e9e8 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052e9ee */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052e9f4 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052e9f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9fa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9fb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052e9fe */
    f->r[1]=(0x0000001du+f->r[7]);
    /* 0052ea01 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea06 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea07 */
    a=0x0000019du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea0c */
    a=0x000000a9u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052ea11:;
    /* 0052ea11 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea13 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ea18u;f->boundary(0x0052ea13u,0x004d6160u,f);
    /* 0052ea18 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052ea1b */
    f->r[2]=(0x0000004bu+f->r[7]);
    /* 0052ea1e */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea20 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea25 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea26 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea2b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ea30u;f->boundary(0x0052ea2bu,0x004cd930u,f);
    /* 0052ea30 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052ea35 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052ea3b */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052ea41 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea42 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea43 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea45 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea46 */
    a=f->r[7];b=0x0000004fu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0052ea49 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea4e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea4f */
    a=0x0000019du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea54 */
    a=0x000000a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea59 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea5b */
    MI_U32((0x00e20778u))=(uint32_t)(0x00000000u);
    /* 0052ea65 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ea6au;f->boundary(0x0052ea65u,0x004d6160u,f);
    /* 0052ea6a */
    f->r[1]=MI_U32((0x005d2a30u));
    /* 0052ea70 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052ea73 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0052ea76 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052ea77 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052ea78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052ea79 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0052ea7c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770ecu+f->r[0]*4u));
    /* 0052ea84 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0052ea87 */
    if((f->flags&MI_ZF))goto mi_0052eb02;
    /* 0052ea89 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00e39088u)))&0xffu)<<0);
    /* 0052ea8f */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052ea95 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052ea97 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052ea9d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ea9e */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052eaa4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eaa5 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052eaab */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eaad */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eaae */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eab3 */
    if((f->flags&MI_ZF))goto mi_0052ead4;
    /* 0052eab5 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052eab8 */
    a=f->r[2];b=0xffffff94u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052eabb */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0052eabe */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eabf */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052eac1 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052eac3 */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eac8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[1]*2u)))&0xffu)<<0);
    /* 0052eacf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ead0 */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ead2 */
    goto mi_0052eaf4;
mi_0052ead4:;
    /* 0052ead4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052ead7 */
    a=f->r[2];b=0xffffff94u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052eada */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0052eadd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eade */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052eae0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052eae2 */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eae7 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[1]*2u)))&0xffu)<<0);
    /* 0052eaee */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eaef */
    a=0x000000c3u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052eaf4:;
    /* 0052eaf4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052eaf9u;f->boundary(0x0052eaf4u,0x004d6250u,f);
    /* 0052eaf9 */
    f->r[1]=MI_U32((0x005d2a30u));
    /* 0052eaff */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0052eb02:;
    /* 0052eb02 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0052eb05 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0052eb08 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770eeu+f->r[0]*4u));
    /* 0052eb10 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0052eb13 */
    if((f->flags&MI_ZF))goto mi_0052eb88;
    /* 0052eb15 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00e39088u)))&0xffu)<<0);
    /* 0052eb1b */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052eb21 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052eb23 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052eb29 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb2a */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052eb30 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb31 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052eb37 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb39 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb3a */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb3f */
    if((f->flags&MI_ZF))goto mi_0052eb60;
    /* 0052eb41 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052eb44 */
    a=f->r[2];b=0xffffffa8u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052eb47 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0052eb4a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb4b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052eb4d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052eb4f */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb54 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[1]*2u)))&0xffu)<<0);
    /* 0052eb5b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb5c */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb5e */
    goto mi_0052eb80;
mi_0052eb60:;
    /* 0052eb60 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052eb63 */
    a=f->r[2];b=0xffffffa8u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052eb66 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0052eb69 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb6a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052eb6c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052eb6e */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb73 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[1]*2u)))&0xffu)<<0);
    /* 0052eb7a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb7b */
    a=0x000000c3u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052eb80:;
    /* 0052eb80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052eb85u;f->boundary(0x0052eb80u,0x004d6250u,f);
    /* 0052eb85 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0052eb88:;
    /* 0052eb88 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052eb8d */
    a=0x0000f73bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb92 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb94 */
    a=f->r[0];b=0xffffffbcu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052eb97 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb9c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eb9d */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eba2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052eba7u;f->boundary(0x0052eba2u,0x004cd9d0u,f);
    /* 0052eba7 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052ebad */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052ebb3 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052ebb8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebb9 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052ebbf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebc0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebc2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebc3 */
    a=f->r[1];b=0xffffffbcu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052ebc6 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebcb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebcc */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebd1 */
    a=0x000000adu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebd6 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebd8 */
    MI_U32((0x00e20778u))=(uint32_t)(0x00000000u);
    /* 0052ebe2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ebe7u;f->boundary(0x0052ebe2u,0x004d6160u,f);
    /* 0052ebe7 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 0052ebec */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052ebf2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ebf3 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052ebf9 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0052ebfc */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0052ebff */
    f->r[0]=(0x00776fb0u+f->r[2]*4u);
    /* 0052ec06 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052ec0c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec0d */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052ec13 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec15 */
    a=f->r[2];b=0xffffffbcu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052ec18 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec19 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec1a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052ec1c */
    f->r[1]=MI_U32((0x00e20778u));
    /* 0052ec22 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 0052ec24 */
    f->r[0]=MI_U32((0x00000134u+f->r[0]));
    /* 0052ec2a */
    a=f->r[0];b=f->r[2];t=mi_imul(f,a,b);f->r[0]=t;
    /* 0052ec2d */
    a=f->r[1];b=0x00000251u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052ec33 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec34 */
    a=0x007027d4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec39 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec3b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ec3c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ec41u;f->boundary(0x0052ec3cu,0x004d6860u,f);
    /* 0052ec41 */
    a=f->r[4];b=0x0000005cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052ec44 */
    f->r[4]+=4;return;
mi_0052ec50:;
    /* 0052ec50 */
    f->r[0]=MI_U32((0x00775928u));
    /* 0052ec55 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0052ec58 */
    if(!(f->flags&MI_ZF))goto mi_0052ec5f;
    /* 0052ec5a */
    goto mi_0052ecc0;
mi_0052ec5f:;
    /* 0052ec5f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0052ec62 */
    if(!(f->flags&MI_ZF))goto mi_0052ec69;
    /* 0052ec64 */
    goto mi_0052ef60;
mi_0052ec69:;
    /* 0052ec69 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0052ec6c */
    if((f->flags&MI_ZF))goto mi_0052ecb9;
    /* 0052ec6e */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0052ec71 */
    if(!(f->flags&MI_ZF))goto mi_0052ec78;
    /* 0052ec73 */
    f->boundary(0x0052ec73u,0x0052f850u,f);return;
mi_0052ec78:;
    /* 0052ec78 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0052ec7b */
    if(!(f->flags&MI_ZF))goto mi_0052ec82;
    /* 0052ec7d */
    f->boundary(0x0052ec7du,0x0052f850u,f);return;
mi_0052ec82:;
    /* 0052ec82 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0052ec85 */
    if(!(f->flags&MI_ZF))goto mi_0052ec8c;
    /* 0052ec87 */
    f->boundary(0x0052ec87u,0x0052f850u,f);return;
mi_0052ec8c:;
    /* 0052ec8c */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0052ec8f */
    if(!(f->flags&MI_ZF))goto mi_0052ec96;
    /* 0052ec91 */
    f->boundary(0x0052ec91u,0x0052f850u,f);return;
mi_0052ec96:;
    /* 0052ec96 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0052ec99 */
    if(!(f->flags&MI_ZF))goto mi_0052eca0;
    /* 0052ec9b */
    f->boundary(0x0052ec9bu,0x0052f850u,f);return;
mi_0052eca0:;
    /* 0052eca0 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 0052eca3 */
    if(!(f->flags&MI_ZF))goto mi_0052ecaa;
    /* 0052eca5 */
    f->boundary(0x0052eca5u,0x0052f850u,f);return;
mi_0052ecaa:;
    /* 0052ecaa */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 0052ecad */
    if((f->flags&MI_ZF))goto mi_0052ecb4;
    /* 0052ecaf */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0052ecb2 */
    if(!(f->flags&MI_ZF))goto mi_0052ecb9;
mi_0052ecb4:;
    /* 0052ecb4 */
    goto mi_0052e8c0;
mi_0052ecb9:;
    /* 0052ecb9 */
    f->r[4]+=4;return;
mi_0052ecc0:;
    /* 0052ecc0 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052ecc5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ecc6 */
    MI_U32((0x00d3b55cu))=(uint32_t)(0x00000126u);
    /* 0052ecd0 */
    MI_U32((0x00d3b554u))=(uint32_t)(0x0000017eu);
    /* 0052ecda */
    f->r[1]=(0xffffff99u+f->r[0]);
    /* 0052ecdd */
    f->r[2]=(0xfffffff1u+f->r[0]);
    /* 0052ece0 */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[1]);
    /* 0052ece6 */
    f->r[1]=MI_U32((0x00775920u));
    /* 0052ecec */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0052ecee */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[2]);
    /* 0052ecf4 */
    if(!(f->flags&MI_ZF))goto mi_0052edb2;
    /* 0052ecfa */
    f->r[6]=MI_U32((0x00775924u));
    /* 0052ed00 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed05 */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed07 */
    a=f->r[0];b=0xffffff98u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052ed0a */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed0c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed0d */
    a=0x00000125u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed12 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ed17u;f->boundary(0x0052ed12u,0x004cd9d0u,f);
    /* 0052ed17 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052ed1c */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052ed1f */
    a=f->r[0];b=0xffffff99u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052ed22 */
    f->r[1]=0x00f55bb0u;
    /* 0052ed27 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed28 */
    a=0x00000126u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed2d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed2e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ed33u;f->boundary(0x0052ed2eu,0x0053e590u,f);
    /* 0052ed33 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0052ed39 */
    f->r[0]=f->r[6];
    /* 0052ed3b */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052ed3d */
    if(mi_signed_less(f->flags))goto mi_0052ed41;
    /* 0052ed3f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052ed41:;
    /* 0052ed41 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052ed44 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x010d6a32u+f->r[0])))&0xffu)<<0);
    /* 0052ed4a */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052ed4c */
    if((f->flags&MI_ZF))goto mi_0052edad;
    /* 0052ed4e */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052ed50 */
    f->r[0]=f->r[6];
    /* 0052ed52 */
    if(mi_signed_less(f->flags))goto mi_0052ed56;
    /* 0052ed54 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052ed56:;
    /* 0052ed56 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052ed59 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed5a */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052ed5c */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[0]));
    /* 0052ed63 */
    f->r[0]=f->r[6];
    /* 0052ed65 */
    if(mi_signed_less(f->flags))goto mi_0052ed69;
    /* 0052ed67 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052ed69:;
    /* 0052ed69 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052ed6c */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052ed6e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[0]));
    /* 0052ed75 */
    f->r[0]=f->r[6];
    /* 0052ed77 */
    if(mi_signed_less(f->flags))goto mi_0052ed7b;
    /* 0052ed79 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052ed7b:;
    /* 0052ed7b */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052ed81 */
    a=f->r[7];b=0x00000126u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0052ed87 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052ed8a */
    f->r[2]=(0xffffff99u+f->r[1]+f->r[2]*1u);
    /* 0052ed8e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052ed90 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x010d6a34u+f->r[0])))&0xffu)<<0);
    /* 0052ed96 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed97 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052ed99 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed9a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ed9b */
    f->r[1]=0x00f55bb0u;
    /* 0052eda0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052eda5u;f->boundary(0x0052eda0u,0x0053e590u,f);
    /* 0052eda5 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052edaa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052edab */
    goto mi_0052edb2;
mi_0052edad:;
    /* 0052edad */
    f->r[0]=MI_U32((0x00e38dacu));
mi_0052edb2:;
    /* 0052edb2 */
    f->r[1]=MI_U32((0x00e38db0u));
    /* 0052edb8 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[0]);
    /* 0052edbd */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[1]);
    /* 0052edc3 */
    a=0x0000c618u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052edc8 */
    a=f->r[1];b=0xfffffe68u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052edce */
    a=0x00000062u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052edd0 */
    a=f->r[0];b=0xffffff92u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052edd3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052edd4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052edd5 */
    a=0x00000183u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052edda */
    MI_U32((0x00d3b55cu))=(uint32_t)(0x00000000u);
    /* 0052ede4 */
    MI_U32((0x00d3b554u))=(uint32_t)(0x00000000u);
    /* 0052edee */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052edf3u;f->boundary(0x0052edeeu,0x004cd9d0u,f);
    /* 0052edf3 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052edf9 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052edfe */
    f->r[1]=MI_U32((0x00775924u));
    /* 0052ee04 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee06 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee07 */
    a=f->r[0];b=0xffffffe4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052ee0a */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee0c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee0d */
    a=0x000000e1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee12 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee17 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0052ee18 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee1a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee1b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ee20u;f->boundary(0x0052ee1bu,0x004cce90u,f);
    /* 0052ee20 */
    f->r[6]=MI_U32((0x00775920u));
    /* 0052ee26 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052ee2c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[6]=t;
    /* 0052ee2e */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,2);f->r[6]=t;
    /* 0052ee30 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee32 */
    a=f->r[2];b=0xffffff96u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052ee35 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee3a */
    a=f->r[6];b=0xffff0800u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0052ee40 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee41 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee46 */
    a=f->r[6];b=0x0000f800u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0052ee4c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ee51u;f->boundary(0x0052ee4cu,0x004cd930u,f);
    /* 0052ee51 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052ee56 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052ee5c */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052ee62 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052ee65 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee66 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052ee6b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee6c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee6d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee6e */
    a=f->r[0];b=0xffffff9au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052ee71 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee76 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee77 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee7c */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee7e */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ee85u;f->boundary(0x0052ee80u,0x004d6250u,f);
    /* 0052ee85 */
    f->r[6]=MI_U32((0x00775920u));
    /* 0052ee8b */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052ee91 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0052ee92 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ee94 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[6]=t;
    /* 0052ee96 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,2);f->r[6]=t;
    /* 0052ee98 */
    a=f->r[1];b=0xffffffafu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052ee9b */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eea0 */
    a=f->r[6];b=0xffff0800u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0052eea6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eea7 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eeac */
    a=f->r[6];b=0x0000f800u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0052eeb2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052eeb7u;f->boundary(0x0052eeb2u,0x004cd930u,f);
    /* 0052eeb7 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052eebd */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052eec2 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052eec8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eec9 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052eecf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eed0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eed1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eed2 */
    a=f->r[2];b=0xffffffb3u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052eed5 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eeda */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eedb */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eee0 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eee2 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052eee4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052eee9u;f->boundary(0x0052eee4u,0x004d6250u,f);
    /* 0052eee9 */
    f->r[6]=MI_U32((0x00775920u));
    /* 0052eeef */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052eef5 */
    a=f->r[4];b=0x00000058u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052eef8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052eefa */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0052eefd */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0052ef00 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052ef01 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef03 */
    a=f->r[1];b=0xffffffc8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052ef06 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef0b */
    a=f->r[0];b=0x0000f800u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0052ef10 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef11 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef16 */
    f->r[6]=f->r[0];
    /* 0052ef18 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ef1du;f->boundary(0x0052ef18u,0x004cd930u,f);
    /* 0052ef1d */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052ef23 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052ef28 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052ef2e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef2f */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052ef35 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef36 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef37 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef38 */
    a=f->r[2];b=0xffffffccu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052ef3b */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef40 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef41 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef46 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef48 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef4a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ef4fu;f->boundary(0x0052ef4au,0x004d6250u,f);
    /* 0052ef4f */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052ef52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052ef53 */
    f->r[4]+=4;return;
mi_0052ef60:;
    /* 0052ef60 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052ef65 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0052ef68 */
    MI_U32((0x00d3b55cu))=(uint32_t)(0x00000126u);
    /* 0052ef72 */
    MI_U32((0x00d3b554u))=(uint32_t)(0x0000017eu);
    /* 0052ef7c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef7d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef7e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef7f */
    f->r[6]=MI_U32((0x00775924u));
    /* 0052ef85 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef86 */
    f->r[1]=(0xffffff99u+f->r[0]);
    /* 0052ef89 */
    f->r[2]=(0xfffffff1u+f->r[0]);
    /* 0052ef8c */
    a=0x0000f73bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef91 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef93 */
    a=f->r[0];b=0xffffffcau;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052ef96 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef98 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef99 */
    a=0x000000e1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ef9e */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[1]);
    /* 0052efa4 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[2]);
    /* 0052efaa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052efafu;f->boundary(0x0052efaau,0x004cd9d0u,f);
    /* 0052efaf */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052efb4 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052efba */
    f->r[2]=MI_U32((0x00775924u));
    /* 0052efc0 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0052efc2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efc3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efc4 */
    a=f->r[1];b=0xffffffccu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052efc7 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efc9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efca */
    a=0x000000e1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efcf */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efd4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0052efd5 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efd7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052efd8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052efddu;f->boundary(0x0052efd8u,0x004cce90u,f);
    /* 0052efdd */
    f->r[0]=MI_U32((0x0077591cu));
    /* 0052efe2 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052efe5 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052efe8 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0052efeb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052efed */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f748u+f->r[1]*2u)))&0xffu)<<0);
    /* 0052eff4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052eff6 */
    if(!(f->flags&MI_ZF))goto mi_0052f0e0;
    /* 0052effc */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f002 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f007 */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f009 */
    a=f->r[2];b=0xffffff98u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f00c */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f00e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f00f */
    a=0x00000125u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f014 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f019u;f->boundary(0x0052f014u,0x004cd9d0u,f);
    /* 0052f019 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f01e */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f021 */
    a=f->r[0];b=0xffffff99u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f024 */
    f->r[1]=0x00f55bb0u;
    /* 0052f029 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f02a */
    a=0x00000126u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f02f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f030 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f035u;f->boundary(0x0052f030u,0x0053e590u,f);
    /* 0052f035 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0052f03b */
    f->r[0]=f->r[6];
    /* 0052f03d */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052f03f */
    if(mi_signed_less(f->flags))goto mi_0052f043;
    /* 0052f041 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052f043:;
    /* 0052f043 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052f046 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x010d6a32u+f->r[0])))&0xffu)<<0);
    /* 0052f04c */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052f04e */
    if((f->flags&MI_ZF))goto mi_0052f0e0;
    /* 0052f054 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052f056 */
    f->r[0]=f->r[6];
    /* 0052f058 */
    if(mi_signed_less(f->flags))goto mi_0052f05c;
    /* 0052f05a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052f05c:;
    /* 0052f05c */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052f05f */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052f061 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[0]));
    /* 0052f068 */
    f->r[0]=f->r[6];
    /* 0052f06a */
    if(mi_signed_less(f->flags))goto mi_0052f06e;
    /* 0052f06c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052f06e:;
    /* 0052f06e */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052f071 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052f073 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[0]));
    /* 0052f07a */
    f->r[0]=f->r[6];
    /* 0052f07c */
    if(mi_signed_less(f->flags))goto mi_0052f080;
    /* 0052f07e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052f080:;
    /* 0052f080 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052f082 */
    f->r[7]=f->r[6];
    /* 0052f084 */
    if(mi_signed_less(f->flags))goto mi_0052f088;
    /* 0052f086 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0052f088:;
    /* 0052f088 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052f08b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052f08d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x010d6a34u+f->r[0])))&0xffu)<<0);
    /* 0052f093 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f094 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f095 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f09au;f->boundary(0x0052f095u,0x0052f790u,f);
    /* 0052f09a */
    a=f->r[7];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 0052f09d */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f0a0 */
    MI_U8((0x010d6a34u+f->r[7]))=(uint8_t)((f->r[0]&0xffu));
    /* 0052f0a6 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0052f0ac */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052f0ae */
    f->r[0]=f->r[6];
    /* 0052f0b0 */
    if(mi_signed_less(f->flags))goto mi_0052f0b4;
    /* 0052f0b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052f0b4:;
    /* 0052f0b4 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f0ba */
    a=f->r[5];b=0x00000126u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0052f0c0 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052f0c3 */
    f->r[1]=(0xffffff99u+f->r[2]+f->r[3]*1u);
    /* 0052f0c7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052f0c9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x010d6a34u+f->r[0])))&0xffu)<<0);
    /* 0052f0cf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f0d0 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f0d2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f0d3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f0d4 */
    f->r[1]=0x00f55bb0u;
    /* 0052f0d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f0deu;f->boundary(0x0052f0d9u,0x0053e590u,f);
    /* 0052f0de */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_0052f0e0:;
    /* 0052f0e0 */
    f->r[0]=MI_U32((0x00e38db0u));
    /* 0052f0e5 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f0eb */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[0]);
    /* 0052f0f0 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[1]);
    /* 0052f0f6 */
    a=0x0000f73bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f0fb */
    a=f->r[0];b=0xfffffe68u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f100 */
    a=0x00000062u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f102 */
    a=f->r[1];b=0xffffff92u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f105 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f106 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f107 */
    a=0x00000183u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f10c */
    MI_U32((0x00d3b55cu))=(uint32_t)(f->r[7]);
    /* 0052f112 */
    MI_U32((0x00d3b554u))=(uint32_t)(f->r[7]);
    /* 0052f118 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f11du;f->boundary(0x0052f118u,0x004cd9d0u,f);
    /* 0052f11d */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052f122 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f128 */
    f->r[2]=MI_U32((0x00775924u));
    /* 0052f12e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f12f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f130 */
    a=f->r[1];b=0xffffffe4u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f133 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f135 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f136 */
    a=0x000000e1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f13b */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f140 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0052f141 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f143 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f144 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f149u;f->boundary(0x0052f144u,0x004cce90u,f);
    /* 0052f149 */
    f->r[0]=MI_U32((0x0077591cu));
    /* 0052f14e */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f151 */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[7]);
    /* 0052f157 */
    f->r[6]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052f15a */
    a=f->r[6];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0052f15d */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0052f15f */
    a=f->r[6];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0052f161 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f748u+f->r[6])))&0xffu)<<0);
    /* 0052f167 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0052f169 */
    if(!(f->flags&MI_ZF))goto mi_0052f5f5;
    /* 0052f16f */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f174 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f176 */
    a=f->r[0];b=0xffffff90u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f179 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f17e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f17f */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f184 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f189u;f->boundary(0x0052f184u,0x004cd930u,f);
    /* 0052f189 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052f18f */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052f195 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052f19a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f19b */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f1a1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1a2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1a4 */
    a=f->r[1];b=0xffffff94u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f1a7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052f1a9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f760u+f->r[6])))&0xffu)<<0);
    /* 0052f1af */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1b5 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1ba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1bb */
    a=0x00000027u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f1c2u;f->boundary(0x0052f1bdu,0x004d6250u,f);
    /* 0052f1c2 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f1c7 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1c9 */
    a=f->r[0];b=0xffffff90u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f1cc */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1d1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1d2 */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f1dcu;f->boundary(0x0052f1d7u,0x004cd930u,f);
    /* 0052f1dc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e39088u)))&0xffu)<<0);
    /* 0052f1e1 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f1e4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052f1e6 */
    if((f->flags&MI_ZF))goto mi_0052f21e;
    /* 0052f1e8 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052f1ee */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052f1f4 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052f1f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f1fa */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f200 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f201 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f202 */
    a=f->r[1];b=0xffffff94u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f205 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f206 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052f208 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[6])))&0xffu)<<0);
    /* 0052f20e */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f213 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f214 */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f219 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f21a */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f21c */
    goto mi_0052f254;
mi_0052f21e:;
    /* 0052f21e */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052f223 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052f229 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052f22f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f230 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f235 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f236 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f237 */
    a=f->r[0];b=0xffffff94u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f23a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f23b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052f23d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[6])))&0xffu)<<0);
    /* 0052f243 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f248 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f249 */
    a=0x00000251u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f24e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f24f */
    a=0x000000c3u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052f254:;
    /* 0052f254 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f259u;f->boundary(0x0052f254u,0x004d6250u,f);
    /* 0052f259 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f25f */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f262 */
    a=f->r[2];b=0xffffff90u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f265 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f267 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f269 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f26a */
    a=0x00000125u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f26f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f274u;f->boundary(0x0052f26fu,0x004cd930u,f);
    /* 0052f274 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052f279 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052f27f */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052f285 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f286 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f28b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f28c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f28d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f28e */
    a=f->r[0];b=0xffffff94u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f291 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052f293 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0077f75bu+f->r[6])))&0xffu)<<0);
    /* 0052f299 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f29b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f29c */
    a=0x00000125u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2a1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2a2 */
    a=0x00000056u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f2a9u;f->boundary(0x0052f2a4u,0x004d6250u,f);
    /* 0052f2a9 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052f2af */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052f2b4 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052f2ba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2bb */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f2c1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2c2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2c3 */
    a=f->r[2];b=0xffffffadu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f2c6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2c7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2c8 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2cd */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2cf */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f2d6u;f->boundary(0x0052f2d1u,0x004d62f0u,f);
    /* 0052f2d6 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f2db */
    a=f->r[4];b=0x00000054u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f2de */
    a=f->r[0];b=0xffffffa9u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f2e1 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2e3 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2e5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2e6 */
    a=0x000001c5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f2f0u;f->boundary(0x0052f2ebu,0x004cd930u,f);
    /* 0052f2f0 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052f2f6 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f2fc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2fd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f2fe */
    a=f->r[2];b=0xffffffadu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f301 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f303 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f304 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052f306 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f762u+f->r[6])))&0xffu)<<0);
    /* 0052f30c */
    a=0x000001c5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f311 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f316 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f318 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f319 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f31eu;f->boundary(0x0052f319u,0x004cce90u,f);
    /* 0052f31e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f760u+f->r[6])))&0xffu)<<0);
    /* 0052f324 */
    a=f->r[4];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f327 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052f329 */
    if((f->flags&MI_ZF))goto mi_0052f375;
    /* 0052f32b */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f331 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f333 */
    a=f->r[1];b=0xffffffa9u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f336 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f338 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f339 */
    a=0x0000020bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f33e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f343u;f->boundary(0x0052f33eu,0x004cd930u,f);
    /* 0052f343 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052f349 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052f34e */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052f354 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f355 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f35b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f35c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f35d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f35e */
    a=f->r[2];b=0xffffffadu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f361 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f363 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f364 */
    a=0x0000020bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f369 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f36b */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f36d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f372u;f->boundary(0x0052f36du,0x004d6250u,f);
    /* 0052f372 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0052f375:;
    /* 0052f375 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052f37a */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052f380 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052f386 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f387 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f38c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f38d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f38e */
    a=f->r[0];b=0xffffffadu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f391 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f392 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f393 */
    a=0x0000028du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f398 */
    a=0x00000022u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f39a */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f39c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f3a1u;f->boundary(0x0052f39cu,0x004d62f0u,f);
    /* 0052f3a1 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f3a7 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3a9 */
    a=f->r[1];b=0xffffffa9u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f3ac */
    a=0x00000050u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3ae */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3af */
    a=0x000002bfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3b4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f3b9u;f->boundary(0x0052f3b4u,0x004cd930u,f);
    /* 0052f3b9 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052f3bf */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f3c4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0077f764u+f->r[6]));
    /* 0052f3cb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3cc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3cd */
    a=f->r[0];b=0xffffffadu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f3d0 */
    a=0x0000003cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3d3 */
    a=0x000002bfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3d8 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3dd */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3df */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f3e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f3e5u;f->boundary(0x0052f3e0u,0x004cce90u,f);
    /* 0052f3e5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f760u+f->r[6])))&0xffu)<<0);
    /* 0052f3eb */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f3ee */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052f3f0 */
    if(!(f->flags&MI_ZF))goto mi_0052f4bc;
    /* 0052f3f6 */
    f->r[7]=(0x0077f76du+f->r[6]);
    /* 0052f3fc */
    f->r[6]=0x0000018fu;
mi_0052f401:;
    /* 0052f401 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f407 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f409 */
    a=f->r[2];b=0xffffffc2u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f40c */
    f->r[3]=(0xffffffffu+f->r[6]);
    /* 0052f40f */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f411 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f412 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f413 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f418u;f->boundary(0x0052f413u,0x004cd890u,f);
    /* 0052f418 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f41e */
    f->r[1]=MI_U32((0x00f571fcu));
    /* 0052f424 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052f426 */
    a=f->r[2];b=0xffffffc3u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f429 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffff9u+f->r[7])))&0xffu)<<0);
    /* 0052f42c */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f42e */
    f->r[5]=f->r[0];
    /* 0052f430 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0052f434 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f435 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0052f439 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f43eu;f->boundary(0x0052f439u,0x0047d350u,f);
    /* 0052f43e */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 0052f442 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0052f446 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f449 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f44b */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f44d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f44e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f44f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f450 */
    f->r[1]=0x00f55bb0u;
    /* 0052f455 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f45au;f->boundary(0x0052f455u,0x0053e590u,f);
    /* 0052f45a */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f460 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f462 */
    a=f->r[2];b=0xffffffdeu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f465 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f467 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f468 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f469 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f46eu;f->boundary(0x0052f469u,0x004cd890u,f);
    /* 0052f46e */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f473 */
    f->r[5]=MI_U32((0x00f571fcu));
    /* 0052f479 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0052f47b */
    a=f->r[0];b=0xffffffdfu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f47e */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[7])))&0xffu)<<0);
    /* 0052f480 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f482 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f483 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0052f487 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f48cu;f->boundary(0x0052f487u,0x0047d350u,f);
    /* 0052f48c */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0052f490 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f493 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f495 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f497 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f498 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f499 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f49a */
    f->r[1]=0x00f55bb0u;
    /* 0052f49f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f4a4u;f->boundary(0x0052f49fu,0x0053e590u,f);
    /* 0052f4a4 */
    a=f->r[6];b=0x0000001bu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0052f4a7 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0052f4a8 */
    a=f->r[6];b=0x0000024cu;t=mi_alu(f,a,b,32,1);
    /* 0052f4ae */
    if(mi_signed_less(f->flags))goto mi_0052f401;
    /* 0052f4b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052f4b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052f4b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052f4b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0052f4b8 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f4bb */
    f->r[4]+=4;return;
mi_0052f4bc:;
    /* 0052f4bc */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052f4c2 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052f4c7 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052f4cd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4ce */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f4d4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4d5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4d6 */
    a=f->r[2];b=0xffffffc9u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f4d9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4da */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4db */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4e0 */
    a=0x00000024u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4e2 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4e4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f4e9u;f->boundary(0x0052f4e4u,0x004d62f0u,f);
    /* 0052f4e9 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f4ec */
    f->r[3]=(0x0077f766u+f->r[6]);
    /* 0052f4f2 */
    f->r[7]=0x0000020cu;
mi_0052f4f7:;
    /* 0052f4f7 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f4fc */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f4fe */
    a=f->r[0];b=0xffffffc2u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f501 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f503 */
    f->r[1]=(0xffffffffu+f->r[7]);
    /* 0052f506 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f507 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f508 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f50du;f->boundary(0x0052f508u,0x004cd890u,f);
    /* 0052f50d */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f513 */
    f->r[0]=MI_U32((0x00f571fcu));
    /* 0052f518 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052f51a */
    a=f->r[1];b=0xffffffc3u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f51d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[3])))&0xffu)<<0);
    /* 0052f51f */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f521 */
    f->r[5]=f->r[2];
    /* 0052f523 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0052f527 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f528 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0052f52c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f531u;f->boundary(0x0052f52cu,0x0047d350u,f);
    /* 0052f531 */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 0052f535 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f538 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f53a */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0052f53e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f540 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f541 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f542 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f543 */
    f->r[1]=0x00f55bb0u;
    /* 0052f548 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f54du;f->boundary(0x0052f548u,0x0053e590u,f);
    /* 0052f54d */
    a=f->r[7];b=0x0000001eu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0052f550 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0052f551 */
    a=f->r[7];b=0x000002fcu;t=mi_alu(f,a,b,32,1);
    /* 0052f557 */
    if(mi_signed_less(f->flags))goto mi_0052f4f7;
    /* 0052f559 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052f55e */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052f564 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052f56a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f56b */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f570 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f571 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f573 */
    a=f->r[0];b=0xffffffe5u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f576 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f577 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f578 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f57d */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f57f */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f581 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f586u;f->boundary(0x0052f581u,0x004d62f0u,f);
    /* 0052f586 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f589 */
    f->r[6]=(0x0077f77cu+f->r[6]);
    /* 0052f58f */
    f->r[7]=0x0000020cu;
mi_0052f594:;
    /* 0052f594 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f59a */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f59c */
    a=f->r[1];b=0xffffffdeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f59f */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5a1 */
    f->r[2]=(0xffffffffu+f->r[7]);
    /* 0052f5a4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5a5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f5abu;f->boundary(0x0052f5a6u,0x004cd890u,f);
    /* 0052f5ab */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f5b0 */
    f->r[5]=MI_U32((0x00f571fcu));
    /* 0052f5b6 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0052f5b8 */
    a=f->r[0];b=0xffffffdfu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f5bb */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 0052f5bd */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5bf */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5c0 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0052f5c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f5c9u;f->boundary(0x0052f5c4u,0x0047d350u,f);
    /* 0052f5c9 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0052f5cd */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f5d0 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f5d2 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f5d4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5d5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5d6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f5d7 */
    f->r[1]=0x00f55bb0u;
    /* 0052f5dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f5e1u;f->boundary(0x0052f5dcu,0x0053e590u,f);
    /* 0052f5e1 */
    a=f->r[7];b=0x0000001eu;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0052f5e4 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0052f5e5 */
    a=f->r[7];b=0x000002fcu;t=mi_alu(f,a,b,32,1);
    /* 0052f5eb */
    if(mi_signed_less(f->flags))goto mi_0052f594;
    /* 0052f5ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052f5ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052f5ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052f5f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0052f5f1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f5f4 */
    f->r[4]+=4;return;
mi_0052f5f5:;
    /* 0052f5f5 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0052f5f7 */
    if(!(f->flags&MI_ZF))goto mi_0052f71e;
    /* 0052f5fd */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052f603 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052f608 */
    f->r[1]=MI_U32((0x00f56fb4u));
    /* 0052f60e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f60f */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f615 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f616 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f617 */
    a=f->r[2];b=0xffffff9cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f61a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f61b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f61c */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f621 */
    a=0x00000045u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f623 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f625 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f62au;f->boundary(0x0052f625u,0x004d62f0u,f);
    /* 0052f62a */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052f62f */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052f635 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052f63b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f63c */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f641 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f642 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f643 */
    a=f->r[0];b=0xffffffc0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f646 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f647 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f648 */
    a=0x000001a7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f64d */
    a=0x00000046u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f64f */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f651 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f656u;f->boundary(0x0052f651u,0x004d62f0u,f);
    /* 0052f656 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f65c */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f65f */
    a=f->r[1];b=0xffffffbcu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f662 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f664 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f666 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f667 */
    a=0x0000020bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f66c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f671u;f->boundary(0x0052f66cu,0x004cd930u,f);
    /* 0052f671 */
    f->r[2]=MI_U32((0x00f56f8cu));
    /* 0052f677 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f67c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f67d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f67e */
    a=f->r[0];b=0xffffffc0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f681 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f683 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f684 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052f686 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0077f784u+f->r[6])))&0xffu)<<0);
    /* 0052f68c */
    a=0x0000020bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f691 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f696 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f698 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f699 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f69eu;f->boundary(0x0052f699u,0x004cce90u,f);
    /* 0052f69e */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052f6a4 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052f6a9 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052f6af */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6b0 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f6b6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6b7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6b8 */
    a=f->r[2];b=0xffffffdeu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f6bb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6bc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6bd */
    a=0x000001a7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6c2 */
    a=0x00000047u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6c4 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f6cbu;f->boundary(0x0052f6c6u,0x004d62f0u,f);
    /* 0052f6cb */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f6d0 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f6d3 */
    a=f->r[0];b=0xffffffdau;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f6d6 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6d8 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6da */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6db */
    a=0x0000020bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6e0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f6e5u;f->boundary(0x0052f6e0u,0x004cd930u,f);
    /* 0052f6e5 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0052f6eb */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f6f1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6f2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6f3 */
    a=f->r[2];b=0xffffffdeu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f6f6 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6f9 */
    a=0x0000020bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f6fe */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f703 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052f705 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f707 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f785u+f->r[6])))&0xffu)<<0);
    /* 0052f70d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f70e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f713u;f->boundary(0x0052f70eu,0x004cce90u,f);
    /* 0052f713 */
    a=f->r[4];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f716 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052f717 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052f718 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052f719 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0052f71a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f71d */
    f->r[4]+=4;return;
mi_0052f71e:;
    /* 0052f71e */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0052f720 */
    if(!(f->flags&MI_ZF))goto mi_0052f77c;
    /* 0052f722 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f728 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f72a */
    a=f->r[1];b=0xffffffacu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f72d */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f732 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f733 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f738 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f73du;f->boundary(0x0052f738u,0x004cd930u,f);
    /* 0052f73d */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052f743 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052f748 */
    f->r[1]=MI_U32((0x00f56f84u));
    /* 0052f74e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f74f */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 0052f755 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f756 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f757 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f758 */
    a=f->r[2];b=0xffffffacu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052f75b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052f75d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f761u+f->r[6])))&0xffu)<<0);
    /* 0052f763 */
    a=0x000000b4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f768 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f769 */
    a=0x00000189u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f76e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f76f */
    a=0x000000c4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f774 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f779u;f->boundary(0x0052f774u,0x004d6250u,f);
    /* 0052f779 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0052f77c:;
    /* 0052f77c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052f77d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052f77e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052f77f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0052f780 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f783 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_0052f790(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_0052f790;
mi_0052f790:;
    /* 0052f790 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 0052f794 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f795 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f796 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0052f798 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f799 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f79a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0052f79c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0052f7a3;
    /* 0052f79e */
    f->r[0]=0x00000001u;
mi_0052f7a3:;
    /* 0052f7a3 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0052f7a7 */
    f->r[7]=MI_U32((0x0126517cu));
    /* 0052f7ad */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0052f7af */
    f->r[6]=f->r[1];
    /* 0052f7b1 */
    if(mi_signed_less(f->flags))goto mi_0052f7b5;
    /* 0052f7b3 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0052f7b5:;
    /* 0052f7b5 */
    a=f->r[6];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0052f7b8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052f7ba */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x010d6a32u+f->r[6])))&0xffu)<<0);
    /* 0052f7c0 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00605b38u+f->r[2])))&0xffu)<<0);
    /* 0052f7c6 */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052f7c8 */
    if((f->flags&MI_ZF))goto mi_0052f846;
    /* 0052f7ca */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0052f7cc */
    f->r[2]=f->r[1];
    /* 0052f7ce */
    if(mi_signed_less(f->flags))goto mi_0052f7d2;
    /* 0052f7d0 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0052f7d2:;
    /* 0052f7d2 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0052f7d5 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x010d6a28u+f->r[2])))&0xffu)<<0);
    /* 0052f7db */
    a=(f->r[3]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052f7dd */
    if((f->flags&MI_ZF))goto mi_0052f82c;
    /* 0052f7df */
    a=(f->r[0]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 0052f7e1 */
    if((f->flags&MI_ZF))goto mi_0052f7e8;
    /* 0052f7e3 */
    f->r[5]=0x00000001u;
mi_0052f7e8:;
    /* 0052f7e8 */
    a=f->r[0];b=0x0000007fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0052f7eb */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0052f7ed */
    if(!(f->flags&MI_ZF))goto mi_0052f81f;
    /* 0052f7ef */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052f7f0 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0052f7f2 */
    f->r[2]=f->r[1];
    /* 0052f7f4 */
    if(mi_signed_less(f->flags))goto mi_0052f7f8;
    /* 0052f7f6 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0052f7f8:;
    /* 0052f7f8 */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0052f7fb */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a16u+f->r[2]));
    /* 0052f802 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0052f804 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0052f846;
    /* 0052f806 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0052f808 */
    f->r[0]=f->r[1];
    /* 0052f80a */
    if(mi_signed_less(f->flags))goto mi_0052f80e;
    /* 0052f80c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052f80e:;
    /* 0052f80e */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052f811 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052f812 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052f813 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a16u+f->r[0]));
    /* 0052f81a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052f81b */
    a=(f->r[0]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0052f81d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0052f81e */
    f->r[4]+=4;return;
mi_0052f81f:;
    /* 0052f81f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052f820 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0052f823 */
    if(mi_signed_less(f->flags))goto mi_0052f841;
    /* 0052f825 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052f826 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052f827 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052f828 */
    a=(f->r[0]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0052f82a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0052f82b */
    f->r[4]+=4;return;
mi_0052f82c:;
    /* 0052f82c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052f82d */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0052f82f */
    if(mi_signed_less(f->flags))goto mi_0052f833;
    /* 0052f831 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0052f833:;
    /* 0052f833 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0052f836 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a16u+f->r[1]));
    /* 0052f83d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052f83f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0052f846;
mi_0052f841:;
    /* 0052f841 */
    f->r[0]=0x00000001u;
mi_0052f846:;
    /* 0052f846 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0052f847 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0052f848 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0052f849 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0052f84a */
    f->r[4]+=4;return;
}

void restore_resource_render_C_0052f850(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_0052f850;
mi_0052f850:;
    /* 0052f850 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0052f855 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f857 */
    a=f->r[0];b=0xffffffdeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052f85a */
    a=0x00000096u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f85f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f860 */
    a=0x000000e7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f865 */
    MI_U32((0x00e20778u))=(uint32_t)(0x00000000u);
    /* 0052f86f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f874u;f->boundary(0x0052f86fu,0x004cd930u,f);
    /* 0052f874 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052f87a */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052f880 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0052f885 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f886 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 0052f88c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f88d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f88f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f890 */
    a=f->r[1];b=0xffffffe2u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052f893 */
    a=0x00000078u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f895 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f896 */
    a=0x000000e7u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f89b */
    a=0x000000ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f8a0 */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f8a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f8a7u;f->boundary(0x0052f8a2u,0x004d6250u,f);
    /* 0052f8a7 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052f8aa */
    f->r[4]+=4;return;
}

void restore_resource_render_C_0052f8b0(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    goto mi_0052f8b0;
mi_0052f8b0:;
    /* 0052f8b0 */
    f->r[0]=MI_U32((0x00f53fd8u));
    /* 0052f8b5 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0052f8b8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052f8b9 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0052f8bc */
    MI_U32((0x00f53fd8u))=(uint32_t)(f->r[0]);
    /* 0052f8c1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0052f8cd;
    /* 0052f8c3 */
    MI_U32((0x00f53fd8u))=(uint32_t)(0x00000000u);
mi_0052f8cd:;
    /* 0052f8cd */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f8ce */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f8cf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f8d0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f8d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f8d6u;f->boundary(0x0052f8d1u,0x004cd810u,f);
    /* 0052f8d6 */
    f->r[6]=MI_U32((0x005d2a18u));
    /* 0052f8dc */
    f->r[1]=MI_U32((0x00e38d9cu));
    /* 0052f8e2 */
    f->r[7]=MI_U32((0x005d2a20u));
    /* 0052f8e8 */
    f->r[2]=MI_U32((0x005d2a10u));
    /* 0052f8ee */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*1u);
    /* 0052f8f1 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052f8f3 */
    f->r[0]=MI_U32((0x00e38d98u));
    /* 0052f8f8 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052f8fa */
    f->r[7]=MI_U32((0x005d2a1cu));
    /* 0052f900 */
    MI_U32((0x00d3b574u))=(uint32_t)(f->r[1]);
    /* 0052f906 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052f908 */
    f->r[1]=MI_U32((0x00775910u));
    /* 0052f90e */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052f910 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0052f912 */
    MI_U32((0x00d3b570u))=(uint32_t)(f->r[0]);
    /* 0052f917 */
    if(mi_signed_less(f->flags))goto mi_0052f922;
    /* 0052f919 */
    f->r[1]=(0xffffffffu+f->r[2]);
    /* 0052f91c */
    MI_U32((0x00775910u))=(uint32_t)(f->r[1]);
mi_0052f922:;
    /* 0052f922 */
    f->r[2]=MI_U32((0x005d2a14u));
    /* 0052f928 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052f92a */
    f->r[0]=MI_U32((0x0077590cu));
    /* 0052f92f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0052f931 */
    if(mi_signed_less(f->flags))goto mi_0052f93b;
    /* 0052f933 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 0052f936 */
    MI_U32((0x0077590cu))=(uint32_t)(f->r[0]);
mi_0052f93b:;
    /* 0052f93b */
    MI_U32((0x00d3b56cu))=(uint32_t)(f->r[1]);
    /* 0052f941 */
    f->r[1]=MI_U32((0x00f56db4u));
    /* 0052f947 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f948 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f949 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052f94a */
    f->r[1]=0x00f55bb0u;
    /* 0052f94f */
    MI_U32((0x00d3b568u))=(uint32_t)(f->r[0]);
    /* 0052f954 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052f959u;f->boundary(0x0052f954u,0x0053e590u,f);
    /* 0052f959 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 0052f95f */
    f->r[2]=MI_U32((0x00e38d9cu));
    /* 0052f965 */
    f->r[0]=MI_U32((0x00e38d98u));
    /* 0052f96a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052f96c */
    f->r[7]=MI_U32((0x005d2a1cu));
    /* 0052f972 */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[2]);
    /* 0052f978 */
    f->r[2]=MI_U32((0x005d2a20u));
    /* 0052f97e */
    MI_U32((0x00d3b55cu))=(uint32_t)(f->r[1]);
    /* 0052f984 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052f986 */
    MI_U32((0x00d3b554u))=(uint32_t)(f->r[7]);
    /* 0052f98c */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[0]);
    /* 0052f991 */
    f->r[0]=MI_U32((0x00e92dfcu));
    /* 0052f996 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0052f999 */
    if(!(f->flags&MI_ZF))goto mi_0052f9ad;
    /* 0052f99b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x000081c4u);
    /* 0052f9a3 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x0000ab0au);
    /* 0052f9ab */
    goto mi_0052f9bd;
mi_0052f9ad:;
    /* 0052f9ad */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x000040e4u);
    /* 0052f9b5 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x0000558au);
mi_0052f9bd:;
    /* 0052f9bd */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0052f9c5 */
    f->r[6]=0x0077f74cu;
mi_0052f9ca:;
    /* 0052f9ca */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffffdu+f->r[6])))&0xffu)<<0);
    /* 0052f9cd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052f9cf */
    if((f->flags&MI_ZF))goto mi_0052feb0;
    /* 0052f9d5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffffcu+f->r[6])))&0xffu)<<0);
    /* 0052f9d8 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0052f9da */
    if(!(f->flags&MI_CF))goto mi_0052fe98;
    /* 0052f9e0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[6]));
    /* 0052f9e4 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 0052f9e7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0052f9e9 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0052f9eb */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[6])))&0xffffu)<<0);
    /* 0052f9ef */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0052f9f3 */
    if(!(f->flags&MI_ZF))goto mi_0052fa95;
    /* 0052f9f9 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052f9fe */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052fa04 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa05 */
    f->r[0]=MI_U32((0x00f56f84u));
    /* 0052fa0a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa0b */
    f->r[2]=f->r[7];
    /* 0052fa0d */
    f->r[7]=MI_U32((0x0077590cu));
    /* 0052fa13 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa15 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa16 */
    f->r[0]=MI_U32((0x00775910u));
    /* 0052fa1b */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052fa1d */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
    /* 0052fa21 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fa23 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052fa25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fa27 */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa29 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[6])))&0xffu)<<0);
    /* 0052fa2c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fa2e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa2f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa30 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa31 */
    a=0x000000c4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa36 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fa3bu;f->boundary(0x0052fa36u,0x004d6250u,f);
    /* 0052fa3b */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052fa41 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052fa47 */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 0052fa4b */
    f->r[3]=MI_U32((0x0077590cu));
    /* 0052fa51 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa52 */
    f->r[1]=MI_U32((0x00f56f84u));
    /* 0052fa58 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa59 */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 0052fa5f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa60 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052fa62 */
    f->r[3]=MI_U32((0x00775910u));
    /* 0052fa68 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa69 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 0052fa6f */
    f->r[0]=(0xffffffffu+f->r[2]+f->r[7]*1u);
    /* 0052fa73 */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa75 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fa77 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa78 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fa7a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[6])))&0xffu)<<0);
    /* 0052fa7d */
    f->r[2]=(0xffffffffu+f->r[1]+f->r[5]*1u);
    /* 0052fa81 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa82 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa83 */
    a=0x000000c4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fa88 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fa8du;f->boundary(0x0052fa88u,0x004d6250u,f);
    /* 0052fa8d */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052fa90 */
    goto mi_0052fe8c;
mi_0052fa95:;
    /* 0052fa95 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0052fa97 */
    if(!(f->flags&MI_ZF))goto mi_0052fbf3;
    /* 0052fa9d */
    a=MI_U8((0x00e38ddau));b=0x00000014u;t=mi_alu(f,a,b,8,1);
    /* 0052faa4 */
    if((f->flags&MI_ZF))goto mi_0052fe98;
    /* 0052faaa */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052fab0 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052fab5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fab6 */
    f->r[2]=MI_U32((0x00f56f84u));
    /* 0052fabc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fabd */
    f->r[0]=MI_U32((0x0077590cu));
    /* 0052fac2 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0052fac4 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0052fac8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052faca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052facb */
    f->r[2]=(0xfffffff6u+f->r[7]+f->r[0]*1u);
    /* 0052facf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fad0 */
    f->r[2]=MI_U32((0x00775910u));
    /* 0052fad6 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fad8 */
    f->r[0]=(0xfffffff8u+f->r[1]+f->r[5]*1u);
    /* 0052fadc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052fade */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000038u+f->r[6])))&0xffu)<<0);
    /* 0052fae1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fae2 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fae7 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fae9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052faea */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052faefu;f->boundary(0x0052faeau,0x004d6860u,f);
    /* 0052faef */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052faf5 */
    f->r[7]=MI_U32((0x0077590cu));
    /* 0052fafb */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052fb00 */
    f->r[1]=MI_U32((0x00f56f84u));
    /* 0052fb06 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb07 */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 0052fb0d */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052fb0f */
    f->r[7]=MI_U32((0x0000003cu+f->r[4]));
    /* 0052fb13 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb14 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb19 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb1a */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 0052fb20 */
    f->r[0]=(0xfffffff7u+f->r[2]+f->r[7]*1u);
    /* 0052fb24 */
    f->r[2]=MI_U32((0x00775910u));
    /* 0052fb2a */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fb2c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb2d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fb2f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000038u+f->r[6])))&0xffu)<<0);
    /* 0052fb32 */
    f->r[2]=(0xfffffff7u+f->r[1]+f->r[5]*1u);
    /* 0052fb36 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb37 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb3c */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb3e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb3f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fb44u;f->boundary(0x0052fb3fu,0x004d6860u,f);
    /* 0052fb44 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0052fb4a */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052fb50 */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052fb53 */
    f->r[0]=MI_U32((0x00f56f84u));
    /* 0052fb58 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb59 */
    f->r[1]=MI_U32((0x005d2a1cu));
    /* 0052fb5f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb60 */
    f->r[2]=MI_U32((0x0077590cu));
    /* 0052fb66 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fb68 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb6a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb6b */
    f->r[0]=MI_U32((0x005d2a18u));
    /* 0052fb70 */
    f->r[2]=(0xfffffff6u+f->r[1]+f->r[7]*1u);
    /* 0052fb74 */
    f->r[1]=MI_U32((0x00775910u));
    /* 0052fb7a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fb7c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb7d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052fb7f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000039u+f->r[6])))&0xffu)<<0);
    /* 0052fb82 */
    f->r[1]=(0x00000010u+f->r[0]+f->r[5]*1u);
    /* 0052fb86 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb87 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb8c */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb8e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fb8f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fb94u;f->boundary(0x0052fb8fu,0x004d6860u,f);
    /* 0052fb94 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 0052fb99 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052fb9f */
    f->r[2]=MI_U32((0x00f56f84u));
    /* 0052fba5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fba6 */
    f->r[0]=MI_U32((0x005d2a1cu));
    /* 0052fbab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbac */
    f->r[1]=MI_U32((0x0077590cu));
    /* 0052fbb2 */
    a=0x0000f800u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbb7 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fbb9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbba */
    f->r[1]=(0xfffffff7u+f->r[0]+f->r[7]*1u);
    /* 0052fbbe */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 0052fbc4 */
    f->r[7]=MI_U32((0x00775910u));
    /* 0052fbca */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052fbcc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbcd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052fbcf */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000039u+f->r[6])))&0xffu)<<0);
    /* 0052fbd2 */
    f->r[0]=(0x0000000fu+f->r[2]+f->r[5]*1u);
    /* 0052fbd6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbd7 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbdc */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbde */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fbdf */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fbe4u;f->boundary(0x0052fbdfu,0x004d6860u,f);
    /* 0052fbe4 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 0052fbea */
    f->r[7]=MI_U32((0x005d2a1cu));
    /* 0052fbf0 */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0052fbf3:;
    /* 0052fbf3 */
    f->r[2]=MI_U32((0x0077590cu));
    /* 0052fbf9 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0052fbfd */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0052fbff */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0052fc01 */
    f->r[0]=MI_U32((0x00775910u));
    /* 0052fc06 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fc08 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc09 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fc0b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc0c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc0d */
    f->r[1]=0x00f55bb0u;
    /* 0052fc12 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fc17u;f->boundary(0x0052fc12u,0x0053e590u,f);
    /* 0052fc17 */
    a=MI_U8((0xfffffffcu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0052fc1b */
    if(!(f->flags&MI_ZF))goto mi_0052fe8c;
    /* 0052fc21 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0052fc23 */
    if((f->flags&MI_ZF))goto mi_0052fe8c;
    /* 0052fc29 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0052fc2d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc2e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fc33u;f->boundary(0x0052fc2eu,0x00444920u,f);
    /* 0052fc33 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e39088u)))&0xffu)<<0);
    /* 0052fc38 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052fc3b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052fc3d */
    f->r[0]=MI_U32((0x00f56f84u));
    /* 0052fc42 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc43 */
    if((f->flags&MI_ZF))goto mi_0052fc4f;
    /* 0052fc45 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fc47 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[6])))&0xffu)<<0);
    /* 0052fc4a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc4b */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc4d */
    goto mi_0052fc5a;
mi_0052fc4f:;
    /* 0052fc4f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0052fc51 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[6])))&0xffu)<<0);
    /* 0052fc54 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fc55 */
    a=0x000000c3u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052fc5a:;
    /* 0052fc5a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fc5fu;f->boundary(0x0052fc5au,0x004cc6c0u,f);
    /* 0052fc5f */
    f->r[2]=f->r[0];
    /* 0052fc61 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fc63 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000fu+f->r[6])))&0xffu)<<0);
    /* 0052fc66 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052fc69 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fc6c */
    if((f->flags&MI_ZF))goto mi_0052fd54;
    /* 0052fc72 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052fc73 */
    if((f->flags&MI_ZF))goto mi_0052fd0b;
    /* 0052fc79 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052fc7a */
    f->r[0]=f->r[3];
    /* 0052fc7c */
    if((f->flags&MI_ZF))goto mi_0052fcdc;
    /* 0052fc7e */
    f->r[7]=MI_U32((0x0126517cu));
    /* 0052fc84 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0052fc86 */
    if(mi_signed_less(f->flags))goto mi_0052fc8a;
    /* 0052fc88 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052fc8a:;
    /* 0052fc8a */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fc8d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[0]));
    /* 0052fc94 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fc96 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0052fc97 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fc99 */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 0052fc9f */
    f->r[1]=f->r[0];
    /* 0052fca1 */
    f->r[0]=MI_U32((0x00775910u));
    /* 0052fca6 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 0052fca8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fcaa */
    f->r[0]=f->r[3];
    /* 0052fcac */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fcae */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fcb0 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0052fcb2 */
    if(mi_signed_less(f->flags))goto mi_0052fcb6;
    /* 0052fcb4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052fcb6:;
    /* 0052fcb6 */
    f->r[7]=MI_U32((0x0077590cu));
    /* 0052fcbc */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fcbf */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 0052fcc6 */
    f->r[0]=MI_U32((0x005d2a1cu));
    /* 0052fccb */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052fccd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052fccf */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0052fcd3 */
    f->r[0]=(0x00000005u+f->r[2]+f->r[0]*1u);
    /* 0052fcd7 */
    goto mi_0052fd99;
mi_0052fcdc:;
    /* 0052fcdc */
    f->r[2]=MI_U32((0x0126517cu));
    /* 0052fce2 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0052fce4 */
    if(mi_signed_less(f->flags))goto mi_0052fce8;
    /* 0052fce6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052fce8:;
    /* 0052fce8 */
    f->r[7]=MI_U32((0x005d2a18u));
    /* 0052fcee */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fcf1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[0]));
    /* 0052fcf8 */
    f->r[0]=MI_U32((0x00775910u));
    /* 0052fcfd */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fcff */
    f->r[0]=f->r[3];
    /* 0052fd01 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fd03 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fd05 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0052fd07 */
    if(mi_signed_less(f->flags))goto mi_0052fd74;
    /* 0052fd09 */
    goto mi_0052fd72;
mi_0052fd0b:;
    /* 0052fd0b */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0052fd11 */
    f->r[0]=f->r[3];
    /* 0052fd13 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052fd15 */
    if(mi_signed_less(f->flags))goto mi_0052fd19;
    /* 0052fd17 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052fd19:;
    /* 0052fd19 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fd1c */
    f->r[7]=MI_U32((0x005d2a18u));
    /* 0052fd22 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[0]));
    /* 0052fd29 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fd2b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0052fd2c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fd2e */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 0052fd34 */
    f->r[1]=f->r[0];
    /* 0052fd36 */
    f->r[0]=MI_U32((0x00775910u));
    /* 0052fd3b */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 0052fd3d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fd3f */
    f->r[0]=MI_U32((0x0077590cu));
    /* 0052fd44 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fd46 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fd48 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0052fd4a */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0052fd4e */
    f->r[0]=(0xfffffff6u+f->r[2]+f->r[0]*1u);
    /* 0052fd52 */
    goto mi_0052fd99;
mi_0052fd54:;
    /* 0052fd54 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 0052fd5a */
    f->r[7]=MI_U32((0x00775910u));
    /* 0052fd60 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fd62 */
    f->r[0]=f->r[3];
    /* 0052fd64 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052fd66 */
    f->r[2]=MI_U32((0x0126517cu));
    /* 0052fd6c */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052fd6e */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0052fd70 */
    if(mi_signed_less(f->flags))goto mi_0052fd74;
mi_0052fd72:;
    /* 0052fd72 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052fd74:;
    /* 0052fd74 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fd77 */
    f->r[7]=MI_U32((0x005d2a1cu));
    /* 0052fd7d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 0052fd84 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0052fd85 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fd87 */
    f->r[2]=MI_U32((0x0077590cu));
    /* 0052fd8d */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0052fd8f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fd91 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0052fd95 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052fd97 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0052fd99:;
    /* 0052fd99 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e39088u)))&0xffu)<<0);
    /* 0052fd9f */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052fda1 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052fda7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fda8 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052fdae */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdaf */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0052fdb3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdb4 */
    f->r[2]=MI_U32((0x00f56f84u));
    /* 0052fdba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdbb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdbc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdbd */
    if((f->flags&MI_ZF))goto mi_0052fdc9;
    /* 0052fdbf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fdc1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[6])))&0xffu)<<0);
    /* 0052fdc4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdc5 */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdc7 */
    goto mi_0052fdd4;
mi_0052fdc9:;
    /* 0052fdc9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fdcb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000015u+f->r[6])))&0xffu)<<0);
    /* 0052fdce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fdcf */
    a=0x000000c3u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052fdd4:;
    /* 0052fdd4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fdd9u;f->boundary(0x0052fdd4u,0x004d62f0u,f);
    /* 0052fdd9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052fddb */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052fdde */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000014u+f->r[6])))&0xffu)<<0);
    /* 0052fde1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0052fde2 */
    if((f->flags&MI_ZF))goto mi_0052fdf2;
    /* 0052fde4 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fde7 */
    if((f->flags&MI_ZF))goto mi_0052fdf2;
    /* 0052fde9 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fdec */
    if(!(f->flags&MI_ZF))goto mi_0052fe8c;
mi_0052fdf2:;
    /* 0052fdf2 */
    f->r[7]=MI_U32((0x005d2a18u));
    /* 0052fdf8 */
    f->r[0]=MI_U32((0x00775910u));
    /* 0052fdfd */
    f->r[1]=MI_U32((0x0077590cu));
    /* 0052fe03 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0052fe05 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0052fe09 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0052fe0b */
    f->r[5]=MI_U32((0x005d2a1cu));
    /* 0052fe11 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0052fe13 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0052fe19 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0052fe1b */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052fe1d */
    f->r[0]=f->r[3];
    /* 0052fe1f */
    if(mi_signed_less(f->flags))goto mi_0052fe23;
    /* 0052fe21 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052fe23:;
    /* 0052fe23 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fe26 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[0]));
    /* 0052fe2d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0052fe2e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0052fe30 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0052fe32 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0052fe34 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052fe36 */
    if(mi_signed_less(f->flags))goto mi_0052fe3a;
    /* 0052fe38 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_0052fe3a:;
    /* 0052fe3a */
    a=f->r[3];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 0052fe3d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0052fe3f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[3]));
    /* 0052fe46 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xfffffffeu+f->r[6])))&0xffu)<<0);
    /* 0052fe49 */
    f->r[3]=MI_U32((0x00f56b0cu));
    /* 0052fe4f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fe50 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fe51 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0052fe53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fe58u;f->boundary(0x0052fe53u,0x0052f790u,f);
    /* 0052fe58 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 0052fe5e */
    MI_U8((0xfffffffeu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0052fe61 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0052fe66 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052fe69 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0052fe6b */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0052fe6d */
    f->r[0]=f->r[3];
    /* 0052fe6f */
    if(mi_signed_less(f->flags))goto mi_0052fe73;
    /* 0052fe71 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052fe73:;
    /* 0052fe73 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fe76 */
    f->r[1]=0x00f55bb0u;
    /* 0052fe7b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 0052fe82 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0052fe84 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fe85 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fe86 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052fe87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052fe8cu;f->boundary(0x0052fe87u,0x0053e590u,f);
mi_0052fe8c:;
    /* 0052fe8c */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 0052fe92 */
    f->r[7]=MI_U32((0x005d2a1cu));
mi_0052fe98:;
    /* 0052fe98 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0052fe9c */
    a=f->r[6];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0052fe9f */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0052fea0 */
    a=f->r[6];b=0x007843dcu;t=mi_alu(f,a,b,32,1);
    /* 0052fea6 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0052feaa */
    if(mi_signed_less(f->flags))goto mi_0052f9ca;
mi_0052feb0:;
    /* 0052feb0 */
    a=MI_U8((0x00e38ddau));b=0x00000015u;t=mi_alu(f,a,b,8,1);
    /* 0052feb7 */
    if(!(f->flags&MI_ZF))goto mi_005301c9;
    /* 0052febd */
    f->r[2]=MI_U32((0x0077591cu));
    /* 0052fec3 */
    f->r[0]=MI_U32((0x00784cb4u));
    /* 0052fec8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0052feca */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0052fecd */
    if((f->flags&MI_ZF))goto mi_0052fee6;
    /* 0052fecf */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fed2 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052fed4 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fed6 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0077f75cu+f->r[0]));
    /* 0052fedd */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0077f75eu+f->r[0]));
    /* 0052fee4 */
    goto mi_0052fefb;
mi_0052fee6:;
    /* 0052fee6 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052fee9 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052feeb */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052feed */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0077f74cu+f->r[0]));
    /* 0052fef4 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0077f74eu+f->r[0]));
mi_0052fefb:;
    /* 0052fefb */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f749u+f->r[0])))&0xffu)<<0);
    /* 0052ff01 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052ff03 */
    if((f->flags&MI_ZF))goto mi_0052ff91;
    /* 0052ff09 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0077f748u+f->r[0])))&0xffu)<<0);
    /* 0052ff0f */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0052ff11 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x0077f754u+f->r[0])))&0xffffu)<<0);
    /* 0052ff18 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0052ff1b */
    if(!(f->flags&MI_ZF))goto mi_0052ff23;
    /* 0052ff1d */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ff1f */
    a=0x0000005au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ff21 */
    goto mi_0052ff58;
mi_0052ff23:;
    /* 0052ff23 */
    f->r[0]=MI_U32((0x0126517cu));
    /* 0052ff28 */
    f->r[2]=f->r[6];
    /* 0052ff2a */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0052ff2c */
    if(mi_signed_less(f->flags))goto mi_0052ff30;
    /* 0052ff2e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0052ff30:;
    /* 0052ff30 */
    a=f->r[6];b=MI_U32((0x0126517cu));t=mi_alu(f,a,b,32,1);
    /* 0052ff36 */
    f->r[0]=f->r[6];
    /* 0052ff38 */
    if(mi_signed_less(f->flags))goto mi_0052ff3c;
    /* 0052ff3a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0052ff3c:;
    /* 0052ff3c */
    a=f->r[2];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0052ff3f */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0052ff42 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[2]));
    /* 0052ff49 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[0]));
    /* 0052ff50 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0052ff53 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052ff56 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ff57 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052ff58:;
    /* 0052ff58 */
    a=f->r[7];b=MI_U32((0x0077590cu));t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0052ff5e */
    f->r[2]=(0xfffffffcu+f->r[7]+f->r[5]*1u);
    /* 0052ff62 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ff63 */
    f->r[2]=MI_U32((0x00775910u));
    /* 0052ff69 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052ff6b */
    f->r[0]=(0xfffffffcu+f->r[1]+f->r[3]*1u);
    /* 0052ff6f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052ff70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052ff75u;f->boundary(0x0052ff70u,0x004cd710u,f);
    /* 0052ff75 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 0052ff7a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052ff7d */
    a=(f->r[0]&0xffu);b=0x00000015u;t=mi_alu(f,a,b,8,1);
    /* 0052ff7f */
    if(!(f->flags&MI_ZF))goto mi_005301c9;
    /* 0052ff85 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 0052ff8b */
    f->r[7]=MI_U32((0x005d2a1cu));
mi_0052ff91:;
    /* 0052ff91 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0052ff99 */
    f->r[6]=0x007770e4u;
mi_0052ff9e:;
    /* 0052ff9e */
    a=MI_U8((0x0000000du+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0052ffa2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0052ffa8;
    /* 0052ffa4 */
    MI_U8((0x0000000eu+f->r[6]))=(uint8_t)(0x00000000u);
mi_0052ffa8:;
    /* 0052ffa8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000eu+f->r[6])))&0xffu)<<0);
    /* 0052ffab */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0052ffad */
    if((f->flags&MI_ZF))goto mi_005301a9;
    /* 0052ffb3 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0052ffb7 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 0052ffbc */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0052ffbe */
    if(!(f->flags&MI_ZF))goto mi_00530136;
    /* 0052ffc4 */
    f->r[0]=MI_U32((0x00775928u));
    /* 0052ffc9 */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 0052ffcc */
    if((f->flags&MI_ZF))goto mi_0052ffd7;
    /* 0052ffce */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0052ffd1 */
    if(!(f->flags&MI_ZF))goto mi_00530136;
mi_0052ffd7:;
    /* 0052ffd7 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 0052ffd9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0052ffdb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00530099;
    /* 0052ffe1 */
    f->r[2]=MI_U32((0xfffffed0u+f->r[6]));
    /* 0052ffe7 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0052ffe9 */
    if((f->flags&MI_ZF))goto mi_00530099;
    /* 0052ffef */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0052fff1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0052fff3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00530042;
    /* 0052fff5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_0052fff7:;
    /* 0052fff7 */
    f->r[0]=MI_U32((0xfffffed0u+f->r[6]));
    /* 0052fffd */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0052ffff */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[0]));
    /* 00530003 */
    a=f->r[2];b=MI_U32((0x0077590cu));t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00530009 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 0053000c */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0053000e */
    f->r[7]=MI_U32((0x00775910u));
    /* 00530014 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00530016 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530017 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00530019 */
    f->r[1]=MI_U32((0x00f568c4u));
    /* 0053001f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00530020 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530021 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530022 */
    f->r[1]=0x00f55bb0u;
    /* 00530027 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053002cu;f->boundary(0x00530027u,0x0053e590u,f);
    /* 0053002c */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 0053002e */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 00530034 */
    f->r[7]=MI_U32((0x005d2a1cu));
    /* 0053003a */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0053003b */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0053003e */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00530040 */
    if(mi_signed_less(f->flags))goto mi_0052fff7;
mi_00530042:;
    /* 00530042 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000du+f->r[6])))&0xffu)<<0);
    /* 00530045 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00530047 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00530049 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00530099;
    /* 0053004b */
    f->r[3]=(0xfffffed4u+f->r[6]);
mi_00530051:;
    /* 00530051 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[3]));
    /* 00530055 */
    a=f->r[2];b=MI_U32((0x0077590cu));t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0053005b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[3]));
    /* 0053005e */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00530060 */
    f->r[7]=MI_U32((0x00775910u));
    /* 00530066 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00530068 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530069 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053006b */
    f->r[1]=MI_U32((0x00f568c4u));
    /* 00530071 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00530074 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530075 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530076 */
    f->r[1]=0x00f55bb0u;
    /* 0053007b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00530080u;f->boundary(0x0053007bu,0x0053e590u,f);
    /* 00530080 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 00530086 */
    f->r[7]=MI_U32((0x005d2a1cu));
    /* 0053008c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0053008e */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0053008f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0000000du+f->r[6])))&0xffu)<<0);
    /* 00530092 */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00530095 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00530097 */
    if(mi_signed_less(f->flags))goto mi_00530051;
mi_00530099:;
    /* 00530099 */
    f->r[0]=MI_U32((0x00f53fe0u));
    /* 0053009e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053009f */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 005300a2 */
    MI_U32((0x00f53fe0u))=(uint32_t)(f->r[0]);
    /* 005300a7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005300b0;
    /* 005300a9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005300ab */
    MI_U32((0x00f53fe0u))=(uint32_t)(f->r[0]);
mi_005300b0:;
    /* 005300b0 */
    f->r[3]=MI_U32((0x00000000u+f->r[6]));
    /* 005300b2 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 005300b5 */
    if(!mi_signed_less(f->flags))goto mi_005300bd;
    /* 005300b7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005300b9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 005300bb */
    goto mi_00530113;
mi_005300bd:;
    /* 005300bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005300bf */
    if(!mi_signed_less(f->flags))goto mi_005300c5;
    /* 005300c1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005300c3 */
    goto mi_005300cf;
mi_005300c5:;
    /* 005300c5 */
    a=f->r[0];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 005300c8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005300cf;
    /* 005300ca */
    f->r[0]=0x00000064u;
mi_005300cf:;
    /* 005300cf */
    f->r[2]=f->r[3];
    /* 005300d1 */
    a=f->r[2];b=f->r[0];t=mi_imul(f,a,b);f->r[2]=t;
    /* 005300d4 */
    f->r[0]=0x51eb851fu;
    /* 005300d9 */
    a=f->r[0];b=f->r[2];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 005300db */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 005300de */
    f->r[0]=f->r[2];
    /* 005300e0 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 005300e3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005300e5 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 005300e7 */
    if(mi_signed_less(f->flags))goto mi_005300ec;
    /* 005300e9 */
    f->r[2]=(0xffffffffu+f->r[3]);
mi_005300ec:;
    /* 005300ec */
    f->r[0]=MI_U32((0xfffffed0u+f->r[6]));
    /* 005300f2 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 005300f5 */
    f->r[3]=MI_U32((0x00775910u));
    /* 005300fb */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 005300fe */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 00530101 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00530103 */
    f->r[3]=MI_U32((0x0077590cu));
    /* 00530109 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053010b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 0053010f */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00530111 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_00530113:;
    /* 00530113 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0000000cu+f->r[6])))&0xffu)<<0);
    /* 00530116 */
    a=(f->r[2]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00530119 */
    f->r[2]=MI_U32((0x00f569b4u));
    /* 0053011f */
    if((f->flags&MI_ZF))goto mi_00530127;
    /* 00530121 */
    f->r[2]=MI_U32((0x00f569acu));
mi_00530127:;
    /* 00530127 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530128 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530129 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053012a */
    f->r[1]=0x00f55bb0u;
    /* 0053012f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00530134u;f->boundary(0x0053012fu,0x0053e590u,f);
    /* 00530134 */
    goto mi_0053019d;
mi_00530136:;
    /* 00530136 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 00530138 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053013a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005301a9;
    /* 0053013c */
    f->r[2]=MI_U32((0xfffffed0u+f->r[6]));
    /* 00530142 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00530144 */
    if((f->flags&MI_ZF))goto mi_005301a9;
    /* 00530146 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00530148 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053014a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005301a9;
    /* 0053014c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0053014e */
    goto mi_0053015c;
mi_00530150:;
    /* 00530150 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 00530156 */
    f->r[7]=MI_U32((0x005d2a1cu));
mi_0053015c:;
    /* 0053015c */
    f->r[2]=MI_U32((0xfffffed0u+f->r[6]));
    /* 00530162 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[5]*1u);
    /* 00530165 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]+f->r[5]*1u));
    /* 0053016a */
    a=f->r[2];b=MI_U32((0x0077590cu));t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00530170 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 00530173 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00530175 */
    f->r[7]=MI_U32((0x00775910u));
    /* 0053017b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0053017d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053017e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00530180 */
    f->r[1]=MI_U32((0x00f568c4u));
    /* 00530186 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00530187 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530188 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530189 */
    f->r[1]=0x00f55bb0u;
    /* 0053018e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00530193u;f->boundary(0x0053018eu,0x0053e590u,f);
    /* 00530193 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 00530195 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00530196 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00530199 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0053019b */
    if(mi_signed_less(f->flags))goto mi_00530150;
mi_0053019d:;
    /* 0053019d */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 005301a3 */
    f->r[7]=MI_U32((0x005d2a1cu));
mi_005301a9:;
    /* 005301a9 */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 005301ad */
    a=f->r[6];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 005301b3 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 005301b4 */
    a=f->r[6];b=0x0077b02cu;t=mi_alu(f,a,b,32,1);
    /* 005301ba */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 005301be */
    if(mi_signed_less(f->flags))goto mi_0052ff9e;
    /* 005301c4 */
    goto mi_00530265;
mi_005301c9:;
    /* 005301c9 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 005301cb */
    f->r[6]=0x007770f2u;
mi_005301d0:;
    /* 005301d0 */
    a=MI_U8((0xffffffffu+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 005301d4 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_005301d9;
    /* 005301d6 */
    MI_U8((0x00000000u+f->r[6]))=(uint8_t)(0x00000000u);
mi_005301d9:;
    /* 005301d9 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 005301dc */
    if((f->flags&MI_ZF))goto mi_00530252;
    /* 005301de */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005301df */
    f->r[4]-=4;MI_U32(f->r[4])=0x005301e4u;f->boundary(0x005301dfu,0x00444980u,f);
    /* 005301e4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005301e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005301e9 */
    if((f->flags&MI_ZF))goto mi_00530252;
    /* 005301eb */
    f->r[0]=MI_U32((0xfffffff2u+f->r[6]));
    /* 005301ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005301f0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00530252;
    /* 005301f2 */
    f->r[1]=MI_U32((0xfffffec2u+f->r[6]));
    /* 005301f8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005301fa */
    if((f->flags&MI_ZF))goto mi_00530252;
    /* 005301fc */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 005301fe */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00530200 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00530252;
    /* 00530202 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_00530204:;
    /* 00530204 */
    f->r[2]=MI_U32((0xfffffec2u+f->r[6]));
    /* 0053020a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[3]+f->r[2]*1u));
    /* 0053020f */
    f->r[0]=(0x00000000u+f->r[3]+f->r[2]*1u);
    /* 00530212 */
    f->r[2]=MI_U32((0x0077590cu));
    /* 00530218 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0053021a */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 00530220 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00530222 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 00530225 */
    f->r[0]=MI_U32((0x00775910u));
    /* 0053022a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053022b */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 00530231 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00530233 */
    f->r[0]=MI_U32((0x00f568c4u));
    /* 00530238 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0053023a */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053023b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053023c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053023d */
    f->r[1]=0x00f55bb0u;
    /* 00530242 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00530247u;f->boundary(0x00530242u,0x0053e590u,f);
    /* 00530247 */
    f->r[0]=MI_U32((0xfffffff2u+f->r[6]));
    /* 0053024a */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0053024b */
    a=f->r[3];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0053024e */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00530250 */
    if(mi_signed_less(f->flags))goto mi_00530204;
mi_00530252:;
    /* 00530252 */
    a=f->r[6];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00530258 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00530259 */
    a=f->r[6];b=0x0077b03au;t=mi_alu(f,a,b,32,1);
    /* 0053025f */
    if(mi_signed_less(f->flags))goto mi_005301d0;
mi_00530265:;
    /* 00530265 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530267 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530269 */
    f->r[1]=0x00785d70u;
    /* 0053026e */
    MI_U32((0x00f53fdcu))=(uint32_t)(0x00000000u);
    /* 00530278 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053027du;f->boundary(0x00530278u,0x0044bb90u,f);
    /* 0053027d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053027f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00530283 */
    if((f->flags&MI_ZF))goto mi_0053042e;
    /* 00530289 */
    goto mi_0053028f;
mi_0053028b:;
    /* 0053028b */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
mi_0053028f:;
    /* 0053028f */
    f->r[1]=f->r[0];
    /* 00530291 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00530296u;f->boundary(0x00530291u,0x0044d700u,f);
    /* 00530296 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00530298 */
    if((f->flags&MI_ZF))goto mi_00530411;
    /* 0053029e */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 005302a2 */
    f->r[1]=f->r[7];
    /* 005302a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005302a9u;f->boundary(0x005302a4u,0x0044d750u,f);
    /* 005302a9 */
    f->r[6]=f->r[0];
    /* 005302ab */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000058u+f->r[7]));
    /* 005302af */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005302b1 */
    if(mi_signed_less(f->flags))goto mi_00530411;
    /* 005302b7 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 005302ba */
    if(!mi_signed_less(f->flags))goto mi_00530411;
    /* 005302c0 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 005302c3 */
    f->r[5]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 005302c6 */
    a=f->r[5];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 005302c9 */
    f->r[7]=(0x00776fb0u+f->r[5]);
    /* 005302cf */
    f->r[1]=f->r[7];
    /* 005302d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005302d6u;f->boundary(0x005302d1u,0x004477d0u,f);
    /* 005302d6 */
    f->r[1]=f->r[0];
    /* 005302d8 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 005302da */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00530411;
    /* 005302e0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005302e2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00530411;
    /* 005302e8 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 005302eb */
    f->r[0]=(0x00000000u+f->r[6]+f->r[2]*8u);
    /* 005302ee */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005302f1 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 005302f2 */
    a=f->r[1];mi_idiv32(f,a);
    /* 005302f4 */
    f->r[1]=0x00000064u;
    /* 005302f9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 005302fb */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 005302fe */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00530302;
    /* 00530300 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00530302:;
    /* 00530302 */
    f->r[0]=MI_U32((0x00f53fd8u));
    /* 00530307 */
    f->r[6]=MI_U32((0x00000134u+f->r[7]));
    /* 0053030d */
    a=f->r[0];b=0xffffffceu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00530310 */
    f->r[3]=0x00000000u;
    /* 00530315 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00530316 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 00530319 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053031b */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0053031e */
    t=((f->flags&MI_SF))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00530321 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00530322 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 00530324 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00530327 */
    f->r[0]=f->r[1];
    /* 00530329 */
    if(!mi_signed_less(f->flags))goto mi_00530331;
    /* 0053032b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0053032d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0053032f */
    goto mi_00530392;
mi_00530331:;
    /* 00530331 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00530333 */
    if(!mi_signed_less(f->flags))goto mi_00530339;
    /* 00530335 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00530337 */
    goto mi_00530343;
mi_00530339:;
    /* 00530339 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0053033c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00530343;
    /* 0053033e */
    f->r[0]=0x00000064u;
mi_00530343:;
    /* 00530343 */
    f->r[1]=f->r[6];
    /* 00530345 */
    a=f->r[1];b=f->r[0];t=mi_imul(f,a,b);f->r[1]=t;
    /* 00530348 */
    f->r[0]=0x51eb851fu;
    /* 0053034d */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 0053034f */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00530352 */
    f->r[1]=f->r[2];
    /* 00530354 */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 00530357 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00530359 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0053035b */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0053035d */
    if(mi_signed_less(f->flags))goto mi_00530362;
    /* 0053035f */
    f->r[2]=(0xffffffffu+f->r[6]);
mi_00530362:;
    /* 00530362 */
    f->r[0]=MI_U32((0x00000004u+f->r[7]));
    /* 00530365 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00530368 */
    f->r[7]=MI_U32((0x00775910u));
    /* 0053036e */
    f->r[1]=MI_U32((0x0077590cu));
    /* 00530374 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]+f->r[2]*2u));
    /* 00530378 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0053037b */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 00530381 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00530383 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[0]));
    /* 00530387 */
    f->r[0]=MI_U32((0x005d2a1cu));
    /* 0053038c */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0053038e */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00530390 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_00530392:;
    /* 00530392 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f0u+f->r[5])))&0xffu)<<0);
    /* 00530398 */
    f->r[3]=MI_U32((0x00f569b4u));
    /* 0053039e */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 005303a0 */
    if((f->flags&MI_ZF))goto mi_005303a8;
    /* 005303a2 */
    f->r[3]=MI_U32((0x00f569acu));
mi_005303a8:;
    /* 005303a8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005303a9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005303aa */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005303ab */
    f->r[1]=0x00f55bb0u;
    /* 005303b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005303b5u;f->boundary(0x005303b0u,0x0053e590u,f);
    /* 005303b5 */
    f->r[1]=MI_U32((0x0126517cu));
    /* 005303bb */
    f->r[0]=f->r[3];
    /* 005303bd */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 005303bf */
    if(mi_signed_less(f->flags))goto mi_005303c3;
    /* 005303c1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_005303c3:;
    /* 005303c3 */
    f->r[1]=MI_U32((0x00f53fdcu));
    /* 005303c9 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005303cc */
    a=f->r[0];b=0x010d69f8u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005303d1 */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 005303d4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000014u+f->r[0]));
    /* 005303d8 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000016u+f->r[0]));
    /* 005303dc */
    if(!mi_signed_less(f->flags))goto mi_00530411;
    /* 005303de */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 005303e2 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 005303e5 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005303e8 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 005303ec */
    MI_U32((0x00f535d0u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 005303f2 */
    MI_U32((0x00f535d4u+f->r[0]))=(uint32_t)(f->r[6]);
    /* 005303f8 */
    MI_U32((0x00f535d8u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 005303fe */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 005303ff */
    MI_U32((0x00f535dcu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00530405 */
    MI_U32((0x00f535e0u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0053040b */
    MI_U32((0x00f53fdcu))=(uint32_t)(f->r[1]);
mi_00530411:;
    /* 00530411 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00530415 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530417 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530418 */
    f->r[1]=0x00785d70u;
    /* 0053041d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00530422u;f->boundary(0x0053041du,0x0044bb90u,f);
    /* 00530422 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00530424 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00530428 */
    if(!(f->flags&MI_ZF))goto mi_0053028b;
mi_0053042e:;
    /* 0053042e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530430 */
    f->r[1]=0x00785d70u;
    /* 00530435 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053043au;f->boundary(0x00530435u,0x0044baf0u,f);
    /* 0053043a */
    f->r[5]=f->r[0];
    /* 0053043c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0053043e */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00530442 */
    if((f->flags&MI_ZF))goto mi_005305a3;
mi_00530448:;
    /* 00530448 */
    a=MI_U16((0x00000004u+f->r[5]));b=0x00000025u;t=mi_alu(f,a,b,16,1);
    /* 0053044d */
    if(!(f->flags&MI_ZF))goto mi_0053058a;
    /* 00530453 */
    f->r[2]=(0x00000018u+f->r[4]);
    /* 00530457 */
    f->r[0]=(0x00000013u+f->r[4]);
    /* 0053045b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053045c */
    f->r[1]=(0x00000020u+f->r[4]);
    /* 00530460 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530461 */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 00530465 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530466 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530467 */
    f->r[1]=f->r[5];
    /* 00530469 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053046eu;f->boundary(0x00530469u,0x00449750u,f);
    /* 0053046e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00530470 */
    if((f->flags&MI_ZF))goto mi_0053058a;
    /* 00530476 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0053047a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053047c */
    if(mi_signed_less(f->flags))goto mi_0053058a;
    /* 00530482 */
    a=f->r[0];b=0x00000032u;t=mi_alu(f,a,b,32,1);
    /* 00530485 */
    if(!mi_signed_less(f->flags))goto mi_0053058a;
    /* 0053048b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0053048e */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00530492 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00530495 */
    f->r[6]=MI_U32((0x007770e4u+f->r[0]*4u));
    /* 0053049c */
    f->r[7]=(0x00776fb0u+f->r[0]*4u);
    /* 005304a3 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 005304a6 */
    if(!mi_signed_less(f->flags))goto mi_005304ae;
    /* 005304a8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 005304aa */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 005304ac */
    goto mi_0053050d;
mi_005304ae:;
    /* 005304ae */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005304b0 */
    if(!mi_signed_less(f->flags))goto mi_005304b6;
    /* 005304b2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 005304b4 */
    goto mi_005304c0;
mi_005304b6:;
    /* 005304b6 */
    a=f->r[2];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 005304b9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005304c0;
    /* 005304bb */
    f->r[2]=0x00000064u;
mi_005304c0:;
    /* 005304c0 */
    f->r[1]=f->r[6];
    /* 005304c2 */
    f->r[0]=0x51eb851fu;
    /* 005304c7 */
    a=f->r[1];b=f->r[2];t=mi_imul(f,a,b);f->r[1]=t;
    /* 005304ca */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 005304cc */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 005304cf */
    f->r[1]=f->r[2];
    /* 005304d1 */
    a=f->r[1];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[1]=t;
    /* 005304d4 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005304d6 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 005304d8 */
    if(mi_signed_less(f->flags))goto mi_005304dd;
    /* 005304da */
    f->r[2]=(0xffffffffu+f->r[6]);
mi_005304dd:;
    /* 005304dd */
    f->r[0]=MI_U32((0x00000004u+f->r[7]));
    /* 005304e0 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 005304e3 */
    f->r[7]=MI_U32((0x00775910u));
    /* 005304e9 */
    f->r[1]=MI_U32((0x0077590cu));
    /* 005304ef */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]+f->r[2]*2u));
    /* 005304f3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 005304f6 */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 005304fc */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 005304fe */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[0]));
    /* 00530502 */
    f->r[0]=MI_U32((0x005d2a1cu));
    /* 00530507 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00530509 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0053050b */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_0053050d:;
    /* 0053050d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000013u+f->r[4])))&0xffu)<<0);
    /* 00530511 */
    f->r[3]=MI_U32((0x00f569acu));
    /* 00530517 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00530519 */
    if(!(f->flags&MI_ZF))goto mi_00530521;
    /* 0053051b */
    f->r[3]=MI_U32((0x00f569b4u));
mi_00530521:;
    /* 00530521 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530522 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530523 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00530524 */
    f->r[1]=0x00f55bb0u;
    /* 00530529 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0053052eu;f->boundary(0x00530529u,0x0053e590u,f);
    /* 0053052e */
    f->r[1]=MI_U32((0x0126517cu));
    /* 00530534 */
    f->r[0]=f->r[3];
    /* 00530536 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00530538 */
    if(mi_signed_less(f->flags))goto mi_0053053c;
    /* 0053053a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0053053c:;
    /* 0053053c */
    f->r[1]=MI_U32((0x00f53fdcu));
    /* 00530542 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00530545 */
    a=f->r[0];b=0x010d69f8u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053054a */
    a=f->r[1];b=0x00000064u;t=mi_alu(f,a,b,32,1);
    /* 0053054d */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000014u+f->r[0]));
    /* 00530551 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000016u+f->r[0]));
    /* 00530555 */
    if(!mi_signed_less(f->flags))goto mi_0053058a;
    /* 00530557 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 0053055b */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0053055e */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00530561 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00530562 */
    MI_U32((0x00f535d0u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 00530568 */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 0053056c */
    MI_U32((0x00f535d4u+f->r[0]))=(uint32_t)(f->r[6]);
    /* 00530572 */
    MI_U32((0x00f535d8u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 00530578 */
    MI_U32((0x00f535dcu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0053057e */
    MI_U32((0x00f535e0u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00530584 */
    MI_U32((0x00f53fdcu))=(uint32_t)(f->r[1]);
mi_0053058a:;
    /* 0053058a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053058b */
    f->r[1]=0x00785d70u;
    /* 00530590 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00530595u;f->boundary(0x00530590u,0x0044baf0u,f);
    /* 00530595 */
    f->r[5]=f->r[0];
    /* 00530597 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00530599 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0053059d */
    if(!(f->flags&MI_ZF))goto mi_00530448;
mi_005305a3:;
    /* 005305a3 */
    f->r[1]=MI_U32((0x00e38db0u));
    /* 005305a9 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 005305af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005305b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005305b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005305b2 */
    MI_U32((0x00d3b55cu))=(uint32_t)(0x00000000u);
    /* 005305bc */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[1]);
    /* 005305c2 */
    MI_U32((0x00d3b554u))=(uint32_t)(0x00000000u);
    /* 005305cc */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[2]);
    /* 005305d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005305d3 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005305d6 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_00520000(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00520000 */
    f->r[2]=MI_U32((0x00f53f78u));
    /* 00520006 */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 0052000b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0052000e */
    f->r[1]=0x00000001u;
    /* 00520013 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00520015 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520016 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520017 */
    f->r[3]=f->r[0];
    /* 00520019 */
    if(!(f->flags&MI_ZF))goto mi_00520021;
    /* 0052001b */
    f->r[3]=MI_U32((0x00f53f60u));
mi_00520021:;
    /* 00520021 */
    a=MI_U32((0x00605a3cu));b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00520027 */
    if(!(f->flags&MI_ZF))goto mi_0052002b;
    /* 00520029 */
    f->r[3]=f->r[0];
mi_0052002b:;
    /* 0052002b */
    f->r[1]=MI_U32((0x007056f0u));
    /* 00520031 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 00520034 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00520036 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00520038 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005208ad;
    /* 0052003e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052003f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520040 */
    goto mi_00520046;
mi_00520042:;
    /* 00520042 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
mi_00520046:;
    /* 00520046 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520047 */
    f->r[0]=MI_U32((0x007056ecu));
    /* 0052004c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052004d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520052u;f->boundary(0x0052004du,0x00403f10u,f);
    /* 00520052 */
    f->r[1]=(0x00000001u+f->r[6]);
    /* 00520055 */
    f->r[2]=f->r[0];
    /* 00520057 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0052005b */
    f->r[1]=MI_U32((0x007056c8u));
    /* 00520061 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 00520065 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00520068 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0052006a */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0052006c */
    f->r[7]=f->r[1];
    /* 0052006e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00520072 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00520075 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 00520077 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00520078 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[7]=t;
    /* 0052007a */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,2);f->r[7]=t;
    /* 0052007c */
    a=f->r[7];b=0xffff4439u;t=mi_alu(f,a,b,32,3);f->r[7]=t;
    /* 00520082 */
    a=f->r[7];b=0x0000fac1u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00520088 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0052008a */
    f->r[5]=MI_U32((0x00f56f94u));
    /* 00520090 */
    if((f->flags&MI_ZF))goto mi_00520098;
    /* 00520092 */
    f->r[5]=MI_U32((0x00f56f8cu));
mi_00520098:;
    /* 00520098 */
    f->r[1]=MI_U32((0x007056ecu));
    /* 0052009e */
    a=f->r[1];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 005200a1 */
    if(!(f->flags&MI_ZF))goto mi_00520128;
    /* 005200a7 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 005200ad */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 005200b0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200b1 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 005200b7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200b8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200b9 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 005200be */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200c0 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 005200c3 */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 005200c9 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200cb */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 005200cf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200d0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200d1 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005200d8u;f->boundary(0x005200d3u,0x004d6480u,f);
    /* 005200d8 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 005200dd */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 005200e3 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 005200e9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200ea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200eb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200ec */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200ed */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 005200f1 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200f6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005200f7 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 005200fd */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 00520101 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520102 */
    f->r[0]=MI_U32((0x007056ecu));
    /* 00520107 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052010a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0052010d */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 00520110 */
    f->r[1]=MI_U32((0x005ce3bcu+f->r[0]*4u));
    /* 00520117 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00520118 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520119 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052011b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520120u;f->boundary(0x0052011bu,0x004d6250u,f);
    /* 00520120 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00520123 */
    goto mi_0052089a;
mi_00520128:;
    /* 00520128 */
    a=f->r[1];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0052012b */
    if(!(f->flags&MI_ZF))goto mi_005201b4;
    /* 00520131 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520137 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 0052013a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052013b */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 00520141 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520142 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520143 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 00520148 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052014a */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0052014d */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 00520153 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520155 */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 00520159 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052015a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052015b */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052015d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520162u;f->boundary(0x0052015du,0x004d6480u,f);
    /* 00520162 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 00520167 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052016d */
    f->r[2]=MI_U32((0x007056e8u));
    /* 00520173 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520174 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520175 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520176 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520177 */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 0052017b */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520180 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520181 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 00520187 */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 0052018b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052018c */
    f->r[0]=MI_U32((0x007056ecu));
    /* 00520191 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00520194 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00520197 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 0052019a */
    f->r[1]=MI_U32((0x005ce3bcu+f->r[0]*4u));
    /* 005201a1 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005201a4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201a5 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201a7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005201acu;f->boundary(0x005201a7u,0x004d6250u,f);
    /* 005201ac */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005201af */
    goto mi_0052089a;
mi_005201b4:;
    /* 005201b4 */
    a=f->r[1];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 005201b7 */
    if(!(f->flags&MI_ZF))goto mi_00520256;
    /* 005201bd */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 005201c3 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 005201c6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201c7 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 005201cd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201ce */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201cf */
    f->r[0]=MI_U32((0x007056e8u));
    /* 005201d4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201d6 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 005201d9 */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 005201df */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201e1 */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 005201e5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201e6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201e7 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005201e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005201eeu;f->boundary(0x005201e9u,0x004d6480u,f);
    /* 005201ee */
    f->r[0]=MI_U32((0x007056ecu));
    /* 005201f3 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 005201f7 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005201fa */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005201fd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00520200 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00520203 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 00520209 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052020a */
    f->r[1]=MI_U32((0x007056e8u));
    /* 00520210 */
    f->r[0]=MI_U32((0x005ce3bcu+f->r[2]*4u));
    /* 00520217 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052021d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052021e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052021f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520220 */
    f->r[2]=(0x00000071u+f->r[6]+f->r[1]*1u);
    /* 00520224 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520229 */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 0052022f */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00520232 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520233 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520234 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00520246;
    /* 00520236 */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00520239 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052023a */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052023c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520241u;f->boundary(0x0052023cu,0x004d6250u,f);
    /* 00520241 */
    goto mi_00520897;
mi_00520246:;
    /* 00520246 */
    a=f->r[0];b=0x00000027u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00520249 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052024a */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052024c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520251u;f->boundary(0x0052024cu,0x004d6250u,f);
    /* 00520251 */
    goto mi_00520897;
mi_00520256:;
    /* 00520256 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00520259 */
    if(!(f->flags&MI_ZF))goto mi_005202e2;
    /* 0052025f */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520265 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 00520268 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520269 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052026f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520270 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520271 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 00520276 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520278 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0052027b */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 00520281 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520283 */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 00520287 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520288 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520289 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052028b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520290u;f->boundary(0x0052028bu,0x004d6480u,f);
    /* 00520290 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 00520295 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052029b */
    f->r[2]=MI_U32((0x007056e8u));
    /* 005202a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202a2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202a3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202a4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202a5 */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 005202a9 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202af */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 005202b5 */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 005202b9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202ba */
    f->r[0]=MI_U32((0x007056ecu));
    /* 005202bf */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005202c2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 005202c5 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 005202c8 */
    f->r[1]=MI_U32((0x005ce3bcu+f->r[0]*4u));
    /* 005202cf */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005202d2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202d3 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202d5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005202dau;f->boundary(0x005202d5u,0x004d6250u,f);
    /* 005202da */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005202dd */
    goto mi_0052089a;
mi_005202e2:;
    /* 005202e2 */
    a=f->r[1];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 005202e5 */
    if(!(f->flags&MI_ZF))goto mi_0052036e;
    /* 005202eb */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 005202f1 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 005202f4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202f5 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 005202fb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202fc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005202fd */
    f->r[0]=MI_U32((0x007056e8u));
    /* 00520302 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520304 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00520307 */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 0052030d */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052030f */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 00520313 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520314 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520315 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520317 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052031cu;f->boundary(0x00520317u,0x004d6480u,f);
    /* 0052031c */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 00520321 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 00520327 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 0052032d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052032e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052032f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520330 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520331 */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 00520335 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052033a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052033b */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 00520341 */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 00520345 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520346 */
    f->r[0]=MI_U32((0x007056ecu));
    /* 0052034b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0052034e */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00520351 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 00520354 */
    f->r[1]=MI_U32((0x005ce3bcu+f->r[0]*4u));
    /* 0052035b */
    a=f->r[1];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052035e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052035f */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520361 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520366u;f->boundary(0x00520361u,0x004d6250u,f);
    /* 00520366 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00520369 */
    goto mi_0052089a;
mi_0052036e:;
    /* 0052036e */
    a=f->r[1];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 00520371 */
    if(!(f->flags&MI_ZF))goto mi_005203fa;
    /* 00520377 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 0052037d */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 00520380 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520381 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 00520387 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520388 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520389 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 0052038e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520390 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00520393 */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 00520399 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052039b */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 0052039f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203a0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203a1 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005203a8u;f->boundary(0x005203a3u,0x004d6480u,f);
    /* 005203a8 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 005203ad */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 005203b3 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 005203b9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203ba */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203bb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203bc */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203bd */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 005203c1 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203c6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203c7 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 005203cd */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 005203d1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203d2 */
    f->r[0]=MI_U32((0x007056ecu));
    /* 005203d7 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 005203da */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 005203dd */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 005203e0 */
    f->r[1]=MI_U32((0x005ce3bcu+f->r[0]*4u));
    /* 005203e7 */
    a=f->r[1];b=0x00000013u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005203ea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203eb */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005203ed */
    f->r[4]-=4;MI_U32(f->r[4])=0x005203f2u;f->boundary(0x005203edu,0x004d6250u,f);
    /* 005203f2 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005203f5 */
    goto mi_0052089a;
mi_005203fa:;
    /* 005203fa */
    a=f->r[1];b=0x0000001au;t=mi_alu(f,a,b,32,1);
    /* 005203fd */
    if(!(f->flags&MI_ZF))goto mi_00520486;
    /* 00520403 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520409 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 0052040c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052040d */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 00520413 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520414 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520415 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 0052041a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052041c */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0052041f */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 00520425 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520427 */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 0052042b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052042c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052042d */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052042f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520434u;f->boundary(0x0052042fu,0x004d6480u,f);
    /* 00520434 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 00520439 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 0052043f */
    f->r[2]=MI_U32((0x007056e8u));
    /* 00520445 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520446 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520447 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520448 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520449 */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 0052044d */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520452 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520453 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 00520459 */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 0052045d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052045e */
    f->r[0]=MI_U32((0x007056ecu));
    /* 00520463 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00520466 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00520469 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 0052046c */
    f->r[1]=MI_U32((0x005ce3bcu+f->r[0]*4u));
    /* 00520473 */
    a=f->r[1];b=0x00000016u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00520476 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520477 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520479 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052047eu;f->boundary(0x00520479u,0x004d6250u,f);
    /* 0052047e */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00520481 */
    goto mi_0052089a;
mi_00520486:;
    /* 00520486 */
    a=f->r[1];b=0x00000025u;t=mi_alu(f,a,b,32,1);
    /* 00520489 */
    if(!(f->flags&MI_ZF))goto mi_0052059d;
    /* 0052048f */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520495 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 00520498 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520499 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052049f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204a0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204a1 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 005204a6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204a8 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 005204ab */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 005204af */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204b1 */
    f->r[1]=(0x0000006eu+f->r[1]+f->r[0]*1u);
    /* 005204b5 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 005204bb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204bc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204bd */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x005204c4u;f->boundary(0x005204bfu,0x004d6480u,f);
    /* 005204c4 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005204c7 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 005204c9 */
    if((f->flags&MI_ZF))goto mi_00520511;
    /* 005204cb */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 005204d1 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 005204d6 */
    f->r[1]=MI_U32((0x007056e8u));
    /* 005204dc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204dd */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 005204e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204e2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204e3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204e4 */
    f->r[0]=(0x00000071u+f->r[2]+f->r[1]*1u);
    /* 005204e8 */
    f->r[1]=MI_U32((0x00784cb0u));
    /* 005204ee */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204f3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005204f4 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 005204fa */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 00520500 */
    a=f->r[1];b=0x00000066u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00520503 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520504 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520505 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520507 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052050cu;f->boundary(0x00520507u,0x004d6250u,f);
    /* 0052050c */
    goto mi_00520897;
mi_00520511:;
    /* 00520511 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00784cb0u)))&0xffu)<<0);
    /* 00520516 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00520518 */
    if(!(f->flags&MI_ZF))goto mi_0052054d;
    /* 0052051a */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520520 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 00520525 */
    f->r[1]=MI_U32((0x007056e8u));
    /* 0052052b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052052c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052052d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052052e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052052f */
    a=f->r[1];b=0x00000071u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00520532 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520537 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 0052053d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052053e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052053f */
    a=0x00000053u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520541 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520543 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520548u;f->boundary(0x00520543u,0x004d6250u,f);
    /* 00520548 */
    goto mi_00520897;
mi_0052054d:;
    /* 0052054d */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520553 */
    f->r[1]=MI_U32((0x007056e8u));
    /* 00520559 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0052055b */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 00520560 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520561 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520562 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520563 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520564 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520569 */
    if(!(f->flags&MI_ZF))goto mi_00520584;
    /* 0052056b */
    a=f->r[1];b=0x00000071u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0052056e */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 00520574 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520575 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520576 */
    a=0x00000052u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520578 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052057a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052057fu;f->boundary(0x0052057au,0x004d6250u,f);
    /* 0052057f */
    goto mi_00520897;
mi_00520584:;
    /* 00520584 */
    a=f->r[1];b=0x00000071u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00520587 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 0052058d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052058e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052058f */
    a=0x00000054u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520591 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520593 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520598u;f->boundary(0x00520593u,0x004d6250u,f);
    /* 00520598 */
    goto mi_00520897;
mi_0052059d:;
    /* 0052059d */
    a=f->r[1];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 005205a0 */
    if(!(f->flags&MI_ZF))goto mi_00520629;
    /* 005205a6 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 005205ac */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 005205af */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205b0 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 005205b6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205b7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205b8 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 005205bd */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205bf */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 005205c2 */
    f->r[1]=(0xffffff56u+f->r[3]);
    /* 005205c8 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205ca */
    f->r[2]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 005205ce */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205cf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205d0 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205d2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005205d7u;f->boundary(0x005205d2u,0x004d6480u,f);
    /* 005205d7 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 005205dc */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 005205e2 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 005205e8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205e9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205ea */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205eb */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205ec */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 005205f0 */
    a=0x000000a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205f5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005205f6 */
    f->r[0]=(0xffffff56u+f->r[3]);
    /* 005205fc */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 00520600 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520601 */
    f->r[0]=MI_U32((0x007056ecu));
    /* 00520606 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00520609 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0052060c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[1]*2u);
    /* 0052060f */
    f->r[1]=MI_U32((0x005ce3bcu+f->r[0]*4u));
    /* 00520616 */
    a=f->r[1];b=0x00000022u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00520619 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052061a */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052061c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520621u;f->boundary(0x0052061cu,0x004d6250u,f);
    /* 00520621 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00520624 */
    goto mi_0052089a;
mi_00520629:;
    /* 00520629 */
    a=f->r[1];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 0052062c */
    if(!(f->flags&MI_ZF))goto mi_00520733;
    /* 00520632 */
    f->r[1]=MI_U32((0x005cf0dcu+f->r[2]*4u));
    /* 00520639 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0052063c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784ddeu+f->r[1]*2u));
    /* 00520644 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00520648 */
    if(!(f->flags&MI_ZF))goto mi_00520670;
    /* 0052064a */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784de0u));
    /* 00520651 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00520653 */
    if(!(f->flags&MI_ZF))goto mi_005206bd;
    /* 00520655 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0052065b */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00520661 */
    a=MI_U32((0x00ea5fa0u+f->r[2]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 00520668 */
    if((f->flags&MI_ZF))goto mi_0052089a;
    /* 0052066e */
    goto mi_005206bd;
mi_00520670:;
    /* 00520670 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00520673 */
    if(!(f->flags&MI_ZF))goto mi_005206bd;
    /* 00520675 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784de0u));
    /* 0052067c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0052067e */
    if(!(f->flags&MI_ZF))goto mi_00520699;
    /* 00520680 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00520686 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 0052068c */
    a=MI_U32((0x00ea5fa0u+f->r[2]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 00520693 */
    if((f->flags&MI_ZF))goto mi_0052089a;
mi_00520699:;
    /* 00520699 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784de2u));
    /* 005206a0 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 005206a2 */
    if(!(f->flags&MI_ZF))goto mi_005206bd;
    /* 005206a4 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 005206aa */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 005206b0 */
    a=MI_U32((0x00ea5fa4u+f->r[2]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 005206b7 */
    if((f->flags&MI_ZF))goto mi_0052089a;
mi_005206bd:;
    /* 005206bd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005206bf */
    if((f->flags&MI_ZF))goto mi_0052089a;
    /* 005206c5 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 005206cb */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 005206d1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206d2 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 005206d5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206d6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206d7 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 005206dc */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206de */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 005206e1 */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206e3 */
    f->r[2]=(0xfffffed6u+f->r[3]);
    /* 005206e9 */
    f->r[1]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 005206ed */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206ee */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206ef */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005206f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005206f6u;f->boundary(0x005206f1u,0x004d6480u,f);
    /* 005206f6 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 005206fb */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 00520701 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 00520707 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520708 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520709 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052070a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052070b */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 0052070f */
    f->r[2]=MI_U32((0x00000048u+f->r[4]));
    /* 00520713 */
    a=0x000000b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520718 */
    f->r[1]=(0xfffffef6u+f->r[3]);
    /* 0052071e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052071f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520720 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520721 */
    a=0x000000c6u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520726 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052072bu;f->boundary(0x00520726u,0x004d6250u,f);
    /* 0052072b */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052072e */
    goto mi_0052089a;
mi_00520733:;
    /* 00520733 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 00520739 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 0052073f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520740 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520741 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*2u);
    /* 00520744 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520745 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520747 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0052074a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0052074fu;f->boundary(0x0052074au,0x004d5ee0u,f);
    /* 0052074f */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00520752 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520753 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 00520758 */
    f->r[1]=(0x0000006eu+f->r[6]+f->r[0]*1u);
    /* 0052075c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052075d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520762u;f->boundary(0x0052075du,0x004d5ed0u,f);
    /* 00520762 */
    f->r[2]=f->r[3];
    /* 00520764 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00520766 */
    a=f->r[2];b=0x00000061u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00520769 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052076a */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052076c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520771u;f->boundary(0x0052076cu,0x004d6480u,f);
    /* 00520771 */
    f->r[0]=MI_U32((0x007056ecu));
    /* 00520776 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00520779 */
    a=f->r[0];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0052077c */
    if(!(f->flags&MI_ZF))goto mi_005207e5;
    /* 0052077e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00520783 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520789 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0052078f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520790 */
    f->r[1]=MI_U32((0x00ea5f9cu+f->r[0]));
    /* 00520796 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0052079b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0052079f */
    f->r[1]=MI_U32((0x007056e8u));
    /* 005207a5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207a6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207a7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207a8 */
    f->r[2]=(0x00000071u+f->r[6]+f->r[1]*1u);
    /* 005207ac */
    a=0x000000b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207b1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005207b7u;f->boundary(0x005207b2u,0x004d5ed0u,f);
    /* 005207b7 */
    f->r[1]=f->r[3];
    /* 005207b9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 005207bb */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 005207bf */
    f->r[2]=f->r[0];
    /* 005207c1 */
    a=f->r[1];b=0x00000061u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 005207c4 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 005207c7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005207c9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207ca */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 005207ce */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[2]*8u));
    /* 005207d6 */
    f->r[2]=(0xffffff7eu+f->r[1]+f->r[0]*2u);
    /* 005207dd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207de */
    a=0x000000bdu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207e3 */
    goto mi_0052082d;
mi_005207e5:;
    /* 005207e5 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 005207ea */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 005207f0 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 005207f6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207f7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207f8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207f9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005207fa */
    f->r[0]=(0x00000071u+f->r[6]+f->r[2]*1u);
    /* 005207fe */
    a=0x000000b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520803 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520804 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520809u;f->boundary(0x00520804u,0x004d5ed0u,f);
    /* 00520809 */
    f->r[1]=f->r[3];
    /* 0052080b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0052080d */
    f->r[0]=MI_U32((0x007056ecu));
    /* 00520812 */
    a=f->r[1];b=0x00000059u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00520815 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00520818 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520819 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0052081c */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 00520820 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00520823 */
    f->r[2]=MI_U32((0x005ce3bcu+f->r[1]*4u));
    /* 0052082a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052082b */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0052082d:;
    /* 0052082d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520832u;f->boundary(0x0052082du,0x004d6250u,f);
    /* 00520832 */
    f->r[0]=MI_U32((0x007056ecu));
    /* 00520837 */
    f->r[1]=MI_U32((0x00000034u+f->r[4]));
    /* 0052083b */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0052083e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00520841 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00520844 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 00520847 */
    f->r[0]=MI_U32((0x005ce3bcu+f->r[2]*4u));
    /* 0052084e */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 00520851 */
    if(!(f->flags&MI_ZF))goto mi_00520858;
    /* 00520853 */
    f->r[0]=0x0000005au;
mi_00520858:;
    /* 00520858 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0052085b */
    a=f->r[0];b=0x00000039u;t=mi_alu(f,a,b,32,1);
    /* 0052085e */
    f->r[1]=MI_U32((0x00705ea4u+f->r[1]*8u));
    /* 00520865 */
    if((f->flags&MI_ZF))goto mi_0052089a;
    /* 00520867 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00520869 */
    if((f->flags&MI_ZF))goto mi_0052089a;
    /* 0052086b */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 00520871 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 00520876 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520877 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 0052087d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052087e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520880 */
    f->r[0]=(0x00000072u+f->r[6]+f->r[2]*1u);
    /* 00520884 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520885 */
    f->r[2]=(0xffffffaeu+f->r[3]);
    /* 00520888 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520889 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052088a */
    a=0x005f89ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0052088f */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520891 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00520892 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00520897u;f->boundary(0x00520892u,0x004d6860u,f);
mi_00520897:;
    /* 00520897 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0052089a:;
    /* 0052089a */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0052089e */
    f->r[0]=MI_U32((0x007056f0u));
    /* 005208a3 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 005208a5 */
    if(mi_signed_less(f->flags))goto mi_00520042;
    /* 005208ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005208ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_005208ad:;
    /* 005208ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005208ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005208af */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005208b2 */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004e1870(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e1870 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004e1876 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004e187b */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e187e */
    MI_U32((0x00e95110u))=(uint32_t)(f->r[0]);
    /* 004e1883 */
    MI_U32((0x00e9510cu))=(uint32_t)(f->r[1]);
    /* 004e1889 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e188c */
    a=f->r[1];b=0x00000046u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e188f */
    MI_U32((0x00e95108u))=(uint32_t)(0x0000001eu);
    /* 004e1899 */
    MI_U32((0x00e95104u))=(uint32_t)(0x00000016u);
    /* 004e18a3 */
    MI_U32((0x00e95100u))=(uint32_t)(f->r[0]);
    /* 004e18a8 */
    MI_U32((0x00e950fcu))=(uint32_t)(f->r[1]);
    /* 004e18ae */
    MI_U32((0x00e950f8u))=(uint32_t)(0x0000001au);
    /* 004e18b8 */
    MI_U32((0x00e950f4u))=(uint32_t)(0x0000000du);
    /* 004e18c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e18c7u;f->boundary(0x004e18c2u,0x004dddc0u,f);
    /* 004e18c7 */
    f->r[2]=MI_U32((0x00e95104u));
    /* 004e18cd */
    f->r[0]=MI_U32((0x00e95108u));
    /* 004e18d2 */
    f->r[1]=MI_U32((0x00e9510cu));
    /* 004e18d8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e18d9 */
    f->r[2]=MI_U32((0x00e95110u));
    /* 004e18df */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e18e0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e18e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e18e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e18e7u;f->boundary(0x004e18e2u,0x00536820u,f);
    /* 004e18e7 */
    f->r[1]=MI_U32((0x00e95108u));
    /* 004e18ed */
    f->r[0]=MI_U32((0x00f56fb4u));
    /* 004e18f2 */
    f->r[2]=MI_U32((0x00e9510cu));
    /* 004e18f8 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e18fd */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1900 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1901 */
    f->r[0]=MI_U32((0x00e95110u));
    /* 004e1906 */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e1909 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e190a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e190b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e190c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e190e */
    a=0x0000003fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1910 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1915u;f->boundary(0x004e1910u,0x004ccb30u,f);
    /* 004e1915 */
    f->r[1]=MI_U32((0x00e950f4u));
    /* 004e191b */
    f->r[2]=MI_U32((0x00e950f8u));
    /* 004e1921 */
    f->r[0]=MI_U32((0x00e950fcu));
    /* 004e1926 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1927 */
    f->r[1]=MI_U32((0x00e95100u));
    /* 004e192d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e192e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e192f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1930 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1935u;f->boundary(0x004e1930u,0x00536a30u,f);
    /* 004e1935 */
    f->r[0]=MI_U32((0x00e951ecu));
    /* 004e193a */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e193d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e193f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e197a;
    /* 004e1941 */
    f->r[0]=MI_U32((0x00e950f8u));
    /* 004e1946 */
    f->r[2]=MI_U32((0x00f56f94u));
    /* 004e194c */
    f->r[1]=MI_U32((0x00e950fcu));
    /* 004e1952 */
    a=0x0000ffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1957 */
    a=f->r[0];b=0xfffffffdu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e195a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e195b */
    f->r[2]=MI_U32((0x00e95100u));
    /* 004e1961 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e1964 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e1967 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1968 */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e196b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e196c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e196d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e196f */
    a=0x0000003fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1971 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1976u;f->boundary(0x004e1971u,0x004ccfd0u,f);
    /* 004e1976 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1979 */
    f->r[4]+=4;return;
mi_004e197a:;
    /* 004e197a */
    f->r[1]=MI_U32((0x00e950fcu));
    /* 004e1980 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 004e1985 */
    f->r[2]=MI_U32((0x00e95100u));
    /* 004e198b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e198d */
    a=f->r[1];b=0xffffffeeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e1990 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1991 */
    a=f->r[2];b=0x0000002au;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e1994 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1995 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1996 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1998 */
    a=0x0000003fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e199a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e199fu;f->boundary(0x004e199au,0x004ccbb0u,f);
    /* 004e199f */
    f->r[1]=MI_U32((0x00e950fcu));
    /* 004e19a5 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 004e19aa */
    f->r[2]=MI_U32((0x00e95100u));
    /* 004e19b0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19b2 */
    a=f->r[1];b=0xffffffeeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e19b5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19b6 */
    a=f->r[2];b=0x000000c8u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e19bc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19bd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19be */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19c0 */
    a=0x0000003fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e19c7u;f->boundary(0x004e19c2u,0x004ccbb0u,f);
    /* 004e19c7 */
    f->r[1]=MI_U32((0x00e950f8u));
    /* 004e19cd */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 004e19d2 */
    f->r[2]=MI_U32((0x00e950f4u));
    /* 004e19d8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19da */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e19dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19de */
    f->r[0]=MI_U32((0x00e950fcu));
    /* 004e19e3 */
    a=f->r[1];b=0x00000032u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004e19e6 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004e19e9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19ea */
    f->r[1]=(0x00000003u+f->r[2]+f->r[0]*1u);
    /* 004e19ee */
    f->r[2]=MI_U32((0x00e95100u));
    /* 004e19f4 */
    a=f->r[2];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e19f7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19f9 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19fb */
    a=0x0000003fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e19fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1a02u;f->boundary(0x004e19fdu,0x004ccfd0u,f);
    /* 004e1a02 */
    f->r[1]=MI_U32((0x00e95104u));
    /* 004e1a08 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 004e1a0d */
    f->r[2]=MI_U32((0x00e9510cu));
    /* 004e1a13 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1a16 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1a19 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a1b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a1c */
    f->r[0]=(0xffffffdfu+f->r[1]+f->r[2]*1u);
    /* 004e1a20 */
    a=0x00000118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a25 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a26 */
    f->r[0]=MI_U32((0x00e95108u));
    /* 004e1a2b */
    f->r[1]=MI_U32((0x00e95110u));
    /* 004e1a31 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e1a34 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e1a35 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e1a37 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004e1a39 */
    f->r[2]=(0xffffff74u+f->r[0]+f->r[1]*1u);
    /* 004e1a40 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a41 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a43 */
    a=0x0000003fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1a4au;f->boundary(0x004e1a45u,0x004ccb30u,f);
    /* 004e1a4a */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1a4d */
    f->r[4]+=4;return;
}

void restore_resource_render_C_004e1a50(struct restore_resource_render_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e1a50 */
    f->r[0]=MI_U32((0x00e95104u));
    /* 004e1a55 */
    f->r[1]=MI_U32((0x00e9510cu));
    /* 004e1a5b */
    a=f->r[4];b=0x000000d4u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e1a61 */
    MI_U8((0x00e38dc9u))=(uint8_t)(0x00000001u);
    /* 004e1a68 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e1a6b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a6c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a6d */
    f->r[2]=(0xffffffd6u+f->r[0]+f->r[1]*1u);
    /* 004e1a71 */
    f->r[0]=MI_U32((0x00e95110u));
    /* 004e1a76 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a78 */
    a=0x005c7e98u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a7d */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e1a80 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a81 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1a87u;f->boundary(0x004e1a82u,0x00425730u,f);
    /* 004e1a87 */
    f->r[1]=MI_U32((0x00e95104u));
    /* 004e1a8d */
    f->r[2]=MI_U32((0x00e9510cu));
    /* 004e1a93 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1a96 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a98 */
    a=0x005c80f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1a9d */
    f->r[0]=(0xffffffdcu+f->r[1]+f->r[2]*1u);
    /* 004e1aa1 */
    f->r[1]=MI_U32((0x00e95108u));
    /* 004e1aa7 */
    f->r[2]=MI_U32((0x00e95110u));
    /* 004e1aad */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1aae */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1ab1 */
    f->r[0]=(0xffffffdau+f->r[1]+f->r[2]*1u);
    /* 004e1ab5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1ab6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1abbu;f->boundary(0x004e1ab6u,0x00425730u,f);
    /* 004e1abb */
    f->r[0]=MI_U32((0x00e951ecu));
    /* 004e1ac0 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1ac3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e1ac5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e1df9;
    /* 004e1acb */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004e1ace */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(0x0000000au);
    /* 004e1ad6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e1adc;
    /* 004e1ad8 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004e1adc:;
    /* 004e1adc */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004e1ae0 */
    f->r[7]=MI_U32((0x00e95120u));
    /* 004e1ae6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1ae7 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004e1ae9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e1aeb */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004e1aef */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e1c8e;
    /* 004e1af5 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004e1af8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1af9 */
    a=f->r[7];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004e1afc */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004e1afe */
    a=f->r[7];b=0x00e95236u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004e1b04:;
    /* 004e1b04 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffcu+f->r[7]));
    /* 004e1b08 */
    MI_U32((0x00e951dcu))=(uint32_t)(f->r[0]);
    /* 004e1b0d */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004e1b10 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1b13 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00e209bau+f->r[1])))&0xffffu)<<0);
    /* 004e1b1a */
    a=(f->r[0]&0xffffu);b=0x00000004u;t=mi_alu(f,a,b,16,1);
    /* 004e1b1e */
    if(!(f->flags&MI_ZF))goto mi_004e1b27;
    /* 004e1b20 */
    f->r[0]=0x00000004u;
    /* 004e1b25 */
    goto mi_004e1b33;
mi_004e1b27:;
    /* 004e1b27 */
    a=(f->r[0]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,16,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e1b29 */
    a=(f->r[0]&0xffffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,16,1);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 004e1b2c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004e1b2e */
    a=(f->r[0]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e1b30 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004e1b33:;
    /* 004e1b33 */
    a=MI_U8((0x00000000u+f->r[7]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004e1b36 */
    if(!(f->flags&MI_ZF))goto mi_004e1b5f;
    /* 004e1b38 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004e1b3b */
    if((f->flags&MI_ZF))goto mi_004e1b5f;
    /* 004e1b3d */
    f->r[2]=MI_U32((0x00e950fcu));
    /* 004e1b43 */
    f->r[1]=(0x00000006u+f->r[6]+f->r[2]*1u);
    /* 004e1b47 */
    f->r[2]=MI_U32((0x00e95100u));
    /* 004e1b4d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1b4e */
    f->r[1]=MI_U32((0x00f566ecu));
    /* 004e1b54 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e1b57 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1b58 */
    f->r[2]=(0x0000000eu+f->r[1]+f->r[0]*1u);
    /* 004e1b5c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1b5d */
    goto mi_004e1b7f;
mi_004e1b5f:;
    /* 004e1b5f */
    f->r[1]=MI_U32((0x00e950fcu));
    /* 004e1b65 */
    f->r[2]=(0x00000006u+f->r[6]+f->r[1]*1u);
    /* 004e1b69 */
    f->r[1]=MI_U32((0x00e95100u));
    /* 004e1b6f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1b70 */
    f->r[2]=MI_U32((0x00f566ecu));
    /* 004e1b76 */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e1b79 */
    f->r[0]=(0x0000000fu+f->r[2]+f->r[0]*1u);
    /* 004e1b7d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1b7e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e1b7f:;
    /* 004e1b7f */
    f->r[1]=0x00f55bb0u;
    /* 004e1b84 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1b89u;f->boundary(0x004e1b84u,0x0053e590u,f);
    /* 004e1b89 */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004e1b8e */
    f->r[3]=(0x00000001u+f->r[5]);
    /* 004e1b91 */
    f->r[5]=MI_U32((0x00f56fa4u));
    /* 004e1b97 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004e1b99 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e1b9d */
    if((f->flags&MI_ZF))goto mi_004e1ba5;
    /* 004e1b9f */
    f->r[5]=MI_U32((0x00f56f9cu));
mi_004e1ba5:;
    /* 004e1ba5 */
    f->r[1]=MI_U32((0x00e950fcu));
    /* 004e1bab */
    f->r[0]=MI_U32((0x00e95100u));
    /* 004e1bb0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bb2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bb3 */
    f->r[2]=(0x00000008u+f->r[6]+f->r[1]*1u);
    /* 004e1bb7 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004e1bb9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000001u+f->r[7])))&0xffu)<<0);
    /* 004e1bbc */
    a=f->r[0];b=0x0000002au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e1bbf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bc0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bc1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bc2 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bc4 */
    MI_U32((0x00e20778u))=(uint32_t)(0x00000000u);
    /* 004e1bce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1bd3u;f->boundary(0x004e1bceu,0x004ccbb0u,f);
    /* 004e1bd3 */
    f->r[2]=MI_U32((0x00e950fcu));
    /* 004e1bd9 */
    f->r[1]=MI_U32((0x00e95100u));
    /* 004e1bdf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1be1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1be2 */
    f->r[0]=(0x00000008u+f->r[6]+f->r[2]*1u);
    /* 004e1be6 */
    a=f->r[1];b=0x0000005au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e1be9 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffau+f->r[7]));
    /* 004e1bed */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bf0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1bf2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1bf7u;f->boundary(0x004e1bf2u,0x004cccb0u,f);
    /* 004e1bf7 */
    f->r[0]=MI_U32((0x00e951dcu));
    /* 004e1bfc */
    f->r[1]=MI_U32((0x00e36c74u));
    /* 004e1c02 */
    a=f->r[4];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1c05 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004e1c08 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e1c0b */
    f->r[0]=MI_U32((0x00e209fcu+f->r[0]));
    /* 004e1c11 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e1c13 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[7]));
    /* 004e1c17 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e1c19 */
    if(mi_signed_less(f->flags))goto mi_004e1c4c;
    /* 004e1c1b */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004e1c1f */
    f->r[3]=MI_U32((0x00e951e4u));
    /* 004e1c25 */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 004e1c29 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c2e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c2f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c30 */
    MI_U32((0x00e951e4u))=(uint32_t)(f->r[2]);
    /* 004e1c36 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1c3bu;f->boundary(0x004e1c36u,0x00448330u,f);
    /* 004e1c3b */
    MI_U32((0x00e951e4u))=(uint32_t)(f->r[3]);
    /* 004e1c41 */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 004e1c45 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1c48 */
    f->r[0]=(0x0000001cu+f->r[4]);
mi_004e1c4c:;
    /* 004e1c4c */
    f->r[1]=MI_U32((0x00e950fcu));
    /* 004e1c52 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c54 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c55 */
    f->r[2]=(0x00000008u+f->r[6]+f->r[1]*1u);
    /* 004e1c59 */
    f->r[1]=MI_U32((0x00e95100u));
    /* 004e1c5f */
    a=f->r[1];b=0x000000c8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e1c65 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c66 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c67 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1c68 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1c6du;f->boundary(0x004e1c68u,0x004cc770u,f);
    /* 004e1c6d */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004e1c71 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004e1c75 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1c78 */
    f->r[5]=f->r[3];
    /* 004e1c7a */
    a=f->r[6];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004e1c7d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004e1c7e */
    a=f->r[7];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004e1c81 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e1c83 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004e1c87 */
    if(mi_signed_less(f->flags))goto mi_004e1b04;
    /* 004e1c8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_004e1c8e:;
    /* 004e1c8e */
    f->r[0]=MI_U32((0x007056c4u));
    /* 004e1c93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e1c94 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e1c96 */
    if((f->flags&MI_ZF))goto mi_004e1cd4;
    /* 004e1c98 */
    f->r[2]=MI_U32((0x00e95104u));
    /* 004e1c9e */
    f->r[0]=MI_U32((0x00e9510cu));
    /* 004e1ca3 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004e1ca6 */
    a=0x0000f800u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1cab */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1cad */
    f->r[1]=(0xffffffd8u+f->r[2]+f->r[0]*1u);
    /* 004e1cb1 */
    f->r[0]=MI_U32((0x00e95108u));
    /* 004e1cb6 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e1cb9 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e1cba */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e1cbc */
    f->r[2]=MI_U32((0x00e95110u));
    /* 004e1cc2 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004e1cc4 */
    a=0x00000118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1cc9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1cca */
    f->r[0]=(0xffffff74u+f->r[0]+f->r[2]*1u);
    /* 004e1cd1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1cd2 */
    goto mi_004e1d0c;
mi_004e1cd4:;
    /* 004e1cd4 */
    f->r[1]=MI_U32((0x00e95104u));
    /* 004e1cda */
    f->r[2]=MI_U32((0x00e9510cu));
    /* 004e1ce0 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1ce3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1ce5 */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1ce7 */
    f->r[0]=(0xffffffd8u+f->r[1]+f->r[2]*1u);
    /* 004e1ceb */
    a=0x00000118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1cf0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1cf1 */
    f->r[0]=MI_U32((0x00e95108u));
    /* 004e1cf6 */
    f->r[1]=MI_U32((0x00e95110u));
    /* 004e1cfc */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e1cff */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004e1d00 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004e1d02 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004e1d04 */
    f->r[2]=(0xffffff74u+f->r[0]+f->r[1]*1u);
    /* 004e1d0b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004e1d0c:;
    /* 004e1d0c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d0e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1d13u;f->boundary(0x004e1d0eu,0x00536c30u,f);
    /* 004e1d13 */
    f->r[0]=MI_U32((0x00e9511cu));
    /* 004e1d18 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1d1b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e1d1d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004e1df9;
    /* 004e1d23 */
    f->r[1]=MI_U32((0x00e950f8u));
    /* 004e1d29 */
    f->r[3]=MI_U32((0x00e95100u));
    /* 004e1d2f */
    f->r[0]=MI_U32((0x00e950fcu));
    /* 004e1d34 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d36 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1d39 */
    a=0x005c8118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d3e */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e1d40 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d41 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d42 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1d47u;f->boundary(0x004e1d42u,0x00425730u,f);
    /* 004e1d47 */
    f->r[2]=MI_U32((0x00e950f4u));
    /* 004e1d4d */
    f->r[0]=MI_U32((0x00e950fcu));
    /* 004e1d52 */
    f->r[7]=MI_U32((0x00e95100u));
    /* 004e1d58 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d5a */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004e1d5d */
    a=0x005c8140u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d62 */
    f->r[1]=(0xffffffe6u+f->r[2]+f->r[0]*1u);
    /* 004e1d66 */
    f->r[2]=MI_U32((0x00e950f8u));
    /* 004e1d6c */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004e1d6f */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e1d71 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d72 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1d78u;f->boundary(0x004e1d73u,0x00425730u,f);
    /* 004e1d78 */
    f->r[0]=MI_U32((0x00e95120u));
    /* 004e1d7d */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1d80 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e1d82 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e1d88;
    /* 004e1d84 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004e1d86 */
    goto mi_004e1da3;
mi_004e1d88:;
    /* 004e1d88 */
    f->r[1]=MI_U32((0x00e9511cu));
    /* 004e1d8e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004e1d90 */
    if(mi_signed_less(f->flags))goto mi_004e1d99;
    /* 004e1d92 */
    f->r[0]=0x00000064u;
    /* 004e1d97 */
    goto mi_004e1da3;
mi_004e1d99:;
    /* 004e1d99 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d9a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1d9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1da0u;f->boundary(0x004e1d9bu,0x004cde60u,f);
    /* 004e1da0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e1da3:;
    /* 004e1da3 */
    f->r[1]=MI_U32((0x00e950f4u));
    /* 004e1da9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1daa */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e1dad */
    a=f->r[1];b=0x0000004du;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004e1db0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1db1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1db6u;f->boundary(0x004e1db1u,0x004cde40u,f);
    /* 004e1db6 */
    f->r[1]=MI_U32((0x00e95118u));
    /* 004e1dbc */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1dbf */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004e1dc1 */
    if((f->flags&MI_ZF))goto mi_004e1dc8;
    /* 004e1dc3 */
    f->r[0]=MI_U32((0x00e95114u));
mi_004e1dc8:;
    /* 004e1dc8 */
    f->r[1]=MI_U32((0x00e950fcu));
    /* 004e1dce */
    f->r[2]=(0x0000001au+f->r[1]+f->r[0]*1u);
    /* 004e1dd2 */
    f->r[0]=MI_U32((0x00e950f8u));
    /* 004e1dd7 */
    f->r[1]=MI_U32((0x00e95100u));
    /* 004e1ddd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1dde */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e1de1 */
    f->r[2]=(0x00000009u+f->r[0]+f->r[1]*1u);
    /* 004e1de5 */
    f->r[0]=MI_U32((0x00f56494u));
    /* 004e1dea */
    a=f->r[0];b=0x00000027u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e1ded */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1dee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e1def */
    f->r[1]=0x00f55bb0u;
    /* 004e1df4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e1df9u;f->boundary(0x004e1df4u,0x0053e590u,f);
mi_004e1df9:;
    /* 004e1df9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e1dfa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e1dfb */
    a=f->r[4];b=0x000000d4u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e1e01 */
    f->r[4]+=4;return;
}
