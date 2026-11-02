/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_input_controller.h"
#include "menu_inline_capacity.h"

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

void restore_menu_input_controller(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004d8310 */
    f->r[0]=MI_U32((0x00e38e14u));
    /* 004d8315 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004d8318 */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004d831e */
    f->r[2]=MI_U32((0x00e38e30u));
    /* 004d8324 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8325 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8326 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8327 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8328 */
    f->r[7]=0x00000002u;
    /* 004d832d */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004d832f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004d8331 */
    MI_U32((0x00e20990u))=(uint32_t)(f->r[5]);
    /* 004d8337 */
    MI_U32((0x007056b8u))=(uint32_t)(f->r[5]);
    /* 004d833d */
    MI_U32((0x007056b0u))=(uint32_t)(f->r[5]);
    /* 004d8343 */
    MI_U32((0x007056a8u))=(uint32_t)(f->r[5]);
    /* 004d8349 */
    MI_U32((0x00e38e2cu))=(uint32_t)(f->r[1]);
    /* 004d834f */
    MI_U32((0x00e38e28u))=(uint32_t)(f->r[2]);
    /* 004d8355 */
    if(!(f->flags&MI_ZF))goto mi_004d8364;
    /* 004d8357 */
    MI_U32((0x00e38e14u))=(uint32_t)(f->r[5]);
    /* 004d835d */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_004d8364:;
    /* 004d8364 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004d8369 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d836b */
    if(!(f->flags&MI_ZF))goto mi_004d83ce;
    /* 004d836d */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8372 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d8378 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8379 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d837a */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d837c */
    a=0x0057bd88u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8381 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8382 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8383 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8388u;f->boundary(0x004d8383u,0x00426450u,f);
    /* 004d8388 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d838e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8391 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8392 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8397 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8399 */
    a=0x0057bd88u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d839e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d839f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d83a5u;f->boundary(0x004d83a0u,0x004d6740u,f);
    /* 004d83a5 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d83ab */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d83b0 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d83b5 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83b7 */
    a=0x0057bd88u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83bc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83bd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d83c3u;f->boundary(0x004d83beu,0x004261d0u,f);
    /* 004d83c3 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d83c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d83c7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d83c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d83c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d83ca */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d83cd */
    f->r[4]+=4;return;
mi_004d83ce:;
    /* 004d83ce */
    a=(f->r[0]&0xffu);b=0x00000017u;t=mi_alu(f,a,b,8,1);
    /* 004d83d0 */
    if(!(f->flags&MI_ZF))goto mi_004d8493;
    /* 004d83d6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d83dbu;f->boundary(0x004d83d6u,0x0041bb30u,f);
    /* 004d83db */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d83dd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d8403;
    /* 004d83df */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d83e5 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d83eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83ec */
    a=0x0057be30u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83f1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83f2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d83f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d83f8u;f->boundary(0x004d83f3u,0x00425990u,f);
    /* 004d83f8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d83fb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d83fd */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004d8403:;
    /* 004d8403 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8408 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d840e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d840f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8410 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8412 */
    a=0x0057be78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8417 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8418 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8419 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d841eu;f->boundary(0x004d8419u,0x00426450u,f);
    /* 004d841e */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d8424 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8427 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8428 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d842d */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d842f */
    a=0x0057be78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8434 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8435 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8436 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d843bu;f->boundary(0x004d8436u,0x004d6740u,f);
    /* 004d843b */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d8441 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d8446 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d844b */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d844d */
    a=0x0057be78u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8452 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8453 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8454 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8459u;f->boundary(0x004d8454u,0x004261d0u,f);
    /* 004d8459 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d845c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d845e */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8464 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8469 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d846b */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8471 */
    a=MI_U32((0x006fdc98u));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004d8478 */
    if((f->flags&MI_ZF))goto mi_004d8486;
    /* 004d847a */
    a=MI_U32((0x007027b0u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8480 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004d8486:;
    /* 004d8486 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d848bu;f->boundary(0x004d8486u,0x004cb6a0u,f);
    /* 004d848b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d848c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d848d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d848e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d848f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8492 */
    f->r[4]+=4;return;
mi_004d8493:;
    /* 004d8493 */
    a=(f->r[0]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 004d8495 */
    if(!(f->flags&MI_ZF))goto mi_004d84a4;
    /* 004d8497 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d849cu;f->boundary(0x004d8497u,0x0041aa60u,f);
    /* 004d849c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d849d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d849e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d849f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d84a0 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d84a3 */
    f->r[4]+=4;return;
mi_004d84a4:;
    /* 004d84a4 */
    a=(f->r[0]&0xffu);b=0x0000003fu;t=mi_alu(f,a,b,8,1);
    /* 004d84a6 */
    if(!(f->flags&MI_ZF))goto mi_004d84b5;
    /* 004d84a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d84adu;f->boundary(0x004d84a8u,0x00426530u,f);
    /* 004d84ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d84ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d84af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d84b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d84b1 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d84b4 */
    f->r[4]+=4;return;
mi_004d84b5:;
    /* 004d84b5 */
    a=(f->r[0]&0xffu);b=0x00000044u;t=mi_alu(f,a,b,8,1);
    /* 004d84b7 */
    if(!(f->flags&MI_ZF))goto mi_004d8501;
    /* 004d84b9 */
    MI_U32((0x00f54118u))=(uint32_t)(f->r[7]);
    /* 004d84bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d84c4u;f->boundary(0x004d84bfu,0x004cf1c0u,f);
    /* 004d84c4 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004d84ca */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d84cc */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d84ce */
    f->r[0]=(0xfffffedcu+f->r[1]);
    /* 004d84d4 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d84d5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d84d7 */
    f->r[2]=MI_U32((0x00e38db0u));
    /* 004d84dd */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d84df */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d84e0 */
    f->r[0]=(0xfffffe0cu+f->r[2]);
    /* 004d84e6 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d84e7 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d84e9 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d84eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d84ec */
    a=0x005e1e18u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d84f1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d84f6u;f->boundary(0x004d84f1u,0x00538dc0u,f);
    /* 004d84f6 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d84f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d84fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d84fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d84fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d84fd */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8500 */
    f->r[4]+=4;return;
mi_004d8501:;
    /* 004d8501 */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,1);
    /* 004d8503 */
    if(!(f->flags&MI_ZF))goto mi_004d854b;
    /* 004d8505 */
    MI_U32((0x00f54118u))=(uint32_t)(f->r[7]);
    /* 004d850b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8510u;f->boundary(0x004d850bu,0x004cf1c0u,f);
    /* 004d8510 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004d8515 */
    f->r[1]=MI_U32((0x00e38db0u));
    /* 004d851b */
    a=f->r[0];b=0xfffffedcu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8520 */
    a=0x00000041u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8522 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d8523 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d8525 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8527 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d8529 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d852a */
    f->r[0]=(0xfffffe0cu+f->r[1]);
    /* 004d8530 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d8531 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d8533 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d8535 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8536 */
    a=0x005e1e08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d853b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8540u;f->boundary(0x004d853bu,0x00538dc0u,f);
    /* 004d8540 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8543 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8544 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8545 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8546 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8547 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d854a */
    f->r[4]+=4;return;
mi_004d854b:;
    /* 004d854b */
    a=(f->r[0]&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,1);
    /* 004d854d */
    if(!(f->flags&MI_ZF))goto mi_004d8595;
    /* 004d854f */
    MI_U32((0x00f54118u))=(uint32_t)(f->r[7]);
    /* 004d8555 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d855au;f->boundary(0x004d8555u,0x004cf1c0u,f);
    /* 004d855a */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004d8560 */
    a=0x0000003cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8562 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8564 */
    f->r[0]=(0xfffffe20u+f->r[2]);
    /* 004d856a */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d856b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d856d */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d856f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8570 */
    f->r[0]=MI_U32((0x00e38db0u));
    /* 004d8575 */
    a=f->r[0];b=0xfffffd80u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d857a */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d857b */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d857d */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d857f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8580 */
    a=0x005e1df8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8585 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d858au;f->boundary(0x004d8585u,0x00538dc0u,f);
    /* 004d858a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d858d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d858e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d858f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8590 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8591 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8594 */
    f->r[4]+=4;return;
mi_004d8595:;
    /* 004d8595 */
    a=(f->r[0]&0xffu);b=0x00000042u;t=mi_alu(f,a,b,8,1);
    /* 004d8597 */
    if(!(f->flags&MI_ZF))goto mi_004d85bc;
    /* 004d8599 */
    MI_U32((0x007028f4u))=(uint32_t)(f->r[5]);
    /* 004d859f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d85a4u;f->boundary(0x004d859fu,0x004c5a40u,f);
    /* 004d85a4 */
    a=0x000003e8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d85a9 */
    a=0x00000033u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d85ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d85ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d85b1u;f->boundary(0x004d85acu,0x0041aa20u,f);
    /* 004d85b1 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d85b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d85b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d85b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d85b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d85b8 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d85bb */
    f->r[4]+=4;return;
mi_004d85bc:;
    /* 004d85bc */
    a=(f->r[0]&0xffu);b=0x00000043u;t=mi_alu(f,a,b,8,1);
    /* 004d85be */
    if(!(f->flags&MI_ZF))goto mi_004d8653;
    /* 004d85c4 */
    a=MI_U8((0x00e38ea8u));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004d85cb */
    if(!mi_signed_less(f->flags))goto mi_004d85e3;
    /* 004d85cd */
    a=MI_U32((0x00e38ebcu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d85d3 */
    if(!(f->flags&MI_ZF))goto mi_004d85e3;
    /* 004d85d5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38ea9u)))&0xffu)<<0);
    /* 004d85da */
    f->r[6]=0x00000001u;
    /* 004d85df */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d85e1 */
    if((f->flags&MI_ZF))goto mi_004d85e8;
mi_004d85e3:;
    /* 004d85e3 */
    f->r[6]=0x00000003u;
mi_004d85e8:;
    /* 004d85e8 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d85ee */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d85f4 */
    f->r[0]=MI_U32((0x00f53db4u));
    /* 004d85f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d85fa */
    f->r[1]=MI_U32((0x00f53db8u));
    /* 004d8600 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8601 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8602 */
    a=0x005c65d0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8607 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8608 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8609 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d860eu;f->boundary(0x004d8609u,0x00426450u,f);
    /* 004d860e */
    f->r[2]=MI_U32((0x00f53db4u));
    /* 004d8614 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8617 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8618 */
    f->r[0]=MI_U32((0x00f53db8u));
    /* 004d861d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d861e */
    a=0x005c65d0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8623 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8624 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8625 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d862au;f->boundary(0x004d8625u,0x004d6740u,f);
    /* 004d862a */
    f->r[1]=MI_U32((0x00f53db4u));
    /* 004d8630 */
    f->r[2]=MI_U32((0x00f53db8u));
    /* 004d8636 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8637 */
    a=0x005c65d0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d863c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d863d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d863e */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d8643 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8648u;f->boundary(0x004d8643u,0x004261d0u,f);
    /* 004d8648 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d864b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d864c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d864d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d864e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d864f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8652 */
    f->r[4]+=4;return;
mi_004d8653:;
    /* 004d8653 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004d8655 */
    if(!(f->flags&MI_ZF))goto mi_004d8aec;
    /* 004d865b */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004d8660 */
    f->r[3]=0x00000001u;
    /* 004d8665 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004d8667 */
    MI_U32((0x00e94e14u))=(uint32_t)(f->r[5]);
    /* 004d866d */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d866f */
    if(!(f->flags&MI_ZF))goto mi_004d8699;
    /* 004d8671 */
    a=0x005c9230u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8676 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d867bu;f->boundary(0x004d8676u,0x00426100u,f);
    /* 004d867b */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d867d */
    a=0x005c9230u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8682 */
    MI_U32((0x007056a8u))=(uint32_t)(f->r[0]);
    /* 004d8687 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d868cu;f->boundary(0x004d8687u,0x00426030u,f);
    /* 004d868c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d868f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8691 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8697 */
    goto mi_004d86bf;
mi_004d8699:;
    /* 004d8699 */
    a=0x005c91f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d869e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d86a3u;f->boundary(0x004d869eu,0x00426100u,f);
    /* 004d86a3 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d86a5 */
    a=0x005c91f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d86aa */
    MI_U32((0x007056a8u))=(uint32_t)(f->r[0]);
    /* 004d86af */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d86b4u;f->boundary(0x004d86afu,0x00426030u,f);
    /* 004d86b4 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d86b7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d86b9 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004d86bf:;
    /* 004d86bf */
    a=MI_U32((0x00605a3cu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d86c5 */
    if(!(f->flags&MI_ZF))goto mi_004d86cc;
    /* 004d86c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d86ccu;f->boundary(0x004d86c7u,0x00417b40u,f);
mi_004d86cc:;
    /* 004d86cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d86d1u;f->boundary(0x004d86ccu,0x004cf660u,f);
    /* 004d86d1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d86d2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d86d7u;f->boundary(0x004d86d2u,0x004ddde0u,f);
    /* 004d86d7 */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004d86dc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d86df */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004d86e1 */
    if(!(f->flags&MI_ZF))goto mi_004d8881;
    /* 004d86e7 */
    a=MI_U32((0x00605a38u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d86ed */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d873d;
    /* 004d86ef */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d86f4 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d86fa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d86fb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d86fc */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d86fe */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8701 */
    a=0x005763d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8706 */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d8709 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d870a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d870b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8710u;f->boundary(0x004d870bu,0x00426450u,f);
    /* 004d8710 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d8716 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8719 */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d871c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d871d */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8722 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8724 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8727 */
    a=0x005763d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d872c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d872d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d872e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8733u;f->boundary(0x004d872eu,0x004d6740u,f);
    /* 004d8733 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8736 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d873b */
    goto mi_004d8756;
mi_004d873d:;
    /* 004d873d */
    f->r[2]=MI_U32((0x00f53f64u));
    /* 004d8743 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8744 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8746 */
    a=0x005c7760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d874b */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d874d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d874e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8753u;f->boundary(0x004d874eu,0x00425bc0u,f);
    /* 004d8753 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004d8756:;
    /* 004d8756 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d875bu;f->boundary(0x004d8756u,0x00406190u,f);
    /* 004d875b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d875c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8761u;f->boundary(0x004d875cu,0x004061e0u,f);
    /* 004d8761 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8764 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8766 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d876c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8771u;f->boundary(0x004d876cu,0x00425840u,f);
    /* 004d8771 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8773 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8779 */
    a=MI_U32((0x00605a38u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d877f */
    if(!(f->flags&MI_ZF))goto mi_004d878b;
    /* 004d8781 */
    f->r[6]=0x0000000fu;
    /* 004d8786 */
    goto mi_004d8852;
mi_004d878b:;
    /* 004d878b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d8790 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8792 */
    if((f->flags&MI_ZF))goto mi_004d87a9;
    /* 004d8794 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8795 */
    MI_U32((0x00605a38u))=(uint32_t)(f->r[5]);
    /* 004d879b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d879c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d879d */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d87a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d87a5 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d87a8 */
    f->r[4]+=4;return;
mi_004d87a9:;
    /* 004d87a9 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87ab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d87b0u;f->boundary(0x004d87abu,0x004cc500u,f);
    /* 004d87b0 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d87b5 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d87bb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d87be */
    f->r[6]=f->r[7];
    /* 004d87c0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87c1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87c2 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87c4 */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d87c7 */
    a=0x005763d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87cc */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d87cf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87d0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87d1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d87d6u;f->boundary(0x004d87d1u,0x00426450u,f);
    /* 004d87d6 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d87dc */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d87df */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d87e2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87e3 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d87e8 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87ea */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d87ed */
    a=0x005763d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87f2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87f3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d87f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d87f9u;f->boundary(0x004d87f4u,0x004d6740u,f);
    /* 004d87f9 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d87ff */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d8804 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d8809 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d880b */
    a=f->r[2];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d880e */
    a=0x005763d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8813 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8816 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8817 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8818 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d881du;f->boundary(0x004d8818u,0x004261d0u,f);
    /* 004d881d */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8820 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8822 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8828 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d882e */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d8834 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8835 */
    a=f->r[1];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d8838 */
    a=0x005c8eb8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d883d */
    a=f->r[2];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d8840 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8841 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8842 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8847u;f->boundary(0x004d8842u,0x00425c40u,f);
    /* 004d8847 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d884a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d884c */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004d8852:;
    /* 004d8852 */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 004d8857 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8858 */
    a=0x005c7760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d885d */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d885f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8860 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8865u;f->boundary(0x004d8860u,0x00425990u,f);
    /* 004d8865 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8868 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d886a */
    if(!(f->flags&MI_ZF))goto mi_004d8bf1;
    /* 004d8870 */
    a=MI_U32((0x00605a38u));b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004d8876 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d887c */
    goto mi_004d8a44;
mi_004d8881:;
    /* 004d8881 */
    f->r[0]=MI_U32((0x00f53f78u));
    /* 004d8886 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8888 */
    if(!(f->flags&MI_ZF))goto mi_004d89a1;
    /* 004d888e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8893u;f->boundary(0x004d888eu,0x00406190u,f);
    /* 004d8893 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8894 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8899u;f->boundary(0x004d8894u,0x004061e0u,f);
    /* 004d8899 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d889c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d889e */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d88a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d88a9u;f->boundary(0x004d88a4u,0x00425840u,f);
    /* 004d88a9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d88ab */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d88b1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d88b6u;f->boundary(0x004d88b1u,0x00406250u,f);
    /* 004d88b6 */
    a=MI_U32((0x0070569cu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d88bc */
    if((f->flags&MI_ZF))goto mi_004d88d4;
    /* 004d88be */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d88c3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d88c5 */
    if((f->flags&MI_ZF))goto mi_004d88d4;
    /* 004d88c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d88ccu;f->boundary(0x004d88c7u,0x004072c0u,f);
    /* 004d88cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d88cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d88ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d88cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d88d0 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d88d3 */
    f->r[4]+=4;return;
mi_004d88d4:;
    /* 004d88d4 */
    f->r[1]=MI_U32((0x00f53f64u));
    /* 004d88da */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88dc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88dd */
    a=0x005c7300u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88e2 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88e4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88e5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d88eau;f->boundary(0x004d88e5u,0x00425bc0u,f);
    /* 004d88ea */
    f->r[2]=MI_U32((0x00f53f64u));
    /* 004d88f0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88f1 */
    a=0x005c7300u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88f6 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88f8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d88f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d88feu;f->boundary(0x004d88f9u,0x00425990u,f);
    /* 004d88fe */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8901 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8903 */
    if(!(f->flags&MI_ZF))goto mi_004d8bf1;
    /* 004d8909 */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 004d890e */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8910 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8912 */
    a=0x005c7370u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8917 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8919 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d891a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d891fu;f->boundary(0x004d891au,0x00425bc0u,f);
    /* 004d891f */
    f->r[1]=MI_U32((0x00f53f64u));
    /* 004d8925 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8926 */
    a=0x005c6cb8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d892b */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d892d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d892e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8933u;f->boundary(0x004d892eu,0x004261d0u,f);
    /* 004d8933 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8936 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8938 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d893e */
    f->r[2]=MI_U32((0x00f53f64u));
    /* 004d8944 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8946 */
    a=0x005c7370u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d894b */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d894d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d894e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8953u;f->boundary(0x004d894eu,0x00425990u,f);
    /* 004d8953 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8956 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8958 */
    if(!(f->flags&MI_ZF))goto mi_004d8bf1;
    /* 004d895e */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 004d8963 */
    a=0x00000032u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8965 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8966 */
    a=0x005c7980u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d896b */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d896d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d896e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8973u;f->boundary(0x004d896eu,0x00425bc0u,f);
    /* 004d8973 */
    f->r[1]=MI_U32((0x00f53f64u));
    /* 004d8979 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d897a */
    a=0x005c7980u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d897f */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8981 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8982 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8987u;f->boundary(0x004d8982u,0x00425990u,f);
    /* 004d8987 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d898a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d898c */
    if((f->flags&MI_ZF))goto mi_004d8a44;
    /* 004d8992 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8993 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8994 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8995 */
    MI_U8((0x00e38dc9u))=(uint8_t)(0x00000001u);
    /* 004d899c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d899d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d89a0 */
    f->r[4]+=4;return;
mi_004d89a1:;
    /* 004d89a1 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004d89a3 */
    if(!(f->flags&MI_ZF))goto mi_004d8a44;
    /* 004d89a9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d89aeu;f->boundary(0x004d89a9u,0x00425840u,f);
    /* 004d89ae */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d89b0 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d89b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d89bbu;f->boundary(0x004d89b6u,0x00406250u,f);
    /* 004d89bb */
    a=MI_U32((0x0070569cu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d89c1 */
    if((f->flags&MI_ZF))goto mi_004d89d9;
    /* 004d89c3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d89c8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d89ca */
    if((f->flags&MI_ZF))goto mi_004d89d9;
    /* 004d89cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d89d1u;f->boundary(0x004d89ccu,0x004072c0u,f);
    /* 004d89d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d89d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d89d3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d89d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d89d5 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d89d8 */
    f->r[4]+=4;return;
mi_004d89d9:;
    /* 004d89d9 */
    f->r[2]=MI_U32((0x00f53f60u));
    /* 004d89df */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89e1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89e2 */
    a=0x005c7348u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89e7 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89e9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d89efu;f->boundary(0x004d89eau,0x00425bc0u,f);
    /* 004d89ef */
    f->r[0]=MI_U32((0x00f53f60u));
    /* 004d89f4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89f5 */
    a=0x005c7348u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89fa */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89fc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d89fd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a02u;f->boundary(0x004d89fdu,0x00425990u,f);
    /* 004d8a02 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8a05 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8a07 */
    if(!(f->flags&MI_ZF))goto mi_004d8bf1;
    /* 004d8a0d */
    f->r[1]=MI_U32((0x00f53f60u));
    /* 004d8a13 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a15 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a17 */
    a=0x005c75b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a1c */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a1e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a24u;f->boundary(0x004d8a1fu,0x00425bc0u,f);
    /* 004d8a24 */
    f->r[2]=MI_U32((0x00f53f60u));
    /* 004d8a2a */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a2c */
    a=0x005c75b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a31 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a33 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8a34 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a39u;f->boundary(0x004d8a34u,0x00425990u,f);
    /* 004d8a39 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8a3c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8a3e */
    if(!(f->flags&MI_ZF))goto mi_004d8bf1;
mi_004d8a44:;
    /* 004d8a44 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a49u;f->boundary(0x004d8a44u,0x00405f80u,f);
    /* 004d8a49 */
    f->r[0]=MI_U32((0x00bca2c4u));
    /* 004d8a4e */
    MI_U32((0x00605994u))=(uint32_t)(f->r[5]);
    /* 004d8a54 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8a56 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8a5c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004d8a61 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8a63 */
    if((f->flags&MI_ZF))goto mi_004d8a99;
    /* 004d8a65 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a6au;f->boundary(0x004d8a65u,0x0040b630u,f);
    /* 004d8a6a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8a6c */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8a72 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a77u;f->boundary(0x004d8a72u,0x0040b790u,f);
    /* 004d8a77 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8a79 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8a7f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a84u;f->boundary(0x004d8a7fu,0x0040b6d0u,f);
    /* 004d8a84 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8a86 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8a8c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8a91u;f->boundary(0x004d8a8cu,0x00405c20u,f);
    /* 004d8a91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8a92 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8a93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8a94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8a95 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8a98 */
    f->r[4]+=4;return;
mi_004d8a99:;
    /* 004d8a99 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e00u)))&0xffu)<<0);
    /* 004d8a9e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8aa0 */
    if((f->flags&MI_ZF))goto mi_004d8aaf;
    /* 004d8aa2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8aa7u;f->boundary(0x004d8aa2u,0x00405d90u,f);
    /* 004d8aa7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8aa8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8aa9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8aaa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8aab */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8aae */
    f->r[4]+=4;return;
mi_004d8aaf:;
    /* 004d8aaf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e04u)))&0xffu)<<0);
    /* 004d8ab4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8ab6 */
    if((f->flags&MI_ZF))goto mi_004d8ac5;
    /* 004d8ab8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8abdu;f->boundary(0x004d8ab8u,0x00405e90u,f);
    /* 004d8abd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8abe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8abf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8ac0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8ac1 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8ac4 */
    f->r[4]+=4;return;
mi_004d8ac5:;
    /* 004d8ac5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8aca */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8acc */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8ad2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8ad7u;f->boundary(0x004d8ad2u,0x00405ba0u,f);
    /* 004d8ad7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8ad9 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8adf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8ae4u;f->boundary(0x004d8adfu,0x004f7e50u,f);
    /* 004d8ae4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8ae5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8ae6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8ae7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8ae8 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8aeb */
    f->r[4]+=4;return;
mi_004d8aec:;
    /* 004d8aec */
    a=(f->r[0]&0xffu);b=0x00000012u;t=mi_alu(f,a,b,8,1);
    /* 004d8aee */
    if(!(f->flags&MI_ZF))goto mi_004d8b28;
    /* 004d8af0 */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004d8af7 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8afd */
    a=MI_U32((0x00f53f78u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8b03 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8b09 */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 004d8b0e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b10 */
    a=0x005c6cb8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b15 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b17 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b18 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8b1du;f->boundary(0x004d8b18u,0x004261d0u,f);
    /* 004d8b1d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8b20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8b21 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8b22 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8b23 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8b24 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8b27 */
    f->r[4]+=4;return;
mi_004d8b28:;
    /* 004d8b28 */
    a=(f->r[0]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 004d8b2a */
    if(!(f->flags&MI_ZF))goto mi_004d8c00;
    /* 004d8b30 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8b35 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8b37 */
    if((f->flags&MI_ZF))goto mi_004d8b4f;
    /* 004d8b39 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8b3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8b3b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8b3c */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d8b43 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d8b4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8b4b */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8b4e */
    f->r[4]+=4;return;
mi_004d8b4f:;
    /* 004d8b4f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8b54u;f->boundary(0x004d8b4fu,0x00403e00u,f);
    /* 004d8b54 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8b56 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8b5c */
    a=MI_U32((0x00605a3cu));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004d8b63 */
    if(!(f->flags&MI_ZF))goto mi_004d8b8e;
    /* 004d8b65 */
    f->r[0]=MI_U32((0x00605a38u));
    /* 004d8b6a */
    f->r[1]=MI_U32((0x00f53f64u));
    /* 004d8b70 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004d8b72 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004d8b74 */
    a=(f->r[0]&0xffu);b=0x000000f3u;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004d8b76 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8b79 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b7a */
    a=0x005c7760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b7f */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b81 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8b87u;f->boundary(0x004d8b82u,0x00425990u,f);
    /* 004d8b87 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8b8a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8b8c */
    if(!(f->flags&MI_ZF))goto mi_004d8bf1;
mi_004d8b8e:;
    /* 004d8b8e */
    f->r[0]=MI_U32((0x00f53f78u));
    /* 004d8b93 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8b95 */
    if(!(f->flags&MI_ZF))goto mi_004d8bc8;
    /* 004d8b97 */
    f->r[2]=MI_U32((0x00f53f64u));
    /* 004d8b9d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8b9f */
    a=0x005c6cb8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ba4 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ba6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ba7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8bacu;f->boundary(0x004d8ba7u,0x004261d0u,f);
    /* 004d8bac */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8baf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8bb1 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8bb7 */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 004d8bbc */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8bbe */
    a=0x005c7370u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8bc3 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8bc5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8bc6 */
    goto mi_004d8be1;
mi_004d8bc8:;
    /* 004d8bc8 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004d8bcb */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8bd1 */
    f->r[1]=MI_U32((0x00f53f60u));
    /* 004d8bd7 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8bd9 */
    a=0x005c75b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8bde */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8be0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004d8be1:;
    /* 004d8be1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8be6u;f->boundary(0x004d8be1u,0x00425990u,f);
    /* 004d8be6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8be9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8beb */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
mi_004d8bf1:;
    /* 004d8bf1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8bf2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8bf3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8bf4 */
    MI_U8((0x00e38dc9u))=(uint8_t)(0x00000001u);
    /* 004d8bfb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8bfc */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8bff */
    f->r[4]+=4;return;
mi_004d8c00:;
    /* 004d8c00 */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,1);
    /* 004d8c02 */
    if(!(f->flags&MI_ZF))goto mi_004d8d70;
    /* 004d8c08 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d8c0d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8c0f */
    if((f->flags&MI_ZF))goto mi_004d8c27;
    /* 004d8c11 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8c12 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8c13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8c14 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d8c1b */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d8c22 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8c23 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8c26 */
    f->r[4]+=4;return;
mi_004d8c27:;
    /* 004d8c27 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d8c2d */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d8c32 */
    f->r[1]=MI_U32((0x007056e8u));
    /* 004d8c38 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c39 */
    f->r[2]=MI_U32((0x00f53f64u));
    /* 004d8c3f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c40 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c42 */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d8c45 */
    a=0x005c2bf0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c4a */
    a=f->r[2];b=0xffffff56u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d8c50 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c51 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8c57u;f->boundary(0x004d8c52u,0x00426450u,f);
    /* 004d8c57 */
    f->r[1]=MI_U32((0x00f53f64u));
    /* 004d8c5d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8c60 */
    a=f->r[1];b=0xffffff56u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d8c66 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c67 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 004d8c6c */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c6e */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8c71 */
    a=0x005c2bf0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c76 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c77 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c78 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8c7du;f->boundary(0x004d8c78u,0x004d6740u,f);
    /* 004d8c7d */
    f->r[2]=MI_U32((0x007056e8u));
    /* 004d8c83 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d8c88 */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 004d8c8d */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c8f */
    a=f->r[2];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d8c92 */
    a=0x005c2bf0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c97 */
    a=f->r[0];b=0xffffff56u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8c9c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8c9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8ca3u;f->boundary(0x004d8c9eu,0x004261d0u,f);
    /* 004d8ca3 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8ca6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8ca8 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8cae */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8cb3u;f->boundary(0x004d8caeu,0x00409e40u,f);
    /* 004d8cb3 */
    a=MI_U32((0x007056dcu));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8cb9 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8cbf */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d8cc5 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d8ccb */
    f->r[0]=MI_U32((0x007056d8u));
    /* 004d8cd0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8cd1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8cd2 */
    f->r[2]=MI_U32((0x007056e8u));
    /* 004d8cd8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8cd9 */
    f->r[0]=MI_U32((0x007056d0u));
    /* 004d8cde */
    a=0x005c2d08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ce3 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004d8ce6 */
    f->r[0]=(0x00000020u+f->r[2]+f->r[1]*8u);
    /* 004d8cea */
    f->r[1]=MI_U32((0x00f53f64u));
    /* 004d8cf0 */
    a=f->r[1];b=0xfffffea4u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d8cf6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8cf7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8cf8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8cfdu;f->boundary(0x004d8cf8u,0x00426450u,f);
    /* 004d8cfd */
    f->r[2]=MI_U32((0x007056d8u));
    /* 004d8d03 */
    f->r[1]=MI_U32((0x007056e8u));
    /* 004d8d09 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8d0c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d0d */
    f->r[0]=MI_U32((0x007056d0u));
    /* 004d8d12 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d13 */
    a=0x005c2d08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d18 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004d8d1b */
    f->r[2]=(0x00000020u+f->r[1]+f->r[0]*8u);
    /* 004d8d1f */
    f->r[0]=MI_U32((0x00f53f64u));
    /* 004d8d24 */
    a=f->r[0];b=0xfffffea4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d8d29 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d2a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d2b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8d30u;f->boundary(0x004d8d2bu,0x004d6740u,f);
    /* 004d8d30 */
    f->r[1]=MI_U32((0x007056d8u));
    /* 004d8d36 */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004d8d3b */
    f->r[0]=MI_U32((0x007056d0u));
    /* 004d8d40 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d41 */
    a=0x005c2d08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d46 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004d8d49 */
    f->r[0]=MI_U32((0x007056e8u));
    /* 004d8d4e */
    f->r[1]=(0x00000020u+f->r[0]+f->r[2]*8u);
    /* 004d8d52 */
    f->r[2]=MI_U32((0x00f53f64u));
    /* 004d8d58 */
    a=f->r[2];b=0xfffffea4u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d8d5e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d5f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8d60 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8d65u;f->boundary(0x004d8d60u,0x004261d0u,f);
    /* 004d8d65 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8d68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8d69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8d6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8d6b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8d6c */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8d6f */
    f->r[4]+=4;return;
mi_004d8d70:;
    /* 004d8d70 */
    a=(f->r[0]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004d8d72 */
    if(!(f->flags&MI_ZF))goto mi_004d8d81;
    /* 004d8d74 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8d79u;f->boundary(0x004d8d74u,0x004c7770u,f);
    /* 004d8d79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8d7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8d7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8d7c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8d7d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8d80 */
    f->r[4]+=4;return;
mi_004d8d81:;
    /* 004d8d81 */
    a=(f->r[0]&0xffu);b=0x00000013u;t=mi_alu(f,a,b,8,1);
    /* 004d8d83 */
    if(!(f->flags&MI_ZF))goto mi_004d8dc2;
    /* 004d8d85 */
    a=MI_U32((0x00d3b180u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8d8b */
    if((f->flags&MI_ZF))goto mi_004d8d9a;
    /* 004d8d8d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8d92u;f->boundary(0x004d8d8du,0x004c7770u,f);
    /* 004d8d92 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8d93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8d94 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8d95 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8d96 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8d99 */
    f->r[4]+=4;return;
mi_004d8d9a:;
    /* 004d8d9a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8d9f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8da1 */
    if((f->flags&MI_ZF))goto mi_004d8da8;
    /* 004d8da3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8da8u;f->boundary(0x004d8da3u,0x00539170u,f);
mi_004d8da8:;
    /* 004d8da8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e04u)))&0xffu)<<0);
    /* 004d8dad */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8daf */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8db5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8dbau;f->boundary(0x004d8db5u,0x00539170u,f);
    /* 004d8dba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8dbb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8dbc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8dbd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8dbe */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8dc1 */
    f->r[4]+=4;return;
mi_004d8dc2:;
    /* 004d8dc2 */
    a=(f->r[0]&0xffu);b=0x0000004au;t=mi_alu(f,a,b,8,1);
    /* 004d8dc4 */
    if(!(f->flags&MI_ZF))goto mi_004d8e19;
    /* 004d8dc6 */
    a=MI_U32((0x00d3b180u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8dcc */
    if((f->flags&MI_ZF))goto mi_004d8ddb;
    /* 004d8dce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8dd3u;f->boundary(0x004d8dceu,0x004c7770u,f);
    /* 004d8dd3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8dd4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8dd5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8dd6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8dd7 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8dda */
    f->r[4]+=4;return;
mi_004d8ddb:;
    /* 004d8ddb */
    f->r[1]=MI_U32((0x006fdc54u));
    /* 004d8de1 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d8de3 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8de9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8dee */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8df0 */
    if((f->flags&MI_ZF))goto mi_004d8dff;
    /* 004d8df2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8df7u;f->boundary(0x004d8df2u,0x00413420u,f);
    /* 004d8df7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8df8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8df9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8dfa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8dfb */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8dfe */
    f->r[4]+=4;return;
mi_004d8dff:;
    /* 004d8dff */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e04u)))&0xffu)<<0);
    /* 004d8e04 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8e06 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8e0c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8e11u;f->boundary(0x004d8e0cu,0x00413420u,f);
    /* 004d8e11 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8e12 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8e13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8e14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8e15 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8e18 */
    f->r[4]+=4;return;
mi_004d8e19:;
    /* 004d8e19 */
    a=(f->r[0]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004d8e1b */
    if(!(f->flags&MI_ZF))goto mi_004d8e2a;
    /* 004d8e1d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8e22u;f->boundary(0x004d8e1du,0x004f96e0u,f);
    /* 004d8e22 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8e23 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8e24 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8e25 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8e26 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8e29 */
    f->r[4]+=4;return;
mi_004d8e2a:;
    /* 004d8e2a */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004d8e2c */
    if(!(f->flags&MI_ZF))goto mi_004d8e80;
    /* 004d8e2e */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004d8e33 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8e35 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004d8e38 */
    if(!(f->flags&MI_ZF))goto mi_004d8e51;
    /* 004d8e3a */
    a=0x005c9230u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8e3f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8e44u;f->boundary(0x004d8e3fu,0x00425f20u,f);
    /* 004d8e44 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8e47 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8e49 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8e4f */
    goto mi_004d8e66;
mi_004d8e51:;
    /* 004d8e51 */
    a=0x005c91f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8e56 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8e5bu;f->boundary(0x004d8e56u,0x00425f20u,f);
    /* 004d8e5b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8e5e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d8e60 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004d8e66:;
    /* 004d8e66 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8e6b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8e6d */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d8e73 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8e78u;f->boundary(0x004d8e73u,0x00426090u,f);
    /* 004d8e78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8e79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8e7a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8e7b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8e7c */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8e7f */
    f->r[4]+=4;return;
mi_004d8e80:;
    /* 004d8e80 */
    a=(f->r[0]&0xffu);b=0x00000011u;t=mi_alu(f,a,b,8,1);
    /* 004d8e82 */
    if(!(f->flags&MI_ZF))goto mi_004d8e91;
    /* 004d8e84 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8e89u;f->boundary(0x004d8e84u,0x004e1e10u,f);
    /* 004d8e89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8e8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8e8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8e8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8e8d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8e90 */
    f->r[4]+=4;return;
mi_004d8e91:;
    /* 004d8e91 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004d8e93 */
    if(!(f->flags&MI_ZF))goto mi_004d8eab;
    /* 004d8e95 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8e9au;f->boundary(0x004d8e95u,0x004cf660u,f);
    /* 004d8e9a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8e9b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8ea0u;f->boundary(0x004d8e9bu,0x004ddde0u,f);
    /* 004d8ea0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8ea3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8ea4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8ea5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8ea6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8ea7 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8eaa */
    f->r[4]+=4;return;
mi_004d8eab:;
    /* 004d8eab */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004d8ead */
    if(!(f->flags&MI_ZF))goto mi_004d8f33;
    /* 004d8eb3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8eb8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8eba */
    if((f->flags&MI_ZF))goto mi_004d8ed2;
    /* 004d8ebc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8ebd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8ebe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8ebf */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d8ec6 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d8ecd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8ece */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8ed1 */
    f->r[4]+=4;return;
mi_004d8ed2:;
    /* 004d8ed2 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8ed7 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d8edd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ede */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8edf */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ee1 */
    a=0x005c2e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ee6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ee7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ee8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8eedu;f->boundary(0x004d8ee8u,0x00426450u,f);
    /* 004d8eed */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d8ef3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8ef6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8ef7 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8efc */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8efe */
    a=0x005c2e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f03 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f04 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8f0au;f->boundary(0x004d8f05u,0x004d6740u,f);
    /* 004d8f0a */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d8f10 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d8f15 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d8f1a */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f1c */
    a=0x005c2e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f21 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f22 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8f28u;f->boundary(0x004d8f23u,0x004261d0u,f);
    /* 004d8f28 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8f2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8f2c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8f2d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8f2e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8f2f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8f32 */
    f->r[4]+=4;return;
mi_004d8f33:;
    /* 004d8f33 */
    a=(f->r[0]&0xffu);b=0x00000056u;t=mi_alu(f,a,b,8,1);
    /* 004d8f35 */
    if(!(f->flags&MI_ZF))goto mi_004d8fbc;
    /* 004d8f3b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8f40 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8f42 */
    if((f->flags&MI_ZF))goto mi_004d8f5a;
    /* 004d8f44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8f45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8f46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8f47 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d8f4e */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d8f55 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8f56 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8f59 */
    f->r[4]+=4;return;
mi_004d8f5a:;
    /* 004d8f5a */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8f5f */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d8f65 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f66 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f67 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f69 */
    a=0x005c2eb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f6e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f6f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8f75u;f->boundary(0x004d8f70u,0x00426450u,f);
    /* 004d8f75 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d8f7b */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d8f81 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8f84 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f85 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f87 */
    a=0x005c2eb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f8c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f8d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8f8e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8f93u;f->boundary(0x004d8f8eu,0x004d6740u,f);
    /* 004d8f93 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d8f99 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d8f9e */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d8fa3 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8fa5 */
    a=0x005c2eb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8faa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8fab */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d8fac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d8fb1u;f->boundary(0x004d8facu,0x004261d0u,f);
    /* 004d8fb1 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8fb4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8fb5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8fb6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d8fb7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d8fb8 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d8fbb */
    f->r[4]+=4;return;
mi_004d8fbc:;
    /* 004d8fbc */
    a=(f->r[0]&0xffu);b=0x00000045u;t=mi_alu(f,a,b,8,1);
    /* 004d8fbe */
    if(!(f->flags&MI_ZF))goto mi_004d9033;
    /* 004d8fc0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d8fc5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d8fc7 */
    if((f->flags&MI_ZF))goto mi_004d9007;
    /* 004d8fc9 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004d8fcf */
    f->r[1]=MI_U32((0x00e39080u));
    /* 004d8fd5 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004d8fdb */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d8fe2 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d8fe9 */
    f->r[0]=(0x00ea6018u+f->r[2]);
    /* 004d8fef */
    f->r[2]=MI_U32((0x00ea6018u+f->r[2]));
    /* 004d8ff5 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004d8ff7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dd8c3;
    /* 004d8ffd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d8ffe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d8fff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9000 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004d9002 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9003 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9006 */
    f->r[4]+=4;return;
mi_004d9007:;
    /* 004d9007 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d900c */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9012 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9013 */
    a=f->r[0];b=0x00000070u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9016 */
    a=0x005c8ee8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d901b */
    a=f->r[1];b=0x00000120u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9021 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9022 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9023 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9028u;f->boundary(0x004d9023u,0x00425c40u,f);
    /* 004d9028 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d902b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d902c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d902d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d902e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d902f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9032 */
    f->r[4]+=4;return;
mi_004d9033:;
    /* 004d9033 */
    a=(f->r[0]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004d9035 */
    if(!(f->flags&MI_ZF))goto mi_004d90e9;
    /* 004d903b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d9040 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9042 */
    if((f->flags&MI_ZF))goto mi_004d905a;
    /* 004d9044 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9045 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9046 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9047 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d904e */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d9055 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9056 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9059 */
    f->r[4]+=4;return;
mi_004d905a:;
    /* 004d905a */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d905f */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9065 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9066 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9067 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9069 */
    a=0x005c3070u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d906e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d906f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9070 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9075u;f->boundary(0x004d9070u,0x00426450u,f);
    /* 004d9075 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d907b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d907e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d907f */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9084 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9086 */
    a=0x005c3070u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d908b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d908c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d908d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9092u;f->boundary(0x004d908du,0x004d6740u,f);
    /* 004d9092 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d9098 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d909e */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90a0 */
    a=0x005c3070u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90a5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90a6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90a7 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d90ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d90b1u;f->boundary(0x004d90acu,0x004261d0u,f);
    /* 004d90b1 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d90b4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d90b6 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d90bc */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d90c1 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d90c7 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90c9 */
    a=f->r[0];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d90cc */
    a=0x005c8df8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90d1 */
    a=f->r[1];b=0x000000d0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d90d7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90d8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d90d9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d90deu;f->boundary(0x004d90d9u,0x00425c40u,f);
    /* 004d90de */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d90e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d90e2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d90e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d90e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d90e5 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d90e8 */
    f->r[4]+=4;return;
mi_004d90e9:;
    /* 004d90e9 */
    a=(f->r[0]&0xffu);b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004d90eb */
    if(!(f->flags&MI_ZF))goto mi_004d919c;
    /* 004d90f1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d90f6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d90f8 */
    if((f->flags&MI_ZF))goto mi_004d9110;
    /* 004d90fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d90fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d90fc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d90fd */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d9104 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d910b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d910c */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d910f */
    f->r[4]+=4;return;
mi_004d9110:;
    /* 004d9110 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9115 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d911b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d911c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d911d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d911e */
    a=0x005c3038u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9123 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9124 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9125 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d912au;f->boundary(0x004d9125u,0x00426450u,f);
    /* 004d912a */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9130 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9133 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9134 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9139 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d913a */
    a=0x005c3038u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d913f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9140 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9141 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9146u;f->boundary(0x004d9141u,0x004d6740u,f);
    /* 004d9146 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d914c */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d9152 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9153 */
    a=0x005c3038u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9158 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9159 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d915a */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d915f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9164u;f->boundary(0x004d915fu,0x004261d0u,f);
    /* 004d9164 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9167 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9169 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d916f */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9174 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d917a */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d917c */
    a=f->r[0];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d917f */
    a=0x005c8d98u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9184 */
    a=f->r[1];b=0x00000080u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d918a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d918b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d918c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9191u;f->boundary(0x004d918cu,0x00425c40u,f);
    /* 004d9191 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9194 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9195 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9196 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9197 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9198 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d919b */
    f->r[4]+=4;return;
mi_004d919c:;
    /* 004d919c */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 004d919e */
    if(!(f->flags&MI_ZF))goto mi_004d92a4;
    /* 004d91a4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d91a5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d91aau;f->boundary(0x004d91a5u,0x004cc500u,f);
    /* 004d91aa */
    f->r[0]=MI_U32((0x00e20998u));
    /* 004d91af */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d91b2 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d91b4 */
    if((f->flags&MI_ZF))goto mi_004d91cb;
    /* 004d91b6 */
    MI_U32((0x00e20998u))=(uint32_t)(f->r[5]);
    /* 004d91bc */
    MI_U32((0x00d3b198u))=(uint32_t)(0x00000001u);
    /* 004d91c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d91cbu;f->boundary(0x004d91c6u,0x00402720u,f);
mi_004d91cb:;
    /* 004d91cb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d91d0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d91d2 */
    if((f->flags&MI_ZF))goto mi_004d91f6;
    /* 004d91d4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00d3b1bcu)))&0xffu)<<0);
    /* 004d91da */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d91db */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d91dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d91dd */
    MI_U8((0x00e38df8u))=(uint8_t)(0x00000000u);
    /* 004d91e4 */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[2]&0xffu));
    /* 004d91ea */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d91f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d91f2 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d91f5 */
    f->r[4]+=4;return;
mi_004d91f6:;
    /* 004d91f6 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d91fb */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9201 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9202 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9203 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9205 */
    a=0x005c3118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d920a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d920b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d920c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9211u;f->boundary(0x004d920cu,0x00426450u,f);
    /* 004d9211 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9217 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d921a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d921b */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9220 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9222 */
    a=0x005c3118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9227 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9228 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9229 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d922eu;f->boundary(0x004d9229u,0x004d6740u,f);
    /* 004d922e */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9234 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d9239 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d923e */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9240 */
    a=0x005c3118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9245 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9246 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9247 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d924cu;f->boundary(0x004d9247u,0x004261d0u,f);
    /* 004d924c */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d924f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9251 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9257 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d925d */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d9263 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9265 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9267 */
    a=0x005c79f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d926c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d926d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d926e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9273u;f->boundary(0x004d926eu,0x00425bc0u,f);
    /* 004d9273 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9278 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d927e */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9280 */
    a=0x005c79f0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9285 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9286 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004d9287:;
    /* 004d9287 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d928cu;f->boundary(0x004d9287u,0x00425990u,f);
    /* 004d928c */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d928f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9291 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9297 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d929cu;f->boundary(0x004d9297u,0x004029d0u,f);
    /* 004d929c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d929d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d929e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d929f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d92a0 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d92a3 */
    f->r[4]+=4;return;
mi_004d92a4:;
    /* 004d92a4 */
    a=(f->r[0]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 004d92a6 */
    if(!(f->flags&MI_ZF))goto mi_004d9389;
    /* 004d92ac */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d92ae */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d92b3u;f->boundary(0x004d92aeu,0x004cc500u,f);
    /* 004d92b3 */
    f->r[0]=MI_U32((0x00e20998u));
    /* 004d92b8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d92bb */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d92bd */
    if((f->flags&MI_ZF))goto mi_004d92d4;
    /* 004d92bf */
    MI_U32((0x00e20998u))=(uint32_t)(f->r[5]);
    /* 004d92c5 */
    MI_U32((0x00d3b198u))=(uint32_t)(0x00000001u);
    /* 004d92cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d92d4u;f->boundary(0x004d92cfu,0x004099b0u,f);
mi_004d92d4:;
    /* 004d92d4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d92d9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d92db */
    if((f->flags&MI_ZF))goto mi_004d92f3;
    /* 004d92dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d92de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d92df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d92e0 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d92e7 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d92ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d92ef */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d92f2 */
    f->r[4]+=4;return;
mi_004d92f3:;
    /* 004d92f3 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d92f8 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d92fe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d92ff */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9300 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9302 */
    a=0x005c3118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9307 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9308 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9309 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d930eu;f->boundary(0x004d9309u,0x00426450u,f);
    /* 004d930e */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9314 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9317 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9318 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d931d */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d931f */
    a=0x005c3118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9324 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9325 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9326 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d932bu;f->boundary(0x004d9326u,0x004d6740u,f);
    /* 004d932b */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d9331 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d9337 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9339 */
    a=0x005c3118u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d933e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d933f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9340 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d9345 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d934au;f->boundary(0x004d9345u,0x004261d0u,f);
    /* 004d934a */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d934d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d934f */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9355 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d935a */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9360 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9362 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9364 */
    a=0x005c7a80u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9369 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d936a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d936b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9370u;f->boundary(0x004d936bu,0x00425bc0u,f);
    /* 004d9370 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9376 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d937b */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d937d */
    a=0x005c7a80u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9382 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9383 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9384 */
    goto mi_004d9287;
mi_004d9389:;
    /* 004d9389 */
    a=(f->r[0]&0xffu);b=0x00000016u;t=mi_alu(f,a,b,8,1);
    /* 004d938b */
    if(!(f->flags&MI_ZF))goto mi_004d942a;
    /* 004d9391 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d9396 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9398 */
    if((f->flags&MI_ZF))goto mi_004d93b0;
    /* 004d939a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d939b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d939c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d939d */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000025u);
    /* 004d93a4 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d93ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d93ac */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d93af */
    f->r[4]+=4;return;
mi_004d93b0:;
    /* 004d93b0 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d93b5 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d93bb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93bc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93bd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93be */
    a=f->r[0];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d93c1 */
    a=0x005c3368u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93c6 */
    a=f->r[1];b=0x000000a0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d93cc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93cd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93ce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d93d3u;f->boundary(0x004d93ceu,0x00426450u,f);
    /* 004d93d3 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d93d9 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d93df */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d93e2 */
    a=f->r[1];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d93e5 */
    a=f->r[2];b=0x000000a0u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d93eb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93ec */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93ed */
    a=0x005c3368u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93f2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93f3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d93f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d93f9u;f->boundary(0x004d93f4u,0x004d6740u,f);
    /* 004d93f9 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d93ff */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d9404 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9409 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d940a */
    a=f->r[0];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d940d */
    a=0x005c3368u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9412 */
    a=f->r[1];b=0x000000a0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9418 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9419 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d941a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d941fu;f->boundary(0x004d941au,0x004261d0u,f);
    /* 004d941f */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9422 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9423 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9424 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9425 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9426 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9429 */
    f->r[4]+=4;return;
mi_004d942a:;
    /* 004d942a */
    a=(f->r[0]&0xffu);b=0x00000019u;t=mi_alu(f,a,b,8,1);
    /* 004d942c */
    if(!(f->flags&MI_ZF))goto mi_004d94bc;
    /* 004d9432 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d9437 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9439 */
    if((f->flags&MI_ZF))goto mi_004d9451;
    /* 004d943b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d943c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d943d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d943e */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d9445 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d944c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d944d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9450 */
    f->r[4]+=4;return;
mi_004d9451:;
    /* 004d9451 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9456 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d945c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d945d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d945e */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9460 */
    a=f->r[0];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9463 */
    a=0x005c5e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9468 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9469 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d946a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d946fu;f->boundary(0x004d946au,0x00426450u,f);
    /* 004d946f */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9475 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9478 */
    a=f->r[2];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d947b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d947c */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9481 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9483 */
    a=0x005c5e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9488 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9489 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d948a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d948fu;f->boundary(0x004d948au,0x004d6740u,f);
    /* 004d948f */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d9495 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d949b */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d949d */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d94a0 */
    a=0x005c5e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94a5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94a6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94a7 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d94ac */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d94b1u;f->boundary(0x004d94acu,0x004261d0u,f);
    /* 004d94b1 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d94b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d94b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d94b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d94b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d94b8 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d94bb */
    f->r[4]+=4;return;
mi_004d94bc:;
    /* 004d94bc */
    a=(f->r[0]&0xffu);b=0x00000018u;t=mi_alu(f,a,b,8,1);
    /* 004d94be */
    if(!(f->flags&MI_ZF))goto mi_004d9537;
    /* 004d94c0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d94c5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d94c7 */
    if((f->flags&MI_ZF))goto mi_004d94d6;
    /* 004d94c9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d94ceu;f->boundary(0x004d94c9u,0x00448cf0u,f);
    /* 004d94ce */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d94cf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d94d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d94d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d94d2 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d94d5 */
    f->r[4]+=4;return;
mi_004d94d6:;
    /* 004d94d6 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d94db */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d94e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94e2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94e3 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94e5 */
    a=0x005d9e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94ea */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94eb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d94f1u;f->boundary(0x004d94ecu,0x00426450u,f);
    /* 004d94f1 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d94f7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d94fa */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d94fb */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9500 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9502 */
    a=0x005d9e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9507 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9508 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9509 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d950eu;f->boundary(0x004d9509u,0x004d6740u,f);
    /* 004d950e */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9514 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d9519 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d951e */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9520 */
    a=0x005d9e20u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9525 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9526 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9527 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d952cu;f->boundary(0x004d9527u,0x004261d0u,f);
    /* 004d952c */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d952f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9530 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9531 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9532 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9533 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9536 */
    f->r[4]+=4;return;
mi_004d9537:;
    /* 004d9537 */
    a=(f->r[0]&0xffu);b=0x0000001bu;t=mi_alu(f,a,b,8,1);
    /* 004d9539 */
    if(!(f->flags&MI_ZF))goto mi_004d95d2;
    /* 004d953f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d9544 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9546 */
    if((f->flags&MI_ZF))goto mi_004d955e;
    /* 004d9548 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9549 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d954a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d954b */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d9552 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d9559 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d955a */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d955d */
    f->r[4]+=4;return;
mi_004d955e:;
    /* 004d955e */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9563 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9569 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d956a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d956b */
    a=0x0000002fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d956d */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9570 */
    a=0x005c5eb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9575 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9578 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9579 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d957a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d957fu;f->boundary(0x004d957au,0x00426450u,f);
    /* 004d957f */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d9585 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d958b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d958e */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9591 */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9594 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9595 */
    a=0x0000002fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9597 */
    a=0x005c5eb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d959c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d959d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d959e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d95a3u;f->boundary(0x004d959eu,0x004d6740u,f);
    /* 004d95a3 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d95a9 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d95ae */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d95b3 */
    a=0x0000002fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d95b5 */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d95b8 */
    a=0x005c5eb0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d95bd */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d95c0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d95c1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d95c2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d95c7u;f->boundary(0x004d95c2u,0x004261d0u,f);
    /* 004d95c7 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d95ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d95cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d95cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d95cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d95ce */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d95d1 */
    f->r[4]+=4;return;
mi_004d95d2:;
    /* 004d95d2 */
    a=(f->r[0]&0xffu);b=0x00000046u;t=mi_alu(f,a,b,8,1);
    /* 004d95d4 */
    if(!(f->flags&MI_ZF))goto mi_004d965b;
    /* 004d95da */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d95df */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d95e1 */
    if((f->flags&MI_ZF))goto mi_004d95f9;
    /* 004d95e3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d95e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d95e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d95e6 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d95ed */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d95f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d95f5 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d95f8 */
    f->r[4]+=4;return;
mi_004d95f9:;
    /* 004d95f9 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d95fe */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9604 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9605 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9606 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9608 */
    a=0x005c6cd8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d960d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d960e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d960f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9614u;f->boundary(0x004d960fu,0x00426450u,f);
    /* 004d9614 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d961a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d961d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d961e */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9623 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9625 */
    a=0x005c6cd8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d962a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d962b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d962c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9631u;f->boundary(0x004d962cu,0x004d6740u,f);
    /* 004d9631 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d9637 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d963d */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d963f */
    a=0x005c6cd8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9644 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9645 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9646 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d964b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9650u;f->boundary(0x004d964bu,0x004261d0u,f);
    /* 004d9650 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9653 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9654 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9655 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9656 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9657 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d965a */
    f->r[4]+=4;return;
mi_004d965b:;
    /* 004d965b */
    a=(f->r[0]&0xffu);b=0x0000004bu;t=mi_alu(f,a,b,8,1);
    /* 004d965d */
    if(!(f->flags&MI_ZF))goto mi_004d96e3;
    /* 004d9663 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d9668 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d966a */
    if((f->flags&MI_ZF))goto mi_004d9682;
    /* 004d966c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d966d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d966e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d966f */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000001cu);
    /* 004d9676 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d967d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d967e */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9681 */
    f->r[4]+=4;return;
mi_004d9682:;
    /* 004d9682 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9687 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d968d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d968e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d968f */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9691 */
    a=0x005c6f08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9696 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9697 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9698 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d969du;f->boundary(0x004d9698u,0x00426450u,f);
    /* 004d969d */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d96a3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d96a6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96a7 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d96ac */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96ae */
    a=0x005c6f08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96b3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d96bau;f->boundary(0x004d96b5u,0x004d6740u,f);
    /* 004d96ba */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d96c0 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d96c5 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d96ca */
    a=0x00000013u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96cc */
    a=0x005c6f08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96d1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96d2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d96d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d96d8u;f->boundary(0x004d96d3u,0x004261d0u,f);
    /* 004d96d8 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d96db */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d96dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d96dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d96de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d96df */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d96e2 */
    f->r[4]+=4;return;
mi_004d96e3:;
    /* 004d96e3 */
    a=(f->r[0]&0xffu);b=0x00000027u;t=mi_alu(f,a,b,8,1);
    /* 004d96e5 */
    if(!(f->flags&MI_ZF))goto mi_004d97b7;
    /* 004d96eb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d96f0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d96f2 */
    if((f->flags&MI_ZF))goto mi_004d970a;
    /* 004d96f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d96f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d96f6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d96f7 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d96fe */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d9705 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9706 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9709 */
    f->r[4]+=4;return;
mi_004d970a:;
    /* 004d970a */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d970f */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9715 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9716 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9717 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9719 */
    a=0x005d9a90u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d971e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d971f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9720 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9725u;f->boundary(0x004d9720u,0x00426450u,f);
    /* 004d9725 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d972b */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d9731 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9734 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9735 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9737 */
    a=0x005d9a90u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d973c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d973d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d973e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9743u;f->boundary(0x004d973eu,0x004d6740u,f);
    /* 004d9743 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9749 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d974e */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9753 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9755 */
    a=0x005d9a90u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d975a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d975b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d975c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9761u;f->boundary(0x004d975cu,0x004261d0u,f);
    /* 004d9761 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9764 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9766 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d976c */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9772 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9777 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9779 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d977a */
    a=0x005d9a48u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d977f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9780 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9781 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9786u;f->boundary(0x004d9781u,0x00425bc0u,f);
    /* 004d9786 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d978c */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d9792 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9793 */
    a=0x005d9a48u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9798 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9799 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d979a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d979fu;f->boundary(0x004d979au,0x00425990u,f);
    /* 004d979f */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d97a2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d97a4 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d97aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d97afu;f->boundary(0x004d97aau,0x004483f0u,f);
    /* 004d97af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d97b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d97b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d97b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d97b3 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d97b6 */
    f->r[4]+=4;return;
mi_004d97b7:;
    /* 004d97b7 */
    a=(f->r[0]&0xffu);b=0x0000001cu;t=mi_alu(f,a,b,8,1);
    /* 004d97b9 */
    if(!(f->flags&MI_ZF))goto mi_004d9851;
    /* 004d97bf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d97c4 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d97c6 */
    if((f->flags&MI_ZF))goto mi_004d97de;
    /* 004d97c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d97c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d97ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d97cb */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d97d2 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d97d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d97da */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d97dd */
    f->r[4]+=4;return;
mi_004d97de:;
    /* 004d97de */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d97e3 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d97e9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d97ea */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d97eb */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d97ed */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d97f0 */
    a=0x005c63d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d97f5 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d97f8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d97f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d97fa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d97ffu;f->boundary(0x004d97fau,0x00426450u,f);
    /* 004d97ff */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9805 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9808 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d980b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d980c */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9811 */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9813 */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9816 */
    a=0x005c63d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d981b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d981c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d981d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9822u;f->boundary(0x004d981du,0x004d6740u,f);
    /* 004d9822 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9828 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d982d */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9832 */
    a=0x00000012u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9834 */
    a=f->r[2];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9837 */
    a=0x005c63d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d983c */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d983f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9840 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9841 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9846u;f->boundary(0x004d9841u,0x004261d0u,f);
    /* 004d9846 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9849 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d984a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d984b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d984c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d984d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9850 */
    f->r[4]+=4;return;
mi_004d9851:;
    /* 004d9851 */
    a=(f->r[0]&0xffu);b=0x00000025u;t=mi_alu(f,a,b,8,1);
    /* 004d9853 */
    if(!(f->flags&MI_ZF))goto mi_004d98ec;
    /* 004d9859 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d985e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9860 */
    if((f->flags&MI_ZF))goto mi_004d9878;
    /* 004d9862 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9863 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9864 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9865 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004d986c */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d9873 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9874 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9877 */
    f->r[4]+=4;return;
mi_004d9878:;
    /* 004d9878 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d987d */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9883 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9884 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9885 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9887 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d988a */
    a=0x005c5cd0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d988f */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9892 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9893 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9894 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9899u;f->boundary(0x004d9894u,0x00426450u,f);
    /* 004d9899 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d989f */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d98a5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d98a8 */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d98ab */
    a=f->r[2];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d98ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98af */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98b1 */
    a=0x005c5cd0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98b6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98b7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98b8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d98bdu;f->boundary(0x004d98b8u,0x004d6740u,f);
    /* 004d98bd */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d98c3 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d98c8 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d98cd */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98cf */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d98d2 */
    a=0x005c5cd0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98d7 */
    a=f->r[1];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d98da */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98db */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d98dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d98e1u;f->boundary(0x004d98dcu,0x004261d0u,f);
    /* 004d98e1 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d98e4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d98e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d98e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d98e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d98e8 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d98eb */
    f->r[4]+=4;return;
mi_004d98ec:;
    /* 004d98ec */
    a=(f->r[0]&0xffu);b=0x00000024u;t=mi_alu(f,a,b,8,1);
    /* 004d98ee */
    if(!(f->flags&MI_ZF))goto mi_004d9987;
    /* 004d98f4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d98f9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d98fb */
    if((f->flags&MI_ZF))goto mi_004d9913;
    /* 004d98fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d98fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d98ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9900 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000015u);
    /* 004d9907 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d990e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d990f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9912 */
    f->r[4]+=4;return;
mi_004d9913:;
    /* 004d9913 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9918 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d991e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d991f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9920 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9922 */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9925 */
    a=0x005c3488u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d992a */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d992d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d992e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d992f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9934u;f->boundary(0x004d992fu,0x00426450u,f);
    /* 004d9934 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d993a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d993d */
    a=f->r[2];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9940 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9941 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9946 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9948 */
    a=0x005c3488u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d994d */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9950 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9951 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9952 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9957u;f->boundary(0x004d9952u,0x004d6740u,f);
    /* 004d9957 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004d995d */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004d9963 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9965 */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9968 */
    a=0x005c3488u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d996d */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9970 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9971 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9972 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d9977 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d997cu;f->boundary(0x004d9977u,0x004261d0u,f);
    /* 004d997c */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d997f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9980 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9981 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9982 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9983 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9986 */
    f->r[4]+=4;return;
mi_004d9987:;
    /* 004d9987 */
    a=(f->r[0]&0xffu);b=0x0000004eu;t=mi_alu(f,a,b,8,1);
    /* 004d9989 */
    if(!(f->flags&MI_ZF))goto mi_004d9a18;
    /* 004d998f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d9994 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9996 */
    if((f->flags&MI_ZF))goto mi_004d99ae;
    /* 004d9998 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9999 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d999a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d999b */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000015u);
    /* 004d99a2 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d99a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d99aa */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d99ad */
    f->r[4]+=4;return;
mi_004d99ae:;
    /* 004d99ae */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d99b3 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d99b9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99ba */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99bb */
    a=0x00000045u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99bd */
    a=0x005c3648u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99c2 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d99c5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99c6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d99ccu;f->boundary(0x004d99c7u,0x00426450u,f);
    /* 004d99cc */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d99d2 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d99d5 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d99d8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99d9 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d99de */
    a=0x00000045u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99e0 */
    a=0x005c3648u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99e5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99e6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d99ecu;f->boundary(0x004d99e7u,0x004d6740u,f);
    /* 004d99ec */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d99f2 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d99f7 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d99fc */
    a=0x00000045u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d99fe */
    a=0x005c3648u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a03 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9a06 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a07 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a08 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9a0du;f->boundary(0x004d9a08u,0x004261d0u,f);
    /* 004d9a0d */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9a10 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9a11 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9a12 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9a13 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9a14 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9a17 */
    f->r[4]+=4;return;
mi_004d9a18:;
    /* 004d9a18 */
    a=(f->r[0]&0xffu);b=0x00000047u;t=mi_alu(f,a,b,8,1);
    /* 004d9a1a */
    if(!(f->flags&MI_ZF))goto mi_004d9a3f;
    /* 004d9a1c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d9a21 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9a23 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9a29 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9a2a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9a2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9a2c */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000015u);
    /* 004d9a33 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d9a3a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9a3b */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9a3e */
    f->r[4]+=4;return;
mi_004d9a3f:;
    /* 004d9a3f */
    a=(f->r[0]&0xffu);b=0x00000014u;t=mi_alu(f,a,b,8,1);
    /* 004d9a41 */
    if(!(f->flags&MI_ZF))goto mi_004d9cad;
    /* 004d9a47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9a4cu;f->boundary(0x004d9a47u,0x004cf660u,f);
    /* 004d9a4c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a4d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9a52u;f->boundary(0x004d9a4du,0x00442730u,f);
    /* 004d9a52 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004d9a58 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a5a */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a5c */
    a=f->r[1];b=0xffffffd4u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9a5f */
    a=0x005c7ce8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a64 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a65 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a67 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9a6cu;f->boundary(0x004d9a67u,0x00425bc0u,f);
    /* 004d9a6c */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004d9a72 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a74 */
    a=f->r[2];b=0xffffffd4u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9a77 */
    a=0x005c7ce8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a7c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a7d */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a7f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9a84u;f->boundary(0x004d9a7fu,0x00425990u,f);
    /* 004d9a84 */
    a=f->r[4];b=0x00000028u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9a87 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9a89 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9a8f */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004d9a94 */
    f->r[1]=MI_U32((0x00e38db0u));
    /* 004d9a9a */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a9c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9a9e */
    a=f->r[0];b=0xffffffd4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9aa1 */
    a=0x005c7d10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9aa6 */
    a=f->r[1];b=0xffffffd4u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9aa9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9aaa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9aab */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9ab0u;f->boundary(0x004d9aabu,0x00425bc0u,f);
    /* 004d9ab0 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004d9ab6 */
    f->r[0]=MI_U32((0x00e38db0u));
    /* 004d9abb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9abd */
    a=f->r[2];b=0xffffffd4u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9ac0 */
    a=0x005c7d10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9ac5 */
    a=f->r[0];b=0xffffffd4u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9ac8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9ac9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9aca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9acfu;f->boundary(0x004d9acau,0x00425990u,f);
    /* 004d9acf */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9ad2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9ad4 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9ada */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004d9ae0 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9ae2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9ae4 */
    a=f->r[1];b=0xffffff92u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9ae7 */
    a=0x005c7d38u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9aec */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9aed */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9aef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9af4u;f->boundary(0x004d9aefu,0x00425bc0u,f);
    /* 004d9af4 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004d9afa */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9afc */
    a=f->r[2];b=0xffffff92u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9aff */
    a=0x005c7d38u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9b04 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9b05 */
    a=0x00000018u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9b07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9b0cu;f->boundary(0x004d9b07u,0x00425990u,f);
    /* 004d9b0c */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9b0f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9b11 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9b17 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004d9b1c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9b1e */
    if((f->flags&MI_ZF))goto mi_004d9b79;
    /* 004d9b20 */
    f->r[0]=MI_U32((0x00e38e34u));
    /* 004d9b25 */
    f->r[1]=MI_U32((0x005d2a18u));
    /* 004d9b2b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004d9b2d */
    if(mi_signed_less(f->flags))goto mi_004d9b79;
    /* 004d9b2f */
    f->r[2]=MI_U32((0x00e38d9cu));
    /* 004d9b35 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9b37 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004d9b39 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004d9b3b */
    if(!mi_signed_less(f->flags))goto mi_004d9b79;
    /* 004d9b3d */
    f->r[0]=MI_U32((0x00e38e30u));
    /* 004d9b42 */
    f->r[1]=MI_U32((0x005d2a1cu));
    /* 004d9b48 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004d9b4a */
    if(mi_signed_less(f->flags))goto mi_004d9b79;
    /* 004d9b4c */
    f->r[2]=MI_U32((0x00e38d98u));
    /* 004d9b52 */
    f->r[3]=MI_U32((0x005d2a20u));
    /* 004d9b58 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004d9b5a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004d9b5c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004d9b5e */
    if(!mi_signed_less(f->flags))goto mi_004d9b79;
    /* 004d9b60 */
    a=0x005d2e88u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9b65 */
    a=0x00784f14u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9b6a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9b6fu;f->boundary(0x004d9b6au,0x00407a10u,f);
    /* 004d9b6f */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9b72 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_004d9b79:;
    /* 004d9b79 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004d9b7e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9b80 */
    if((f->flags&MI_ZF))goto mi_004d9b9e;
    /* 004d9b82 */
    MI_U32((0x00784f14u))=(uint32_t)(f->r[5]);
    /* 004d9b88 */
    MI_U32((0x005d2e88u))=(uint32_t)(0xffffffffu);
    /* 004d9b92 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004d9b99 */
    goto mi_004d9c93;
mi_004d9b9e:;
    /* 004d9b9e */
    f->r[0]=MI_U32((0x00784f14u));
    /* 004d9ba3 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004d9ba5 */
    if((f->flags&MI_ZF))goto mi_004d9c93;
    /* 004d9bab */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004d9bac */
    f->r[1]=f->r[0];
    /* 004d9bae */
    a=f->r[1];b=0x00000062u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 004d9bb1 */
    a=MI_U8((0x0077f748u+f->r[1]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004d9bb8 */
    if(!(f->flags&MI_ZF))goto mi_004d9c93;
    /* 004d9bbe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9bbf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9bc4u;f->boundary(0x004d9bbfu,0x00444920u,f);
    /* 004d9bc4 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004d9bc7 */
    MI_U32((0x00784f18u))=(uint32_t)(f->r[0]);
    /* 004d9bcc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9bcf */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004d9bd2 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004d9bd4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004d9bd6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004d9bd8 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004d9bda */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de02u+f->r[1])))&0xffu)<<0);
    /* 004d9be0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004d9be1 */
    if((f->flags&MI_ZF))goto mi_004d9bef;
    /* 004d9be3 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d9be5 */
    if((f->flags&MI_ZF))goto mi_004d9bef;
    /* 004d9be7 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d9be9 */
    if(!(f->flags&MI_ZF))goto mi_004d9c93;
mi_004d9bef:;
    /* 004d9bef */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077de05u+f->r[1])))&0xffu)<<0);
    /* 004d9bf5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9bf7 */
    if(!(f->flags&MI_ZF))goto mi_004d9c93;
    /* 004d9bfd */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9c03 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9c08 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c09 */
    f->r[2]=MI_U32((0x00e38db0u));
    /* 004d9c0f */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004d9c15 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c16 */
    f->r[0]=(0xfffffe0cu+f->r[2]);
    /* 004d9c1c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c1e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d9c1f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d9c21 */
    a=f->r[1];b=0xffffff97u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9c24 */
    a=0x005c3da0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c29 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c2a */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d9c2c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c2d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9c32u;f->boundary(0x004d9c2du,0x00426450u,f);
    /* 004d9c32 */
    f->r[1]=MI_U32((0x00e38db0u));
    /* 004d9c38 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9c3b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c3c */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004d9c41 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c43 */
    a=f->r[0];b=0xffffff97u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9c46 */
    a=0x005c3da0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c4b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c4c */
    f->r[0]=(0xfffffe0cu+f->r[1]);
    /* 004d9c52 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d9c53 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d9c55 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d9c57 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c58 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9c5du;f->boundary(0x004d9c58u,0x004d6740u,f);
    /* 004d9c5d */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004d9c63 */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004d9c68 */
    f->r[0]=MI_U32((0x00e38db0u));
    /* 004d9c6d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c6f */
    a=f->r[2];b=0xffffff97u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9c72 */
    a=0x005c3da0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c77 */
    a=f->r[0];b=0xfffffe0cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9c7c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c7d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004d9c7e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d9c80 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004d9c82 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9c83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9c88u;f->boundary(0x004d9c83u,0x004261d0u,f);
    /* 004d9c88 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9c8b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9c8d */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004d9c93:;
    /* 004d9c93 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004d9c98 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9c9a */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9ca0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9ca5u;f->boundary(0x004d9ca0u,0x00409520u,f);
    /* 004d9ca5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004d9ca6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004d9ca7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004d9ca8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004d9ca9 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9cac */
    f->r[4]+=4;return;
mi_004d9cad:;
    /* 004d9cad */
    a=(f->r[0]&0xffu);b=0x00000015u;t=mi_alu(f,a,b,8,1);
    /* 004d9caf */
    if(!(f->flags&MI_ZF))goto mi_004dbe57;
    /* 004d9cb5 */
    f->r[5]=MI_U32((0x00775910u));
    /* 004d9cbb */
    f->r[3]=MI_U32((0x005d2a18u));
    /* 004d9cc1 */
    f->r[0]=MI_U32((0x005d2a1cu));
    /* 004d9cc6 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 004d9cc8 */
    f->r[3]=MI_U32((0x0077590cu));
    /* 004d9cce */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004d9cd0 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004d9cd2 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004d9cd7 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004d9cd9 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004d9cdd */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004d9ce0 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004d9ce4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004d9ce7 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004d9cea */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770ecu+f->r[0]));
    /* 004d9cf1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770eeu+f->r[0]));
    /* 004d9cf8 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004d9cfc */
    MI_U32((0x00000044u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004d9d00 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9d05u;f->boundary(0x004d9d00u,0x00442500u,f);
    /* 004d9d05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9d0au;f->boundary(0x004d9d05u,0x004426c0u,f);
    /* 004d9d0a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9d0fu;f->boundary(0x004d9d0au,0x004cf660u,f);
    /* 004d9d0f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d10 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9d15u;f->boundary(0x004d9d10u,0x00442730u,f);
    /* 004d9d15 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004d9d1a */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004d9d20 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004d9d26 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9d29 */
    a=f->r[2];b=0xffffff8cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9d2c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d2d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d2e */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d30 */
    a=0x005c3268u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d35 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d36 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d38 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9d3du;f->boundary(0x004d9d38u,0x00426450u,f);
    /* 004d9d3d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9d40 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d41 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004d9d46 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d48 */
    a=f->r[0];b=0xffffff8cu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9d4b */
    a=0x005c3268u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d50 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d51 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9d58u;f->boundary(0x004d9d53u,0x004d6740u,f);
    /* 004d9d58 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004d9d5e */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d60 */
    a=f->r[1];b=0xffffff8cu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9d63 */
    a=0x005c3268u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d68 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d69 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d6b */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d9d70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9d75u;f->boundary(0x004d9d70u,0x004261d0u,f);
    /* 004d9d75 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9d78 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9d7a */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9d80 */
    f->r[0]=MI_U32((0x007056c8u));
    /* 004d9d85 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9d87 */
    if(!(f->flags&MI_ZF))goto mi_004d9e11;
    /* 004d9d8d */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004d9d93 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004d9d98 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004d9d9e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9d9f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9da0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9da2 */
    a=f->r[1];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9da5 */
    a=0x005c3348u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9daa */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9dab */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9db0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9db5u;f->boundary(0x004d9db0u,0x00426450u,f);
    /* 004d9db5 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004d9dbb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9dbe */
    a=f->r[2];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004d9dc1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9dc2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9dc4 */
    a=0x005c3348u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9dc9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9dca */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9dcf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9dd4u;f->boundary(0x004d9dcfu,0x004d6740u,f);
    /* 004d9dd4 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004d9dd9 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004d9dde */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9de0 */
    a=f->r[0];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9de3 */
    a=0x005c3348u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9de8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9de9 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9dee */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9df3u;f->boundary(0x004d9deeu,0x004261d0u,f);
    /* 004d9df3 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9df6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9df8 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9dfe */
    a=MI_U32((0x007056c8u));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004d9e05 */
    if(!(f->flags&MI_ZF))goto mi_004d9e11;
    /* 004d9e07 */
    MI_U32((0x007056c8u))=(uint32_t)(0x0000000au);
mi_004d9e11:;
    /* 004d9e11 */
    f->r[0]=MI_U32((0x00775928u));
    /* 004d9e16 */
    f->r[1]=f->r[0];
    /* 004d9e18 */
    a=f->r[1];b=0x0000000eu;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004d9e1b */
    if((f->flags&MI_ZF))goto mi_004db079;
    /* 004d9e21 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004d9e22 */
    if(!(f->flags&MI_ZF))goto mi_004db0ff;
    /* 004d9e28 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004d9e2e */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9e30 */
    a=f->r[1];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004d9e33 */
    a=0x005c3418u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9e38 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9e39 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004d9e3e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004d9e43u;f->boundary(0x004d9e3eu,0x004261d0u,f);
    /* 004d9e43 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004d9e46 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9e48 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004d9e4e */
    f->r[6]=MI_U32((0x005d2a30u));
    /* 004d9e54 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004d9e57 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004d9e5a */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004d9e5d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770eeu+f->r[6]));
    /* 004d9e64 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770ecu+f->r[6]));
    /* 004d9e6b */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004d9e6f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004d9e74 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9e76 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004d9e7a */
    if((f->flags&MI_ZF))goto mi_004da7d5;
    /* 004d9e80 */
    f->r[1]=(0xfffffff8u+f->r[3]);
    /* 004d9e83 */
    f->r[0]=(0xfffffff8u+f->r[5]);
    /* 004d9e86 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004d9e8a */
    f->r[1]=(0x00000008u+f->r[5]);
    /* 004d9e8d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004d9e91 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004d9e93 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004d9e99 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004d9e9d */
    f->r[2]=(0x00000008u+f->r[3]);
    /* 004d9ea0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004d9ea2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004d9ea4 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004d9ea8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d9eef;
    /* 004d9eaa */
    f->r[2]=(0x00776fbau+f->r[6]);
    /* 004d9eb0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
mi_004d9eb4:;
    /* 004d9eb4 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004d9eb8 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004d9ebc */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[2]));
    /* 004d9ec0 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004d9ec2 */
    if(mi_signed_less(f->flags))goto mi_004d9edf;
    /* 004d9ec4 */
    a=f->r[2];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004d9ec8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004d9edf;
    /* 004d9eca */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004d9ece */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004d9ed2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004d9ed5 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004d9ed7 */
    if(mi_signed_less(f->flags))goto mi_004d9edf;
    /* 004d9ed9 */
    a=f->r[2];b=MI_U32((0x00000024u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004d9edd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d9f35;
mi_004d9edf:;
    /* 004d9edf */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 004d9ee3 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004d9ee4 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004d9ee7 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004d9ee9 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004d9eed */
    if(mi_signed_less(f->flags))goto mi_004d9eb4;
mi_004d9eef:;
    /* 004d9eef */
    f->r[0]=MI_U32((0x007770e4u+f->r[6]));
    /* 004d9ef5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004d9ef7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004d9ef9 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 004d9efe */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d9f4f;
    /* 004d9f00 */
    f->r[0]=MI_U32((0x00776fb4u+f->r[6]));
mi_004d9f06:;
    /* 004d9f06 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 004d9f09 */
    a=f->r[1];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004d9f0d */
    if(mi_signed_less(f->flags))goto mi_004d9f25;
    /* 004d9f0f */
    a=f->r[1];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004d9f13 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004d9f25;
    /* 004d9f15 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[0]));
    /* 004d9f19 */
    a=f->r[1];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004d9f1d */
    if(mi_signed_less(f->flags))goto mi_004d9f25;
    /* 004d9f1f */
    a=f->r[1];b=MI_U32((0x00000024u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004d9f23 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004d9f3c;
mi_004d9f25:;
    /* 004d9f25 */
    f->r[1]=MI_U32((0x007770e4u+f->r[6]));
    /* 004d9f2b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004d9f2c */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004d9f2f */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004d9f31 */
    if(mi_signed_less(f->flags))goto mi_004d9f06;
    /* 004d9f33 */
    goto mi_004d9f4f;
mi_004d9f35:;
    /* 004d9f35 */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
    /* 004d9f3a */
    goto mi_004d9f52;
mi_004d9f3c:;
    /* 004d9f3c */
    f->r[0]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004d9f42 */
    f->r[1]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004d9f45 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004d9f47 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[0]+f->r[1]*2u)))&0xffu)<<0);
    /* 004d9f4b */
    f->r[0]=f->r[2];
    /* 004d9f4d */
    goto mi_004d9f52;
mi_004d9f4f:;
    /* 004d9f4f */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004d9f52:;
    /* 004d9f52 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004d9f54 */
    MI_U32((0x005e1db0u))=(uint32_t)(f->r[0]);
    /* 004d9f59 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004d9f5b */
    if(mi_signed_less(f->flags))goto mi_004da6f7;
    /* 004d9f61 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000013u+f->r[4])))&0xffu)<<0);
    /* 004d9f65 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004d9f67 */
    if(!(f->flags&MI_ZF))goto mi_004da340;
    /* 004d9f6d */
    f->r[2]=(0x00000001u+f->r[0]);
    /* 004d9f70 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004d9f72 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004d9f76 */
    if(!mi_signed_less(f->flags))goto mi_004d9f7e;
    /* 004d9f78 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004d9f7c */
    f->r[2]=f->r[1];
mi_004d9f7e:;
    /* 004d9f7e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004d9f84 */
    a=(f->r[0]&0xffu);b=0x00000032u;t=mi_alu(f,a,b,8,1);
    /* 004d9f86 */
    if(!(f->flags&MI_CF))goto mi_004da333;
    /* 004d9f8c */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004d9f91 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004d9f92 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004d9f94 */
    if(mi_signed_less(f->flags))goto mi_004d9fc5;
    /* 004d9f96 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004d9f99 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004d9f9b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004d9f9c */
    f->r[1]=(0x00776fbeu+f->r[6]+f->r[1]*2u);
    /* 004d9fa3 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004d9fa7:;
    /* 004d9fa7 */
    f->r[2]=(0xfffffffau+f->r[1]);
    /* 004d9faa */
    f->r[0]=f->r[2];
    /* 004d9fac */
    f->r[7]=MI_U32((0x00000000u+f->r[0]));
    /* 004d9fae */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[7]);
    /* 004d9fb0 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[0])))&0xffffu)<<0);
    /* 004d9fb4 */
    MI_U16((0x00000004u+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004d9fb8 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 004d9fbc */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004d9fbd */
    f->r[1]=f->r[2];
    /* 004d9fbf */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004d9fc3 */
    if(!(f->flags&MI_ZF))goto mi_004d9fa7;
mi_004d9fc5:;
    /* 004d9fc5 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004d9fc9 */
    f->r[7]=0x00000001u;
    /* 004d9fce */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004d9fd1 */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[1]*2u);
    /* 004d9fd8 */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004d9fdc */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004d9fe0 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004d9fe6 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004d9fe8 */
    a=(f->r[3]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004d9fea */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[3]&0xffu))&0xffu)<<0);
    /* 004d9fec */
    MI_U8((0x007770f1u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004d9ff2 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004d9ff7 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004d9ff9 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004d9ffd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da05a;
    /* 004d9fff */
    f->r[2]=(0x00776fb8u+f->r[6]);
mi_004da005:;
    /* 004da005 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004da008 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004da00c */
    f->r[0]=f->r[3];
    /* 004da00e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da010 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004da012 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da014 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da018 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da01e;
    /* 004da01a */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004da01e:;
    /* 004da01e */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004da022 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004da026 */
    f->r[0]=f->r[3];
    /* 004da028 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da02a */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004da02c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da02e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da032;
    /* 004da030 */
    f->r[0]=f->r[1];
mi_004da032:;
    /* 004da032 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004da036 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da038 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da045;
    /* 004da03a */
    f->r[5]=(0x00000001u+f->r[1]+f->r[5]*1u);
    /* 004da03e */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004da043 */
    goto mi_004da04e;
mi_004da045:;
    /* 004da045 */
    f->r[5]=(0x00000001u+f->r[0]+f->r[5]*1u);
    /* 004da049 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004da04e:;
    /* 004da04e */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004da052 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004da053 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da056 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da058 */
    if(mi_signed_less(f->flags))goto mi_004da005;
mi_004da05a:;
    /* 004da05a */
    restore_menu_inline_capacity_before(0x004da05au,f);
    f->r[0]=MI_U32((0x007770e8u+f->r[6]));
    /* 004da060 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da062 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da0a2;
    /* 004da064 */
    f->r[7]=(0x00000032u+f->r[5]);
    /* 004da067 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da069 */
    if(!mi_signed_less(f->flags))goto mi_004da0a2;
    /* 004da06b */
    f->r[0]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da071 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004da073 */
    if((f->flags&MI_ZF))goto mi_004da088;
    /* 004da075 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004da076 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da07bu;f->boundary(0x004da076u,0x00561a2bu,f);
    /* 004da07b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004da07e */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004da088:;
    /* 004da088 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004da08b */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004da08d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004da08e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da093u;f->boundary(0x004da08eu,0x00561a1du,f);
    /* 004da093 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004da096 */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004da09c */
    MI_U32((0x007770e8u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004da0a2:;
    /* 004da0a2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da0a8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004da0aa */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004da0ac */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004da0b2 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da0b6 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004da0be */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004da21d;
    /* 004da0c4 */
    f->r[5]=(0x00776fbau+f->r[6]);
mi_004da0ca:;
    /* 004da0ca */
    a=f->r[2];b=MI_U32((0x007770e4u+f->r[6]));t=mi_alu(f,a,b,32,1);
    /* 004da0d0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da333;
    /* 004da0d6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004da0da */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004da0de */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004da0e2 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da0e6 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da0e8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004da0ec */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da0ee */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004da0f2 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da0f6 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004da0f8 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da0fc */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da100 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004da104 */
    __asm { fld st(0) }
    /* 004da106 */
    __asm { fmul st(0),st(1) }
    /* 004da108 */
    __asm { fld st(2) }
    /* 004da10a */
    __asm { fmul st(0),st(3) }
    /* 004da10c */
    __asm { faddp st(1),st(0) }
    /* 004da10e */
    __asm { fsqrt }
    /* 004da110 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da114 */
    __asm { fstp st(0) }
    /* 004da116 */
    __asm { fstp st(0) }
    /* 004da118 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da11c */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004da122 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da124 */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004da127 */
    if(!(f->flags&MI_ZF))goto mi_004da1fb;
    /* 004da12d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004da133 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da135 */
    if(!(f->flags&MI_ZF))goto mi_004da13e;
    /* 004da137 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004da139 */
    goto mi_004da1fb;
mi_004da13e:;
    /* 004da13e */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da142 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004da146 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da14a */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da14e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004da156 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004da15a */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da15e */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da164 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004da168 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da16a */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004da16d */
    if((f->flags&MI_ZF))goto mi_004da1f7;
    /* 004da173 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da177 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da17d */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004da181 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da185 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004da188 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004da18a */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004da18e:;
    /* 004da18e */
    __asm { fld st(0) }
    /* 004da190 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fmul dword ptr [eax] }
    /* 004da194 */
    __asm { fadd st(0),st(2) }
    /* 004da196 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da19bu;f->boundary(0x004da196u,0x0056133cu,f);
    /* 004da19b */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fmul dword ptr [eax] }
    /* 004da19f */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da1a5 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004da1a9 */
    MI_U16((0x00000000u+f->r[7]+f->r[1]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da1ad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da1b2u;f->boundary(0x004da1adu,0x0056133cu,f);
    /* 004da1b2 */
    f->r[2]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da1b8 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004da1bb */
    MI_U16((0xfffffffcu+f->r[7]+f->r[2]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da1c0 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004da1c4 */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da1ca */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004da1cd */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004da1d1 */
    MI_U8((0xfffffffeu+f->r[7]+f->r[1]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004da1d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004da1d7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004da1dd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da1df */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004da1e0 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da1e4 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da1e8 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004da1ec */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da1ee */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004da1f1 */
    if(!(f->flags&MI_ZF))goto mi_004da18e;
    /* 004da1f3 */
    __asm { fstp st(0) }
    /* 004da1f5 */
    __asm { fstp st(0) }
mi_004da1f7:;
    /* 004da1f7 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
mi_004da1fb:;
    /* 004da1fb */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004da1ff */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004da201 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da207 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da209 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004da20a */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004da20d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da20f */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da213 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da217 */
    if(mi_signed_less(f->flags))goto mi_004da0ca;
mi_004da21d:;
    /* 004da21d */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004da220 */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004da226 */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da22d */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da234 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da23a */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da23c */
    MI_U8((0x00000034u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 004da240 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004da325;
    /* 004da246 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004da24e */
    f->r[5]=0x0077f748u;
mi_004da253:;
    /* 004da253 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000001u+f->r[5])))&0xffu)<<0);
    /* 004da256 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da258 */
    if((f->flags&MI_ZF))goto mi_004da30d;
    /* 004da25e */
    a=MI_U8((0x00000000u+f->r[5]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004da262 */
    if(!(f->flags&MI_ZF))goto mi_004da30d;
    /* 004da268 */
    f->r[2]=MI_U32((0x0126517cu));
    /* 004da26e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004da270 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[5])))&0xffffu)<<0);
    /* 004da274 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da276 */
    f->r[1]=f->r[0];
    /* 004da278 */
    if(mi_signed_less(f->flags))goto mi_004da27c;
    /* 004da27a */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004da27c:;
    /* 004da27c */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004da27f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da281 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[1]));
    /* 004da288 */
    if(mi_signed_less(f->flags))goto mi_004da28c;
    /* 004da28a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004da28c:;
    /* 004da28c */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004da290 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004da294 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004da297 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004da299 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 004da2a0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb8u+f->r[6]));
    /* 004da2a7 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da2a9 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da2ab */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004da2af */
    if(mi_signed_less(f->flags))goto mi_004da2d0;
    /* 004da2b1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da2b3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da2d0;
    /* 004da2b5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fbau+f->r[6]));
    /* 004da2bc */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004da2be */
    if(mi_signed_less(f->flags))goto mi_004da2d0;
    /* 004da2c0 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da2c2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da2d0;
    /* 004da2c4 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000018u+f->r[4])))&0xffffu)<<0);
    /* 004da2c9 */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_004da2d0:;
    /* 004da2d0 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004da2d4 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004da2d9 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004da2dc */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb2u+f->r[6]+f->r[0]*2u));
    /* 004da2e4 */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[0]*2u);
    /* 004da2eb */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da2ed */
    if(mi_signed_less(f->flags))goto mi_004da30d;
    /* 004da2ef */
    a=f->r[7];b=MI_U32((0x00000050u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da2f3 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da30d;
    /* 004da2f5 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[0]));
    /* 004da2f9 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004da2fb */
    if(mi_signed_less(f->flags))goto mi_004da30d;
    /* 004da2fd */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da2ff */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da30d;
    /* 004da301 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000018u+f->r[4])))&0xffffu)<<0);
    /* 004da306 */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_004da30d:;
    /* 004da30d */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004da311 */
    a=f->r[5];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004da314 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004da315 */
    a=f->r[5];b=0x007843d8u;t=mi_alu(f,a,b,32,1);
    /* 004da31b */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da31f */
    if(mi_signed_less(f->flags))goto mi_004da253;
mi_004da325:;
    /* 004da325 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004da329 */
    MI_U32((0x005e1db0u))=(uint32_t)(f->r[0]);
    /* 004da32e */
    goto mi_004db01e;
mi_004da333:;
    /* 004da333 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004da336 */
    MI_U32((0x005e1db0u))=(uint32_t)(f->r[0]);
    /* 004da33b */
    goto mi_004db01e;
mi_004da340:;
    /* 004da340 */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004da346 */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 004da34c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da34e */
    if(mi_signed_less(f->flags))goto mi_004db01e;
    /* 004da354 */
    f->r[7]=MI_U32((0x00e38d9cu));
    /* 004da35a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da35c */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da35e */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da360 */
    if(!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004da366 */
    f->r[1]=MI_U32((0x00e38e30u));
    /* 004da36c */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 004da372 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da374 */
    if(mi_signed_less(f->flags))goto mi_004db01e;
    /* 004da37a */
    f->r[7]=MI_U32((0x00e38d98u));
    /* 004da380 */
    a=f->r[7];b=MI_U32((0x005d2a20u));t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da386 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da388 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da38a */
    if(!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004da390 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004da392 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da398 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da39a */
    if(!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004da3a0 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004da3a3 */
    f->r[7]=0x00000001u;
    /* 004da3a8 */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[2]*2u);
    /* 004da3af */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004da3b3 */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004da3b7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004da3b9 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004da3bb */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da3c1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da3c3 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da3c7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da424;
    /* 004da3c9 */
    f->r[2]=(0x00776fb8u+f->r[6]);
mi_004da3cf:;
    /* 004da3cf */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004da3d2 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004da3d6 */
    f->r[0]=f->r[3];
    /* 004da3d8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da3da */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004da3dc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da3de */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da3e2 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da3e8;
    /* 004da3e4 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004da3e8:;
    /* 004da3e8 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004da3ec */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004da3f0 */
    f->r[0]=f->r[3];
    /* 004da3f2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da3f4 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004da3f6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da3f8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da3fc;
    /* 004da3fa */
    f->r[0]=f->r[1];
mi_004da3fc:;
    /* 004da3fc */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004da400 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da402 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da40f;
    /* 004da404 */
    f->r[5]=(0x00000001u+f->r[1]+f->r[5]*1u);
    /* 004da408 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004da40d */
    goto mi_004da418;
mi_004da40f:;
    /* 004da40f */
    f->r[5]=(0x00000001u+f->r[0]+f->r[5]*1u);
    /* 004da413 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004da418:;
    /* 004da418 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004da41c */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004da41d */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da420 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da422 */
    if(mi_signed_less(f->flags))goto mi_004da3cf;
mi_004da424:;
    /* 004da424 */
    restore_menu_inline_capacity_before(0x004da424u,f);
    f->r[0]=MI_U32((0x007770e8u+f->r[6]));
    /* 004da42a */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da42c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da46c;
    /* 004da42e */
    f->r[7]=(0x00000032u+f->r[5]);
    /* 004da431 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da433 */
    if(!mi_signed_less(f->flags))goto mi_004da46c;
    /* 004da435 */
    f->r[0]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da43b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004da43d */
    if((f->flags&MI_ZF))goto mi_004da452;
    /* 004da43f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004da440 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da445u;f->boundary(0x004da440u,0x00561a2bu,f);
    /* 004da445 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004da448 */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004da452:;
    /* 004da452 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004da455 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004da457 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004da458 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da45du;f->boundary(0x004da458u,0x00561a1du,f);
    /* 004da45d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004da460 */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004da466 */
    MI_U32((0x007770e8u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004da46c:;
    /* 004da46c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da472 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004da474 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004da476 */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004da47c */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da480 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004da488 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004da5eb;
    /* 004da48e */
    f->r[5]=(0x00776fbau+f->r[6]);
mi_004da494:;
    /* 004da494 */
    a=f->r[2];b=MI_U32((0x007770e4u+f->r[6]));t=mi_alu(f,a,b,32,1);
    /* 004da49a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004da4a0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004da4a4 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004da4a8 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004da4ac */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da4b0 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da4b2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004da4b6 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da4b8 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004da4bc */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da4c0 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004da4c2 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da4c6 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da4ca */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004da4ce */
    __asm { fld st(0) }
    /* 004da4d0 */
    __asm { fmul st(0),st(1) }
    /* 004da4d2 */
    __asm { fld st(2) }
    /* 004da4d4 */
    __asm { fmul st(0),st(3) }
    /* 004da4d6 */
    __asm { faddp st(1),st(0) }
    /* 004da4d8 */
    __asm { fsqrt }
    /* 004da4da */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da4de */
    __asm { fstp st(0) }
    /* 004da4e0 */
    __asm { fstp st(0) }
    /* 004da4e2 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da4e6 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004da4ec */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da4ee */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004da4f1 */
    if(!(f->flags&MI_ZF))goto mi_004da5c9;
    /* 004da4f7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004da4fd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da4ff */
    if(!(f->flags&MI_ZF))goto mi_004da508;
    /* 004da501 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004da503 */
    goto mi_004da5c9;
mi_004da508:;
    /* 004da508 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da50c */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004da510 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da514 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da518 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004da520 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004da524 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da528 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da52e */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004da532 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da534 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004da537 */
    if((f->flags&MI_ZF))goto mi_004da5c5;
    /* 004da53d */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da541 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da547 */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004da54b */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da54f */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004da552 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004da554 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004da558:;
    /* 004da558 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da55c */
    __asm { fmul st(0),st(1) }
    /* 004da55e */
    __asm { fadd st(0),st(2) }
    /* 004da560 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da565u;f->boundary(0x004da560u,0x0056133cu,f);
    /* 004da565 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da569 */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da56f */
    __asm { fmul st(0),st(1) }
    /* 004da571 */
    MI_U16((0x00000000u+f->r[7]+f->r[1]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da575 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004da579 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da57eu;f->boundary(0x004da579u,0x0056133cu,f);
    /* 004da57e */
    f->r[2]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da584 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004da587 */
    __asm { fstp st(0) }
    /* 004da589 */
    MI_U16((0xfffffffcu+f->r[7]+f->r[2]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da58e */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004da592 */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da598 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004da59b */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004da59f */
    MI_U8((0xfffffffeu+f->r[7]+f->r[1]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004da5a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004da5a5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004da5ab */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da5ad */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004da5ae */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da5b2 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da5b6 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004da5ba */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da5bc */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004da5bf */
    if(!(f->flags&MI_ZF))goto mi_004da558;
    /* 004da5c1 */
    __asm { fstp st(0) }
    /* 004da5c3 */
    __asm { fstp st(0) }
mi_004da5c5:;
    /* 004da5c5 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
mi_004da5c9:;
    /* 004da5c9 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004da5cd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004da5cf */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da5d5 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da5d7 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004da5d8 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004da5db */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da5dd */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da5e1 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da5e5 */
    if(mi_signed_less(f->flags))goto mi_004da494;
mi_004da5eb:;
    /* 004da5eb */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004da5ee */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004da5f4 */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da5fb */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004da602 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da608 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da60a */
    MI_U8((0x00000034u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 004da60e */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004db01e;
    /* 004da614 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004da61c */
    f->r[7]=0x0077f748u;
mi_004da621:;
    /* 004da621 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000001u+f->r[7])))&0xffu)<<0);
    /* 004da624 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da626 */
    if((f->flags&MI_ZF))goto mi_004da6da;
    /* 004da62c */
    a=MI_U8((0x00000000u+f->r[7]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004da62f */
    if(!(f->flags&MI_ZF))goto mi_004da6da;
    /* 004da635 */
    f->r[2]=MI_U32((0x0126517cu));
    /* 004da63b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004da63d */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[7])))&0xffffu)<<0);
    /* 004da641 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da643 */
    f->r[1]=f->r[0];
    /* 004da645 */
    if(mi_signed_less(f->flags))goto mi_004da649;
    /* 004da647 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004da649:;
    /* 004da649 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004da64c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da64e */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[1]));
    /* 004da655 */
    if(mi_signed_less(f->flags))goto mi_004da659;
    /* 004da657 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004da659:;
    /* 004da659 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[7]));
    /* 004da65d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[7]));
    /* 004da661 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004da664 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004da666 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 004da66d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb8u+f->r[6]));
    /* 004da674 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da676 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da678 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004da67c */
    if(mi_signed_less(f->flags))goto mi_004da69d;
    /* 004da67e */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004da680 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da69d;
    /* 004da682 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fbau+f->r[6]));
    /* 004da689 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004da68b */
    if(mi_signed_less(f->flags))goto mi_004da69d;
    /* 004da68d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da68f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da69d;
    /* 004da691 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000001cu+f->r[4])))&0xffffu)<<0);
    /* 004da696 */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_004da69d:;
    /* 004da69d */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004da6a1 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004da6a6 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004da6a9 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb2u+f->r[6]+f->r[0]*2u));
    /* 004da6b1 */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[0]*2u);
    /* 004da6b8 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da6ba */
    if(mi_signed_less(f->flags))goto mi_004da6da;
    /* 004da6bc */
    a=f->r[5];b=MI_U32((0x00000050u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da6c0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da6da;
    /* 004da6c2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[0]));
    /* 004da6c6 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004da6c8 */
    if(mi_signed_less(f->flags))goto mi_004da6da;
    /* 004da6ca */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da6cc */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da6da;
    /* 004da6ce */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000001cu+f->r[4])))&0xffffu)<<0);
    /* 004da6d3 */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_004da6da:;
    /* 004da6da */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004da6de */
    a=f->r[7];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004da6e1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004da6e2 */
    a=f->r[7];b=0x007843d8u;t=mi_alu(f,a,b,32,1);
    /* 004da6e8 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da6ec */
    if(mi_signed_less(f->flags))goto mi_004da621;
    /* 004da6f2 */
    goto mi_004db01e;
mi_004da6f7:;
    /* 004da6f7 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004da6fb */
    f->r[5]=0x00776fbau;
mi_004da700:;
    /* 004da700 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000137u+f->r[5])))&0xffu)<<0);
    /* 004da706 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004da708 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da70a */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004da741;
    /* 004da70c */
    f->r[6]=MI_U32((0x00000020u+f->r[4]));
    /* 004da710 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004da714 */
    f->r[0]=f->r[5];
mi_004da716:;
    /* 004da716 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[0]));
    /* 004da71a */
    a=f->r[1];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da71e */
    if(mi_signed_less(f->flags))goto mi_004da731;
    /* 004da720 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da722 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da731;
    /* 004da724 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 004da727 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004da729 */
    if(mi_signed_less(f->flags))goto mi_004da731;
    /* 004da72b */
    a=f->r[1];b=MI_U32((0x00000024u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da72f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da77b;
mi_004da731:;
    /* 004da731 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004da733 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004da734 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000137u+f->r[5])))&0xffu)<<0);
    /* 004da73a */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004da73d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da73f */
    if(mi_signed_less(f->flags))goto mi_004da716;
mi_004da741:;
    /* 004da741 */
    f->r[6]=MI_U32((0x0000012au+f->r[5]));
    /* 004da747 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004da749 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004da74b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da78a;
    /* 004da74d */
    f->r[3]=MI_U32((0xfffffffau+f->r[5]));
    /* 004da750 */
    f->r[0]=f->r[3];
mi_004da752:;
    /* 004da752 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[0]));
    /* 004da755 */
    a=f->r[1];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da759 */
    if(mi_signed_less(f->flags))goto mi_004da771;
    /* 004da75b */
    a=f->r[1];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da75f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da771;
    /* 004da761 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[0]));
    /* 004da765 */
    a=f->r[1];b=MI_U32((0x00000020u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da769 */
    if(mi_signed_less(f->flags))goto mi_004da771;
    /* 004da76b */
    a=f->r[1];b=MI_U32((0x00000024u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004da76f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da77f;
mi_004da771:;
    /* 004da771 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004da772 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004da775 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004da777 */
    if(mi_signed_less(f->flags))goto mi_004da752;
    /* 004da779 */
    goto mi_004da78a;
mi_004da77b:;
    /* 004da77b */
    f->r[0]=f->r[2];
    /* 004da77d */
    goto mi_004da78d;
mi_004da77f:;
    /* 004da77f */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004da782 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004da784 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[3]+f->r[2]*2u)))&0xffu)<<0);
    /* 004da788 */
    goto mi_004da78d;
mi_004da78a:;
    /* 004da78a */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_004da78d:;
    /* 004da78d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004da78f */
    MI_U32((0x005e1db0u))=(uint32_t)(f->r[0]);
    /* 004da794 */
    if(!mi_signed_less(f->flags))goto mi_004da7b6;
    /* 004da796 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004da79a */
    a=f->r[5];b=0x00000144u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004da7a0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004da7a1 */
    a=f->r[5];b=0x0077af02u;t=mi_alu(f,a,b,32,1);
    /* 004da7a7 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da7ab */
    if(mi_signed_less(f->flags))goto mi_004da700;
    /* 004da7b1 */
    goto mi_004db028;
mi_004da7b6:;
    /* 004da7b6 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004da7ba */
    MI_U32((0x005e1db0u))=(uint32_t)(0xffffffffu);
    /* 004da7c4 */
    MI_U32((0x005d2a30u))=(uint32_t)(f->r[0]);
    /* 004da7c9 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004da7d0 */
    goto mi_004db028;
mi_004da7d5:;
    /* 004da7d5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e04u)))&0xffu)<<0);
    /* 004da7da */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da7dc */
    if((f->flags&MI_ZF))goto mi_004dabba;
    /* 004da7e2 */
    f->r[0]=MI_U32((0x005e1db0u));
    /* 004da7e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004da7e9 */
    if(mi_signed_less(f->flags))goto mi_004dabab;
    /* 004da7ef */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004da7f5 */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 004da7fb */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da7fd */
    if(mi_signed_less(f->flags))goto mi_004daba1;
    /* 004da803 */
    f->r[7]=MI_U32((0x00e38d9cu));
    /* 004da809 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da80b */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da80d */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da80f */
    if(!mi_signed_less(f->flags))goto mi_004daba1;
    /* 004da815 */
    f->r[1]=MI_U32((0x00e38e30u));
    /* 004da81b */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 004da821 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004da823 */
    if(mi_signed_less(f->flags))goto mi_004daba1;
    /* 004da829 */
    f->r[7]=MI_U32((0x00e38d98u));
    /* 004da82f */
    a=f->r[7];b=MI_U32((0x005d2a20u));t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da835 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da837 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da839 */
    if(!mi_signed_less(f->flags))goto mi_004daba1;
    /* 004da83f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004da841 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da847 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da849 */
    if(!mi_signed_less(f->flags))goto mi_004daba1;
    /* 004da84f */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004da852 */
    f->r[7]=0x00000001u;
    /* 004da857 */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[2]*2u);
    /* 004da85e */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004da862 */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004da866 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004da868 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004da86a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da870 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da872 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da876 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da8d3;
    /* 004da878 */
    f->r[2]=(0x00776fb8u+f->r[6]);
mi_004da87e:;
    /* 004da87e */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004da881 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004da885 */
    f->r[0]=f->r[3];
    /* 004da887 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da889 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004da88b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da88d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da891 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da897;
    /* 004da893 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004da897:;
    /* 004da897 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004da89b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004da89f */
    f->r[0]=f->r[3];
    /* 004da8a1 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da8a3 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004da8a5 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004da8a7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004da8ab;
    /* 004da8a9 */
    f->r[0]=f->r[1];
mi_004da8ab:;
    /* 004da8ab */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004da8af */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da8b1 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da8be;
    /* 004da8b3 */
    f->r[5]=(0x00000001u+f->r[1]+f->r[5]*1u);
    /* 004da8b7 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004da8bc */
    goto mi_004da8c7;
mi_004da8be:;
    /* 004da8be */
    f->r[5]=(0x00000001u+f->r[0]+f->r[5]*1u);
    /* 004da8c2 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004da8c7:;
    /* 004da8c7 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004da8cb */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004da8cc */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004da8cf */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da8d1 */
    if(mi_signed_less(f->flags))goto mi_004da87e;
mi_004da8d3:;
    /* 004da8d3 */
    restore_menu_inline_capacity_before(0x004da8d3u,f);
    f->r[0]=MI_U32((0x007770e8u+f->r[6]));
    /* 004da8d9 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004da8db */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004da91b;
    /* 004da8dd */
    f->r[7]=(0x00000032u+f->r[5]);
    /* 004da8e0 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004da8e2 */
    if(!mi_signed_less(f->flags))goto mi_004da91b;
    /* 004da8e4 */
    f->r[0]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004da8ea */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004da8ec */
    if((f->flags&MI_ZF))goto mi_004da901;
    /* 004da8ee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004da8ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da8f4u;f->boundary(0x004da8efu,0x00561a2bu,f);
    /* 004da8f4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004da8f7 */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004da901:;
    /* 004da901 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004da904 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004da906 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004da907 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004da90cu;f->boundary(0x004da907u,0x00561a1du,f);
    /* 004da90c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004da90f */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004da915 */
    MI_U32((0x007770e8u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004da91b:;
    /* 004da91b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004da921 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004da923 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004da925 */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004da92b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004da92f */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004da937 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004daa9a;
    /* 004da93d */
    f->r[5]=(0x00776fbau+f->r[6]);
mi_004da943:;
    /* 004da943 */
    a=f->r[2];b=MI_U32((0x007770e4u+f->r[6]));t=mi_alu(f,a,b,32,1);
    /* 004da949 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004daba1;
    /* 004da94f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004da953 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004da957 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004da95b */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da95f */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004da961 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004da965 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004da967 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004da96b */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004da96f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004da971 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da975 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da979 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004da97d */
    __asm { fld st(0) }
    /* 004da97f */
    __asm { fmul st(0),st(1) }
    /* 004da981 */
    __asm { fld st(2) }
    /* 004da983 */
    __asm { fmul st(0),st(3) }
    /* 004da985 */
    __asm { faddp st(1),st(0) }
    /* 004da987 */
    __asm { fsqrt }
    /* 004da989 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da98d */
    __asm { fstp st(0) }
    /* 004da98f */
    __asm { fstp st(0) }
    /* 004da991 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da995 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004da99b */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da99d */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004da9a0 */
    if(!(f->flags&MI_ZF))goto mi_004daa78;
    /* 004da9a6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004da9ac */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004da9ae */
    if(!(f->flags&MI_ZF))goto mi_004da9b7;
    /* 004da9b0 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004da9b2 */
    goto mi_004daa78;
mi_004da9b7:;
    /* 004da9b7 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da9bb */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004da9bf */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da9c3 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da9c7 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004da9cf */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004da9d3 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004da9d7 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da9dd */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004da9e1 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004da9e3 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004da9e6 */
    if((f->flags&MI_ZF))goto mi_004daa74;
    /* 004da9ec */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da9f0 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004da9f6 */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004da9fa */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004da9fe */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004daa01 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004daa03 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004daa07:;
    /* 004daa07 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004daa0b */
    __asm { fmul st(0),st(1) }
    /* 004daa0d */
    __asm { fadd st(0),st(2) }
    /* 004daa0f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004daa14u;f->boundary(0x004daa0fu,0x0056133cu,f);
    /* 004daa14 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004daa18 */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004daa1e */
    __asm { fmul st(0),st(1) }
    /* 004daa20 */
    MI_U16((0x00000000u+f->r[7]+f->r[1]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004daa24 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004daa28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004daa2du;f->boundary(0x004daa28u,0x0056133cu,f);
    /* 004daa2d */
    f->r[2]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004daa33 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004daa36 */
    __asm { fstp st(0) }
    /* 004daa38 */
    MI_U16((0xfffffffcu+f->r[7]+f->r[2]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004daa3d */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004daa41 */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004daa47 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004daa4a */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004daa4e */
    MI_U8((0xfffffffeu+f->r[7]+f->r[1]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004daa52 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004daa54 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004daa5a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004daa5c */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004daa5d */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004daa61 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004daa65 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004daa69 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004daa6b */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004daa6e */
    if(!(f->flags&MI_ZF))goto mi_004daa07;
    /* 004daa70 */
    __asm { fstp st(0) }
    /* 004daa72 */
    __asm { fstp st(0) }
mi_004daa74:;
    /* 004daa74 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
mi_004daa78:;
    /* 004daa78 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004daa7c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004daa7e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004daa84 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004daa86 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004daa87 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004daa8a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004daa8c */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004daa90 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004daa94 */
    if(mi_signed_less(f->flags))goto mi_004da943;
mi_004daa9a:;
    /* 004daa9a */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004daa9d */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004daaa3 */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004daaaa */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004daab1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004daab7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004daab9 */
    MI_U8((0x00000034u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 004daabd */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004daba1;
    /* 004daac3 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004daacb */
    f->r[7]=0x0077f748u;
mi_004daad0:;
    /* 004daad0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000001u+f->r[7])))&0xffu)<<0);
    /* 004daad3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004daad5 */
    if((f->flags&MI_ZF))goto mi_004dab89;
    /* 004daadb */
    a=MI_U8((0x00000000u+f->r[7]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004daade */
    if(!(f->flags&MI_ZF))goto mi_004dab89;
    /* 004daae4 */
    f->r[2]=MI_U32((0x0126517cu));
    /* 004daaea */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004daaec */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[7])))&0xffffu)<<0);
    /* 004daaf0 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004daaf2 */
    f->r[1]=f->r[0];
    /* 004daaf4 */
    if(mi_signed_less(f->flags))goto mi_004daaf8;
    /* 004daaf6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004daaf8:;
    /* 004daaf8 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004daafb */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004daafd */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[1]));
    /* 004dab04 */
    if(mi_signed_less(f->flags))goto mi_004dab08;
    /* 004dab06 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004dab08:;
    /* 004dab08 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[7]));
    /* 004dab0c */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[7]));
    /* 004dab10 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004dab13 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004dab15 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 004dab1c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb8u+f->r[6]));
    /* 004dab23 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dab25 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dab27 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004dab2b */
    if(mi_signed_less(f->flags))goto mi_004dab4c;
    /* 004dab2d */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dab2f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dab4c;
    /* 004dab31 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fbau+f->r[6]));
    /* 004dab38 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004dab3a */
    if(mi_signed_less(f->flags))goto mi_004dab4c;
    /* 004dab3c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dab3e */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dab4c;
    /* 004dab40 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[4])))&0xffffu)<<0);
    /* 004dab45 */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_004dab4c:;
    /* 004dab4c */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004dab50 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004dab55 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004dab58 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb2u+f->r[6]+f->r[0]*2u));
    /* 004dab60 */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[0]*2u);
    /* 004dab67 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dab69 */
    if(mi_signed_less(f->flags))goto mi_004dab89;
    /* 004dab6b */
    a=f->r[5];b=MI_U32((0x00000050u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004dab6f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dab89;
    /* 004dab71 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[0]));
    /* 004dab75 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004dab77 */
    if(mi_signed_less(f->flags))goto mi_004dab89;
    /* 004dab79 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dab7b */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dab89;
    /* 004dab7d */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[4])))&0xffffu)<<0);
    /* 004dab82 */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_004dab89:;
    /* 004dab89 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004dab8d */
    a=f->r[7];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004dab90 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004dab91 */
    a=f->r[7];b=0x007843d8u;t=mi_alu(f,a,b,32,1);
    /* 004dab97 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004dab9b */
    if(mi_signed_less(f->flags))goto mi_004daad0;
mi_004daba1:;
    /* 004daba1 */
    MI_U32((0x005d2a2cu))=(uint32_t)(0x00000000u);
mi_004dabab:;
    /* 004dabab */
    MI_U32((0x005e1db0u))=(uint32_t)(0xffffffffu);
    /* 004dabb5 */
    goto mi_004db028;
mi_004dabba:;
    /* 004dabba */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e00u)))&0xffu)<<0);
    /* 004dabbf */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dabc1 */
    if((f->flags&MI_ZF))goto mi_004daf90;
    /* 004dabc7 */
    f->r[0]=MI_U32((0x005e1db0u));
    /* 004dabcc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dabce */
    if(mi_signed_less(f->flags))goto mi_004db028;
    /* 004dabd4 */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004dabda */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 004dabe0 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dabe2 */
    if(mi_signed_less(f->flags))goto mi_004db01e;
    /* 004dabe8 */
    f->r[7]=MI_U32((0x00e38d9cu));
    /* 004dabee */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dabf0 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004dabf2 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dabf4 */
    if(!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004dabfa */
    f->r[1]=MI_U32((0x00e38e30u));
    /* 004dac00 */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 004dac06 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dac08 */
    if(mi_signed_less(f->flags))goto mi_004db01e;
    /* 004dac0e */
    f->r[7]=MI_U32((0x00e38d98u));
    /* 004dac14 */
    a=f->r[7];b=MI_U32((0x005d2a20u));t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004dac1a */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004dac1c */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dac1e */
    if(!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004dac24 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dac26 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004dac2c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dac2e */
    if(!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004dac34 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004dac37 */
    f->r[7]=0x00000001u;
    /* 004dac3c */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[2]*2u);
    /* 004dac43 */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004dac47 */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004dac4b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004dac4d */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004dac4f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004dac55 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dac57 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dac5b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dacb8;
    /* 004dac5d */
    f->r[2]=(0x00776fb8u+f->r[6]);
mi_004dac63:;
    /* 004dac63 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004dac66 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004dac6a */
    f->r[0]=f->r[3];
    /* 004dac6c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004dac6e */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004dac70 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dac72 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dac76 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dac7c;
    /* 004dac78 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004dac7c:;
    /* 004dac7c */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004dac80 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004dac84 */
    f->r[0]=f->r[3];
    /* 004dac86 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004dac88 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004dac8a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dac8c */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dac90;
    /* 004dac8e */
    f->r[0]=f->r[1];
mi_004dac90:;
    /* 004dac90 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004dac94 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dac96 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004daca3;
    /* 004dac98 */
    f->r[5]=(0x00000001u+f->r[1]+f->r[5]*1u);
    /* 004dac9c */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004daca1 */
    goto mi_004dacac;
mi_004daca3:;
    /* 004daca3 */
    f->r[5]=(0x00000001u+f->r[0]+f->r[5]*1u);
    /* 004daca7 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004dacac:;
    /* 004dacac */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004dacb0 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004dacb1 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dacb4 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dacb6 */
    if(mi_signed_less(f->flags))goto mi_004dac63;
mi_004dacb8:;
    /* 004dacb8 */
    restore_menu_inline_capacity_before(0x004dacb8u,f);
    f->r[0]=MI_U32((0x007770e8u+f->r[6]));
    /* 004dacbe */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dacc0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dad00;
    /* 004dacc2 */
    f->r[7]=(0x00000032u+f->r[5]);
    /* 004dacc5 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dacc7 */
    if(!mi_signed_less(f->flags))goto mi_004dad00;
    /* 004dacc9 */
    f->r[0]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004daccf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dacd1 */
    if((f->flags&MI_ZF))goto mi_004dace6;
    /* 004dacd3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dacd4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dacd9u;f->boundary(0x004dacd4u,0x00561a2bu,f);
    /* 004dacd9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dacdc */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004dace6:;
    /* 004dace6 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004dace9 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004daceb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dacec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dacf1u;f->boundary(0x004dacecu,0x00561a1du,f);
    /* 004dacf1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dacf4 */
    MI_U32((0x00776fb4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004dacfa */
    MI_U32((0x007770e8u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004dad00:;
    /* 004dad00 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004dad06 */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004dad0c */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004dad0e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004dad16 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004dad1e */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004dae80;
    /* 004dad24 */
    f->r[5]=(0x00776fbau+f->r[6]);
mi_004dad2a:;
    /* 004dad2a */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004dad2e */
    f->r[0]=MI_U32((0x007770e4u+f->r[6]));
    /* 004dad34 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dad36 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db01e;
    /* 004dad3c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004dad40 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004dad44 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004dad48 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dad4c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004dad4e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004dad52 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004dad54 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004dad58 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dad5c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004dad5e */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dad62 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dad66 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004dad6a */
    __asm { fld st(0) }
    /* 004dad6c */
    __asm { fmul st(0),st(1) }
    /* 004dad6e */
    __asm { fld st(2) }
    /* 004dad70 */
    __asm { fmul st(0),st(3) }
    /* 004dad72 */
    __asm { faddp st(1),st(0) }
    /* 004dad74 */
    __asm { fsqrt }
    /* 004dad76 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dad7a */
    __asm { fstp st(0) }
    /* 004dad7c */
    __asm { fstp st(0) }
    /* 004dad7e */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dad82 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004dad88 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dad8a */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004dad8d */
    if(!(f->flags&MI_ZF))goto mi_004dae5a;
    /* 004dad93 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004dad99 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dad9b */
    if((f->flags&MI_ZF))goto mi_004dae5a;
    /* 004dada1 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dada5 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004dada9 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dadad */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dadb1 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004dadb5 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004dadb9 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dadbd */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dadc3 */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004dadc7 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dadc9 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004dadcc */
    if((f->flags&MI_ZF))goto mi_004dae5a;
    /* 004dadd2 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dadd6 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004daddc */
    f->r[7]=MI_U32((0x00000018u+f->r[4]));
    /* 004dade0 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dade4 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004dade7 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004dade9 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004daded:;
    /* 004daded */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dadf1 */
    __asm { fmul st(0),st(1) }
    /* 004dadf3 */
    __asm { fadd st(0),st(2) }
    /* 004dadf5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dadfau;f->boundary(0x004dadf5u,0x0056133cu,f);
    /* 004dadfa */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dadfe */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004dae04 */
    __asm { fmul st(0),st(1) }
    /* 004dae06 */
    MI_U16((0x00000000u+f->r[7]+f->r[1]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dae0a */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004dae0e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dae13u;f->boundary(0x004dae0eu,0x0056133cu,f);
    /* 004dae13 */
    f->r[2]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004dae19 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004dae1c */
    __asm { fstp st(0) }
    /* 004dae1e */
    MI_U16((0xfffffffcu+f->r[7]+f->r[2]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dae23 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004dae27 */
    f->r[1]=MI_U32((0x00776fb4u+f->r[6]));
    /* 004dae2d */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004dae30 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004dae34 */
    MI_U8((0xfffffffeu+f->r[7]+f->r[1]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004dae38 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004dae3a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00776fb0u+f->r[6])))&0xffu)<<0);
    /* 004dae40 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dae42 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004dae43 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004dae47 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dae4b */
    p=(0x00000014u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004dae4f */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dae51 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004dae54 */
    if(!(f->flags&MI_ZF))goto mi_004daded;
    /* 004dae56 */
    __asm { fstp st(0) }
    /* 004dae58 */
    __asm { fstp st(0) }
mi_004dae5a:;
    /* 004dae5a */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004dae5e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dae60 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004dae66 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dae68 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dae6c */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004dae70 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004dae71 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004dae74 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dae76 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dae7a */
    if(mi_signed_less(f->flags))goto mi_004dad2a;
mi_004dae80:;
    /* 004dae80 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004dae84 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004dae87 */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dae8e */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dae95 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004dae9b */
    MI_U32((0x007770e4u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004daea1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004daea3 */
    MI_U8((0x00000034u+f->r[4]))=(uint8_t)((f->r[0]&0xffu));
    /* 004daea7 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004db01e;
    /* 004daead */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004daeb5 */
    f->r[7]=0x0077f748u;
mi_004daeba:;
    /* 004daeba */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000001u+f->r[7])))&0xffu)<<0);
    /* 004daebd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004daebf */
    if((f->flags&MI_ZF))goto mi_004daf73;
    /* 004daec5 */
    a=MI_U8((0x00000000u+f->r[7]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004daec8 */
    if(!(f->flags&MI_ZF))goto mi_004daf73;
    /* 004daece */
    f->r[2]=MI_U32((0x0126517cu));
    /* 004daed4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004daed6 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000000cu+f->r[7])))&0xffffu)<<0);
    /* 004daeda */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004daedc */
    f->r[1]=f->r[0];
    /* 004daede */
    if(mi_signed_less(f->flags))goto mi_004daee2;
    /* 004daee0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004daee2:;
    /* 004daee2 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004daee5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004daee7 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0cu+f->r[1]));
    /* 004daeee */
    if(mi_signed_less(f->flags))goto mi_004daef2;
    /* 004daef0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004daef2:;
    /* 004daef2 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[7]));
    /* 004daef6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[7]));
    /* 004daefa */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004daefd */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004daeff */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a0eu+f->r[0]));
    /* 004daf06 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb8u+f->r[6]));
    /* 004daf0d */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004daf0f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004daf11 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004daf15 */
    if(mi_signed_less(f->flags))goto mi_004daf36;
    /* 004daf17 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004daf19 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004daf36;
    /* 004daf1b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fbau+f->r[6]));
    /* 004daf22 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004daf24 */
    if(mi_signed_less(f->flags))goto mi_004daf36;
    /* 004daf26 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004daf28 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004daf36;
    /* 004daf2a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[4])))&0xffffu)<<0);
    /* 004daf2f */
    MI_U16((0x007770ecu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_004daf36:;
    /* 004daf36 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004daf3a */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004daf3f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004daf42 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00776fb2u+f->r[6]+f->r[0]*2u));
    /* 004daf4a */
    f->r[0]=(0x00776fb0u+f->r[6]+f->r[0]*2u);
    /* 004daf51 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004daf53 */
    if(mi_signed_less(f->flags))goto mi_004daf73;
    /* 004daf55 */
    a=f->r[5];b=MI_U32((0x00000050u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004daf59 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004daf73;
    /* 004daf5b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[0]));
    /* 004daf5f */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004daf61 */
    if(mi_signed_less(f->flags))goto mi_004daf73;
    /* 004daf63 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004daf65 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004daf73;
    /* 004daf67 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[4])))&0xffffu)<<0);
    /* 004daf6c */
    MI_U16((0x007770eeu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
mi_004daf73:;
    /* 004daf73 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004daf77 */
    a=f->r[7];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004daf7a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004daf7b */
    a=f->r[7];b=0x007843d8u;t=mi_alu(f,a,b,32,1);
    /* 004daf81 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004daf85 */
    if(mi_signed_less(f->flags))goto mi_004daeba;
    /* 004daf8b */
    goto mi_004db01e;
mi_004daf90:;
    /* 004daf90 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004daf95 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004daf97 */
    if((f->flags&MI_ZF))goto mi_004db028;
    /* 004daf9d */
    f->r[2]=(0x00000013u+f->r[4]);
    /* 004dafa1 */
    f->r[1]=(0x00776fb0u+f->r[6]);
    /* 004dafa7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dafa8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dafa9 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dafaa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dafafu;f->boundary(0x004dafaau,0x004ddbd0u,f);
    /* 004dafaf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dafb1 */
    if(mi_signed_less(f->flags))goto mi_004db028;
    /* 004dafb3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000013u+f->r[4])))&0xffu)<<0);
    /* 004dafb7 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dafb9 */
    if((f->flags&MI_ZF))goto mi_004db028;
    /* 004dafbb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dafbc */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004dafc1 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004dafc4 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004dafc7 */
    f->r[1]=(0x00776fb0u+f->r[0]*4u);
    /* 004dafce */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dafd3u;f->boundary(0x004dafceu,0x004dd930u,f);
    /* 004dafd3 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004dafd8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004dafdb */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004dafde */
    f->r[1]=(0x00776fb0u+f->r[1]*4u);
    /* 004dafe5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dafeau;f->boundary(0x004dafe5u,0x004dd8d0u,f);
    /* 004dafea */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dafec */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db028;
    /* 004dafee */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004daff3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004daff5 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004daff8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004daffb */
    f->r[1]=(0x00776fb0u+f->r[2]*4u);
    /* 004db002 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db007u;f->boundary(0x004db002u,0x004dd900u,f);
    /* 004db007 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004db00c */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db00f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db012 */
    f->r[1]=(0x00776fb0u+f->r[0]*4u);
    /* 004db019 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db01eu;f->boundary(0x004db019u,0x004dd8e0u,f);
mi_004db01e:;
    /* 004db01e */
    MI_U32((0x005d2a2cu))=(uint32_t)(0x00000000u);
mi_004db028:;
    /* 004db028 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004db02d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db030 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db033 */
    f->r[1]=(0x00776fb0u+f->r[1]*4u);
    /* 004db03a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db03fu;f->boundary(0x004db03au,0x004dd910u,f);
    /* 004db03f */
    a=MI_U32((0x00000038u+f->r[4]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db043 */
    if(!(f->flags&MI_ZF))goto mi_004dd55e;
    /* 004db049 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004db04e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db051 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db054 */
    f->r[1]=(0x00776fb0u+f->r[2]*4u);
    /* 004db05b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db060u;f->boundary(0x004db05bu,0x004dd920u,f);
    /* 004db060 */
    a=MI_U32((0x0000003cu+f->r[4]));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db064 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db06a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004db06b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004db06c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004db06d */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004db074 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004db075 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db078 */
    f->r[4]+=4;return;
mi_004db079:;
    /* 004db079 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e04u)))&0xffu)<<0);
    /* 004db07e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004db080 */
    if((f->flags&MI_ZF))goto mi_004dbb3a;
    /* 004db086 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004db08b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004db08d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db090 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db093 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004db096 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[0])))&0xffu)<<0);
    /* 004db09c */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004db09d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004db09f */
    if(!mi_signed_less(f->flags))goto mi_004db5f1;
    /* 004db0a5 */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004db0ab */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 004db0b1 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004db0b3 */
    if(mi_signed_less(f->flags))goto mi_004dd8c3;
    /* 004db0b9 */
    f->r[6]=MI_U32((0x00e38d9cu));
    /* 004db0bf */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db0c1 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004db0c3 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004db0c5 */
    if(!mi_signed_less(f->flags))goto mi_004dd8c3;
    /* 004db0cb */
    f->r[1]=MI_U32((0x00e38e30u));
    /* 004db0d1 */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 004db0d7 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004db0d9 */
    if(mi_signed_less(f->flags))goto mi_004dd8c3;
    /* 004db0df */
    f->r[6]=MI_U32((0x00e38d98u));
    /* 004db0e5 */
    f->r[3]=MI_U32((0x005d2a20u));
    /* 004db0eb */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004db0ed */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004db0ef */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004db0f1 */
    if(mi_signed_less(f->flags))goto mi_004db34d;
    /* 004db0f7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004db0f8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004db0f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004db0fa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004db0fb */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db0fe */
    f->r[4]+=4;return;
mi_004db0ff:;
    /* 004db0ff */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004db102 */
    if(!(f->flags&MI_ZF))goto mi_004db19f;
    /* 004db108 */
    f->r[0]=MI_U32((0x00775920u));
    /* 004db10d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db10f */
    if(!(f->flags&MI_ZF))goto mi_004db136;
    /* 004db111 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004db116 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db118 */
    a=f->r[0];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004db11b */
    a=0x005c8c60u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db120 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db121 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db126 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db12bu;f->boundary(0x004db126u,0x00425c40u,f);
    /* 004db12b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db12e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db130 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004db136:;
    /* 004db136 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004db13c */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db13e */
    a=f->r[1];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004db141 */
    a=0x005c33c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db146 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db147 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db14c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db151u;f->boundary(0x004db14cu,0x004261d0u,f);
    /* 004db151 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db154 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db156 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db15c */
    f->r[2]=MI_U32((0x00775918u));
    /* 004db162 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db163 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db164 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db169u;f->boundary(0x004db164u,0x00407620u,f);
    /* 004db169 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db16c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db171u;f->boundary(0x004db16cu,0x00407870u,f);
    /* 004db171 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db173 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db179 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004db17a */
    MI_U32((0x00775928u))=(uint32_t)(f->r[7]);
    /* 004db180 */
    MI_U32((0x0077591cu))=(uint32_t)(f->r[0]);
mi_004db185:;
    /* 004db185 */
    a=f->r[0];b=0x00000062u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004db188 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004db18a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004db18b */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0077f754u+f->r[0])))&0xffffu)<<0);
    /* 004db192 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004db193 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004db194 */
    MI_U32((0x00775924u))=(uint32_t)(f->r[1]);
    /* 004db19a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004db19b */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db19e */
    f->r[4]+=4;return;
mi_004db19f:;
    /* 004db19f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db1a1 */
    if(!(f->flags&MI_ZF))goto mi_004db31e;
    /* 004db1a7 */
    f->r[0]=MI_U32((0x00e38ddcu));
    /* 004db1ac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db1ae */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db1be;
    /* 004db1b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004db1b1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004db1b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004db1b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004db1b4 */
    MI_U32((0x00e38ddcu))=(uint32_t)(f->r[0]);
    /* 004db1b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004db1ba */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db1bd */
    f->r[4]+=4;return;
mi_004db1be:;
    /* 004db1be */
    f->r[2]=MI_U32((0x0077591cu));
    /* 004db1c4 */
    a=f->r[2];b=0x00000062u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004db1c7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f748u+f->r[2])))&0xffu)<<0);
    /* 004db1cd */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004db1cf */
    if(!(f->flags&MI_ZF))goto mi_004db1f6;
    /* 004db1d1 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004db1d6 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db1d8 */
    a=f->r[0];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004db1db */
    a=0x005c8c60u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db1e0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db1e1 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db1e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db1ebu;f->boundary(0x004db1e6u,0x00425c40u,f);
    /* 004db1eb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db1ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db1f0 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004db1f6:;
    /* 004db1f6 */
    f->r[3]=MI_U32((0x0077591cu));
    /* 004db1fc */
    f->r[6]=f->r[3];
    /* 004db1fe */
    a=f->r[6];b=0x00000062u;t=mi_imul(f,a,b);f->r[6]=t;
    /* 004db201 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f748u+f->r[6])))&0xffu)<<0);
    /* 004db207 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004db209 */
    if(!(f->flags&MI_ZF))goto mi_004db2a1;
    /* 004db20f */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004db215 */
    a=0x00000006u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db217 */
    a=f->r[1];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004db21a */
    a=0x005c3dc0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db21f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db220 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db225 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db22au;f->boundary(0x004db225u,0x004261d0u,f);
    /* 004db22a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db22d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db22f */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db235 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0077f760u+f->r[6])))&0xffu)<<0);
    /* 004db23b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004db23d */
    if(!(f->flags&MI_ZF))goto mi_004db267;
    /* 004db23f */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004db245 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db247 */
    a=f->r[2];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db24a */
    a=0x005c3ec0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db24f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db250 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db255 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db25au;f->boundary(0x004db255u,0x004261d0u,f);
    /* 004db25a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db25d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db25f */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db265 */
    goto mi_004db28c;
mi_004db267:;
    /* 004db267 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 004db26c */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db26e */
    a=f->r[0];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004db271 */
    a=0x005c4048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db276 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db277 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db27c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db281u;f->boundary(0x004db27cu,0x004261d0u,f);
    /* 004db281 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db284 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db286 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004db28c:;
    /* 004db28c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db28d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db292u;f->boundary(0x004db28du,0x00442a40u,f);
    /* 004db292 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db295 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db29au;f->boundary(0x004db295u,0x00442ef0u,f);
    /* 004db29a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db29fu;f->boundary(0x004db29au,0x00442f80u,f);
    /* 004db29f */
    goto mi_004db2f6;
mi_004db2a1:;
    /* 004db2a1 */
    a=(f->r[0]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 004db2a3 */
    if(!(f->flags&MI_ZF))goto mi_004db2cc;
    /* 004db2a5 */
    f->r[1]=MI_U32((0x00e38dacu));
    /* 004db2ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2ac */
    a=f->r[1];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004db2af */
    a=0x005c3e88u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2b5 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db2bfu;f->boundary(0x004db2bau,0x004261d0u,f);
    /* 004db2bf */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db2c2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db2c4 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db2ca */
    goto mi_004db2f6;
mi_004db2cc:;
    /* 004db2cc */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004db2ce */
    if(!(f->flags&MI_ZF))goto mi_004db2f6;
    /* 004db2d0 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004db2d6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2d8 */
    a=f->r[2];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db2db */
    a=0x005c3e68u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2e0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2e1 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db2ebu;f->boundary(0x004db2e6u,0x004261d0u,f);
    /* 004db2eb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db2ee */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db2f0 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004db2f6:;
    /* 004db2f6 */
    f->r[0]=MI_U32((0x0077591cu));
    /* 004db2fb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db2fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db303u;f->boundary(0x004db2feu,0x00407620u,f);
    /* 004db303 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db306 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db30bu;f->boundary(0x004db306u,0x00407870u,f);
    /* 004db30b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db30d */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db313 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004db314 */
    MI_U32((0x0077591cu))=(uint32_t)(f->r[0]);
    /* 004db319 */
    goto mi_004db185;
mi_004db31e:;
    /* 004db31e */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004db321 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004db327 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 004db32d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db32f */
    a=f->r[2];b=0xffffff8eu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db332 */
    a=0x005c3348u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db337 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db338 */
    a=0x000000dfu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db33d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db342u;f->boundary(0x004db33du,0x004261d0u,f);
    /* 004db342 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db345 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004db346 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004db347 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004db348 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004db349 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db34c */
    f->r[4]+=4;return;
mi_004db34d:;
    /* 004db34d */
    MI_U8((0x007770f2u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 004db354 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004db359 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004db35b */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db35e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db361 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004db364 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[0])))&0xffu)<<0);
    /* 004db36a */
    f->r[6]=(0x00776fb0u+f->r[0]);
    /* 004db370 */
    f->r[0]=f->r[1];
    /* 004db372 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db374 */
    if(!mi_signed_less(f->flags))goto mi_004db378;
    /* 004db376 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004db378:;
    /* 004db378 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db37e */
    a=(f->r[1]&0xffu);b=0x00000032u;t=mi_alu(f,a,b,8,1);
    /* 004db381 */
    if(!(f->flags&MI_CF))goto mi_004db899;
    /* 004db387 */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004db38d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004db38e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db390 */
    if(mi_signed_less(f->flags))goto mi_004db3b2;
    /* 004db392 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004db395 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db397 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004db398 */
    f->r[2]=(0x0000000eu+f->r[6]+f->r[2]*2u);
mi_004db39c:;
    /* 004db39c */
    f->r[7]=(0xfffffffau+f->r[2]);
    /* 004db39f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004db3a0 */
    f->r[3]=f->r[7];
    /* 004db3a2 */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 004db3a4 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 004db3a6 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[3])))&0xffffu)<<0);
    /* 004db3aa */
    MI_U16((0x00000004u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004db3ae */
    f->r[2]=f->r[7];
    /* 004db3b0 */
    if(!(f->flags&MI_ZF))goto mi_004db39c;
mi_004db3b2:;
    /* 004db3b2 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000001cu+f->r[4])))&0xffffu)<<0);
    /* 004db3b7 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 004db3bc */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004db3bf */
    f->r[7]=0x00000001u;
    /* 004db3c4 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004db3c6 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 004db3c9 */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004db3cd */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004db3d1 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db3d7 */
    a=(f->r[3]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004db3d9 */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[3]&0xffu))&0xffu)<<0);
    /* 004db3db */
    MI_U8((0x00000141u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004db3e1 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004db3e6 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db3e8 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db3ec */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db446;
    /* 004db3ee */
    f->r[2]=(0x00000008u+f->r[6]);
mi_004db3f1:;
    /* 004db3f1 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004db3f4 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004db3f8 */
    f->r[0]=f->r[3];
    /* 004db3fa */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db3fc */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db3fe */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db400 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db404 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db40a;
    /* 004db406 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004db40a:;
    /* 004db40a */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004db40e */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004db412 */
    f->r[0]=f->r[3];
    /* 004db414 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db416 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db418 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db41a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db41e;
    /* 004db41c */
    f->r[0]=f->r[1];
mi_004db41e:;
    /* 004db41e */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 004db422 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db424 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db431;
    /* 004db426 */
    f->r[5]=(0x00000001u+f->r[1]+f->r[5]*1u);
    /* 004db42a */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004db42f */
    goto mi_004db43a;
mi_004db431:;
    /* 004db431 */
    f->r[5]=(0x00000001u+f->r[0]+f->r[5]*1u);
    /* 004db435 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004db43a:;
    /* 004db43a */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 004db43e */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004db43f */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db442 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db444 */
    if(mi_signed_less(f->flags))goto mi_004db3f1;
mi_004db446:;
    /* 004db446 */
    restore_menu_inline_capacity_before(0x004db446u,f);
    f->r[0]=MI_U32((0x00000138u+f->r[6]));
    /* 004db44c */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db44e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db485;
    /* 004db450 */
    f->r[7]=(0x00000032u+f->r[5]);
    /* 004db453 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db455 */
    if(!mi_signed_less(f->flags))goto mi_004db485;
    /* 004db457 */
    f->r[0]=MI_U32((0x00000004u+f->r[6]));
    /* 004db45a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db45c */
    if((f->flags&MI_ZF))goto mi_004db46e;
    /* 004db45e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db45f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db464u;f->boundary(0x004db45fu,0x00561a2bu,f);
    /* 004db464 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db467 */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004db46e:;
    /* 004db46e */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004db471 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004db473 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db474 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db479u;f->boundary(0x004db474u,0x00561a1du,f);
    /* 004db479 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db47c */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004db47f */
    MI_U32((0x00000138u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004db485:;
    /* 004db485 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db48b */
    MI_U32((0x00000134u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004db491 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004db493 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004db49b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004db4a3 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004db5e8;
    /* 004db4a9 */
    f->r[5]=(0x0000000au+f->r[6]);
mi_004db4ac:;
    /* 004db4ac */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004db4b0 */
    f->r[0]=MI_U32((0x00000134u+f->r[6]));
    /* 004db4b6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db4b8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db899;
    /* 004db4be */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004db4c2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004db4c6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004db4ca */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db4ce */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004db4d0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004db4d4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db4d6 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004db4da */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db4de */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004db4e0 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db4e4 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db4e8 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004db4ec */
    __asm { fld st(0) }
    /* 004db4ee */
    __asm { fmul st(0),st(1) }
    /* 004db4f0 */
    __asm { fld st(2) }
    /* 004db4f2 */
    __asm { fmul st(0),st(3) }
    /* 004db4f4 */
    __asm { faddp st(1),st(0) }
    /* 004db4f6 */
    __asm { fsqrt }
    /* 004db4f8 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004db4fc */
    __asm { fstp st(0) }
    /* 004db4fe */
    __asm { fstp st(0) }
    /* 004db500 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004db504 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004db50a */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004db50c */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004db50f */
    if(!(f->flags&MI_ZF))goto mi_004db5c2;
    /* 004db515 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004db518 */
    if((f->flags&MI_ZF))goto mi_004db5c2;
    /* 004db51e */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db522 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004db526 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004db52a */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004db52e */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db532 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004db536 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004db53a */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004db540 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004db544 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004db546 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004db549 */
    if((f->flags&MI_ZF))goto mi_004db5c2;
    /* 004db54b */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db54f */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004db555 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004db559 */
    p=(0x00000038u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db55d */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004db560 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004db562 */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004db566:;
    /* 004db566 */
    __asm { fld st(0) }
    /* 004db568 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fmul dword ptr [eax] }
    /* 004db56c */
    __asm { fadd st(0),st(2) }
    /* 004db56e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db573u;f->boundary(0x004db56eu,0x0056133cu,f);
    /* 004db573 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fmul dword ptr [eax] }
    /* 004db577 */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004db57a */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004db57e */
    MI_U16((0x00000000u+f->r[7]+f->r[1]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004db582 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db587u;f->boundary(0x004db582u,0x0056133cu,f);
    /* 004db587 */
    f->r[2]=MI_U32((0x00000004u+f->r[6]));
    /* 004db58a */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004db58d */
    MI_U16((0xfffffffcu+f->r[7]+f->r[2]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004db592 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004db596 */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004db599 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004db59c */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004db5a0 */
    MI_U8((0xfffffffeu+f->r[7]+f->r[1]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004db5a4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004db5a6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004db5a8 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db5aa */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004db5ab */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004db5af */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db5b3 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004db5b7 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004db5b9 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004db5bc */
    if(!(f->flags&MI_ZF))goto mi_004db566;
    /* 004db5be */
    __asm { fstp st(0) }
    /* 004db5c0 */
    __asm { fstp st(0) }
mi_004db5c2:;
    /* 004db5c2 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004db5c6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004db5c8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db5ce */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004db5d0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db5d4 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004db5d8 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004db5d9 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004db5dc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db5de */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db5e2 */
    if(mi_signed_less(f->flags))goto mi_004db4ac;
mi_004db5e8:;
    /* 004db5e8 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004db5ec */
    goto mi_004db88c;
mi_004db5f1:;
    /* 004db5f1 */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 004db5f7 */
    f->r[6]=(0x00776fb0u+f->r[0]);
    /* 004db5fd */
    f->r[0]=MI_U32((0x00e38e34u));
    /* 004db602 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004db604 */
    if(mi_signed_less(f->flags))goto mi_004db899;
    /* 004db60a */
    f->r[7]=MI_U32((0x00e38d9cu));
    /* 004db610 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db612 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004db614 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db616 */
    if(!mi_signed_less(f->flags))goto mi_004db899;
    /* 004db61c */
    f->r[0]=MI_U32((0x00e38e30u));
    /* 004db621 */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 004db627 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004db629 */
    if(mi_signed_less(f->flags))goto mi_004db899;
    /* 004db62f */
    f->r[7]=MI_U32((0x00e38d98u));
    /* 004db635 */
    f->r[5]=MI_U32((0x005d2a20u));
    /* 004db63b */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004db63d */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004db63f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db641 */
    if(!mi_signed_less(f->flags))goto mi_004db899;
    /* 004db647 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004db649 */
    if(mi_signed_less(f->flags))goto mi_004db899;
    /* 004db64f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004db651 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db657 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db659 */
    if(!mi_signed_less(f->flags))goto mi_004db899;
    /* 004db65f */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x0000001cu+f->r[4])))&0xffffu)<<0);
    /* 004db664 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004db667 */
    f->r[7]=0x00000001u;
    /* 004db66c */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004db66e */
    f->r[0]=(0x00000000u+f->r[6]+f->r[1]*2u);
    /* 004db671 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 004db676 */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004db67a */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004db67e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004db680 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db686 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db688 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db68c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db6e6;
    /* 004db68e */
    f->r[2]=(0x00000008u+f->r[6]);
mi_004db691:;
    /* 004db691 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004db694 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004db698 */
    f->r[0]=f->r[3];
    /* 004db69a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db69c */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db69e */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db6a0 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db6a4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db6aa;
    /* 004db6a6 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_004db6aa:;
    /* 004db6aa */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004db6ae */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004db6b2 */
    f->r[0]=f->r[3];
    /* 004db6b4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db6b6 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db6b8 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db6ba */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db6be;
    /* 004db6bc */
    f->r[0]=f->r[1];
mi_004db6be:;
    /* 004db6be */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004db6c2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db6c4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db6d1;
    /* 004db6c6 */
    f->r[5]=(0x00000001u+f->r[1]+f->r[5]*1u);
    /* 004db6ca */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004db6cf */
    goto mi_004db6da;
mi_004db6d1:;
    /* 004db6d1 */
    f->r[5]=(0x00000001u+f->r[0]+f->r[5]*1u);
    /* 004db6d5 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004db6da:;
    /* 004db6da */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 004db6de */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004db6df */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db6e2 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db6e4 */
    if(mi_signed_less(f->flags))goto mi_004db691;
mi_004db6e6:;
    /* 004db6e6 */
    restore_menu_inline_capacity_before(0x004db6e6u,f);
    f->r[0]=MI_U32((0x00000138u+f->r[6]));
    /* 004db6ec */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db6ee */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db725;
    /* 004db6f0 */
    f->r[7]=(0x00000032u+f->r[5]);
    /* 004db6f3 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db6f5 */
    if(!mi_signed_less(f->flags))goto mi_004db725;
    /* 004db6f7 */
    f->r[0]=MI_U32((0x00000004u+f->r[6]));
    /* 004db6fa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db6fc */
    if((f->flags&MI_ZF))goto mi_004db70e;
    /* 004db6fe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db6ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db704u;f->boundary(0x004db6ffu,0x00561a2bu,f);
    /* 004db704 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db707 */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004db70e:;
    /* 004db70e */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004db711 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004db713 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db714 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db719u;f->boundary(0x004db714u,0x00561a1du,f);
    /* 004db719 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db71c */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004db71f */
    MI_U32((0x00000138u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004db725:;
    /* 004db725 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db72b */
    MI_U32((0x00000134u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004db731 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004db733 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004db73b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004db743 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004db888;
    /* 004db749 */
    f->r[5]=(0x0000000au+f->r[6]);
mi_004db74c:;
    /* 004db74c */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004db750 */
    f->r[1]=MI_U32((0x00000134u+f->r[6]));
    /* 004db756 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db758 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db899;
    /* 004db75e */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004db762 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004db766 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004db76a */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db76e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004db770 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004db774 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db776 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004db77a */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db77e */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004db780 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db784 */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db788 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004db78c */
    __asm { fld st(0) }
    /* 004db78e */
    __asm { fmul st(0),st(1) }
    /* 004db790 */
    __asm { fld st(2) }
    /* 004db792 */
    __asm { fmul st(0),st(3) }
    /* 004db794 */
    __asm { faddp st(1),st(0) }
    /* 004db796 */
    __asm { fsqrt }
    /* 004db798 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004db79c */
    __asm { fstp st(0) }
    /* 004db79e */
    __asm { fstp st(0) }
    /* 004db7a0 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004db7a4 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004db7aa */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004db7ac */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004db7af */
    if(!(f->flags&MI_ZF))goto mi_004db862;
    /* 004db7b5 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004db7b8 */
    if((f->flags&MI_ZF))goto mi_004db862;
    /* 004db7be */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db7c2 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004db7c6 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004db7ca */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db7ce */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004db7d2 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004db7d6 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004db7da */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004db7e0 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004db7e4 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004db7e6 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004db7e9 */
    if((f->flags&MI_ZF))goto mi_004db862;
    /* 004db7eb */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db7ef */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004db7f5 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004db7f9 */
    p=(0x00000038u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db7fd */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004db800 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004db802 */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004db806:;
    /* 004db806 */
    __asm { fld st(0) }
    /* 004db808 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fmul dword ptr [eax] }
    /* 004db80c */
    __asm { fadd st(0),st(2) }
    /* 004db80e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db813u;f->boundary(0x004db80eu,0x0056133cu,f);
    /* 004db813 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fmul dword ptr [eax] }
    /* 004db817 */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004db81a */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004db81e */
    MI_U16((0x00000000u+f->r[7]+f->r[1]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004db822 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db827u;f->boundary(0x004db822u,0x0056133cu,f);
    /* 004db827 */
    f->r[2]=MI_U32((0x00000004u+f->r[6]));
    /* 004db82a */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004db82d */
    MI_U16((0xfffffffcu+f->r[7]+f->r[2]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004db832 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004db836 */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004db839 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004db83c */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004db840 */
    MI_U8((0xfffffffeu+f->r[7]+f->r[1]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004db844 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004db846 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004db848 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db84a */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004db84b */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004db84f */
    p=(0x00000024u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004db853 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004db857 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004db859 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004db85c */
    if(!(f->flags&MI_ZF))goto mi_004db806;
    /* 004db85e */
    __asm { fstp st(0) }
    /* 004db860 */
    __asm { fstp st(0) }
mi_004db862:;
    /* 004db862 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004db866 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004db868 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db86e */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004db870 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db874 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004db878 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004db879 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004db87c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db87e */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db882 */
    if(mi_signed_less(f->flags))goto mi_004db74c;
mi_004db888:;
    /* 004db888 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
mi_004db88c:;
    /* 004db88c */
    f->r[1]=f->r[6];
    /* 004db88e */
    MI_U32((0x00000134u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004db894 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db899u;f->boundary(0x004db894u,0x00407f90u,f);
mi_004db899:;
    /* 004db899 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004db89e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004db8a0 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db8a3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004db8a6 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004db8a9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[0])))&0xffu)<<0);
    /* 004db8af */
    f->r[6]=(0x00776fb0u+f->r[0]);
    /* 004db8b5 */
    f->r[0]=f->r[1];
    /* 004db8b7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db8b9 */
    if(!mi_signed_less(f->flags))goto mi_004db8bd;
    /* 004db8bb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004db8bd:;
    /* 004db8bd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db8c3 */
    a=(f->r[1]&0xffu);b=0x00000032u;t=mi_alu(f,a,b,8,1);
    /* 004db8c6 */
    if(!(f->flags&MI_CF))goto mi_004dbe0c;
    /* 004db8cc */
    a=f->r[1];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004db8d2 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004db8d3 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db8d5 */
    if(mi_signed_less(f->flags))goto mi_004db8f7;
    /* 004db8d7 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004db8da */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db8dc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004db8dd */
    f->r[2]=(0x0000000eu+f->r[6]+f->r[2]*2u);
mi_004db8e1:;
    /* 004db8e1 */
    f->r[7]=(0xfffffffau+f->r[2]);
    /* 004db8e4 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004db8e5 */
    f->r[3]=f->r[7];
    /* 004db8e7 */
    f->r[5]=MI_U32((0x00000000u+f->r[3]));
    /* 004db8e9 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 004db8eb */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000004u+f->r[3])))&0xffffu)<<0);
    /* 004db8ef */
    MI_U16((0x00000004u+f->r[2]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004db8f3 */
    f->r[2]=f->r[7];
    /* 004db8f5 */
    if(!(f->flags&MI_ZF))goto mi_004db8e1;
mi_004db8f7:;
    /* 004db8f7 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000001cu+f->r[4])))&0xffffu)<<0);
    /* 004db8fc */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 004db901 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004db904 */
    f->r[7]=0x00000001u;
    /* 004db909 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 004db90c */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004db910 */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004db914 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db91a */
    a=(f->r[3]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004db91c */
    f->r[0]=(f->r[0]&0xffffff00u)|((((f->r[3]&0xffu))&0xffu)<<0);
    /* 004db91e */
    MI_U8((0x00000141u+f->r[6]))=(uint8_t)((f->r[3]&0xffu));
    /* 004db924 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004db929 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004db92b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db92d */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db931 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db98b;
    /* 004db933 */
    f->r[2]=(0x00000008u+f->r[6]);
mi_004db936:;
    /* 004db936 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004db939 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004db93d */
    f->r[0]=f->r[5];
    /* 004db93f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db941 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db943 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db945 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004db949 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db94f;
    /* 004db94b */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_004db94f:;
    /* 004db94f */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004db953 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004db957 */
    f->r[0]=f->r[5];
    /* 004db959 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004db95b */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004db95d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004db95f */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004db963;
    /* 004db961 */
    f->r[0]=f->r[1];
mi_004db963:;
    /* 004db963 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004db967 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db969 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db976;
    /* 004db96b */
    f->r[3]=(0x00000001u+f->r[3]+f->r[1]*1u);
    /* 004db96f */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004db974 */
    goto mi_004db97f;
mi_004db976:;
    /* 004db976 */
    f->r[3]=(0x00000001u+f->r[3]+f->r[0]*1u);
    /* 004db97a */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004db97f:;
    /* 004db97f */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 004db983 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004db984 */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004db987 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db989 */
    if(mi_signed_less(f->flags))goto mi_004db936;
mi_004db98b:;
    /* 004db98b */
    restore_menu_inline_capacity_before(0x004db98bu,f);
    f->r[0]=MI_U32((0x00000138u+f->r[6]));
    /* 004db991 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db993 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004db9ca;
    /* 004db995 */
    f->r[7]=(0x00000032u+f->r[3]);
    /* 004db998 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004db99a */
    if(!mi_signed_less(f->flags))goto mi_004db9ca;
    /* 004db99c */
    f->r[0]=MI_U32((0x00000004u+f->r[6]));
    /* 004db99f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004db9a1 */
    if((f->flags&MI_ZF))goto mi_004db9b3;
    /* 004db9a3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db9a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db9a9u;f->boundary(0x004db9a4u,0x00561a2bu,f);
    /* 004db9a9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db9ac */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004db9b3:;
    /* 004db9b3 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004db9b6 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004db9b8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004db9b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004db9beu;f->boundary(0x004db9b9u,0x00561a1du,f);
    /* 004db9be */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004db9c1 */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004db9c4 */
    MI_U32((0x00000138u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004db9ca:;
    /* 004db9ca */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004db9d0 */
    MI_U32((0x00000134u+f->r[6]))=(uint32_t)(f->r[3]);
    /* 004db9d6 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004db9d8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004db9e0 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004db9e8 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004dbb31;
    /* 004db9ee */
    f->r[5]=(0x0000000au+f->r[6]);
mi_004db9f1:;
    /* 004db9f1 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004db9f5 */
    f->r[0]=MI_U32((0x00000134u+f->r[6]));
    /* 004db9fb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004db9fd */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dba03 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004dba07 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004dba0b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004dba0f */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dba13 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004dba15 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004dba19 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004dba1b */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004dba1f */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dba23 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004dba25 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dba29 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dba2d */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004dba31 */
    __asm { fld st(0) }
    /* 004dba33 */
    __asm { fmul st(0),st(1) }
    /* 004dba35 */
    __asm { fld st(2) }
    /* 004dba37 */
    __asm { fmul st(0),st(3) }
    /* 004dba39 */
    __asm { faddp st(1),st(0) }
    /* 004dba3b */
    __asm { fsqrt }
    /* 004dba3d */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dba41 */
    __asm { fstp st(0) }
    /* 004dba43 */
    __asm { fstp st(0) }
    /* 004dba45 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dba49 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004dba4f */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dba51 */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004dba54 */
    if(!(f->flags&MI_ZF))goto mi_004dbb0b;
    /* 004dba5a */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004dba5d */
    if((f->flags&MI_ZF))goto mi_004dbb0b;
    /* 004dba63 */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dba67 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004dba6b */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dba6f */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dba73 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004dba77 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004dba7b */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dba7f */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dba85 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004dba89 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dba8b */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004dba8e */
    if((f->flags&MI_ZF))goto mi_004dbb0b;
    /* 004dba90 */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dba94 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dba9a */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004dba9e */
    p=(0x00000038u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbaa2 */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004dbaa5 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004dbaa7 */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004dbaab:;
    /* 004dbaab */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dbaaf */
    __asm { fmul st(0),st(1) }
    /* 004dbab1 */
    __asm { fadd st(0),st(2) }
    /* 004dbab3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbab8u;f->boundary(0x004dbab3u,0x0056133cu,f);
    /* 004dbab8 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dbabc */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004dbabf */
    __asm { fmul st(0),st(1) }
    /* 004dbac1 */
    MI_U16((0x00000000u+f->r[1]+f->r[7]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dbac5 */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004dbac9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbaceu;f->boundary(0x004dbac9u,0x0056133cu,f);
    /* 004dbace */
    f->r[2]=MI_U32((0x00000004u+f->r[6]));
    /* 004dbad1 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004dbad4 */
    __asm { fstp st(0) }
    /* 004dbad6 */
    MI_U16((0xfffffffcu+f->r[2]+f->r[7]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dbadb */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004dbadf */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004dbae2 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004dbae5 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004dbae9 */
    MI_U8((0xfffffffeu+f->r[1]+f->r[7]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004dbaed */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004dbaef */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004dbaf1 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dbaf3 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004dbaf4 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004dbaf8 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbafc */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004dbb00 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dbb02 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004dbb05 */
    if(!(f->flags&MI_ZF))goto mi_004dbaab;
    /* 004dbb07 */
    __asm { fstp st(0) }
    /* 004dbb09 */
    __asm { fstp st(0) }
mi_004dbb0b:;
    /* 004dbb0b */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004dbb0f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dbb11 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004dbb17 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dbb19 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbb1d */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004dbb21 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004dbb22 */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004dbb25 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dbb27 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbb2b */
    if(mi_signed_less(f->flags))goto mi_004db9f1;
mi_004dbb31:;
    /* 004dbb31 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004dbb35 */
    goto mi_004dbdff;
mi_004dbb3a:;
    /* 004dbb3a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dbb3f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dbb41 */
    if((f->flags&MI_ZF))goto mi_004dbb52;
    /* 004dbb43 */
    MI_U32((0x00775928u))=(uint32_t)(0x0000000fu);
    /* 004dbb4d */
    goto mi_004dbe16;
mi_004dbb52:;
    /* 004dbb52 */
    f->r[6]=MI_U32((0x005d2a30u));
    /* 004dbb58 */
    f->r[1]=MI_U32((0x00e38e34u));
    /* 004dbb5e */
    f->r[2]=MI_U32((0x005d2a18u));
    /* 004dbb64 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004dbb66 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004dbb69 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 004dbb6c */
    a=f->r[6];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004dbb6f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007770f1u+f->r[6])))&0xffu)<<0);
    /* 004dbb75 */
    f->r[6]=(0x00776fb0u+f->r[6]);
    /* 004dbb7b */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004dbb7c */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dbb7e */
    if(mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dbb84 */
    f->r[7]=MI_U32((0x00e38d9cu));
    /* 004dbb8a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dbb8c */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004dbb8e */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dbb90 */
    if(!mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dbb96 */
    f->r[1]=MI_U32((0x00e38e30u));
    /* 004dbb9c */
    f->r[2]=MI_U32((0x005d2a1cu));
    /* 004dbba2 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dbba4 */
    if(mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dbbaa */
    f->r[7]=MI_U32((0x00e38d98u));
    /* 004dbbb0 */
    a=f->r[7];b=MI_U32((0x005d2a20u));t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004dbbb6 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004dbbb8 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dbbba */
    if(!mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dbbc0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dbbc2 */
    if(mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dbbc8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dbbca */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004dbbd0 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dbbd2 */
    if(!mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dbbd8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004dbbdb */
    f->r[7]=0x00000001u;
    /* 004dbbe0 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[2]*2u);
    /* 004dbbe3 */
    MI_U16((0x00000008u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004dbbe7 */
    MI_U16((0x0000000au+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004dbbeb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004dbbed */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004dbbef */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004dbbf5 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dbbf7 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbbfb */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dbc55;
    /* 004dbbfd */
    f->r[2]=(0x00000008u+f->r[6]);
mi_004dbc00:;
    /* 004dbc00 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[2]));
    /* 004dbc03 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[2]));
    /* 004dbc07 */
    f->r[0]=f->r[3];
    /* 004dbc09 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004dbc0b */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004dbc0d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dbc0f */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbc13 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dbc19;
    /* 004dbc15 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
mi_004dbc19:;
    /* 004dbc19 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[2]));
    /* 004dbc1d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000008u+f->r[2]));
    /* 004dbc21 */
    f->r[0]=f->r[3];
    /* 004dbc23 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004dbc25 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004dbc27 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dbc29 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dbc2d;
    /* 004dbc2b */
    f->r[0]=f->r[1];
mi_004dbc2d:;
    /* 004dbc2d */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004dbc31 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dbc33 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dbc40;
    /* 004dbc35 */
    f->r[5]=(0x00000001u+f->r[1]+f->r[5]*1u);
    /* 004dbc39 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000001u);
    /* 004dbc3e */
    goto mi_004dbc49;
mi_004dbc40:;
    /* 004dbc40 */
    f->r[5]=(0x00000001u+f->r[0]+f->r[5]*1u);
    /* 004dbc44 */
    MI_U8((0x00000058u+f->r[4]+f->r[7]*1u))=(uint8_t)(0x00000000u);
mi_004dbc49:;
    /* 004dbc49 */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 004dbc4d */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004dbc4e */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dbc51 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dbc53 */
    if(mi_signed_less(f->flags))goto mi_004dbc00;
mi_004dbc55:;
    /* 004dbc55 */
    restore_menu_inline_capacity_before(0x004dbc55u,f);
    f->r[0]=MI_U32((0x00000138u+f->r[6]));
    /* 004dbc5b */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dbc5d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dbc94;
    /* 004dbc5f */
    f->r[7]=(0x00000032u+f->r[5]);
    /* 004dbc62 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dbc64 */
    if(!mi_signed_less(f->flags))goto mi_004dbc94;
    /* 004dbc66 */
    f->r[0]=MI_U32((0x00000004u+f->r[6]));
    /* 004dbc69 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dbc6b */
    if((f->flags&MI_ZF))goto mi_004dbc7d;
    /* 004dbc6d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbc6e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbc73u;f->boundary(0x004dbc6eu,0x00561a2bu,f);
    /* 004dbc73 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbc76 */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(0x00000000u);
mi_004dbc7d:;
    /* 004dbc7d */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004dbc80 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004dbc82 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbc83 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbc88u;f->boundary(0x004dbc83u,0x00561a1du,f);
    /* 004dbc88 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbc8b */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004dbc8e */
    MI_U32((0x00000138u+f->r[6]))=(uint32_t)(f->r[7]);
mi_004dbc94:;
    /* 004dbc94 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004dbc9a */
    MI_U32((0x00000134u+f->r[6]))=(uint32_t)(f->r[5]);
    /* 004dbca0 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004dbca2 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004dbcaa */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 004dbcb2 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004dbdfb;
    /* 004dbcb8 */
    f->r[5]=(0x0000000au+f->r[6]);
mi_004dbcbb:;
    /* 004dbcbb */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 004dbcbf */
    f->r[0]=MI_U32((0x00000134u+f->r[6]));
    /* 004dbcc5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004dbcc7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dbe0c;
    /* 004dbccd */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffffeu+f->r[5]));
    /* 004dbcd1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000004u+f->r[5]));
    /* 004dbcd5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[5]));
    /* 004dbcd9 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbcdd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004dbcdf */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000006u+f->r[5]));
    /* 004dbce3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004dbce5 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004dbce9 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbced */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004dbcef */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbcf3 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbcf7 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004dbcfb */
    __asm { fld st(0) }
    /* 004dbcfd */
    __asm { fmul st(0),st(1) }
    /* 004dbcff */
    __asm { fld st(2) }
    /* 004dbd01 */
    __asm { fmul st(0),st(3) }
    /* 004dbd03 */
    __asm { faddp st(1),st(0) }
    /* 004dbd05 */
    __asm { fsqrt }
    /* 004dbd07 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dbd0b */
    __asm { fstp st(0) }
    /* 004dbd0d */
    __asm { fstp st(0) }
    /* 004dbd0f */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dbd13 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004dbd19 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dbd1b */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004dbd1e */
    if(!(f->flags&MI_ZF))goto mi_004dbdd5;
    /* 004dbd24 */
    a=MI_U8((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 004dbd27 */
    if((f->flags&MI_ZF))goto mi_004dbdd5;
    /* 004dbd2d */
    p=(0x0000002cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbd31 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004dbd35 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dbd39 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbd3d */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004dbd41 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fdiv dword ptr [eax] }
    /* 004dbd45 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
    /* 004dbd49 */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dbd4f */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcomp dword ptr [eax] }
    /* 004dbd53 */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dbd55 */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004dbd58 */
    if((f->flags&MI_ZF))goto mi_004dbdd5;
    /* 004dbd5a */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbd5e */
    p=(0x0056f480u);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dbd64 */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004dbd68 */
    p=(0x00000038u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbd6c */
    f->r[7]=(0x00000000u+f->r[7]+f->r[7]*2u);
    /* 004dbd6f */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004dbd71 */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fstp dword ptr [eax] }
mi_004dbd75:;
    /* 004dbd75 */
    p=(0x00000034u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dbd79 */
    __asm { fmul st(0),st(1) }
    /* 004dbd7b */
    __asm { fadd st(0),st(2) }
    /* 004dbd7d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbd82u;f->boundary(0x004dbd7du,0x0056133cu,f);
    /* 004dbd82 */
    p=(0x00000030u+f->r[4]);
    __asm { mov eax,p }
    __asm { fld dword ptr [eax] }
    /* 004dbd86 */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004dbd89 */
    __asm { fmul st(0),st(1) }
    /* 004dbd8b */
    MI_U16((0x00000000u+f->r[7]+f->r[1]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dbd8f */
    p=(0x0000003cu+f->r[4]);
    __asm { mov eax,p }
    __asm { fadd dword ptr [eax] }
    /* 004dbd93 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbd98u;f->boundary(0x004dbd93u,0x0056133cu,f);
    /* 004dbd98 */
    f->r[2]=MI_U32((0x00000004u+f->r[6]));
    /* 004dbd9b */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004dbd9e */
    __asm { fstp st(0) }
    /* 004dbda0 */
    MI_U16((0xfffffffcu+f->r[7]+f->r[2]*1u))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dbda5 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004dbda9 */
    f->r[1]=MI_U32((0x00000004u+f->r[6]));
    /* 004dbdac */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 004dbdaf */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004dbdb3 */
    MI_U8((0xfffffffeu+f->r[7]+f->r[1]*1u))=(uint8_t)((f->r[0]&0xffu));
    /* 004dbdb7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004dbdb9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6])))&0xffu)<<0);
    /* 004dbdbb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dbdbd */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004dbdbe */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004dbdc2 */
    p=(0x00000028u+f->r[4]);
    __asm { mov eax,p }
    __asm { fild dword ptr [eax] }
    /* 004dbdc6 */
    p=(0x00000018u+f->r[4]);
    __asm { mov eax,p }
    __asm { fcom dword ptr [eax] }
    /* 004dbdca */
    __asm { fnstsw sw }
    f->r[0]=(f->r[0]&0xffff0000u)|(((sw)&0xffffu)<<0);
    /* 004dbdcc */
    a=((f->r[0]>>8)&0xffu);b=0x00000041u;t=mi_alu(f,a,b,8,3);
    /* 004dbdcf */
    if(!(f->flags&MI_ZF))goto mi_004dbd75;
    /* 004dbdd1 */
    __asm { fstp st(0) }
    /* 004dbdd3 */
    __asm { fstp st(0) }
mi_004dbdd5:;
    /* 004dbdd5 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004dbdd9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dbddb */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000141u+f->r[6])))&0xffu)<<0);
    /* 004dbde1 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dbde3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbde7 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004dbdeb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004dbdec */
    a=f->r[5];b=0x00000006u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004dbdef */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dbdf1 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dbdf5 */
    if(mi_signed_less(f->flags))goto mi_004dbcbb;
mi_004dbdfb:;
    /* 004dbdfb */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
mi_004dbdff:;
    /* 004dbdff */
    f->r[1]=f->r[6];
    /* 004dbe01 */
    MI_U32((0x00000134u+f->r[6]))=(uint32_t)(f->r[2]);
    /* 004dbe07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbe0cu;f->boundary(0x004dbe07u,0x00407f90u,f);
mi_004dbe0c:;
    /* 004dbe0c */
    MI_U32((0x005d2a2cu))=(uint32_t)(0x00000000u);
mi_004dbe16:;
    /* 004dbe16 */
    f->r[0]=MI_U32((0x005d2a30u));
    /* 004dbe1b */
    f->r[2]=MI_U32((0x00000040u+f->r[4]));
    /* 004dbe1f */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004dbe22 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004dbe25 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004dbe28 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770ecu+f->r[0]));
    /* 004dbe2f */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004dbe31 */
    if(!(f->flags&MI_ZF))goto mi_004dd55e;
    /* 004dbe37 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007770eeu+f->r[0]));
    /* 004dbe3e */
    a=MI_U32((0x00000044u+f->r[4]));b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dbe42 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dbe48 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dbe49 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dbe4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dbe4b */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dbe52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dbe53 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbe56 */
    f->r[4]+=4;return;
mi_004dbe57:;
    /* 004dbe57 */
    a=(f->r[0]&0xffu);b=0x00000026u;t=mi_alu(f,a,b,8,1);
    /* 004dbe59 */
    if(!(f->flags&MI_ZF))goto mi_004dbe98;
    /* 004dbe5b */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dbe60 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dbe66 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe68 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe69 */
    a=0x005c7d60u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe6e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe6f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbe75u;f->boundary(0x004dbe70u,0x00425bc0u,f);
    /* 004dbe75 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dbe7b */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dbe80 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe81 */
    a=0x005c7d60u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe86 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe87 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbe88 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbe8du;f->boundary(0x004dbe88u,0x00425990u,f);
    /* 004dbe8d */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbe90 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dbe91 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dbe92 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dbe93 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dbe94 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbe97 */
    f->r[4]+=4;return;
mi_004dbe98:;
    /* 004dbe98 */
    a=(f->r[0]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 004dbe9a */
    if(!(f->flags&MI_ZF))goto mi_004dbf31;
    /* 004dbea0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004dbea5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dbea7 */
    if((f->flags&MI_ZF))goto mi_004dbec4;
    /* 004dbea9 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00605980u)))&0xffu)<<0);
    /* 004dbeaf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dbeb0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dbeb1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dbeb2 */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[1]&0xffu));
    /* 004dbeb8 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dbebf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dbec0 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbec3 */
    f->r[4]+=4;return;
mi_004dbec4:;
    /* 004dbec4 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dbeca */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dbecf */
    f->r[1]=MI_U32((0x00605970u));
    /* 004dbed5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbed6 */
    f->r[2]=MI_U32((0x00605974u));
    /* 004dbedc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbedd */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbedf */
    a=0x005c4208u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbee4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbee5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbee6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbeebu;f->boundary(0x004dbee6u,0x00426450u,f);
    /* 004dbeeb */
    f->r[1]=MI_U32((0x00605974u));
    /* 004dbef1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbef4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbef5 */
    f->r[0]=MI_U32((0x00605970u));
    /* 004dbefa */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbefc */
    a=0x005c4208u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf01 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf02 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbf08u;f->boundary(0x004dbf03u,0x004d6740u,f);
    /* 004dbf08 */
    f->r[2]=MI_U32((0x00605970u));
    /* 004dbf0e */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dbf13 */
    f->r[0]=MI_U32((0x00605974u));
    /* 004dbf18 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf1a */
    a=0x005c4208u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf1f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf20 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf21 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbf26u;f->boundary(0x004dbf21u,0x004261d0u,f);
    /* 004dbf26 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbf29 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dbf2a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dbf2b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dbf2c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dbf2d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbf30 */
    f->r[4]+=4;return;
mi_004dbf31:;
    /* 004dbf31 */
    a=(f->r[0]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 004dbf33 */
    if((f->flags&MI_ZF))goto mi_004dd826;
    /* 004dbf39 */
    a=(f->r[0]&0xffu);b=0x00000048u;t=mi_alu(f,a,b,8,1);
    /* 004dbf3b */
    if((f->flags&MI_ZF))goto mi_004dd826;
    /* 004dbf41 */
    f->r[3]=0x00000007u;
    /* 004dbf46 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004dbf48 */
    if(!(f->flags&MI_ZF))goto mi_004dc566;
    /* 004dbf4e */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dbf54 */
    f->r[6]=MI_U32((0x00e38da8u));
    /* 004dbf5a */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf5c */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf5e */
    f->r[5]=(0x000001b0u+f->r[1]);
    /* 004dbf64 */
    a=0x005f8320u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf69 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf6a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf6b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbf70u;f->boundary(0x004dbf6bu,0x00425bc0u,f);
    /* 004dbf70 */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf72 */
    a=0x005f8320u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf77 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf78 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbf79 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbf7eu;f->boundary(0x004dbf79u,0x00425990u,f);
    /* 004dbf7e */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbf81 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dbf83 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dbf89 */
    f->r[2]=MI_U32((0x00f53f00u));
    /* 004dbf8f */
    f->r[5]=MI_U32((0x00f53f08u));
    /* 004dbf95 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dbf9a */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dbf9d */
    a=f->r[2];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004dbfa0 */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfa2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfa4 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dbfa6 */
    a=0x005c7b80u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfac */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfad */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbfb2u;f->boundary(0x004dbfadu,0x00425bc0u,f);
    /* 004dbfb2 */
    f->r[1]=MI_U32((0x00f53f00u));
    /* 004dbfb8 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dbfbd */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dbfc3 */
    a=f->r[1];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dbfc6 */
    a=f->r[1];b=0x00000004u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004dbfc9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfcb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dbfcd */
    a=0x005c7b80u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfd2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfd3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dbfd4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dbfd9u;f->boundary(0x004dbfd4u,0x00425990u,f);
    /* 004dbfd9 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dbfdc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dbfde */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dbfe4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dbfe9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dbfeb */
    if((f->flags&MI_ZF))goto mi_004dc015;
    /* 004dbfed */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004dbff2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dbff3 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004dbff9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dbffa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dbffb */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004dc002 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dc009 */
    MI_U8((0x00ea623cu+f->r[0]))=(uint8_t)(0x00000000u);
    /* 004dc010 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc011 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc014 */
    f->r[4]+=4;return;
mi_004dc015:;
    /* 004dc015 */
    f->r[0]=MI_U32((0x00605a44u));
    /* 004dc01a */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004dc01d */
    if(!(f->flags&MI_ZF))goto mi_004dc0b6;
    /* 004dc023 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc029 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc02f */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc034 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc035 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc03b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc03c */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc03e */
    a=0x005c4ab8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc043 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc044 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc045 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc04au;f->boundary(0x004dc045u,0x00426450u,f);
    /* 004dc04a */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc050 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc053 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc054 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc059 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc05b */
    a=0x005c4ab8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc060 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc061 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc062 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc067u;f->boundary(0x004dc062u,0x004d6740u,f);
    /* 004dc067 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc06d */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc073 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc075 */
    a=0x005c4ab8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc07a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc07b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc07c */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc081 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc086u;f->boundary(0x004dc081u,0x004261d0u,f);
    /* 004dc086 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc089 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc08b */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc091 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc096 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc09c */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc09e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc09f */
    a=0x005c8d08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0a4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0a5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc0abu;f->boundary(0x004dc0a6u,0x00425e80u,f);
    /* 004dc0ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0ac */
    a=0x005c8d08u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0b1 */
    goto mi_004dc479;
mi_004dc0b6:;
    /* 004dc0b6 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004dc0b8 */
    if(!(f->flags&MI_ZF))goto mi_004dc10f;
    /* 004dc0ba */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc0c0 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc0c6 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc0cb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0cc */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc0d2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0d3 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0d5 */
    a=0x005c4cd0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0da */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0db */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc0e1u;f->boundary(0x004dc0dcu,0x00426450u,f);
    /* 004dc0e1 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc0e7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc0ea */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0eb */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc0f0 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0f2 */
    a=0x005c4cd0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0f7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0f8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc0f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc0feu;f->boundary(0x004dc0f9u,0x004d6740u,f);
    /* 004dc0fe */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc100 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc105 */
    a=0x005c4cd0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc10a */
    goto mi_004dcc8c;
mi_004dc10f:;
    /* 004dc10f */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004dc112 */
    if(!(f->flags&MI_ZF))goto mi_004dc181;
    /* 004dc114 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dc119 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dc11f */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc125 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc126 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc12b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc12c */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc12e */
    a=0x005c4f00u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc133 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc134 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc135 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc13au;f->boundary(0x004dc135u,0x00426450u,f);
    /* 004dc13a */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc140 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc146 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc149 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc14a */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc14c */
    a=0x005c4f00u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc151 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc152 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc153 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc158u;f->boundary(0x004dc153u,0x004d6740u,f);
    /* 004dc158 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc15e */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc163 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc168 */
    a=0x00000014u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc16a */
    a=0x005c4f00u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc16f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc170 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc171 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc176u;f->boundary(0x004dc171u,0x004261d0u,f);
    /* 004dc176 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc179 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc17a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc17b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc17c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc17d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc180 */
    f->r[4]+=4;return;
mi_004dc181:;
    /* 004dc181 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004dc184 */
    if(!(f->flags&MI_ZF))goto mi_004dc1f3;
    /* 004dc186 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dc18c */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dc191 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc197 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc198 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc19e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc19f */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1a1 */
    a=0x005c5368u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1a6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1a7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1a8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc1adu;f->boundary(0x004dc1a8u,0x00426450u,f);
    /* 004dc1ad */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc1b3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc1b6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1b7 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc1bc */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1be */
    a=0x005c5368u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1c3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1c4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc1cau;f->boundary(0x004dc1c5u,0x004d6740u,f);
    /* 004dc1ca */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc1d0 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc1d5 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc1da */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1dc */
    a=0x005c5368u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1e2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc1e3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc1e8u;f->boundary(0x004dc1e3u,0x004261d0u,f);
    /* 004dc1e8 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc1eb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc1ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc1ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc1ee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc1ef */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc1f2 */
    f->r[4]+=4;return;
mi_004dc1f3:;
    /* 004dc1f3 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004dc1f6 */
    if(!(f->flags&MI_ZF))goto mi_004dc24d;
    /* 004dc1f8 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc1fe */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc204 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc209 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc20a */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc210 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc211 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc213 */
    a=0x005f7650u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc218 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc219 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc21a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc21fu;f->boundary(0x004dc21au,0x00426450u,f);
    /* 004dc21f */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc225 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc228 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc229 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc22e */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc230 */
    a=0x005f7650u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc235 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc236 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc237 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc23cu;f->boundary(0x004dc237u,0x004d6740u,f);
    /* 004dc23c */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc23e */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc243 */
    a=0x005f7650u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc248 */
    goto mi_004dcc8c;
mi_004dc24d:;
    /* 004dc24d */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 004dc250 */
    if(!(f->flags&MI_ZF))goto mi_004dc305;
    /* 004dc256 */
    f->r[6]=MI_U32((0x00f53e6cu));
    /* 004dc25c */
    f->r[0]=0x0000000fu;
    /* 004dc261 */
    a=f->r[6];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004dc264 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004dc268;
    /* 004dc266 */
    f->r[0]=f->r[6];
mi_004dc268:;
    /* 004dc268 */
    a=f->r[6];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004dc26b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dc272;
    /* 004dc26d */
    f->r[6]=0x0000000fu;
mi_004dc272:;
    /* 004dc272 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc278 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc27e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc27f */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc285 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dc288 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc289 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc28a */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc28f */
    a=0x006032e8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc294 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc295 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc296 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc29bu;f->boundary(0x004dc296u,0x00426450u,f);
    /* 004dc29b */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc2a1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc2a4 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004dc2a7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2a8 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc2ad */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2ae */
    a=0x006032e8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2b3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2b4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc2bau;f->boundary(0x004dc2b5u,0x004d6740u,f);
    /* 004dc2ba */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc2bf */
    f->r[0]=MI_U32((0x00f53e6cu));
    /* 004dc2c4 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc2c7 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 004dc2ca */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dc2d1;
    /* 004dc2cc */
    f->r[0]=0x0000000fu;
mi_004dc2d1:;
    /* 004dc2d1 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc2d7 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc2dd */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dc2e0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2e1 */
    a=0x006032e8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2e6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2e7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc2e8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc2edu;f->boundary(0x004dc2e8u,0x004261d0u,f);
    /* 004dc2ed */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc2f0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc2f2 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc2f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc2fdu;f->boundary(0x004dc2f8u,0x0055bb90u,f);
    /* 004dc2fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc2fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc2ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc300 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc301 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc304 */
    f->r[4]+=4;return;
mi_004dc305:;
    /* 004dc305 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004dc307 */
    if(!(f->flags&MI_ZF))goto mi_004dc373;
    /* 004dc309 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dc30e */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dc314 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc31a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc31b */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc320 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc321 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc322 */
    a=0x005c5130u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc327 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc328 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc329 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc32eu;f->boundary(0x004dc329u,0x00426450u,f);
    /* 004dc32e */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc334 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc33a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc33d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc33e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc33f */
    a=0x005c5130u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc344 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc345 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc346 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc34bu;f->boundary(0x004dc346u,0x004d6740u,f);
    /* 004dc34b */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc351 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc356 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc35b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc35c */
    a=0x005c5130u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc361 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc362 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc363 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc368u;f->boundary(0x004dc363u,0x004261d0u,f);
    /* 004dc368 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc36b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc36c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc36d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc36e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc36f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc372 */
    f->r[4]+=4;return;
mi_004dc373:;
    /* 004dc373 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004dc376 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc37c */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004dc37f */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc385 */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004dc388 */
    if(!(f->flags&MI_ZF))goto mi_004dc44b;
    /* 004dc38e */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dc394 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dc399 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc39f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3a0 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc3a6 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3a7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3a9 */
    a=0x005c5168u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3ae */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3af */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3b0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc3b5u;f->boundary(0x004dc3b0u,0x00426450u,f);
    /* 004dc3b5 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc3bb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc3be */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3bf */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc3c4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3c6 */
    a=0x005c5168u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3cc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc3d2u;f->boundary(0x004dc3cdu,0x004d6740u,f);
    /* 004dc3d2 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc3d8 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc3dd */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc3e2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3e4 */
    a=0x005c5168u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3e9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3ea */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc3eb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc3f0u;f->boundary(0x004dc3ebu,0x004261d0u,f);
    /* 004dc3f0 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc3f3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc3f5 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc3fb */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc401 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc407 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc40c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc40d */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc413 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc414 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc416 */
    a=0x005c59e8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc41b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc41c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc41d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc422u;f->boundary(0x004dc41du,0x00426450u,f);
    /* 004dc422 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc428 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc42b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc42c */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc431 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc433 */
    a=0x005c59e8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc438 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc439 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc43a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc43fu;f->boundary(0x004dc43au,0x004d6740u,f);
    /* 004dc43f */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc441 */
    a=0x005c59e8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc446 */
    goto mi_004dcc8c;
mi_004dc44b:;
    /* 004dc44b */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 004dc44e */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc454 */
    a=f->r[0];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 004dc457 */
    if(!(f->flags&MI_ZF))goto mi_004dc496;
    /* 004dc459 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc45e */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc464 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc466 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc467 */
    a=0x005c8d38u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc46c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc46d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc46e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc473u;f->boundary(0x004dc46eu,0x00425e80u,f);
    /* 004dc473 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc474 */
    a=0x005c8d38u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004dc479:;
    /* 004dc479 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc47f */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc484 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc485 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc486 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc48bu;f->boundary(0x004dc486u,0x00425c40u,f);
    /* 004dc48b */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc48e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc48f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc490 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc491 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc492 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc495 */
    f->r[4]+=4;return;
mi_004dc496:;
    /* 004dc496 */
    a=f->r[0];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 004dc499 */
    if(!(f->flags&MI_ZF))goto mi_004dc4f0;
    /* 004dc49b */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc4a1 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc4a7 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc4ac */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4ad */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc4b3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4b4 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4b6 */
    a=0x005c5798u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4bb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4bc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc4c2u;f->boundary(0x004dc4bdu,0x00426450u,f);
    /* 004dc4c2 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc4c8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc4cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4cc */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc4d1 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4d3 */
    a=0x005c5798u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4d8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4d9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4da */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc4dfu;f->boundary(0x004dc4dau,0x004d6740u,f);
    /* 004dc4df */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4e1 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc4e6 */
    a=0x005c5798u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc4eb */
    goto mi_004dcc8c;
mi_004dc4f0:;
    /* 004dc4f0 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004dc4f3 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc4f9 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dc4fe */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dc504 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc50a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc50b */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc510 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc511 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc513 */
    a=0x005c5878u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc518 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc519 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc51a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc51fu;f->boundary(0x004dc51au,0x00426450u,f);
    /* 004dc51f */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc525 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc52b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc52e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc52f */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc531 */
    a=0x005c5878u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc536 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc537 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc538 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc53du;f->boundary(0x004dc538u,0x004d6740u,f);
    /* 004dc53d */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc543 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc548 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc54d */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc54f */
    a=0x005c5878u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc554 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc555 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc556 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc55bu;f->boundary(0x004dc556u,0x004261d0u,f);
    /* 004dc55b */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc55e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc55f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc560 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc561 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc562 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc565 */
    f->r[4]+=4;return;
mi_004dc566:;
    /* 004dc566 */
    a=(f->r[0]&0xffu);b=0x00000028u;t=mi_alu(f,a,b,8,1);
    /* 004dc568 */
    if(!(f->flags&MI_ZF))goto mi_004dc84c;
    /* 004dc56e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dc573 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[5]);
    /* 004dc579 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dc57b */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[5]);
    /* 004dc581 */
    MI_U32((0x007056c0u))=(uint32_t)(f->r[5]);
    /* 004dc587 */
    if((f->flags&MI_ZF))goto mi_004dc59e;
    /* 004dc589 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc58a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc58b */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dc591 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc592 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dc599 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc59a */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc59d */
    f->r[4]+=4;return;
mi_004dc59e:;
    /* 004dc59e */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc5a4 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc5a9 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5ab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5ac */
    a=0x006035e0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5b1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5b2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc5b8u;f->boundary(0x004dc5b3u,0x00425bc0u,f);
    /* 004dc5b8 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc5be */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc5c4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5c5 */
    a=0x006035e0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5ca */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5cb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc5cc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc5d1u;f->boundary(0x004dc5ccu,0x00425990u,f);
    /* 004dc5d1 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc5d4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc5d6 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc5dc */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004dc5e1 */
    f->r[2]=MI_U32((0x0060598cu));
    /* 004dc5e7 */
    a=f->r[0];b=0x000024ecu;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004dc5ed */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dc5ef */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00ea4096u+f->r[0]*2u)))&0xffffu)<<0);
    /* 004dc5f7 */
    a=(f->r[0]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,1);
    /* 004dc5fa */
    if(!(f->flags&MI_ZF))goto mi_004dc63c;
    /* 004dc5fc */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc602 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc608 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc60a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc60b */
    a=0x00603580u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc610 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc611 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc612 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc617u;f->boundary(0x004dc612u,0x00425e80u,f);
    /* 004dc617 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc61c */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc622 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc623 */
    a=0x00603580u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc628 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc629 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc62a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc62fu;f->boundary(0x004dc62au,0x00425c40u,f);
    /* 004dc62f */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc632 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc634 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc63a */
    goto mi_004dc680;
mi_004dc63c:;
    /* 004dc63c */
    a=(f->r[0]&0xffffu);b=0x00000001u;t=mi_alu(f,a,b,16,1);
    /* 004dc640 */
    if(!(f->flags&MI_ZF))goto mi_004dc680;
    /* 004dc642 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc648 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc64d */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc64f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc650 */
    a=0x006035b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc655 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc656 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc657 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc65cu;f->boundary(0x004dc657u,0x00425e80u,f);
    /* 004dc65c */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc662 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc668 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc669 */
    a=0x006035b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc66e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc66f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc670 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc675u;f->boundary(0x004dc670u,0x00425c40u,f);
    /* 004dc675 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc678 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc67a */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dc680:;
    /* 004dc680 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dc685 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dc68b */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc691 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc692 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc697 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc698 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc69a */
    a=0x006034e0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc69f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6a0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc6a6u;f->boundary(0x004dc6a1u,0x00426450u,f);
    /* 004dc6a6 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc6ac */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc6b2 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc6b5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6b6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6b8 */
    a=0x006034e0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6bd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6be */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6bf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc6c4u;f->boundary(0x004dc6bfu,0x004d6740u,f);
    /* 004dc6c4 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc6ca */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc6cf */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc6d4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6d6 */
    a=0x006034e0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6db */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6dc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6dd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc6e2u;f->boundary(0x004dc6ddu,0x004261d0u,f);
    /* 004dc6e2 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc6e5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc6e7 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc6ed */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dc6f3 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dc6f8 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc6fe */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc6ff */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc705 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc706 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc708 */
    a=0x00603520u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc70d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc70e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc70f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc714u;f->boundary(0x004dc70fu,0x00426450u,f);
    /* 004dc714 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc71a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc71d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc71e */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc723 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc725 */
    a=0x00603520u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc72a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc72b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc72c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc731u;f->boundary(0x004dc72cu,0x004d6740u,f);
    /* 004dc731 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc737 */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004dc73c */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc741 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc743 */
    a=0x00603520u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc748 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc749 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc74a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc74fu;f->boundary(0x004dc74au,0x004261d0u,f);
    /* 004dc74f */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc752 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc754 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc75a */
    a=MI_U32((0x007056c4u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dc760 */
    if(!(f->flags&MI_ZF))goto mi_004dc7df;
    /* 004dc762 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc768 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc76e */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc773 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc774 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc77a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc77b */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc77d */
    a=0x00603540u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc782 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc783 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc784 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc789u;f->boundary(0x004dc784u,0x00426450u,f);
    /* 004dc789 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc78f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc792 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc793 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc798 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc79a */
    a=0x00603540u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc79f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7a0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7a1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc7a6u;f->boundary(0x004dc7a1u,0x004d6740u,f);
    /* 004dc7a6 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc7ac */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc7b2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7b4 */
    a=0x00603540u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7b9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7ba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7bb */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004dc7c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc7c5u;f->boundary(0x004dc7c0u,0x004261d0u,f);
    /* 004dc7c5 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc7c8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc7ca */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc7d0 */
    f->r[0]=MI_U32((0x007056c4u));
    /* 004dc7d5 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dc7d7 */
    if((f->flags&MI_ZF))goto mi_004dc7df;
    /* 004dc7d9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004dc7da */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
mi_004dc7df:;
    /* 004dc7df */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dc7e4 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dc7ea */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc7f0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7f1 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc7f6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7f7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7f9 */
    a=0x00603560u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7fe */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc7ff */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc800 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc805u;f->boundary(0x004dc800u,0x00426450u,f);
    /* 004dc805 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc80b */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc811 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc814 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc815 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc817 */
    a=0x00603560u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc81c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc81d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc81e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc823u;f->boundary(0x004dc81eu,0x004d6740u,f);
    /* 004dc823 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc829 */
    MI_U32((0x007056c0u))=(uint32_t)(f->r[0]);
    /* 004dc82e */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc833 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc835 */
    a=0x00603560u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc83a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc83b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc83c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc841u;f->boundary(0x004dc83cu,0x004261d0u,f);
    /* 004dc841 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc844 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc845 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc846 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc847 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc848 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc84b */
    f->r[4]+=4;return;
mi_004dc84c:;
    /* 004dc84c */
    a=(f->r[0]&0xffu);b=0x0000002au;t=mi_alu(f,a,b,8,1);
    /* 004dc84e */
    if(!(f->flags&MI_ZF))goto mi_004dc8d8;
    /* 004dc854 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dc859 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dc85b */
    if((f->flags&MI_ZF))goto mi_004dc872;
    /* 004dc85d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc85e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc85f */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dc865 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc866 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dc86d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc86e */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc871 */
    f->r[4]+=4;return;
mi_004dc872:;
    /* 004dc872 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dc878 */
    f->r[1]=MI_U32((0x00605984u));
    /* 004dc87e */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dc883 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc884 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc88a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004dc88b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc88c */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc891 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc892 */
    a=0x005c4bb8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc897 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc898 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc899 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc89eu;f->boundary(0x004dc899u,0x00426450u,f);
    /* 004dc89e */
    f->r[1]=MI_U32((0x00605984u));
    /* 004dc8a4 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dc8aa */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc8ad */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004dc8ae */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc8af */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dc8b4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc8b5 */
    a=0x005c4bb8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc8ba */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc8bb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc8bc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc8c1u;f->boundary(0x004dc8bcu,0x004d6740u,f);
    /* 004dc8c1 */
    f->r[1]=MI_U32((0x00605984u));
    /* 004dc8c7 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc8cc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004dc8cd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc8ce */
    a=0x005c4bb8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc8d3 */
    goto mi_004dcb43;
mi_004dc8d8:;
    /* 004dc8d8 */
    a=(f->r[0]&0xffu);b=0x0000002bu;t=mi_alu(f,a,b,8,1);
    /* 004dc8da */
    if(!(f->flags&MI_ZF))goto mi_004dc993;
    /* 004dc8e0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dc8e5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dc8e7 */
    if((f->flags&MI_ZF))goto mi_004dc8fe;
    /* 004dc8e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc8ea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc8eb */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dc8f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc8f2 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dc8f9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc8fa */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc8fd */
    f->r[4]+=4;return;
mi_004dc8fe:;
    /* 004dc8fe */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc904 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc90a */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc90c */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc90e */
    a=0x005c7bc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc913 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc914 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc915 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc91au;f->boundary(0x004dc915u,0x00425bc0u,f);
    /* 004dc91a */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc91f */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc925 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc927 */
    a=0x005c7bc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc92c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc92d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc92e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc933u;f->boundary(0x004dc92eu,0x00425990u,f);
    /* 004dc933 */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc936 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc938 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc93e */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dc944 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dc949 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dc94f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc950 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dc956 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc957 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc959 */
    a=0x005c51a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc95e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc95f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc960 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc965u;f->boundary(0x004dc960u,0x00426450u,f);
    /* 004dc965 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dc96b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc96e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc96f */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dc974 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc976 */
    a=0x005c51a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc97b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc97c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc97d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc982u;f->boundary(0x004dc97du,0x004d6740u,f);
    /* 004dc982 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc984 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dc989 */
    a=0x005c51a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc98e */
    goto mi_004dcb43;
mi_004dc993:;
    /* 004dc993 */
    a=(f->r[0]&0xffu);b=0x00000052u;t=mi_alu(f,a,b,8,1);
    /* 004dc995 */
    if(!(f->flags&MI_ZF))goto mi_004dca39;
    /* 004dc99b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dc9a0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dc9a2 */
    if((f->flags&MI_ZF))goto mi_004dc9ba;
    /* 004dc9a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dc9a5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dc9a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dc9a7 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000009u);
    /* 004dc9ae */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dc9b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dc9b6 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc9b9 */
    f->r[4]+=4;return;
mi_004dc9ba:;
    /* 004dc9ba */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9bc */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9be */
    a=0x005c7c58u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9c3 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9c5 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc9ccu;f->boundary(0x004dc9c7u,0x00425bc0u,f);
    /* 004dc9cc */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9ce */
    a=0x005c7c58u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9d3 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9d5 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dc9dcu;f->boundary(0x004dc9d7u,0x00425990u,f);
    /* 004dc9dc */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dc9df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dc9e1 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dc9e7 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dc9ed */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dc9f3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9f4 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9f5 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9f7 */
    a=0x005c5288u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9fc */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dc9fe */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca00 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dca05u;f->boundary(0x004dca00u,0x00426450u,f);
    /* 004dca05 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dca08 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca09 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca0b */
    a=0x005c5288u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca10 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca12 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dca19u;f->boundary(0x004dca14u,0x004d6740u,f);
    /* 004dca19 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca1b */
    a=0x005c5288u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca20 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca22 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca24 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dca29 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dca2eu;f->boundary(0x004dca29u,0x004261d0u,f);
    /* 004dca2e */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dca31 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dca32 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dca33 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dca34 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dca35 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dca38 */
    f->r[4]+=4;return;
mi_004dca39:;
    /* 004dca39 */
    a=(f->r[0]&0xffu);b=0x0000002cu;t=mi_alu(f,a,b,8,1);
    /* 004dca3b */
    if(!(f->flags&MI_ZF))goto mi_004dcacc;
    /* 004dca41 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dca46 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dca48 */
    if((f->flags&MI_ZF))goto mi_004dca5f;
    /* 004dca4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dca4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dca4c */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dca52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dca53 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dca5a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dca5b */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dca5e */
    f->r[4]+=4;return;
mi_004dca5f:;
    /* 004dca5f */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dca64 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dca6a */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dca70 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca71 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dca76 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca77 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca79 */
    a=0x005c5468u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca7e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca7f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca80 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dca85u;f->boundary(0x004dca80u,0x00426450u,f);
    /* 004dca85 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dca8b */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dca91 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dca94 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca95 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca97 */
    a=0x005c5468u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca9c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca9d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dca9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcaa3u;f->boundary(0x004dca9eu,0x004d6740u,f);
    /* 004dcaa3 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dcaa9 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dcaae */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dcab3 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcab5 */
    a=0x005c5468u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcaba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcabb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcabc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcac1u;f->boundary(0x004dcabcu,0x004261d0u,f);
    /* 004dcac1 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcac4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcac5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcac6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcac7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcac8 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcacb */
    f->r[4]+=4;return;
mi_004dcacc:;
    /* 004dcacc */
    a=(f->r[0]&0xffu);b=0x0000004du;t=mi_alu(f,a,b,8,1);
    /* 004dcace */
    if(!(f->flags&MI_ZF))goto mi_004dcb60;
    /* 004dcad4 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dcad9 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dcadb */
    if((f->flags&MI_ZF))goto mi_004dcaf3;
    /* 004dcadd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcade */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcadf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcae0 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000009u);
    /* 004dcae7 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dcaee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcaef */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcaf2 */
    f->r[4]+=4;return;
mi_004dcaf3:;
    /* 004dcaf3 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dcaf9 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dcafe */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dcb04 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb05 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dcb0b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb0c */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb0e */
    a=0x005c5468u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb13 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb14 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb15 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcb1au;f->boundary(0x004dcb15u,0x00426450u,f);
    /* 004dcb1a */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dcb20 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcb23 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb24 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dcb29 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb2b */
    a=0x005c5468u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb30 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb31 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb32 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcb37u;f->boundary(0x004dcb32u,0x004d6740u,f);
    /* 004dcb37 */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb39 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dcb3e */
    a=0x005c5468u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004dcb43:;
    /* 004dcb43 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dcb49 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dcb4e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb4f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb50 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcb55u;f->boundary(0x004dcb50u,0x004261d0u,f);
    /* 004dcb55 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcb58 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcb59 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcb5a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcb5b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcb5c */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcb5f */
    f->r[4]+=4;return;
mi_004dcb60:;
    /* 004dcb60 */
    a=(f->r[0]&0xffu);b=0x0000002du;t=mi_alu(f,a,b,8,1);
    /* 004dcb62 */
    if(!(f->flags&MI_ZF))goto mi_004dcc16;
    /* 004dcb68 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dcb6d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dcb6f */
    if((f->flags&MI_ZF))goto mi_004dcb86;
    /* 004dcb71 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcb72 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcb73 */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dcb79 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcb7a */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dcb81 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcb82 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcb85 */
    f->r[4]+=4;return;
mi_004dcb86:;
    /* 004dcb86 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dcb8c */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dcb92 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dcb97 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb98 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dcb9e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcb9f */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcba0 */
    a=0x005c55b8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcba5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcba6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcba7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcbacu;f->boundary(0x004dcba7u,0x00426450u,f);
    /* 004dcbac */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dcbb2 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcbb5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbb6 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dcbbb */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbbc */
    a=0x005c55b8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbc1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbc2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbc3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcbc8u;f->boundary(0x004dcbc3u,0x004d6740u,f);
    /* 004dcbc8 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dcbce */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dcbd4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbd5 */
    a=0x005c55b8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbda */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbdb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbdc */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dcbe1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcbe6u;f->boundary(0x004dcbe1u,0x004261d0u,f);
    /* 004dcbe6 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcbe9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dcbeb */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dcbf1 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dcbf6 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dcbfc */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbfe */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcbff */
    a=0x005c8d68u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc04 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc05 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc06 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcc0bu;f->boundary(0x004dcc06u,0x00425e80u,f);
    /* 004dcc0b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc0c */
    a=0x005c8d68u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc11 */
    goto mi_004dc479;
mi_004dcc16:;
    /* 004dcc16 */
    a=(f->r[0]&0xffu);b=0x0000002eu;t=mi_alu(f,a,b,8,1);
    /* 004dcc18 */
    if(!(f->flags&MI_ZF))goto mi_004dccaa;
    /* 004dcc1e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dcc23 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dcc25 */
    if((f->flags&MI_ZF))goto mi_004dcc3c;
    /* 004dcc27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcc28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcc29 */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dcc2f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcc30 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dcc37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcc38 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcc3b */
    f->r[4]+=4;return;
mi_004dcc3c:;
    /* 004dcc3c */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dcc42 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dcc48 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dcc4d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc4e */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dcc54 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc55 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc57 */
    a=0x005c5680u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc5c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc5d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc5e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcc63u;f->boundary(0x004dcc5eu,0x00426450u,f);
    /* 004dcc63 */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dcc69 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcc6c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc6d */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dcc72 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc74 */
    a=0x005c5680u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc79 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc7a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcc80u;f->boundary(0x004dcc7bu,0x004d6740u,f);
    /* 004dcc80 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc82 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dcc87 */
    a=0x005c5680u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004dcc8c:;
    /* 004dcc8c */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dcc92 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dcc98 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc99 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcc9a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcc9fu;f->boundary(0x004dcc9au,0x004261d0u,f);
    /* 004dcc9f */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcca2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcca3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcca4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcca5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcca6 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcca9 */
    f->r[4]+=4;return;
mi_004dccaa:;
    /* 004dccaa */
    a=(f->r[0]&0xffu);b=0x0000004cu;t=mi_alu(f,a,b,8,1);
    /* 004dccac */
    if(!(f->flags&MI_ZF))goto mi_004dcdb5;
    /* 004dccb2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dccb7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dccb9 */
    if((f->flags&MI_ZF))goto mi_004dccd0;
    /* 004dccbb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dccbc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dccbd */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dccc3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dccc4 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dcccb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcccc */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcccf */
    f->r[4]+=4;return;
mi_004dccd0:;
    /* 004dccd0 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dccd5 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dccdb */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dcce1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcce2 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dcce7 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcce8 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dccea */
    a=0x005c5710u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dccef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dccf0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dccf1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dccf6u;f->boundary(0x004dccf1u,0x00426450u,f);
    /* 004dccf6 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dccfc */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dcd02 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcd05 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd06 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd08 */
    a=0x005c5710u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd0d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd0e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd0f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcd14u;f->boundary(0x004dcd0fu,0x004d6740u,f);
    /* 004dcd14 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dcd1a */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dcd1f */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dcd24 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd26 */
    a=0x005c5710u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd2b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd2c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd2d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcd32u;f->boundary(0x004dcd2du,0x004261d0u,f);
    /* 004dcd32 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcd35 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dcd37 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dcd3d */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dcd43 */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dcd48 */
    a=0x0000000au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd4a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd4b */
    a=f->r[2];b=0x000000e9u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dcd51 */
    a=0x005c5768u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd56 */
    a=f->r[0];b=0x00000130u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dcd5b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd5c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd5d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcd62u;f->boundary(0x004dcd5du,0x00425e80u,f);
    /* 004dcd62 */
    f->r[1]=MI_U32((0x00f53f08u));
    /* 004dcd68 */
    f->r[2]=MI_U32((0x00f53f0cu));
    /* 004dcd6e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd6f */
    a=f->r[1];b=0x000000e9u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dcd75 */
    a=0x005c5768u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd7a */
    a=f->r[2];b=0x00000130u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dcd80 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd81 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd82 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcd87u;f->boundary(0x004dcd82u,0x00425c40u,f);
    /* 004dcd87 */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dcd8c */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dcd92 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd93 */
    a=f->r[0];b=0x000000e9u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dcd98 */
    a=0x005c5768u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcd9d */
    a=f->r[1];b=0x00000130u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dcda3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcda4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcda5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcdaau;f->boundary(0x004dcda5u,0x004256a0u,f);
    /* 004dcdaa */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcdad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcdae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcdaf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcdb0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcdb1 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcdb4 */
    f->r[4]+=4;return;
mi_004dcdb5:;
    /* 004dcdb5 */
    a=(f->r[0]&0xffu);b=0x00000033u;t=mi_alu(f,a,b,8,1);
    /* 004dcdb7 */
    if(!(f->flags&MI_ZF))goto mi_004dce78;
    /* 004dcdbd */
    a=MI_U8((0x00e38dd9u));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004dcdc4 */
    if(!(f->flags&MI_ZF))goto mi_004dcdf0;
    /* 004dcdc6 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dcdcb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dcdcd */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dcdd3 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcdd4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcdd9u;f->boundary(0x004dcdd4u,0x0051f780u,f);
    /* 004dcdd9 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcddb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcde0u;f->boundary(0x004dcddbu,0x0051f780u,f);
    /* 004dcde0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcde3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcde8u;f->boundary(0x004dcde3u,0x00418640u,f);
    /* 004dcde8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcde9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcdea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcdeb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcdec */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcdef */
    f->r[4]+=4;return;
mi_004dcdf0:;
    /* 004dcdf0 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dcdf6 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dcdfb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcdfc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcdfd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce02u;f->boundary(0x004dcdfdu,0x004ea880u,f);
    /* 004dce02 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce08u;f->boundary(0x004dce03u,0x004ea8d0u,f);
    /* 004dce08 */
    f->r[1]=MI_U32((0x00f53db4u));
    /* 004dce0e */
    f->r[2]=MI_U32((0x00f53db8u));
    /* 004dce14 */
    a=f->r[1];b=0xffffff90u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dce17 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce18 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce19 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce1a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce1fu;f->boundary(0x004dce1au,0x00426450u,f);
    /* 004dce1f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dce22 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce23 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce28u;f->boundary(0x004dce23u,0x004ea880u,f);
    /* 004dce28 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce29 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce2eu;f->boundary(0x004dce29u,0x004ea8d0u,f);
    /* 004dce2e */
    f->r[1]=MI_U32((0x00f53db8u));
    /* 004dce34 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce35 */
    f->r[0]=MI_U32((0x00f53db4u));
    /* 004dce3a */
    a=f->r[0];b=0xffffff90u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dce3d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce3e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce3f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce44u;f->boundary(0x004dce3fu,0x004d6740u,f);
    /* 004dce44 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dce47 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dce4c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce51u;f->boundary(0x004dce4cu,0x004ea880u,f);
    /* 004dce51 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce57u;f->boundary(0x004dce52u,0x004ea8d0u,f);
    /* 004dce57 */
    f->r[2]=MI_U32((0x00f53db4u));
    /* 004dce5d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce5e */
    f->r[0]=MI_U32((0x00f53db8u));
    /* 004dce63 */
    a=f->r[2];b=0xffffff90u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dce66 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce67 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce68 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dce6du;f->boundary(0x004dce68u,0x004261d0u,f);
    /* 004dce6d */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dce70 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dce71 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dce72 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dce73 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dce74 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dce77 */
    f->r[4]+=4;return;
mi_004dce78:;
    /* 004dce78 */
    a=(f->r[0]&0xffu);b=0x00000035u;t=mi_alu(f,a,b,8,1);
    /* 004dce7a */
    if(!(f->flags&MI_ZF))goto mi_004dcf24;
    /* 004dce80 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 004dce86 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 004dce8c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce8e */
    a=f->r[1];b=0x0000019au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dce94 */
    a=0x00579788u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dce99 */
    a=f->r[2];b=0x00000234u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dce9f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcea0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcea1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcea6u;f->boundary(0x004dcea1u,0x00425990u,f);
    /* 004dcea6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcea9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dceab */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dceb1 */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 004dceb6 */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 004dcebc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcebd */
    a=f->r[0];b=0x000001a0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dcec2 */
    a=0x005c81b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcec7 */
    a=f->r[1];b=0x0000006eu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dceca */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcecb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcecc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dced1u;f->boundary(0x004dceccu,0x00425990u,f);
    /* 004dced1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dced4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dced6 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dcedc */
    f->r[1]=0x006fdd08u;
    /* 004dcee1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcee6u;f->boundary(0x004dcee1u,0x0041b8f0u,f);
    /* 004dcee6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dcee8 */
    if((f->flags&MI_ZF))goto mi_004dcf17;
    /* 004dceea */
    f->r[2]=MI_U32((0x006fdc9cu));
    /* 004dcef0 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 004dcef5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcef7 */
    a=f->r[2];b=0x0000019cu;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dcefd */
    a=0x005797b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf02 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf03 */
    a=f->r[0];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_004dcf06:;
    /* 004dcf06 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcf0cu;f->boundary(0x004dcf07u,0x00425990u,f);
    /* 004dcf0c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcf0f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dcf11 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dcf17:;
    /* 004dcf17 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcf1cu;f->boundary(0x004dcf17u,0x00418fe0u,f);
    /* 004dcf1c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcf1d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcf1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcf1f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcf20 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcf23 */
    f->r[4]+=4;return;
mi_004dcf24:;
    /* 004dcf24 */
    a=(f->r[0]&0xffu);b=0x00000034u;t=mi_alu(f,a,b,8,1);
    /* 004dcf26 */
    if(!(f->flags&MI_ZF))goto mi_004dcf6f;
    /* 004dcf28 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007027a4u)))&0xffu)<<0);
    /* 004dcf2d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dcf2f */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004dcf62;
    /* 004dcf31 */
    f->r[1]=MI_U32((0x006fdc9cu));
    /* 004dcf37 */
    f->r[2]=MI_U32((0x006fdca0u));
    /* 004dcf3d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf3f */
    a=f->r[1];b=0x0000019au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dcf45 */
    a=0x00579788u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf4a */
    a=f->r[2];b=0x00000244u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dcf50 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf51 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcf57u;f->boundary(0x004dcf52u,0x00425990u,f);
    /* 004dcf57 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcf5a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dcf5c */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dcf62:;
    /* 004dcf62 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcf67u;f->boundary(0x004dcf62u,0x00419090u,f);
    /* 004dcf67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dcf68 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dcf69 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dcf6a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dcf6b */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcf6e */
    f->r[4]+=4;return;
mi_004dcf6f:;
    /* 004dcf6f */
    a=(f->r[0]&0xffu);b=0x0000003bu;t=mi_alu(f,a,b,8,1);
    /* 004dcf71 */
    if(!(f->flags&MI_ZF))goto mi_004dcfc2;
    /* 004dcf73 */
    f->r[0]=MI_U32((0x006fdc9cu));
    /* 004dcf78 */
    f->r[1]=MI_U32((0x006fdca0u));
    /* 004dcf7e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf80 */
    a=f->r[0];b=0x0000019au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dcf85 */
    a=0x00579788u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf8a */
    a=f->r[1];b=0x00000234u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dcf90 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf91 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcf92 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcf97u;f->boundary(0x004dcf92u,0x00425990u,f);
    /* 004dcf97 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcf9a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dcf9c */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dcfa2 */
    f->r[2]=MI_U32((0x006fdc9cu));
    /* 004dcfa8 */
    f->r[0]=MI_U32((0x006fdca0u));
    /* 004dcfad */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcfae */
    a=f->r[2];b=0x000001a0u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dcfb4 */
    a=0x005c81b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcfb9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcfba */
    a=f->r[0];b=0x0000006eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dcfbd */
    goto mi_004dcf06;
mi_004dcfc2:;
    /* 004dcfc2 */
    a=(f->r[0]&0xffu);b=0x0000003du;t=mi_alu(f,a,b,8,1);
    /* 004dcfc4 */
    if(!(f->flags&MI_ZF))goto mi_004dd16a;
    /* 004dcfca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcfcfu;f->boundary(0x004dcfcau,0x0041f4d0u,f);
    /* 004dcfcf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dcfd1 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dcfd7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcfdcu;f->boundary(0x004dcfd7u,0x0041e170u,f);
    /* 004dcfdc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcfdd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcfe2u;f->boundary(0x004dcfddu,0x0041e100u,f);
    /* 004dcfe2 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dcfe8 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dcfee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcfef */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcff0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dcff1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dcff6u;f->boundary(0x004dcff1u,0x00425990u,f);
    /* 004dcff6 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dcff9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dcffb */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd001 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd006u;f->boundary(0x004dd001u,0x0041e140u,f);
    /* 004dd006 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd008 */
    if((f->flags&MI_ZF))goto mi_004dd02e;
    /* 004dd00a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd00b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd010u;f->boundary(0x004dd00bu,0x0041e130u,f);
    /* 004dd010 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd016 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd017 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd01c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd01d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd01e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd023u;f->boundary(0x004dd01eu,0x00425990u,f);
    /* 004dd023 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd026 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd028 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd02e:;
    /* 004dd02e */
    a=MI_U32((0x00704db0u));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004dd035 */
    if(!(f->flags&MI_ZF))goto mi_004dd0d0;
    /* 004dd03b */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dd041 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dd046 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd047 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd048 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd04a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd04fu;f->boundary(0x004dd04au,0x0041e120u,f);
    /* 004dd04f */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dd055 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dd05b */
    a=f->r[1];b=0x0000019au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dd061 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd062 */
    a=f->r[2];b=0x00000168u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dd068 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd069 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd06a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd06fu;f->boundary(0x004dd06au,0x00426450u,f);
    /* 004dd06f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd072 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd073 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd075 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd07au;f->boundary(0x004dd075u,0x0041e120u,f);
    /* 004dd07a */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd080 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd081 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd086 */
    a=f->r[1];b=0x00000168u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dd08c */
    a=f->r[0];b=0x0000019au;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dd091 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd092 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd093 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd098u;f->boundary(0x004dd093u,0x004d6740u,f);
    /* 004dd098 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd09b */
    MI_U32((0x007056c4u))=(uint32_t)(f->r[0]);
    /* 004dd0a0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0a2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd0a7u;f->boundary(0x004dd0a2u,0x0041e120u,f);
    /* 004dd0a7 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dd0ad */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0ae */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dd0b3 */
    a=f->r[2];b=0x0000019au;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dd0b9 */
    a=f->r[0];b=0x0000015eu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dd0be */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0bf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0c0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd0c5u;f->boundary(0x004dd0c0u,0x004261d0u,f);
    /* 004dd0c5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd0c8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd0ca */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd0d0:;
    /* 004dd0d0 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dd0d6 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dd0dc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0dd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0de */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd0e3u;f->boundary(0x004dd0deu,0x0041f210u,f);
    /* 004dd0e3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0e4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd0e9u;f->boundary(0x004dd0e4u,0x0041e190u,f);
    /* 004dd0e9 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd0ef */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0f0 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd0f5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0f6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd0f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd0fcu;f->boundary(0x004dd0f7u,0x00426450u,f);
    /* 004dd0fc */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd0ff */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd100 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd105u;f->boundary(0x004dd100u,0x0041f210u,f);
    /* 004dd105 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd106 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd10bu;f->boundary(0x004dd106u,0x0041e190u,f);
    /* 004dd10b */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dd111 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd112 */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dd117 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd118 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd119 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd11eu;f->boundary(0x004dd119u,0x004d6740u,f);
    /* 004dd11e */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd121 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dd126 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd12bu;f->boundary(0x004dd126u,0x0041f210u,f);
    /* 004dd12b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd12c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd131u;f->boundary(0x004dd12cu,0x0041e190u,f);
    /* 004dd131 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dd137 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dd13d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd13e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd13f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd140 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd145u;f->boundary(0x004dd140u,0x004261d0u,f);
    /* 004dd145 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd148 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd14a */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd150 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd155 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd157 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd15d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd162u;f->boundary(0x004dd15du,0x0041f650u,f);
    /* 004dd162 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd163 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd164 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd165 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd166 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd169 */
    f->r[4]+=4;return;
mi_004dd16a:;
    /* 004dd16a */
    a=(f->r[0]&0xffu);b=0x0000002fu;t=mi_alu(f,a,b,8,1);
    /* 004dd16c */
    if(!(f->flags&MI_ZF))goto mi_004dd223;
    /* 004dd172 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd177u;f->boundary(0x004dd172u,0x005168e0u,f);
    /* 004dd177 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd179 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd17f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd184u;f->boundary(0x004dd17fu,0x005165b0u,f);
    /* 004dd184 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd18a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd18b */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd190 */
    a=0x005f7ee0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd195 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd196 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd197 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd19cu;f->boundary(0x004dd197u,0x00425990u,f);
    /* 004dd19c */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd19f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd1a1 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd1a7 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd1ac */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd1b2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1b3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1b4 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1b6 */
    a=0x005f7f28u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1bb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1bc */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd1c2u;f->boundary(0x004dd1bdu,0x00426450u,f);
    /* 004dd1c2 */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dd1c8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd1cb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1cc */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dd1d1 */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1d3 */
    a=0x005f7f28u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1d8 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1d9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1da */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd1dfu;f->boundary(0x004dd1dau,0x004d6740u,f);
    /* 004dd1df */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dd1e5 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dd1eb */
    a=0x00000010u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1ed */
    a=0x005f7f28u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1f2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1f3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd1f4 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dd1f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd1feu;f->boundary(0x004dd1f9u,0x004261d0u,f);
    /* 004dd1fe */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd201 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd203 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd209 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd20e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd210 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd216 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd21bu;f->boundary(0x004dd216u,0x00402bb0u,f);
    /* 004dd21b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd21c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd21d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd21e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd21f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd222 */
    f->r[4]+=4;return;
mi_004dd223:;
    /* 004dd223 */
    a=(f->r[0]&0xffu);b=0x00000030u;t=mi_alu(f,a,b,8,1);
    /* 004dd225 */
    if(!(f->flags&MI_ZF))goto mi_004dd2a2;
    /* 004dd227 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd22c */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd232 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd233 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd234 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd236 */
    a=0x005e0408u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd23b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd23c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd23d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd242u;f->boundary(0x004dd23du,0x00426450u,f);
    /* 004dd242 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd248 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd24b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd24c */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd251 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd253 */
    a=0x005e0408u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd258 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd259 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd25a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd25fu;f->boundary(0x004dd25au,0x004d6740u,f);
    /* 004dd25f */
    f->r[2]=MI_U32((0x00e38da4u));
    /* 004dd265 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dd26a */
    f->r[0]=MI_U32((0x00e38da8u));
    /* 004dd26f */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd271 */
    a=0x005e0408u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd276 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd277 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd278 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd27du;f->boundary(0x004dd278u,0x004261d0u,f);
    /* 004dd27d */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd280 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd282 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd288 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd28d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd28f */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd295 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd29au;f->boundary(0x004dd295u,0x004cb970u,f);
    /* 004dd29a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd29b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd29c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd29d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd29e */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd2a1 */
    f->r[4]+=4;return;
mi_004dd2a2:;
    /* 004dd2a2 */
    a=(f->r[0]&0xffu);b=0x0000003eu;t=mi_alu(f,a,b,8,1);
    /* 004dd2a4 */
    if(!(f->flags&MI_ZF))goto mi_004dd345;
    /* 004dd2aa */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd2af */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd2b5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2b6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2b7 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2b9 */
    a=f->r[0];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dd2bc */
    a=0x005c68a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2c1 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dd2c4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2c5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd2cbu;f->boundary(0x004dd2c6u,0x00426450u,f);
    /* 004dd2cb */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dd2d1 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dd2d7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd2da */
    a=f->r[1];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dd2dd */
    a=f->r[2];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dd2e0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2e1 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2e3 */
    a=0x005c68a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2e8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2e9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd2ea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd2efu;f->boundary(0x004dd2eau,0x004d6740u,f);
    /* 004dd2ef */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd2f5 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dd2fa */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd2ff */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd301 */
    a=f->r[0];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dd304 */
    a=0x005c68a8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd309 */
    a=f->r[1];b=0x00000050u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dd30c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd30d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd30e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd313u;f->boundary(0x004dd30eu,0x004261d0u,f);
    /* 004dd313 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd316 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd318 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd31e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd324u;f->boundary(0x004dd31eu,0x0056f22cu,f);
    /* 004dd324 */
    f->r[2]=MI_U32((0x006057e8u));
    /* 004dd32a */
    a=f->r[2];b=0x00002710u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dd330 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004dd332 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004dd8c3;
    /* 004dd338 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd33du;f->boundary(0x004dd338u,0x00402de0u,f);
    /* 004dd33d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd33e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd33f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd340 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd341 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd344 */
    f->r[4]+=4;return;
mi_004dd345:;
    /* 004dd345 */
    a=(f->r[0]&0xffu);b=0x00000036u;t=mi_alu(f,a,b,8,1);
    /* 004dd347 */
    if(!(f->flags&MI_ZF))goto mi_004dd398;
    /* 004dd349 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd34b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd350u;f->boundary(0x004dd34bu,0x004cc500u,f);
    /* 004dd350 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd355 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd35b */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd35d */
    a=f->r[0];b=0x000000f9u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dd362 */
    a=0x00579788u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd367 */
    a=f->r[1];b=0x000001d0u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dd36d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd36e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd36f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd374u;f->boundary(0x004dd36fu,0x00425990u,f);
    /* 004dd374 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd377 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd379 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd37f */
    a=MI_U32((0x00e20998u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dd385 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd38b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd390u;f->boundary(0x004dd38bu,0x00418220u,f);
    /* 004dd390 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd391 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd392 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd393 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd394 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd397 */
    f->r[4]+=4;return;
mi_004dd398:;
    /* 004dd398 */
    a=(f->r[0]&0xffu);b=0x00000051u;t=mi_alu(f,a,b,8,1);
    /* 004dd39a */
    if(!(f->flags&MI_ZF))goto mi_004dd3c5;
    /* 004dd39c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd3a1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd3a3 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd3a9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd3aa */
    MI_U32((0x00e20998u))=(uint32_t)(f->r[5]);
    /* 004dd3b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd3b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd3b2 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000036u);
    /* 004dd3b9 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd3c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd3c1 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd3c4 */
    f->r[4]+=4;return;
mi_004dd3c5:;
    /* 004dd3c5 */
    a=(f->r[0]&0xffu);b=0x0000003cu;t=mi_alu(f,a,b,8,1);
    /* 004dd3c7 */
    if(!(f->flags&MI_ZF))goto mi_004dd3f5;
    /* 004dd3c9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd3ce */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd3d0 */
    if(!(f->flags&MI_ZF))goto mi_004dd3df;
    /* 004dd3d2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e04u)))&0xffu)<<0);
    /* 004dd3d7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd3d9 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd3df:;
    /* 004dd3df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd3e0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd3e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd3e2 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000000u);
    /* 004dd3e9 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd3f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd3f1 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd3f4 */
    f->r[4]+=4;return;
mi_004dd3f5:;
    /* 004dd3f5 */
    a=(f->r[0]&0xffu);b=0x00000037u;t=mi_alu(f,a,b,8,1);
    /* 004dd3f7 */
    if(!(f->flags&MI_ZF))goto mi_004dd425;
    /* 004dd3f9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd3fe */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd400 */
    if(!(f->flags&MI_ZF))goto mi_004dd40f;
    /* 004dd402 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd407 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd409 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd40f:;
    /* 004dd40f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd410 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd411 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd412 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000038u);
    /* 004dd419 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd420 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd421 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd424 */
    f->r[4]+=4;return;
mi_004dd425:;
    /* 004dd425 */
    a=(f->r[0]&0xffu);b=0x00000038u;t=mi_alu(f,a,b,8,1);
    /* 004dd427 */
    if(!(f->flags&MI_ZF))goto mi_004dd455;
    /* 004dd429 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd42e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd430 */
    if(!(f->flags&MI_ZF))goto mi_004dd46f;
    /* 004dd432 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd437 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd439 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd43f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd440 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd441 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd442 */
    MI_U8((0x00d3b43du))=(uint8_t)(0x00000001u);
    /* 004dd449 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd450 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd451 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd454 */
    f->r[4]+=4;return;
mi_004dd455:;
    /* 004dd455 */
    a=(f->r[0]&0xffu);b=0x00000039u;t=mi_alu(f,a,b,8,1);
    /* 004dd457 */
    if(!(f->flags&MI_ZF))goto mi_004dd485;
    /* 004dd459 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd45e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd460 */
    if(!(f->flags&MI_ZF))goto mi_004dd46f;
    /* 004dd462 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd467 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd469 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd46f:;
    /* 004dd46f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd470 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd471 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd472 */
    MI_U8((0x00d3b43du))=(uint8_t)(0x00000001u);
    /* 004dd479 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd480 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd481 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd484 */
    f->r[4]+=4;return;
mi_004dd485:;
    /* 004dd485 */
    a=(f->r[0]&0xffu);b=0x0000003au;t=mi_alu(f,a,b,8,1);
    /* 004dd487 */
    if(!(f->flags&MI_ZF))goto mi_004dd4b5;
    /* 004dd489 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd48e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd490 */
    if(!(f->flags&MI_ZF))goto mi_004dd49f;
    /* 004dd492 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd497 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd499 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd49f:;
    /* 004dd49f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd4a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd4a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd4a2 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000000u);
    /* 004dd4a9 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd4b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd4b1 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd4b4 */
    f->r[4]+=4;return;
mi_004dd4b5:;
    /* 004dd4b5 */
    a=(f->r[0]&0xffu);b=0x0000001eu;t=mi_alu(f,a,b,8,1);
    /* 004dd4b7 */
    if(!(f->flags&MI_ZF))goto mi_004dd513;
    /* 004dd4b9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd4beu;f->boundary(0x004dd4b9u,0x00405f80u,f);
    /* 004dd4be */
    a=MI_U32((0x00f53f78u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dd4c4 */
    if(!(f->flags&MI_ZF))goto mi_004dd4dd;
    /* 004dd4c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd4cbu;f->boundary(0x004dd4c6u,0x00406190u,f);
    /* 004dd4cb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd4cd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd4d2u;f->boundary(0x004dd4cdu,0x004061e0u,f);
    /* 004dd4d2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd4d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd4d7 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd4dd:;
    /* 004dd4dd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd4e2u;f->boundary(0x004dd4ddu,0x004cf660u,f);
    /* 004dd4e2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd4e3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd4e8u;f->boundary(0x004dd4e3u,0x004ddde0u,f);
    /* 004dd4e8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd4ed */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd4f0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd4f2 */
    if(!(f->flags&MI_ZF))goto mi_004dd552;
    /* 004dd4f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd4f9u;f->boundary(0x004dd4f4u,0x00405f80u,f);
    /* 004dd4f9 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd4fe */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd500 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd506 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd50bu;f->boundary(0x004dd506u,0x0040c440u,f);
    /* 004dd50b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd50c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd50d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd50e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd50f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd512 */
    f->r[4]+=4;return;
mi_004dd513:;
    /* 004dd513 */
    a=(f->r[0]&0xffu);b=0x00000049u;t=mi_alu(f,a,b,8,1);
    /* 004dd515 */
    if(!(f->flags&MI_ZF))goto mi_004dd58c;
    /* 004dd517 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd51cu;f->boundary(0x004dd517u,0x00405f80u,f);
    /* 004dd51c */
    a=MI_U32((0x00f53f78u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dd522 */
    if(!(f->flags&MI_ZF))goto mi_004dd53b;
    /* 004dd524 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd529u;f->boundary(0x004dd524u,0x00406190u,f);
    /* 004dd529 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd52b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd530u;f->boundary(0x004dd52bu,0x004061e0u,f);
    /* 004dd530 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd533 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd535 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd53b:;
    /* 004dd53b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd540u;f->boundary(0x004dd53bu,0x004cf660u,f);
    /* 004dd540 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd541 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd546u;f->boundary(0x004dd541u,0x004ddde0u,f);
    /* 004dd546 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd54b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd54e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd550 */
    if((f->flags&MI_ZF))goto mi_004dd56d;
mi_004dd552:;
    /* 004dd552 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004dd559 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd55eu;f->boundary(0x004dd559u,0x004e2ab0u,f);
mi_004dd55e:;
    /* 004dd55e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd55f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd560 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd561 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd568 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd569 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd56c */
    f->r[4]+=4;return;
mi_004dd56d:;
    /* 004dd56d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd572u;f->boundary(0x004dd56du,0x00405f80u,f);
    /* 004dd572 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd577 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd579 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd57f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd584u;f->boundary(0x004dd57fu,0x0040c570u,f);
    /* 004dd584 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd585 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd586 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd587 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd588 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd58b */
    f->r[4]+=4;return;
mi_004dd58c:;
    /* 004dd58c */
    a=(f->r[0]&0xffu);b=0x0000001fu;t=mi_alu(f,a,b,8,1);
    /* 004dd58e */
    if(!(f->flags&MI_ZF))goto mi_004dd60b;
    /* 004dd590 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd595u;f->boundary(0x004dd590u,0x00405f80u,f);
    /* 004dd595 */
    a=MI_U32((0x00f53f78u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dd59b */
    if(!(f->flags&MI_ZF))goto mi_004dd5b4;
    /* 004dd59d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd5a2u;f->boundary(0x004dd59du,0x00406190u,f);
    /* 004dd5a2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd5a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd5a9u;f->boundary(0x004dd5a4u,0x004061e0u,f);
    /* 004dd5a9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd5ac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd5ae */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd5b4:;
    /* 004dd5b4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd5b9u;f->boundary(0x004dd5b4u,0x004cf660u,f);
    /* 004dd5b9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd5ba */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd5bfu;f->boundary(0x004dd5bau,0x004ddde0u,f);
    /* 004dd5bf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd5c4 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd5c7 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd5c9 */
    if((f->flags&MI_ZF))goto mi_004dd5ec;
    /* 004dd5cb */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004dd5d2 */
    MI_U32((0x00d05708u))=(uint32_t)(f->r[5]);
    /* 004dd5d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd5ddu;f->boundary(0x004dd5d8u,0x004e2ab0u,f);
    /* 004dd5dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd5de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd5df */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd5e0 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd5e7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd5e8 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd5eb */
    f->r[4]+=4;return;
mi_004dd5ec:;
    /* 004dd5ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd5f1u;f->boundary(0x004dd5ecu,0x00405f80u,f);
    /* 004dd5f1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd5f6 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd5f8 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd5fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd603u;f->boundary(0x004dd5feu,0x0040ba40u,f);
    /* 004dd603 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd604 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd605 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd606 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd607 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd60a */
    f->r[4]+=4;return;
mi_004dd60b:;
    /* 004dd60b */
    a=(f->r[0]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,1);
    /* 004dd60d */
    if(!(f->flags&MI_ZF))goto mi_004dd68a;
    /* 004dd60f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd614u;f->boundary(0x004dd60fu,0x00405f80u,f);
    /* 004dd614 */
    a=MI_U32((0x00f53f78u));b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004dd61a */
    if(!(f->flags&MI_ZF))goto mi_004dd633;
    /* 004dd61c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd621u;f->boundary(0x004dd61cu,0x00406190u,f);
    /* 004dd621 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd623 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd628u;f->boundary(0x004dd623u,0x004061e0u,f);
    /* 004dd628 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd62b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd62d */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
mi_004dd633:;
    /* 004dd633 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd638u;f->boundary(0x004dd633u,0x004cf660u,f);
    /* 004dd638 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd639 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd63eu;f->boundary(0x004dd639u,0x004ddde0u,f);
    /* 004dd63e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd643 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd646 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd648 */
    if((f->flags&MI_ZF))goto mi_004dd66b;
    /* 004dd64a */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000001u);
    /* 004dd651 */
    MI_U32((0x00d0570cu))=(uint32_t)(f->r[5]);
    /* 004dd657 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd65cu;f->boundary(0x004dd657u,0x004e2ab0u,f);
    /* 004dd65c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd65d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd65e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd65f */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd666 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd667 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd66a */
    f->r[4]+=4;return;
mi_004dd66b:;
    /* 004dd66b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd670u;f->boundary(0x004dd66bu,0x00405f80u,f);
    /* 004dd670 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38e03u)))&0xffu)<<0);
    /* 004dd675 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd677 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd67d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd682u;f->boundary(0x004dd67du,0x0040b830u,f);
    /* 004dd682 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd683 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd684 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd685 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd686 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd689 */
    f->r[4]+=4;return;
mi_004dd68a:;
    /* 004dd68a */
    a=(f->r[0]&0xffu);b=0x0000004fu;t=mi_alu(f,a,b,8,1);
    /* 004dd68c */
    if(!(f->flags&MI_ZF))goto mi_004dd73a;
    /* 004dd692 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd697 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd699 */
    if((f->flags&MI_ZF))goto mi_004dd6b0;
    /* 004dd69b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd69c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd69d */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dd6a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd6a4 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd6ab */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd6ac */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd6af */
    f->r[4]+=4;return;
mi_004dd6b0:;
    /* 004dd6b0 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004dd6b6 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004dd6bc */
    f->r[0]=MI_U32((0x00ea5fd8u+f->r[2]));
    /* 004dd6c2 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x006fdc88u+f->r[0])))&0xffu)<<0);
    /* 004dd6c8 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd6ca */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd6d0 */
    f->r[2]=MI_U32((0x005f78f0u+f->r[0]*4u));
    /* 004dd6d7 */
    f->r[1]=(0x00000028u+f->r[0]);
    /* 004dd6da */
    f->r[0]=MI_U32((0x00f53f08u));
    /* 004dd6df */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd6e0 */
    f->r[1]=MI_U32((0x00f53f0cu));
    /* 004dd6e6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd6e8 */
    a=f->r[0];b=0x00000110u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dd6ed */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd6ee */
    a=f->r[1];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dd6f1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd6f2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd6f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd6f8u;f->boundary(0x004dd6f3u,0x00425bc0u,f);
    /* 004dd6f8 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004dd6fe */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd700 */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 004dd706 */
    f->r[0]=MI_U32((0x00ea5fd8u+f->r[2]));
    /* 004dd70c */
    f->r[2]=MI_U32((0x00f53f08u));
    /* 004dd712 */
    a=f->r[2];b=0x00000110u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dd718 */
    f->r[1]=MI_U32((0x005f78f0u+f->r[0]*4u));
    /* 004dd71f */
    f->r[0]=MI_U32((0x00f53f0cu));
    /* 004dd724 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd725 */
    a=f->r[0];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dd728 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd729 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd72a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd72fu;f->boundary(0x004dd72au,0x00425990u,f);
    /* 004dd72f */
    a=f->r[4];b=0x00000024u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd732 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd733 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd734 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd735 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd736 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd739 */
    f->r[4]+=4;return;
mi_004dd73a:;
    /* 004dd73a */
    a=(f->r[0]&0xffu);b=0x00000050u;t=mi_alu(f,a,b,8,1);
    /* 004dd73c */
    if(!(f->flags&MI_ZF))goto mi_004dd760;
    /* 004dd73e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd743 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd745 */
    if((f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd74b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd74c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd74d */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[3]&0xffu));
    /* 004dd753 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd754 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd75b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd75c */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd75f */
    f->r[4]+=4;return;
mi_004dd760:;
    /* 004dd760 */
    a=(f->r[0]&0xffu);b=0x00000053u;t=mi_alu(f,a,b,8,1);
    /* 004dd762 */
    if(!(f->flags&MI_ZF))goto mi_004dd7b4;
    /* 004dd764 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df8u)))&0xffu)<<0);
    /* 004dd769 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd76b */
    if((f->flags&MI_ZF))goto mi_004dd783;
    /* 004dd76d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd76e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd76f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd770 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000000u);
    /* 004dd777 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd77e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd77f */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd782 */
    f->r[4]+=4;return;
mi_004dd783:;
    /* 004dd783 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd788u;f->boundary(0x004dd783u,0x004c57d0u,f);
    /* 004dd788 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dd78a */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd790 */
    f->r[1]=MI_U32((0x00e38da4u));
    /* 004dd796 */
    f->r[2]=MI_U32((0x00e38da8u));
    /* 004dd79c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd79d */
    a=0x005e01d0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7a2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7a3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd7a9u;f->boundary(0x004dd7a4u,0x00425990u,f);
    /* 004dd7a9 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd7ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd7ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd7ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd7af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd7b0 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd7b3 */
    f->r[4]+=4;return;
mi_004dd7b4:;
    /* 004dd7b4 */
    a=(f->r[0]&0xffu);b=0x00000054u;t=mi_alu(f,a,b,8,1);
    /* 004dd7b6 */
    if(!(f->flags&MI_ZF))goto mi_004dd811;
    /* 004dd7b8 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd7bd */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd7c3 */
    f->r[2]=MI_U32((0x00f535c8u));
    /* 004dd7c9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7ca */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7cb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7cc */
    a=0x005f8910u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7d1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7d2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7d3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd7d8u;f->boundary(0x004dd7d3u,0x00426450u,f);
    /* 004dd7d8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd7db */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7dc */
    f->r[0]=MI_U32((0x00f535c8u));
    /* 004dd7e1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7e2 */
    a=0x005f8910u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7e7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7e8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd7eeu;f->boundary(0x004dd7e9u,0x004d6740u,f);
    /* 004dd7ee */
    f->r[1]=MI_U32((0x00f535c8u));
    /* 004dd7f4 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dd7f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7fa */
    a=0x005f8910u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd7ff */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd800 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd801 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd806u;f->boundary(0x004dd801u,0x004261d0u,f);
    /* 004dd806 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd809 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd80a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd80b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd80c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd80d */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd810 */
    f->r[4]+=4;return;
mi_004dd811:;
    /* 004dd811 */
    a=(f->r[0]&0xffu);b=0x00000055u;t=mi_alu(f,a,b,8,1);
    /* 004dd813 */
    if(!(f->flags&MI_ZF))goto mi_004dd8c3;
    /* 004dd819 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd81eu;f->boundary(0x004dd819u,0x0041d310u,f);
    /* 004dd81e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd81f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd820 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd821 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd822 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd825 */
    f->r[4]+=4;return;
mi_004dd826:;
    /* 004dd826 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 004dd82b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004dd82d */
    if((f->flags&MI_ZF))goto mi_004dd850;
    /* 004dd82f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00605958u)))&0xffu)<<0);
    /* 004dd835 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd836 */
    MI_U32((0x00605a58u))=(uint32_t)(f->r[5]);
    /* 004dd83c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd83d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd83e */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[2]&0xffu));
    /* 004dd844 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004dd84b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd84c */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd84f */
    f->r[4]+=4;return;
mi_004dd850:;
    /* 004dd850 */
    f->r[0]=MI_U32((0x00e38da4u));
    /* 004dd855 */
    f->r[1]=MI_U32((0x00e38da8u));
    /* 004dd85b */
    f->r[2]=MI_U32((0x00605948u));
    /* 004dd861 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd862 */
    f->r[0]=MI_U32((0x0060594cu));
    /* 004dd867 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd868 */
    f->r[1]=MI_U32((0x00605954u));
    /* 004dd86e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd86f */
    a=0x00704dc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd874 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd875 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd876 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd87bu;f->boundary(0x004dd876u,0x00426450u,f);
    /* 004dd87b */
    f->r[2]=MI_U32((0x00605948u));
    /* 004dd881 */
    f->r[1]=MI_U32((0x00605954u));
    /* 004dd887 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd88a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd88b */
    f->r[0]=MI_U32((0x0060594cu));
    /* 004dd890 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd891 */
    a=0x00704dc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd896 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd897 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd898 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd89du;f->boundary(0x004dd898u,0x004d6740u,f);
    /* 004dd89d */
    f->r[2]=MI_U32((0x00605948u));
    /* 004dd8a3 */
    f->r[1]=MI_U32((0x00605954u));
    /* 004dd8a9 */
    MI_U32((0x007056c8u))=(uint32_t)(f->r[0]);
    /* 004dd8ae */
    f->r[0]=MI_U32((0x0060594cu));
    /* 004dd8b3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd8b4 */
    a=0x00704dc8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd8b9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd8ba */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dd8bb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dd8c0u;f->boundary(0x004dd8bbu,0x004261d0u,f);
    /* 004dd8c0 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004dd8c3:;
    /* 004dd8c3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dd8c4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dd8c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dd8c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dd8c7 */
    a=f->r[4];b=0x0000007cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dd8ca */
    f->r[4]+=4;return;
}
