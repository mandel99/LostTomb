/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "road_check_model.h"

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

void restore_C_0043f410(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0043f410 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0043f413 */
    f->r[0]=0x00000001u;
    /* 0043f418 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f419 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f41a */
    f->r[5]=MI_U32((0x00bca308u));
    /* 0043f420 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043f424 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043f428 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 0043f42c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f42d */
    f->r[6]=MI_U32((0x00bca304u));
    /* 0043f433 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0043f435 */
    a=f->r[0];b=0x0000001fu;t=mi_alu(f,a,b,32,1);
    /* 0043f438 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043f43c */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0043f440 */
    f->r[3]=0x00000005u;
    /* 0043f445 */
    if(!(f->flags&MI_ZF))goto mi_0043f44e;
    /* 0043f447 */
    f->r[3]=0x00000002u;
    /* 0043f44c */
    goto mi_0043f472;
mi_0043f44e:;
    /* 0043f44e */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 0043f451 */
    if(!(f->flags&MI_ZF))goto mi_0043f45a;
    /* 0043f453 */
    f->r[3]=0x00000003u;
    /* 0043f458 */
    goto mi_0043f472;
mi_0043f45a:;
    /* 0043f45a */
    a=f->r[0];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 0043f45d */
    if(!(f->flags&MI_ZF))goto mi_0043f466;
    /* 0043f45f */
    f->r[3]=0x00000004u;
    /* 0043f464 */
    goto mi_0043f472;
mi_0043f466:;
    /* 0043f466 */
    a=f->r[0];b=0x000000d1u;t=mi_alu(f,a,b,32,1);
    /* 0043f46b */
    if(!(f->flags&MI_ZF))goto mi_0043f472;
    /* 0043f46d */
    f->r[3]=0x00000005u;
mi_0043f472:;
    /* 0043f472 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[5]*1u);
    /* 0043f475 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0043f479 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043f47b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043f47f */
    if(!mi_signed_less(f->flags))goto mi_0043f598;
    /* 0043f485 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 0043f489 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f48a */
    f->r[0]=(0x00000000u+f->r[5]+f->r[0]*2u);
    /* 0043f48e */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0043f491 */
    f->r[7]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 0043f494 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
mi_0043f498:;
    /* 0043f498 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[6]*1u);
    /* 0043f49b */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043f49f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043f4a1 */
    if(!mi_signed_less(f->flags))goto mi_0043f54c;
mi_0043f4a7:;
    /* 0043f4a7 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0043f4ab */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 0043f4af */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4b0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4b1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4b2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043f4b7u;f->boundary(0x0043f4b2u,0x0043f3a0u,f);
    /* 0043f4b7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f4ba */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043f4bc */
    if((f->flags&MI_ZF))goto mi_0043f4df;
    /* 0043f4be */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0043f4c2 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 0043f4c6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4c7 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4c8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4c9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4ca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0043f4cb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0043f4d0u;f->boundary(0x0043f4cbu,0x0043f250u,f);
    /* 0043f4d0 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f4d3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0043f4d5 */
    if((f->flags&MI_ZF))goto mi_0043f4df;
    /* 0043f4d7 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(0x00000001u);
mi_0043f4df:;
    /* 0043f4df */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0043f4e4 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0043f4e6 */
    f->r[1]=MI_U32((0x00d05720u+f->r[0]*4u));
    /* 0043f4ed */
    f->r[2]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0043f4f4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0043f4f6 */
    if((f->flags&MI_ZF))goto mi_0043f506;
    /* 0043f4f8 */
    a=MI_U8((0x00000018u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,8,1);
    /* 0043f4fc */
    if(!(f->flags&MI_ZF))goto mi_0043f506;
    /* 0043f4fe */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
mi_0043f506:;
    /* 0043f506 */
    a=f->r[2];b=0xeeffd77fu;t=mi_alu(f,a,b,32,3);
    /* 0043f50c */
    if((f->flags&MI_ZF))goto mi_0043f51b;
    /* 0043f50e */
    a=(f->r[2]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0043f511 */
    if(!(f->flags&MI_ZF))goto mi_0043f51b;
    /* 0043f513 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
mi_0043f51b:;
    /* 0043f51b */
    a=MI_U16((0x00b4e6fcu+f->r[0]*2u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0043f524 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0043f52e;
    /* 0043f526 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(0x00000000u);
mi_0043f52e:;
    /* 0043f52e */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0043f532 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 0043f536 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0043f537 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0043f538 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[1]*1u);
    /* 0043f53b */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0043f53c */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043f53e */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0043f542 */
    if(mi_signed_less(f->flags))goto mi_0043f4a7;
    /* 0043f548 */
    f->r[6]=f->r[1];
    /* 0043f54a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0043f54c:;
    /* 0043f54c */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0043f550 */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 0043f554 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0043f558 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0043f559 */
    a=f->r[7];b=0x000000e4u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0043f55f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0043f560 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0043f564 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[5]*1u);
    /* 0043f567 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0043f569 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0043f56d */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0043f571 */
    if(mi_signed_less(f->flags))goto mi_0043f498;
    /* 0043f577 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0043f57b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0043f57c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043f57e */
    if((f->flags&MI_ZF))goto mi_0043f598;
    /* 0043f580 */
    a=MI_U32((0x00000018u+f->r[4]));b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043f584 */
    if((f->flags&MI_ZF))goto mi_0043f598;
    /* 0043f586 */
    a=MI_U32((0x00000024u+f->r[4]));b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0043f58a */
    if((f->flags&MI_ZF))goto mi_0043f598;
    /* 0043f58c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043f58d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043f58e */
    f->r[0]=0x00000001u;
    /* 0043f593 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043f594 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f597 */
    f->r[4]+=4;return;
mi_0043f598:;
    /* 0043f598 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0043f599 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0043f59a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0043f59c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0043f59d */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0043f5a0 */
    f->r[4]+=4;return;
}
