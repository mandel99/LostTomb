/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "preview_parent_model.h"

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

void restore_C_00405d90(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00405d90 */
    f->r[0]=MI_U32((0x006059e4u));
    /* 00405d95 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00405d97 */
    if((f->flags&MI_ZF))goto mi_00405e8f;
    /* 00405d9d */
    f->r[0]=MI_U32((0x00bca300u));
    /* 00405da2 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00405da4 */
    if((f->flags&MI_ZF))goto mi_00405e8f;
    /* 00405daa */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00605a5cu)))&0xffu)<<0);
    /* 00405db0 */
    MI_U8((0x00605798u))=(uint8_t)(0x00000001u);
    /* 00405db7 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00405db9 */
    if(!(f->flags&MI_ZF))goto mi_00405dd8;
    /* 00405dbb */
    f->r[1]=MI_U32((0x00bca308u));
    /* 00405dc1 */
    MI_U32((0x00bca2e4u))=(uint32_t)(f->r[0]);
    /* 00405dc6 */
    f->r[0]=MI_U32((0x00bca304u));
    /* 00405dcb */
    MI_U32((0x006059d4u))=(uint32_t)(f->r[1]);
    /* 00405dd1 */
    MI_U32((0x006059dcu))=(uint32_t)(f->r[0]);
    /* 00405dd6 */
    goto mi_00405de3;
mi_00405dd8:;
    /* 00405dd8 */
    f->r[0]=MI_U32((0x006059dcu));
    /* 00405ddd */
    f->r[1]=MI_U32((0x006059d4u));
mi_00405de3:;
    /* 00405de3 */
    f->r[2]=MI_U32((0x00605a3cu));
    /* 00405de9 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 00405deb */
    f->r[2]=MI_U32((0x006059e8u));
    /* 00405df1 */
    if(!(f->flags&MI_ZF))goto mi_00405e0c;
    /* 00405df3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405df4 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405df5 */
    f->r[1]=MI_U32((0x006059e0u));
    /* 00405dfb */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405dfc */
    f->r[0]=MI_U32((0x006059d8u));
    /* 00405e01 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e02 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00405e08u;f->boundary(0x00405e03u,0x0046fef0u,f);
    /* 00405e08 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00405e0b */
    f->r[4]+=4;return;
mi_00405e0c:;
    /* 00405e0c */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 00405e0f */
    if(!(f->flags&MI_ZF))goto mi_00405e2a;
    /* 00405e11 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e12 */
    f->r[2]=MI_U32((0x006059d8u));
    /* 00405e18 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e19 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e1a */
    f->r[0]=MI_U32((0x006059e0u));
    /* 00405e1f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e20 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e21 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00405e26u;f->boundary(0x00405e21u,0x0046fef0u,f);
    /* 00405e26 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00405e29 */
    f->r[4]+=4;return;
mi_00405e2a:;
    /* 00405e2a */
    a=f->r[2];b=0x00000058u;t=mi_alu(f,a,b,32,1);
    /* 00405e2d */
    if(!(f->flags&MI_ZF))goto mi_00405e49;
    /* 00405e2f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e30 */
    f->r[2]=MI_U32((0x006059e0u));
    /* 00405e36 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e37 */
    f->r[1]=MI_U32((0x006059d8u));
    /* 00405e3d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e3e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e3f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e40 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00405e45u;f->boundary(0x00405e40u,0x0046fef0u,f);
    /* 00405e45 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00405e48 */
    f->r[4]+=4;return;
mi_00405e49:;
    /* 00405e49 */
    a=f->r[2];b=0x00000059u;t=mi_alu(f,a,b,32,1);
    /* 00405e4c */
    if(!(f->flags&MI_ZF))goto mi_00405e67;
    /* 00405e4e */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e4f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e50 */
    f->r[1]=MI_U32((0x006059e0u));
    /* 00405e56 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e57 */
    f->r[0]=MI_U32((0x006059d8u));
    /* 00405e5c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e5d */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e5e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00405e63u;f->boundary(0x00405e5eu,0x0046fef0u,f);
    /* 00405e63 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00405e66 */
    f->r[4]+=4;return;
mi_00405e67:;
    /* 00405e67 */
    a=f->r[2];b=0x0000005du;t=mi_alu(f,a,b,32,1);
    /* 00405e6a */
    if(!(f->flags&MI_ZF))goto mi_00405e85;
    /* 00405e6c */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e6d */
    f->r[2]=MI_U32((0x006059d8u));
    /* 00405e73 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e74 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e75 */
    f->r[0]=MI_U32((0x006059e0u));
    /* 00405e7a */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e7b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e7c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00405e81u;f->boundary(0x00405e7cu,0x0046fef0u,f);
    /* 00405e81 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00405e84 */
    f->r[4]+=4;return;
mi_00405e85:;
    /* 00405e85 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e86 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00405e87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00405e8cu;f->boundary(0x00405e87u,0x00478440u,f);
    /* 00405e8c */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00405e8f:;
    /* 00405e8f */
    f->r[4]+=4;return;
}
