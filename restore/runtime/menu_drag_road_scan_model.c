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

void restore_C_0043f250(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0043f250 */
    f->r[2]=MI_U32((0x00000008u+f->r[4]));
    /* 0043f254 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0043f257 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 0043f25a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f25b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f25c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f25d */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*2u);
    /* 0043f260 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f261 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0043f264 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0043f269 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[1]*4u);
    /* 0043f26c */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 0043f270 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0043f272 */
    a=MI_U8((0x00af598cu+f->r[3]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0043f27a */
    if((f->flags&MI_ZF))goto mi_0043f394;
    /* 0043f280 */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 0043f284 */
    f->r[6]=f->r[2];
    /* 0043f286 */
    f->r[7]=MI_U32((0x00000044u+f->r[4]));
    /* 0043f28a */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 0043f28c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0043f290 */
    f->r[6]=MI_U32((0x0000003cu+f->r[4]));
    /* 0043f294 */
    f->r[5]=f->r[6];
    /* 0043f296 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0043f298 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 0043f29a */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0043f29c */
    f->r[1]=(0xffffffffu+f->r[1]+f->r[7]*1u);
    /* 0043f2a0 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043f2a4 */
    f->r[5]=(0xffffffffu+f->r[7]+f->r[5]*1u);
    /* 0043f2a8 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043f2ac */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043f2b0 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0043f2b2 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0043f2b4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0043f2b6 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043f2ba */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0043f2be */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0043f2c2:;
    /* 0043f2c2 */
    f->r[0]=MI_U32((0x005d2914u+f->r[1]*4u));
    /* 0043f2c9 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[3]*1u);
    /* 0043f2cc */
    f->r[2]=MI_U32((0x00af598cu+f->r[2]*4u));
    /* 0043f2d3 */
    a=f->r[2];b=0x00000040u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0043f2d6 */
    MI_U32((0x00000020u+f->r[4]+f->r[1]*4u))=(uint32_t)(f->r[2]);
    /* 0043f2da */
    f->r[2]=MI_U32((0x00000010u+f->r[4]+f->r[1]*4u));
    /* 0043f2de */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043f2e1 */
    if(mi_signed_less(f->flags))goto mi_0043f311;
    /* 0043f2e3 */
    f->r[7]=(0x00000000u+f->r[0]*4u);
    /* 0043f2ea */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*2u);
    /* 0043f2ed */
    f->r[6]=(0xffffffffu+f->r[2]);
    /* 0043f2f0 */
    f->r[0]=(0x00af598cu+f->r[0]*4u);
mi_0043f2f7:;
    /* 0043f2f7 */
    f->r[5]=MI_U32((0x00000000u+f->r[0]));
    /* 0043f2f9 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043f2fb */
    a=f->r[5];b=0x00000040u;t=mi_alu(f,a,b,32,3);f->r[5]=t;
    /* 0043f2fe */
    a=MI_U32((0x00000020u+f->r[4]+f->r[1]*4u));b=f->r[5];t=mi_alu(f,a,b,32,3);MI_U32((0x00000020u+f->r[4]+f->r[1]*4u))=(uint32_t)(t);
    /* 0043f302 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043f303 */
    if(!(f->flags&MI_ZF))goto mi_0043f2f7;
    /* 0043f305 */
    f->r[7]=MI_U32((0x00000044u+f->r[4]));
    /* 0043f309 */
    f->r[6]=MI_U32((0x00000034u+f->r[4]));
    /* 0043f30d */
    f->r[5]=MI_U32((0x00000040u+f->r[4]));
mi_0043f311:;
    /* 0043f311 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]+f->r[1]*4u));
    /* 0043f315 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043f317 */
    if((f->flags&MI_ZF))goto mi_0043f329;
    /* 0043f319 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043f31a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0043f31c */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0043f320 */
    if((f->flags&MI_ZF))goto mi_0043f32d;
    /* 0043f322 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0043f323 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043f327 */
    goto mi_0043f32d;
mi_0043f329:;
    /* 0043f329 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
mi_0043f32d:;
    /* 0043f32d */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0043f32e */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043f331 */
    if(mi_signed_less(f->flags))goto mi_0043f2c2;
    /* 0043f333 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0043f336 */
    if(!(f->flags&MI_ZF))goto mi_0043f35c;
    /* 0043f338 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043f33b */
    if(mi_signed_less(f->flags))goto mi_0043f35c;
    /* 0043f33d */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0043f33f */
    if(mi_signed_less(f->flags))goto mi_0043f35c;
    /* 0043f341 */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 0043f345 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]+f->r[1]*4u));
    /* 0043f349 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043f34b */
    if((f->flags&MI_ZF))goto mi_0043f352;
    /* 0043f34d */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0043f350 */
    if(!(f->flags&MI_ZF))goto mi_0043f35f;
mi_0043f352:;
    /* 0043f352 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043f353 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043f354 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043f355 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0043f357 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043f358 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f35b */
    f->r[4]+=4;return;
mi_0043f35c:;
    /* 0043f35c */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
mi_0043f35f:;
    /* 0043f35f */
    if(mi_signed_less(f->flags))goto mi_0043f370;
    /* 0043f361 */
    a=f->r[5];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043f364 */
    if(mi_signed_less(f->flags))goto mi_0043f370;
    /* 0043f366 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043f367 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043f368 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043f369 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0043f36b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043f36c */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f36f */
    f->r[4]+=4;return;
mi_0043f370:;
    /* 0043f370 */
    a=f->r[7];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0043f373 */
    if((f->flags&MI_ZF))goto mi_0043f394;
    /* 0043f375 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0043f378 */
    if(!(f->flags&MI_ZF))goto mi_0043f394;
    /* 0043f37a */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0043f37c */
    if(!(f->flags&MI_ZF))goto mi_0043f394;
    /* 0043f37e */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 0043f382 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]+f->r[2]*4u));
    /* 0043f386 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0043f388 */
    if(!(f->flags&MI_ZF))goto mi_0043f394;
    /* 0043f38a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043f38b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043f38c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043f38d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 0043f38f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043f390 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f393 */
    f->r[4]+=4;return;
mi_0043f394:;
    /* 0043f394 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043f395 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043f396 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043f397 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0043f399 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043f39a */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f39d */
    f->r[4]+=4;return;
}
