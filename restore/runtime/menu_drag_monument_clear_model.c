/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "monument_clear_model.h"

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

void restore_C_004c0920(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004c0920 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004c0923 */
    f->r[0]=MI_U32((0x00000038u+f->r[4]));
    /* 004c0927 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0928 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0929 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c092a */
    f->r[6]=f->r[0];
    /* 004c092c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c092d */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c0930 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c0932 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004c0934 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004c0937 */
    MI_U32((0x00000040u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c093b */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a82u+f->r[6])))&0xffu)<<0);
    /* 004c0941 */
    MI_U32((0x0000003cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0945 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 004c0946 */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999deu+f->r[6]));
    /* 004c094d */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999e0u+f->r[6]));
    /* 004c0954 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c0956 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 004c0958 */
    a=f->r[0];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[0]=t;
    /* 004c095b */
    f->r[2]=MI_U32((0x005e00ccu+f->r[0]));
    /* 004c0961 */
    f->r[1]=MI_U32((0x005e00d0u+f->r[0]));
    /* 004c0967 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 004c0969 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 004c096b */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[3]);
    /* 004c096f */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 004c0973 */
    f->r[0]=0x005df9b0u;
mi_004c0978:;
    /* 004c0978 */
    f->r[2]=f->r[5];
    /* 004c097a */
    f->r[6]=f->r[3];
    /* 004c097c */
    a=f->r[2];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 004c097e */
    f->r[7]=f->r[5];
    /* 004c0980 */
    f->r[1]=f->r[3];
    /* 004c0982 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004c0984 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0988 */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c098c */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0990 */
    MI_U32((0x00000034u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c0994 */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c0998 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(0x0000000du);
    /* 004c09a0 */
    goto mi_004c09aa;
mi_004c09a2:;
    /* 004c09a2 */
    f->r[2]=MI_U32((0x00000034u+f->r[4]));
    /* 004c09a6 */
    f->r[6]=MI_U32((0x00000038u+f->r[4]));
mi_004c09aa:;
    /* 004c09aa */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 004c09ae */
    a=f->r[0];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c09b1 */
    if((f->flags&MI_ZF))goto mi_004c0a10;
    /* 004c09b3 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c09b6 */
    if((f->flags&MI_ZF))goto mi_004c09f6;
    /* 004c09b8 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004c09bb */
    if((f->flags&MI_ZF))goto mi_004c09d9;
    /* 004c09bd */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 004c09c1 */
    f->r[1]=(0x00000007u+f->r[5]);
    /* 004c09c4 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004c09c6 */
    f->r[2]=(0x0000000du+f->r[3]);
    /* 004c09c9 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c09cd */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c09cf */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c09d3 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c09d7 */
    goto mi_004c0a26;
mi_004c09d9:;
    /* 004c09d9 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004c09dd */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c09df */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c09e3 */
    f->r[1]=(0xfffffff3u+f->r[5]);
    /* 004c09e6 */
    f->r[2]=(0xfffffff9u+f->r[3]);
    /* 004c09e9 */
    f->r[7]=(0x00000000u+f->r[0]+f->r[6]*1u);
    /* 004c09ec */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c09f0 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c09f4 */
    goto mi_004c0a26;
mi_004c09f6:;
    /* 004c09f6 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004c09fa */
    f->r[7]=f->r[1];
    /* 004c09fc */
    f->r[1]=(0xfffffff9u+f->r[5]);
    /* 004c09ff */
    f->r[2]=(0xfffffff3u+f->r[3]);
    /* 004c0a02 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0a06 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0a0a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c0a0e */
    goto mi_004c0a26;
mi_004c0a10:;
    /* 004c0a10 */
    f->r[0]=(0x0000000du+f->r[5]);
    /* 004c0a13 */
    f->r[1]=(0x00000007u+f->r[3]);
    /* 004c0a16 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0a1a */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
    /* 004c0a1e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0a22 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[1]);
mi_004c0a26:;
    /* 004c0a26 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004c0a2c */
    f->r[2]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 004c0a2f */
    f->r[0]=(0x00000000u+f->r[7]+f->r[2]*2u);
    /* 004c0a32 */
    f->r[2]=MI_U32((0x0000001cu+f->r[4]));
    /* 004c0a36 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004c0a39 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*4u);
    /* 004c0a3c */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0a40 */
    f->r[0]=MI_U32((0x00000000u+f->r[2]));
    /* 004c0a42 */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004c0a44 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004c0a46 */
    if((f->flags&MI_ZF))goto mi_004c0b20;
    /* 004c0a4c */
    f->r[1]=MI_U32((0x00000048u+f->r[4]));
    /* 004c0a50 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 004c0a52 */
    if((f->flags&MI_ZF))goto mi_004c0a8f;
    /* 004c0a54 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00b4e6fcu+f->r[6]*2u));
    /* 004c0a5c */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004c0a5e */
    if((f->flags&MI_ZF))goto mi_004c0a75;
    /* 004c0a60 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0a61 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0a66u;f->boundary(0x004c0a61u,0x00475b50u,f);
    /* 004c0a66 */
    f->r[1]=MI_U32((0x0000004cu+f->r[4]));
    /* 004c0a6a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0a6d */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004c0a6f */
    if(!(f->flags&MI_ZF))goto mi_004c0b20;
mi_004c0a75:;
    /* 004c0a75 */
    f->r[0]=MI_U32((0x00000040u+f->r[4]));
    /* 004c0a79 */
    a=MI_U16((0x007999e8u+f->r[0]));b=0x00000063u;t=mi_alu(f,a,b,16,1);
    /* 004c0a81 */
    if((f->flags&MI_ZF))goto mi_004c0a8f;
    /* 004c0a83 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999e8u+f->r[0])))&0xffu)<<0);
    /* 004c0a89 */
    MI_U8((0x00a9010cu+f->r[6]))=(uint8_t)((f->r[0]&0xffu));
mi_004c0a8f:;
    /* 004c0a8f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[6])))&0xffu)<<0);
    /* 004c0a95 */
    f->r[0]=(0x00af598cu+f->r[6]*4u);
    /* 004c0a9c */
    a=(f->r[1]&0xffu);b=0x000000e0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c0a9f */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 004c0aa1 */
    MI_U8((0x00b285ccu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004c0aa7 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[6])))&0xffu)<<0);
    /* 004c0aad */
    a=(f->r[1]&0xffu);b=0x000000c0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c0ab0 */
    MI_U16((0x00b4e6fcu+f->r[6]*2u))=(uint16_t)((f->r[2]&0xffffu));
    /* 004c0ab8 */
    a=(f->r[1]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004c0abb */
    MI_U16((0x00a9cc1cu+f->r[6]*2u))=(uint16_t)((f->r[2]&0xffffu));
    /* 004c0ac3 */
    MI_U8((0x00b67d1cu+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 004c0ac9 */
    f->r[1]=MI_U32((0x00000000u+f->r[0]));
    /* 004c0acb */
    a=f->r[1];b=0xfffffff7u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004c0ace */
    MI_U8((0x00adc36cu+f->r[6]))=(uint8_t)((f->r[2]&0xffu));
    /* 004c0ad4 */
    f->r[2]=f->r[1];
    /* 004c0ad6 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 004c0ad8 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004c0adb */
    MI_U8((0x00ae8e7cu+f->r[6]))=(uint8_t)(0x00000000u);
    /* 004c0ae2 */
    if((f->flags&MI_ZF))goto mi_004c0b00;
    /* 004c0ae4 */
    a=f->r[1];b=0x00010000u;t=mi_alu(f,a,b,32,3);
    /* 004c0aea */
    if(!(f->flags&MI_ZF))goto mi_004c0b00;
    /* 004c0aec */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 004c0aee */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
    /* 004c0af2 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0af4 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0af5 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0af6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0afbu;f->boundary(0x004c0af6u,0x0047a220u,f);
    /* 004c0afb */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0afe */
    goto mi_004c0b20;
mi_004c0b00:;
    /* 004c0b00 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00acf85cu+f->r[6])))&0xffu)<<0);
    /* 004c0b06 */
    f->r[7]=MI_U32((0x00f56b54u));
    /* 004c0b0c */
    a=f->r[2];b=0x00000007u;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 004c0b0f */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004c0b11 */
    a=f->r[1];b=0x03052e84u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 004c0b17 */
    MI_U32((0x00b7482cu+f->r[6]*4u))=(uint32_t)(f->r[2]);
    /* 004c0b1e */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(f->r[1]);
mi_004c0b20:;
    /* 004c0b20 */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004c0b24 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 004c0b28 */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 004c0b2c */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004c0b2f */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0b33 */
    f->r[7]=MI_U32((0x0000002cu+f->r[4]));
    /* 004c0b37 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004c0b38 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004c0b39 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004c0b3a */
    MI_U32((0x00000028u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004c0b3e */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004c0b42 */
    MI_U32((0x00000030u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004c0b46 */
    if(!(f->flags&MI_ZF))goto mi_004c09a2;
    /* 004c0b4c */
    f->r[6]=MI_U32((0x00000020u+f->r[4]));
    /* 004c0b50 */
    f->r[2]=MI_U32((0x00000024u+f->r[4]));
    /* 004c0b54 */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 004c0b58 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 004c0b59 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004c0b5a */
    a=f->r[0];b=0x005e00ccu;t=mi_alu(f,a,b,32,1);
    /* 004c0b5f */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 004c0b63 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 004c0b67 */
    if(mi_signed_less(f->flags))goto mi_004c0978;
    /* 004c0b6d */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004c0b71 */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 004c0b75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b76 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b77 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b78 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b79 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0b7eu;f->boundary(0x004c0b79u,0x0047b590u,f);
    /* 004c0b7e */
    f->r[7]=MI_U32((0x00000024u+f->r[4]));
    /* 004c0b82 */
    f->r[6]=MI_U32((0x00000028u+f->r[4]));
    /* 004c0b86 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b88 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b8a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b8b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b8c */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b8d */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b8e */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0b93u;f->boundary(0x004c0b8eu,0x004767d0u,f);
    /* 004c0b93 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b94 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b95 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b96 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b97 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0b9cu;f->boundary(0x004c0b97u,0x00477500u,f);
    /* 004c0b9c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b9d */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b9e */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0b9f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0ba0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0ba5u;f->boundary(0x004c0ba0u,0x004777c0u,f);
    /* 004c0ba5 */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0ba8 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0baa */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bab */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bac */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bad */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bae */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0bb3u;f->boundary(0x004c0baeu,0x00475cc0u,f);
    /* 004c0bb3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bb4 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bb5 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bb6 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bb7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0bbcu;f->boundary(0x004c0bb7u,0x004776f0u,f);
    /* 004c0bbc */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bbd */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bbe */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bbf */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004c0bc0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004c0bc5u;f->boundary(0x004c0bc0u,0x00477890u,f);
    /* 004c0bc5 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0bc8 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004c0bcf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004c0bd0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004c0bd1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004c0bd2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004c0bd3 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004c0bd6 */
    f->r[4]+=4;return;
}
