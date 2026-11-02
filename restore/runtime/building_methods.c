#include <stdlib.h>
/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "building_methods.h"

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


void restore_building_method_C_004015a0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004015a0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004015a3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000018u+f->r[4])))&0xffu)<<0);
    /* 004015a7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004015a8 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004015a9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004015aa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004015ab */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
    /* 004015af */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004015b1 */
    f->r[5]=f->r[1];
    /* 004015b3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004015b7 */
    if(!(f->flags&MI_ZF))goto mi_004015c2;
    /* 004015b9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004015ba */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x004015bfu;f->boundary(0x004015bau,p,f);
    /* 004015bf */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004015c2:;
    /* 004015c2 */
    f->r[1]=MI_U32((0x0000004cu+f->r[5]));
    /* 004015c5 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004015c9 */
    f->r[0]=MI_U32((0x00000050u+f->r[5]));
    /* 004015cc */
    f->r[6]=f->r[7];
    /* 004015ce */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004015d1 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004015d3 */
    f->r[3]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 004015d6 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004015d9 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004015dd */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004015e1 */
    MI_U16((0x007999deu+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004015e8 */
    f->r[3]=MI_U32((0x00000020u+f->r[4]));
    /* 004015ec */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004015ee */
    MI_U16((0x007999e0u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004015f5 */
    f->r[3]=MI_U32((0x00000020u+f->r[4]));
    /* 004015f9 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004015fb */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004015fe */
    f->r[0]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 00401601 */
    f->r[3]=MI_U32((0x00bca2d8u));
    /* 00401607 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040160a */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 0040160d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0040160f */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00401611 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00401613 */
    MI_U32((0x007999e4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00401619 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00401620 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000054u+f->r[5])))&0xffu)<<0);
    /* 00401623 */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00401627 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0040162d */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00401634 */
    MI_U32((0x00799a2cu+f->r[6]))=(uint32_t)(f->r[3]);
    /* 0040163a */
    MI_U32((0x00799a34u+f->r[6]))=(uint32_t)(f->r[3]);
    /* 00401640 */
    f->r[1]=(0x00000028u+f->r[4]);
    /* 00401644 */
    MI_U32((0x00799a30u+f->r[6]))=(uint32_t)(f->r[3]);
    /* 0040164a */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 0040164d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040164e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040164f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401650 */
    f->r[1]=f->r[5];
    /* 00401652 */
    p=MI_U32((0x00000020u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00401655u;f->boundary(0x00401652u,p,f);
    /* 00401655 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 0040165a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[4])))&0xffffu)<<0);
    /* 0040165f */
    MI_U16((0x007999feu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00401666 */
    MI_U16((0x00799a00u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0040166d */
    f->r[6]=(0x00799a39u+f->r[6]);
    /* 00401673 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00401675 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401676 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00401678 */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0040167b */
    MI_U32((0x00000008u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0040167e */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 00401681 */
    f->r[1]=f->r[5];
    /* 00401683 */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00401686u;f->boundary(0x00401683u,p,f);
    /* 00401686 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401687 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x0040168cu;f->boundary(0x00401687u,p,f);
    /* 0040168c */
    MI_U32((0x00000008u+f->r[5]))=(uint32_t)(f->r[3]);
    /* 0040168f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00401692 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0040169a */
    f->r[3]=(0x00000084u+f->r[5]);
mi_004016a0:;
    /* 004016a0 */
    f->r[0]=MI_U32((0xfffffffcu+f->r[3]));
    /* 004016a3 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004016a7 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004016ab */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 004016ad */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004016ae */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004016b2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004016b3 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004016b5 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004016bb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004016bd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004016be */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004016c2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004016c4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004016c5 */
    a=0x000000e6u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004016ca */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004016cb */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x004016d0u;f->boundary(0x004016cbu,p,f);
    /* 004016d0 */
    f->r[7]=f->r[0];
    /* 004016d2 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004016d6 */
    f->r[6]=f->r[7];
    /* 004016d8 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 004016dd */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004016e0 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004016e2 */
    f->r[1]=f->r[0];
    /* 004016e4 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004016e7 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004016ea */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004016ec */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004016f3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004016f5 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004016f8 */
    MI_U16((0x00799a12u+f->r[1]*8u))=(uint16_t)((f->r[7]&0xffffu));
    /* 00401700 */
    MI_U16((0x00799a12u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00401707 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0040170e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[3])))&0xffu)<<0);
    /* 00401711 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 00401717 */
    MI_U32((0x00799a2cu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0040171d */
    MI_U32((0x00799a34u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00401723 */
    MI_U32((0x00799a30u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00401729 */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 0040172c */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 00401730 */
    f->r[1]=(0x00000028u+f->r[4]);
    /* 00401734 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401735 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401736 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401737 */
    f->r[1]=f->r[5];
    /* 00401739 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0040173d */
    p=MI_U32((0x00000020u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00401740u;f->boundary(0x0040173du,p,f);
    /* 00401740 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 00401745 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[4])))&0xffffu)<<0);
    /* 0040174a */
    MI_U16((0x007999feu+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00401751 */
    MI_U16((0x00799a00u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00401758 */
    f->r[6]=(0x00799a39u+f->r[6]);
    /* 0040175e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00401760 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401761 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00401763 */
    MI_U32((0x00000004u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00401766 */
    MI_U32((0x00000008u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00401769 */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 0040176c */
    f->r[1]=f->r[5];
    /* 0040176e */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00401771u;f->boundary(0x0040176eu,p,f);
    /* 00401771 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00401772 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x00401777u;f->boundary(0x00401772u,p,f);
    /* 00401777 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0040177b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040177e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040177f */
    a=f->r[3];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00401782 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 00401785 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00401789 */
    if(mi_signed_less(f->flags))goto mi_004016a0;
    /* 0040178f */
    p=0x004dea50u;f->r[4]-=4;MI_U32(f->r[4])=0x00401794u;f->boundary(0x0040178fu,p,f);
    /* 00401794 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00401795 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00401796 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00401797 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00401798 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040179b */
    f->r[4]+=20;return;
}

void restore_building_method_C_004017a0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004017a0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017a1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017a2 */
    f->r[6]=f->r[1];
    /* 004017a4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017a5 */
    f->r[7]=MI_U32((0x00000010u+f->r[4]));
    /* 004017a9 */
    f->r[1]=(0x00000010u+f->r[4]);
    /* 004017ad */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 004017af */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017b0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017b1 */
    f->r[1]=f->r[6];
    /* 004017b3 */
    p=MI_U32((0x0000001cu+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x004017b6u;f->boundary(0x004017b3u,p,f);
    /* 004017b6 */
    f->r[1]=f->r[7];
    /* 004017b8 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017ba */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004017bd */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004017bf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017c0 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004017c3 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]));
    /* 004017ca */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 004017d1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 004017d8 */
    f->r[3]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004017db */
    f->r[2]=(0x00000000u+f->r[2]+f->r[3]*4u);
    /* 004017de */
    f->r[6]=MI_U32((0x00000054u+f->r[6]+f->r[2]*4u));
    /* 004017e2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017e3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017e4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017e5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017e6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004017e7 */
    p=0x0047bf50u;f->r[4]-=4;MI_U32(f->r[4])=0x004017ecu;f->boundary(0x004017e7u,p,f);
    /* 004017ec */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004017ef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004017f0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004017f1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004017f2 */
    f->r[4]+=8;return;
}

void restore_building_method_C_0040da90(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040da90 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0040da93 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000001cu+f->r[4])))&0xffu)<<0);
    /* 0040da97 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040da98 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040da99 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040da9a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040da9b */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 0040da9f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040daa1 */
    f->r[5]=f->r[1];
    /* 0040daa3 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0040daa7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0040daaf */
    if(!(f->flags&MI_ZF))goto mi_0040dabe;
    /* 0040dab1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dab2 */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x0040dab7u;f->boundary(0x0040dab2u,p,f);
    /* 0040dab7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040daba */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0040dabe:;
    /* 0040dabe */
    f->r[1]=MI_U32((0x0000004cu+f->r[5]));
    /* 0040dac1 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0040dac5 */
    f->r[0]=MI_U32((0x00000050u+f->r[5]));
    /* 0040dac8 */
    f->r[6]=f->r[7];
    /* 0040daca */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040dacd */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040dacf */
    f->r[3]=(0x00000000u+f->r[2]+f->r[1]*1u);
    /* 0040dad2 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040dad5 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040dad9 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0040dadd */
    MI_U16((0x007999deu+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0040dae4 */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 0040dae8 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0040daea */
    MI_U16((0x007999e0u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0040daf1 */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 0040daf5 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040daf7 */
    f->r[3]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 0040dafa */
    f->r[0]=(0x00000000u+f->r[0]+f->r[3]*2u);
    /* 0040dafd */
    f->r[3]=MI_U32((0x00bca2d8u));
    /* 0040db03 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0040db06 */
    f->r[0]=(0x00000000u+f->r[3]+f->r[0]*4u);
    /* 0040db09 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0040db0b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040db0d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040db0f */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0040db13 */
    MI_U32((0x007999e4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0040db19 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0040db20 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000054u+f->r[5])))&0xffu)<<0);
    /* 0040db23 */
    f->r[0]=0x00000001u;
    /* 0040db28 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 0040db2e */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0040db35 */
    MI_U32((0x00799a2cu+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0040db3b */
    MI_U32((0x00799a30u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0040db41 */
    f->r[1]=(0x00000028u+f->r[4]);
    /* 0040db45 */
    MI_U32((0x00799a38u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0040db4b */
    f->r[2]=(0x0000002cu+f->r[4]);
    /* 0040db4f */
    MI_U32((0x00799a34u+f->r[6]))=(uint32_t)(f->r[3]);
    /* 0040db55 */
    f->r[0]=MI_U32((0x00000000u+f->r[5]));
    /* 0040db58 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040db59 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040db5a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040db5b */
    f->r[1]=f->r[5];
    /* 0040db5d */
    p=MI_U32((0x00000020u+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x0040db60u;f->boundary(0x0040db5du,p,f);
    /* 0040db60 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000002cu+f->r[4])))&0xffffu)<<0);
    /* 0040db65 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 0040db6a */
    MI_U16((0x007999feu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0040db71 */
    MI_U16((0x00799a00u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0040db78 */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 0040db7b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040db7c */
    f->r[1]=f->r[5];
    /* 0040db7e */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x0040db81u;f->boundary(0x0040db7eu,p,f);
    /* 0040db81 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040db82 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x0040db87u;f->boundary(0x0040db82u,p,f);
    /* 0040db87 */
    MI_U32((0x00000008u+f->r[5]))=(uint32_t)(f->r[3]);
    /* 0040db8a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040db8d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0040db95 */
    f->r[3]=(0x00000084u+f->r[5]);
mi_0040db9b:;
    /* 0040db9b */
    f->r[0]=MI_U32((0xfffffffcu+f->r[3]));
    /* 0040db9e */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0040dba2 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040dba6 */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 0040dba8 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0040dba9 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0040dbad */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dbae */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040dbb0 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0040dbb6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dbb8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dbb9 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 0040dbbd */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040dbbf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dbc0 */
    a=0x000000e1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dbc5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dbc6 */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x0040dbcbu;f->boundary(0x0040dbc6u,p,f);
    /* 0040dbcb */
    f->r[7]=f->r[0];
    /* 0040dbcd */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 0040dbd1 */
    f->r[6]=f->r[7];
    /* 0040dbd3 */
    f->r[1]=f->r[0];
    /* 0040dbd5 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040dbd8 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0040dbda */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x0000002cu+f->r[4])))&0xffffu)<<0);
    /* 0040dbdf */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040dbe2 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 0040dbe5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040dbe7 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040dbea */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0040dbf1 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0040dbf5 */
    MI_U16((0x00799a12u+f->r[1]*8u))=(uint16_t)((f->r[7]&0xffffu));
    /* 0040dbfd */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0040dbff */
    MI_U16((0x00799a12u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0040dc06 */
    MI_U16((0x007999eau+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0040dc0d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[3])))&0xffu)<<0);
    /* 0040dc10 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0040dc14 */
    MI_U8((0x007999dbu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
    /* 0040dc1a */
    f->r[0]=0x00000001u;
    /* 0040dc1f */
    MI_U32((0x00799a2cu+f->r[6]))=(uint32_t)(f->r[2]);
    /* 0040dc25 */
    MI_U32((0x00799a30u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0040dc2b */
    MI_U32((0x00799a38u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 0040dc31 */
    MI_U32((0x00799a34u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0040dc37 */
    f->r[0]=MI_U32((0x00000000u+f->r[5]));
    /* 0040dc3a */
    f->r[1]=(0x00000028u+f->r[4]);
    /* 0040dc3e */
    f->r[2]=(0x0000002cu+f->r[4]);
    /* 0040dc42 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dc43 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dc44 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dc45 */
    f->r[1]=f->r[5];
    /* 0040dc47 */
    p=MI_U32((0x00000020u+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x0040dc4au;f->boundary(0x0040dc47u,p,f);
    /* 0040dc4a */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000002cu+f->r[4])))&0xffffu)<<0);
    /* 0040dc4f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 0040dc54 */
    MI_U16((0x007999feu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0040dc5b */
    MI_U16((0x00799a00u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0040dc62 */
    f->r[2]=MI_U32((0x00000000u+f->r[5]));
    /* 0040dc65 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dc66 */
    f->r[1]=f->r[5];
    /* 0040dc68 */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x0040dc6bu;f->boundary(0x0040dc68u,p,f);
    /* 0040dc6b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dc6c */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x0040dc71u;f->boundary(0x0040dc6cu,p,f);
    /* 0040dc71 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0040dc75 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040dc78 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040dc79 */
    a=f->r[3];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0040dc7c */
    a=f->r[0];b=0x00000019u;t=mi_alu(f,a,b,32,1);
    /* 0040dc7f */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040dc83 */
    if(mi_signed_less(f->flags))goto mi_0040db9b;
    /* 0040dc89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040dc8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040dc8b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040dc8c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040dc8d */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040dc90 */
    f->r[4]+=20;return;
}

void restore_building_method_C_0040dca0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040dca0 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0040dca3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dca4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dca5 */
    f->r[5]=MI_U32((0x00000028u+f->r[4]));
    /* 0040dca9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dcaa */
    f->r[6]=f->r[1];
    /* 0040dcac */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dcad */
    f->r[1]=(0x00000014u+f->r[4]);
    /* 0040dcb1 */
    f->r[0]=MI_U32((0x00000000u+f->r[6]));
    /* 0040dcb3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dcb4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040dcb5 */
    f->r[1]=f->r[6];
    /* 0040dcb7 */
    p=MI_U32((0x0000001cu+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x0040dcbau;f->boundary(0x0040dcb7u,p,f);
    /* 0040dcba */
    f->r[1]=f->r[5];
    /* 0040dcbc */
    f->r[3]=f->r[5];
    /* 0040dcbe */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040dcc1 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040dcc3 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040dcc6 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]));
    /* 0040dccd */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0040dcd1 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[3]=t;
    /* 0040dcd3 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0040dcd6 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0040dcda */
    f->r[7]=(0x00000000u+f->r[2]+f->r[7]*4u);
    /* 0040dcdd */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000014u+f->r[4])))&0xffu)<<0);
    /* 0040dce1 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0040dce4 */
    f->r[6]=MI_U32((0x00000054u+f->r[6]+f->r[7]*4u));
    /* 0040dce8 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0040dcec */
    if((f->flags&MI_ZF))goto mi_0040de53;
    /* 0040dcf2 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0040dcf9 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0040dd00 */
    f->r[7]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 0040dd03 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[7]*2u);
    /* 0040dd06 */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 0040dd0c */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0040dd0e */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 0040dd11 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0040dd13 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[2]*4u);
    /* 0040dd16 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 0040dd1c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040de71;
    /* 0040dd22 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x005cc8c0u);
    /* 0040dd2a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0040dd2e */
    goto mi_0040dd38;
mi_0040dd30:;
    /* 0040dd30 */
    f->r[3]=MI_U32((0x0000001cu+f->r[4]));
    /* 0040dd34 */
    f->r[5]=MI_U32((0x00000030u+f->r[4]));
mi_0040dd38:;
    /* 0040dd38 */
    f->r[6]=MI_U32((0x00000014u+f->r[4]));
    /* 0040dd3c */
    f->r[1]=f->r[3];
    /* 0040dd3e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0040dd41 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040dd43 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 0040dd47 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 0040dd49 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
    /* 0040dd4c */
    f->r[2]=MI_U32((0x00799a30u+f->r[1]*8u));
    /* 0040dd53 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0040dd57 */
    goto mi_0040dd5d;
mi_0040dd59:;
    /* 0040dd59 */
    f->r[5]=MI_U32((0x00000030u+f->r[4]));
mi_0040dd5d:;
    /* 0040dd5d */
    f->r[1]=MI_U32((0x00000000u+f->r[3]));
    /* 0040dd5f */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 0040dd65 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040dd67 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 0040dd6b */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0040dd6e */
    f->r[2]=(0x00000000u+f->r[1]*4u);
    /* 0040dd75 */
    if(!mi_signed_less(f->flags))goto mi_0040dd80;
    /* 0040dd77 */
    a=MI_U8((0x00af598fu+f->r[2]));b=0x00000010u;t=mi_alu(f,a,b,8,4);MI_U8((0x00af598fu+f->r[2]))=(uint8_t)(t);
    /* 0040dd7e */
    goto mi_0040dd8a;
mi_0040dd80:;
    /* 0040dd80 */
    MI_U32((0x00af598cu+f->r[2]))=(uint32_t)(0x00000008u);
mi_0040dd8a:;
    /* 0040dd8a */
    a=MI_U8((0x00b285ccu+f->r[1]));b=0x000000e0u;t=mi_alu(f,a,b,8,3);MI_U8((0x00b285ccu+f->r[1]))=(uint8_t)(t);
    /* 0040dd91 */
    a=MI_U8((0x00b67d1cu+f->r[1]));b=0x00000040u;t=mi_alu(f,a,b,8,4);MI_U8((0x00b67d1cu+f->r[1]))=(uint8_t)(t);
    /* 0040dd98 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0040dd9a */
    MI_U16((0x00b4e6fcu+f->r[1]*2u))=(uint16_t)((f->r[5]&0xffffu));
    /* 0040dda2 */
    if((f->flags&MI_ZF))goto mi_0040ddfa;
    /* 0040dda4 */
    a=f->r[0];b=MI_U32((0x00f573bcu));t=mi_alu(f,a,b,32,1);
    /* 0040ddaa */
    if(!(f->flags&MI_ZF))goto mi_0040ddfa;
    /* 0040ddac */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 0040ddb0 */
    a=f->r[5];b=0x0000000du;t=mi_alu(f,a,b,32,1);
    /* 0040ddb3 */
    if(!(f->flags&MI_ZF))goto mi_0040ddbb;
    /* 0040ddb5 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0040ddb7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040de1f;
    /* 0040ddb9 */
    goto mi_0040ddc5;
mi_0040ddbb:;
    /* 0040ddbb */
    a=f->r[5];b=0x0000000eu;t=mi_alu(f,a,b,32,1);
    /* 0040ddbe */
    if(!(f->flags&MI_ZF))goto mi_0040ddd1;
    /* 0040ddc0 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040ddc3 */
    if(mi_signed_less(f->flags))goto mi_0040de1f;
mi_0040ddc5:;
    /* 0040ddc5 */
    a=MI_U32((0x00000010u+f->r[4]));b=0x005cc920u;t=mi_alu(f,a,b,32,1);
    /* 0040ddcd */
    if(!mi_signed_less(f->flags))goto mi_0040ddfa;
    /* 0040ddcf */
    goto mi_0040de1f;
mi_0040ddd1:;
    /* 0040ddd1 */
    a=f->r[5];b=0x0000000fu;t=mi_alu(f,a,b,32,1);
    /* 0040ddd4 */
    if(!(f->flags&MI_ZF))goto mi_0040dde6;
    /* 0040ddd6 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0040ddd8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040de1f;
    /* 0040ddda */
    a=MI_U32((0x00000010u+f->r[4]));b=0x005cc8c0u;t=mi_alu(f,a,b,32,1);
    /* 0040dde2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040ddfa;
    /* 0040dde4 */
    goto mi_0040de1f;
mi_0040dde6:;
    /* 0040dde6 */
    a=f->r[5];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0040dde9 */
    if(!(f->flags&MI_ZF))goto mi_0040ddfa;
    /* 0040ddeb */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0040ddee */
    if(mi_signed_less(f->flags))goto mi_0040de1f;
    /* 0040ddf0 */
    a=MI_U32((0x00000010u+f->r[4]));b=0x005cc8c0u;t=mi_alu(f,a,b,32,1);
    /* 0040ddf8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0040de1f;
mi_0040ddfa:;
    /* 0040ddfa */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040ddfc */
    if((f->flags&MI_ZF))goto mi_0040de1f;
    /* 0040ddfe */
    a=f->r[0];b=MI_U32((0x00f56b54u));t=mi_alu(f,a,b,32,1);
    /* 0040de04 */
    if(!(f->flags&MI_ZF))goto mi_0040de19;
    /* 0040de06 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[1])))&0xffu)<<0);
    /* 0040de0c */
    a=f->r[1];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0040de0f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040de11 */
    MI_U32((0x00b7482cu+f->r[2]))=(uint32_t)(f->r[1]);
    /* 0040de17 */
    goto mi_0040de1f;
mi_0040de19:;
    /* 0040de19 */
    MI_U32((0x00b7482cu+f->r[2]))=(uint32_t)(f->r[0]);
mi_0040de1f:;
    /* 0040de1f */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 0040de23 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 0040de24 */
    a=f->r[3];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0040de27 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040de29 */
    if(mi_signed_less(f->flags))goto mi_0040dd59;
    /* 0040de2f */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0040de33 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0040de37 */
    a=f->r[2];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0040de3a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0040de3b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0040de3f */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0040de43 */
    if(!(f->flags&MI_ZF))goto mi_0040dd30;
    /* 0040de49 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040de4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040de4b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040de4c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040de4d */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040de50 */
    f->r[4]+=8;return;
mi_0040de53:;
    /* 0040de53 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 0040de5a */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de5c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de5d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 0040de64 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de65 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de66 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de67 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de68 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040de69 */
    p=0x0047bf50u;f->r[4]-=4;MI_U32(f->r[4])=0x0040de6eu;f->boundary(0x0040de69u,p,f);
    /* 0040de6e */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0040de71:;
    /* 0040de71 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040de72 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040de73 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040de74 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040de75 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040de78 */
    f->r[4]+=8;return;
}

void restore_building_method_C_00417180(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00417180 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000010u+f->r[4])))&0xffu)<<0);
    /* 00417184 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417185 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417186 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417187 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 0041718b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041718c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0041718e */
    f->r[7]=f->r[1];
    /* 00417190 */
    f->r[3]=f->r[6];
    /* 00417192 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000000u);
    /* 0041719a */
    if(!(f->flags&MI_ZF))goto mi_004171a9;
    /* 0041719c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041719d */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x004171a2u;f->boundary(0x0041719du,p,f);
    /* 004171a2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004171a5 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_004171a9:;
    /* 004171a9 */
    f->r[2]=MI_U32((0x0000004cu+f->r[7]));
    /* 004171ac */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 004171b0 */
    f->r[1]=MI_U32((0x00000050u+f->r[7]));
    /* 004171b3 */
    f->r[0]=f->r[6];
    /* 004171b5 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004171b8 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004171ba */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004171bc */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004171bf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004171c0 */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004171c7 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004171cb */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004171cd */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004171d4 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004171d8 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004171da */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004171dd */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 004171e0 */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 004171e6 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004171e9 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 004171ed */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004171ef */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004171f1 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 004171f5 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004171f7 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004171fd */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 00417201 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 00417208 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000054u+f->r[7])))&0xffu)<<0);
    /* 0041720b */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00417211 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 00417218 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0041721e */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 00417224 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 0041722e */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 00417234 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 00417236 */
    f->r[1]=f->r[7];
    /* 00417238 */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x0041723bu;f->boundary(0x00417238u,p,f);
    /* 0041723b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041723c */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x00417241u;f->boundary(0x0041723cu,p,f);
    /* 00417241 */
    MI_U32((0x00000008u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 00417244 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00417247 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0041724f */
    f->r[5]=(0x00000084u+f->r[7]);
mi_00417255:;
    /* 00417255 */
    f->r[1]=MI_U32((0x00000000u+f->r[5]));
    /* 00417258 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 0041725c */
    f->r[0]=MI_U32((0xfffffffcu+f->r[5]));
    /* 0041725f */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00417260 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417261 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00417263 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00417269 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041726b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0041726c */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 00417270 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00417272 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417273 */
    a=0x000000e3u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417278 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00417279 */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x0041727eu;f->boundary(0x00417279u,p,f);
    /* 0041727e */
    f->r[6]=f->r[0];
    /* 00417280 */
    f->r[1]=f->r[3];
    /* 00417282 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00417285 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00417287 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000038u+f->r[4])))&0xffffu)<<0);
    /* 0041728c */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0041728f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00417292 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00417294 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00417297 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0041729e */
    f->r[3]=f->r[6];
    /* 004172a0 */
    MI_U16((0x00799a12u+f->r[1]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 004172a8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004172aa */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004172b1 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004172b8 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[5])))&0xffu)<<0);
    /* 004172bb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004172bc */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004172c2 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004172c6 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004172cc */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004172d2 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 004172dc */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004172e2 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 004172e4 */
    f->r[1]=f->r[7];
    /* 004172e6 */
    p=MI_U32((0x00000018u+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x004172e9u;f->boundary(0x004172e6u,p,f);
    /* 004172e9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004172ea */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x004172efu;f->boundary(0x004172eau,p,f);
    /* 004172ef */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004172f3 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004172f6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004172f7 */
    a=f->r[5];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004172fa */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004172fd */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00417301 */
    if(mi_signed_less(f->flags))goto mi_00417255;
    /* 00417307 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00417308 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00417309 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0041730a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0041730b */
    f->r[4]+=20;return;
}

void restore_building_method_C_004e6840(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e6840 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004e6843 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000018u+f->r[4])))&0xffu)<<0);
    /* 004e6847 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6848 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6849 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e684a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e684b */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
    /* 004e684f */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004e6851 */
    f->r[6]=f->r[1];
    /* 004e6853 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004e6855 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004e6859 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004e685d */
    if(!(f->flags&MI_ZF))goto mi_004e686c;
    /* 004e685f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6860 */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x004e6865u;f->boundary(0x004e6860u,p,f);
    /* 004e6865 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e6868 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004e686c:;
    /* 004e686c */
    f->r[2]=MI_U32((0x0000004cu+f->r[6]));
    /* 004e686f */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 004e6873 */
    f->r[1]=MI_U32((0x00000050u+f->r[6]));
    /* 004e6876 */
    f->r[0]=f->r[7];
    /* 004e6878 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e687b */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e687d */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004e687f */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e6882 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004e6886 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6887 */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 004e688e */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 004e6892 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e6894 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004e6898 */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004e689f */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 004e68a3 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e68a5 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 004e68a8 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 004e68ab */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 004e68b1 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 004e68b4 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 004e68b8 */
    f->r[5]=MI_U32((0x00000020u+f->r[4]));
    /* 004e68bc */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e68be */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e68c0 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e68c6 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004e68ca */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004e68d1 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000054u+f->r[6])))&0xffu)<<0);
    /* 004e68d4 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004e68da */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004e68e1 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e68e7 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e68ed */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(0x00000001u);
    /* 004e68f7 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e68fd */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x004e6902u;f->boundary(0x004e68fdu,p,f);
    /* 004e6902 */
    f->r[2]=MI_U32((0x00000000u+f->r[6]));
    /* 004e6904 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e6907 */
    f->r[1]=f->r[6];
    /* 004e6909 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e690a */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x004e690du;f->boundary(0x004e690au,p,f);
    /* 004e690d */
    f->r[1]=MI_U32((0x00000084u+f->r[6]));
    /* 004e6913 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 004e6917 */
    f->r[0]=MI_U32((0x00000080u+f->r[6]));
    /* 004e691d */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004e691e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e691f */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e6921 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6922 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e6924 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6925 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6926 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004e692b */
    a=0x000000e4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6930 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6931 */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x004e6936u;f->boundary(0x004e6931u,p,f);
    /* 004e6936 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000028u+f->r[4])))&0xffffu)<<0);
    /* 004e693b */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 004e693f */
    f->r[7]=f->r[0];
    /* 004e6941 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e6944 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004e6946 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6947 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004e694a */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004e6951 */
    MI_U16((0x00799a12u+f->r[2]))=(uint16_t)((f->r[7]&0xffffu));
    /* 004e6958 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 004e695f */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000008cu+f->r[6])))&0xffffu)<<0);
    /* 004e6966 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004e696d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000088u+f->r[6])))&0xffu)<<0);
    /* 004e6973 */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 004e6977 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 004e697d */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004e6983 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e6989 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e698f */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 004e6995 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x004e699au;f->boundary(0x004e6995u,p,f);
    /* 004e699a */
    f->r[2]=MI_U32((0x00000000u+f->r[6]));
    /* 004e699c */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e699f */
    f->r[1]=f->r[6];
    /* 004e69a1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e69a2 */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x004e69a5u;f->boundary(0x004e69a2u,p,f);
    /* 004e69a5 */
    MI_U32((0x00000008u+f->r[6]))=(uint32_t)(f->r[3]);
    /* 004e69a8 */
    p=0x004dea50u;f->r[4]-=4;MI_U32(f->r[4])=0x004e69adu;f->boundary(0x004e69a8u,p,f);
    /* 004e69ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e69ae */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e69af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e69b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e69b1 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e69b4 */
    f->r[4]+=20;return;
}

void restore_building_method_C_004e69c0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004e69c0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e69c1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e69c2 */
    f->r[6]=MI_U32((0x0000000cu+f->r[4]));
    /* 004e69c6 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e69c7 */
    f->r[7]=f->r[1];
    /* 004e69c9 */
    f->r[1]=(0x00000010u+f->r[4]);
    /* 004e69cd */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 004e69cf */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e69d0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e69d1 */
    f->r[1]=f->r[7];
    /* 004e69d3 */
    p=MI_U32((0x0000001cu+f->r[0]));f->r[4]-=4;MI_U32(f->r[4])=0x004e69d6u;f->boundary(0x004e69d3u,p,f);
    /* 004e69d6 */
    f->r[1]=f->r[6];
    /* 004e69d8 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e69db */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004e69dd */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004e69e0 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[1]));
    /* 004e69e7 */
    f->r[3]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 004e69ea */
    f->r[2]=(0x00000000u+f->r[2]+f->r[3]*4u);
    /* 004e69ed */
    f->r[3]=MI_U32((0x00000058u+f->r[7]+f->r[2]*4u));
    /* 004e69f1 */
    a=f->r[3];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004e69f4 */
    f->r[2]=(0x00000000u+f->r[7]+f->r[2]*4u);
    /* 004e69f7 */
    f->r[7]=MI_U32((0x00000054u+f->r[2]));
    /* 004e69fa */
    if(!(f->flags&MI_ZF))goto mi_004e6a40;
    /* 004e69fc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 004e6a03 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 004e6a0a */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004e6a0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e6a0e */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004e6a11 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004e6a17 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004e6a19 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004e6a1c */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 004e6a1f */
    f->r[1]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 004e6a26 */
    MI_U16((0x00b4e6fcu+f->r[0]*2u))=(uint16_t)((f->r[6]&0xffffu));
    /* 004e6a2e */
    a=f->r[1];b=0x10000000u;t=mi_alu(f,a,b,32,4);f->r[1]=t;
    /* 004e6a34 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e6a35 */
    MI_U32((0x00af598cu+f->r[0]*4u))=(uint32_t)(f->r[1]);
    /* 004e6a3c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e6a3d */
    f->r[4]+=8;return;
mi_004e6a40:;
    /* 004e6a40 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a42 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a43 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 004e6a4a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 004e6a51 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a52 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a53 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a54 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a55 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e6a56 */
    p=0x0047bf50u;f->r[4]-=4;MI_U32(f->r[4])=0x004e6a5bu;f->boundary(0x004e6a56u,p,f);
    /* 004e6a5b */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e6a5e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e6a5f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e6a60 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e6a61 */
    f->r[4]+=8;return;
}

void restore_building_method_C_005417b0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005417b0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005417b1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005417b2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005417b3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005417b4 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 005417b8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005417b9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005417ba */
    f->r[3]=f->r[6];
    /* 005417bc */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x005417c1u;f->boundary(0x005417bcu,p,f);
    /* 005417c1 */
    f->r[2]=MI_U32((0x00601dd8u));
    /* 005417c7 */
    f->r[5]=MI_U32((0x0000001cu+f->r[4]));
    /* 005417cb */
    f->r[7]=f->r[0];
    /* 005417cd */
    f->r[0]=f->r[6];
    /* 005417cf */
    f->r[1]=MI_U32((0x00601ddcu));
    /* 005417d5 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 005417d7 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005417da */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005417dc */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 005417e0 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005417e3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005417e4 */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 005417eb */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 005417ef */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 005417f1 */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 005417f8 */
    f->r[5]=MI_U32((0x00000024u+f->r[4]));
    /* 005417fc */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005417fe */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00541801 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 00541804 */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 0054180a */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0054180d */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 00541811 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 00541813 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00541815 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00541819 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054181b */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00601de0u)))&0xffu)<<0);
    /* 00541821 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00541827 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0054182e */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00541834 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0054183b */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[7]);
    /* 00541841 */
    f->r[7]=0x00000001u;
    /* 00541846 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 0054184c */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[7]);
    /* 00541852 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 00541858 */
    p=0x00541920u;f->r[4]-=4;MI_U32(f->r[4])=0x0054185du;f->boundary(0x00541858u,p,f);
    /* 0054185d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054185e */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x00541863u;f->boundary(0x0054185eu,p,f);
    /* 00541863 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00541867 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054186a */
    f->r[7]=0x00601e10u;
mi_0054186f:;
    /* 0054186f */
    f->r[1]=MI_U32((0x00000000u+f->r[7]));
    /* 00541871 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00541875 */
    f->r[0]=MI_U32((0xfffffffcu+f->r[7]));
    /* 00541878 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00541879 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054187a */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054187c */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00541882 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541883 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541884 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00541888 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054188a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054188b */
    a=0x000000d5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541890 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541891 */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x00541896u;f->boundary(0x00541891u,p,f);
    /* 00541896 */
    f->r[6]=f->r[0];
    /* 00541898 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000038u+f->r[4])))&0xffffu)<<0);
    /* 0054189d */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005418a0 */
    f->r[1]=f->r[3];
    /* 005418a2 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005418a4 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 005418a7 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005418aa */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005418ac */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005418ad */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 005418b4 */
    f->r[3]=f->r[6];
    /* 005418b6 */
    MI_U16((0x00799a12u+f->r[1]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 005418be */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[7])))&0xffu)<<0);
    /* 005418c1 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 005418c8 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 005418cf */
    f->r[2]=MI_U32((0x0000002cu+f->r[4]));
    /* 005418d3 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 005418d9 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 005418df */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 005418e5 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 005418eb */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 005418f1 */
    p=0x00541920u;f->r[4]-=4;MI_U32(f->r[4])=0x005418f6u;f->boundary(0x005418f1u,p,f);
    /* 005418f6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005418f7 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x005418fcu;f->boundary(0x005418f7u,p,f);
    /* 005418fc */
    f->r[2]=MI_U32((0x00000040u+f->r[4]));
    /* 00541900 */
    a=f->r[4];b=0x00000020u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00541903 */
    a=f->r[7];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00541906 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00541907 */
    a=f->r[7];b=0x0060322cu;t=mi_alu(f,a,b,32,1);
    /* 0054190d */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00541911 */
    if(mi_signed_less(f->flags))goto mi_0054186f;
    /* 00541917 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00541918 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00541919 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054191a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054191b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0054191c */
    f->r[4]+=4;return;
}

void restore_building_method_C_00541920(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00541920 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00541923 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541924 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541925 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541926 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541927 */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
    /* 0054192b */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 0054192f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541930 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541931 */
    p=0x005414b0u;f->r[4]-=4;MI_U32(f->r[4])=0x00541936u;f->boundary(0x00541931u,p,f);
    /* 00541936 */
    f->r[1]=f->r[7];
    /* 00541938 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054193b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0054193e */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00541940 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00541944 */
    a=f->r[1];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00541947 */
    f->r[6]=(f->r[6]&0xffff0000u)|(((MI_U16((0x007999eau+f->r[1])))&0xffffu)<<0);
    /* 0054194e */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[6]&0xffffu);
    /* 00541951 */
    f->r[3]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00541954 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[3]*4u);
    /* 00541957 */
    f->r[5]=MI_U32((0x00601de0u+f->r[2]*4u));
    /* 0054195e */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000018u+f->r[4])))&0xffu)<<0);
    /* 00541962 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00541965 */
    if((f->flags&MI_ZF))goto mi_00541a7d;
    /* 0054196b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 00541972 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 00541979 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[2]*8u);
    /* 0054197c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00541984 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[6]*2u);
    /* 00541987 */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 0054198d */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0054198f */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*2u);
    /* 00541992 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00541994 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[2]*4u);
    /* 00541997 */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[2]);
    /* 0054199d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00541aa7;
    /* 005419a3 */
    f->r[2]=0x005cc8c0u;
    /* 005419a8 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
mi_005419ac:;
    /* 005419ac */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 005419b0 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
    /* 005419b2 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[6]=t;
mi_005419b5:;
    /* 005419b5 */
    f->r[1]=MI_U32((0x00000000u+f->r[2]));
    /* 005419b7 */
    f->r[3]=MI_U32((0x00bc9768u));
    /* 005419bd */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005419bf */
    f->r[3]=MI_U32((0x00af598cu+f->r[1]*4u));
    /* 005419c6 */
    a=f->r[3];b=0x10000000u;t=mi_alu(f,a,b,32,4);f->r[3]=t;
    /* 005419cc */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[3]);
    /* 005419d3 */
    f->r[3]=(f->r[3]&0xffff0000u)|(((MI_U16((0x00000024u+f->r[4])))&0xffffu)<<0);
    /* 005419d8 */
    MI_U16((0x00b4e6fcu+f->r[1]*2u))=(uint16_t)((f->r[3]&0xffffu));
    /* 005419e0 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[1])))&0xffu)<<0);
    /* 005419e6 */
    a=(f->r[3]&0xffu);b=0x000000e0u;t=mi_alu(f,a,b,8,3);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 005419e9 */
    MI_U8((0x00b285ccu+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 005419ef */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[1])))&0xffu)<<0);
    /* 005419f5 */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 005419f8 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 005419fa */
    MI_U8((0x00b67d1cu+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 00541a00 */
    if((f->flags&MI_ZF))goto mi_00541a26;
    /* 00541a02 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00541a04 */
    if((f->flags&MI_ZF))goto mi_00541a0d;
    /* 00541a06 */
    f->r[3]=(0xffffffffu+f->r[5]);
    /* 00541a09 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00541a0b */
    if(!(f->flags&MI_ZF))goto mi_00541a4d;
mi_00541a0d:;
    /* 00541a0d */
    a=MI_U32((0x00000014u+f->r[4]));b=0x005cc8c0u;t=mi_alu(f,a,b,32,1);
    /* 00541a15 */
    if((f->flags&MI_ZF))goto mi_00541a26;
    /* 00541a17 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
    /* 00541a1b */
    f->r[0]=(0xffffffffu+f->r[5]);
    /* 00541a1e */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00541a20 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 00541a24 */
    if(!(f->flags&MI_ZF))goto mi_00541a4d;
mi_00541a26:;
    /* 00541a26 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00541a28 */
    if((f->flags&MI_ZF))goto mi_00541a4d;
    /* 00541a2a */
    a=f->r[0];b=MI_U32((0x00f56b54u));t=mi_alu(f,a,b,32,1);
    /* 00541a30 */
    if(!(f->flags&MI_ZF))goto mi_00541a46;
    /* 00541a32 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[1])))&0xffu)<<0);
    /* 00541a38 */
    a=f->r[3];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 00541a3b */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00541a3d */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[3]);
    /* 00541a44 */
    goto mi_00541a4d;
mi_00541a46:;
    /* 00541a46 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[0]);
mi_00541a4d:;
    /* 00541a4d */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00541a4e */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00541a51 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00541a53 */
    if(mi_signed_less(f->flags))goto mi_005419b5;
    /* 00541a59 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00541a5d */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00541a61 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00541a62 */
    a=f->r[2];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00541a65 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00541a67 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00541a6b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00541a6f */
    if(mi_signed_less(f->flags))goto mi_005419ac;
    /* 00541a75 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00541a76 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00541a77 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00541a78 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00541a79 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00541a7c */
    f->r[4]+=4;return;
mi_00541a7d:;
    /* 00541a7d */
    a=(f->r[6]&0xffffu);b=(f->r[6]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00541a80 */
    if((f->flags&MI_ZF))goto mi_00541a89;
    /* 00541a82 */
    a=0x10000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541a87 */
    goto mi_00541a8b;
mi_00541a89:;
    /* 00541a89 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00541a8b:;
    /* 00541a8b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[1]));
    /* 00541a92 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541a93 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541a94 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[1]));
    /* 00541a9b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541a9c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541a9d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541a9e */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00541a9f */
    p=0x0047bf50u;f->r[4]-=4;MI_U32(f->r[4])=0x00541aa4u;f->boundary(0x00541a9fu,p,f);
    /* 00541aa4 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00541aa7:;
    /* 00541aa7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00541aa8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00541aa9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00541aaa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00541aab */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00541aae */
    f->r[4]+=4;return;
}

void restore_building_method_C_00545c30(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00545c30 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00545c33 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000002cu+f->r[4])))&0xffu)<<0);
    /* 00545c37 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545c38 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545c39 */
    f->r[6]=MI_U32((0x00000030u+f->r[4]));
    /* 00545c3d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00545c3f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545c40 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00545c42 */
    f->r[7]=f->r[1];
    /* 00545c44 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00545c48 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00545c4c */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00545c50 */
    if(!(f->flags&MI_ZF))goto mi_00545c5f;
    /* 00545c52 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545c53 */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x00545c58u;f->boundary(0x00545c53u,p,f);
    /* 00545c58 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00545c5b */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00545c5f:;
    /* 00545c5f */
    f->r[2]=MI_U32((0x00001e5cu+f->r[7]));
    /* 00545c65 */
    f->r[1]=MI_U32((0x00001e60u+f->r[7]));
    /* 00545c6b */
    f->r[0]=f->r[6];
    /* 00545c6d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545c6e */
    f->r[5]=MI_U32((0x00000030u+f->r[4]));
    /* 00545c72 */
    MI_U8((0x0000004cu+f->r[7]))=(uint8_t)((f->r[3]&0xffu));
    /* 00545c75 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00545c78 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545c7a */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00545c7c */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00545c7f */
    MI_U32((0x00000054u+f->r[7]))=(uint32_t)(0x00000001u);
    /* 00545c86 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 00545c8e */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 00545c95 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 00545c99 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00545c9b */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 00545ca2 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 00545ca6 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545ca8 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00545cab */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 00545cae */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 00545cb4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00545cb7 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 00545cbb */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545cbd */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 00545cc1 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545cc3 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00545cc9 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00001e68u+f->r[7])))&0xffffu)<<0);
    /* 00545cd0 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00545cd7 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00001e64u+f->r[7])))&0xffu)<<0);
    /* 00545cdd */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 00545ce1 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00545ce7 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00545cee */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545cf4 */
    f->r[1]=0x00000001u;
    /* 00545cf9 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545cff */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00545d05 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545d0b */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00545d11 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00001db3u+f->r[7])))&0xffu)<<0);
    /* 00545d17 */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00545d1d */
    f->r[2]=MI_U32((0x00000058u+f->r[7]));
    /* 00545d20 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545d26 */
    f->r[1]=MI_U32((0x000002f4u+f->r[7]));
    /* 00545d2c */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00545d32 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00001d0cu+f->r[7])))&0xffu)<<0);
    /* 00545d38 */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00545d3e */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545d44 */
    f->r[2]=(0x00799a44u+f->r[0]);
    /* 00545d4a */
    f->r[1]=(0x00000590u+f->r[7]);
mi_00545d50:;
    /* 00545d50 */
    f->r[5]=MI_U32((0x00000000u+f->r[1]));
    /* 00545d52 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545d55 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 00545d57 */
    f->r[5]=MI_U32((0x0000003cu+f->r[4]));
    /* 00545d5b */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00545d5e */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00545d5f */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00545d63 */
    if(!(f->flags&MI_ZF))goto mi_00545d50;
    /* 00545d65 */
    f->r[1]=(0x00799a5cu+f->r[0]);
    /* 00545d6b */
    f->r[0]=(0x00001538u+f->r[7]);
    /* 00545d71 */
    f->r[2]=0x00000003u;
mi_00545d76:;
    /* 00545d76 */
    f->r[5]=MI_U32((0x00000000u+f->r[0]));
    /* 00545d78 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545d7b */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[5]);
    /* 00545d7d */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545d80 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00545d81 */
    if(!(f->flags&MI_ZF))goto mi_00545d76;
    /* 00545d83 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545d84 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x00545d89u;f->boundary(0x00545d84u,p,f);
    /* 00545d89 */
    f->r[0]=(0x00001db4u+f->r[7]);
    /* 00545d8f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00545d92 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00545d96 */
    f->r[0]=(0x000002f8u+f->r[7]);
    /* 00545d9c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00545da0 */
    f->r[0]=0xffffe24du;
    /* 00545da5 */
    f->r[1]=(0x00001544u+f->r[7]);
    /* 00545dab */
    f->r[2]=(0x000005a8u+f->r[7]);
    /* 00545db1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00545db3 */
    MI_U32((0x00000008u+f->r[7]))=(uint32_t)(f->r[3]);
    /* 00545db6 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00545dba */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00545dbe */
    f->r[5]=(0x00001e94u+f->r[7]);
    /* 00545dc4 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00545dc8:;
    /* 00545dc8 */
    f->r[1]=MI_U32((0x00000000u+f->r[5]));
    /* 00545dcb */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 00545dcf */
    f->r[0]=MI_U32((0xfffffffcu+f->r[5]));
    /* 00545dd2 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00545dd3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545dd4 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545dd6 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00545ddc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545ddd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545dde */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 00545de2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545de4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545de5 */
    a=0x000000e5u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545dea */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545deb */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x00545df0u;f->boundary(0x00545debu,p,f);
    /* 00545df0 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 00545df4 */
    f->r[6]=f->r[0];
    /* 00545df6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00545df9 */
    f->r[2]=f->r[1];
    /* 00545dfb */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545dfd */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00545e00 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00545e03 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00545e07 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00545e0a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00545e0c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 00545e14 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00545e1b */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 00545e23 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00545e2a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[5])))&0xffffu)<<0);
    /* 00545e2e */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00545e35 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[5])))&0xffu)<<0);
    /* 00545e38 */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 00545e3c */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00545e42 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00545e48 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00545e4c */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545e52 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545e58 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545e5e */
    f->r[2]=MI_U32((0xfffffd64u+f->r[1]));
    /* 00545e64 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545e6a */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545e70 */
    f->r[1]=MI_U32((0x00000000u+f->r[1]));
    /* 00545e72 */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00545e78 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00545e7c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffff59u+f->r[1])))&0xffu)<<0);
    /* 00545e82 */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00545e88 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 00545e8c */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00545e92 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 00545e94 */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00545e9a */
    f->r[1]=(0x00799a44u+f->r[0]);
    /* 00545ea0 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
mi_00545ea4:;
    /* 00545ea4 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 00545ea8 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545eab */
    f->r[2]=MI_U32((0x00000000u+f->r[2]));
    /* 00545ead */
    MI_U32((0xfffffffcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00545eb0 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 00545eb4 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00545eb7 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00545ebb */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00545ebf */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00545ec0 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00545ec4 */
    if(!(f->flags&MI_ZF))goto mi_00545ea4;
    /* 00545ec6 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 00545eca */
    f->r[0]=(0x00799a5cu+f->r[0]);
    /* 00545ed0 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000003u);
mi_00545ed8:;
    /* 00545ed8 */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 00545eda */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545edd */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00545edf */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00545ee3 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545ee6 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00545ee7 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00545eeb */
    if(!(f->flags&MI_ZF))goto mi_00545ed8;
    /* 00545eed */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545eee */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00545ef2 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x00545ef7u;f->boundary(0x00545ef2u,p,f);
    /* 00545ef7 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00545efb */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00545eff */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00545f02 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00545f06 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00545f0a */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 00545f0e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00545f11 */
    a=f->r[5];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00545f14 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00545f15 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00545f19 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00545f1d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00545f21 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00545f23 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00545f27 */
    a=f->r[0];b=0x000000a7u;t=mi_alu(f,a,b,32,1);
    /* 00545f2c */
    if(mi_signed_less(f->flags))goto mi_00545dc8;
    /* 00545f32 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_00545f33:;
    /* 00545f33 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00545f37 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 00545f39 */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*1u);
    /* 00545f3c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00545f3d */
    f->r[1]=f->r[7];
    /* 00545f3f */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00545f42u;f->boundary(0x00545f3fu,p,f);
    /* 00545f42 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00545f43 */
    a=f->r[3];b=0x000000a7u;t=mi_alu(f,a,b,32,1);
    /* 00545f49 */
    if(mi_signed_less(f->flags))goto mi_00545f33;
    /* 00545f4b */
    p=0x004dea50u;f->r[4]-=4;MI_U32(f->r[4])=0x00545f50u;f->boundary(0x00545f4bu,p,f);
    /* 00545f50 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00545f51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00545f52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00545f53 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00545f56 */
    f->r[4]+=20;return;
}

static const uint8_t mi_rom_00547710[]={0xad,0x6b,0x54,0x00,0x77,0x6c,0x54,0x00,0xdd,0x6d,0x54,0x00,0xc3,0x6e,0x54,0x00,0xc9,0x6f,0x54,0x00,0x58,0x70,0x54,0x00,0x9b,0x73,0x54,0x00,0x65,0x74,0x54,0x00,0x90,0x6b,0x54,0x00,0x05,0x77,0x54,0x00};
static const uint8_t mi_rom_00547738[]={0x00,0x01,0x09,0x09,0x09,0x09,0x09,0x09,0x02,0x02,0x03,0x09,0x02,0x02,0x09,0x09,0x09,0x09,0x04,0x09,0x09,0x05,0x09,0x06,0x09,0x07,0x08,0x08,0x00};
static const uint8_t mi_rom_00547758[]={0xc3,0x6b,0x54,0x00,0xf0,0x6b,0x54,0x00,0x1d,0x6c,0x54,0x00,0x4a,0x6c,0x54,0x00};
static const uint8_t mi_rom_00547768[]={0x8d,0x6c,0x54,0x00,0xf6,0x6c,0x54,0x00,0x35,0x6d,0x54,0x00,0x74,0x6d,0x54,0x00};
static const uint8_t mi_rom_00547778[]={0xf3,0x6d,0x54,0x00,0x2f,0x6e,0x54,0x00,0x6b,0x6e,0x54,0x00,0x87,0x6e,0x54,0x00};
static const uint8_t mi_rom_00547788[]={0xd9,0x6e,0x54,0x00,0x15,0x6f,0x54,0x00,0x51,0x6f,0x54,0x00,0x8d,0x6f,0x54,0x00};
static const uint8_t mi_rom_00547798[]={0xdf,0x6f,0x54,0x00,0xc5,0x75,0x54,0x00,0x6a,0x76,0x54,0x00,0x7b,0x74,0x54,0x00};
static const uint8_t mi_rom_005477a8[]={0x6e,0x70,0x54,0x00,0x5e,0x71,0x54,0x00,0xbb,0x71,0x54,0x00,0xab,0x72,0x54,0x00};
static const uint8_t mi_rom_005477b8[]={0xb1,0x73,0x54,0x00,0xde,0x73,0x54,0x00,0x0b,0x74,0x54,0x00,0x38,0x74,0x54,0x00};
static const uint8_t mi_rom_005477c8[]={0x7b,0x74,0x54,0x00,0x20,0x75,0x54,0x00,0xc5,0x75,0x54,0x00,0x6a,0x76,0x54,0x00};
static __inline uint32_t mi_read_005468d0(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x00547710u&&width<=40u&&(offset=address-0x00547710u)<=40u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00547710[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00547738u&&width<=29u&&(offset=address-0x00547738u)<=29u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00547738[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00547758u&&width<=16u&&(offset=address-0x00547758u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00547758[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00547768u&&width<=16u&&(offset=address-0x00547768u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00547768[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00547778u&&width<=16u&&(offset=address-0x00547778u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00547778[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00547788u&&width<=16u&&(offset=address-0x00547788u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00547788[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00547798u&&width<=16u&&(offset=address-0x00547798u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00547798[offset+k]<<(k*8u);return result;
 }
 if(address>=0x005477a8u&&width<=16u&&(offset=address-0x005477a8u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_005477a8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x005477b8u&&width<=16u&&(offset=address-0x005477b8u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_005477b8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x005477c8u&&width<=16u&&(offset=address-0x005477c8u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_005477c8[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}


void restore_building_method_C_005468d0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005468d0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 005468d3 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005468d4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005468d5 */
    f->r[5]=mi_read_005468d0((0x0000001cu+f->r[4]),4u);
    /* 005468d9 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005468da */
    f->r[6]=f->r[5];
    /* 005468dc */
    f->r[3]=f->r[1];
    /* 005468de */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 005468e1 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 005468e3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005468e4 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 005468e7 */
    f->r[7]=f->r[5];
    /* 005468e9 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((mi_read_005468d0((0x007999eau+f->r[6]),2u))&0xffffu)<<0);
    /* 005468f0 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 005468f3 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[7]=t;
    /* 005468f5 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 005468f8 */
    if(!(f->flags&MI_ZF))goto mi_0054690a;
    /* 005468fa */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005468fb */
    f->r[1]=f->r[3];
    /* 005468fd */
    p=0x00547ba0u;f->r[4]-=4;MI_U32(f->r[4])=0x00546902u;f->boundary(0x005468fdu,p,f);
    /* 00546902 */
    a=f->r[0];b=0x000000ffu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00546907 */
    MI_U32((0x00000054u+f->r[3]))=(uint32_t)(f->r[0]);
mi_0054690a:;
    /* 0054690a */
    f->r[2]=f->r[7];
    /* 0054690c */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0054690e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_005468d0((0x007999dbu+f->r[6]),1u))&0xffu)<<0);
    /* 00546914 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00546917 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546919 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0054691d */
    a=mi_read_005468d0((0x00799a30u+f->r[2]*8u),4u);b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00546925 */
    if(!(f->flags&MI_ZF))goto mi_00546a3d;
    /* 0054692b */
    a=mi_read_005468d0((0x00e94e0cu),4u);b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 00546932 */
    if((f->flags&MI_ZF))goto mi_00546a3d;
    /* 00546938 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999e0u+f->r[6]),2u);
    /* 0054693f */
    f->r[7]=mi_read_005468d0((0x00bca2d8u),4u);
    /* 00546945 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00546948 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 0054694b */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999deu+f->r[6]),2u);
    /* 00546952 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00546954 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00546957 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999eau+f->r[6]),2u);
    /* 0054695e */
    f->r[6]=mi_read_005468d0((0x00000050u+f->r[3]),4u);
    /* 00546961 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 00546964 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00546966 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0054696a */
    if(!(f->flags&MI_ZF))goto mi_005469b4;
    /* 0054696c */
    f->r[1]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546972 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546973 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00546975 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546976 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546977 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_005468d0((0x00b285ccu+f->r[0]),1u))&0xffu)<<0);
    /* 0054697d */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_005468d0((0x00acf85cu+f->r[0]),1u))&0xffu)<<0);
    /* 00546983 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read_005468d0((0x00b67d1cu+f->r[0]),1u))&0xffu)<<0);
    /* 00546989 */
    a=(f->r[1]&0xffu);b=0x000000e0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0054698c */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00546992 */
    f->r[1]=mi_read_005468d0((0x00f56b54u),4u);
    /* 00546998 */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0054699b */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0054699e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005469a0 */
    MI_U8((0x00b67d1cu+f->r[0]))=(uint8_t)((f->r[3]&0xffu));
    /* 005469a6 */
    MI_U32((0x00b7482cu+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 005469ad */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005469ae */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005469b1 */
    f->r[4]+=8;return;
mi_005469b4:;
    /* 005469b4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 005469b6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00547705;
    /* 005469bc */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x005cc8c0u);
    /* 005469c4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 005469c8 */
    f->r[6]=0x00000008u;
    /* 005469cd */
    goto mi_005469d3;
mi_005469cf:;
    /* 005469cf */
    f->r[1]=mi_read_005468d0((0x00000024u+f->r[4]),4u);
mi_005469d3:;
    /* 005469d3 */
    f->r[5]=mi_read_005468d0((0x00000010u+f->r[4]),4u);
    /* 005469d7 */
    f->r[3]=f->r[1];
mi_005469d9:;
    /* 005469d9 */
    f->r[7]=mi_read_005468d0((0x00000000u+f->r[5]),4u);
    /* 005469dc */
    f->r[1]=mi_read_005468d0((0x00000018u+f->r[4]),4u);
    /* 005469e0 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 005469e2 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005469e3 */
    p=0x00475fd0u;f->r[4]-=4;MI_U32(f->r[4])=0x005469e8u;f->boundary(0x005469e3u,p,f);
    /* 005469e8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read_005468d0((0x00b285ccu+f->r[7]),1u))&0xffu)<<0);
    /* 005469ee */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_005468d0((0x00b67d1cu+f->r[7]),1u))&0xffu)<<0);
    /* 005469f4 */
    a=(f->r[1]&0xffu);b=0x000000e0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 005469f7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005469fa */
    MI_U8((0x00b285ccu+f->r[7]))=(uint8_t)((f->r[1]&0xffu));
    /* 00546a00 */
    f->r[1]=mi_read_005468d0((0x00f572ecu),4u);
    /* 00546a06 */
    a=(f->r[2]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00546a09 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546a0b */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00546a0d */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00546a0e */
    MI_U8((0x00b67d1cu+f->r[7]))=(uint8_t)((f->r[2]&0xffu));
    /* 00546a14 */
    MI_U32((0x00b7482cu+f->r[7]*4u))=(uint32_t)(f->r[1]);
    /* 00546a1b */
    if(!(f->flags&MI_ZF))goto mi_005469d9;
    /* 00546a1d */
    f->r[1]=mi_read_005468d0((0x00000010u+f->r[4]),4u);
    /* 00546a21 */
    f->r[0]=mi_read_005468d0((0x00000014u+f->r[4]),4u);
    /* 00546a25 */
    a=f->r[1];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546a28 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00546a29 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00546a2d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00546a31 */
    if(!(f->flags&MI_ZF))goto mi_005469cf;
    /* 00546a33 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546a34 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546a35 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546a36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546a37 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546a3a */
    f->r[4]+=8;return;
mi_00546a3d:;
    /* 00546a3d */
    f->r[2]=mi_read_005468d0((0x00000000u+f->r[3]),4u);
    /* 00546a3f */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 00546a43 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546a44 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546a45 */
    f->r[1]=f->r[3];
    /* 00546a47 */
    p=MI_U32((0x0000001cu+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00546a4au;f->boundary(0x00546a47u,p,f);
    /* 00546a4a */
    a=mi_read_005468d0((0x0000001cu+f->r[4]),1u);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00546a4f */
    if((f->flags&MI_ZF))goto mi_00546b2a;
    /* 00546a55 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999e0u+f->r[6]),2u);
    /* 00546a5c */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 00546a5f */
    f->r[1]=(0x00000000u+f->r[1]+f->r[2]*2u);
    /* 00546a62 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999deu+f->r[6]),2u);
    /* 00546a69 */
    f->r[6]=mi_read_005468d0((0x00bca2d8u),4u);
    /* 00546a6f */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00546a72 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00546a74 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[1]*4u);
    /* 00546a77 */
    f->r[1]=mi_read_005468d0((0x00000024u+f->r[4]),4u);
    /* 00546a7b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00546a7d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00547705;
    /* 00546a83 */
    f->r[7]=0x005cc8c0u;
    /* 00546a88 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00546a8c */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00546a90 */
    f->r[6]=0x00000008u;
    /* 00546a95 */
    goto mi_00546a9b;
mi_00546a97:;
    /* 00546a97 */
    f->r[1]=mi_read_005468d0((0x00000024u+f->r[4]),4u);
mi_00546a9b:;
    /* 00546a9b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00546a9f:;
    /* 00546a9f */
    f->r[3]=mi_read_005468d0((0x00000000u+f->r[7]),4u);
    /* 00546aa1 */
    f->r[1]=f->r[2];
    /* 00546aa3 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546aa5 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read_005468d0((0x00b285ccu+f->r[1]),1u))&0xffu)<<0);
    /* 00546aab */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546ab2 */
    a=(f->r[3]&0xffu);b=0x000000e0u;t=mi_alu(f,a,b,8,3);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00546ab5 */
    MI_U16((0x00b4e6fcu+f->r[1]*2u))=(uint16_t)((f->r[5]&0xffffu));
    /* 00546abd */
    MI_U8((0x00b285ccu+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 00546ac3 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read_005468d0((0x00b67d1cu+f->r[1]),1u))&0xffu)<<0);
    /* 00546ac9 */
    a=(f->r[3]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00546acc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00546ace */
    MI_U8((0x00b67d1cu+f->r[1]))=(uint8_t)((f->r[3]&0xffu));
    /* 00546ad4 */
    if((f->flags&MI_ZF))goto mi_00546af9;
    /* 00546ad6 */
    a=f->r[0];b=mi_read_005468d0((0x00f56b54u),4u);t=mi_alu(f,a,b,32,1);
    /* 00546adc */
    if(!(f->flags&MI_ZF))goto mi_00546af2;
    /* 00546ade */
    f->r[3]=(f->r[3]&0xffffff00u)|(((mi_read_005468d0((0x00acf85cu+f->r[1]),1u))&0xffu)<<0);
    /* 00546ae4 */
    a=f->r[3];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[3]=t;
    /* 00546ae7 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00546ae9 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[3]);
    /* 00546af0 */
    goto mi_00546af9;
mi_00546af2:;
    /* 00546af2 */
    MI_U32((0x00b7482cu+f->r[1]*4u))=(uint32_t)(f->r[0]);
mi_00546af9:;
    /* 00546af9 */
    f->r[1]=mi_read_005468d0((0x00000014u+f->r[4]),4u);
    /* 00546afd */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00546aff */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00546b00 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00546b04 */
    if(!(f->flags&MI_ZF))goto mi_00546a9f;
    /* 00546b06 */
    f->r[7]=mi_read_005468d0((0x00000010u+f->r[4]),4u);
    /* 00546b0a */
    f->r[1]=mi_read_005468d0((0x00000018u+f->r[4]),4u);
    /* 00546b0e */
    a=f->r[7];b=0x00000030u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00546b11 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00546b12 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 00546b16 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00546b1a */
    if(!(f->flags&MI_ZF))goto mi_00546a97;
    /* 00546b20 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546b21 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546b22 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546b23 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546b24 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546b27 */
    f->r[4]+=8;return;
mi_00546b2a:;
    /* 00546b2a */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999deu+f->r[6]),2u);
    /* 00546b31 */
    a=0x40000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546b36 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546b37 */
    f->r[0]=mi_read_005468d0((0x0000002cu+f->r[4]),4u);
    /* 00546b3b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546b3c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546b3d */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999e0u+f->r[6]),2u);
    /* 00546b44 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546b45 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546b46 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00546b47 */
    p=0x0047bf50u;f->r[4]-=4;MI_U32(f->r[4])=0x00546b4cu;f->boundary(0x00546b47u,p,f);
    /* 00546b4c */
    f->r[0]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999e0u+f->r[6]),2u);
    /* 00546b53 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)mi_read_005468d0((0x007999deu+f->r[6]),2u);
    /* 00546b5a */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00546b5d */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546b60 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 00546b63 */
    f->r[2]=mi_read_005468d0((0x00bca2d8u),4u);
    /* 00546b69 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546b6b */
    f->r[1]=mi_read_005468d0((0x00799a40u+f->r[6]),4u);
    /* 00546b71 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00546b74 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00546b75 */
    a=f->r[1];b=0x0000001cu;t=mi_alu(f,a,b,32,1);
    /* 00546b78 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 00546b7b */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00546b81 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00546b83 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read_005468d0((0x00547738u+f->r[1]),1u))&0xffu)<<0);
    /* 00546b89 */
    switch(mi_read_005468d0((0x00547710u+f->r[2]*4u),4u)){case 0x00546b90u:goto mi_00546b90;case 0x00546badu:goto mi_00546bad;case 0x00546c77u:goto mi_00546c77;case 0x00546dddu:goto mi_00546ddd;case 0x00546ec3u:goto mi_00546ec3;case 0x00546fc9u:goto mi_00546fc9;case 0x00547058u:goto mi_00547058;case 0x0054739bu:goto mi_0054739b;case 0x00547465u:goto mi_00547465;case 0x00547705u:goto mi_00547705;default:abort();}
mi_00546b90:;
    /* 00546b90 */
    f->r[1]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546b96 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546b97 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546b99 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546b9a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546b9b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546b9c */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(0x00000008u);
    /* 00546ba7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546baa */
    f->r[4]+=8;return;
mi_00546bad:;
    /* 00546bad */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 00546bb3 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00546bb6 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00546bbc */
    switch(mi_read_005468d0((0x00547758u+f->r[6]*4u),4u)){case 0x00546bc3u:goto mi_00546bc3;case 0x00546bf0u:goto mi_00546bf0;case 0x00546c1du:goto mi_00546c1d;case 0x00546c4au:goto mi_00546c4a;default:abort();}
mi_00546bc3:;
    /* 00546bc3 */
    f->r[2]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00546bc9 */
    f->r[1]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 00546bcf */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546bd1 */
    f->r[6]=0x00000008u;
    /* 00546bd6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546bd8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546bd9 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546be0 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546be7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546be8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546be9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546bea */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546bed */
    f->r[4]+=8;return;
mi_00546bf0:;
    /* 00546bf0 */
    f->r[2]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00546bf6 */
    f->r[1]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 00546bfc */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546bfe */
    f->r[6]=0x00000008u;
    /* 00546c03 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546c05 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546c06 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546c0d */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546c14 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546c15 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546c16 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546c17 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546c1a */
    f->r[4]+=8;return;
mi_00546c1d:;
    /* 00546c1d */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546c23 */
    f->r[1]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00546c29 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546c2b */
    f->r[6]=0x00000008u;
    /* 00546c30 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546c32 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546c33 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546c3a */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546c41 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546c42 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546c43 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546c44 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546c47 */
    f->r[4]+=8;return;
mi_00546c4a:;
    /* 00546c4a */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546c50 */
    f->r[1]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00546c56 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546c58 */
    f->r[6]=0x00000008u;
    /* 00546c5d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546c5f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546c60 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546c67 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546c6e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546c6f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546c70 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546c71 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546c74 */
    f->r[4]+=8;return;
mi_00546c77:;
    /* 00546c77 */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 00546c7d */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00546c80 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00546c86 */
    switch(mi_read_005468d0((0x00547768u+f->r[6]*4u),4u)){case 0x00546c8du:goto mi_00546c8d;case 0x00546cf6u:goto mi_00546cf6;case 0x00546d35u:goto mi_00546d35;case 0x00546d74u:goto mi_00546d74;default:abort();}
mi_00546c8d:;
    /* 00546c8d */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546c93 */
    f->r[1]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00546c99 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546c9b */
    f->r[6]=0x00000008u;
    /* 00546ca0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546ca2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546ca3 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546caa */
    f->r[2]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00546cb0 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546cb7 */
    f->r[1]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00546cbd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546cbf */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546cc1 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546cc8 */
    f->r[2]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 00546cce */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546cd5 */
    f->r[1]=mi_read_005468d0((0x005cc900u),4u);
    /* 00546cdb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546cdd */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546cdf */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546ce6 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546ced */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546cee */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546cef */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546cf0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546cf3 */
    f->r[4]+=8;return;
mi_00546cf6:;
    /* 00546cf6 */
    f->r[2]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00546cfc */
    f->r[1]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00546d02 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546d04 */
    f->r[6]=0x00000008u;
    /* 00546d09 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546d0b */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546d12 */
    f->r[2]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 00546d18 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546d1f */
    f->r[1]=mi_read_005468d0((0x005cc900u),4u);
    /* 00546d25 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546d27 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546d29 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546d30 */
    goto mi_00546f22;
mi_00546d35:;
    /* 00546d35 */
    f->r[2]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00546d3b */
    f->r[1]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 00546d41 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546d43 */
    f->r[6]=0x00000008u;
    /* 00546d48 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546d4a */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546d51 */
    f->r[2]=mi_read_005468d0((0x005cc900u),4u);
    /* 00546d57 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546d5e */
    f->r[1]=mi_read_005468d0((0x005cc920u),4u);
    /* 00546d64 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546d66 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546d68 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546d6f */
    goto mi_00546f22;
mi_00546d74:;
    /* 00546d74 */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546d7a */
    f->r[1]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00546d80 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546d82 */
    f->r[6]=0x00000008u;
    /* 00546d87 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546d89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546d8a */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546d91 */
    f->r[2]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00546d97 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546d9e */
    f->r[1]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 00546da4 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546da6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546da8 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546daf */
    f->r[2]=mi_read_005468d0((0x005cc920u),4u);
    /* 00546db5 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546dbc */
    f->r[1]=mi_read_005468d0((0x005cc928u),4u);
    /* 00546dc2 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546dc4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546dc6 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546dcd */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546dd4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546dd5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546dd6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546dd7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546dda */
    f->r[4]+=8;return;
mi_00546ddd:;
    /* 00546ddd */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 00546de3 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00546de6 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00546dec */
    switch(mi_read_005468d0((0x00547778u+f->r[6]*4u),4u)){case 0x00546df3u:goto mi_00546df3;case 0x00546e2fu:goto mi_00546e2f;case 0x00546e6bu:goto mi_00546e6b;case 0x00546e87u:goto mi_00546e87;default:abort();}
mi_00546df3:;
    /* 00546df3 */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546df9 */
    f->r[1]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00546dff */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546e01 */
    f->r[6]=0x00000008u;
    /* 00546e06 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546e08 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546e09 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546e10 */
    f->r[2]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00546e16 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546e18 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546e1f */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546e26 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546e27 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546e28 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546e29 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546e2c */
    f->r[4]+=8;return;
mi_00546e2f:;
    /* 00546e2f */
    f->r[1]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00546e35 */
    f->r[2]=mi_read_005468d0((0x005cc900u),4u);
    /* 00546e3b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546e3d */
    f->r[6]=0x00000008u;
    /* 00546e42 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546e44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546e45 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546e4c */
    f->r[1]=mi_read_005468d0((0x005cc930u),4u);
    /* 00546e52 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546e54 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546e5b */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546e62 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546e63 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546e64 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546e65 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546e68 */
    f->r[4]+=8;return;
mi_00546e6b:;
    /* 00546e6b */
    f->r[2]=mi_read_005468d0((0x005cc920u),4u);
    /* 00546e71 */
    f->r[1]=mi_read_005468d0((0x005cc928u),4u);
    /* 00546e77 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546e79 */
    f->r[6]=0x00000008u;
    /* 00546e7e */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546e85 */
    goto mi_00546ef3;
mi_00546e87:;
    /* 00546e87 */
    f->r[1]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546e8d */
    f->r[2]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00546e93 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546e95 */
    f->r[6]=0x00000008u;
    /* 00546e9a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546e9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546e9d */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546ea4 */
    f->r[1]=mi_read_005468d0((0x005cc920u),4u);
    /* 00546eaa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546eac */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546eb3 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546eba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546ebb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546ebc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546ebd */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546ec0 */
    f->r[4]+=8;return;
mi_00546ec3:;
    /* 00546ec3 */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 00546ec9 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00546ecc */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00546ed2 */
    switch(mi_read_005468d0((0x00547788u+f->r[6]*4u),4u)){case 0x00546ed9u:goto mi_00546ed9;case 0x00546f15u:goto mi_00546f15;case 0x00546f51u:goto mi_00546f51;case 0x00546f8du:goto mi_00546f8d;default:abort();}
mi_00546ed9:;
    /* 00546ed9 */
    f->r[2]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00546edf */
    f->r[1]=mi_read_005468d0((0x005cc900u),4u);
    /* 00546ee5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546ee7 */
    f->r[6]=0x00000008u;
    /* 00546eec */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
mi_00546ef3:;
    /* 00546ef3 */
    f->r[2]=mi_read_005468d0((0x005cc930u),4u);
    /* 00546ef9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546efb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546efd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546efe */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546f05 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546f0c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546f0d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546f0e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546f0f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546f12 */
    f->r[4]+=8;return;
mi_00546f15:;
    /* 00546f15 */
    f->r[1]=mi_read_005468d0((0x005cc920u),4u);
    /* 00546f1b */
    f->r[6]=0x00000008u;
    /* 00546f20 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
mi_00546f22:;
    /* 00546f22 */
    f->r[2]=mi_read_005468d0((0x005cc928u),4u);
    /* 00546f28 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546f2f */
    f->r[1]=mi_read_005468d0((0x005cc930u),4u);
    /* 00546f35 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546f37 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546f39 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546f3a */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546f41 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546f48 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546f49 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546f4a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546f4b */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546f4e */
    f->r[4]+=8;return;
mi_00546f51:;
    /* 00546f51 */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546f57 */
    f->r[1]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00546f5d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546f5f */
    f->r[6]=0x00000008u;
    /* 00546f64 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546f66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546f67 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546f6e */
    f->r[2]=mi_read_005468d0((0x005cc920u),4u);
    /* 00546f74 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546f76 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546f7d */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546f84 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546f85 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546f86 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546f87 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546f8a */
    f->r[4]+=8;return;
mi_00546f8d:;
    /* 00546f8d */
    f->r[1]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00546f93 */
    f->r[2]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00546f99 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546f9b */
    f->r[6]=0x00000008u;
    /* 00546fa0 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546fa2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00546fa3 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546faa */
    f->r[1]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00546fb0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546fb2 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546fb9 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00546fc0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00546fc1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00546fc2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00546fc3 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00546fc6 */
    f->r[4]+=8;return;
mi_00546fc9:;
    /* 00546fc9 */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 00546fcf */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00546fd2 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00546fd8 */
    switch(mi_read_005468d0((0x00547798u+f->r[6]*4u),4u)){case 0x00546fdfu:goto mi_00546fdf;case 0x0054747bu:goto mi_0054747b;case 0x005475c5u:goto mi_005475c5;case 0x0054766au:goto mi_0054766a;default:abort();}
mi_00546fdf:;
    /* 00546fdf */
    f->r[2]=mi_read_005468d0((0x005cc8d8u),4u);
    /* 00546fe5 */
    f->r[1]=mi_read_005468d0((0x005cc8e0u),4u);
    /* 00546feb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00546fed */
    f->r[6]=0x00000008u;
    /* 00546ff2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00546ff4 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00546ffb */
    f->r[2]=mi_read_005468d0((0x005cc908u),4u);
    /* 00547001 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547003 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054700a */
    f->r[1]=mi_read_005468d0((0x005cc910u),4u);
    /* 00547010 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547017 */
    f->r[2]=mi_read_005468d0((0x005cc938u),4u);
    /* 0054701d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054701f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547021 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547028 */
    f->r[1]=mi_read_005468d0((0x005cc940u),4u);
    /* 0054702e */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547035 */
    f->r[2]=mi_read_005468d0((0x005cc968u),4u);
    /* 0054703b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054703d */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054703f */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547046 */
    f->r[1]=mi_read_005468d0((0x005cc970u),4u);
    /* 0054704c */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547053 */
    goto mi_00547639;
mi_00547058:;
    /* 00547058 */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 0054705e */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00547061 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00547067 */
    switch(mi_read_005468d0((0x005477a8u+f->r[6]*4u),4u)){case 0x0054706eu:goto mi_0054706e;case 0x0054715eu:goto mi_0054715e;case 0x005471bbu:goto mi_005471bb;case 0x005472abu:goto mi_005472ab;default:abort();}
mi_0054706e:;
    /* 0054706e */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00547074 */
    f->r[1]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 0054707a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054707c */
    f->r[6]=0x00000008u;
    /* 00547081 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547083 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00547084 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054708b */
    f->r[2]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00547091 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547093 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054709a */
    f->r[1]=mi_read_005468d0((0x005cc8d8u),4u);
    /* 005470a0 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005470a7 */
    f->r[2]=mi_read_005468d0((0x005cc8e0u),4u);
    /* 005470ad */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005470af */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005470b1 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005470b8 */
    f->r[1]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 005470be */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005470c5 */
    f->r[2]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 005470cb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005470cd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005470cf */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005470d6 */
    f->r[1]=mi_read_005468d0((0x005cc900u),4u);
    /* 005470dc */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005470e3 */
    f->r[2]=mi_read_005468d0((0x005cc908u),4u);
    /* 005470e9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005470eb */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005470ed */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005470f4 */
    f->r[1]=mi_read_005468d0((0x005cc910u),4u);
    /* 005470fa */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547101 */
    f->r[2]=mi_read_005468d0((0x005cc920u),4u);
    /* 00547107 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547109 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054710b */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547112 */
    f->r[1]=mi_read_005468d0((0x005cc928u),4u);
    /* 00547118 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054711f */
    f->r[2]=mi_read_005468d0((0x005cc930u),4u);
    /* 00547125 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547127 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547129 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547130 */
    f->r[1]=mi_read_005468d0((0x005cc938u),4u);
    /* 00547136 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054713d */
    f->r[2]=mi_read_005468d0((0x005cc940u),4u);
    /* 00547143 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547145 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547147 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054714e */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547155 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547156 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00547157 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00547158 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054715b */
    f->r[4]+=8;return;
mi_0054715e:;
    /* 0054715e */
    f->r[1]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00547164 */
    f->r[2]=mi_read_005468d0((0x005cc900u),4u);
    /* 0054716a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054716c */
    f->r[6]=0x00000008u;
    /* 00547171 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547173 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054717a */
    f->r[1]=mi_read_005468d0((0x005cc930u),4u);
    /* 00547180 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547187 */
    f->r[2]=mi_read_005468d0((0x005cc960u),4u);
    /* 0054718d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054718f */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547191 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547198 */
    f->r[1]=mi_read_005468d0((0x005cc990u),4u);
    /* 0054719e */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005471a5 */
    f->r[2]=mi_read_005468d0((0x005cc8d8u),4u);
    /* 005471ab */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005471ad */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005471af */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005471b6 */
    goto mi_0054752d;
mi_005471bb:;
    /* 005471bb */
    f->r[2]=mi_read_005468d0((0x005cc920u),4u);
    /* 005471c1 */
    f->r[1]=mi_read_005468d0((0x005cc928u),4u);
    /* 005471c7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005471c9 */
    f->r[6]=0x00000008u;
    /* 005471ce */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005471d0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005471d1 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005471d8 */
    f->r[2]=mi_read_005468d0((0x005cc930u),4u);
    /* 005471de */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005471e0 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005471e7 */
    f->r[1]=mi_read_005468d0((0x005cc938u),4u);
    /* 005471ed */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005471f4 */
    f->r[2]=mi_read_005468d0((0x005cc940u),4u);
    /* 005471fa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005471fc */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005471fe */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547205 */
    f->r[1]=mi_read_005468d0((0x005cc950u),4u);
    /* 0054720b */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547212 */
    f->r[2]=mi_read_005468d0((0x005cc958u),4u);
    /* 00547218 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054721a */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054721c */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547223 */
    f->r[1]=mi_read_005468d0((0x005cc960u),4u);
    /* 00547229 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547230 */
    f->r[2]=mi_read_005468d0((0x005cc968u),4u);
    /* 00547236 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547238 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054723a */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547241 */
    f->r[1]=mi_read_005468d0((0x005cc970u),4u);
    /* 00547247 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054724e */
    f->r[2]=mi_read_005468d0((0x005cc980u),4u);
    /* 00547254 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547256 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547258 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054725f */
    f->r[1]=mi_read_005468d0((0x005cc988u),4u);
    /* 00547265 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054726c */
    f->r[2]=mi_read_005468d0((0x005cc990u),4u);
    /* 00547272 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547274 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547276 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054727d */
    f->r[1]=mi_read_005468d0((0x005cc998u),4u);
    /* 00547283 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054728a */
    f->r[2]=mi_read_005468d0((0x005cc9a0u),4u);
    /* 00547290 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547292 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547294 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054729b */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005472a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005472a3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005472a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005472a5 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005472a8 */
    f->r[4]+=8;return;
mi_005472ab:;
    /* 005472ab */
    f->r[1]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 005472b1 */
    f->r[2]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 005472b7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005472b9 */
    f->r[6]=0x00000008u;
    /* 005472be */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005472c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005472c1 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005472c8 */
    f->r[1]=mi_read_005468d0((0x005cc920u),4u);
    /* 005472ce */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005472d0 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005472d7 */
    f->r[2]=mi_read_005468d0((0x005cc950u),4u);
    /* 005472dd */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005472e4 */
    f->r[1]=mi_read_005468d0((0x005cc980u),4u);
    /* 005472ea */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005472ec */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005472ee */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005472f5 */
    f->r[2]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 005472fb */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547302 */
    f->r[1]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 00547308 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054730a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054730c */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547313 */
    f->r[2]=mi_read_005468d0((0x005cc928u),4u);
    /* 00547319 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547320 */
    f->r[1]=mi_read_005468d0((0x005cc958u),4u);
    /* 00547326 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547328 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054732a */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547331 */
    f->r[2]=mi_read_005468d0((0x005cc988u),4u);
    /* 00547337 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054733e */
    f->r[1]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00547344 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547346 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547348 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054734f */
    f->r[2]=mi_read_005468d0((0x005cc900u),4u);
    /* 00547355 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054735c */
    f->r[1]=mi_read_005468d0((0x005cc930u),4u);
    /* 00547362 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547364 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547366 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054736d */
    f->r[2]=mi_read_005468d0((0x005cc960u),4u);
    /* 00547373 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054737a */
    f->r[1]=mi_read_005468d0((0x005cc990u),4u);
    /* 00547380 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547382 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547384 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054738b */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547392 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547393 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00547394 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00547395 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00547398 */
    f->r[4]+=8;return;
mi_0054739b:;
    /* 0054739b */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 005473a1 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 005473a4 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 005473aa */
    switch(mi_read_005468d0((0x005477b8u+f->r[6]*4u),4u)){case 0x005473b1u:goto mi_005473b1;case 0x005473deu:goto mi_005473de;case 0x0054740bu:goto mi_0054740b;case 0x00547438u:goto mi_00547438;default:abort();}
mi_005473b1:;
    /* 005473b1 */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 005473b7 */
    f->r[1]=mi_read_005468d0((0x005cc930u),4u);
    /* 005473bd */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005473bf */
    f->r[6]=0x00000008u;
    /* 005473c4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005473c6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005473c7 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005473ce */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005473d5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005473d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005473d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005473d8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005473db */
    f->r[4]+=8;return;
mi_005473de:;
    /* 005473de */
    f->r[2]=mi_read_005468d0((0x005cc920u),4u);
    /* 005473e4 */
    f->r[1]=mi_read_005468d0((0x005cc930u),4u);
    /* 005473ea */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005473ec */
    f->r[6]=0x00000008u;
    /* 005473f1 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005473f3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005473f4 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005473fb */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547402 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547403 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00547404 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00547405 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00547408 */
    f->r[4]+=8;return;
mi_0054740b:;
    /* 0054740b */
    f->r[2]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00547411 */
    f->r[1]=mi_read_005468d0((0x005cc930u),4u);
    /* 00547417 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547419 */
    f->r[6]=0x00000008u;
    /* 0054741e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547420 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00547421 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547428 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054742f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547430 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00547431 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00547432 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00547435 */
    f->r[4]+=8;return;
mi_00547438:;
    /* 00547438 */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 0054743e */
    f->r[1]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 00547444 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547446 */
    f->r[6]=0x00000008u;
    /* 0054744b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054744d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054744e */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547455 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054745c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054745d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0054745e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054745f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00547462 */
    f->r[4]+=8;return;
mi_00547465:;
    /* 00547465 */
    f->r[6]=mi_read_005468d0((0x00799a68u+f->r[6]),4u);
    /* 0054746b */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0054746e */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00547705;
    /* 00547474 */
    switch(mi_read_005468d0((0x005477c8u+f->r[6]*4u),4u)){case 0x0054747bu:goto mi_0054747b;case 0x00547520u:goto mi_00547520;case 0x005475c5u:goto mi_005475c5;case 0x0054766au:goto mi_0054766a;default:abort();}
mi_0054747b:;
    /* 0054747b */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00547481 */
    f->r[1]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 00547487 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547489 */
    f->r[6]=0x00000008u;
    /* 0054748e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547490 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00547491 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547498 */
    f->r[2]=mi_read_005468d0((0x005cc8d0u),4u);
    /* 0054749e */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005474a5 */
    f->r[1]=mi_read_005468d0((0x005cc8d8u),4u);
    /* 005474ab */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005474ad */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005474af */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005474b6 */
    f->r[2]=mi_read_005468d0((0x005cc8e0u),4u);
    /* 005474bc */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005474c3 */
    f->r[1]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 005474c9 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005474cb */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005474cd */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005474d4 */
    f->r[2]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 005474da */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005474e1 */
    f->r[1]=mi_read_005468d0((0x005cc900u),4u);
    /* 005474e7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005474e9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005474eb */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005474f2 */
    f->r[2]=mi_read_005468d0((0x005cc908u),4u);
    /* 005474f8 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005474ff */
    f->r[1]=mi_read_005468d0((0x005cc910u),4u);
    /* 00547505 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547507 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547509 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547510 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547517 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547518 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00547519 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054751a */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054751d */
    f->r[4]+=8;return;
mi_00547520:;
    /* 00547520 */
    f->r[2]=mi_read_005468d0((0x005cc8d8u),4u);
    /* 00547526 */
    f->r[6]=0x00000008u;
    /* 0054752b */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_0054752d:;
    /* 0054752d */
    f->r[1]=mi_read_005468d0((0x005cc908u),4u);
    /* 00547533 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054753a */
    f->r[2]=mi_read_005468d0((0x005cc938u),4u);
    /* 00547540 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547542 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547544 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00547545 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054754c */
    f->r[1]=mi_read_005468d0((0x005cc968u),4u);
    /* 00547552 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547554 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054755b */
    f->r[2]=mi_read_005468d0((0x005cc998u),4u);
    /* 00547561 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547568 */
    f->r[1]=mi_read_005468d0((0x005cc8e0u),4u);
    /* 0054756e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547570 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547572 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547579 */
    f->r[2]=mi_read_005468d0((0x005cc910u),4u);
    /* 0054757f */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547586 */
    f->r[1]=mi_read_005468d0((0x005cc940u),4u);
    /* 0054758c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054758e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547590 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547597 */
    f->r[2]=mi_read_005468d0((0x005cc970u),4u);
    /* 0054759d */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005475a4 */
    f->r[1]=mi_read_005468d0((0x005cc9a0u),4u);
    /* 005475aa */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005475ac */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005475ae */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005475b5 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005475bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005475bd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 005475be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005475bf */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005475c2 */
    f->r[4]+=8;return;
mi_005475c5:;
    /* 005475c5 */
    f->r[2]=mi_read_005468d0((0x005cc950u),4u);
    /* 005475cb */
    f->r[1]=mi_read_005468d0((0x005cc958u),4u);
    /* 005475d1 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005475d3 */
    f->r[6]=0x00000008u;
    /* 005475d8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005475da */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005475e1 */
    f->r[2]=mi_read_005468d0((0x005cc960u),4u);
    /* 005475e7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005475e9 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005475f0 */
    f->r[1]=mi_read_005468d0((0x005cc968u),4u);
    /* 005475f6 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005475fd */
    f->r[2]=mi_read_005468d0((0x005cc970u),4u);
    /* 00547603 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547605 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547607 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054760e */
    f->r[1]=mi_read_005468d0((0x005cc980u),4u);
    /* 00547614 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054761b */
    f->r[2]=mi_read_005468d0((0x005cc988u),4u);
    /* 00547621 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547623 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547625 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054762c */
    f->r[1]=mi_read_005468d0((0x005cc990u),4u);
    /* 00547632 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
mi_00547639:;
    /* 00547639 */
    f->r[2]=mi_read_005468d0((0x005cc998u),4u);
    /* 0054763f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547641 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547643 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00547644 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 0054764b */
    f->r[1]=mi_read_005468d0((0x005cc9a0u),4u);
    /* 00547651 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00547653 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 0054765a */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547661 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547662 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00547663 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00547664 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00547667 */
    f->r[4]+=8;return;
mi_0054766a:;
    /* 0054766a */
    f->r[2]=mi_read_005468d0((0x005cc8c0u),4u);
    /* 00547670 */
    f->r[1]=mi_read_005468d0((0x005cc8f0u),4u);
    /* 00547676 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00547678 */
    f->r[6]=0x00000008u;
    /* 0054767d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054767f */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 00547686 */
    f->r[2]=mi_read_005468d0((0x005cc920u),4u);
    /* 0054768c */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 00547693 */
    f->r[1]=mi_read_005468d0((0x005cc950u),4u);
    /* 00547699 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054769b */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054769d */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005476a4 */
    f->r[2]=mi_read_005468d0((0x005cc980u),4u);
    /* 005476aa */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005476b1 */
    f->r[1]=mi_read_005468d0((0x005cc8c8u),4u);
    /* 005476b7 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005476b9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005476bb */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005476c2 */
    f->r[2]=mi_read_005468d0((0x005cc8f8u),4u);
    /* 005476c8 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005476cf */
    f->r[1]=mi_read_005468d0((0x005cc928u),4u);
    /* 005476d5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005476d7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005476d9 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005476e0 */
    f->r[2]=mi_read_005468d0((0x005cc958u),4u);
    /* 005476e6 */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
    /* 005476ed */
    f->r[1]=mi_read_005468d0((0x005cc988u),4u);
    /* 005476f3 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005476f5 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005476f7 */
    MI_U32((0x00af598cu+f->r[2]*4u))=(uint32_t)(f->r[6]);
    /* 005476fe */
    MI_U32((0x00af598cu+f->r[1]*4u))=(uint32_t)(f->r[6]);
mi_00547705:;
    /* 00547705 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00547706 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00547707 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00547708 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00547709 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054770c */
    f->r[4]+=8;return;
}

void restore_building_method_C_0054fba0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0054fba0 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0054fba3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000002cu+f->r[4])))&0xffu)<<0);
    /* 0054fba7 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fba8 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fba9 */
    f->r[6]=MI_U32((0x00000030u+f->r[4]));
    /* 0054fbad */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0054fbaf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fbb0 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 0054fbb2 */
    f->r[7]=f->r[1];
    /* 0054fbb4 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0054fbb8 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0054fbbc */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0054fbc0 */
    if(!(f->flags&MI_ZF))goto mi_0054fbcf;
    /* 0054fbc2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fbc3 */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x0054fbc8u;f->boundary(0x0054fbc3u,p,f);
    /* 0054fbc8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054fbcb */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_0054fbcf:;
    /* 0054fbcf */
    f->r[2]=MI_U32((0x00005c88u+f->r[7]));
    /* 0054fbd5 */
    f->r[1]=MI_U32((0x00005c8cu+f->r[7]));
    /* 0054fbdb */
    f->r[0]=f->r[6];
    /* 0054fbdd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fbde */
    f->r[5]=MI_U32((0x00000030u+f->r[4]));
    /* 0054fbe2 */
    MI_U8((0x0000004cu+f->r[7]))=(uint8_t)((f->r[3]&0xffu));
    /* 0054fbe5 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0054fbe8 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054fbea */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0054fbec */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0054fbef */
    MI_U32((0x00000054u+f->r[7]))=(uint32_t)(0x00000001u);
    /* 0054fbf6 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 0054fbfe */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0054fc05 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 0054fc09 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0054fc0b */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 0054fc12 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 0054fc16 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fc18 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 0054fc1b */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 0054fc1e */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 0054fc24 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 0054fc27 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 0054fc2b */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fc2d */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 0054fc31 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fc33 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0054fc39 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00005c94u+f->r[7])))&0xffffu)<<0);
    /* 0054fc40 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0054fc47 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00005c90u+f->r[7])))&0xffu)<<0);
    /* 0054fc4d */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 0054fc51 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0054fc57 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0054fc5e */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054fc64 */
    f->r[1]=0x00000001u;
    /* 0054fc69 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fc6f */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0054fc75 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fc7b */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0054fc81 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00005a85u+f->r[7])))&0xffu)<<0);
    /* 0054fc87 */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0054fc8d */
    f->r[2]=MI_U32((0x00000058u+f->r[7]));
    /* 0054fc90 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054fc96 */
    f->r[1]=MI_U32((0x0000085cu+f->r[7]));
    /* 0054fc9c */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0054fca2 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00005884u+f->r[7])))&0xffu)<<0);
    /* 0054fca8 */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0054fcae */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fcb4 */
    f->r[2]=(0x00799a44u+f->r[0]);
    /* 0054fcba */
    f->r[1]=(0x00001060u+f->r[7]);
mi_0054fcc0:;
    /* 0054fcc0 */
    f->r[5]=MI_U32((0x00000000u+f->r[1]));
    /* 0054fcc2 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fcc5 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 0054fcc7 */
    f->r[5]=MI_U32((0x0000003cu+f->r[4]));
    /* 0054fccb */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054fcce */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0054fccf */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 0054fcd3 */
    if(!(f->flags&MI_ZF))goto mi_0054fcc0;
    /* 0054fcd5 */
    f->r[1]=(0x00799a5cu+f->r[0]);
    /* 0054fcdb */
    f->r[0]=(0x00004078u+f->r[7]);
    /* 0054fce1 */
    f->r[2]=0x00000003u;
mi_0054fce6:;
    /* 0054fce6 */
    f->r[5]=MI_U32((0x00000000u+f->r[0]));
    /* 0054fce8 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054fceb */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[5]);
    /* 0054fced */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fcf0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0054fcf1 */
    if(!(f->flags&MI_ZF))goto mi_0054fce6;
    /* 0054fcf3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fcf4 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x0054fcf9u;f->boundary(0x0054fcf4u,p,f);
    /* 0054fcf9 */
    f->r[0]=(0x00005885u+f->r[7]);
    /* 0054fcff */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054fd02 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0054fd06 */
    f->r[0]=(0x00000860u+f->r[7]);
    /* 0054fd0c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0054fd10 */
    f->r[0]=0xffffa77cu;
    /* 0054fd15 */
    f->r[1]=(0x00004084u+f->r[7]);
    /* 0054fd1b */
    f->r[2]=(0x00001078u+f->r[7]);
    /* 0054fd21 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0054fd23 */
    MI_U32((0x00000008u+f->r[7]))=(uint32_t)(f->r[3]);
    /* 0054fd26 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0054fd2a */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0054fd2e */
    f->r[5]=(0x00005cc0u+f->r[7]);
    /* 0054fd34 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0054fd38:;
    /* 0054fd38 */
    f->r[1]=MI_U32((0x00000000u+f->r[5]));
    /* 0054fd3b */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 0054fd3f */
    f->r[0]=MI_U32((0xfffffffcu+f->r[5]));
    /* 0054fd42 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0054fd43 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fd44 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fd46 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0054fd4c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fd4d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fd4e */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 0054fd52 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054fd54 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fd55 */
    a=0x000000ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fd5a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fd5b */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x0054fd60u;f->boundary(0x0054fd5bu,p,f);
    /* 0054fd60 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 0054fd64 */
    f->r[6]=f->r[0];
    /* 0054fd66 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0054fd69 */
    f->r[2]=f->r[1];
    /* 0054fd6b */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054fd6d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 0054fd70 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054fd73 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0054fd77 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0054fd7a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054fd7c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 0054fd84 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0054fd8b */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 0054fd93 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0054fd9a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[5])))&0xffffu)<<0);
    /* 0054fd9e */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0054fda5 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[5])))&0xffu)<<0);
    /* 0054fda8 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0054fdac */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0054fdb2 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0054fdb8 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0054fdbc */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fdc2 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fdc8 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fdce */
    f->r[2]=MI_U32((0xfffff7fcu+f->r[1]));
    /* 0054fdd4 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054fdda */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 0054fdde */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fde4 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000201u+f->r[2])))&0xffu)<<0);
    /* 0054fdea */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0054fdf0 */
    f->r[1]=MI_U32((0x00000000u+f->r[1]));
    /* 0054fdf2 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 0054fdf6 */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0054fdfc */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[2])))&0xffu)<<0);
    /* 0054fdfe */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 0054fe02 */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0054fe08 */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0054fe0e */
    f->r[1]=(0x00799a44u+f->r[0]);
    /* 0054fe14 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
mi_0054fe18:;
    /* 0054fe18 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 0054fe1c */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fe1f */
    f->r[2]=MI_U32((0x00000000u+f->r[2]));
    /* 0054fe21 */
    MI_U32((0xfffffffcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 0054fe24 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 0054fe28 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0054fe2b */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0054fe2f */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0054fe33 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0054fe34 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0054fe38 */
    if(!(f->flags&MI_ZF))goto mi_0054fe18;
    /* 0054fe3a */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 0054fe3e */
    f->r[0]=(0x00799a5cu+f->r[0]);
    /* 0054fe44 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000003u);
mi_0054fe4c:;
    /* 0054fe4c */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 0054fe4e */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fe51 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0054fe53 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0054fe57 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054fe5a */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0054fe5b */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0054fe5f */
    if(!(f->flags&MI_ZF))goto mi_0054fe4c;
    /* 0054fe61 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054fe62 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0054fe66 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x0054fe6bu;f->boundary(0x0054fe66u,p,f);
    /* 0054fe6b */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0054fe6f */
    f->r[0]=MI_U32((0x00000040u+f->r[4]));
    /* 0054fe73 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0054fe76 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0054fe7a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0054fe7e */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 0054fe82 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054fe85 */
    a=f->r[5];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0054fe88 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0054fe89 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0054fe8d */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0054fe91 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0054fe95 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0054fe97 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0054fe9b */
    a=f->r[0];b=0x00000201u;t=mi_alu(f,a,b,32,1);
    /* 0054fea0 */
    if(mi_signed_less(f->flags))goto mi_0054fd38;
    /* 0054fea6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_0054fea7:;
    /* 0054fea7 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0054feab */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 0054fead */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*1u);
    /* 0054feb0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0054feb1 */
    f->r[1]=f->r[7];
    /* 0054feb3 */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x0054feb6u;f->boundary(0x0054feb3u,p,f);
    /* 0054feb6 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0054feb7 */
    a=f->r[3];b=0x00000201u;t=mi_alu(f,a,b,32,1);
    /* 0054febd */
    if(mi_signed_less(f->flags))goto mi_0054fea7;
    /* 0054febf */
    p=0x004dea50u;f->r[4]-=4;MI_U32(f->r[4])=0x0054fec4u;f->boundary(0x0054febfu,p,f);
    /* 0054fec4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0054fec5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0054fec6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0054fec7 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0054feca */
    f->r[4]+=20;return;
}

void restore_building_method_C_00556480(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00556480 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00556483 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000002cu+f->r[4])))&0xffu)<<0);
    /* 00556487 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00556488 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00556489 */
    f->r[6]=MI_U32((0x00000030u+f->r[4]));
    /* 0055648d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0055648f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00556490 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00556492 */
    f->r[7]=f->r[1];
    /* 00556494 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00556498 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0055649c */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 005564a0 */
    if(!(f->flags&MI_ZF))goto mi_005564af;
    /* 005564a2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005564a3 */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x005564a8u;f->boundary(0x005564a3u,p,f);
    /* 005564a8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005564ab */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[0]);
mi_005564af:;
    /* 005564af */
    f->r[2]=MI_U32((0x00004ee0u+f->r[7]));
    /* 005564b5 */
    f->r[1]=MI_U32((0x00004ee4u+f->r[7]));
    /* 005564bb */
    f->r[0]=f->r[6];
    /* 005564bd */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005564be */
    f->r[5]=MI_U32((0x00000030u+f->r[4]));
    /* 005564c2 */
    MI_U8((0x0000004cu+f->r[7]))=(uint8_t)((f->r[3]&0xffu));
    /* 005564c5 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005564c8 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005564ca */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 005564cc */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005564cf */
    MI_U32((0x00000054u+f->r[7]))=(uint32_t)(0x00000001u);
    /* 005564d6 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 005564de */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 005564e5 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 005564e9 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 005564eb */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 005564f2 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 005564f6 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005564f8 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 005564fb */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 005564fe */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 00556504 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 00556507 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 0055650b */
    f->r[5]=0x00000001u;
    /* 00556510 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00556512 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 00556516 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00556518 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0055651e */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00004eecu+f->r[7])))&0xffffu)<<0);
    /* 00556525 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0055652c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00004ee8u+f->r[7])))&0xffu)<<0);
    /* 00556532 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 00556536 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0055653c */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00556543 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00556549 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0055654f */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 00556555 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0055655b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00004d29u+f->r[7])))&0xffu)<<0);
    /* 00556561 */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00556567 */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[5]);
    /* 0055656d */
    f->r[2]=MI_U32((0x00000058u+f->r[7]));
    /* 00556570 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00556576 */
    f->r[1]=MI_U32((0x0000072cu+f->r[7]));
    /* 0055657c */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00556582 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00004b74u+f->r[7])))&0xffu)<<0);
    /* 00556588 */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0055658e */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00556594 */
    f->r[2]=(0x00799a44u+f->r[0]);
    /* 0055659a */
    f->r[1]=(0x00000e00u+f->r[7]);
mi_005565a0:;
    /* 005565a0 */
    f->r[5]=MI_U32((0x00000000u+f->r[1]));
    /* 005565a2 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005565a5 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 005565a7 */
    f->r[5]=MI_U32((0x0000003cu+f->r[4]));
    /* 005565ab */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005565ae */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 005565af */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 005565b3 */
    if(!(f->flags&MI_ZF))goto mi_005565a0;
    /* 005565b5 */
    f->r[1]=(0x00799a5cu+f->r[0]);
    /* 005565bb */
    f->r[0]=(0x000036f8u+f->r[7]);
    /* 005565c1 */
    f->r[2]=0x00000003u;
mi_005565c6:;
    /* 005565c6 */
    f->r[5]=MI_U32((0x00000000u+f->r[0]));
    /* 005565c8 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005565cb */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[5]);
    /* 005565cd */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005565d0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005565d1 */
    if(!(f->flags&MI_ZF))goto mi_005565c6;
    /* 005565d3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005565d4 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x005565d9u;f->boundary(0x005565d4u,p,f);
    /* 005565d9 */
    f->r[0]=(0x00004b75u+f->r[7]);
    /* 005565df */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005565e2 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 005565e6 */
    f->r[0]=(0x00000730u+f->r[7]);
    /* 005565ec */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 005565f0 */
    f->r[0]=0xffffb48cu;
    /* 005565f5 */
    f->r[1]=(0x00003704u+f->r[7]);
    /* 005565fb */
    f->r[2]=(0x00000e18u+f->r[7]);
    /* 00556601 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00556603 */
    MI_U32((0x00000008u+f->r[7]))=(uint32_t)(f->r[3]);
    /* 00556606 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0055660a */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0055660e */
    f->r[5]=(0x00004f18u+f->r[7]);
    /* 00556614 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00556618:;
    /* 00556618 */
    f->r[1]=MI_U32((0x00000000u+f->r[5]));
    /* 0055661b */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 0055661f */
    f->r[0]=MI_U32((0xfffffffcu+f->r[5]));
    /* 00556622 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00556623 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00556624 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00556626 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0055662c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055662d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055662e */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 00556632 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556634 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00556635 */
    a=0x000000ebu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055663a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055663b */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x00556640u;f->boundary(0x0055663bu,p,f);
    /* 00556640 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 00556644 */
    f->r[6]=f->r[0];
    /* 00556646 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00556649 */
    f->r[2]=f->r[1];
    /* 0055664b */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055664d */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00556650 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00556653 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00556657 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0055665a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055665c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 00556664 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0055666b */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 00556673 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0055667a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[5])))&0xffffu)<<0);
    /* 0055667e */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00556685 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[5])))&0xffu)<<0);
    /* 00556688 */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 0055668c */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00556692 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00556698 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0055669c */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 005566a2 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 005566a8 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 005566ae */
    f->r[2]=MI_U32((0xfffff92cu+f->r[1]));
    /* 005566b4 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 005566ba */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 005566c0 */
    f->r[1]=MI_U32((0x00000000u+f->r[1]));
    /* 005566c2 */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 005566c8 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 005566cc */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x000001b5u+f->r[1])))&0xffu)<<0);
    /* 005566d2 */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 005566d8 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 005566da */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 005566de */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 005566e4 */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 005566ea */
    f->r[1]=(0x00799a44u+f->r[0]);
    /* 005566f0 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
mi_005566f4:;
    /* 005566f4 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 005566f8 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005566fb */
    f->r[2]=MI_U32((0x00000000u+f->r[2]));
    /* 005566fd */
    MI_U32((0xfffffffcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00556700 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 00556704 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00556707 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0055670b */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 0055670f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00556710 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00556714 */
    if(!(f->flags&MI_ZF))goto mi_005566f4;
    /* 00556716 */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 0055671a */
    f->r[0]=(0x00799a5cu+f->r[0]);
    /* 00556720 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000003u);
mi_00556728:;
    /* 00556728 */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 0055672a */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0055672d */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0055672f */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 00556733 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556736 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00556737 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0055673b */
    if(!(f->flags&MI_ZF))goto mi_00556728;
    /* 0055673d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055673e */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00556742 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x00556747u;f->boundary(0x00556742u,p,f);
    /* 00556747 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0055674b */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 0055674f */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00556752 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 00556756 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0055675a */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 0055675e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00556761 */
    a=f->r[5];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00556764 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00556765 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00556769 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 0055676d */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00556771 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00556773 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00556777 */
    a=f->r[0];b=0x000001b5u;t=mi_alu(f,a,b,32,1);
    /* 0055677c */
    if(mi_signed_less(f->flags))goto mi_00556618;
    /* 00556782 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_00556783:;
    /* 00556783 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00556787 */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 00556789 */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*1u);
    /* 0055678c */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055678d */
    f->r[1]=f->r[7];
    /* 0055678f */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00556792u;f->boundary(0x0055678fu,p,f);
    /* 00556792 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00556793 */
    a=f->r[3];b=0x000001b5u;t=mi_alu(f,a,b,32,1);
    /* 00556799 */
    if(mi_signed_less(f->flags))goto mi_00556783;
    /* 0055679b */
    p=0x004dea50u;f->r[4]-=4;MI_U32(f->r[4])=0x005567a0u;f->boundary(0x0055679bu,p,f);
    /* 005567a0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005567a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005567a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 005567a3 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005567a6 */
    f->r[4]+=20;return;
}

void restore_building_method_C_00559130(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00559130 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00559133 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000002cu+f->r[4])))&0xffu)<<0);
    /* 00559137 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559138 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559139 */
    f->r[6]=MI_U32((0x00000030u+f->r[4]));
    /* 0055913d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0055913f */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559140 */
    a=(f->r[0]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00559142 */
    f->r[7]=f->r[1];
    /* 00559144 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00559148 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 0055914c */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 00559150 */
    if(!(f->flags&MI_ZF))goto mi_0055915f;
    /* 00559152 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559153 */
    p=0x004f4f80u;f->r[4]-=4;MI_U32(f->r[4])=0x00559158u;f->boundary(0x00559153u,p,f);
    /* 00559158 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055915b */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_0055915f:;
    /* 0055915f */
    f->r[2]=MI_U32((0x00001cbcu+f->r[7]));
    /* 00559165 */
    f->r[1]=MI_U32((0x00001cc0u+f->r[7]));
    /* 0055916b */
    f->r[0]=f->r[6];
    /* 0055916d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055916e */
    f->r[5]=MI_U32((0x00000030u+f->r[4]));
    /* 00559172 */
    MI_U8((0x0000004cu+f->r[7]))=(uint8_t)((f->r[3]&0xffu));
    /* 00559175 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 00559178 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055917a */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0055917c */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0055917f */
    MI_U32((0x00000054u+f->r[7]))=(uint32_t)(0x00000001u);
    /* 00559186 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 0055918e */
    MI_U16((0x007999deu+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 00559195 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 00559199 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 0055919b */
    MI_U16((0x007999e0u+f->r[0]))=(uint16_t)((f->r[5]&0xffffu));
    /* 005591a2 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 005591a6 */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005591a8 */
    f->r[5]=(0x00000000u+f->r[1]+f->r[1]*8u);
    /* 005591ab */
    f->r[1]=(0x00000000u+f->r[1]+f->r[5]*2u);
    /* 005591ae */
    f->r[5]=MI_U32((0x00bca2d8u));
    /* 005591b4 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*2u);
    /* 005591b7 */
    f->r[1]=(0x00000000u+f->r[5]+f->r[1]*4u);
    /* 005591bb */
    f->r[5]=0x00000001u;
    /* 005591c0 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005591c2 */
    f->r[2]=MI_U32((0x00000030u+f->r[4]));
    /* 005591c6 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005591c8 */
    MI_U32((0x007999e4u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 005591ce */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00001cc8u+f->r[7])))&0xffffu)<<0);
    /* 005591d5 */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[2]&0xffffu));
    /* 005591dc */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00001cc4u+f->r[7])))&0xffu)<<0);
    /* 005591e2 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 005591e6 */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 005591ec */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 005591f3 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[2]);
    /* 005591f9 */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 005591ff */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[5]);
    /* 00559205 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0055920b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00001c1eu+f->r[7])))&0xffu)<<0);
    /* 00559211 */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00559217 */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[5]);
    /* 0055921d */
    f->r[2]=MI_U32((0x00000058u+f->r[7]));
    /* 00559220 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 00559226 */
    f->r[1]=MI_U32((0x000002d0u+f->r[7]));
    /* 0055922c */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00559232 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00001b80u+f->r[7])))&0xffu)<<0);
    /* 00559238 */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0055923e */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00559244 */
    f->r[2]=(0x00799a44u+f->r[0]);
    /* 0055924a */
    f->r[1]=(0x00000548u+f->r[7]);
mi_00559250:;
    /* 00559250 */
    f->r[5]=MI_U32((0x00000000u+f->r[1]));
    /* 00559252 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559255 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(f->r[5]);
    /* 00559257 */
    f->r[5]=MI_U32((0x0000003cu+f->r[4]));
    /* 0055925b */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055925e */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0055925f */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00559263 */
    if(!(f->flags&MI_ZF))goto mi_00559250;
    /* 00559265 */
    f->r[1]=(0x00799a5cu+f->r[0]);
    /* 0055926b */
    f->r[0]=(0x00001418u+f->r[7]);
    /* 00559271 */
    f->r[2]=0x00000003u;
mi_00559276:;
    /* 00559276 */
    f->r[5]=MI_U32((0x00000000u+f->r[0]));
    /* 00559278 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0055927b */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[5]);
    /* 0055927d */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559280 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00559281 */
    if(!(f->flags&MI_ZF))goto mi_00559276;
    /* 00559283 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559284 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x00559289u;f->boundary(0x00559284u,p,f);
    /* 00559289 */
    f->r[0]=(0x00001b81u+f->r[7]);
    /* 0055928f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559292 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00559296 */
    f->r[0]=(0x000002d4u+f->r[7]);
    /* 0055929c */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 005592a0 */
    f->r[0]=0xffffe480u;
    /* 005592a5 */
    f->r[1]=(0x00001424u+f->r[7]);
    /* 005592ab */
    f->r[2]=(0x00000560u+f->r[7]);
    /* 005592b1 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 005592b3 */
    MI_U32((0x00000008u+f->r[7]))=(uint32_t)(f->r[3]);
    /* 005592b6 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 005592ba */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 005592be */
    f->r[5]=(0x00001cf4u+f->r[7]);
    /* 005592c4 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_005592c8:;
    /* 005592c8 */
    f->r[1]=MI_U32((0x00000000u+f->r[5]));
    /* 005592cb */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 005592cf */
    f->r[0]=MI_U32((0xfffffffcu+f->r[5]));
    /* 005592d2 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 005592d3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005592d4 */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005592d6 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 005592dc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005592dd */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005592de */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 005592e2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005592e4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005592e5 */
    a=0x000000eau;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005592ea */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005592eb */
    p=0x0046a400u;f->r[4]-=4;MI_U32(f->r[4])=0x005592f0u;f->boundary(0x005592ebu,p,f);
    /* 005592f0 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 005592f4 */
    f->r[6]=f->r[0];
    /* 005592f6 */
    a=f->r[0];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 005592f9 */
    f->r[2]=f->r[1];
    /* 005592fb */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005592fd */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00559300 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559303 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00559307 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 0055930a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0055930c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000006u);
    /* 00559314 */
    MI_U16((0x00799a10u+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0055931b */
    MI_U16((0x00799a12u+f->r[2]*8u))=(uint16_t)((f->r[6]&0xffffu));
    /* 00559323 */
    MI_U16((0x00799a12u+f->r[0]))=(uint16_t)((f->r[3]&0xffffu));
    /* 0055932a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[5])))&0xffffu)<<0);
    /* 0055932e */
    MI_U16((0x007999eau+f->r[0]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00559335 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[5])))&0xffu)<<0);
    /* 00559338 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 0055933c */
    MI_U8((0x007999dbu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00559342 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 00559346 */
    MI_U32((0x00799a2cu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0055934c */
    MI_U32((0x00799a30u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00559352 */
    MI_U32((0x00799a38u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00559358 */
    MI_U32((0x00799a34u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0055935e */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0000009eu+f->r[2])))&0xffu)<<0);
    /* 00559364 */
    MI_U8((0x00799a74u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0055936a */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 0055936e */
    f->r[2]=MI_U32((0xfffffd88u+f->r[1]));
    /* 00559374 */
    MI_U32((0x00799a68u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0055937a */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 0055937e */
    MI_U32((0x00799a3cu+f->r[0]))=(uint32_t)(f->r[3]);
    /* 00559384 */
    f->r[1]=MI_U32((0x00000000u+f->r[1]));
    /* 00559386 */
    MI_U32((0x00799a40u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0055938c */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[2])))&0xffu)<<0);
    /* 0055938e */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 00559392 */
    MI_U8((0x00799a6cu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00559398 */
    MI_U32((0x00799a70u+f->r[0]))=(uint32_t)(f->r[3]);
    /* 0055939e */
    f->r[1]=(0x00799a44u+f->r[0]);
    /* 005593a4 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
mi_005593a8:;
    /* 005593a8 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 005593ac */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005593af */
    f->r[2]=MI_U32((0x00000000u+f->r[2]));
    /* 005593b1 */
    MI_U32((0xfffffffcu+f->r[1]))=(uint32_t)(f->r[2]);
    /* 005593b4 */
    f->r[2]=MI_U32((0x00000038u+f->r[4]));
    /* 005593b8 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 005593bb */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 005593bf */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 005593c3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005593c4 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 005593c8 */
    if(!(f->flags&MI_ZF))goto mi_005593a8;
    /* 005593ca */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 005593ce */
    f->r[0]=(0x00799a5cu+f->r[0]);
    /* 005593d4 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00000003u);
mi_005593dc:;
    /* 005593dc */
    f->r[2]=MI_U32((0x00000000u+f->r[1]));
    /* 005593de */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005593e1 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 005593e3 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 005593e7 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 005593ea */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005593eb */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 005593ef */
    if(!(f->flags&MI_ZF))goto mi_005593dc;
    /* 005593f1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005593f2 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 005593f6 */
    p=0x00485100u;f->r[4]-=4;MI_U32(f->r[4])=0x005593fbu;f->boundary(0x005593f6u,p,f);
    /* 005593fb */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 005593ff */
    f->r[0]=MI_U32((0x00000040u+f->r[4]));
    /* 00559403 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559406 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 0055940a */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0055940e */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 00559412 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559415 */
    a=f->r[5];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00559418 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00559419 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 0055941d */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00559421 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00559425 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00559427 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 0055942b */
    a=f->r[0];b=0x0000009eu;t=mi_alu(f,a,b,32,1);
    /* 00559430 */
    if(mi_signed_less(f->flags))goto mi_005592c8;
    /* 00559436 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
mi_00559437:;
    /* 00559437 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 0055943b */
    f->r[2]=MI_U32((0x00000000u+f->r[7]));
    /* 0055943d */
    f->r[1]=(0x00000000u+f->r[3]+f->r[0]*1u);
    /* 00559440 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559441 */
    f->r[1]=f->r[7];
    /* 00559443 */
    p=MI_U32((0x00000018u+f->r[2]));f->r[4]-=4;MI_U32(f->r[4])=0x00559446u;f->boundary(0x00559443u,p,f);
    /* 00559446 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00559447 */
    a=f->r[3];b=0x0000009eu;t=mi_alu(f,a,b,32,1);
    /* 0055944d */
    if(mi_signed_less(f->flags))goto mi_00559437;
    /* 0055944f */
    p=0x004dea50u;f->r[4]-=4;MI_U32(f->r[4])=0x00559454u;f->boundary(0x0055944fu,p,f);
    /* 00559454 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559455 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00559456 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00559457 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055945a */
    f->r[4]+=20;return;
}

int restore_building_method_has(uint32_t target)
{switch(target){case 0x004015a0u:case 0x004017a0u:case 0x0040da90u:case 0x0040dca0u:case 0x00417180u:case 0x004e6840u:case 0x004e69c0u:case 0x005417b0u:case 0x00541920u:case 0x00545c30u:case 0x005468d0u:case 0x0054fba0u:case 0x00556480u:case 0x00559130u:return 1;default:return 0;}}

void restore_building_method_dispatch(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{(void)site;switch(target){
case 0x004015a0u:restore_building_method_C_004015a0(f);return;
case 0x004017a0u:restore_building_method_C_004017a0(f);return;
case 0x0040da90u:restore_building_method_C_0040da90(f);return;
case 0x0040dca0u:restore_building_method_C_0040dca0(f);return;
case 0x00417180u:restore_building_method_C_00417180(f);return;
case 0x004e6840u:restore_building_method_C_004e6840(f);return;
case 0x004e69c0u:restore_building_method_C_004e69c0(f);return;
case 0x005417b0u:restore_building_method_C_005417b0(f);return;
case 0x00541920u:restore_building_method_C_00541920(f);return;
case 0x00545c30u:restore_building_method_C_00545c30(f);return;
case 0x005468d0u:restore_building_method_C_005468d0(f);return;
case 0x0054fba0u:restore_building_method_C_0054fba0(f);return;
case 0x00556480u:restore_building_method_C_00556480(f);return;
case 0x00559130u:restore_building_method_C_00559130(f);return;
default:abort();}}
