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

void restore_C_00440b10(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00440b10 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 00440b15 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00440b18 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b19 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b1a */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b1b */
    f->r[7]=MI_U32((0x00000048u+f->r[4]));
    /* 00440b1f */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00440b21 */
    if(mi_signed_less(f->flags))goto mi_00440b26;
    /* 00440b23 */
    f->r[7]=(0xffffffffu+f->r[0]);
mi_00440b26:;
    /* 00440b26 */
    f->r[6]=MI_U32((0x0000004cu+f->r[4]));
    /* 00440b2a */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 00440b2f */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00440b31 */
    if(mi_signed_less(f->flags))goto mi_00440b36;
    /* 00440b33 */
    f->r[6]=(0xffffffffu+f->r[0]);
mi_00440b36:;
    /* 00440b36 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 00440b3c */
    f->r[0]=(0x00000000u+f->r[6]+f->r[6]*8u);
    /* 00440b3f */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b41 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b42 */
    f->r[0]=(0x00000000u+f->r[6]+f->r[0]*2u);
    /* 00440b45 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b46 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 00440b49 */
    f->r[5]=(0x00000000u+f->r[2]+f->r[1]*4u);
    /* 00440b4c */
    a=f->r[5];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00440b4e */
    MI_U32((0x0000001cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00440b52 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440b57u;f->boundary(0x00440b52u,0x004413b0u,f);
    /* 00440b57 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b59 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440b5eu;f->boundary(0x00440b59u,0x0043f410u,f);
    /* 00440b5e */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440b61 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440b63 */
    if(!(f->flags&MI_ZF))goto mi_00440b6c;
    /* 00440b65 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440b66 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440b67 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440b68 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440b6b */
    f->r[4]+=4;return;
mi_00440b6c:;
    /* 00440b6c */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b6e */
    f->r[0]=(0x00000018u+f->r[4]);
    /* 00440b72 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b74 */
    f->r[1]=(0x00000020u+f->r[4]);
    /* 00440b78 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b79 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b7a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b7b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b7c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440b81u;f->boundary(0x00440b7cu,0x00441130u,f);
    /* 00440b81 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440b84 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00440b87 */
    if(!(f->flags&MI_ZF))goto mi_00440b92;
    /* 00440b89 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440b8a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440b8b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00440b8d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440b8e */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440b91 */
    f->r[4]+=4;return;
mi_00440b92:;
    /* 00440b92 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b94 */
    f->r[2]=(0x0000004cu+f->r[4]);
    /* 00440b98 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b9a */
    f->r[0]=(0x00000054u+f->r[4]);
    /* 00440b9e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440b9f */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ba0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ba1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ba2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440ba7u;f->boundary(0x00440ba2u,0x00441130u,f);
    /* 00440ba7 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440baa */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00440bad */
    MI_U32((0x0000000cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00440bb1 */
    if(!(f->flags&MI_ZF))goto mi_00440bbc;
    /* 00440bb3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440bb4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440bb5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00440bb7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440bb8 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440bbb */
    f->r[4]+=4;return;
mi_00440bbc:;
    /* 00440bbc */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bbe */
    f->r[1]=(0x00000020u+f->r[4]);
    /* 00440bc2 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bc4 */
    f->r[2]=(0x00000028u+f->r[4]);
    /* 00440bc8 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bc9 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bcb */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bcc */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440bd1u;f->boundary(0x00440bccu,0x00441130u,f);
    /* 00440bd1 */
    a=f->r[4];b=0x00000018u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440bd4 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,1);
    /* 00440bd7 */
    if(!(f->flags&MI_ZF))goto mi_00440be2;
    /* 00440bd9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440bda */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440bdb */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00440bdd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440bde */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440be1 */
    f->r[4]+=4;return;
mi_00440be2:;
    /* 00440be2 */
    f->r[0]=MI_U32((0x0000003cu+f->r[4]));
    /* 00440be6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440be8 */
    if((f->flags&MI_ZF))goto mi_00440bf6;
    /* 00440bea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440beb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440bec */
    f->r[0]=0x00000001u;
    /* 00440bf1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440bf2 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440bf5 */
    f->r[4]+=4;return;
mi_00440bf6:;
    /* 00440bf6 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bf7 */
    a=0x00032c40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440bfc */
    a=0x00af598cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c01 */
    a=0x009de62cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c06 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440c0bu;f->boundary(0x00440c06u,0x0040f860u,f);
    /* 00440c0b */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c10 */
    a=0x00ae8e7cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c15 */
    a=0x009d1b1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c1a */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440c1fu;f->boundary(0x00440c1au,0x0040f860u,f);
    /* 00440c1f */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c24 */
    a=0x00b285ccu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c29 */
    a=0x009b84fcu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c2e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440c33u;f->boundary(0x00440c2eu,0x0040f860u,f);
    /* 00440c33 */
    a=0x0000cb10u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c38 */
    a=0x00b67d1cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c3d */
    a=0x009ab9ecu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c42 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440c47u;f->boundary(0x00440c42u,0x0040f860u,f);
    /* 00440c47 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c49 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440c4eu;f->boundary(0x00440c49u,0x00485830u,f);
    /* 00440c4e */
    f->r[0]=MI_U32((0x0000004cu+f->r[4]));
    /* 00440c52 */
    f->r[1]=MI_U32((0x00000050u+f->r[4]));
    /* 00440c56 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00440c5c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c5e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c60 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c61 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c62 */
    f->r[7]=0x00000001u;
    /* 00440c67 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c69 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c6a */
    MI_U32((0x00bc971cu))=(uint32_t)(f->r[7]);
    /* 00440c70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440c75u;f->boundary(0x00440c70u,0x0046a400u,f);
    /* 00440c75 */
    f->r[1]=MI_U32((0x00000064u+f->r[4]));
    /* 00440c79 */
    f->r[6]=f->r[0];
    /* 00440c7b */
    f->r[0]=MI_U32((0x00f5659cu));
    /* 00440c80 */
    f->r[2]=MI_U32((0x00000068u+f->r[4]));
    /* 00440c84 */
    a=f->r[4];b=0x0000004cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440c87 */
    f->r[3]=f->r[6];
    /* 00440c89 */
    a=f->r[3];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 00440c8c */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c8e */
    a=f->r[3];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[3]=t;
    /* 00440c90 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c91 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c93 */
    a=f->r[3];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[3]=t;
    /* 00440c96 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c98 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c99 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440c9a */
    MI_U32((0x00799ab0u+f->r[3]))=(uint32_t)(f->r[7]);
    /* 00440ca0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ca1 */
    MI_U32((0x00000050u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00440ca5 */
    MI_U32((0x00799aa8u+f->r[3]))=(uint32_t)(f->r[5]);
    /* 00440cab */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440cb0u;f->boundary(0x00440cabu,0x0047bf50u,f);
    /* 00440cb0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cb1 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440cb6u;f->boundary(0x00440cb1u,0x00485100u,f);
    /* 00440cb6 */
    f->r[0]=MI_U32((0x00000040u+f->r[4]));
    /* 00440cba */
    f->r[1]=MI_U32((0x00000044u+f->r[4]));
    /* 00440cbe */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00440cc4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cc6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cc8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cc9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cca */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ccc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ccd */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440cd2u;f->boundary(0x00440ccdu,0x0046a400u,f);
    /* 00440cd2 */
    f->r[6]=f->r[0];
    /* 00440cd4 */
    f->r[0]=MI_U32((0x00f567acu));
    /* 00440cd9 */
    f->r[1]=MI_U32((0x00000058u+f->r[4]));
    /* 00440cdd */
    f->r[2]=MI_U32((0x0000005cu+f->r[4]));
    /* 00440ce1 */
    f->r[7]=f->r[6];
    /* 00440ce3 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ce5 */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00440ce8 */
    a=f->r[7];b=f->r[6];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 00440cea */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ceb */
    a=f->r[7];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 00440cee */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cf0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cf2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cf3 */
    MI_U32((0x00799ab0u+f->r[7]))=(uint32_t)(0x00000000u);
    /* 00440cfd */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440cfe */
    MI_U32((0x00000080u+f->r[4]))=(uint32_t)(f->r[6]);
    /* 00440d05 */
    MI_U32((0x00799aa8u+f->r[7]))=(uint32_t)(f->r[5]);
    /* 00440d0b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d0c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440d11u;f->boundary(0x00440d0cu,0x0047bf50u,f);
    /* 00440d11 */
    a=f->r[4];b=0x00000054u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440d14 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d15 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440d1au;f->boundary(0x00440d15u,0x00485100u,f);
    /* 00440d1a */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
    /* 00440d1e */
    f->r[0]=(0x0000002cu+f->r[4]);
    /* 00440d22 */
    f->r[1]=(0x00000030u+f->r[4]);
    /* 00440d26 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d27 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d28 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d29 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440d2eu;f->boundary(0x00440d29u,0x004405c0u,f);
    /* 00440d2e */
    f->r[0]=MI_U32((0x0000005cu+f->r[4]));
    /* 00440d32 */
    f->r[1]=MI_U32((0x00000060u+f->r[4]));
    /* 00440d36 */
    f->r[2]=MI_U32((0x00eaac28u));
    /* 00440d3c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d3e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d40 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d41 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d42 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d44 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d45 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440d4au;f->boundary(0x00440d45u,0x0046a400u,f);
    /* 00440d4a */
    f->r[5]=f->r[0];
    /* 00440d4c */
    f->r[0]=MI_U32((0x00000038u+f->r[4]));
    /* 00440d50 */
    f->r[6]=f->r[5];
    /* 00440d52 */
    f->r[2]=MI_U32((0x00000054u+f->r[4]));
    /* 00440d56 */
    f->r[1]=MI_U32((0x0000003cu+f->r[4]));
    /* 00440d5a */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d5c */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00440d5f */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 00440d61 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d62 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 00440d65 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d67 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d69 */
    MI_U32((0x00799ab0u+f->r[6]))=(uint32_t)(0x00000000u);
    /* 00440d73 */
    MI_U32((0x00799aacu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00440d79 */
    f->r[0]=MI_U32((0x00000084u+f->r[4]));
    /* 00440d80 */
    MI_U32((0x00799aa8u+f->r[6]))=(uint32_t)(f->r[1]);
    /* 00440d86 */
    f->r[1]=MI_U32((0x00000088u+f->r[4]));
    /* 00440d8d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d8e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d8f */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d90 */
    MI_U32((0x0000007cu+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00440d94 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440d99u;f->boundary(0x00440d94u,0x0047bf50u,f);
    /* 00440d99 */
    a=f->r[4];b=0x00000044u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440d9c */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440d9d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440da2u;f->boundary(0x00440d9du,0x00485100u,f);
    /* 00440da2 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00440da6 */
    f->r[2]=MI_U32((0x00000050u+f->r[4]));
    /* 00440daa */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440dad */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00440daf */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440db1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440db3 */
    if(!(f->flags&MI_ZF))goto mi_00440df1;
    /* 00440db5 */
    f->r[0]=MI_U32((0x00000058u+f->r[4]));
    /* 00440db9 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00440dbf */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00440dc0 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440dc1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440dc2 */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440dc4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440dc5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440dcau;f->boundary(0x00440dc5u,0x0046a400u,f);
    /* 00440dca */
    f->r[2]=MI_U32((0x00000040u+f->r[4]));
    /* 00440dce */
    f->r[1]=MI_U32((0x00000068u+f->r[4]));
    /* 00440dd2 */
    f->r[5]=f->r[0];
    /* 00440dd4 */
    f->r[0]=MI_U32((0x00000064u+f->r[4]));
    /* 00440dd8 */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440dda */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ddb */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ddd */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00440dde */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440de0 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440de1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440de2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440de3 */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[5]);
    /* 00440de7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440decu;f->boundary(0x00440de7u,0x0047bf50u,f);
    /* 00440dec */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440def */
    goto mi_00440e2d;
mi_00440df1:;
    /* 00440df1 */
    f->r[0]=MI_U32((0x00000058u+f->r[4]));
    /* 00440df5 */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00440dfb */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00440dfc */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440dfd */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440dfe */
    a=0x0000001eu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e00 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e01 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440e06u;f->boundary(0x00440e01u,0x0046a400u,f);
    /* 00440e06 */
    f->r[2]=MI_U32((0x00000040u+f->r[4]));
    /* 00440e0a */
    f->r[1]=MI_U32((0x00000064u+f->r[4]));
    /* 00440e0e */
    a=0x00000008u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e10 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e11 */
    f->r[2]=MI_U32((0x00000070u+f->r[4]));
    /* 00440e15 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e17 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e19 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00440e1a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e1b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e1c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e1d */
    MI_U32((0x00000074u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00440e21 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440e26u;f->boundary(0x00440e21u,0x0047bf50u,f);
    /* 00440e26 */
    f->r[5]=MI_U32((0x00000074u+f->r[4]));
    /* 00440e2a */
    a=f->r[4];b=0x00000034u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00440e2d:;
    /* 00440e2d */
    f->r[1]=MI_U32((0x00000040u+f->r[4]));
    /* 00440e31 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00440e35 */
    a=f->r[5];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00440e38 */
    a=f->r[5];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[5]=t;
    /* 00440e3a */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440e3b */
    a=f->r[5];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[5]=t;
    /* 00440e3e */
    MI_U32((0x00799ab0u+f->r[5]))=(uint32_t)(0x00000000u);
    /* 00440e48 */
    MI_U32((0x00799aa8u+f->r[5]))=(uint32_t)(f->r[0]);
    /* 00440e4e */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 00440e52 */
    MI_U32((0x00799aacu+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00440e58 */
    MI_U32((0x00799aacu+f->r[7]))=(uint32_t)(f->r[0]);
    /* 00440e5e */
    MI_U32((0x00799aacu+f->r[3]))=(uint32_t)(f->r[0]);
    /* 00440e64 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440e69u;f->boundary(0x00440e64u,0x00485100u,f);
    /* 00440e69 */
    f->r[1]=MI_U32((0x00000034u+f->r[4]));
    /* 00440e6d */
    f->r[0]=MI_U32((0x00000038u+f->r[4]));
    /* 00440e71 */
    f->r[2]=MI_U32((0x0000003cu+f->r[4]));
    /* 00440e75 */
    MI_U16((0x00799a12u+f->r[3]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00440e7c */
    MI_U16((0x00799a10u+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00440e83 */
    MI_U16((0x00799a12u+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00440e8a */
    MI_U16((0x00799a10u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00440e91 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000044u+f->r[4])))&0xffffu)<<0);
    /* 00440e96 */
    MI_U16((0x00799a12u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00440e9d */
    MI_U16((0x00799a10u+f->r[5]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00440ea4 */
    f->r[2]=MI_U32((0x00000018u+f->r[4]));
    /* 00440ea8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440ea9 */
    a=0x00000021u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440eab */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00440eac */
    f->r[4]-=4;MI_U32(f->r[4])=0x00440eb1u;f->boundary(0x00440eacu,0x00440060u,f);
    /* 00440eb1 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440eb4 */
    f->r[0]=0x00000001u;
    /* 00440eb9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00440eba */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00440ebb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00440ebc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00440ebd */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00440ec0 */
    f->r[4]+=4;return;
}
