/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "route_query_model.h"

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

void restore_C_00486a30(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00486a30 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 00486a33 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 00486a38 */
    MI_U8((0x00000003u+f->r[4]))=(uint8_t)(0x00000000u);
    /* 00486a3d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00486a3f */
    if(!(f->flags&MI_ZF))goto mi_00486a91;
    /* 00486a41 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
mi_00486a45:;
    /* 00486a45 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
mi_00486a49:;
    /* 00486a49 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486a4a */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486a4b */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486a4c */
    f->r[6]=MI_U32((0x00000028u+f->r[4]));
    /* 00486a50 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486a51 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486a52 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486a53 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00486a58u;f->boundary(0x00486a53u,0x004806c0u,f);
    /* 00486a58 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00486a5b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00486a5d */
    if((f->flags&MI_ZF))goto mi_00486be0;
    /* 00486a63 */
    f->r[0]=MI_U32((0x00bca310u));
    /* 00486a68 */
    f->r[5]=MI_U32((0x00d3b4e4u));
    /* 00486a6e */
    f->r[3]=MI_U32((0x00d3b4e0u));
    /* 00486a74 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00486a76 */
    if(!(f->flags&MI_ZF))goto mi_00486ac7;
    /* 00486a78 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 00486a7d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00486a7f */
    if((f->flags&MI_ZF))goto mi_00486ac7;
    /* 00486a81 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00486a84 */
    if(!(f->flags&MI_ZF))goto mi_00486ab4;
    /* 00486a86 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00486a8a */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00486a8e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00486a8f */
    goto mi_00486acf;
mi_00486a91:;
    /* 00486a91 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00486a94 */
    if(!(f->flags&MI_ZF))goto mi_00486aa1;
    /* 00486a96 */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00486a9a */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 00486a9e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00486a9f */
    goto mi_00486a49;
mi_00486aa1:;
    /* 00486aa1 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00486aa5 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00486aa8 */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 00486aab */
    if(!(f->flags&MI_ZF))goto mi_00486a45;
    /* 00486aad */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00486ab1 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00486ab2 */
    goto mi_00486a49;
mi_00486ab4:;
    /* 00486ab4 */
    f->r[2]=MI_U32((0x00000020u+f->r[4]));
    /* 00486ab8 */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
    /* 00486abc */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00486abf */
    f->r[0]=(0xffffffffu+f->r[2]);
    /* 00486ac2 */
    if(!(f->flags&MI_ZF))goto mi_00486acf;
    /* 00486ac4 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00486ac5 */
    goto mi_00486acf;
mi_00486ac7:;
    /* 00486ac7 */
    f->r[0]=MI_U32((0x00000020u+f->r[4]));
    /* 00486acb */
    f->r[1]=MI_U32((0x00000024u+f->r[4]));
mi_00486acf:;
    /* 00486acf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486ad0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486ad1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486ad2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00486ad7u;f->boundary(0x00486ad2u,0x004806c0u,f);
    /* 00486ad7 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00486ada */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00486adc */
    if((f->flags&MI_ZF))goto mi_00486be0;
    /* 00486ae2 */
    f->r[6]=MI_U32((0x00d3b4e4u));
    /* 00486ae8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486ae9 */
    f->r[7]=MI_U32((0x00d3b4e0u));
    /* 00486aef */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486af0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486af1 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486af2 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486af3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00486af8u;f->boundary(0x00486af3u,0x0051de70u,f);
    /* 00486af8 */
    f->r[0]=MI_U32((0x00000048u+f->r[4]));
    /* 00486afc */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486afd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486aff */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486b00 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486b01 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486b02 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486b03 */
    a=0x000003e9u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00486b08 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00486b0du;f->boundary(0x00486b08u,0x0051e720u,f);
    /* 00486b0d */
    f->r[1]=MI_U32((0x00000068u+f->r[4]));
    /* 00486b11 */
    f->r[2]=f->r[0];
    /* 00486b13 */
    a=f->r[4];b=0x0000002cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00486b16 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00486b18 */
    MI_U32((0x00000000u+f->r[1]))=(uint32_t)(f->r[2]);
    /* 00486b1a */
    f->r[1]=MI_U32((0x00bc96e4u));
    /* 00486b20 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00486b22 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00486b24 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00486b26 */
    MI_U32((0x00000014u+f->r[4]))=(uint32_t)(f->r[2]);
    /* 00486b2a */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00486b2e */
    MI_U32((0x00000038u+f->r[4]))=(uint32_t)(f->r[1]);
    /* 00486b32 */
    if(mi_signed_less(f->flags))goto mi_00486bbb;
    /* 00486b38 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
mi_00486b3a:;
    /* 00486b3a */
    f->r[6]=f->r[0];
    /* 00486b3c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[6]=t;
    /* 00486b3e */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00486b40 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00486b9e;
    /* 00486b42 */
    f->r[7]=f->r[6];
    /* 00486b44 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[7]=t;
mi_00486b46:;
    /* 00486b46 */
    f->r[1]=MI_U32((0x00000030u+f->r[4]));
    /* 00486b4a */
    a=f->r[1];b=0x00000000u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00486b4d */
    if((f->flags&MI_ZF))goto mi_00486b6b;
    /* 00486b4f */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00486b52 */
    if((f->flags&MI_ZF))goto mi_00486b65;
    /* 00486b54 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00486b57 */
    if((f->flags&MI_ZF))goto mi_00486b5f;
    /* 00486b59 */
    f->r[1]=f->r[5];
    /* 00486b5b */
    f->r[2]=f->r[6];
    /* 00486b5d */
    goto mi_00486b6f;
mi_00486b5f:;
    /* 00486b5f */
    f->r[1]=f->r[7];
    /* 00486b61 */
    f->r[2]=f->r[3];
    /* 00486b63 */
    goto mi_00486b6f;
mi_00486b65:;
    /* 00486b65 */
    f->r[1]=f->r[3];
    /* 00486b67 */
    f->r[2]=f->r[7];
    /* 00486b69 */
    goto mi_00486b6f;
mi_00486b6b:;
    /* 00486b6b */
    f->r[1]=f->r[6];
    /* 00486b6d */
    f->r[2]=f->r[5];
mi_00486b6f:;
    /* 00486b6f */
    f->r[0]=MI_U32((0x0000001cu+f->r[4]));
    /* 00486b73 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00486b75 */
    f->r[0]=MI_U32((0x00000024u+f->r[4]));
    /* 00486b79 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00486b7b */
    if(!(f->flags&MI_ZF))goto mi_00486b90;
    /* 00486b7d */
    f->r[1]=MI_U32((0x00000020u+f->r[4]));
    /* 00486b81 */
    f->r[0]=MI_U32((0x00000028u+f->r[4]));
    /* 00486b85 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 00486b87 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00486b89 */
    if(!(f->flags&MI_ZF))goto mi_00486b90;
    /* 00486b8b */
    MI_U8((0x00000013u+f->r[4]))=(uint8_t)(0x00000001u);
mi_00486b90:;
    /* 00486b90 */
    f->r[0]=MI_U32((0x0000002cu+f->r[4]));
    /* 00486b94 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00486b95 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00486b96 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00486b98 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00486b46;
    /* 00486b9a */
    f->r[1]=MI_U32((0x00000038u+f->r[4]));
mi_00486b9e:;
    /* 00486b9e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00486b9f */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00486ba2 */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
    /* 00486ba6 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00486bb1;
    /* 00486ba8 */
    f->r[0]=0x00000005u;
    /* 00486bad */
    MI_U32((0x0000002cu+f->r[4]))=(uint32_t)(f->r[0]);
mi_00486bb1:;
    /* 00486bb1 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 00486bb2 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 00486bb3 */
    a=f->r[3];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00486bb5 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00486b3a;
    /* 00486bb7 */
    f->r[2]=MI_U32((0x00000014u+f->r[4]));
mi_00486bbb:;
    /* 00486bbb */
    f->r[0]=MI_U32((0x00000030u+f->r[4]));
    /* 00486bbf */
    f->r[1]=MI_U32((0x00000034u+f->r[4]));
    /* 00486bc3 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 00486bc5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00486bc6 */
    if(!(f->flags&MI_ZF))goto mi_00486be0;
    /* 00486bc8 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00486bca */
    if((f->flags&MI_ZF))goto mi_00486be0;
    /* 00486bcc */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x0000000fu+f->r[4])))&0xffu)<<0);
    /* 00486bd0 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00486bd2 */
    if((f->flags&MI_ZF))goto mi_00486be0;
    /* 00486bd4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00486bd5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00486bd6 */
    f->r[0]=0x00000001u;
    /* 00486bdb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00486bdc */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00486bdf */
    f->r[4]+=4;return;
mi_00486be0:;
    /* 00486be0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00486be1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00486be2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00486be4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00486be5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00486be8 */
    f->r[4]+=4;return;
}
