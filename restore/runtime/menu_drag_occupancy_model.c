/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "road_scan_model.h"

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

void restore_C_004413b0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004413b0 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 004413b4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004413b5 */
    a=f->r[0];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 004413b8 */
    if(!(f->flags&MI_ZF))goto mi_004413c1;
    /* 004413ba */
    f->r[6]=0x00000002u;
    /* 004413bf */
    goto mi_004413e9;
mi_004413c1:;
    /* 004413c1 */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 004413c4 */
    if(!(f->flags&MI_ZF))goto mi_004413cd;
    /* 004413c6 */
    f->r[6]=0x00000003u;
    /* 004413cb */
    goto mi_004413e9;
mi_004413cd:;
    /* 004413cd */
    a=f->r[0];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 004413d0 */
    if(!(f->flags&MI_ZF))goto mi_004413d9;
    /* 004413d2 */
    f->r[6]=0x00000004u;
    /* 004413d7 */
    goto mi_004413e9;
mi_004413d9:;
    /* 004413d9 */
    a=f->r[0];b=0x000000d1u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004413de */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004413e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004413e2 */
    a=(f->r[0]&0xffu);b=0x000000fcu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004413e4 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004413e7 */
    f->r[6]=f->r[0];
mi_004413e9:;
    /* 004413e9 */
    f->r[2]=0x00775614u;
mi_004413ee:;
    /* 004413ee */
    f->r[0]=f->r[2];
    /* 004413f0 */
    f->r[1]=0x00000005u;
mi_004413f5:;
    /* 004413f5 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 004413fb */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004413fe */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004413ff */
    if(!(f->flags&MI_ZF))goto mi_004413f5;
    /* 00441401 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00441404 */
    a=f->r[2];b=0x00775628u;t=mi_alu(f,a,b,32,1);
    /* 0044140a */
    if(mi_signed_less(f->flags))goto mi_004413ee;
    /* 0044140c */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 00441410 */
    f->r[1]=(0x00000000u+f->r[6]+f->r[0]*1u);
    /* 00441413 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00441415 */
    if(!mi_signed_less(f->flags))goto mi_0044147f;
    /* 00441417 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0044141a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044141b */
    f->r[3]=MI_U32((0x0000000cu+f->r[4]));
    /* 0044141f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441420 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00441423 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00441424 */
    f->r[7]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 00441427 */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 0044142d */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00441430 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00441432 */
    f->r[5]=0x00775614u;
    /* 00441437 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0044143b */
    f->r[6]=(0x00000000u+f->r[6]+f->r[2]*4u);
mi_0044143e:;
    /* 0044143e */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00441440 */
    f->r[0]=f->r[3];
    /* 00441442 */
    if(!mi_signed_less(f->flags))goto mi_00441468;
    /* 00441444 */
    f->r[2]=f->r[5];
mi_00441446:;
    /* 00441446 */
    f->r[3]=(0x00000000u+f->r[6]+f->r[0]*1u);
    /* 00441449 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044144b */
    a=MI_U8((0x00af598cu+f->r[3]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 00441453 */
    if((f->flags&MI_ZF))goto mi_0044145a;
    /* 00441455 */
    f->r[1]=0x00000001u;
mi_0044145a:;
    /* 0044145a */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0044145c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044145d */
    a=f->r[2];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00441460 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00441462 */
    if(mi_signed_less(f->flags))goto mi_00441446;
    /* 00441464 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]));
mi_00441468:;
    /* 00441468 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 0044146c */
    a=f->r[6];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00441472 */
    a=f->r[5];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00441475 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00441476 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0044147a */
    if(!(f->flags&MI_ZF))goto mi_0044143e;
    /* 0044147c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044147d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0044147e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0044147f:;
    /* 0044147f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00441480 */
    f->r[4]+=4;return;
}
