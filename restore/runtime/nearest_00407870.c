/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "nearest_rect.h"

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

void restore_C_00407870(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00407870 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00e38df7u)))&0xffu)<<0);
    /* 00407875 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00407878 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040787a */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040787b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040787c */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0040787d */
    if(!(f->flags&MI_ZF))goto mi_00407888;
    /* 0040787f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00407880 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00407881 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00407883 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00407884 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00407887 */
    f->r[4]+=4;return;
mi_00407888:;
    /* 00407888 */
    f->r[2]=MI_U32((0x00e38e34u));
    /* 0040788e */
    f->r[7]=MI_U32((0x005d2a18u));
    /* 00407894 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 00407896 */
    if(!mi_signed_less(f->flags))goto mi_004078a1;
    /* 00407898 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00407899 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0040789a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 0040789c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0040789d */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004078a0 */
    f->r[4]+=4;return;
mi_004078a1:;
    /* 004078a1 */
    f->r[1]=MI_U32((0x00e38d9cu));
    /* 004078a7 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*1u);
    /* 004078aa */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004078ac */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004078ae */
    if(mi_signed_less(f->flags))goto mi_004078b9;
    /* 004078b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004078b1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004078b2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004078b4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004078b5 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004078b8 */
    f->r[4]+=4;return;
mi_004078b9:;
    /* 004078b9 */
    f->r[0]=MI_U32((0x00e38e30u));
    /* 004078be */
    f->r[1]=MI_U32((0x005d2a1cu));
    /* 004078c4 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004078c6 */
    if(!mi_signed_less(f->flags))goto mi_004078d1;
    /* 004078c8 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004078c9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004078ca */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004078cc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004078cd */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004078d0 */
    f->r[4]+=4;return;
mi_004078d1:;
    /* 004078d1 */
    f->r[6]=MI_U32((0x00e38d98u));
    /* 004078d7 */
    f->r[3]=MI_U32((0x005d2a20u));
    /* 004078dd */
    a=f->r[6];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004078df */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004078e1 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004078e3 */
    if(mi_signed_less(f->flags))goto mi_004078ee;
    /* 004078e5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004078e6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004078e7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004078e9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004078ea */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004078ed */
    f->r[4]+=4;return;
mi_004078ee:;
    /* 004078ee */
    f->r[6]=MI_U32((0x00775910u));
    /* 004078f4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004078f5 */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,1);f->r[6]=t;
    /* 004078f7 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(0x00002710u);
    /* 004078ff */
    a=f->r[6];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00407901 */
    f->r[2]=MI_U32((0x0077590cu));
    /* 00407907 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00407909 */
    f->r[1]=0x0077f75cu;
    /* 0040790e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00407910 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00407912 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00407916 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040791a */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 0040791e */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00407922 */
    goto mi_00407928;
mi_00407924:;
    /* 00407924 */
    f->r[2]=MI_U32((0x00000010u+f->r[4]));
mi_00407928:;
    /* 00407928 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffedu+f->r[1])))&0xffu)<<0);
    /* 0040792b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0040792d */
    if((f->flags&MI_ZF))goto mi_004079fa;
    /* 00407933 */
    f->r[0]=MI_U32((0x00784cb4u));
    /* 00407938 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0040793a */
    if((f->flags&MI_ZF))goto mi_00407945;
    /* 0040793c */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[1]));
    /* 0040793f */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[1]));
    /* 00407943 */
    goto mi_0040794d;
mi_00407945:;
    /* 00407945 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff0u+f->r[1]));
    /* 00407949 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff2u+f->r[1]));
mi_0040794d:;
    /* 0040794d */
    f->r[0]=(0xfffffff8u+f->r[7]);
    /* 00407950 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00407952 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004079de;
    /* 00407958 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff4u+f->r[1]));
    /* 0040795c */
    f->r[5]=(0x00000008u+f->r[0]+f->r[7]*1u);
    /* 00407960 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 00407962 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004079de;
    /* 00407964 */
    f->r[5]=(0xfffffff8u+f->r[3]);
    /* 00407967 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00407969 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_004079de;
    /* 0040796b */
    f->r[5]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff6u+f->r[1]));
    /* 0040796f */
    f->r[2]=(0x00000008u+f->r[3]+f->r[5]*1u);
    /* 00407973 */
    a=f->r[2];b=MI_U32((0x00000010u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 00407977 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004079de;
    /* 00407979 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0040797a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0040797c */
    f->r[1]=f->r[0];
    /* 0040797e */
    f->r[0]=f->r[5];
    /* 00407980 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00407981 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00407983 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 00407985 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00407987 */
    a=f->r[1];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00407989 */
    a=f->r[0];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0040798b */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0040798d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407995;
    /* 0040798f */
    f->r[2]=f->r[6];
    /* 00407991 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00407993 */
    goto mi_0040799f;
mi_00407995:;
    /* 00407995 */
    if(!mi_signed_less(f->flags))goto mi_0040799d;
    /* 00407997 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00407999 */
    f->r[2]=f->r[1];
    /* 0040799b */
    goto mi_0040799f;
mi_0040799d:;
    /* 0040799d */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_0040799f:;
    /* 0040799f */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 004079a3 */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 004079a9 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 004079ab */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004079b1;
    /* 004079ad */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004079af */
    goto mi_004079bb;
mi_004079b1:;
    /* 004079b1 */
    if(!mi_signed_less(f->flags))goto mi_004079b9;
    /* 004079b3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 004079b5 */
    f->r[1]=f->r[0];
    /* 004079b7 */
    goto mi_004079bb;
mi_004079b9:;
    /* 004079b9 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_004079bb:;
    /* 004079bb */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 004079bd */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[1]);
    /* 004079c3 */
    if(mi_signed_less(f->flags))goto mi_004079c7;
    /* 004079c5 */
    f->r[1]=f->r[2];
mi_004079c7:;
    /* 004079c7 */
    a=f->r[1];b=MI_U32((0x0000001cu+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 004079cb */
    if(!mi_signed_less(f->flags))goto mi_004079da;
    /* 004079cd */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004079d1 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004079d5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004079d6 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
mi_004079da:;
    /* 004079da */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
mi_004079de:;
    /* 004079de */
    f->r[7]=MI_U32((0x00000014u+f->r[4]));
    /* 004079e2 */
    a=f->r[1];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004079e5 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004079e6 */
    a=f->r[1];b=0x007843ecu;t=mi_alu(f,a,b,32,1);
    /* 004079ec */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[7]);
    /* 004079f0 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004079f4 */
    if(mi_signed_less(f->flags))goto mi_00407924;
mi_004079fa:;
    /* 004079fa */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 004079fe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004079ff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00407a00 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00407a01 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 00407a08 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00407a09 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00407a0c */
    f->r[4]+=4;return;
}
