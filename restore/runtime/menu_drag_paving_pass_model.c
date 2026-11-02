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

void restore_C_00475720(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00475720 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475721 */
    f->r[0]=MI_U32((0x00000008u+f->r[4]));
    /* 00475725 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00475729 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047572b */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00475735;
    /* 0047572d */
    MI_U32((0x00000008u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00475731 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00475735:;
    /* 00475735 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00475739 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047573a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047573b */
    f->r[5]=MI_U32((0x00000014u+f->r[4]));
    /* 0047573f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475740 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00475742 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475743 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047574d;
    /* 00475745 */
    f->r[0]=f->r[5];
    /* 00475747 */
    f->r[5]=f->r[1];
    /* 00475749 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0047574d:;
    /* 0047574d */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00475751 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00475753 */
    if(!mi_signed_less(f->flags))goto mi_0047575d;
    /* 00475755 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
mi_0047575d:;
    /* 0047575d */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 0047575f */
    if(!mi_signed_less(f->flags))goto mi_00475763;
    /* 00475761 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_00475763:;
    /* 00475763 */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 00475769 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 0047576d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 0047576f */
    if(mi_signed_less(f->flags))goto mi_00475778;
    /* 00475771 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 00475774 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
mi_00475778:;
    /* 00475778 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0047577d */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00475781 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00475783 */
    if(mi_signed_less(f->flags))goto mi_0047578a;
    /* 00475785 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00475786 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
mi_0047578a:;
    /* 0047578a */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 0047578e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00475790 */
    if((f->flags&MI_ZF))goto mi_0047588a;
    /* 00475796 */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 0047579a */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 0047579e */
    f->r[0]=f->r[2];
    /* 004757a0 */
    f->r[6]=MI_U32((0x00bca2d8u));
    /* 004757a6 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004757a8 */
    f->r[0]=(0xffffffffu+f->r[0]+f->r[1]*1u);
    /* 004757ac */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004757b0 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 004757b4 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[0]*2u);
    /* 004757b8 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004757bb */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*4u);
    /* 004757be */
    f->r[6]=MI_U32((0x00000024u+f->r[4]));
    /* 004757c2 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004757c4 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004757c6 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00475892;
    /* 004757cc */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004757ce */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004757cf */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
mi_004757d3:;
    /* 004757d3 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004757d5 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_0047586b;
    /* 004757db */
    f->r[2]=(0x00b4e6fcu+f->r[0]*2u);
    /* 004757e2 */
    f->r[6]=f->r[7];
    /* 004757e4 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004757e8 */
    f->r[2]=0x00b67d1cu;
    /* 004757ed */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004757ef */
    a=f->r[2];b=0x00b285ccu;t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004757f5 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
mi_004757f6:;
    /* 004757f6 */
    a=MI_U8((0x00af598cu+f->r[0]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004757fe */
    if((f->flags&MI_ZF))goto mi_0047584e;
    /* 00475800 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 00475806 */
    a=(f->r[1]&0xffu);b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 00475809 */
    if((f->flags&MI_ZF))goto mi_0047584e;
    /* 0047580b */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 0047580f */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[7]));
    /* 00475812 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00475814 */
    if((f->flags&MI_ZF))goto mi_00475829;
    /* 00475816 */
    f->r[3]=f->r[7];
    /* 00475818 */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 0047581b */
    a=f->r[3];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 0047581d */
    a=MI_U16((0x007999e8u+f->r[3]*8u));b=0x0000008au;t=mi_alu(f,a,b,16,1);
    /* 00475827 */
    if((f->flags&MI_ZF))goto mi_0047584e;
mi_00475829:;
    /* 00475829 */
    a=(f->r[1]&0xffu);b=0x000000f0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047582c */
    MI_U32((0x00b7482cu+f->r[0]*4u))=(uint32_t)(0x00000000u);
    /* 00475837 */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047583d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[2]+f->r[0]*1u)))&0xffu)<<0);
    /* 00475844 */
    a=(f->r[1]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00475847 */
    MI_U8((0x00b285ccu+f->r[2]+f->r[0]*1u))=(uint8_t)((f->r[1]&0xffu));
mi_0047584e:;
    /* 0047584e */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 00475852 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00475853 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00475856 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00475857 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 0047585b */
    if(!(f->flags&MI_ZF))goto mi_004757f6;
    /* 0047585d */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 00475861 */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 00475867 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
mi_0047586b:;
    /* 0047586b */
    f->r[6]=MI_U32((0x00bca2dcu));
    /* 00475871 */
    f->r[3]=MI_U32((0x00000028u+f->r[4]));
    /* 00475875 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00475877 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00475879 */
    f->r[6]=MI_U32((0x00000010u+f->r[4]));
    /* 0047587d */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047587e */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00475882 */
    if(!(f->flags&MI_ZF))goto mi_004757d3;
    /* 00475888 */
    goto mi_00475892;
mi_0047588a:;
    /* 0047588a */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 0047588e */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
mi_00475892:;
    /* 00475892 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00475894 */
    f->r[3]=f->r[5];
    /* 00475896 */
    f->r[0]=(0xffffffffu+f->r[2]+f->r[1]*1u);
    /* 0047589a */
    f->r[2]=(0x00000000u+f->r[5]+f->r[5]*8u);
    /* 0047589e */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004758a2 */
    f->r[0]=(0x00000000u+f->r[5]+f->r[2]*2u);
    /* 004758a6 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004758ac */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004758af */
    f->r[6]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 004758b2 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004758b6 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004758b8 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004758ba */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004759d6;
mi_004758c0:;
    /* 004758c0 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004758c2 */
    f->r[5]=f->r[1];
    /* 004758c4 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004759bc;
    /* 004758ca */
    f->r[1]=0x00b285ccu;
    /* 004758cf */
    f->r[0]=0x00acf85cu;
    /* 004758d4 */
    a=f->r[1];b=0x00acf85cu;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004758da */
    a=f->r[0];b=0x00b67d1cu;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004758df */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004758e3 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004758e7 */
    goto mi_004758f1;
mi_004758e9:;
    /* 004758e9 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 004758ed */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
mi_004758f1:;
    /* 004758f1 */
    a=MI_U8((0x00af598cu+f->r[6]*4u));b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 004758f9 */
    if((f->flags&MI_ZF))goto mi_004759aa;
    /* 004758ff */
    f->r[2]=MI_U32((0x00b7482cu+f->r[6]*4u));
    /* 00475906 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00475908 */
    if(!(f->flags&MI_ZF))goto mi_004759aa;
    /* 0047590e */
    f->r[0]=(0x00b67d1cu+f->r[0]+f->r[6]*1u);
    /* 00475915 */
    a=MI_U8((0x00000000u+f->r[0]+f->r[1]*1u));b=0x00000080u;t=mi_alu(f,a,b,8,3);
    /* 00475919 */
    if((f->flags&MI_ZF))goto mi_004759aa;
    /* 0047591f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 00475921 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475922 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00475925 */
    f->r[7]=f->r[0];
    /* 00475927 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047592cu;f->boundary(0x00475927u,0x00475670u,f);
    /* 0047592c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047592f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00475931 */
    if((f->flags&MI_ZF))goto mi_00475960;
    /* 00475933 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00475935 */
    if(!(f->flags&MI_ZF))goto mi_00475942;
    /* 00475937 */
    f->r[1]=MI_U32((0x00f5695cu));
    /* 0047593d */
    f->r[0]=(0x00000006u+f->r[1]);
    /* 00475940 */
    goto mi_0047594b;
mi_00475942:;
    /* 00475942 */
    f->r[2]=MI_U32((0x00f5695cu));
    /* 00475948 */
    f->r[0]=(0x00000007u+f->r[2]);
mi_0047594b:;
    /* 0047594b */
    a=0x00000040u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047594d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047594e */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475950 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475952 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475953 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475954 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475956 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047595bu;f->boundary(0x00475956u,0x0047bf50u,f);
    /* 0047595b */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047595e */
    goto mi_004759aa;
mi_00475960:;
    /* 00475960 */
    f->r[0]=f->r[5];
    /* 00475962 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 00475965 */
    if((f->flags&MI_ZF))goto mi_0047597b;
    /* 00475967 */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 0047596a */
    if((f->flags&MI_ZF))goto mi_0047597b;
    /* 0047596c */
    f->r[0]=MI_U32((0x00f5695cu));
    /* 00475971 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00475972 */
    MI_U32((0x00b7482cu+f->r[6]*4u))=(uint32_t)(f->r[0]);
    /* 00475979 */
    goto mi_004759a3;
mi_0047597b:;
    /* 0047597b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047597d */
    if(!(f->flags&MI_ZF))goto mi_00475993;
    /* 0047597f */
    a=(f->r[3]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00475982 */
    if(!(f->flags&MI_ZF))goto mi_00475993;
    /* 00475984 */
    f->r[1]=MI_U32((0x00f5695cu));
    /* 0047598a */
    MI_U32((0x00b7482cu+f->r[6]*4u))=(uint32_t)(f->r[1]);
    /* 00475991 */
    goto mi_004759a3;
mi_00475993:;
    /* 00475993 */
    f->r[2]=MI_U32((0x00f5695cu));
    /* 00475999 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047599c */
    MI_U32((0x00b7482cu+f->r[6]*4u))=(uint32_t)(f->r[2]);
mi_004759a3:;
    /* 004759a3 */
    a=MI_U8((0x00b67d1cu+f->r[6]));b=0x00000040u;t=mi_alu(f,a,b,8,4);MI_U8((0x00b67d1cu+f->r[6]))=(uint8_t)(t);
mi_004759aa:;
    /* 004759aa */
    f->r[7]=MI_U32((0x00000020u+f->r[4]));
    /* 004759ae */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 004759af */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004759b0 */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 004759b2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004758e9;
    /* 004759b8 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
mi_004759bc:;
    /* 004759bc */
    f->r[0]=MI_U32((0x00bca2dcu));
    /* 004759c1 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 004759c5 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004759c7 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 004759c8 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004759ca */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004759ce */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004759d0 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004758c0;
mi_004759d6:;
    /* 004759d6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004759d7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004759d8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004759d9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004759da */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[1]=a;
    /* 004759db */
    f->r[4]+=4;return;
}
