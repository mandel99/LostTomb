/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "numeric_input.h"

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
static __inline uint32_t mi_alu(struct restore_numeric_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_numeric_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_numeric_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


void restore_numeric_C_004035a0(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004035a0 */
    f->r[0]=MI_U32((0x0060596cu));
    /* 004035a5 */
    f->r[1]=MI_U32((0x00605978u));
    /* 004035ab */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004035ac */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004035ae */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 004035b0 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 004035b3 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004035b6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004035b8 */
    if(!mi_signed_less(f->flags))goto mi_004035ed;
    /* 004035ba */
    f->r[0]=MI_U32((0x0060597cu));
    /* 004035bf */
    f->r[1]=MI_U32((0x00d3b198u));
    /* 004035c5 */
    a=0x00000004u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004035c7 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004035ce */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004035d1 */
    f->r[2]=(0x00000000u+f->r[1]+f->r[0]*2u);
    /* 004035d4 */
    MI_U32((0x0060597cu))=(uint32_t)(f->r[2]);
    /* 004035da */
    f->r[2]=MI_U32((0x00605978u));
    /* 004035e0 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004035e1 */
    MI_U32((0x00605978u))=(uint32_t)(f->r[2]);
    /* 004035e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004035ecu;f->boundary(0x004035e7u,0x0047d350u,f);
    /* 004035ec */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
mi_004035ed:;
    /* 004035ed */
    f->r[4]+=4;return;
}

void restore_numeric_C_004035f0(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004035f0 */
    f->r[0]=MI_U32((0x0060596cu));
    /* 004035f5 */
    f->r[1]=MI_U32((0x00605960u));
    /* 004035fb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004035fc */
    f->r[6]=MI_U32((0x0060595cu));
    /* 00403602 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00403605 */
    if(!(f->flags&MI_ZF))goto mi_0040361b;
    /* 00403607 */
    a=MI_U16((0x00000000u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0040360b */
    if(!mi_signed_less(f->flags))goto mi_0040362a;
    /* 0040360d */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0060597cu)))&0xffffu)<<0);
    /* 00403613 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 00403616 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00403619 */
    goto mi_0040365f;
mi_0040361b:;
    /* 0040361b */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0040361e */
    if(!(f->flags&MI_ZF))goto mi_0040362a;
    /* 00403620 */
    a=MI_U32((0x00000000u+f->r[6]));b=0x00000000u;t=mi_alu(f,a,b,32,1);
    /* 00403623 */
    if(!mi_signed_less(f->flags))goto mi_0040362a;
    /* 00403625 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 00403628 */
    goto mi_00403657;
mi_0040362a:;
    /* 0040362a */
    f->r[2]=0x00000001u;
    /* 0040362f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00403631 */
    if(!(f->flags&MI_ZF))goto mi_00403642;
    /* 00403633 */
    f->r[0]=MI_U32((0x00605964u));
    /* 00403638 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0060597cu)))&0xffu)<<0);
    /* 0040363e */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 00403640 */
    goto mi_0040365f;
mi_00403642:;
    /* 00403642 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00403645 */
    if(!(f->flags&MI_ZF))goto mi_00403652;
    /* 00403647 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0060597cu)))&0xffffu)<<0);
    /* 0040364d */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00403650 */
    goto mi_0040365f;
mi_00403652:;
    /* 00403652 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00403655 */
    if(!(f->flags&MI_ZF))goto mi_00403664;
mi_00403657:;
    /* 00403657 */
    f->r[1]=MI_U32((0x0060597cu));
    /* 0040365d */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[1]);
mi_0040365f:;
    /* 0040365f */
    f->r[0]=MI_U32((0x0060596cu));
mi_00403664:;
    /* 00403664 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00403667 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00403668 */
    if(!(f->flags&MI_ZF))goto mi_00403684;
    /* 0040366a */
    f->r[1]=MI_U32((0x00605964u));
    /* 00403670 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[1])))&0xffu)<<0);
    /* 00403672 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00403674 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004036d3;
    /* 00403676 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00403679 */
    if(!(f->flags&MI_ZF))goto mi_004036d3;
    /* 0040367b */
    a=(f->r[0]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,0,a,8,1);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0040367d */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 0040367f */
    f->r[0]=MI_U32((0x0060596cu));
mi_00403684:;
    /* 00403684 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00403687 */
    if(!(f->flags&MI_ZF))goto mi_004036b0;
    /* 00403689 */
    f->r[1]=MI_U32((0x00605960u));
    /* 0040368f */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000000u+f->r[1])))&0xffffu)<<0);
    /* 00403692 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00403695 */
    if(!mi_signed_less(f->flags))goto mi_0040369f;
    /* 00403697 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0040369a */
    if((f->flags&MI_ZF))goto mi_004036a6;
    /* 0040369c */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
mi_0040369f:;
    /* 0040369f */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004036d3;
    /* 004036a1 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004036a4 */
    if(!(f->flags&MI_ZF))goto mi_004036d3;
mi_004036a6:;
    /* 004036a6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004036a8 */
    MI_U16((0x00000000u+f->r[1]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004036ab */
    f->r[0]=MI_U32((0x0060596cu));
mi_004036b0:;
    /* 004036b0 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004036b3 */
    if(!(f->flags&MI_ZF))goto mi_004036d3;
    /* 004036b5 */
    f->r[1]=MI_U32((0x0060595cu));
    /* 004036bb */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 004036bd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004036bf */
    if(!mi_signed_less(f->flags))goto mi_004036c8;
    /* 004036c1 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004036c4 */
    if((f->flags&MI_ZF))goto mi_004036cf;
    /* 004036c6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
mi_004036c8:;
    /* 004036c8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004036d3;
    /* 004036ca */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004036cd */
    if(!(f->flags&MI_ZF))goto mi_004036d3;
mi_004036cf:;
    /* 004036cf */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 004036d1 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
mi_004036d3:;
    /* 004036d3 */
    a=MI_U32((0x00605968u));b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004036da */
    if(!(f->flags&MI_ZF))goto mi_00403710;
    /* 004036dc */
    f->r[0]=MI_U32((0x00605a14u));
    /* 004036e1 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004036e2 */
    MI_U32((0x00605a14u))=(uint32_t)(f->r[0]);
    /* 004036e7 */
    if((f->flags&MI_SF))goto mi_00403729;
    /* 004036e9 */
    a=f->r[0];b=0x00000021u;t=mi_alu(f,a,b,32,1);
    /* 004036ec */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00403729;
    /* 004036ee */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004036ef */
    MI_U32((0x00bca2b8u))=(uint32_t)(f->r[0]);
    /* 004036f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004036f9u;f->boundary(0x004036f4u,0x004de160u,f);
    /* 004036f9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004036fc */
    MI_U32((0x00bca2bcu))=(uint32_t)(0x0000006du);
    /* 00403706 */
    MI_U32((0x00bca2c0u))=(uint32_t)(0x000000dau);
mi_00403710:;
    /* 00403710 */
    a=MI_U32((0x00605968u));b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00403717 */
    if(!(f->flags&MI_ZF))goto mi_00403729;
    /* 00403719 */
    f->r[2]=MI_U32((0x00605a10u));
    /* 0040371f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00403720 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00403721 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00403726u;f->boundary(0x00403721u,0x00444270u,f);
    /* 00403726 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00403729:;
    /* 00403729 */
    f->r[0]=MI_U32((0x00605a4cu));
    /* 0040372e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00403730 */
    if((f->flags&MI_ZF))goto mi_00403750;
    /* 00403732 */
    p=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=0x00403734u;f->boundary(0x00403732u,p,f);
    /* 00403734 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00605980u)))&0xffu)<<0);
    /* 00403739 */
    MI_U32((0x00605a4cu))=(uint32_t)(0x00000000u);
    /* 00403743 */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[0]&0xffu));
    /* 00403748 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040374f */
    f->r[4]+=4;return;
mi_00403750:;
    /* 00403750 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00605980u)))&0xffu)<<0);
    /* 00403756 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040375d */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[1]&0xffu));
    /* 00403763 */
    f->r[4]+=4;return;
}

void restore_numeric_C_00403770(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00403770 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00605980u)))&0xffu)<<0);
    /* 00403775 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 0040377c */
    MI_U8((0x00e38ddau))=(uint8_t)((f->r[0]&0xffu));
    /* 00403781 */
    f->r[4]+=4;return;
}

void restore_numeric_C_00403790(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00403790 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00403794 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00403798 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 0040379c */
    MI_U32((0x00605974u))=(uint32_t)(f->r[0]);
    /* 004037a1 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004037a5 */
    MI_U32((0x00605970u))=(uint32_t)(f->r[1]);
    /* 004037ab */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004037af */
    MI_U32((0x00605968u))=(uint32_t)(f->r[0]);
    /* 004037b4 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 004037b8 */
    MI_U32((0x00605964u))=(uint32_t)(f->r[1]);
    /* 004037be */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004037c0 */
    MI_U32((0x0060596cu))=(uint32_t)(f->r[2]);
    /* 004037c6 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 004037ca */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004037d0 */
    MI_U32((0x0060595cu))=(uint32_t)(f->r[0]);
    /* 004037d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004037d7 */
    MI_U32((0x00605960u))=(uint32_t)(f->r[2]);
    /* 004037dd */
    MI_U32((0x00605a4cu))=(uint32_t)(f->r[0]);
    /* 004037e2 */
    MI_U32((0x00605980u))=(uint32_t)(f->r[1]);
    /* 004037e8 */
    MI_U32((0x0060597cu))=(uint32_t)(f->r[0]);
    /* 004037ed */
    MI_U32((0x00605978u))=(uint32_t)(f->r[0]);
    /* 004037f2 */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000000eu);
    /* 004037f9 */
    f->r[4]+=4;return;
}

void restore_numeric_C_00403800(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00403800 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00403804 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00403808 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0040380c */
    MI_U32((0x0060595cu))=(uint32_t)(f->r[2]);
    /* 00403812 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00403814 */
    MI_U32((0x00605974u))=(uint32_t)(f->r[0]);
    /* 00403819 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 0040381f */
    MI_U32((0x00605970u))=(uint32_t)(f->r[1]);
    /* 00403825 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00403829 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040382b */
    MI_U32((0x0060596cu))=(uint32_t)(0x00000004u);
    /* 00403835 */
    MI_U32((0x00605968u))=(uint32_t)(f->r[0]);
    /* 0040383a */
    MI_U32((0x00605964u))=(uint32_t)(f->r[0]);
    /* 0040383f */
    MI_U32((0x00605960u))=(uint32_t)(f->r[0]);
    /* 00403844 */
    MI_U32((0x00605a4cu))=(uint32_t)(f->r[1]);
    /* 0040384a */
    MI_U32((0x00605980u))=(uint32_t)(f->r[2]);
    /* 00403850 */
    MI_U32((0x0060597cu))=(uint32_t)(f->r[0]);
    /* 00403855 */
    MI_U32((0x00605978u))=(uint32_t)(f->r[0]);
    /* 0040385a */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000000eu);
    /* 00403861 */
    f->r[4]+=4;return;
}

void restore_numeric_C_00403870(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00403870 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00403874 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00403878 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0040387c */
    MI_U32((0x00605960u))=(uint32_t)(f->r[2]);
    /* 00403882 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00403884 */
    MI_U32((0x00605974u))=(uint32_t)(f->r[0]);
    /* 00403889 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 0040388f */
    MI_U32((0x00605970u))=(uint32_t)(f->r[1]);
    /* 00403895 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00403899 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040389b */
    MI_U32((0x0060596cu))=(uint32_t)(0x00000002u);
    /* 004038a5 */
    MI_U32((0x00605968u))=(uint32_t)(f->r[0]);
    /* 004038aa */
    MI_U32((0x00605964u))=(uint32_t)(f->r[0]);
    /* 004038af */
    MI_U32((0x0060595cu))=(uint32_t)(f->r[0]);
    /* 004038b4 */
    MI_U32((0x00605a4cu))=(uint32_t)(f->r[1]);
    /* 004038ba */
    MI_U32((0x00605980u))=(uint32_t)(f->r[2]);
    /* 004038c0 */
    MI_U32((0x0060597cu))=(uint32_t)(f->r[0]);
    /* 004038c5 */
    MI_U32((0x00605978u))=(uint32_t)(f->r[0]);
    /* 004038ca */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000000eu);
    /* 004038d1 */
    f->r[4]+=4;return;
}

void restore_numeric_C_004038e0(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004038e0 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 004038e4 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004038e8 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 004038ec */
    MI_U32((0x00605964u))=(uint32_t)(f->r[2]);
    /* 004038f2 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004038f4 */
    MI_U32((0x00605974u))=(uint32_t)(f->r[0]);
    /* 004038f9 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00e38ddau)))&0xffu)<<0);
    /* 004038ff */
    MI_U32((0x00605970u))=(uint32_t)(f->r[1]);
    /* 00403905 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00403909 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040390b */
    MI_U32((0x0060596cu))=(uint32_t)(0x00000001u);
    /* 00403915 */
    MI_U32((0x00605968u))=(uint32_t)(f->r[0]);
    /* 0040391a */
    MI_U32((0x00605960u))=(uint32_t)(f->r[0]);
    /* 0040391f */
    MI_U32((0x0060595cu))=(uint32_t)(f->r[0]);
    /* 00403924 */
    MI_U32((0x00605a4cu))=(uint32_t)(f->r[1]);
    /* 0040392a */
    MI_U32((0x00605980u))=(uint32_t)(f->r[2]);
    /* 00403930 */
    MI_U32((0x0060597cu))=(uint32_t)(f->r[0]);
    /* 00403935 */
    MI_U32((0x00605978u))=(uint32_t)(f->r[0]);
    /* 0040393a */
    MI_U8((0x00e38ddau))=(uint8_t)(0x0000000eu);
    /* 00403941 */
    f->r[4]+=4;return;
}

void restore_numeric_C_004cede0(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004cede0 */
    f->r[1]=MI_U32((0x00000004u+f->r[4]));
    /* 004cede4 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004cede7 */
    f->r[0]=(0x00000000u+f->r[4]);
    /* 004cedeb */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ceded */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cedee */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cedef */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cedf4u;f->boundary(0x004cedefu,0x004cbea0u,f);
    /* 004cedf4 */
    f->r[0]=MI_U32((0x00000016u+f->r[4]));
    /* 004cedf8 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cedfb */
    f->r[4]+=4;return;
}

void restore_numeric_C_004cee00(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004cee00 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004cee03 */
    f->r[0]=(0x00000000u+f->r[4]);
    /* 004cee07 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee08 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee09 */
    f->r[6]=MI_U32((0x00000044u+f->r[4]));
    /* 004cee0d */
    a=0x0000000eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee0f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee10 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee11 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cee16u;f->boundary(0x004cee11u,0x004cbea0u,f);
    /* 004cee16 */
    f->r[1]=(0x00000024u+f->r[4]);
    /* 004cee1a */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee1c */
    f->r[2]=(0x0000000eu+f->r[6]);
    /* 004cee1f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee20 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee21 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cee26u;f->boundary(0x004cee21u,0x004cbea0u,f);
    /* 004cee26 */
    f->r[0]=MI_U32((0x00000034u+f->r[4]));
    /* 004cee2a */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
    /* 004cee2e */
    MI_U32((0x00e92e3cu))=(uint32_t)(f->r[0]);
    /* 004cee33 */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 004cee37 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee38 */
    MI_U32((0x00e92e40u))=(uint32_t)(f->r[1]);
    /* 004cee3e */
    MI_U32((0x00e92e44u))=(uint32_t)(f->r[0]);
    /* 004cee43 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004cee48u;f->boundary(0x004cee43u,0x00561b68u,f);
    /* 004cee48 */
    f->r[3]=f->r[0];
    /* 004cee4a */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cee4d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004cee4f */
    if(!(f->flags&MI_ZF))goto mi_004cee57;
    /* 004cee51 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004cee52 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004cee53 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004cee56 */
    f->r[4]+=4;return;
mi_004cee57:;
    /* 004cee57 */
    f->r[2]=MI_U32((0x00000012u+f->r[4]));
    /* 004cee5b */
    f->r[0]=MI_U32((0x00e92e44u));
    /* 004cee60 */
    f->r[1]=MI_U32((0x00e92e40u));
    /* 004cee66 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cee68 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee69 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004cee6b */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004cee6d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004cee6e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004cee70 */
    f->r[7]=f->r[3];
    /* 004cee72 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004ceea7;
    /* 004cee74 */
    f->r[2]=MI_U32((0x00e92e3cu));
mi_004cee7a:;
    /* 004cee7a */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 004cee7c */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004cee7e */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004cee96;
    /* 004cee80 */
    f->r[1]=(0xffffffffu+f->r[2]+f->r[7]*1u);
mi_004cee84:;
    /* 004cee84 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0xffffffffu+f->r[0])))&0xffu)<<0);
    /* 004cee87 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004cee88 */
    MI_U8((0x00000000u+f->r[1]))=(uint8_t)((f->r[2]&0xffu));
    /* 004cee8a */
    f->r[2]=MI_U32((0x00e92e3cu));
    /* 004cee90 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004cee91 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004cee92 */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 004cee94 */
    if(mi_signed_less(f->flags))goto mi_004cee84;
mi_004cee96:;
    /* 004cee96 */
    f->r[1]=MI_U32((0x00e92e40u));
    /* 004cee9c */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004cee9e */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004cee9f */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004ceea1 */
    if(mi_signed_less(f->flags))goto mi_004cee7a;
    /* 004ceea3 */
    f->r[6]=MI_U32((0x0000004cu+f->r[4]));
mi_004ceea7:;
    /* 004ceea7 */
    f->r[1]=MI_U32((0x0000001au+f->r[4]));
    /* 004ceeab */
    f->r[0]=MI_U32((0x00e92e44u));
    /* 004ceeb0 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004ceeb2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ceeb3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ceeb4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ceeb5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ceebau;f->boundary(0x004ceeb5u,0x004cbea0u,f);
    /* 004ceeba */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ceebb */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ceec0u;f->boundary(0x004ceebbu,0x00561bdcu,f);
    /* 004ceec0 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ceec3 */
    f->r[0]=0x00000001u;
    /* 004ceec8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ceec9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004ceeca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ceecb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004ceecc */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ceecf */
    f->r[4]+=4;return;
}

void restore_numeric_C_004de160(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004de160 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de161 */
    f->r[6]=MI_U32((0x00000008u+f->r[4]));
    /* 004de165 */
    a=0x00000054u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de167 */
    a=0x00bca2b8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de16c */
    f->r[0]=(0x00000000u+f->r[6]*8u);
    /* 004de173 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004de175 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004de178 */
    f->r[1]=(0x005e1e28u+f->r[0]*4u);
    /* 004de17f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de180 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de185u;f->boundary(0x004de180u,0x004cbea0u,f);
    /* 004de185 */
    a=0x0016e360u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de18a */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de18fu;f->boundary(0x004de18au,0x00561b68u,f);
    /* 004de18f */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004de192 */
    MI_U32((0x00e94dd8u))=(uint32_t)(f->r[0]);
    /* 004de197 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004de199 */
    if((f->flags&MI_ZF))goto mi_004de1fb;
    /* 004de19b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de19d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de19e */
    a=0x005e2ac4u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de1a3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1a8u;f->boundary(0x004de1a3u,0x004ceb60u,f);
    /* 004de1a8 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004de1ab */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004de1ad */
    if(!(f->flags&MI_ZF))goto mi_004de1c0;
    /* 004de1af */
    f->r[2]=MI_U32((0x00e94dd8u));
    /* 004de1b5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de1b6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1bbu;f->boundary(0x004de1b6u,0x00561bdcu,f);
    /* 004de1bb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004de1be */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004de1bf */
    f->r[4]+=4;return;
mi_004de1c0:;
    /* 004de1c0 */
    f->r[0]=MI_U32((0x00e94dd8u));
    /* 004de1c5 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de1c6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1cbu;f->boundary(0x004de1c6u,0x004cede0u,f);
    /* 004de1cb */
    f->r[1]=MI_U32((0x00e94dd8u));
    /* 004de1d1 */
    MI_U32((0x00e94dd4u))=(uint32_t)(f->r[0]);
    /* 004de1d6 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de1d7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1dcu;f->boundary(0x004de1d7u,0x004cee00u,f);
    /* 004de1dc */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1e1u;f->boundary(0x004de1dcu,0x0046fa40u,f);
    /* 004de1e1 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de1e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1e7u;f->boundary(0x004de1e2u,0x004de200u,f);
    /* 004de1e7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1ecu;f->boundary(0x004de1e7u,0x004dea50u,f);
    /* 004de1ec */
    f->r[2]=MI_U32((0x00e94dd8u));
    /* 004de1f2 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de1f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004de1f8u;f->boundary(0x004de1f3u,0x00561bdcu,f);
    /* 004de1f8 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004de1fb:;
    /* 004de1fb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004de1fc */
    f->r[4]+=4;return;
}

void restore_numeric_C_004de200(struct restore_numeric_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004de200 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004de204 */
    f->r[2]=MI_U32((0x00bca2d4u));
    /* 004de20a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de20b */
    f->r[7]=MI_U32((0x00bca2d8u));
    /* 004de211 */
    f->r[1]=MI_U32((0x005e2950u+f->r[0]*8u));
    /* 004de218 */
    f->r[0]=MI_U32((0x005e2954u+f->r[0]*8u));
    /* 004de21f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 004de221 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004de2b9;
    /* 004de227 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004de22a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de22b */
    f->r[3]=MI_U32((0x00e94dd8u));
    /* 004de231 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de232 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004de235 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004de236 */
    f->r[6]=MI_U32((0x00e94dd4u));
    /* 004de23c */
    f->r[5]=MI_U32((0x00bca2d0u));
    /* 004de242 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 004de245 */
    f->r[6]=(0x00000000u+f->r[6]+f->r[0]*8u);
    /* 004de248 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004de24a */
    f->r[3]=f->r[2];
    /* 004de24c */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_004de24e:;
    /* 004de24e */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004de250 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 004de252 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004de2a5;
    /* 004de254 */
    f->r[1]=(0x00af598cu+f->r[7]*4u);
mi_004de25b:;
    /* 004de25b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004de25d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[6]+f->r[2]*1u)))&0xffu)<<0);
    /* 004de260 */
    a=f->r[0];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 004de263 */
    if(!(f->flags&MI_ZF))goto mi_004de26d;
    /* 004de265 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(0x00000002u);
    /* 004de26b */
    goto mi_004de29c;
mi_004de26d:;
    /* 004de26d */
    a=f->r[0];b=0x0000000bu;t=mi_alu(f,a,b,32,1);
    /* 004de270 */
    if(!(f->flags&MI_ZF))goto mi_004de27a;
    /* 004de272 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(0x00000001u);
    /* 004de278 */
    goto mi_004de29c;
mi_004de27a:;
    /* 004de27a */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 004de27d */
    if(!(f->flags&MI_ZF))goto mi_004de283;
    /* 004de27f */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
    /* 004de281 */
    goto mi_004de29c;
mi_004de283:;
    /* 004de283 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004de286 */
    if(!(f->flags&MI_ZF))goto mi_004de290;
    /* 004de288 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(0x00000040u);
    /* 004de28e */
    goto mi_004de29c;
mi_004de290:;
    /* 004de290 */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 004de293 */
    if(!(f->flags&MI_ZF))goto mi_004de29c;
    /* 004de295 */
    a=MI_U8((0x00b285ccu+f->r[7]));b=0x00000020u;t=mi_alu(f,a,b,8,4);MI_U8((0x00b285ccu+f->r[7]))=(uint8_t)(t);
mi_004de29c:;
    /* 004de29c */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004de29d */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004de29e */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004de2a1 */
    a=f->r[2];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 004de2a3 */
    if(mi_signed_less(f->flags))goto mi_004de25b;
mi_004de2a5:;
    /* 004de2a5 */
    f->r[1]=MI_U32((0x00bca2dcu));
    /* 004de2ab */
    a=f->r[6];b=0x000003e8u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004de2b1 */
    a=f->r[7];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004de2b3 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004de2b4 */
    if(!(f->flags&MI_ZF))goto mi_004de24e;
    /* 004de2b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004de2b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004de2b8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_004de2b9:;
    /* 004de2b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004de2ba */
    f->r[4]+=4;return;
}
