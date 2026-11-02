/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_view_helpers_model.h"
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
static __inline uint32_t mi_alu(struct restore_building_world_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_building_world_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_building_world_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_building_world_00431690(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00431690 */
    f->r[0]=MI_U32((0x00e94e0cu));
    /* 00431695 */
    a=f->r[0];b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 00431698 */
    if(!(f->flags&MI_ZF))goto mi_0043169d;
    /* 0043169a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0043169c */
    f->r[4]+=4;return;
mi_0043169d:;
    /* 0043169d */
    f->r[2]=MI_U32((0x00000004u+f->r[4]));
    /* 004316a1 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004316a4 */
    if(!(f->flags&MI_ZF))goto mi_004316bb;
    /* 004316a6 */
    f->r[1]=f->r[2];
    /* 004316a8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004316ab */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004316ad */
    a=MI_U16((0x00799a22u+f->r[1]*8u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004316b6 */
    if((f->flags&MI_ZF))goto mi_004316bb;
mi_004316b8:;
    /* 004316b8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004316ba */
    f->r[4]+=4;return;
mi_004316bb:;
    /* 004316bb */
    f->r[1]=MI_U32((0x005d25acu+f->r[0]*4u));
    /* 004316c2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004316c4 */
    if((f->flags&MI_ZF))goto mi_004316eb;
    /* 004316c6 */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 004316c8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004316ca */
    if((f->flags&MI_ZF))goto mi_004316eb;
    /* 004316cc */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004316cd */
    f->r[6]=f->r[2];
    /* 004316cf */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004316d2 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004316d4 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e8u+f->r[6]*8u));
    /* 004316dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_004316dd:;
    /* 004316dd */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004316df */
    if((f->flags&MI_ZF))goto mi_004316b8;
    /* 004316e1 */
    f->r[0]=MI_U32((0x00000004u+f->r[1]));
    /* 004316e4 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004316e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004316e9 */
    if(!(f->flags&MI_ZF))goto mi_004316dd;
mi_004316eb:;
    /* 004316eb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004316ed */
    f->r[4]+=4;return;
}

void restore_building_world_0043e300(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0043e300 */
    f->r[1]=MI_U32((0x0078d7d8u));
    /* 0043e306 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0043e309 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e30b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e30c */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043e30e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e30f */
    if(!(f->flags&MI_ZF))goto mi_0043e325;
    /* 0043e311 */
    a=MI_U32((0x0078d7dcu));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043e317 */
    if(!(f->flags&MI_ZF))goto mi_0043e325;
    /* 0043e319 */
    a=MI_U32((0x0078d7d4u));b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043e31f */
    if((f->flags&MI_ZF))goto mi_0043e40e;
mi_0043e325:;
    /* 0043e325 */
    f->r[7]=MI_U32((0x0070e48cu));
    /* 0043e32b */
    f->r[6]=MI_U32((0x0070e44cu));
    /* 0043e331 */
    f->r[2]=MI_U32((0x0070e460u));
    /* 0043e337 */
    f->r[1]=MI_U32((0x0070e474u));
    /* 0043e33d */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 0043e33f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043e341 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043e345 */
    MI_U32((0x0070e47cu))=(uint32_t)(f->r[1]);
    /* 0043e34b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043e40e;
    /* 0043e351 */
    f->r[2]=MI_U32((0x0070e464u));
    /* 0043e357 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e358 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_0043e359:;
    /* 0043e359 */
    f->r[6]=MI_U32((0x0070e490u));
    /* 0043e35f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043e361 */
    if(!(f->flags&MI_ZF))goto mi_0043e36a;
    /* 0043e363 */
    f->r[0]=0x00000001u;
    /* 0043e368 */
    goto mi_0043e373;
mi_0043e36a:;
    /* 0043e36a */
    a=f->r[6];b=MI_U32((0x0070e45cu));t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043e370 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043e371 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_0043e373:;
    /* 0043e373 */
    f->r[5]=MI_U32((0x0070e478u));
    /* 0043e379 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0043e37b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0043e37d */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043e381 */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[5]);
    /* 0043e387 */
    if(mi_signed_less(f->flags))goto mi_0043e3e6;
mi_0043e389:;
    /* 0043e389 */
    f->r[5]=MI_U32((0x0070e480u));
    /* 0043e38f */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0043e392 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 0043e395 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 0043e398 */
    f->r[5]=(0x00000000u+f->r[5]+f->r[0]*2u);
    /* 0043e39c */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043e39e */
    f->r[0]=MI_U32((0x0070e494u+f->r[0]*4u));
    /* 0043e3a5 */
    a=f->r[0];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 0043e3aa */
    MI_U32((0x0070e488u))=(uint32_t)(f->r[0]);
    /* 0043e3af */
    if(!mi_signed_less(f->flags))goto mi_0043e3c7;
    /* 0043e3b1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e3b2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e3b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043e3b8u;f->boundary(0x0043e3b3u,0x0043e420u,f);
    /* 0043e3b8 */
    f->r[1]=MI_U32((0x0070e47cu));
    /* 0043e3be */
    f->r[2]=MI_U32((0x0070e464u));
    /* 0043e3c4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043e3c7:;
    /* 0043e3c7 */
    f->r[5]=MI_U32((0x0070e480u));
    /* 0043e3cd */
    f->r[0]=MI_U32((0x0070e458u));
    /* 0043e3d2 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0043e3d3 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0043e3d4 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043e3d6 */
    f->r[6]=(0x00000002u+f->r[6]+f->r[0]*1u);
    /* 0043e3da */
    MI_U32((0x0070e480u))=(uint32_t)(f->r[5]);
    /* 0043e3e0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043e389;
    /* 0043e3e2 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
mi_0043e3e6:;
    /* 0043e3e6 */
    f->r[5]=MI_U32((0x0070e44cu));
    /* 0043e3ec */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0043e3f0 */
    f->r[3]=MI_U32((0x0070e460u));
    /* 0043e3f6 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0043e3f8 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043e3f9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0043e3fa */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0043e3fc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0043e400 */
    MI_U32((0x0070e47cu))=(uint32_t)(f->r[1]);
    /* 0043e406 */
    if(mi_signed_less(f->flags))goto mi_0043e359;
    /* 0043e40c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043e40d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0043e40e:;
    /* 0043e40e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043e40f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043e410 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043e413 */
    f->r[4]+=4;return;
}

void restore_building_world_0043e420(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0043e420 */
    f->r[1]=MI_U32((0x0070e488u));
    /* 0043e426 */
    f->r[2]=MI_U32((0x00e38e94u));
    /* 0043e42c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e42d */
    f->r[6]=0x0000ffffu;
    /* 0043e432 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[1]*2u));
    /* 0043e43a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0043e43c */
    if((f->flags&MI_ZF))goto mi_0043e62b;
    /* 0043e442 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0043e445 */
    if(!(f->flags&MI_ZF))goto mi_0043e453;
    /* 0043e447 */
    f->r[0]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 0043e44e */
    goto mi_0043e62b;
mi_0043e453:;
    /* 0043e453 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043e456 */
    if(!(f->flags&MI_ZF))goto mi_0043e465;
    /* 0043e458 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e45a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[1])))&0xffu)<<0);
    /* 0043e460 */
    goto mi_0043e62b;
mi_0043e465:;
    /* 0043e465 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043e468 */
    if(!(f->flags&MI_ZF))goto mi_0043e476;
    /* 0043e46a */
    f->r[0]=MI_U32((0x00b7482cu+f->r[1]*4u));
    /* 0043e471 */
    goto mi_0043e62b;
mi_0043e476:;
    /* 0043e476 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043e479 */
    if(!(f->flags&MI_ZF))goto mi_0043e488;
    /* 0043e47b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e47d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[1])))&0xffu)<<0);
    /* 0043e483 */
    goto mi_0043e62b;
mi_0043e488:;
    /* 0043e488 */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0043e48b */
    if(!(f->flags&MI_ZF))goto mi_0043e49a;
    /* 0043e48d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e48f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ae8e7cu+f->r[1])))&0xffu)<<0);
    /* 0043e495 */
    goto mi_0043e62b;
mi_0043e49a:;
    /* 0043e49a */
    a=f->r[2];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0043e49d */
    if(!(f->flags&MI_ZF))goto mi_0043e4ac;
    /* 0043e49f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e4a1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[1])))&0xffu)<<0);
    /* 0043e4a7 */
    goto mi_0043e62b;
mi_0043e4ac:;
    /* 0043e4ac */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 0043e4af */
    if(!(f->flags&MI_ZF))goto mi_0043e4be;
    /* 0043e4b1 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[1]*2u));
    /* 0043e4b9 */
    goto mi_0043e62b;
mi_0043e4be:;
    /* 0043e4be */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0043e4c1 */
    if(!(f->flags&MI_ZF))goto mi_0043e4d0;
    /* 0043e4c3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e4c5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00adc36cu+f->r[1])))&0xffu)<<0);
    /* 0043e4cb */
    goto mi_0043e62b;
mi_0043e4d0:;
    /* 0043e4d0 */
    a=f->r[2];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0043e4d3 */
    if(!(f->flags&MI_ZF))goto mi_0043e4e1;
    /* 0043e4d5 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00ee0c50u+f->r[1]));
    /* 0043e4dc */
    goto mi_0043e62b;
mi_0043e4e1:;
    /* 0043e4e1 */
    a=f->r[2];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0043e4e4 */
    if(!(f->flags&MI_ZF))goto mi_0043e4f2;
    /* 0043e4e6 */
    f->r[0]=MI_U32((0x00eed760u+f->r[1]*4u));
    /* 0043e4ed */
    goto mi_0043e62b;
mi_0043e4f2:;
    /* 0043e4f2 */
    a=f->r[2];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0043e4f5 */
    if(!(f->flags&MI_ZF))goto mi_0043e50c;
    /* 0043e4f7 */
    f->r[1]=f->r[0];
    /* 0043e4f9 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043e4fc */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043e4fe */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e500 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a0du+f->r[1]*8u)))&0xffu)<<0);
    /* 0043e507 */
    goto mi_0043e62b;
mi_0043e50c:;
    /* 0043e50c */
    a=f->r[2];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0043e50f */
    if(!(f->flags&MI_ZF))goto mi_0043e525;
    /* 0043e511 */
    f->r[2]=f->r[0];
    /* 0043e513 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043e516 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043e518 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f0u+f->r[2]*8u));
    /* 0043e520 */
    goto mi_0043e62b;
mi_0043e525:;
    /* 0043e525 */
    a=f->r[2];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 0043e528 */
    if(!(f->flags&MI_ZF))goto mi_0043e53e;
    /* 0043e52a */
    f->r[1]=f->r[0];
    /* 0043e52c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043e52f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043e531 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f8u+f->r[1]*8u));
    /* 0043e539 */
    goto mi_0043e62b;
mi_0043e53e:;
    /* 0043e53e */
    a=f->r[2];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 0043e541 */
    if(!(f->flags&MI_ZF))goto mi_0043e557;
    /* 0043e543 */
    f->r[2]=f->r[0];
    /* 0043e545 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0043e548 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043e54a */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00799a1eu+f->r[2]*8u));
    /* 0043e552 */
    goto mi_0043e62b;
mi_0043e557:;
    /* 0043e557 */
    a=f->r[2];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0043e55a */
    if(!(f->flags&MI_ZF))goto mi_0043e570;
    /* 0043e55c */
    f->r[1]=f->r[0];
    /* 0043e55e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043e561 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043e563 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999f4u+f->r[1]*8u));
    /* 0043e56b */
    goto mi_0043e62b;
mi_0043e570:;
    /* 0043e570 */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0043e573 */
    if(!(f->flags&MI_ZF))goto mi_0043e581;
    /* 0043e575 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00ac2d4cu+f->r[1]));
    /* 0043e57c */
    goto mi_0043e62b;
mi_0043e581:;
    /* 0043e581 */
    a=f->r[2];b=0x00000011u;t=mi_alu(f,a,b,32,1);
    /* 0043e584 */
    if(!(f->flags&MI_ZF))goto mi_0043e593;
    /* 0043e586 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e588 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ab623cu+f->r[1])))&0xffu)<<0);
    /* 0043e58e */
    goto mi_0043e62b;
mi_0043e593:;
    /* 0043e593 */
    a=f->r[2];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 0043e596 */
    if(!(f->flags&MI_ZF))goto mi_0043e5a4;
    /* 0043e598 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00ed4140u+f->r[1]));
    /* 0043e59f */
    goto mi_0043e62b;
mi_0043e5a4:;
    /* 0043e5a4 */
    a=f->r[2];b=0x00000013u;t=mi_alu(f,a,b,32,1);
    /* 0043e5a7 */
    if(!(f->flags&MI_ZF))goto mi_0043e5b2;
    /* 0043e5a9 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00ec7630u+f->r[1]));
    /* 0043e5b0 */
    goto mi_0043e62b;
mi_0043e5b2:;
    /* 0043e5b2 */
    a=f->r[2];b=0x00000014u;t=mi_alu(f,a,b,32,1);
    /* 0043e5b5 */
    if(!(f->flags&MI_ZF))goto mi_0043e5c1;
    /* 0043e5b7 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00a9cc1cu+f->r[1]*2u));
    /* 0043e5bf */
    goto mi_0043e62b;
mi_0043e5c1:;
    /* 0043e5c1 */
    a=f->r[2];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0043e5c4 */
    if(!(f->flags&MI_ZF))goto mi_0043e5d1;
    /* 0043e5c6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e5c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043e5ccu;f->boundary(0x0043e5c7u,0x004b54f0u,f);
    /* 0043e5cc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043e5cf */
    goto mi_0043e62b;
mi_0043e5d1:;
    /* 0043e5d1 */
    a=f->r[2];b=0x00000016u;t=mi_alu(f,a,b,32,1);
    /* 0043e5d4 */
    if(!(f->flags&MI_ZF))goto mi_0043e5e0;
    /* 0043e5d6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e5d8 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00a9010cu+f->r[1])))&0xffu)<<0);
    /* 0043e5de */
    goto mi_0043e62b;
mi_0043e5e0:;
    /* 0043e5e0 */
    a=f->r[2];b=0x00000017u;t=mi_alu(f,a,b,32,1);
    /* 0043e5e3 */
    if(!(f->flags&MI_ZF))goto mi_0043e5ef;
    /* 0043e5e5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e5e7 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00a835fcu+f->r[1])))&0xffu)<<0);
    /* 0043e5ed */
    goto mi_0043e62b;
mi_0043e5ef:;
    /* 0043e5ef */
    a=f->r[2];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 0043e5f2 */
    if(!(f->flags&MI_ZF))goto mi_0043e5fd;
    /* 0043e5f4 */
    f->r[0]=(uint32_t)(int32_t)(int8_t)MI_U8((0x00ebab20u+f->r[1]));
    /* 0043e5fb */
    goto mi_0043e62b;
mi_0043e5fd:;
    /* 0043e5fd */
    a=f->r[2];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0043e600 */
    if(!(f->flags&MI_ZF))goto mi_0043e60c;
    /* 0043e602 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e604 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00a76aecu+f->r[1])))&0xffu)<<0);
    /* 0043e60a */
    goto mi_0043e62b;
mi_0043e60c:;
    /* 0043e60c */
    a=f->r[2];b=0x0000001au;t=mi_alu(f,a,b,32,1);
    /* 0043e60f */
    if(!(f->flags&MI_ZF))goto mi_0043e61b;
    /* 0043e611 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043e613 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00a69fdcu+f->r[1])))&0xffu)<<0);
    /* 0043e619 */
    goto mi_0043e62b;
mi_0043e61b:;
    /* 0043e61b */
    a=f->r[2];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 0043e61e */
    if(!(f->flags&MI_ZF))goto mi_0043e660;
    /* 0043e620 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e621 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e623 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043e628u;f->boundary(0x0043e623u,0x0047e230u,f);
    /* 0043e628 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043e62b:;
    /* 0043e62b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043e62d */
    if((f->flags&MI_ZF))goto mi_0043e660;
    /* 0043e62f */
    if(!mi_signed_less(f->flags))goto mi_0043e638;
    /* 0043e631 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0043e633 */
    f->r[6]=0x0000f800u;
mi_0043e638:;
    /* 0043e638 */
    f->r[2]=MI_U32((0x00f56f84u));
    /* 0043e63e */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 0043e642 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e643 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e644 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0043e648 */
    a=f->r[1];b=0x0000000au;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0043e64b */
    a=f->r[2];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043e64e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e64f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e650 */
    a=0x0057c048u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e655 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e657 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043e658 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043e65du;f->boundary(0x0043e658u,0x004cce00u,f);
    /* 0043e65d */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0043e660:;
    /* 0043e660 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043e661 */
    f->r[4]+=4;return;
}

void restore_building_world_0047e230(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047e230 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 0047e234 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047e237 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 0047e23b */
    f->r[0]=MI_U32((0x00978d9cu+f->r[0]*4u));
    /* 0047e242 */
    a=f->r[0];b=((f->r[1]&0xffu))&31u;if(b){t=a>>b;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>(b-1u))&1u)|mi_szp(t,32);if(b==1u)f->flags=(f->flags&~MI_OF)|((a&0x80000000u)?MI_OF:0);f->r[0]=t;} /* AF and OF for count>1 are architecturally unspecified; retained bits are not a status-equivalence claim. */
    /* 0047e244 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047e247 */
    f->r[4]+=4;return;
}

void restore_building_world_004b54f0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004b54f0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004b54f4 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004b54f6 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b350dcu+f->r[1]*2u));
    /* 004b54fe */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b5500 */
    if((f->flags&MI_ZF))goto mi_004b5517;
mi_004b5502:;
    /* 004b5502 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004b5505 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004b5506 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004b5509 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004b550b */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd600u+f->r[2]*4u));
    /* 004b5513 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004b5515 */
    if(!(f->flags&MI_ZF))goto mi_004b5502;
mi_004b5517:;
    /* 004b5517 */
    f->r[4]+=4;return;
}
