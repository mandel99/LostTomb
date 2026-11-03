#include "storage_delivery.h"
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "placement_support.h"
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
static __inline uint32_t mi_alu(struct restore_placement_support_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_placement_support_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_placement_support_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


void restore_placement_support_C_00457ac0(struct restore_placement_support_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00457ac0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c8eu));
    /* 00457ac7 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00784c8cu));
    /* 00457ace */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00457acf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00457ad0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00457ad5u;f->boundary(0x00457ad0u,0x0051ce90u,f);
    /* 00457ad5 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 00457ad9 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 00457add */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00457adf */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00457ae0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00457ae1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00457ae6u;f->boundary(0x00457ae1u,0x00481420u,f);
    /* 00457ae6 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00457ae9 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 00457aeb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 00457aed */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 00457aef */
    f->r[4]+=4;return;
}

void restore_placement_support_C_00459fa0(struct restore_placement_support_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00459fa0 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 00459fa5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459fa6 */
    f->r[6]=0x00000001u;
    /* 00459fab */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459fac */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00459fae */
    if(mi_signed_less(f->flags))goto mi_00459fd7;
    /* 00459fb0 */
    f->r[7]=0x00799ae0u;
mi_00459fb5:;
    /* 00459fb5 */
    a=MI_U8((0x00000000u+f->r[7]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00459fb8 */
    if(!(f->flags&MI_ZF))goto mi_00459fc7;
    /* 00459fba */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459fbb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459fc0u;f->boundary(0x00459fbbu,0x00459ef0u,f);
    /* 00459fc0 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00459fc3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00459fc5 */
    if(!(f->flags&MI_ZF))goto mi_00459fdc;
mi_00459fc7:;
    /* 00459fc7 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 00459fcc */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00459fcd */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00459fd3 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00459fd5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459fb5;
mi_00459fd7:;
    /* 00459fd7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00459fd8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00459fda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00459fdb */
    f->r[4]+=4;return;
mi_00459fdc:;
    /* 00459fdc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00459fdd */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00459fdf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00459fe0 */
    f->r[4]+=4;return;
}

void restore_placement_support_C_0045b4d0(struct restore_placement_support_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_withdraw_city((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_placement_support_C_0047f530(struct restore_placement_support_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047f530 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f531 */
    f->r[0]=MI_U32((0x00bc96fcu));
    /* 0047f536 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f537 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f538 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0047f53a */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0047f53c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f53d */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0047f541 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047f54e;
    /* 0047f543 */
    MI_U32((0x00bc96f8u))=(uint32_t)(f->r[6]);
    /* 0047f549 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047f54a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047f54b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047f54c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0047f54d */
    f->r[4]+=4;return;
mi_0047f54e:;
    /* 0047f54e */
    f->r[7]=MI_U32((0x00bc96f8u));
    /* 0047f554 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047f557 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047f564;
    /* 0047f559 */
    MI_U32((0x00bc96f8u))=(uint32_t)(f->r[6]);
    /* 0047f55f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047f560 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047f561 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047f562 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0047f563 */
    f->r[4]+=4;return;
mi_0047f564:;
    /* 0047f564 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047f568 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047f56b */
    if(!(f->flags&MI_ZF))goto mi_0047f57d;
    /* 0047f56d */
    a=f->r[7];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0047f570 */
    if(!mi_signed_less(f->flags))goto mi_0047f57d;
    /* 0047f572 */
    MI_U32((0x00bc96f8u))=(uint32_t)(f->r[6]);
    /* 0047f578 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047f579 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047f57a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047f57b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0047f57c */
    f->r[4]+=4;return;
mi_0047f57d:;
    /* 0047f57d */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0047f581 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0047f584 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 0047f587 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0047f58b */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0047f58e */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0047f593 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0047f596 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 0047f59b */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047f59d */
    f->r[1]=MI_U32((0x00bc96f4u));
    /* 0047f5a3 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047f5a5 */
    MI_U32((0x00bc96f4u))=(uint32_t)(f->r[1]);
    /* 0047f5ab */
    if(!(f->flags&MI_SF))goto mi_0047f5b6;
    /* 0047f5ad */
    a=f->r[1];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047f5b0 */
    MI_U32((0x00bc96f4u))=(uint32_t)(f->r[1]);
mi_0047f5b6:;
    /* 0047f5b6 */
    a=f->r[5];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047f5b9 */
    if(!(f->flags&MI_ZF))goto mi_0047f633;
    /* 0047f5bb */
    a=f->r[7];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0047f5be */
    if(!(f->flags&MI_ZF))goto mi_0047f5ca;
    /* 0047f5c0 */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000004u);
    /* 0047f5c8 */
    goto mi_0047f633;
mi_0047f5ca:;
    /* 0047f5ca */
    a=f->r[7];b=0x0000000au;t=mi_alu(f,a,b,32,1);
    /* 0047f5cd */
    if(!(f->flags&MI_ZF))goto mi_0047f5d9;
    /* 0047f5cf */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000004u);
    /* 0047f5d7 */
    goto mi_0047f633;
mi_0047f5d9:;
    /* 0047f5d9 */
    a=f->r[7];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 0047f5dc */
    if(!(f->flags&MI_ZF))goto mi_0047f5e8;
    /* 0047f5de */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000005u);
    /* 0047f5e6 */
    goto mi_0047f633;
mi_0047f5e8:;
    /* 0047f5e8 */
    a=f->r[7];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 0047f5eb */
    if(!(f->flags&MI_ZF))goto mi_0047f5f7;
    /* 0047f5ed */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000005u);
    /* 0047f5f5 */
    goto mi_0047f633;
mi_0047f5f7:;
    /* 0047f5f7 */
    a=f->r[7];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 0047f5fa */
    if(!(f->flags&MI_ZF))goto mi_0047f606;
    /* 0047f5fc */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 0047f604 */
    goto mi_0047f633;
mi_0047f606:;
    /* 0047f606 */
    a=f->r[7];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 0047f609 */
    if(!(f->flags&MI_ZF))goto mi_0047f615;
    /* 0047f60b */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 0047f613 */
    goto mi_0047f633;
mi_0047f615:;
    /* 0047f615 */
    a=f->r[7];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0047f618 */
    if(!(f->flags&MI_ZF))goto mi_0047f624;
    /* 0047f61a */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(0x00000007u);
    /* 0047f622 */
    goto mi_0047f633;
mi_0047f624:;
    /* 0047f624 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0047f626 */
    a=f->r[7];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0047f629 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047f62c */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f62f */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_0047f633:;
    /* 0047f633 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0047f635 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047f7ee;
    /* 0047f63b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047f63c */
    f->r[3]=MI_U32((0x00bc9700u));
    /* 0047f642 */
    f->r[0]=(0x00adc36cu+f->r[2]);
    /* 0047f648 */
    f->r[2]=(0x00af598cu+f->r[2]*4u);
    /* 0047f64f */
    goto mi_0047f655;
mi_0047f651:;
    /* 0047f651 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
mi_0047f655:;
    /* 0047f655 */
    a=MI_U32((0x00000000u+f->r[2]));b=0x00000040u;t=mi_alu(f,a,b,32,4);MI_U32((0x00000000u+f->r[2]))=(uint32_t)(t);
    /* 0047f658 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0047f65a */
    if(!(f->flags&MI_ZF))goto mi_0047f70e;
    /* 0047f660 */
    f->r[5]=(0xffffffffu+f->r[7]);
    /* 0047f663 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047f665 */
    if(!(f->flags&MI_ZF))goto mi_0047f69d;
    /* 0047f667 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047f66a */
    if(!(f->flags&MI_ZF))goto mi_0047f673;
    /* 0047f66c */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047f66e */
    goto mi_0047f7d9;
mi_0047f673:;
    /* 0047f673 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047f676 */
    if(!(f->flags&MI_ZF))goto mi_0047f680;
    /* 0047f678 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 0047f67b */
    goto mi_0047f7d9;
mi_0047f680:;
    /* 0047f680 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047f682 */
    if(!(f->flags&MI_ZF))goto mi_0047f68c;
    /* 0047f684 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000006u);
    /* 0047f687 */
    goto mi_0047f7d9;
mi_0047f68c:;
    /* 0047f68c */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047f68f */
    if(!(f->flags&MI_ZF))goto mi_0047f7d9;
    /* 0047f695 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000003u);
    /* 0047f698 */
    goto mi_0047f7d9;
mi_0047f69d:;
    /* 0047f69d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047f69f */
    if(!(f->flags&MI_ZF))goto mi_0047f6d8;
    /* 0047f6a1 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047f6a4 */
    if(!(f->flags&MI_ZF))goto mi_0047f6ae;
    /* 0047f6a6 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000006u);
    /* 0047f6a9 */
    goto mi_0047f7d9;
mi_0047f6ae:;
    /* 0047f6ae */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047f6b1 */
    if(!(f->flags&MI_ZF))goto mi_0047f6bb;
    /* 0047f6b3 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000003u);
    /* 0047f6b6 */
    goto mi_0047f7d9;
mi_0047f6bb:;
    /* 0047f6bb */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047f6bd */
    if(!(f->flags&MI_ZF))goto mi_0047f6c7;
    /* 0047f6bf */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000004u);
    /* 0047f6c2 */
    goto mi_0047f7d9;
mi_0047f6c7:;
    /* 0047f6c7 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047f6ca */
    if(!(f->flags&MI_ZF))goto mi_0047f7d9;
    /* 0047f6d0 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000001u);
    /* 0047f6d3 */
    goto mi_0047f7d9;
mi_0047f6d8:;
    /* 0047f6d8 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047f6db */
    if(!(f->flags&MI_ZF))goto mi_0047f6e5;
    /* 0047f6dd */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000005u);
    /* 0047f6e0 */
    goto mi_0047f7d9;
mi_0047f6e5:;
    /* 0047f6e5 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047f6e8 */
    if(!(f->flags&MI_ZF))goto mi_0047f6f2;
    /* 0047f6ea */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 0047f6ed */
    goto mi_0047f7d9;
mi_0047f6f2:;
    /* 0047f6f2 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047f6f4 */
    if(!(f->flags&MI_ZF))goto mi_0047f6fe;
    /* 0047f6f6 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000005u);
    /* 0047f6f9 */
    goto mi_0047f7d9;
mi_0047f6fe:;
    /* 0047f6fe */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047f701 */
    if(!(f->flags&MI_ZF))goto mi_0047f7d9;
    /* 0047f707 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047f709 */
    goto mi_0047f7d9;
mi_0047f70e:;
    /* 0047f70e */
    f->r[5]=(0xfffffffeu+f->r[7]);
    /* 0047f711 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047f713 */
    if(!(f->flags&MI_ZF))goto mi_0047f71d;
    /* 0047f715 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000du);
    /* 0047f718 */
    goto mi_0047f7d9;
mi_0047f71d:;
    /* 0047f71d */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047f720 */
    if(!(f->flags&MI_ZF))goto mi_0047f72a;
    /* 0047f722 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000du);
    /* 0047f725 */
    goto mi_0047f7d9;
mi_0047f72a:;
    /* 0047f72a */
    f->r[5]=(0xffffffffu+f->r[7]);
    /* 0047f72d */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0047f72f */
    if(!(f->flags&MI_ZF))goto mi_0047f761;
    /* 0047f731 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047f734 */
    if(!(f->flags&MI_ZF))goto mi_0047f73e;
    /* 0047f736 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000007u);
    /* 0047f739 */
    goto mi_0047f7d9;
mi_0047f73e:;
    /* 0047f73e */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047f741 */
    if(!(f->flags&MI_ZF))goto mi_0047f74b;
    /* 0047f743 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0047f746 */
    goto mi_0047f7d9;
mi_0047f74b:;
    /* 0047f74b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047f74d */
    if(!(f->flags&MI_ZF))goto mi_0047f757;
    /* 0047f74f */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 0047f752 */
    goto mi_0047f7d9;
mi_0047f757:;
    /* 0047f757 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047f75a */
    if(!(f->flags&MI_ZF))goto mi_0047f7d9;
    /* 0047f75c */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000au);
    /* 0047f75f */
    goto mi_0047f7d9;
mi_0047f761:;
    /* 0047f761 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047f763 */
    if(!(f->flags&MI_ZF))goto mi_0047f78c;
    /* 0047f765 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047f768 */
    if(!(f->flags&MI_ZF))goto mi_0047f76f;
    /* 0047f76a */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000009u);
    /* 0047f76d */
    goto mi_0047f7d9;
mi_0047f76f:;
    /* 0047f76f */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047f772 */
    if(!(f->flags&MI_ZF))goto mi_0047f779;
    /* 0047f774 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000au);
    /* 0047f777 */
    goto mi_0047f7d9;
mi_0047f779:;
    /* 0047f779 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047f77b */
    if(!(f->flags&MI_ZF))goto mi_0047f782;
    /* 0047f77d */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000007u);
    /* 0047f780 */
    goto mi_0047f7d9;
mi_0047f782:;
    /* 0047f782 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047f785 */
    if(!(f->flags&MI_ZF))goto mi_0047f7d9;
    /* 0047f787 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000008u);
    /* 0047f78a */
    goto mi_0047f7d9;
mi_0047f78c:;
    /* 0047f78c */
    a=f->r[6];b=MI_U32((0x00000010u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 0047f790 */
    if(!(f->flags&MI_ZF))goto mi_0047f7b9;
    /* 0047f792 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047f795 */
    if(!(f->flags&MI_ZF))goto mi_0047f79c;
    /* 0047f797 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000eu);
    /* 0047f79a */
    goto mi_0047f7d9;
mi_0047f79c:;
    /* 0047f79c */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047f79f */
    if(!(f->flags&MI_ZF))goto mi_0047f7a6;
    /* 0047f7a1 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000fu);
    /* 0047f7a4 */
    goto mi_0047f7d9;
mi_0047f7a6:;
    /* 0047f7a6 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047f7a8 */
    if(!(f->flags&MI_ZF))goto mi_0047f7af;
    /* 0047f7aa */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000eu);
    /* 0047f7ad */
    goto mi_0047f7d9;
mi_0047f7af:;
    /* 0047f7af */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047f7b2 */
    if(!(f->flags&MI_ZF))goto mi_0047f7d9;
    /* 0047f7b4 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000fu);
    /* 0047f7b7 */
    goto mi_0047f7d9;
mi_0047f7b9:;
    /* 0047f7b9 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047f7bc */
    if(!(f->flags&MI_ZF))goto mi_0047f7c3;
    /* 0047f7be */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 0047f7c1 */
    goto mi_0047f7d9;
mi_0047f7c3:;
    /* 0047f7c3 */
    a=f->r[1];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047f7c6 */
    if((f->flags&MI_ZF))goto mi_0047f7d6;
    /* 0047f7c8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047f7ca */
    if(!(f->flags&MI_ZF))goto mi_0047f7d1;
    /* 0047f7cc */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000bu);
    /* 0047f7cf */
    goto mi_0047f7d9;
mi_0047f7d1:;
    /* 0047f7d1 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047f7d4 */
    if(!(f->flags&MI_ZF))goto mi_0047f7d9;
mi_0047f7d6:;
    /* 0047f7d6 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x0000000cu);
mi_0047f7d9:;
    /* 0047f7d9 */
    f->r[5]=(0x00000000u+f->r[3]*4u);
    /* 0047f7e0 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047f7e2 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047f7e4 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047f7e5 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0047f7e7 */
    if(mi_signed_less(f->flags))goto mi_0047f651;
    /* 0047f7ed */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0047f7ee:;
    /* 0047f7ee */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047f7f3u;f->boundary(0x0047f7eeu,0x005178b0u,f);
    /* 0047f7f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047f7f8u;f->boundary(0x0047f7f3u,0x00517370u,f);
    /* 0047f7f8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047f7fdu;f->boundary(0x0047f7f8u,0x00517660u,f);
    /* 0047f7fd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047f7fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047f7ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047f800 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0047f801 */
    f->r[4]+=4;return;
}

void restore_placement_support_C_004bff60(struct restore_placement_support_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004bff60 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004bff63 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bff64 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bff65 */
    f->r[1]=0x005df818u;
    /* 004bff6a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bff6b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bff6c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004bff70:;
    /* 004bff70 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 004bff74 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004bff77 */
    if((f->flags&MI_ZF))goto mi_004bff93;
    /* 004bff79 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004bff7a */
    if((f->flags&MI_ZF))goto mi_004bff86;
    /* 004bff7c */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004bff80 */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 004bff84 */
    goto mi_004bffa2;
mi_004bff86:;
    /* 004bff86 */
    f->r[7]=MI_U32((0x00000000u+f->r[1]));
    /* 004bff88 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004bff8c */
    f->r[5]=MI_U32((0x00000004u+f->r[1]));
    /* 004bff8f */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004bff91 */
    goto mi_004bff9e;
mi_004bff93:;
    /* 004bff93 */
    f->r[7]=MI_U32((0x00000004u+f->r[1]));
    /* 004bff96 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004bff9a */
    f->r[5]=MI_U32((0x00000000u+f->r[1]));
    /* 004bff9c */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_004bff9e:;
    /* 004bff9e */
    a=f->r[5];b=MI_U32((0x00000024u+f->r[4]));t=mi_alu(f,a,b,32,0);f->r[5]=t;
mi_004bffa2:;
    /* 004bffa2 */
    f->r[0]=MI_U32((0xfffffffcu+f->r[1]));
    /* 004bffa5 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004bffad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bffaf */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004bffb3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004c0016;
mi_004bffb5:;
    /* 004bffb5 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004bffb7 */
    f->r[6]=f->r[5];
    /* 004bffb9 */
    goto mi_004bffbf;
mi_004bffbb:;
    /* 004bffbb */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_004bffbf:;
    /* 004bffbf */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xfffffff8u+f->r[1])))&0xffu)<<0);
    /* 004bffc2 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004bffc4 */
    if((f->flags&MI_ZF))goto mi_004bffde;
    /* 004bffc6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffc8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffca */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffcc */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffce */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffd0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffd1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffd2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bffd7u;f->boundary(0x004bffd2u,0x0047c880u,f);
    /* 004bffd7 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bffda */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bffdc */
    if((f->flags&MI_ZF))goto mi_004bfff6;
mi_004bffde:;
    /* 004bffde */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffe0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffe2 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffe4 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffe6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffe8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffe9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004bffea */
    f->r[4]-=4;MI_U32(f->r[4])=0x004bffefu;f->boundary(0x004bffeau,0x0047c880u,f);
    /* 004bffef */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004bfff2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004bfff4 */
    if((f->flags&MI_ZF))goto mi_004c0033;
mi_004bfff6:;
    /* 004bfff6 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004bfffa */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004bfffb */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004bfffc */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004bfffe */
    if(mi_signed_less(f->flags))goto mi_004bffbb;
    /* 004c0000 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004c0004 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004c0008 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0009 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004c000a */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004c000c */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0010 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0014 */
    if(mi_signed_less(f->flags))goto mi_004bffb5;
mi_004c0016:;
    /* 004c0016 */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c0019 */
    a=f->r[1];b=0x005df858u;t=mi_alu(f,a,b,32,1);
    /* 004c001f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0023 */
    if(mi_signed_less(f->flags))goto mi_004bff70;
    /* 004c0029 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c002a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c002b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c002c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004c002e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c002f */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0032 */
    f->r[4]+=4;return;
mi_004c0033:;
    /* 004c0033 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c0034 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c0035 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c0036 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c0038 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c0039 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c003c */
    f->r[4]+=4;return;
}

void restore_placement_support_C_004c3730(struct restore_placement_support_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c3730 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c3733 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004c3738 */
    f->r[1]=MI_U32((0x00bc9714u));
    /* 004c373e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c373f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3741 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3742 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004c3744 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3745 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3746 */
    f->r[6]=MI_U32((0x00000028u+f->r[4]));
    /* 004c374a */
    f->r[0]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004c374d */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c3750 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c3751 */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 004c3755 */
    f->r[2]=MI_U32((0x005e00ccu+f->r[0]));
    /* 004c375b */
    f->r[0]=MI_U32((0x005e00d0u+f->r[0]));
    /* 004c3761 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c3763 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c3765 */
    f->r[6]=f->r[2];
    /* 004c3767 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c376b */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c376f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004c3777 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c377b */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c377f */
    goto mi_004c3793;
mi_004c3781:;
    /* 004c3781 */
    f->r[1]=MI_U32((0x00bc9714u));
    /* 004c3787 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004c378b */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004c378f */
    f->r[6]=MI_U32((0x0000002cu+f->r[4]));
mi_004c3793:;
    /* 004c3793 */
    f->r[7]=f->r[2];
    /* 004c3795 */
    f->r[3]=f->r[0];
    /* 004c3797 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 004c3799 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 004c37a1 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c37a5 */
    f->r[5]=f->r[0];
    /* 004c37a7 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 004c37a9 */
    goto mi_004c37b9;
mi_004c37ab:;
    /* 004c37ab */
    f->r[1]=MI_U32((0x00bc9714u));
    /* 004c37b1 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004c37b5 */
    f->r[6]=MI_U32((0x0000002cu+f->r[4]));
mi_004c37b9:;
    /* 004c37b9 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 004c37be */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37c1 */
    if((f->flags&MI_ZF))goto mi_004c381a;
    /* 004c37c3 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37c6 */
    if((f->flags&MI_ZF))goto mi_004c3800;
    /* 004c37c8 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37cb */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*1u);
    /* 004c37ce */
    if((f->flags&MI_ZF))goto mi_004c37eb;
    /* 004c37d0 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37d3 */
    if((f->flags&MI_ZF))goto mi_004c37fa;
    /* 004c37d5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37d8 */
    if((f->flags&MI_ZF))goto mi_004c3812;
    /* 004c37da */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37dd */
    if(!(f->flags&MI_ZF))goto mi_004c3837;
mi_004c37df:;
    /* 004c37df */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 004c37e3 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[7]*1u);
    /* 004c37e6 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[2]*1u);
    /* 004c37e9 */
    goto mi_004c383d;
mi_004c37eb:;
    /* 004c37eb */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37ee */
    if((f->flags&MI_ZF))goto mi_004c3812;
    /* 004c37f0 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37f3 */
    if((f->flags&MI_ZF))goto mi_004c382c;
    /* 004c37f5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c37f8 */
    if((f->flags&MI_ZF))goto mi_004c3837;
mi_004c37fa:;
    /* 004c37fa */
    f->r[1]=f->r[6];
    /* 004c37fc */
    f->r[0]=f->r[5];
    /* 004c37fe */
    goto mi_004c383d;
mi_004c3800:;
    /* 004c3800 */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*1u);
    /* 004c3803 */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3806 */
    if((f->flags&MI_ZF))goto mi_004c37df;
    /* 004c3808 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c380b */
    if((f->flags&MI_ZF))goto mi_004c3837;
    /* 004c380d */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3810 */
    if((f->flags&MI_ZF))goto mi_004c37fa;
mi_004c3812:;
    /* 004c3812 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[5]*1u);
    /* 004c3815 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[3]*1u);
    /* 004c3818 */
    goto mi_004c383d;
mi_004c381a:;
    /* 004c381a */
    f->r[0]=(0x00000000u+f->r[1]+f->r[1]*1u);
    /* 004c381d */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3820 */
    if((f->flags&MI_ZF))goto mi_004c3837;
    /* 004c3822 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c3825 */
    if((f->flags&MI_ZF))goto mi_004c37fa;
    /* 004c3827 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c382a */
    if((f->flags&MI_ZF))goto mi_004c3812;
mi_004c382c:;
    /* 004c382c */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004c3830 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[2]*1u);
    /* 004c3833 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004c3835 */
    goto mi_004c383d;
mi_004c3837:;
    /* 004c3837 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 004c383b */
    f->r[1]=f->r[2];
mi_004c383d:;
    /* 004c383d */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 004c3843 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004c3846 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004c3849 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004c384c */
    f->r[2]=(0x00000000u+f->r[6]+f->r[2]*4u);
    /* 004c384f */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c3851 */
    f->r[6]=MI_U32((0x00af598cu+f->r[2]*4u));
    /* 004c3858 */
    a=f->r[6];b=0xeeffd77fu;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 004c385e */
    a=MI_U16((0x00b350dcu+f->r[2]*2u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004c3867 */
    if(!(f->flags&MI_ZF))goto mi_004c38d7;
    /* 004c3869 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c386a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c386b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c386c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c386d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c3872u;f->boundary(0x004c386du,0x00470900u,f);
    /* 004c3872 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c3875 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004c3877 */
    if(!(f->flags&MI_ZF))goto mi_004c38ca;
    /* 004c3879 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004c387b */
    if(!(f->flags&MI_ZF))goto mi_004c38d7;
    /* 004c387d */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004c3881 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004c3885 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c3886 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004c3887 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004c3888 */
    a=f->r[0];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 004c388b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c388f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c3893 */
    if(mi_signed_less(f->flags))goto mi_004c37ab;
    /* 004c3899 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004c389d */
    f->r[6]=MI_U32((0x0000002cu+f->r[4]));
    /* 004c38a1 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004c38a5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c38a6 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c38a7 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004c38a8 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,1);
    /* 004c38ab */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c38af */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c38b3 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c38b7 */
    if(mi_signed_less(f->flags))goto mi_004c3781;
    /* 004c38bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c38be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c38bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c38c0 */
    f->r[0]=0x00000001u;
    /* 004c38c5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c38c6 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c38c9 */
    f->r[4]+=4;return;
mi_004c38ca:;
    /* 004c38ca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c38cb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c38cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c38cd */
    f->r[0]=0xfffffffeu;
    /* 004c38d2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c38d3 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c38d6 */
    f->r[4]+=4;return;
mi_004c38d7:;
    /* 004c38d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c38d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c38d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c38da */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 004c38dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c38de */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c38e1 */
    f->r[4]+=4;return;
}

void restore_placement_support_C_00481420(struct restore_placement_support_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00481420 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00481424 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00481425 */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 00481429 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0048142a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0048142d */
    f->r[0]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00481430 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00481433 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 00481438 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 0048143b */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0048143f */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00481441 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00481443 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00481446 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00481449 */
    a=f->r[0];b=0x005cc9e0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_0048144e:;
    /* 0048144e */
    f->r[1]=MI_U32((0x00000000u+f->r[0]));
    /* 00481450 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00481452 */
    if((f->flags&MI_ZF))goto mi_00481475;
    /* 00481454 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00481456 */
    a=f->r[1];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00481459 */
    a=MI_U8((0x00af598cu+f->r[1]));b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 00481460 */
    if((f->flags&MI_ZF))goto mi_0048146c;
    /* 00481462 */
    f->r[7]=MI_U32((0x00eed760u+f->r[1]));
    /* 00481468 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0048146a */
    if(!(f->flags&MI_ZF))goto mi_0048147a;
mi_0048146c:;
    /* 0048146c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0048146d */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00481470 */
    a=f->r[6];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 00481473 */
    if(mi_signed_less(f->flags))goto mi_0048144e;
mi_00481475:;
    /* 00481475 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00481476 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00481478 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00481479 */
    f->r[4]+=4;return;
mi_0048147a:;
    /* 0048147a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0048147b */
    f->r[0]=0x00000001u;
    /* 00481480 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00481481 */
    f->r[4]+=4;return;
}

static const uint8_t mi_rom_00459f24[]={0x1a,0x9f,0x45,0x00,0x20,0x9f,0x45,0x00};
static const uint8_t mi_rom_00459f2c[]={0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x00,0x00,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00459f24u&&width<=8u&&(offset=address-0x00459f24u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00459f24[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00459f2cu&&width<=106u&&(offset=address-0x00459f2cu)<=106u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00459f2c[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


void restore_placement_support_C_00459ef0(struct restore_placement_support_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00459ef0 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00459ef4 */
    f->r[1]=f->r[0];
    /* 00459ef6 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00459ef9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00459efb */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read((0x007999e8u+f->r[1]*8u),2u);
    /* 00459f03 */
    a=f->r[0];b=0xffffffacu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00459f06 */
    a=f->r[0];b=0x00000069u;t=mi_alu(f,a,b,32,1);
    /* 00459f09 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00459f20;
    /* 00459f0b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00459f0d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x00459f2cu+f->r[0]),1u))&0xffu)<<0);
    /* 00459f13 */
    switch(mi_read((0x00459f24u+f->r[2]*4u),4u)){case 0x00459f1au:goto mi_00459f1a;case 0x00459f20u:goto mi_00459f20;default:abort();}
mi_00459f1a:;
    /* 00459f1a */
    f->r[0]=0x00000001u;
    /* 00459f1f */
    f->r[4]+=4;return;
mi_00459f20:;
    /* 00459f20 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00459f22 */
    f->r[4]+=4;return;
}

void restore_placement_support_C_0045b570(struct restore_placement_support_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_withdraw((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_placement_support_C_0045b490(struct restore_placement_support_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_pile_frame((int32_t)MI_U32(sp+4u));f->r[4]=sp+4u;
}
