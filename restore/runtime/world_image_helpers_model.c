/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_image_helpers_model.h"
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

void restore_building_world_004381b0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004381b0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004381b4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004381b5 */
    f->r[6]=MI_U32((0x0126517cu));
    /* 004381bb */
    f->r[0]=f->r[1];
    /* 004381bd */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004381bf */
    if(mi_signed_less(f->flags))goto mi_004381c3;
    /* 004381c1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004381c3:;
    /* 004381c3 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004381c7 */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004381ca */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[0]));
    /* 004381d1 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004381d3 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004381d5 */
    f->r[0]=f->r[1];
    /* 004381d7 */
    if(mi_signed_less(f->flags))goto mi_004381db;
    /* 004381d9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004381db:;
    /* 004381db */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 004381df */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004381e2 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[0]));
    /* 004381e9 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004381eb */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004381ef */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004381f1 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 004381f5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004381f7 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004381f8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004381f9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004381fa */
    f->r[1]=0x00f55bb0u;
    /* 004381ff */
    f->r[4]-=4;MI_U32(f->r[4])=0x00438204u;f->boundary(0x004381ffu,0x0053e590u,f);
    /* 00438204 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00438205 */
    f->r[4]+=4;return;
}

void restore_building_world_00438210(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00438210 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00438214 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00438217 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00438219 */
    if((f->flags&MI_ZF))goto mi_004382f6;
    /* 0043821f */
    f->r[1]=f->r[0];
    /* 00438221 */
    a=f->r[0];b=0x00000fffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00438226 */
    a=f->r[1];b=0x0000f000u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0043822c */
    f->r[2]=f->r[0];
    /* 0043822e */
    a=f->r[1];b=0x00001000u;t=mi_alu(f,a,b,32,1);
    /* 00438234 */
    if(!(f->flags&MI_ZF))goto mi_00438304;
    /* 0043823a */
    a=f->r[2];b=0x00000019u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043823d */
    if((f->flags&MI_ZF))goto mi_004382fa;
    /* 00438243 */
    a=f->r[2];b=0x00000034u;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00438246 */
    if((f->flags&MI_ZF))goto mi_00438342;
    /* 0043824c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_0043824d:;
    /* 0043824d */
    f->r[1]=0x0000026eu;
mi_00438252:;
    /* 00438252 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00438256 */
    f->r[2]=MI_U32((0x00f56c0cu));
    /* 0043825c */
    f->r[1]=MI_U32((0x00f55bb4u+f->r[1]*8u));
    /* 00438263 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438264 */
    f->r[6]=0x00000055u;
    /* 00438269 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043826b */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0043826f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00438273 */
    f->r[6]=MI_U32((0x0000002cu+f->r[4]));
    /* 00438277 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438278 */
    f->r[7]=MI_U32((0x0126517cu));
    /* 0043827e */
    f->r[2]=0x00000014u;
    /* 00438283 */
    f->r[1]=(0xffffffffu+f->r[1]+f->r[6]*1u);
    /* 00438287 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(0x0000006eu);
    /* 0043828f */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00438293 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00438297 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043829b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000064u);
    /* 004382a3 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x0000000au);
    /* 004382ab */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]+f->r[0]*8u));
    /* 004382af */
    f->r[0]=MI_U32((0x00000008u+f->r[4]+f->r[0]*8u));
    /* 004382b3 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004382b5 */
    f->r[6]=f->r[1];
    /* 004382b7 */
    if(mi_signed_less(f->flags))goto mi_004382bb;
    /* 004382b9 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004382bb:;
    /* 004382bb */
    a=f->r[6];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004382be */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[6]));
    /* 004382c5 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004382c7 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004382c9 */
    f->r[6]=f->r[1];
    /* 004382cb */
    if(mi_signed_less(f->flags))goto mi_004382cf;
    /* 004382cd */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_004382cf:;
    /* 004382cf */
    f->r[7]=MI_U32((0x00000038u+f->r[4]));
    /* 004382d3 */
    a=f->r[6];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004382d6 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[6]));
    /* 004382dd */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004382df */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004382e1 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004382e2 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 004382e6 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004382e8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004382e9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004382ea */
    f->r[1]=0x00f55bb0u;
    /* 004382ef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004382f4u;f->boundary(0x004382efu,0x0053e590u,f);
    /* 004382f4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004382f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_004382f6:;
    /* 004382f6 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004382f9 */
    f->r[4]+=4;return;
mi_004382fa:;
    /* 004382fa */
    f->r[1]=0x00000277u;
    /* 004382ff */
    goto mi_00438252;
mi_00438304:;
    /* 00438304 */
    a=f->r[1];b=0x00002000u;t=mi_alu(f,a,b,32,1);
    /* 0043830a */
    if(!(f->flags&MI_ZF))goto mi_0043824d;
    /* 00438310 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00438313 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00438316 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00438318 */
    a=f->r[0];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043831b */
    a=MI_U8((0x00bcd606u+f->r[0]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00438322 */
    if(!(f->flags&MI_ZF))goto mi_004382f6;
    /* 00438324 */
    a=MI_U8((0x00bcd630u+f->r[0]));b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0043832b */
    if((f->flags&MI_ZF))goto mi_004382f6;
    /* 0043832d */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0043832f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[0])))&0xffu)<<0);
    /* 00438335 */
    f->r[0]=f->r[1];
    /* 00438337 */
    a=f->r[0];b=0x0000004du;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043833a */
    if((f->flags&MI_ZF))goto mi_00438342;
    /* 0043833c */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043833d */
    goto mi_0043824d;
mi_00438342:;
    /* 00438342 */
    f->r[1]=0x00000275u;
    /* 00438347 */
    goto mi_00438252;
}

void restore_building_world_00438350(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00438350 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00438353 */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 00438357 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438358 */
    f->r[6]=MI_U32((0x00f56bccu));
    /* 0043835e */
    f->r[1]=0x00000008u;
    /* 00438363 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00438365 */
    f->r[2]=0x00000030u;
    /* 0043836a */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043836e */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00438372 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00438376 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043837a */
    f->r[2]=MI_U32((0x00000058u+f->r[4]));
    /* 0043837e */
    f->r[1]=(0xffffffffu+f->r[0]);
    /* 00438381 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438382 */
    f->r[6]=0x00000012u;
    /* 00438387 */
    f->r[7]=0x0000003eu;
    /* 0043838c */
    a=f->r[1];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0043838f */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00438393 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00438397 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00438399 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(0x0000004bu);
    /* 004383a1 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x0000000au);
    /* 004383a9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000004eu);
    /* 004383b1 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000007u);
    /* 004383b9 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004383bd */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x0000004bu);
    /* 004383c5 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(0x0000000au);
    /* 004383cd */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x0000004eu);
    /* 004383d5 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(0x00000007u);
    /* 004383dd */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004383e1 */
    f->r[6]=0x00000278u;
    /* 004383e6 */
    if((f->flags&MI_ZF))goto mi_00438402;
    /* 004383e8 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004383eb */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004383ee */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004383f0 */
    a=MI_U8((0x00bcd630u+f->r[7]*4u));b=0x0000000cu;t=mi_alu(f,a,b,8,1);
    /* 004383f8 */
    if(!(f->flags&MI_ZF))goto mi_00438402;
    /* 004383fa */
    f->r[6]=0x0000026fu;
    /* 004383ff */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_00438402:;
    /* 00438402 */
    f->r[6]=MI_U32((0x00f55bb4u+f->r[6]*8u));
    /* 00438409 */
    f->r[7]=MI_U32((0x00000050u+f->r[4]));
    /* 0043840d */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]+f->r[0]*8u));
    /* 00438411 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]+f->r[0]*8u));
    /* 00438415 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[7]*4u);
    /* 00438418 */
    f->r[7]=MI_U32((0x0126517cu));
    /* 0043841e */
    f->r[1]=(0xfffffffcu+f->r[6]+f->r[1]*1u);
    /* 00438422 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00438424 */
    f->r[6]=f->r[1];
    /* 00438426 */
    if(mi_signed_less(f->flags))goto mi_0043842a;
    /* 00438428 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0043842a:;
    /* 0043842a */
    a=f->r[6];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0043842d */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[6]));
    /* 00438434 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00438436 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00438438 */
    f->r[6]=f->r[1];
    /* 0043843a */
    if(mi_signed_less(f->flags))goto mi_0043843e;
    /* 0043843c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_0043843e:;
    /* 0043843e */
    f->r[7]=MI_U32((0x00000058u+f->r[4]));
    /* 00438442 */
    a=f->r[6];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00438445 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[6]));
    /* 0043844c */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043844e */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00438450 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438451 */
    f->r[2]=MI_U32((0x00000058u+f->r[4]));
    /* 00438455 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00438457 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438458 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438459 */
    f->r[1]=0x00f55bb0u;
    /* 0043845e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00438463u;f->boundary(0x0043845eu,0x0053e590u,f);
    /* 00438463 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00438464 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00438465 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00438468 */
    f->r[4]+=4;return;
}

void restore_building_world_00438470(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00438470 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00438474 */
    f->r[1]=MI_U32((0x00f56f3cu));
    /* 0043847a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043847b */
    f->r[6]=MI_U32((0x0126517cu));
    /* 00438481 */
    f->r[0]=(0xffffffffu+f->r[1]+f->r[0]*1u);
    /* 00438485 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438487 */
    f->r[1]=f->r[0];
    /* 00438489 */
    if(mi_signed_less(f->flags))goto mi_0043848d;
    /* 0043848b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043848d:;
    /* 0043848d */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00438490 */
    f->r[2]=0x00000036u;
    /* 00438495 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 0043849c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043849e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004384a0 */
    f->r[1]=f->r[0];
    /* 004384a2 */
    if(mi_signed_less(f->flags))goto mi_004384a6;
    /* 004384a4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004384a6:;
    /* 004384a6 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004384aa */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004384ad */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 004384b4 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004384b6 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004384ba */
    a=f->r[6];b=0x0000000fu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004384bd */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004384bf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004384c0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004384c1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004384c2 */
    f->r[1]=0x00f55bb0u;
    /* 004384c7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004384ccu;f->boundary(0x004384c7u,0x0053e590u,f);
    /* 004384cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004384cd */
    f->r[4]+=4;return;
}

void restore_building_world_004384d0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004384d0 */
    f->r[0]=MI_U32((0x00f56f44u));
    /* 004384d5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004384d6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004384d7 */
    f->r[6]=MI_U32((0x0126517cu));
    /* 004384dd */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004384de */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004384e2 */
    f->r[1]=(0xffffffffu+f->r[0]+f->r[7]*1u);
    /* 004384e6 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004384e8 */
    f->r[0]=f->r[1];
    /* 004384ea */
    if(mi_signed_less(f->flags))goto mi_004384ee;
    /* 004384ec */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_004384ee:;
    /* 004384ee */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004384f1 */
    f->r[2]=0x0000005au;
    /* 004384f6 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[0]));
    /* 004384fd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004384ff */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438501 */
    f->r[0]=f->r[1];
    /* 00438503 */
    if(mi_signed_less(f->flags))goto mi_00438507;
    /* 00438505 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
mi_00438507:;
    /* 00438507 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 0043850b */
    a=f->r[0];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0043850e */
    f->r[3]=f->r[6];
    /* 00438510 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[0]));
    /* 00438517 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 00438519 */
    a=f->r[3];b=0x00000011u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0043851c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043851d */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 00438521 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00438523 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438524 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438525 */
    f->r[1]=0x00f55bb0u;
    /* 0043852a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043852fu;f->boundary(0x0043852au,0x0053e590u,f);
    /* 0043852f */
    f->r[1]=MI_U32((0x00f56f4cu));
    /* 00438535 */
    f->r[0]=(0xffffffffu+f->r[1]+f->r[7]*1u);
    /* 00438539 */
    f->r[7]=MI_U32((0x0126517cu));
    /* 0043853f */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00438541 */
    f->r[1]=f->r[0];
    /* 00438543 */
    if(mi_signed_less(f->flags))goto mi_00438547;
    /* 00438545 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00438547:;
    /* 00438547 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043854a */
    f->r[2]=0x00000028u;
    /* 0043854f */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 00438556 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00438558 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0043855a */
    f->r[1]=f->r[0];
    /* 0043855c */
    if(mi_signed_less(f->flags))goto mi_00438560;
    /* 0043855e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00438560:;
    /* 00438560 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00438563 */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00438565 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 0043856c */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043856e */
    f->r[1]=0x00f55bb0u;
    /* 00438573 */
    a=f->r[6];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00438576 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438577 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438578 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438579 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043857eu;f->boundary(0x00438579u,0x0053e590u,f);
    /* 0043857e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043857f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00438580 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00438581 */
    f->r[4]+=4;return;
}

void restore_building_world_00438590(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00438590 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00438594 */
    f->r[1]=MI_U32((0x00f566fcu));
    /* 0043859a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0043859c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043859d */
    if((f->flags&MI_ZF))goto mi_004385e6;
    /* 0043859f */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 004385a3 */
    f->r[2]=MI_U32((0x00f56e0cu));
    /* 004385a9 */
    f->r[6]=MI_U32((0x0126517cu));
    /* 004385af */
    f->r[0]=(0xffffffffu+f->r[2]+f->r[1]*1u);
    /* 004385b3 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004385b5 */
    f->r[1]=f->r[0];
    /* 004385b7 */
    if(mi_signed_less(f->flags))goto mi_004385bb;
    /* 004385b9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004385bb:;
    /* 004385bb */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004385be */
    f->r[2]=0x00000014u;
    /* 004385c3 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 004385ca */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004385cc */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004385ce */
    f->r[1]=f->r[0];
    /* 004385d0 */
    if(mi_signed_less(f->flags))goto mi_004385d4;
    /* 004385d2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004385d4:;
    /* 004385d4 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004385d8 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004385db */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 004385e2 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004385e4 */
    goto mi_0043862d;
mi_004385e6:;
    /* 004385e6 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004385ea */
    f->r[0]=MI_U32((0x00f56e04u));
    /* 004385ef */
    f->r[6]=MI_U32((0x0126517cu));
    /* 004385f5 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[2]*1u);
    /* 004385f9 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004385fb */
    f->r[1]=f->r[0];
    /* 004385fd */
    if(mi_signed_less(f->flags))goto mi_00438601;
    /* 004385ff */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00438601:;
    /* 00438601 */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00438604 */
    f->r[2]=0x00000034u;
    /* 00438609 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 00438610 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00438612 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438614 */
    f->r[1]=f->r[0];
    /* 00438616 */
    if(mi_signed_less(f->flags))goto mi_0043861a;
    /* 00438618 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043861a:;
    /* 0043861a */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0043861e */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00438621 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 00438628 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043862a */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);f->r[6]=t;
mi_0043862d:;
    /* 0043862d */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00438631 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438632 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00438634 */
    f->r[1]=0x00f55bb0u;
    /* 00438639 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043863a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043863b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00438640u;f->boundary(0x0043863bu,0x0053e590u,f);
    /* 00438640 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00438641 */
    f->r[4]+=4;return;
}

void restore_building_world_00438650(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00438650 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00438654 */
    f->r[1]=MI_U32((0x00f56e0cu));
    /* 0043865a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043865b */
    f->r[6]=MI_U32((0x0126517cu));
    /* 00438661 */
    f->r[0]=(0xffffffffu+f->r[1]+f->r[0]*1u);
    /* 00438665 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438667 */
    f->r[1]=f->r[0];
    /* 00438669 */
    if(mi_signed_less(f->flags))goto mi_0043866d;
    /* 0043866b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043866d:;
    /* 0043866d */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00438670 */
    f->r[2]=0x00000052u;
    /* 00438675 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 0043867c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043867e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438680 */
    f->r[1]=f->r[0];
    /* 00438682 */
    if(mi_signed_less(f->flags))goto mi_00438686;
    /* 00438684 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00438686:;
    /* 00438686 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0043868a */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043868d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 00438694 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00438696 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0043869a */
    a=f->r[6];b=0x0000000eu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0043869d */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043869f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004386a0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004386a1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004386a2 */
    f->r[1]=0x00f55bb0u;
    /* 004386a7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004386acu;f->boundary(0x004386a7u,0x0053e590u,f);
    /* 004386ac */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004386ad */
    f->r[4]+=4;return;
}

void restore_building_world_004386b0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004386b0 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 004386b4 */
    f->r[1]=MI_U32((0x00f56df4u));
    /* 004386ba */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004386bb */
    f->r[6]=MI_U32((0x0126517cu));
    /* 004386c1 */
    f->r[0]=(0xffffffffu+f->r[1]+f->r[0]*1u);
    /* 004386c5 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004386c7 */
    f->r[1]=f->r[0];
    /* 004386c9 */
    if(mi_signed_less(f->flags))goto mi_004386cd;
    /* 004386cb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004386cd:;
    /* 004386cd */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004386d0 */
    f->r[2]=0x0000001du;
    /* 004386d5 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 004386dc */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004386de */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004386e0 */
    f->r[1]=f->r[0];
    /* 004386e2 */
    if(mi_signed_less(f->flags))goto mi_004386e6;
    /* 004386e4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004386e6:;
    /* 004386e6 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004386ea */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004386ed */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 004386f4 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004386f6 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004386fa */
    a=f->r[6];b=0x0000000eu;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004386fd */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004386ff */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438700 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438701 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00438702 */
    f->r[1]=0x00f55bb0u;
    /* 00438707 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043870cu;f->boundary(0x00438707u,0x0053e590u,f);
    /* 0043870c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043870d */
    f->r[4]+=4;return;
}

void restore_building_world_00438710(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00438710 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00438714 */
    f->r[1]=MI_U32((0x00f56decu));
    /* 0043871a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043871b */
    f->r[6]=MI_U32((0x0126517cu));
    /* 00438721 */
    f->r[0]=(0xffffffffu+f->r[1]+f->r[0]*1u);
    /* 00438725 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438727 */
    f->r[1]=f->r[0];
    /* 00438729 */
    if(mi_signed_less(f->flags))goto mi_0043872d;
    /* 0043872b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043872d:;
    /* 0043872d */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00438730 */
    f->r[2]=0x00000040u;
    /* 00438735 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 0043873c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043873e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438740 */
    f->r[1]=f->r[0];
    /* 00438742 */
    if(mi_signed_less(f->flags))goto mi_00438746;
    /* 00438744 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00438746:;
    /* 00438746 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0043874a */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0043874d */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 00438754 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00438756 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0043875a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0043875c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043875d */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043875e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043875f */
    f->r[1]=0x00f55bb0u;
    /* 00438764 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00438769u;f->boundary(0x00438764u,0x0053e590u,f);
    /* 00438769 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043876a */
    f->r[4]+=4;return;
}

void restore_building_world_00438770(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00438770 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00438774 */
    f->r[1]=MI_U32((0x00f56decu));
    /* 0043877a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043877b */
    f->r[6]=MI_U32((0x0126517cu));
    /* 00438781 */
    f->r[0]=(0xffffffffu+f->r[1]+f->r[0]*1u);
    /* 00438785 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00438787 */
    f->r[1]=f->r[0];
    /* 00438789 */
    if(mi_signed_less(f->flags))goto mi_0043878d;
    /* 0043878b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0043878d:;
    /* 0043878d */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00438790 */
    f->r[2]=0x0000006cu;
    /* 00438795 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1au+f->r[1]));
    /* 0043879c */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 0043879e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004387a0 */
    f->r[1]=f->r[0];
    /* 004387a2 */
    if(mi_signed_less(f->flags))goto mi_004387a6;
    /* 004387a4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004387a6:;
    /* 004387a6 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 004387aa */
    a=f->r[1];b=0x00000006u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004387ad */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x010d6a1cu+f->r[1]));
    /* 004387b4 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004387b6 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004387ba */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004387bb */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004387bd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004387be */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004387bf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004387c0 */
    f->r[1]=0x00f55bb0u;
    /* 004387c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004387cau;f->boundary(0x004387c5u,0x0053e590u,f);
    /* 004387ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004387cb */
    f->r[4]+=4;return;
}
