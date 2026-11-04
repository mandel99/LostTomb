#include "labor_allocation.h"
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
 uint32_t sp=f->r[4];restore_labor_allocate_groups();f->r[4]=sp+4u;
}

void restore_labor_C_004595a0(struct restore_labor_frame *f)
{
 uint32_t sp=f->r[4];restore_labor_rebuild_shares();f->r[4]=sp+4u;
}

void restore_labor_C_00459670(struct restore_labor_frame *f)
{
 uint32_t sp=f->r[4];restore_labor_assign_buildings();f->r[4]=sp+4u;
}
