/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "paving_region_model.h"

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

void restore_C_004759e0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004759e0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004759e1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004759e2 */
    f->r[5]=MI_U32((0x00000010u+f->r[4]));
    /* 004759e6 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004759e7 */
    f->r[6]=MI_U32((0x0000001cu+f->r[4]));
    /* 004759eb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004759ec */
    a=f->r[5];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 004759ee */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004759fa;
    /* 004759f0 */
    f->r[0]=f->r[5];
    /* 004759f2 */
    f->r[5]=f->r[6];
    /* 004759f4 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004759f8 */
    f->r[6]=f->r[0];
mi_004759fa:;
    /* 004759fa */
    f->r[7]=MI_U32((0x0000001cu+f->r[4]));
    /* 004759fe */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 00475a02 */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 00475a04 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00475a10;
    /* 00475a06 */
    f->r[0]=f->r[7];
    /* 00475a08 */
    f->r[7]=f->r[3];
    /* 00475a0a */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00475a0e */
    f->r[3]=f->r[0];
mi_00475a10:;
    /* 00475a10 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a15 */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a1a */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a1f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475a24u;f->boundary(0x00475a1fu,0x0040f860u,f);
    /* 00475a24 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a29 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a2e */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a33 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475a38u;f->boundary(0x00475a33u,0x0040f860u,f);
    /* 00475a38 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a3d */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a42 */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a47 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475a4cu;f->boundary(0x00475a47u,0x0040f860u,f);
    /* 00475a4c */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a51 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a56 */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475a60u;f->boundary(0x00475a5bu,0x0040f860u,f);
    /* 00475a60 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475a62 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475a67u;f->boundary(0x00475a62u,0x00485830u,f);
    /* 00475a67 */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00475a6a */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,3);
    /* 00475a6c */
    if(!mi_signed_less(f->flags))goto mi_00475a70;
    /* 00475a6e */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_00475a70:;
    /* 00475a70 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00475a72 */
    if(!mi_signed_less(f->flags))goto mi_00475a76;
    /* 00475a74 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_00475a76:;
    /* 00475a76 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 00475a7b */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00475a7d */
    if(mi_signed_less(f->flags))goto mi_00475a88;
    /* 00475a7f */
    f->r[1]=(0xffffffffu+f->r[0]);
    /* 00475a82 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00475a86 */
    f->r[6]=f->r[1];
mi_00475a88:;
    /* 00475a88 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 00475a8e */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00475a90 */
    if(mi_signed_less(f->flags))goto mi_00475a97;
    /* 00475a92 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00475a93 */
    MI_U32((0x00000024u+f->r[4]))=(uint32_t)(f->r[1]);
mi_00475a97:;
    /* 00475a97 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00475a99 */
    MI_U32((0x00bc971cu))=(uint32_t)(0x00000000u);
    /* 00475aa3 */
    f->r[3]=f->r[7];
    /* 00475aa5 */
    f->r[2]=(0xffffffffu+f->r[0]+f->r[5]*1u);
    /* 00475aa9 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[7]*8u);
    /* 00475aac */
    MI_U32((0x00000018u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00475ab0 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00475ab6 */
    f->r[0]=(0x00000000u+f->r[7]+f->r[0]*2u);
    /* 00475ab9 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00475abc */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00475ac0 */
    f->r[6]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00475ac3 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00475ac5 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00475ac7 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00475b3f;
mi_00475ac9:;
    /* 00475ac9 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00475acd */
    f->r[7]=f->r[5];
    /* 00475acf */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00475ad1 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00475b29;
mi_00475ad3:;
    /* 00475ad3 */
    a=MI_U32((0x00af598cu+f->r[6]*4u));b=0xeeffd77fu;t=mi_alu(f,a,b,32,3);
    /* 00475ade */
    if(!(f->flags&MI_ZF))goto mi_00475b1f;
    /* 00475ae0 */
    f->r[0]=MI_U32((0x00d05720u+f->r[6]*4u));
    /* 00475ae7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00475ae9 */
    if((f->flags&MI_ZF))goto mi_00475af2;
    /* 00475aeb */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000018u+f->r[0])))&0xffu)<<0);
    /* 00475aee */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00475af0 */
    if((f->flags&MI_ZF))goto mi_00475b1f;
mi_00475af2:;
    /* 00475af2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475af3 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475af4 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475af5 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00475af6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475afbu;f->boundary(0x00475af6u,0x00470900u,f);
    /* 00475afb */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00475afe */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00475b00 */
    if(!(f->flags&MI_ZF))goto mi_00475b1f;
    /* 00475b02 */
    f->r[1]=MI_U32((0x00bc971cu));
    /* 00475b08 */
    f->r[0]=MI_U32((0x00af598cu+f->r[6]*4u));
    /* 00475b0f */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00475b10 */
    a=(f->r[0]&0xffu);b=0x00000020u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00475b12 */
    MI_U32((0x00bc971cu))=(uint32_t)(f->r[1]);
    /* 00475b18 */
    MI_U32((0x00af598cu+f->r[6]*4u))=(uint32_t)(f->r[0]);
mi_00475b1f:;
    /* 00475b1f */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00475b23 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00475b24 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00475b25 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00475b27 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00475ad3;
mi_00475b29:;
    /* 00475b29 */
    f->r[0]=MI_U32((0x00bca2dcu));
    /* 00475b2e */
    f->r[1]=MI_U32((0x00000018u+f->r[4]));
    /* 00475b32 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00475b34 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00475b35 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00475b37 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00475b3b */
    a=f->r[3];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00475b3d */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00475ac9;
mi_00475b3f:;
    /* 00475b3f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00475b44u;f->boundary(0x00475b3fu,0x00476530u,f);
    /* 00475b44 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00475b45 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00475b46 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00475b47 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00475b48 */
    f->r[4]+=4;return;
}
