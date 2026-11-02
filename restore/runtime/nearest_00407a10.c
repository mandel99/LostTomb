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

void restore_C_00407a10(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00407a10 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00407a13 */
    f->r[2]=MI_U32((0x00775910u));
    /* 00407a19 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00407a1a */
    f->r[1]=MI_U32((0x005d2a1cu));
    /* 00407a20 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00407a21 */
    f->r[5]=MI_U32((0x00e38e34u));
    /* 00407a27 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00407a28 */
    f->r[6]=MI_U32((0x005d2a18u));
    /* 00407a2e */
    f->r[0]=MI_U32((0x0077590cu));
    /* 00407a33 */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[5]=t;
    /* 00407a35 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00407a36 */
    a=f->r[5];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00407a38 */
    f->r[2]=MI_U32((0x00e38e30u));
    /* 00407a3e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00407a40 */
    f->r[1]=0x0077f75cu;
    /* 00407a45 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00407a47 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00407a4b */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00407a4f */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(0x0000001eu);
    /* 00407a57 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 00407a5d */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(0x00000000u);
    /* 00407a65 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00407a69 */
    goto mi_00407a6f;
mi_00407a6b:;
    /* 00407a6b */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
mi_00407a6f:;
    /* 00407a6f */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0xffffffedu+f->r[1])))&0xffu)<<0);
    /* 00407a72 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00407a74 */
    if((f->flags&MI_ZF))goto mi_00407b43;
    /* 00407a7a */
    f->r[0]=MI_U32((0x00784cb4u));
    /* 00407a7f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00407a81 */
    if((f->flags&MI_ZF))goto mi_00407a8c;
    /* 00407a83 */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000000u+f->r[1]));
    /* 00407a86 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00000002u+f->r[1]));
    /* 00407a8a */
    goto mi_00407a94;
mi_00407a8c:;
    /* 00407a8c */
    f->r[6]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff0u+f->r[1]));
    /* 00407a90 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff2u+f->r[1]));
mi_00407a94:;
    /* 00407a94 */
    f->r[0]=(0xfffffff8u+f->r[6]);
    /* 00407a97 */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00407a99 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00407b27;
    /* 00407a9f */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff4u+f->r[1]));
    /* 00407aa3 */
    f->r[3]=(0x00000008u+f->r[0]+f->r[6]*1u);
    /* 00407aa7 */
    a=f->r[3];b=f->r[5];t=mi_alu(f,a,b,32,1);
    /* 00407aa9 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407b27;
    /* 00407aab */
    f->r[3]=(0xfffffff8u+f->r[7]);
    /* 00407aae */
    a=f->r[3];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 00407ab0 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00407b27;
    /* 00407ab2 */
    f->r[3]=(uint32_t)(int32_t)(int16_t)MI_U16((0xfffffff6u+f->r[1]));
    /* 00407ab6 */
    f->r[2]=(0x00000008u+f->r[3]+f->r[7]*1u);
    /* 00407aba */
    a=f->r[2];b=MI_U32((0x00000014u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 00407abe */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407b27;
    /* 00407ac0 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00407ac1 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00407ac3 */
    f->r[1]=f->r[0];
    /* 00407ac5 */
    f->r[0]=f->r[3];
    /* 00407ac7 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00407ac8 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00407aca */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 00407acc */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00407ace */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00407ad0 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00407ad2 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00407ad4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407adc;
    /* 00407ad6 */
    f->r[2]=f->r[5];
    /* 00407ad8 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00407ada */
    goto mi_00407ae6;
mi_00407adc:;
    /* 00407adc */
    if(!mi_signed_less(f->flags))goto mi_00407ae4;
    /* 00407ade */
    a=f->r[1];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00407ae0 */
    f->r[2]=f->r[1];
    /* 00407ae2 */
    goto mi_00407ae6;
mi_00407ae4:;
    /* 00407ae4 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_00407ae6:;
    /* 00407ae6 */
    f->r[1]=MI_U32((0x00000014u+f->r[4]));
    /* 00407aea */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 00407af0 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00407af2 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407af8;
    /* 00407af4 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00407af6 */
    goto mi_00407b02;
mi_00407af8:;
    /* 00407af8 */
    if(!mi_signed_less(f->flags))goto mi_00407b00;
    /* 00407afa */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00407afc */
    f->r[1]=f->r[0];
    /* 00407afe */
    goto mi_00407b02;
mi_00407b00:;
    /* 00407b00 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00407b02:;
    /* 00407b02 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00407b04 */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[1]);
    /* 00407b0a */
    if(mi_signed_less(f->flags))goto mi_00407b0e;
    /* 00407b0c */
    f->r[1]=f->r[2];
mi_00407b0e:;
    /* 00407b0e */
    a=f->r[1];b=MI_U32((0x00000010u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 00407b12 */
    if(!mi_signed_less(f->flags))goto mi_00407b23;
    /* 00407b14 */
    f->r[0]=MI_U32((0x00000018u+f->r[4]));
    /* 00407b18 */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00407b1c */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00407b20 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00407b21 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[0]);
mi_00407b23:;
    /* 00407b23 */
    f->r[1]=MI_U32((0x0000001cu+f->r[4]));
mi_00407b27:;
    /* 00407b27 */
    f->r[6]=MI_U32((0x00000018u+f->r[4]));
    /* 00407b2b */
    a=f->r[1];b=0x00000062u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00407b2e */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00407b2f */
    a=f->r[1];b=0x007843ecu;t=mi_alu(f,a,b,32,1);
    /* 00407b35 */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00407b39 */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00407b3d */
    if(mi_signed_less(f->flags))goto mi_00407a6b;
mi_00407b43:;
    /* 00407b43 */
    f->r[2]=MI_U32((0x00000028u+f->r[4]));
    /* 00407b47 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
    /* 00407b49 */
    MI_U32((0x00000000u+f->r[2]))=(uint32_t)(0xffffffffu);
    /* 00407b4f */
    f->r[0]=MI_U32((0x00f53fdcu));
    /* 00407b54 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00407b56 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407bf9;
    /* 00407b5c */
    f->r[3]=MI_U32((0x00e38e34u));
    /* 00407b62 */
    f->r[5]=MI_U32((0x00e38e30u));
    /* 00407b68 */
    f->r[7]=0x00f535d4u;
mi_00407b6d:;
    /* 00407b6d */
    f->r[0]=MI_U32((0x00000008u+f->r[7]));
    /* 00407b70 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00407b71 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00407b73 */
    f->r[1]=f->r[0];
    /* 00407b75 */
    f->r[0]=MI_U32((0x00000000u+f->r[7]));
    /* 00407b77 */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[1]=t;
    /* 00407b79 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00407b7b */
    f->r[0]=MI_U32((0x0000000cu+f->r[7]));
    /* 00407b7e */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00407b7f */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00407b81 */
    f->r[2]=MI_U32((0x00000004u+f->r[7]));
    /* 00407b84 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00407b86 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00407b88 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00407b8a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407b92;
    /* 00407b8c */
    f->r[2]=f->r[3];
    /* 00407b8e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00407b90 */
    goto mi_00407b9c;
mi_00407b92:;
    /* 00407b92 */
    if(!mi_signed_less(f->flags))goto mi_00407b9a;
    /* 00407b94 */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00407b96 */
    f->r[2]=f->r[1];
    /* 00407b98 */
    goto mi_00407b9c;
mi_00407b9a:;
    /* 00407b9a */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
mi_00407b9c:;
    /* 00407b9c */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00407b9e */
    MI_U32((0x00d3b4fcu))=(uint32_t)(f->r[2]);
    /* 00407ba4 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00407bac;
    /* 00407ba6 */
    f->r[1]=f->r[5];
    /* 00407ba8 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00407baa */
    goto mi_00407bb6;
mi_00407bac:;
    /* 00407bac */
    if(!mi_signed_less(f->flags))goto mi_00407bb4;
    /* 00407bae */
    a=f->r[0];b=f->r[5];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00407bb0 */
    f->r[1]=f->r[0];
    /* 00407bb2 */
    goto mi_00407bb6;
mi_00407bb4:;
    /* 00407bb4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_00407bb6:;
    /* 00407bb6 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00407bb8 */
    MI_U32((0x00d3b4f8u))=(uint32_t)(f->r[1]);
    /* 00407bbe */
    if(mi_signed_less(f->flags))goto mi_00407bc2;
    /* 00407bc0 */
    f->r[1]=f->r[2];
mi_00407bc2:;
    /* 00407bc2 */
    a=f->r[1];b=MI_U32((0x00000010u+f->r[4]));t=mi_alu(f,a,b,32,1);
    /* 00407bc6 */
    if(!mi_signed_less(f->flags))goto mi_00407be8;
    /* 00407bc8 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00407bcc */
    MI_U32((0x00000010u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00407bd0 */
    f->r[1]=MI_U32((0x00000028u+f->r[4]));
    /* 00407bd4 */
    MI_U32((0x00000000u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 00407bda */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[6]);
    /* 00407bdc */
    f->r[3]=MI_U32((0x00e38e34u));
    /* 00407be2 */
    f->r[5]=MI_U32((0x00e38e30u));
mi_00407be8:;
    /* 00407be8 */
    f->r[0]=MI_U32((0x00f53fdcu));
    /* 00407bed */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00407bee */
    a=f->r[7];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00407bf1 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00407bf3 */
    if(mi_signed_less(f->flags))goto mi_00407b6d;
mi_00407bf9:;
    /* 00407bf9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00407bfa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00407bfb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00407bfc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00407bfd */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00407c00 */
    f->r[4]+=4;return;
}
