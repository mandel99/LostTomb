/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "briefing_draw.h"
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
static __inline uint32_t mi_alu(struct restore_briefing_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_briefing_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_briefing_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_briefing_C_0041a180(struct restore_briefing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0041a180 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a181 */
    f->r[0]=MI_U32((0x007027a8u));
    /* 0041a186 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a187 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0041a189 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0041a18b */
    if(!(f->flags&MI_ZF))goto mi_0041a1b2;
    /* 0041a18d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a18e */
    a=0x00e37078u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a193 */
    MI_U32((0x007027a8u))=(uint32_t)(0x00000001u);
    /* 0041a19d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a1a2u;f->boundary(0x0041a19du,0x005405b0u,f);
    /* 0041a1a2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 0041a1a7 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a1aa */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0041a1ac */
    if((f->flags&MI_ZF))goto mi_0041a849;
mi_0041a1b2:;
    /* 0041a1b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a1b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a1b8u;f->boundary(0x0041a1b3u,0x00418140u,f);
    /* 0041a1b8 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 0041a1be */
    MI_U32((0x00d3b124u))=(uint32_t)(f->r[0]);
    /* 0041a1c3 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 0041a1c8 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a1cb */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a1ce */
    MI_U32((0x006fdc9cu))=(uint32_t)(f->r[1]);
    /* 0041a1d4 */
    f->r[1]=MI_U32((0x00e38db0u));
    /* 0041a1da */
    MI_U32((0x006fdca0u))=(uint32_t)(f->r[0]);
    /* 0041a1df */
    f->r[0]=0x00000400u;
    /* 0041a1e4 */
    f->r[6]=MI_U32((0x00e38dacu));
    /* 0041a1ea */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041a1ec */
    MI_U32((0x00d3b574u))=(uint32_t)(f->r[1]);
    /* 0041a1f2 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041a1f3 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041a1f5 */
    MI_U32((0x00d3b570u))=(uint32_t)(f->r[6]);
    /* 0041a1fb */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0041a1fd */
    MI_U32((0x00d3b56cu))=(uint32_t)(f->r[0]);
    /* 0041a202 */
    f->r[0]=0x00000300u;
    /* 0041a207 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041a209 */
    MI_U32((0x00d3b14cu))=(uint32_t)(0x00000023u);
    /* 0041a213 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0041a214 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041a216 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 0041a218 */
    MI_U32((0x00d3b568u))=(uint32_t)(f->r[0]);
    /* 0041a21d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38dcau)))&0xffu)<<0);
    /* 0041a222 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0041a224 */
    if(!(f->flags&MI_ZF))goto mi_0041a23a;
    /* 0041a226 */
    f->r[2]=MI_U32((0x00f55c44u));
    /* 0041a22c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a22d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0041a22e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a22f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a230 */
    f->r[1]=0x00f55bb0u;
    /* 0041a235 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a23au;f->boundary(0x0041a235u,0x0053e590u,f);
mi_0041a23a:;
    /* 0041a23a */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 0041a23f */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 0041a245 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a246 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a247 */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a249 */
    a=0x00000026u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a24b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a24c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a24d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a252u;f->boundary(0x0041a24du,0x00536820u,f);
    /* 0041a252 */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 0041a257 */
    f->r[2]=MI_U32((0x00f56fb4u));
    /* 0041a25d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a25e */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a261 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a262 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a263 */
    f->r[0]=MI_U32((0x00d3b124u));
    /* 0041a268 */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 0041a26e */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a271 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0041a274 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a275 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0041a278 */
    f->r[0]=MI_U32((0x00e209fcu+f->r[2]));
    /* 0041a27e */
    f->r[2]=MI_U32((0x00e36c74u));
    /* 0041a284 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a286 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a287 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a28cu;f->boundary(0x0041a287u,0x004cc770u,f);
    /* 0041a28c */
    f->r[2]=MI_U32((0x006fdc9cu));
    /* 0041a292 */
    f->r[1]=MI_U32((0x00f56f8cu));
    /* 0041a298 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 0041a29d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a29e */
    a=f->r[2];b=0x0000002eu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a2a1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2a2 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a2a5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2a6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2a7 */
    f->r[0]=MI_U32((0x00d3b124u));
    /* 0041a2ac */
    f->r[5]=MI_U32((0x00e36c74u));
    /* 0041a2b2 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0041a2b5 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0041a2b8 */
    f->r[2]=MI_U32((0x00e20a00u+f->r[1]));
    /* 0041a2be */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a2c0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2c1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a2c6u;f->boundary(0x0041a2c1u,0x004cc770u,f);
    /* 0041a2c6 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a2cc */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0041a2d1 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a2d7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2d8 */
    a=f->r[1];b=0x000001a1u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a2de */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2df */
    a=f->r[2];b=0x000001dcu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a2e5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2e6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2e7 */
    a=0x00000007u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2e9 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a2eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a2f0u;f->boundary(0x0041a2ebu,0x004ccbb0u,f);
    /* 0041a2f0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 0041a2f5 */
    a=f->r[4];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a2f8 */
    a=(f->r[0]&0xffu);b=0x00000035u;t=mi_alu(f,a,b,8,1);
    /* 0041a2fa */
    if(!(f->flags&MI_ZF))goto mi_0041a334;
    /* 0041a2fc */
    f->r[1]=0x006fdd08u;
    /* 0041a301 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a306u;f->boundary(0x0041a301u,0x0041b8f0u,f);
    /* 0041a306 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041a308 */
    if((f->flags&MI_ZF))goto mi_0041a334;
    /* 0041a30a */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a310 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0041a315 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a31b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a31c */
    a=f->r[1];b=0x000001a3u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a322 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a323 */
    a=f->r[2];b=0x00000032u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a326 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a327 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a328 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a32a */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a32c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a331u;f->boundary(0x0041a32cu,0x004ccbb0u,f);
    /* 0041a331 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0041a334:;
    /* 0041a334 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a33a */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0041a33f */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a345 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a346 */
    a=f->r[1];b=0x000001a1u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a34c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a34d */
    a=f->r[2];b=0x00000096u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a353 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a354 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a355 */
    a=0x000000d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a35a */
    a=0x0000002cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a35c */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[3]);
    /* 0041a362 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a367u;f->boundary(0x0041a362u,0x004ccbb0u,f);
    /* 0041a367 */
    f->r[0]=MI_U32((0x00f56f8cu));
    /* 0041a36c */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a372 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a378 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a379 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a37a */
    f->r[0]=MI_U32((0x00e20778u));
    /* 0041a37f */
    a=f->r[1];b=0x000001a1u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a385 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a386 */
    f->r[1]=(0x00000096u+f->r[0]+f->r[2]*1u);
    /* 0041a38d */
    f->r[2]=MI_U32((0x00e39080u));
    /* 0041a393 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a394 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0041a395 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a396 */
    a=0x00000099u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a39b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a3a0u;f->boundary(0x0041a39bu,0x004ccbb0u,f);
    /* 0041a3a0 */
    f->r[0]=MI_U32((0x00d3b14cu));
    /* 0041a3a5 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a3ab */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a3b1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0041a3b2 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3b4 */
    a=f->r[1];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a3b7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3b8 */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a3bb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3bc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a3c2u;f->boundary(0x0041a3bdu,0x00536a30u,f);
    /* 0041a3c2 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a3c8 */
    f->r[0]=MI_U32((0x00f56f9cu));
    /* 0041a3cd */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a3d3 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a3d6 */
    a=f->r[1];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a3d9 */
    a=f->r[2];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a3dc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3dd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3de */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3df */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3e0 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3e2 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a3e4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a3e9u;f->boundary(0x0041a3e4u,0x004ccbb0u,f);
    /* 0041a3e9 */
    f->r[0]=MI_U32((0x00784c58u));
    /* 0041a3ee */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a3f1 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0041a3f3 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0041a3f5 */
    MI_U32((0x00f53f28u))=(uint32_t)(0x00000064u);
    /* 0041a3ff */
    if((f->flags&MI_ZF))goto mi_0041a49a;
    /* 0041a405 */
    f->r[5]=MI_U32((0x005797d8u));
    /* 0041a40b */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 0041a410 */
    f->r[7]=MI_U32((0x005797d4u));
    /* 0041a416 */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 0041a41c */
    f->r[6]=0x00000001u;
    /* 0041a421 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a423 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a424 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a425 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a427 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a429 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a42a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a42b */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a42d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a432u;f->boundary(0x0041a42du,0x00537000u,f);
    /* 0041a432 */
    f->r[2]=MI_U32((0x00f56fa4u));
    /* 0041a438 */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 0041a43d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a43e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a43f */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a445 */
    f->r[1]=(0x00000003u+f->r[0]+f->r[5]*1u);
    /* 0041a449 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a44a */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[3]);
    /* 0041a450 */
    f->r[0]=(0x00000008u+f->r[2]+f->r[7]*1u);
    /* 0041a454 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a455 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a457 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a459 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a45eu;f->boundary(0x0041a459u,0x004ccbb0u,f);
    /* 0041a45e */
    f->r[2]=MI_U32((0x006fdc9cu));
    /* 0041a464 */
    f->r[1]=MI_U32((0x00f56fa4u));
    /* 0041a46a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a46b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a46c */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 0041a472 */
    f->r[0]=(0x00000003u+f->r[2]+f->r[5]*1u);
    /* 0041a476 */
    f->r[2]=MI_U32((0x00e20778u));
    /* 0041a47c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a47d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a47f */
    f->r[1]=MI_U32((0x00784c5cu));
    /* 0041a485 */
    f->r[0]=(0x00000008u+f->r[2]+f->r[7]*1u);
    /* 0041a489 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a48a */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a48f */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a491 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a492 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a497u;f->boundary(0x0041a492u,0x004cce00u,f);
    /* 0041a497 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0041a49a:;
    /* 0041a49a */
    a=MI_U32((0x00784c34u));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0041a4a0 */
    if((f->flags&MI_ZF))goto mi_0041a542;
    /* 0041a4a6 */
    f->r[5]=MI_U32((0x005797d8u+f->r[6]*8u));
    /* 0041a4ad */
    f->r[2]=MI_U32((0x006fdc9cu));
    /* 0041a4b3 */
    f->r[7]=MI_U32((0x005797d4u+f->r[6]*8u));
    /* 0041a4ba */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 0041a4bf */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4c1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4c2 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a4c4 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4c6 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a4c8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4c9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4ca */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4cc */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0041a4cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a4d2u;f->boundary(0x0041a4cdu,0x00537000u,f);
    /* 0041a4d2 */
    f->r[1]=MI_U32((0x00f56fa4u));
    /* 0041a4d8 */
    f->r[2]=MI_U32((0x006fdc9cu));
    /* 0041a4de */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4df */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4e0 */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 0041a4e6 */
    f->r[0]=(0x00000003u+f->r[2]+f->r[5]*1u);
    /* 0041a4ea */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4eb */
    f->r[0]=MI_U32((0x00784c34u));
    /* 0041a4f0 */
    f->r[2]=(0x00000008u+f->r[1]+f->r[7]*1u);
    /* 0041a4f4 */
    MI_U32((0x00e20778u))=(uint32_t)(f->r[3]);
    /* 0041a4fa */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a4fb */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a500 */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a502 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a503 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a508u;f->boundary(0x0041a503u,0x004cce00u,f);
    /* 0041a508 */
    f->r[2]=MI_U32((0x006fdc9cu));
    /* 0041a50e */
    f->r[1]=MI_U32((0x00f56fa4u));
    /* 0041a514 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a515 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a516 */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 0041a51c */
    f->r[0]=(0x00000003u+f->r[2]+f->r[5]*1u);
    /* 0041a520 */
    f->r[2]=MI_U32((0x00e20778u));
    /* 0041a526 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a527 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a529 */
    f->r[1]=MI_U32((0x00784c38u));
    /* 0041a52f */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a532 */
    f->r[0]=(0x00000008u+f->r[2]+f->r[7]*1u);
    /* 0041a536 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a537 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a538 */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a53a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a53fu;f->boundary(0x0041a53au,0x004ccbb0u,f);
    /* 0041a53f */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0041a542:;
    /* 0041a542 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000000cu);
    /* 0041a54a */
    f->r[3]=0x00784c24u;
    /* 0041a54f */
    f->r[5]=(0x005797d8u+f->r[6]*8u);
mi_0041a556:;
    /* 0041a556 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0041a55a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00784c30u+f->r[2])))&0xffu)<<0);
    /* 0041a560 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041a562 */
    if((f->flags&MI_ZF))goto mi_0041a5fa;
    /* 0041a568 */
    f->r[7]=MI_U32((0x00000000u+f->r[5]));
    /* 0041a56b */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 0041a570 */
    f->r[6]=MI_U32((0xfffffffcu+f->r[5]));
    /* 0041a573 */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 0041a579 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a57b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a57d */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a57f */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a581 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a583 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a584 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a585 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a587 */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0041a58a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a58fu;f->boundary(0x0041a58au,0x00537000u,f);
    /* 0041a58f */
    f->r[2]=MI_U32((0x00f56fa4u));
    /* 0041a595 */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 0041a59a */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a59c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a59d */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a5a3 */
    f->r[1]=(0x00000003u+f->r[0]+f->r[7]*1u);
    /* 0041a5a7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5a8 */
    f->r[1]=MI_U32((0x00000034u+f->r[4]));
    /* 0041a5ac */
    f->r[0]=(0x00000008u+f->r[2]+f->r[6]*1u);
    /* 0041a5b0 */
    MI_U32((0x00e20778u))=(uint32_t)(0x00000000u);
    /* 0041a5ba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5bb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5bc */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5be */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a5c3u;f->boundary(0x0041a5beu,0x004ccbb0u,f);
    /* 0041a5c3 */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 0041a5c8 */
    f->r[2]=MI_U32((0x00f56fa4u));
    /* 0041a5ce */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5d0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5d1 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a5d7 */
    f->r[1]=(0x00000003u+f->r[0]+f->r[7]*1u);
    /* 0041a5db */
    f->r[0]=MI_U32((0x00e20778u));
    /* 0041a5e0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5e1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a5e3 */
    f->r[2]=MI_U32((0x00000000u+f->r[3]));
    /* 0041a5e5 */
    f->r[1]=(0x00000008u+f->r[0]+f->r[6]*1u);
    /* 0041a5e9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5ea */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5ef */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5f1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a5f2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a5f7u;f->boundary(0x0041a5f2u,0x004cce00u,f);
    /* 0041a5f7 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0041a5fa:;
    /* 0041a5fa */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0041a5fe */
    a=f->r[3];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0041a601 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0041a602 */
    a=f->r[3];b=0x00784c34u;t=mi_alu(f,a,b,32,1);
    /* 0041a608 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0041a60c */
    if(mi_signed_less(f->flags))goto mi_0041a556;
    /* 0041a612 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a617u;f->boundary(0x0041a612u,0x004e2530u,f);
    /* 0041a617 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041a619 */
    if((f->flags&MI_ZF))goto mi_0041a679;
    /* 0041a61b */
    f->r[0]=MI_U32((0x00d3b14cu));
    /* 0041a620 */
    f->r[7]=MI_U32((0x005797f8u));
    /* 0041a626 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a62c */
    f->r[6]=MI_U32((0x005797f4u));
    /* 0041a632 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a638 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0041a63a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a63c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0041a63d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a63e */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a640 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a641 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a643 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a644 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a645 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a647 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a64cu;f->boundary(0x0041a647u,0x00537000u,f);
    /* 0041a64c */
    f->r[0]=MI_U32((0x00f56fa4u));
    /* 0041a651 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a657 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a658 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a659 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 0041a65e */
    f->r[2]=(0x00000003u+f->r[1]+f->r[7]*1u);
    /* 0041a662 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a663 */
    f->r[2]=MI_U32((0x00e20768u));
    /* 0041a669 */
    f->r[1]=(0x00000008u+f->r[0]+f->r[6]*1u);
    /* 0041a66d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a66e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a66f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a674u;f->boundary(0x0041a66fu,0x004cc770u,f);
    /* 0041a674 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a677 */
    goto mi_0041a67b;
mi_0041a679:;
    /* 0041a679 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
mi_0041a67b:;
    /* 0041a67b */
    f->r[0]=MI_U32((0x00d3b14cu));
    /* 0041a680 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a686 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 0041a68c */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a68f */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a691 */
    a=f->r[1];b=0x000000a8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a697 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a698 */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a69b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a69c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a69d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a6a2u;f->boundary(0x0041a69du,0x00536a30u,f);
    /* 0041a6a2 */
    f->r[0]=MI_U32((0x00f56f9cu));
    /* 0041a6a7 */
    f->r[1]=MI_U32((0x00f56fa4u));
    /* 0041a6ad */
    MI_U32((0x00d3b168u))=(uint32_t)(f->r[0]);
    /* 0041a6b2 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 0041a6b7 */
    MI_U32((0x00d3b164u))=(uint32_t)(f->r[1]);
    /* 0041a6bd */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a6c3 */
    f->r[2]=(0xfffffffau+f->r[0]);
    /* 0041a6c6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a6c9 */
    MI_U32((0x00d3b154u))=(uint32_t)(f->r[2]);
    /* 0041a6cf */
    f->r[2]=(0x000000aau+f->r[1]);
    /* 0041a6d5 */
    MI_U32((0x00d3b150u))=(uint32_t)(f->r[2]);
    /* 0041a6db */
    f->r[2]=MI_U32((0x00d3b138u));
    /* 0041a6e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0041a6e2 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0041a6e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0041a6e5 */
    if(!(f->flags&MI_ZF))goto mi_0041a791;
    /* 0041a6eb */
    f->r[2]=MI_U32((0x00d3b14cu));
    /* 0041a6f1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a6f2 */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0041a6f5 */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0041a6f8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a6fa */
    a=f->r[1];b=0x000000b8u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a700 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a701 */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a704 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a705 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a706 */
    f->r[0]=MI_U32((0x00d3b124u));
    /* 0041a70b */
    f->r[6]=MI_U32((0x00e36c74u));
    /* 0041a711 */
    MI_U32((0x00d3b148u))=(uint32_t)(0x0000000fu);
    /* 0041a71b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0041a71e */
    MI_U32((0x00d3b13cu))=(uint32_t)(0x0000000eu);
    /* 0041a728 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0041a72b */
    MI_U8((0x005c7e10u))=(uint8_t)(0x00000001u);
    /* 0041a732 */
    MI_U8((0x005c7e38u))=(uint8_t)(0x00000001u);
    /* 0041a739 */
    f->r[1]=MI_U32((0x00e20a04u+f->r[0]));
    /* 0041a73f */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a741 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a742 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a747u;f->boundary(0x0041a742u,0x004c8070u,f);
    /* 0041a747 */
    f->r[2]=MI_U32((0x00d3b138u));
    /* 0041a74d */
    f->r[6]=MI_U32((0x00d3b13cu));
    /* 0041a753 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a756 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0041a758 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0041a770;
    /* 0041a75a */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a760 */
    f->r[0]=f->r[2];
    /* 0041a762 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0041a764 */
    MI_U32((0x00d3b134u))=(uint32_t)(f->r[0]);
    /* 0041a769 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 0041a76e */
    goto mi_0041a797;
mi_0041a770:;
    /* 0041a770 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 0041a775 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a77b */
    MI_U32((0x00d3b134u))=(uint32_t)(f->r[3]);
    /* 0041a781 */
    MI_U8((0x005c7e10u))=(uint8_t)(0x00000000u);
    /* 0041a788 */
    MI_U8((0x005c7e38u))=(uint8_t)(0x00000000u);
    /* 0041a78f */
    goto mi_0041a797;
mi_0041a791:;
    /* 0041a791 */
    f->r[6]=MI_U32((0x00d3b13cu));
mi_0041a797:;
    /* 0041a797 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0041a799 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0041a7bf;
    /* 0041a79b */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a79d */
    a=f->r[1];b=0x000000c0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a7a3 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7a5 */
    a=f->r[0];b=0x0000022du;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a7aa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7ab */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a7b1u;f->boundary(0x0041a7acu,0x00536a30u,f);
    /* 0041a7b1 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 0041a7b6 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 0041a7bc */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0041a7bf:;
    /* 0041a7bf */
    f->r[2]=(0x00000013u+f->r[0]);
    /* 0041a7c2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7c3 */
    MI_U32((0x00d3b55cu))=(uint32_t)(f->r[2]);
    /* 0041a7c9 */
    f->r[2]=MI_U32((0x00d3b14cu));
    /* 0041a7cf */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0041a7d2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7d3 */
    f->r[6]=(0x00000013u+f->r[2]+f->r[0]*1u);
    /* 0041a7d7 */
    a=f->r[2];b=0xfffffff0u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0041a7da */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[6]);
    /* 0041a7e0 */
    f->r[6]=(0x000000abu+f->r[1]);
    /* 0041a7e6 */
    MI_U32((0x00d3b554u))=(uint32_t)(f->r[6]);
    /* 0041a7ec */
    f->r[6]=(0x00000195u+f->r[1]);
    /* 0041a7f2 */
    a=f->r[1];b=0x000000b4u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a7f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7f9 */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0041a7fc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a7fe */
    f->r[0]=MI_U32((0x00d3b124u));
    /* 0041a803 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[6]);
    /* 0041a809 */
    f->r[6]=MI_U32((0x00e36c74u));
    /* 0041a80f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0041a812 */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0041a815 */
    f->r[1]=MI_U32((0x00e20a04u+f->r[0]));
    /* 0041a81b */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0041a81d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041a81e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0041a823u;f->boundary(0x0041a81eu,0x004c8070u,f);
    /* 0041a823 */
    f->r[2]=MI_U32((0x00e38db0u));
    /* 0041a829 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 0041a82e */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0041a831 */
    MI_U32((0x00d3b55cu))=(uint32_t)(f->r[3]);
    /* 0041a837 */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[2]);
    /* 0041a83d */
    MI_U32((0x00d3b554u))=(uint32_t)(f->r[3]);
    /* 0041a843 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[0]);
    /* 0041a848 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0041a849:;
    /* 0041a849 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0041a84a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0041a84b */
    f->r[4]+=4;return;
}

void restore_briefing_C_00537000(struct restore_briefing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00537000 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00537004 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00537005 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00537008 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00537009 */
    if(!(f->flags&MI_ZF))goto mi_00537011;
    /* 0053700b */
    f->r[3]=MI_U32((0x00000018u+f->r[4]));
    /* 0053700f */
    goto mi_00537048;
mi_00537011:;
    /* 00537011 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 00537015 */
    f->r[0]=MI_U32((0x00f53f10u+f->r[0]*4u));
    /* 0053701c */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0053701f */
    if(mi_signed_less(f->flags))goto mi_005370be;
    /* 00537025 */
    f->r[3]=(0xffffffeeu+f->r[0]);
    /* 00537028 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0053702c */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0053702e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00537034;
    /* 00537030 */
    f->r[3]=f->r[0];
    /* 00537032 */
    goto mi_00537048;
mi_00537034:;
    /* 00537034 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00537036 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0053703a */
    a=f->r[0];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0053703d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0053703e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00537040 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00537042 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00537044 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00537048:;
    /* 00537048 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0053704a */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0053704c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005370be;
    /* 0053704e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053704f */
    f->r[5]=MI_U32((0x00000018u+f->r[4]));
    /* 00537053 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00537054 */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
mi_00537058:;
    /* 00537058 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0053705c */
    f->r[1]=f->r[6];
    /* 0053705e */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00537061 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00537063 */
    MI_U32((0x00d3b5dcu))=(uint32_t)(f->r[5]);
    /* 00537069 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0053706b */
    MI_U32((0x00d3b5e0u))=(uint32_t)(f->r[1]);
    /* 00537071 */
    if(!(f->flags&MI_ZF))goto mi_00537082;
    /* 00537073 */
    f->r[2]=MI_U32((0x00f56494u));
    /* 00537079 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[7]*2u);
    /* 0053707c */
    f->r[0]=(0x00000028u+f->r[7]+f->r[0]*1u);
    /* 00537080 */
    goto mi_005370a5;
mi_00537082:;
    /* 00537082 */
    f->r[2]=(0xffffffffu+f->r[3]);
    /* 00537085 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00537087 */
    if(mi_signed_less(f->flags))goto mi_00537099;
    /* 00537089 */
    f->r[2]=MI_U32((0x00f56494u));
    /* 0053708f */
    f->r[0]=(0x0000000eu+f->r[7]);
    /* 00537092 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 00537095 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00537097 */
    goto mi_005370a5;
mi_00537099:;
    /* 00537099 */
    f->r[0]=MI_U32((0x00f56494u));
    /* 0053709e */
    f->r[2]=(0x00000000u+f->r[0]+f->r[7]*2u);
    /* 005370a1 */
    f->r[0]=(0x00000029u+f->r[7]+f->r[2]*1u);
mi_005370a5:;
    /* 005370a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005370a6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005370a7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005370a8 */
    f->r[1]=0x00f55bb0u;
    /* 005370ad */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 005370b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005370b7u;f->boundary(0x005370b2u,0x0053e590u,f);
    /* 005370b7 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 005370b8 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 005370ba */
    if(mi_signed_less(f->flags))goto mi_00537058;
    /* 005370bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005370bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_005370be:;
    /* 005370be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005370bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005370c0 */
    f->r[4]+=4;return;
}

void restore_briefing_C_004e2530(struct restore_briefing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e2530 */
    f->r[0]=MI_U32((0x00e38ebcu));
    /* 004e2535 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e2537 */
    if(!(f->flags&MI_ZF))goto mi_004e2710;
    /* 004e253d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ea8u)))&0xffu)<<0);
    /* 004e2542 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e2544 */
    if(!(f->flags&MI_ZF))goto mi_004e25cb;
    /* 004e254a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027b8u)))&0xffu)<<0);
    /* 004e254f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e2551 */
    if((f->flags&MI_ZF))goto mi_004e256b;
    /* 004e2553 */
    f->r[0]=MI_U32((0x007059d8u));
    /* 004e2558 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e255a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e256b;
    /* 004e255c */
    a=0x00000011u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e255e */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2560 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e2565u;f->boundary(0x004e2560u,0x004cc110u,f);
    /* 004e2565 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e2568 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e256a */
    f->r[4]+=4;return;
mi_004e256b:;
    /* 004e256b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027bbu)))&0xffu)<<0);
    /* 004e2570 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e2572 */
    if((f->flags&MI_ZF))goto mi_004e258c;
    /* 004e2574 */
    f->r[0]=MI_U32((0x00705880u));
    /* 004e2579 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e257b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e258c;
    /* 004e257d */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e257f */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2581 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e2586u;f->boundary(0x004e2581u,0x004cc110u,f);
    /* 004e2586 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e2589 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e258b */
    f->r[4]+=4;return;
mi_004e258c:;
    /* 004e258c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027bau)))&0xffu)<<0);
    /* 004e2591 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e2593 */
    if((f->flags&MI_ZF))goto mi_004e25a4;
    /* 004e2595 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2597 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2599 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e259eu;f->boundary(0x004e2599u,0x004cc110u,f);
    /* 004e259e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e25a1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e25a3 */
    f->r[4]+=4;return;
mi_004e25a4:;
    /* 004e25a4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027b9u)))&0xffu)<<0);
    /* 004e25a9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e25ab */
    if((f->flags&MI_ZF))goto mi_004e25bc;
    /* 004e25ad */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25af */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e25b6u;f->boundary(0x004e25b1u,0x004cc110u,f);
    /* 004e25b6 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e25b9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e25bb */
    f->r[4]+=4;return;
mi_004e25bc:;
    /* 004e25bc */
    a=0x00000015u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25be */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e25c5u;f->boundary(0x004e25c0u,0x004cc110u,f);
    /* 004e25c5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e25c8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e25ca */
    f->r[4]+=4;return;
mi_004e25cb:;
    /* 004e25cb */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004e25cd */
    if(!(f->flags&MI_ZF))goto mi_004e260e;
    /* 004e25cf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027bdu)))&0xffu)<<0);
    /* 004e25d4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e25d6 */
    if((f->flags&MI_ZF))goto mi_004e25e7;
    /* 004e25d8 */
    a=0x00000016u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25da */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e25e1u;f->boundary(0x004e25dcu,0x004cc110u,f);
    /* 004e25e1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e25e4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e25e6 */
    f->r[4]+=4;return;
mi_004e25e7:;
    /* 004e25e7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027bcu)))&0xffu)<<0);
    /* 004e25ec */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e25ee */
    if((f->flags&MI_ZF))goto mi_004e25ff;
    /* 004e25f0 */
    a=0x00000017u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25f2 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e25f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e25f9u;f->boundary(0x004e25f4u,0x004cc110u,f);
    /* 004e25f9 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e25fc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e25fe */
    f->r[4]+=4;return;
mi_004e25ff:;
    /* 004e25ff */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2601 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2603 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e2608u;f->boundary(0x004e2603u,0x004cc110u,f);
    /* 004e2608 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e260b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e260d */
    f->r[4]+=4;return;
mi_004e260e:;
    /* 004e260e */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004e2610 */
    if(!(f->flags&MI_ZF))goto mi_004e267b;
    /* 004e2612 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027beu)))&0xffu)<<0);
    /* 004e2617 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e2619 */
    if((f->flags&MI_ZF))goto mi_004e263c;
    /* 004e261b */
    f->r[0]=MI_U32((0x00705a74u));
    /* 004e2620 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e2622 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e263c;
    /* 004e2624 */
    f->r[0]=MI_U32((0x007057f4u));
    /* 004e2629 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004e262b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004e263c;
    /* 004e262d */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e262f */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2631 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e2636u;f->boundary(0x004e2631u,0x004cc110u,f);
    /* 004e2636 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e2639 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e263b */
    f->r[4]+=4;return;
mi_004e263c:;
    /* 004e263c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027c1u)))&0xffu)<<0);
    /* 004e2641 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e2643 */
    if((f->flags&MI_ZF))goto mi_004e2654;
    /* 004e2645 */
    a=0x0000001au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2647 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2649 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e264eu;f->boundary(0x004e2649u,0x004cc110u,f);
    /* 004e264e */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e2651 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e2653 */
    f->r[4]+=4;return;
mi_004e2654:;
    /* 004e2654 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027bfu)))&0xffu)<<0);
    /* 004e2659 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e265b */
    if((f->flags&MI_ZF))goto mi_004e266c;
    /* 004e265d */
    a=0x0000001bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e265f */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2661 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e2666u;f->boundary(0x004e2661u,0x004cc110u,f);
    /* 004e2666 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e2669 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e266b */
    f->r[4]+=4;return;
mi_004e266c:;
    /* 004e266c */
    a=0x0000001cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e266e */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2670 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e2675u;f->boundary(0x004e2670u,0x004cc110u,f);
    /* 004e2675 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e2678 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e267a */
    f->r[4]+=4;return;
mi_004e267b:;
    /* 004e267b */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004e267d */
    if(!(f->flags&MI_ZF))goto mi_004e269b;
    /* 004e267f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027c2u)))&0xffu)<<0);
    /* 004e2684 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e2686 */
    if(!(f->flags&MI_ZF))goto mi_004e2710;
    /* 004e268c */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e268e */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2690 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e2695u;f->boundary(0x004e2690u,0x004cc110u,f);
    /* 004e2695 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e2698 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e269a */
    f->r[4]+=4;return;
mi_004e269b:;
    /* 004e269b */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004e269d */
    if(!(f->flags&MI_ZF))goto mi_004e2710;
    /* 004e269f */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e26a4 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004e26aa */
    f->r[1]=MI_U32((0x00ea5b44u+f->r[0]));
    /* 004e26b0 */
    f->r[0]=MI_U32((0x00784c2cu));
    /* 004e26b5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004e26b7 */
    if((f->flags&MI_ZF))goto mi_004e2710;
    /* 004e26b9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027c5u)))&0xffu)<<0);
    /* 004e26be */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e26c0 */
    if((f->flags&MI_ZF))goto mi_004e26d1;
    /* 004e26c2 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e26c4 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e26c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e26cbu;f->boundary(0x004e26c6u,0x004cc110u,f);
    /* 004e26cb */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e26ce */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e26d0 */
    f->r[4]+=4;return;
mi_004e26d1:;
    /* 004e26d1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027c4u)))&0xffu)<<0);
    /* 004e26d6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e26d8 */
    if((f->flags&MI_ZF))goto mi_004e26e9;
    /* 004e26da */
    a=0x0000001du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e26dc */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e26de */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e26e3u;f->boundary(0x004e26deu,0x004cc110u,f);
    /* 004e26e3 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e26e6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e26e8 */
    f->r[4]+=4;return;
mi_004e26e9:;
    /* 004e26e9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027c3u)))&0xffu)<<0);
    /* 004e26ee */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004e26f0 */
    if((f->flags&MI_ZF))goto mi_004e2701;
    /* 004e26f2 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e26f4 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e26f6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e26fbu;f->boundary(0x004e26f6u,0x004cc110u,f);
    /* 004e26fb */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e26fe */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e2700 */
    f->r[4]+=4;return;
mi_004e2701:;
    /* 004e2701 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2703 */
    a=0x0000003eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e2705 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e270au;f->boundary(0x004e2705u,0x004cc110u,f);
    /* 004e270a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e270d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004e270f */
    f->r[4]+=4;return;
mi_004e2710:;
    /* 004e2710 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004e2712 */
    f->r[4]+=4;return;
}

void restore_briefing_C_00536820(struct restore_briefing_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00536820 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00536823 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 00536827 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536828 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00536829 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0053682b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0053682d */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0053682f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00536831 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00536835 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053698a;
    /* 0053683b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053683c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0053683d:;
    /* 0053683d */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00536841 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 00536843 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00536845 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00536847 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0053696e;
    /* 0053684d */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00536851 */
    f->r[1]=f->r[0];
    /* 00536853 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00536856 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00536858 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0053685c:;
    /* 0053685c */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00536860 */
    f->r[1]=f->r[7];
    /* 00536862 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00536865 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00536867 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0053686b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0053686d */
    MI_U32((0x00d3b5e0u))=(uint32_t)(f->r[1]);
    /* 00536873 */
    MI_U32((0x00d3b5dcu))=(uint32_t)(f->r[2]);
    /* 00536879 */
    if(!(f->flags&MI_ZF))goto mi_005368be;
    /* 0053687b */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0053687d */
    if(!(f->flags&MI_ZF))goto mi_00536890;
    /* 0053687f */
    f->r[0]=MI_U32((0x00f5683cu));
    /* 00536884 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536886 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0053688b */
    goto mi_00536949;
mi_00536890:;
    /* 00536890 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00536894 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00536895 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00536897 */
    f->r[0]=MI_U32((0x00f5683cu));
    /* 0053689c */
    if(mi_signed_less(f->flags))goto mi_005368ad;
    /* 0053689e */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005368a1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 005368a3 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 005368a8 */
    goto mi_00536949;
mi_005368ad:;
    /* 005368ad */
    f->r[0]=(0x00000001u+f->r[0]+f->r[6]*1u);
    /* 005368b1 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 005368b2 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 005368b7 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 005368b9 */
    goto mi_00536949;
mi_005368be:;
    /* 005368be */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 005368c2 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 005368c3 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 005368c5 */
    if(mi_signed_less(f->flags))goto mi_0053690b;
    /* 005368c7 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 005368c9 */
    if(!(f->flags&MI_ZF))goto mi_005368de;
    /* 005368cb */
    f->r[0]=MI_U32((0x00f5683cu));
    /* 005368d0 */
    a=f->r[0];b=0x00000084u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005368d5 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 005368d7 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 005368dc */
    goto mi_00536949;
mi_005368de:;
    /* 005368de */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 005368e2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005368e3 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 005368e5 */
    f->r[0]=MI_U32((0x00f5683cu));
    /* 005368ea */
    if(mi_signed_less(f->flags))goto mi_005368fa;
    /* 005368ec */
    a=f->r[0];b=0x0000008fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005368f1 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 005368f3 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 005368f8 */
    goto mi_00536949;
mi_005368fa:;
    /* 005368fa */
    f->r[0]=(0x00000085u+f->r[0]+f->r[6]*1u);
    /* 00536901 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00536902 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536907 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00536909 */
    goto mi_00536949;
mi_0053690b:;
    /* 0053690b */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0053690d */
    if(!(f->flags&MI_ZF))goto mi_0053691f;
    /* 0053690f */
    f->r[0]=MI_U32((0x00f5683cu));
    /* 00536914 */
    f->r[0]=(0x0000000cu+f->r[0]+f->r[5]*1u);
    /* 00536918 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 0053691d */
    goto mi_00536944;
mi_0053691f:;
    /* 0053691f */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00536923 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00536924 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00536926 */
    f->r[0]=MI_U32((0x00f5683cu));
    /* 0053692b */
    if(mi_signed_less(f->flags))goto mi_00536938;
    /* 0053692d */
    f->r[0]=(0x00000017u+f->r[0]+f->r[5]*1u);
    /* 00536931 */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
    /* 00536936 */
    goto mi_00536944;
mi_00536938:;
    /* 00536938 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0053693a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0053693b */
    f->r[0]=(0x0000000du+f->r[0]+f->r[5]*1u);
    /* 0053693f */
    MI_U32((0x00d3b5c0u))=(uint32_t)(f->r[0]);
mi_00536944:;
    /* 00536944 */
    f->r[3]=0x0000000cu;
mi_00536949:;
    /* 00536949 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053694a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053694b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0053694c */
    f->r[1]=0x00f55bb0u;
    /* 00536951 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00536956u;f->boundary(0x00536951u,0x0053e590u,f);
    /* 00536956 */
    a=f->r[6];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 00536959 */
    if(mi_signed_less(f->flags))goto mi_0053695d;
    /* 0053695b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0053695d:;
    /* 0053695d */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00536961 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00536962 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00536964 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00536968 */
    if(mi_signed_less(f->flags))goto mi_0053685c;
mi_0053696e:;
    /* 0053696e */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00536970 */
    a=f->r[5];b=0x00000078u;t=mi_alu(f,a,b,32,1);
    /* 00536973 */
    if(mi_signed_less(f->flags))goto mi_00536977;
    /* 00536975 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_00536977:;
    /* 00536977 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 0053697b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0053697c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0053697e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00536982 */
    if(mi_signed_less(f->flags))goto mi_0053683d;
    /* 00536988 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00536989 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0053698a:;
    /* 0053698a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0053698b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0053698c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0053698f */
    f->r[4]+=4;return;
}
