/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "small_constructor_model.h"

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

void restore_C_00440850(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00440850 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00440853 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440854 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440855 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440856 */
    f->r[6]=MI_U32((0x00000038u+f->r[4]));
    /* 0044085a */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00440860 */
    f->r[5]=MI_U32((0x00000034u+f->r[4]));
    /* 00440864 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 00440867 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440868 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044086a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044086b */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 0044086e */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044086f */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00440872 */
    f->r[3]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00440875 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00440877 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[3]);
    /* 0044087b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440880u;f->boundary(0x0044087bu,0x004413b0u,f);
    /* 00440880 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440882 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440887u;f->boundary(0x00440882u,0x0043f410u,f);
    /* 00440887 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044088a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044088c */
    if(!(f->flags&MI_ZF))goto mi_00440896;
    /* 0044088e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044088f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440890 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440891 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00440892 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440895 */
    f->r[4]+=4;return;
mi_00440896:;
    /* 00440896 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440898 */
    f->r[0]=(0x00000040u+f->r[4]);
    /* 0044089c */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044089e */
    f->r[1]=(0x00000040u+f->r[4]);
    /* 004408a2 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408a3 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408a4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408a5 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408a6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004408abu;f->boundary(0x004408a6u,0x00441130u,f);
    /* 004408ab */
    f->r[7]=f->r[0];
    /* 004408ad */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004408b0 */
    a=f->r[7];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004408b3 */
    if(!(f->flags&MI_ZF))goto mi_004408bf;
    /* 004408b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004408b6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004408b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004408b8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004408ba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004408bb */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004408be */
    f->r[4]+=4;return;
mi_004408bf:;
    /* 004408bf */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408c1 */
    f->r[2]=(0x0000001cu+f->r[4]);
    /* 004408c5 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408c7 */
    f->r[0]=(0x00000024u+f->r[4]);
    /* 004408cb */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408cc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408cd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408ce */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408cf */
    f->r[4]-=4;MI_U32(f->r[4])=0x004408d4u;f->boundary(0x004408cfu,0x00441130u,f);
    /* 004408d4 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004408d7 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 004408da */
    if(!(f->flags&MI_ZF))goto mi_004408e6;
    /* 004408dc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004408dd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004408de */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004408df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004408e1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004408e2 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004408e5 */
    f->r[4]+=4;return;
mi_004408e6:;
    /* 004408e6 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004408ea */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004408ec */
    if(!(f->flags&MI_ZF))goto mi_00440aff;
    /* 004408f2 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408f7 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004408fc */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440901 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440906u;f->boundary(0x00440901u,0x0040f860u,f);
    /* 00440906 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044090b */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440910 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440915 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044091au;f->boundary(0x00440915u,0x0040f860u,f);
    /* 0044091a */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044091f */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440924 */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440929 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044092eu;f->boundary(0x00440929u,0x0040f860u,f);
    /* 0044092e */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440933 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440938 */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044093d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440942u;f->boundary(0x0044093du,0x0040f860u,f);
    /* 00440942 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440944 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440949u;f->boundary(0x00440944u,0x00485830u,f);
    /* 00440949 */
    f->r[1]=(0x00000048u+f->r[4]);
    /* 0044094d */
    f->r[2]=(0x00000054u+f->r[4]);
    /* 00440951 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440952 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440953 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440954 */
    MI_U32((0x00bc971cu))=(uint32_t)(0x00000001u);
    /* 0044095e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440963u;f->boundary(0x0044095eu,0x004405c0u,f);
    /* 00440963 */
    f->r[0]=MI_U32((0x0000007cu+f->r[4]));
    /* 00440967 */
    f->r[1]=MI_U32((0x00000078u+f->r[4]));
    /* 0044096b */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00440971 */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440974 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440976 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440978 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440979 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044097a */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044097c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044097d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440982u;f->boundary(0x0044097du,0x0046a400u,f);
    /* 00440982 */
    f->r[6]=f->r[0];
    /* 00440984 */
    f->r[0]=MI_U32((0x00000038u+f->r[4]));
    /* 00440988 */
    f->r[1]=MI_U32((0x00000054u+f->r[4]));
    /* 0044098c */
    f->r[2]=MI_U32((0x00000050u+f->r[4]));
    /* 00440990 */
    f->r[5]=f->r[6];
    /* 00440992 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440994 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00440997 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00440999 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044099a */
    a=f->r[5];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 0044099d */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044099f */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409a1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409a2 */
    MI_U32((0x00799ab0u+f->r[5]))=(uint32_t)(0x00000001u);
    /* 004409ac */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409ad */
    MI_U32((0x00799aacu+f->r[5]))=(uint32_t)(f->r[7]);
    /* 004409b3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409b4 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004409b8 */
    MI_U32((0x00799aa8u+f->r[5]))=(uint32_t)(f->r[3]);
    /* 004409be */
    f->r[4]-=4;MI_U32(f->r[4])=0x004409c3u;f->boundary(0x004409beu,0x0047bf50u,f);
    /* 004409c3 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409c4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004409c9u;f->boundary(0x004409c4u,0x00485100u,f);
    /* 004409c9 */
    f->r[0]=MI_U32((0x00000074u+f->r[4]));
    /* 004409cd */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004409d0 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 004409d2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409d4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409d6 */
    if(!(f->flags&MI_ZF))goto mi_00440a06;
    /* 004409d8 */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 004409dc */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 004409e2 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004409e3 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409e4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409e5 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409e7 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409e8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004409edu;f->boundary(0x004409e8u,0x0046a400u,f);
    /* 004409ed */
    f->r[1]=MI_U32((0x00000054u+f->r[4]));
    /* 004409f1 */
    f->r[2]=MI_U32((0x00000050u+f->r[4]));
    /* 004409f5 */
    f->r[3]=f->r[0];
    /* 004409f7 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 004409fb */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409fd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004409fe */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a00 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00440a01 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a03 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a04 */
    goto mi_00440a32;
mi_00440a06:;
    /* 00440a06 */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 00440a0a */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00440a10 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00440a11 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a12 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a13 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a15 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a16 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440a1bu;f->boundary(0x00440a16u,0x0046a400u,f);
    /* 00440a1b */
    f->r[1]=MI_U32((0x00000054u+f->r[4]));
    /* 00440a1f */
    f->r[2]=MI_U32((0x00000050u+f->r[4]));
    /* 00440a23 */
    f->r[3]=f->r[0];
    /* 00440a25 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 00440a29 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a2b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a2c */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a2e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a30 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a31 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
mi_00440a32:;
    /* 00440a32 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a33 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a34 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440a39u;f->boundary(0x00440a34u,0x0047bf50u,f);
    /* 00440a39 */
    f->r[0]=MI_U32((0x00000044u+f->r[4]));
    /* 00440a3d */
    f->r[6]=f->r[3];
    /* 00440a3f */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00440a42 */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00440a44 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440a47 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00440a4a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a4b */
    MI_U32((0x00799aa8u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00440a51 */
    MI_U32((0x00799aacu+f->r[5]))=(uint32_t)(f->r[7]);
    /* 00440a57 */
    MI_U32((0x00799ab0u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 00440a61 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440a66u;f->boundary(0x00440a61u,0x00485100u,f);
    /* 00440a66 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
    /* 00440a6a */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00440a6e */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00440a73 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a75 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a77 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a78 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a79 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a7b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a7c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440a81u;f->boundary(0x00440a7cu,0x0046a400u,f);
    /* 00440a81 */
    f->r[7]=f->r[0];
    /* 00440a83 */
    f->r[1]=MI_U32((0x0000002cu+f->r[4]));
    /* 00440a87 */
    f->r[2]=MI_U32((0x00f567acu));
    /* 00440a8d */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a8f */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00440a92 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00440a94 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a95 */
    f->r[2]=MI_U32((0x00000040u+f->r[4]));
    /* 00440a99 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440a9b */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00440a9e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440aa0 */
    MI_U32((0x00000058u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00440aa4 */
    MI_U32((0x00799ab0u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 00440aae */
    MI_U32((0x00799aa8u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 00440ab4 */
    f->r[1]=MI_U32((0x00000044u+f->r[4]));
    /* 00440ab8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ab9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440aba */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440abb */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440ac0u;f->boundary(0x00440abbu,0x0047bf50u,f);
    /* 00440ac0 */
    f->r[0]=MI_U32((0x00000064u+f->r[4]));
    /* 00440ac4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ac5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440acau;f->boundary(0x00440ac5u,0x00485100u,f);
    /* 00440aca */
    f->r[0]=MI_U32((0x00000060u+f->r[4]));
    /* 00440ace */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000068u+f->r[4])))&0xffffu)<<0);
    /* 00440ad3 */
    f->r[2]=MI_U32((0x0000004cu+f->r[4]));
    /* 00440ad7 */
    MI_U16((0x00799a12u+f->r[5]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00440ade */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440adf */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00440ae6 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ae8 */
    MI_U16((0x00799a12u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00440aef */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440af0 */
    MI_U16((0x00799a10u+f->r[7]))=(uint16_t)((f->r[3]&0xffffu));
    /* 00440af7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440afcu;f->boundary(0x00440af7u,0x00440060u,f);
    /* 00440afc */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00440aff:;
    /* 00440aff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440b00 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440b01 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440b02 */
    f->r[0]=0x00000001u;
    /* 00440b07 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00440b08 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440b0b */
    f->r[4]+=4;return;
}
