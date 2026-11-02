/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "surface_passes.h"

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

void restore_C_00476530(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00476530 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476531 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476532 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476533 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476534 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476535 */
    f->r[7]=MI_U32((0x00bca2d4u));
    /* 0047653b */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0047653d */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 0047653f */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00476541 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047659e;
    /* 00476543 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000040u)&0xffu)<<0);
mi_00476545:;
    /* 00476545 */
    f->r[2]=MI_U32((0x00bc9f24u+f->r[6]*4u));
    /* 0047654c */
    f->r[1]=MI_U32((0x00bc9b94u+f->r[6]*4u));
    /* 00476553 */
    f->r[0]=MI_U32((0x00bc9804u+f->r[6]*4u));
    /* 0047655a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047655c */
    if(!mi_signed_less(f->flags))goto mi_00476599;
    /* 0047655e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
mi_00476560:;
    /* 00476560 */
    f->r[2]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 00476567 */
    a=(f->r[2]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,3);
    /* 0047656a */
    if((f->flags&MI_ZF))goto mi_00476595;
    /* 0047656c */
    a=((f->r[2]>>8)&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,3);
    /* 0047656f */
    if(!(f->flags&MI_ZF))goto mi_00476595;
    /* 00476571 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 00476577 */
    MI_U32((0x00b7482cu+f->r[0]*4u))=(uint32_t)(f->r[5]);
    /* 0047657e */
    a=(f->r[2]&0xffu);b=0x000000f0u;t=mi_alu(f,a,b,8,3);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00476581 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 00476587 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[0])))&0xffu)<<0);
    /* 0047658d */
    a=(f->r[2]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,4);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047658f */
    MI_U8((0x00b67d1cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_00476595:;
    /* 00476595 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00476596 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00476597 */
    if(!(f->flags&MI_ZF))goto mi_00476560;
mi_00476599:;
    /* 00476599 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047659a */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0047659c */
    if(mi_signed_less(f->flags))goto mi_00476545;
mi_0047659e:;
    /* 0047659e */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 004765a0 */
    a=f->r[7];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004765a2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047673f;
mi_004765a8:;
    /* 004765a8 */
    f->r[6]=MI_U32((0x00bc9f24u+f->r[3]*4u));
    /* 004765af */
    f->r[1]=MI_U32((0x00bc9b94u+f->r[3]*4u));
    /* 004765b6 */
    f->r[0]=MI_U32((0x00bc9804u+f->r[3]*4u));
    /* 004765bd */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004765bf */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004765c3 */
    if(!mi_signed_less(f->flags))goto mi_00476736;
    /* 004765c9 */
    f->r[5]=(0x00acf85cu+f->r[0]);
    /* 004765cf */
    f->r[7]=(0x00000000u+f->r[0]*4u);
mi_004765d6:;
    /* 004765d6 */
    f->r[0]=MI_U32((0x00af598cu+f->r[7]));
    /* 004765dc */
    a=(f->r[0]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,3);
    /* 004765de */
    if((f->flags&MI_ZF))goto mi_0047671f;
    /* 004765e4 */
    a=((f->r[0]>>8)&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,3);
    /* 004765e7 */
    if(!(f->flags&MI_ZF))goto mi_0047671f;
    /* 004765ed */
    f->r[0]=MI_U32((0x00b7482cu+f->r[7]));
    /* 004765f3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004765f5 */
    if(!(f->flags&MI_ZF))goto mi_0047671f;
    /* 004765fb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004765fd */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004765ff */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476601 */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476603 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476604 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476605 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047660au;f->boundary(0x00476605u,0x0047cae0u,f);
    /* 0047660a */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047660d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047660f */
    if((f->flags&MI_ZF))goto mi_00476631;
    /* 00476611 */
    f->r[0]=MI_U32((0x00f565f4u));
    /* 00476616 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476618 */
    a=f->r[0];b=0x00000007u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047661b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047661c */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047661e */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476620 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476621 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476622 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476624 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00476629u;f->boundary(0x00476624u,0x0047bf50u,f);
    /* 00476629 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047662c */
    goto mi_0047671f;
mi_00476631:;
    /* 00476631 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476633 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476635 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476637 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476639 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047663a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047663b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00476640u;f->boundary(0x0047663bu,0x0047cae0u,f);
    /* 00476640 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00476643 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00476645 */
    if((f->flags&MI_ZF))goto mi_0047668d;
    /* 00476647 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[5])))&0xffu)<<0);
    /* 0047664a */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047664d */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00476650 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047665d;
    /* 00476652 */
    f->r[1]=MI_U32((0x00f565f4u));
    /* 00476658 */
    f->r[0]=(0x00000006u+f->r[1]);
    /* 0047665b */
    goto mi_00476675;
mi_0047665d:;
    /* 0047665d */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00476660 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047666d;
    /* 00476662 */
    f->r[2]=MI_U32((0x00f565f4u));
    /* 00476668 */
    f->r[0]=(0x00000005u+f->r[2]);
    /* 0047666b */
    goto mi_00476675;
mi_0047666d:;
    /* 0047666d */
    f->r[0]=MI_U32((0x00f565f4u));
    /* 00476672 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_00476675:;
    /* 00476675 */
    a=0x00000020u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476677 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476678 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047667a */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047667c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047667d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047667e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00476680 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00476685u;f->boundary(0x00476680u,0x0047bf50u,f);
    /* 00476685 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00476688 */
    goto mi_0047671f;
mi_0047668d:;
    /* 0047668d */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00476690 */
    f->r[0]=f->r[6];
    /* 00476692 */
    if(!(f->flags&MI_ZF))goto mi_004766d6;
    /* 00476694 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00476697 */
    if(!(f->flags&MI_ZF))goto mi_004766a7;
    /* 00476699 */
    f->r[1]=MI_U32((0x00f565f4u));
    /* 0047669f */
    MI_U32((0x00b7482cu+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004766a5 */
    goto mi_0047671f;
mi_004766a7:;
    /* 004766a7 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004766aa */
    if(!(f->flags&MI_ZF))goto mi_004766b5;
    /* 004766ac */
    f->r[2]=MI_U32((0x00f565f4u));
    /* 004766b2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004766b3 */
    goto mi_00476719;
mi_004766b5:;
    /* 004766b5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004766b8 */
    if(!(f->flags&MI_ZF))goto mi_004766c7;
    /* 004766ba */
    f->r[0]=MI_U32((0x00f565f4u));
    /* 004766bf */
    MI_U32((0x00b7482cu+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004766c5 */
    goto mi_0047671f;
mi_004766c7:;
    /* 004766c7 */
    f->r[1]=MI_U32((0x00f565f4u));
    /* 004766cd */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004766ce */
    MI_U32((0x00b7482cu+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004766d4 */
    goto mi_0047671f;
mi_004766d6:;
    /* 004766d6 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004766d9 */
    if(!(f->flags&MI_ZF))goto mi_004766e6;
    /* 004766db */
    f->r[2]=MI_U32((0x00f565f4u));
    /* 004766e1 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004766e4 */
    goto mi_00476719;
mi_004766e6:;
    /* 004766e6 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004766e9 */
    if(!(f->flags&MI_ZF))goto mi_004766fb;
    /* 004766eb */
    f->r[0]=MI_U32((0x00f565f4u));
    /* 004766f0 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004766f3 */
    MI_U32((0x00b7482cu+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004766f9 */
    goto mi_0047671f;
mi_004766fb:;
    /* 004766fb */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004766fe */
    if(!(f->flags&MI_ZF))goto mi_00476710;
    /* 00476700 */
    f->r[1]=MI_U32((0x00f565f4u));
    /* 00476706 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00476708 */
    MI_U32((0x00b7482cu+f->r[7]))=(uint32_t)(f->r[1]);
    /* 0047670e */
    goto mi_0047671f;
mi_00476710:;
    /* 00476710 */
    f->r[2]=MI_U32((0x00f565f4u));
    /* 00476716 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
mi_00476719:;
    /* 00476719 */
    MI_U32((0x00b7482cu+f->r[7]))=(uint32_t)(f->r[2]);
mi_0047671f:;
    /* 0047671f */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00476723 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00476724 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00476725 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00476728 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047672a */
    if(mi_signed_less(f->flags))goto mi_004765d6;
    /* 00476730 */
    f->r[7]=MI_U32((0x00bca2d4u));
mi_00476736:;
    /* 00476736 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00476737 */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00476739 */
    if(mi_signed_less(f->flags))goto mi_004765a8;
mi_0047673f:;
    /* 0047673f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00476740 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00476741 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00476742 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00476743 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 00476744 */
    f->r[4]+=4;return;
}
