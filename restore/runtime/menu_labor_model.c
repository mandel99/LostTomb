/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_labor_actions.h"
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
static __inline uint32_t mi_alu(struct restore_labor_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_labor_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_labor_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_labor_C_0040a730(struct restore_labor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0040a730 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0040a733 */
    f->r[0]=MI_U32((0x00605988u));
    /* 0040a738 */
    f->r[1]=MI_U32((0x00d3b198u));
    /* 0040a73e */
    MI_U8((0x00e38ddau))=(uint8_t)(0x00000007u);
    /* 0040a745 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a746 */
    f->r[5]=MI_U32((0x00eaac28u));
    /* 0040a74c */
    a=f->r[5];b=0x000049d8u;t=mi_imul(f,a,b);f->r[5]=t;
    /* 0040a752 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0040a755 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a756 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040a75d */
    f->r[6]=(0x00ea43acu+f->r[5]+f->r[0]*4u);
    /* 0040a764 */
    f->r[0]=MI_U32((0x00ea43acu+f->r[5]+f->r[0]*4u));
    /* 0040a76b */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040a76d */
    if((f->flags&MI_ZF))goto mi_0040a894;
    /* 0040a773 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040a775 */
    if(!(f->flags&MI_ZF))goto mi_0040a789;
    /* 0040a777 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0040a779 */
    if((f->flags&MI_ZF))goto mi_0040a785;
    /* 0040a77b */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(0x00000001u);
    /* 0040a783 */
    goto mi_0040a7a7;
mi_0040a785:;
    /* 0040a785 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040a787 */
    if((f->flags&MI_ZF))goto mi_0040a797;
mi_0040a789:;
    /* 0040a789 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0040a78b */
    if(!(f->flags&MI_ZF))goto mi_0040a797;
    /* 0040a78d */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(0xffffffffu);
    /* 0040a795 */
    goto mi_0040a7a7;
mi_0040a797:;
    /* 0040a797 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0040a799 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040a79b */
    t=(!mi_signed_less(f->flags))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0040a79e */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040a79f */
    a=f->r[2];b=0xfffffffeu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0040a7a2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040a7a3 */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[2]);
mi_0040a7a7:;
    /* 0040a7a7 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 0040a7a9 */
    f->r[0]=(0x00ea43acu+f->r[5]);
    /* 0040a7af */
    f->r[1]=0x00000009u;
mi_0040a7b4:;
    /* 0040a7b4 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040a7b7 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0040a7b8 */
    if(!(f->flags&MI_ZF))goto mi_0040a7b4;
    /* 0040a7ba */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0040a7bc */
    f->r[0]=0x00605768u;
mi_0040a7c1:;
    /* 0040a7c1 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0040a7c3 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040a7c6 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0040a7c7 */
    a=f->r[0];b=0x0060578cu;t=mi_alu(f,a,b,32,1);
    /* 0040a7cc */
    if(mi_signed_less(f->flags))goto mi_0040a7c1;
    /* 0040a7ce */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a7cf */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040a7d0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(0x00000009u);
mi_0040a7d8:;
    /* 0040a7d8 */
    f->r[0]=0x0060576cu;
mi_0040a7dd:;
    /* 0040a7dd */
    f->r[1]=MI_U32((0x00000000u+f->r[0]));
    /* 0040a7df */
    f->r[6]=MI_U32((0xfffffffcu+f->r[0]));
    /* 0040a7e2 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0040a7e4 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0040a7e7 */
    f->r[2]=MI_U32((0x00ea43acu+f->r[5]+f->r[2]*4u));
    /* 0040a7ee */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0040a7f0 */
    if((f->flags&MI_ZF))goto mi_0040a835;
    /* 0040a7f2 */
    f->r[7]=(0x00000000u+f->r[6]+f->r[6]*4u);
    /* 0040a7f5 */
    f->r[7]=MI_U32((0x00ea43acu+f->r[5]+f->r[7]*4u));
    /* 0040a7fc */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0040a7fe */
    if(!(f->flags&MI_ZF))goto mi_0040a805;
    /* 0040a800 */
    f->r[3]=0x00000001u;
mi_0040a805:;
    /* 0040a805 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0040a807 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0040a80e;
    /* 0040a809 */
    f->r[3]=0x00000001u;
mi_0040a80e:;
    /* 0040a80e */
    if(!(f->flags&MI_ZF))goto mi_0040a82c;
    /* 0040a810 */
    f->r[2]=MI_U32((0x00605988u));
    /* 0040a816 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040a818 */
    if(!(f->flags&MI_ZF))goto mi_0040a821;
    /* 0040a81a */
    a=MI_U32((0x00000010u+f->r[4]));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040a81f */
    if((f->flags&MI_ZF))goto mi_0040a830;
mi_0040a821:;
    /* 0040a821 */
    a=f->r[2];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0040a823 */
    if(!(f->flags&MI_ZF))goto mi_0040a82c;
    /* 0040a825 */
    a=MI_U32((0x00000010u+f->r[4]));b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 0040a82a */
    if((f->flags&MI_ZF))goto mi_0040a830;
mi_0040a82c:;
    /* 0040a82c */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0040a82e */
    if((f->flags&MI_ZF))goto mi_0040a835;
mi_0040a830:;
    /* 0040a830 */
    MI_U32((0xfffffffcu+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0040a833 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[6]);
mi_0040a835:;
    /* 0040a835 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040a838 */
    a=f->r[0];b=0x0060578cu;t=mi_alu(f,a,b,32,1);
    /* 0040a83d */
    if(mi_signed_less(f->flags))goto mi_0040a7dd;
    /* 0040a83f */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 0040a843 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0040a844 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040a848 */
    if(!(f->flags&MI_ZF))goto mi_0040a7d8;
    /* 0040a84a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0040a84b */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 0040a84d */
    f->r[1]=0x00605768u;
    /* 0040a852 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0040a853:;
    /* 0040a853 */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 0040a855 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0040a858 */
    f->r[6]=MI_U32((0x00ea43acu+f->r[5]+f->r[0]*4u));
    /* 0040a85f */
    f->r[0]=(0x00ea43acu+f->r[5]+f->r[0]*4u);
    /* 0040a866 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0040a868 */
    if((f->flags&MI_ZF))goto mi_0040a86f;
    /* 0040a86a */
    f->r[6]=(0x00000001u+f->r[2]);
    /* 0040a86d */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[6]);
mi_0040a86f:;
    /* 0040a86f */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0040a872 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0040a873 */
    a=f->r[1];b=0x0060578cu;t=mi_alu(f,a,b,32,1);
    /* 0040a879 */
    if(mi_signed_less(f->flags))goto mi_0040a853;
    /* 0040a87b */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a880u;f->boundary(0x0040a87bu,0x00459400u,f);
    /* 0040a880 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a885u;f->boundary(0x0040a880u,0x004595a0u,f);
    /* 0040a885 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a88au;f->boundary(0x0040a885u,0x0051f780u,f);
    /* 0040a88a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a88fu;f->boundary(0x0040a88au,0x0051f780u,f);
    /* 0040a88f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0040a894u;f->boundary(0x0040a88fu,0x00459670u,f);
mi_0040a894:;
    /* 0040a894 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040a895 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0040a896 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0040a899 */
    f->r[4]+=4;return;
}

void restore_labor_C_00459400(struct restore_labor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00459400 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459401 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459402 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459403 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459404 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459405 */
    f->r[7]=MI_U32((0x00eaac28u));
    /* 0045940b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0045940d */
    a=f->r[7];b=0x000049d8u;t=mi_imul(f,a,b);f->r[7]=t;
    /* 00459413 */
    f->r[2]=0x0000000au;
    /* 00459418 */
    f->r[0]=(0x00ea43a0u+f->r[7]);
mi_0045941e:;
    /* 0045941e */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 00459424 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00459427 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00459428 */
    if(!(f->flags&MI_ZF))goto mi_0045941e;
    /* 0045942a */
    f->r[0]=(0x00ea439cu+f->r[7]);
    /* 00459430 */
    f->r[2]=0x0000000au;
mi_00459435:;
    /* 00459435 */
    f->r[5]=MI_U32((0x00000000u+f->r[0]));
    /* 00459437 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045943a */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045943c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045943d */
    if(!(f->flags&MI_ZF))goto mi_00459435;
    /* 0045943f */
    MI_U32((0x00ea4474u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 00459449 */
    f->r[6]=MI_U32((0x00ea4398u+f->r[7]));
    /* 0045944f */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00459451 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00459474;
    /* 00459453 */
    f->r[0]=(0x00ea43a0u+f->r[7]);
    /* 00459459 */
    f->r[2]=0x0000000au;
mi_0045945e:;
    /* 0045945e */
    f->r[6]=MI_U32((0xfffffffcu+f->r[0]));
    /* 00459461 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[6]);
    /* 00459463 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00459466 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00459467 */
    if(!(f->flags&MI_ZF))goto mi_0045945e;
    /* 00459469 */
    MI_U32((0x00ea4464u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 0045946f */
    goto mi_00459563;
mi_00459474:;
    /* 00459474 */
    f->r[3]=0x00000001u;
    /* 00459479 */
    f->r[2]=f->r[3];
mi_0045947b:;
    /* 0045947b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0045947d */
    f->r[1]=(0x00ea43acu+f->r[7]);
mi_00459483:;
    /* 00459483 */
    a=MI_U32((0x00000000u+f->r[1]));b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00459485 */
    if((f->flags&MI_ZF))goto mi_00459492;
    /* 00459487 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00459488 */
    a=f->r[1];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0045948b */
    a=f->r[0];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0045948e */
    if(mi_signed_less(f->flags))goto mi_00459483;
    /* 00459490 */
    goto mi_004594ac;
mi_00459492:;
    /* 00459492 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00459495 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[0]*4u);
    /* 00459498 */
    f->r[0]=MI_U32((0x00ea439cu+f->r[1]));
    /* 0045949e */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004594a0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004594a4;
    /* 004594a2 */
    f->r[0]=f->r[6];
mi_004594a4:;
    /* 004594a4 */
    MI_U32((0x00ea43a0u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004594aa */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
mi_004594ac:;
    /* 004594ac */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004594ae */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004594b6;
    /* 004594b0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004594b1 */
    a=f->r[2];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 004594b4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045947b;
mi_004594b6:;
    /* 004594b6 */
    f->r[0]=MI_U32((0x00ea4398u+f->r[7]));
    /* 004594bc */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004594c0 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004594c2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459531;
mi_004594c4:;
    /* 004594c4 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004594c6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459531;
    /* 004594c8 */
    f->r[5]=0x005da450u;
mi_004594cd:;
    /* 004594cd */
    f->r[0]=MI_U32((0xfffffffcu+f->r[5]));
    /* 004594d0 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004594d3 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[1]*4u);
    /* 004594d6 */
    f->r[0]=MI_U32((0x00ea43acu+f->r[1]));
    /* 004594dc */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004594de */
    if(!(f->flags&MI_ZF))goto mi_0045950f;
    /* 004594e0 */
    f->r[3]=MI_U32((0x00ea43a0u+f->r[1]));
    /* 004594e6 */
    f->r[2]=MI_U32((0x00ea439cu+f->r[1]));
    /* 004594ec */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004594ee */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004594f0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045950f;
    /* 004594f2 */
    f->r[0]=MI_U32((0x00000000u+f->r[5]));
    /* 004594f5 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004594f7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004594fb;
    /* 004594f9 */
    f->r[0]=f->r[6];
mi_004594fb:;
    /* 004594fb */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004594fd */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459501;
    /* 004594ff */
    f->r[0]=f->r[2];
mi_00459501:;
    /* 00459501 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 00459503 */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00459505 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00459507 */
    MI_U32((0x00ea43a0u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 0045950d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459531;
mi_0045950f:;
    /* 0045950f */
    a=f->r[5];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00459512 */
    a=f->r[5];b=0x005da498u;t=mi_alu(f,a,b,32,1);
    /* 00459518 */
    if(mi_signed_less(f->flags))goto mi_004594cd;
    /* 0045951a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0045951c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459531;
    /* 0045951e */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00459522 */
    f->r[1]=MI_U32((0x00ea4398u+f->r[7]));
    /* 00459528 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00459529 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045952b */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0045952f */
    if(mi_signed_less(f->flags))goto mi_004594c4;
mi_00459531:;
    /* 00459531 */
    f->r[2]=MI_U32((0x00ea4398u+f->r[7]));
    /* 00459537 */
    f->r[0]=(0x00ea43a0u+f->r[7]);
    /* 0045953d */
    MI_U32((0x00ea4464u+f->r[7]))=(uint32_t)(f->r[2]);
    /* 00459543 */
    f->r[1]=0x00000009u;
mi_00459548:;
    /* 00459548 */
    f->r[2]=MI_U32((0xfffffffcu+f->r[0]));
    /* 0045954b */
    f->r[5]=MI_U32((0x00000000u+f->r[0]));
    /* 0045954d */
    f->r[3]=MI_U32((0x00ea4474u+f->r[7]));
    /* 00459553 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00459555 */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00459557 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045955a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0045955b */
    MI_U32((0x00ea4474u+f->r[7]))=(uint32_t)(f->r[3]);
    /* 00459561 */
    if(!(f->flags&MI_ZF))goto mi_00459548;
mi_00459563:;
    /* 00459563 */
    f->r[0]=MI_U32((0x00ea4398u+f->r[7]));
    /* 00459569 */
    f->r[3]=MI_U32((0x00ea4464u+f->r[7]));
    /* 0045956f */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00459571 */
    MI_U32((0x00ea4468u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 00459577 */
    f->r[1]=MI_U32((0x00ea4398u+f->r[7]));
    /* 0045957d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045957e */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045957f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459584u;f->boundary(0x0045957fu,0x004cde60u,f);
    /* 00459584 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 0045958a */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045958d */
    a=f->r[2];b=0x000049d8u;t=mi_imul(f,a,b);f->r[2]=t;
    /* 00459593 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00459594 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00459595 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00459596 */
    MI_U32((0x00ea446cu+f->r[2]))=(uint32_t)(f->r[0]);
    /* 0045959c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0045959d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 0045959e */
    f->r[4]+=4;return;
}

void restore_labor_C_004595a0(struct restore_labor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004595a0 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004595a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004595a6 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004595ac */
    f->r[1]=MI_U32((0x00ea4448u+f->r[0]));
    /* 004595b2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004595b3 */
    a=0x00000064u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004595b5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004595bau;f->boundary(0x004595b5u,0x004cde60u,f);
    /* 004595ba */
    f->r[0]=MI_U32((0x0078d790u));
    /* 004595bf */
    f->r[5]=0x00000001u;
    /* 004595c4 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004595c7 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004595c9 */
    if(mi_signed_less(f->flags))goto mi_00459664;
    /* 004595cf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004595d0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004595d1 */
    f->r[6]=0x00799af0u;
mi_004595d6:;
    /* 004595d6 */
    a=MI_U8((0xfffffff0u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004595da */
    if(!(f->flags&MI_ZF))goto mi_0045964e;
    /* 004595dc */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[6]));
    /* 004595df */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004595e2 */
    f->r[7]=MI_U32((0x005d06ecu+f->r[2]*4u));
    /* 004595e9 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004595eb */
    if(!(f->flags&MI_ZF))goto mi_004595fe;
    /* 004595ed */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004595ee */
    f->r[1]=0x00bca350u;
    /* 004595f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004595f8u;f->boundary(0x004595f3u,0x00488050u,f);
    /* 004595f8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004595fa */
    if((f->flags&MI_ZF))goto mi_00459610;
    /* 004595fc */
    goto mi_0045964e;
mi_004595fe:;
    /* 004595fe */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00459601 */
    if(!(f->flags&MI_ZF))goto mi_0045960c;
    /* 00459603 */
    a=MI_U16((0x00000028u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00459608 */
    if(!(f->flags&MI_ZF))goto mi_0045964e;
    /* 0045960a */
    goto mi_00459610;
mi_0045960c:;
    /* 0045960c */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0045960e */
    if(mi_signed_less(f->flags))goto mi_0045964e;
mi_00459610:;
    /* 00459610 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00459615 */
    f->r[1]=(0x00000000u+f->r[7]+f->r[7]*4u);
    /* 00459618 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045961e */
    MI_U16((0x0000000au+f->r[6]))=(uint16_t)(0x00000000u);
    /* 00459624 */
    f->r[1]=MI_U32((0x00ea43a4u+f->r[0]+f->r[1]*4u));
    /* 0045962b */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[6])))&0xffffu)<<0);
    /* 0045962f */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00459632 */
    if((f->flags&MI_ZF))goto mi_0045964e;
    /* 00459634 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 00459637 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459638 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0045963b */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0045963e */
    a=f->r[2];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 00459641 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459642 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459647u;f->boundary(0x00459642u,0x004cde60u,f);
    /* 00459647 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045964a */
    MI_U16((0x0000000au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_0045964e:;
    /* 0045964e */
    f->r[0]=MI_U32((0x0078d790u));
    /* 00459653 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00459654 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045965a */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0045965c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004595d6;
    /* 00459662 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00459663 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_00459664:;
    /* 00459664 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00459665 */
    f->r[4]+=4;return;
}

void restore_labor_C_00459670(struct restore_labor_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00459670 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00459673 */
    f->r[1]=(0x00000004u+f->r[4]);
    /* 00459677 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459678 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459679 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045967a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045967b */
    f->r[7]=MI_U32((0x00eaac28u));
    /* 00459681 */
    f->r[6]=0x0000000au;
    /* 00459686 */
    f->r[0]=f->r[7];
    /* 00459688 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0045968e */
    a=f->r[0];b=0x00ea43a0u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
mi_00459693:;
    /* 00459693 */
    f->r[2]=MI_U32((0x00000000u+f->r[0]));
    /* 00459695 */
    f->r[5]=MI_U32((0xfffffffcu+f->r[0]));
    /* 00459698 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 0045969a */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 0045969c */
    t=(mi_signed_less(f->flags))?1u:0u;f->r[3]=(f->r[3]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0045969f */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[3]);
    /* 004596a1 */
    a=f->r[0];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004596a4 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004596a7 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004596a8 */
    if(!(f->flags&MI_ZF))goto mi_00459693;
    /* 004596aa */
    f->r[5]=MI_U32((0x0078d790u));
    /* 004596b0 */
    f->r[3]=0x00000001u;
    /* 004596b5 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 004596b7 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004596bb */
    if(mi_signed_less(f->flags))goto mi_00459817;
    /* 004596c1 */
    f->r[7]=0x00799af0u;
mi_004596c6:;
    /* 004596c6 */
    a=MI_U8((0xfffffff0u+f->r[7]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004596ca */
    if(!(f->flags&MI_ZF))goto mi_004597f8;
    /* 004596d0 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 004596d3 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004596d6 */
    f->r[6]=MI_U32((0x005d06ecu+f->r[0]*4u));
    /* 004596dd */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004596e0 */
    if((f->flags&MI_ZF))goto mi_004596e6;
    /* 004596e2 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004596e4 */
    if(!(f->flags&MI_ZF))goto mi_004596fb;
mi_004596e6:;
    /* 004596e6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004596e7 */
    f->r[1]=0x00bca350u;
    /* 004596ec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004596f1u;f->boundary(0x004596ecu,0x00488050u,f);
    /* 004596f1 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004596f3 */
    if(!(f->flags&MI_ZF))goto mi_004597f8;
    /* 004596f9 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
mi_004596fb:;
    /* 004596fb */
    if(mi_signed_less(f->flags))goto mi_004597f8;
    /* 00459701 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00459704 */
    if(!(f->flags&MI_ZF))goto mi_00459711;
    /* 00459706 */
    a=MI_U16((0x00000028u+f->r[7]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045970b */
    if(!(f->flags&MI_ZF))goto mi_004597f8;
mi_00459711:;
    /* 00459711 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00459714 */
    MI_U16((0x00000030u+f->r[7]))=(uint16_t)(0x00000000u);
    /* 0045971a */
    if((f->flags&MI_ZF))goto mi_0045976d;
    /* 0045971c */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0045971e */
    if((f->flags&MI_ZF))goto mi_0045976d;
    /* 00459720 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00459723 */
    if(!(f->flags&MI_ZF))goto mi_00459747;
    /* 00459725 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[7])))&0xffffu)<<0);
    /* 00459728 */
    a=(f->r[0]&0xffffu);b=0x0000001fu;t=mi_alu(f,a,b,16,1);
    /* 0045972c */
    if((f->flags&MI_ZF))goto mi_0045973a;
    /* 0045972e */
    a=(f->r[0]&0xffffu);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 00459732 */
    if((f->flags&MI_ZF))goto mi_0045973a;
    /* 00459734 */
    a=(f->r[0]&0xffffu);b=0x00000021u;t=mi_alu(f,a,b,16,1);
    /* 00459738 */
    if(!(f->flags&MI_ZF))goto mi_0045977a;
mi_0045973a:;
    /* 0045973a */
    a=MI_U16((0x00000028u+f->r[7]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045973f */
    if(!(f->flags&MI_ZF))goto mi_004597f8;
    /* 00459745 */
    goto mi_0045977a;
mi_00459747:;
    /* 00459747 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0045974a */
    if(!(f->flags&MI_ZF))goto mi_0045977a;
    /* 0045974c */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[7])))&0xffffu)<<0);
    /* 0045974f */
    a=(f->r[0]&0xffffu);b=0x00000070u;t=mi_alu(f,a,b,16,1);
    /* 00459753 */
    if((f->flags&MI_ZF))goto mi_0045975b;
    /* 00459755 */
    a=(f->r[0]&0xffffu);b=0x000000cdu;t=mi_alu(f,a,b,16,1);
    /* 00459759 */
    if(!(f->flags&MI_ZF))goto mi_0045977a;
mi_0045975b:;
    /* 0045975b */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045975c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459761u;f->boundary(0x0045975cu,0x00459240u,f);
    /* 00459761 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00459764 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00459766 */
    if((f->flags&MI_ZF))goto mi_0045977a;
    /* 00459768 */
    goto mi_004597f8;
mi_0045976d:;
    /* 0045976d */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045976e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459773u;f->boundary(0x0045976eu,0x00459240u,f);
    /* 00459773 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00459776 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00459778 */
    if(!(f->flags&MI_ZF))goto mi_004597f8;
mi_0045977a:;
    /* 0045977a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000000au+f->r[7])))&0xffffu)<<0);
    /* 0045977e */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00459781 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004597f8;
    /* 00459783 */
    f->r[3]=MI_U32((0x00000014u+f->r[4]+f->r[6]*4u));
    /* 00459787 */
    f->r[5]=(0x00000014u+f->r[4]+f->r[6]*4u);
    /* 0045978b */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0045978d */
    if(!(f->flags&MI_ZF))goto mi_004597a3;
    /* 0045978f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 00459792 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00459795 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00705eb8u+f->r[1]*8u)))&0xffffu)<<0);
    /* 0045979d */
    MI_U16((0x00000030u+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004597a1 */
    goto mi_004597f4;
mi_004597a3:;
    /* 004597a3 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 004597a8 */
    f->r[2]=(0x00000000u+f->r[6]+f->r[6]*4u);
    /* 004597ab */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 004597b1 */
    f->r[1]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 004597b4 */
    f->r[0]=MI_U32((0x00ea43a0u+f->r[0]+f->r[2]*4u));
    /* 004597bb */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004597bc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004597bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004597c2u;f->boundary(0x004597bdu,0x004cde40u,f);
    /* 004597c2 */
    f->r[1]=f->r[0];
    /* 004597c4 */
    f->r[0]=0x51eb851fu;
    /* 004597c9 */
    a=f->r[0];b=f->r[1];{int64_t wide=(int64_t)(int32_t)a*(int32_t)b;uint32_t low=(uint32_t)wide;f->flags&=~(MI_CF|MI_OF);if(wide!=(int64_t)(int32_t)low)f->flags|=MI_CF|MI_OF;f->r[0]=low;f->r[2]=(uint32_t)((uint64_t)wide>>32);}
    /* 004597cb */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 004597ce */
    f->r[0]=f->r[2];
    /* 004597d0 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004597d3 */
    a=f->r[0];t=a>>31u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>30u)&1u)|mi_szp(t,32);f->r[0]=t;
    /* 004597d6 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004597d8 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 004597db */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004597de */
    f->r[0]=MI_U32((0x00705eb8u+f->r[1]*8u));
    /* 004597e5 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004597e7 */
    if(mi_signed_less(f->flags))goto mi_004597eb;
    /* 004597e9 */
    f->r[2]=f->r[0];
mi_004597eb:;
    /* 004597eb */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004597ed */
    MI_U16((0x00000030u+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004597f1 */
    MI_U32((0x00000000u+f->r[5]))=(uint32_t)(f->r[3]);
mi_004597f4:;
    /* 004597f4 */
    f->r[3]=MI_U32((0x00000010u+f->r[4]));
mi_004597f8:;
    /* 004597f8 */
    f->r[5]=MI_U32((0x0078d790u));
    /* 004597fe */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004597ff */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00459805 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00459807 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0045980b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004596c6;
    /* 00459811 */
    f->r[7]=MI_U32((0x00eaac28u));
mi_00459817:;
    /* 00459817 */
    f->r[0]=(0x00000014u+f->r[4]);
    /* 0045981b */
    f->r[2]=0x0000000au;
mi_00459820:;
    /* 00459820 */
    f->r[1]=MI_U32((0x00000000u+f->r[0]));
    /* 00459822 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00459824 */
    if((f->flags&MI_ZF))goto mi_00459829;
    /* 00459826 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00459827 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_00459829:;
    /* 00459829 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0045982c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0045982d */
    if(!(f->flags&MI_ZF))goto mi_00459820;
    /* 0045982f */
    a=f->r[7];b=0x000049d8u;t=mi_imul(f,a,b);f->r[7]=t;
    /* 00459835 */
    a=f->r[7];b=0x00ea43a0u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045983b */
    f->r[0]=(0x00000014u+f->r[4]);
    /* 0045983f */
    f->r[6]=f->r[7];
    /* 00459841 */
    f->r[7]=0x0000000au;
mi_00459846:;
    /* 00459846 */
    f->r[2]=MI_U32((0x00000000u+f->r[0]));
    /* 00459848 */
    f->r[1]=MI_U32((0x00000000u+f->r[6]));
    /* 0045984a */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0045984c */
    if(!mi_signed_less(f->flags))goto mi_00459854;
    /* 0045984e */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00459850 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 00459852 */
    goto mi_0045985a;
mi_00459854:;
    /* 00459854 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
mi_0045985a:;
    /* 0045985a */
    a=f->r[6];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045985d */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00459860 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00459861 */
    if(!(f->flags&MI_ZF))goto mi_00459846;
    /* 00459863 */
    f->r[3]=0x00000001u;
    /* 00459868 */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0045986a */
    if(mi_signed_less(f->flags))goto mi_00459926;
    /* 00459870 */
    f->r[6]=0x00799b20u;
mi_00459875:;
    /* 00459875 */
    a=MI_U8((0xffffffc0u+f->r[6]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00459879 */
    if(!(f->flags&MI_ZF))goto mi_00459911;
    /* 0045987f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffd0u+f->r[6]));
    /* 00459883 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00459886 */
    f->r[7]=MI_U32((0x005d06ecu+f->r[2]*4u));
    /* 0045988d */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0045988f */
    if(!(f->flags&MI_ZF))goto mi_004598a2;
    /* 00459891 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459892 */
    f->r[1]=0x00bca350u;
    /* 00459897 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045989cu;f->boundary(0x00459897u,0x00488050u,f);
    /* 0045989c */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0045989e */
    if((f->flags&MI_ZF))goto mi_004598bc;
    /* 004598a0 */
    goto mi_00459911;
mi_004598a2:;
    /* 004598a2 */
    if(mi_signed_less(f->flags))goto mi_00459911;
    /* 004598a4 */
    a=f->r[7];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004598a7 */
    if((f->flags&MI_ZF))goto mi_00459911;
    /* 004598a9 */
    a=f->r[7];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004598ac */
    if((f->flags&MI_ZF))goto mi_004598bc;
    /* 004598ae */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 004598b1 */
    if(!(f->flags&MI_ZF))goto mi_004598c9;
    /* 004598b3 */
    a=MI_U16((0xfffffff8u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004598b8 */
    if((f->flags&MI_ZF))goto mi_004598c9;
    /* 004598ba */
    goto mi_00459911;
mi_004598bc:;
    /* 004598bc */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004598bd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004598c2u;f->boundary(0x004598bdu,0x00459240u,f);
    /* 004598c2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004598c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004598c7 */
    if(!(f->flags&MI_ZF))goto mi_00459911;
mi_004598c9:;
    /* 004598c9 */
    a=MI_U16((0xffffffdau+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004598ce */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459911;
    /* 004598d0 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]+f->r[7]*4u));
    /* 004598d4 */
    f->r[7]=(0x00000014u+f->r[4]+f->r[7]*4u);
    /* 004598d8 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004598da */
    if((f->flags&MI_ZF))goto mi_00459911;
    /* 004598dc */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0xffffffd0u+f->r[6]));
    /* 004598e0 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[6])))&0xffffu)<<0);
    /* 004598e3 */
    f->r[2]=(0x00000000u+f->r[2]+f->r[2]*4u);
    /* 004598e6 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 004598e9 */
    f->r[2]=MI_U32((0x00705eb8u+f->r[2]*8u));
    /* 004598f0 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004598f2 */
    if(!mi_signed_less(f->flags))goto mi_00459911;
    /* 004598f4 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004598f6 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004598f8 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00459905;
    /* 004598fa */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004598fc */
    a=f->r[1];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004598fe */
    MI_U16((0x00000000u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00459901 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 00459903 */
    goto mi_00459911;
mi_00459905:;
    /* 00459905 */
    a=(f->r[0]&0xffffu);b=MI_U16((0x00000000u+f->r[7]));t=mi_alu(f,a,b,16,0);f->r[0]=(f->r[0]&0xffff0000u)|(((t)&0xffffu)<<0);
    /* 00459908 */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 0045990e */
    MI_U16((0x00000000u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_00459911:;
    /* 00459911 */
    f->r[5]=MI_U32((0x0078d790u));
    /* 00459917 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00459918 */
    a=f->r[6];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 0045991e */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00459920 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459875;
mi_00459926:;
    /* 00459926 */
    f->r[6]=0x00000001u;
    /* 0045992b */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 0045992d */
    if(mi_signed_less(f->flags))goto mi_00459992;
    /* 0045992f */
    f->r[7]=0x00799af0u;
mi_00459934:;
    /* 00459934 */
    a=MI_U8((0xfffffff0u+f->r[7]));b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00459938 */
    if(!(f->flags&MI_ZF))goto mi_00459982;
    /* 0045993a */
    a=MI_U16((0x00000000u+f->r[7]));b=0x00000088u;t=mi_alu(f,a,b,16,1);
    /* 0045993f */
    if(!(f->flags&MI_ZF))goto mi_00459982;
    /* 00459941 */
    a=MI_U16((0x00000030u+f->r[7]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00459946 */
    if((f->flags&MI_ZF))goto mi_0045996f;
    /* 00459948 */
    f->r[0]=MI_U32((0x000000b4u+f->r[7]));
    /* 0045994e */
    f->r[1]=f->r[0];
    /* 00459950 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00459953 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00459955 */
    a=MI_U16((0x00799a18u+f->r[1]*8u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0045995e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0045996f;
    /* 00459960 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459962 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459964 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459965 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045996au;f->boundary(0x00459965u,0x0043f060u,f);
    /* 0045996a */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0045996d */
    goto mi_00459982;
mi_0045996f:;
    /* 0045996f */
    a=0x00000003u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459971 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459973 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00459974 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00459979u;f->boundary(0x00459974u,0x0043f060u,f);
    /* 00459979 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0045997a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0045997fu;f->boundary(0x0045997au,0x004a36b0u,f);
    /* 0045997f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00459982:;
    /* 00459982 */
    f->r[0]=MI_U32((0x0078d790u));
    /* 00459987 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00459988 */
    a=f->r[7];b=0x00000108u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 0045998e */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00459990 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00459934;
mi_00459992:;
    /* 00459992 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00459993 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00459994 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00459995 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00459996 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00459999 */
    f->r[4]+=4;return;
}
