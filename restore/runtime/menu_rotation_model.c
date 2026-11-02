/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "menu_rotation_actions.h"
#include <stdlib.h>

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

void restore_rotation_C_00409eb0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00409eb0 */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 00409eb5 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00409eb8 */
    if(!(f->flags&MI_ZF))goto mi_00409ec1;
    /* 00409eba */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409ebfu;f->boundary(0x00409ebau,0x00430b20u,f);
    /* 00409ebf */
    goto mi_00409ed4;
mi_00409ec1:;
    /* 00409ec1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00409ec3 */
    if(!(f->flags&MI_ZF))goto mi_00409eca;
    /* 00409ec5 */
    goto mi_00409f10;
mi_00409eca:;
    /* 00409eca */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 00409ecd */
    if(!(f->flags&MI_ZF))goto mi_00409ed9;
    /* 00409ecf */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409ed4u;f->boundary(0x00409ecfu,0x00430be0u,f);
mi_00409ed4:;
    /* 00409ed4 */
    f->r[0]=MI_U32((0x00d3b198u));
mi_00409ed9:;
    /* 00409ed9 */
    MI_U32((0x00bc96c8u))=(uint32_t)(f->r[0]);
    /* 00409ede */
    f->r[0]=MI_U32((0x00e94e04u));
    /* 00409ee3 */
    MI_U32((0x00bca2e0u))=(uint32_t)(f->r[0]);
    /* 00409ee8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409eedu;f->boundary(0x00409ee8u,0x0047d6e0u,f);
    /* 00409eed */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409ef2u;f->boundary(0x00409eedu,0x004de080u,f);
    /* 00409ef2 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00409ef4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00409ef6 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00409ef8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409efdu;f->boundary(0x00409ef8u,0x004e2970u,f);
    /* 00409efd */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00409f00 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 00409f07 */
    f->r[4]+=4;return;
    /* 00409f08 */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 00409f09 */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 00409f0a */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 00409f0b */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 00409f0c */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 00409f0d */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 00409f0e */
    ; /* Native NOP preserves registers, memory and flags. */
    /* 00409f0f */
    ; /* Native NOP preserves registers, memory and flags. */
mi_00409f10:;
    /* 00409f10 */
    f->r[0]=MI_U32((0x00bca2e0u));
    /* 00409f15 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00409f17 */
    if((f->flags&MI_ZF))goto mi_00409f95;
    /* 00409f19 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 00409f1c */
    if(!(f->flags&MI_ZF))goto mi_00409f39;
    /* 00409f1e */
    MI_U32((0x00bc96c8u))=(uint32_t)(0x00000001u);
    /* 00409f28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f2du;f->boundary(0x00409f28u,0x00430be0u,f);
    /* 00409f2d */
    f->r[0]=MI_U32((0x00e94e04u));
    /* 00409f32 */
    MI_U32((0x00bca2e0u))=(uint32_t)(f->r[0]);
    /* 00409f37 */
    goto mi_00409f76;
mi_00409f39:;
    /* 00409f39 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00409f3c */
    MI_U32((0x00bc96c8u))=(uint32_t)(0x00000000u);
    /* 00409f46 */
    if(!(f->flags&MI_ZF))goto mi_00409f5b;
    /* 00409f48 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f4du;f->boundary(0x00409f48u,0x00430b20u,f);
    /* 00409f4d */
    f->r[1]=MI_U32((0x00e94e04u));
    /* 00409f53 */
    MI_U32((0x00bca2e0u))=(uint32_t)(f->r[1]);
    /* 00409f59 */
    goto mi_00409f76;
mi_00409f5b:;
    /* 00409f5b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f60u;f->boundary(0x00409f5bu,0x00430b20u,f);
    /* 00409f60 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f65u;f->boundary(0x00409f60u,0x00430b20u,f);
    /* 00409f65 */
    f->r[2]=MI_U32((0x00e94e04u));
    /* 00409f6b */
    MI_U32((0x00bca2e0u))=(uint32_t)(f->r[2]);
    /* 00409f71 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f76u;f->boundary(0x00409f71u,0x0047d6e0u,f);
mi_00409f76:;
    /* 00409f76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f7bu;f->boundary(0x00409f76u,0x0047d6e0u,f);
    /* 00409f7b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f80u;f->boundary(0x00409f7bu,0x004de080u,f);
    /* 00409f80 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00409f82 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00409f84 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00409f86 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00409f8bu;f->boundary(0x00409f86u,0x004e2970u,f);
    /* 00409f8b */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00409f8e */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_00409f95:;
    /* 00409f95 */
    f->r[4]+=4;return;
}

void restore_rotation_C_00430b20(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00430b20 */
    f->r[0]=MI_U32((0x0070e460u));
    /* 00430b25 */
    f->r[1]=MI_U32((0x00bca2c0u));
    /* 00430b2b */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430b2c */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430b2e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430b2f */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00430b31 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430b33 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00430b36 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00430b39 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00430b3c */
    f->r[0]=MI_U32((0x00bca2bcu));
    /* 00430b41 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 00430b44 */
    f->r[0]=MI_U32((0x0070e464u));
    /* 00430b49 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430b4a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430b4c */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*2u);
    /* 00430b4f */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00430b51 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430b53 */
    f->r[6]=MI_U32((0x0070e494u+f->r[0]*4u));
    /* 00430b5a */
    f->r[0]=MI_U32((0x00e94e04u));
    /* 00430b5f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430b62 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00430b65 */
    MI_U32((0x00e94e04u))=(uint32_t)(f->r[0]);
    /* 00430b6a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00430b73;
    /* 00430b6c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00430b6e */
    MI_U32((0x00e94e04u))=(uint32_t)(f->r[0]);
mi_00430b73:;
    /* 00430b73 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430b75 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430b76 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430b7bu;f->boundary(0x00430b76u,0x00430530u,f);
    /* 00430b7b */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430b7e */
    a=f->r[6];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00430b84 */
    if(!mi_signed_less(f->flags))goto mi_00430bd4;
    /* 00430b86 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430b87 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430b8cu;f->boundary(0x00430b87u,0x004304c0u,f);
    /* 00430b8c */
    f->r[0]=MI_U32((0x0070e464u));
    /* 00430b91 */
    f->r[1]=MI_U32((0x00d3b4e4u));
    /* 00430b97 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430b98 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430b9a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430b9d */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00430b9f */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00430ba1 */
    f->r[0]=MI_U32((0x0070e460u));
    /* 00430ba6 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430ba7 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430ba9 */
    f->r[2]=MI_U32((0x00d3b4e0u));
    /* 00430baf */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00430bb1 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[2]=t;
    /* 00430bb3 */
    f->r[0]=MI_U32((0x00e94e04u));
    /* 00430bb8 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00430bbb */
    MI_U32((0x00bca2bcu))=(uint32_t)(f->r[1]);
    /* 00430bc1 */
    MI_U32((0x00bca2c0u))=(uint32_t)(f->r[2]);
    /* 00430bc7 */
    if((f->flags&MI_ZF))goto mi_00430bcd;
    /* 00430bc9 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00430bcb */
    if(!(f->flags&MI_ZF))goto mi_00430bd4;
mi_00430bcd:;
    /* 00430bcd */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 00430bce */
    MI_U32((0x00bca2bcu))=(uint32_t)(f->r[1]);
mi_00430bd4:;
    /* 00430bd4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00430bd5 */
    f->r[4]+=4;return;
}

void restore_rotation_C_00430be0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00430be0 */
    f->r[0]=MI_U32((0x0070e460u));
    /* 00430be5 */
    f->r[1]=MI_U32((0x00bca2c0u));
    /* 00430beb */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430bec */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430bee */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430bef */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00430bf1 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430bf3 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 00430bf6 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[1]*2u);
    /* 00430bf9 */
    f->r[1]=(0x00000000u+f->r[0]+f->r[2]*4u);
    /* 00430bfc */
    f->r[0]=MI_U32((0x00bca2bcu));
    /* 00430c01 */
    f->r[6]=(0x00000000u+f->r[1]+f->r[0]*1u);
    /* 00430c04 */
    f->r[0]=MI_U32((0x0070e464u));
    /* 00430c09 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430c0a */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430c0c */
    f->r[1]=(0x00000000u+f->r[6]+f->r[1]*2u);
    /* 00430c0f */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00430c11 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430c13 */
    f->r[6]=MI_U32((0x0070e494u+f->r[0]*4u));
    /* 00430c1a */
    f->r[0]=MI_U32((0x00e94e04u));
    /* 00430c1f */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430c22 */
    MI_U32((0x00e94e04u))=(uint32_t)(f->r[0]);
    /* 00430c27 */
    if(!(f->flags&MI_SF))goto mi_00430c33;
    /* 00430c29 */
    MI_U32((0x00e94e04u))=(uint32_t)(0x00000006u);
mi_00430c33:;
    /* 00430c33 */
    f->r[2]=MI_U32((0x00e94e04u));
    /* 00430c39 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430c3b */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430c3c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430c41u;f->boundary(0x00430c3cu,0x00430530u,f);
    /* 00430c41 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430c44 */
    a=f->r[6];b=0x0fff0000u;t=mi_alu(f,a,b,32,1);
    /* 00430c4a */
    if(!mi_signed_less(f->flags))goto mi_00430c9c;
    /* 00430c4c */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430c4d */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430c52u;f->boundary(0x00430c4du,0x004304c0u,f);
    /* 00430c52 */
    f->r[0]=MI_U32((0x0070e464u));
    /* 00430c57 */
    f->r[1]=MI_U32((0x00d3b4e4u));
    /* 00430c5d */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430c5e */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430c60 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430c63 */
    a=f->r[0];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[0]=t;
    /* 00430c65 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00430c67 */
    f->r[0]=MI_U32((0x0070e460u));
    /* 00430c6c */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 00430c6d */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430c6f */
    MI_U32((0x00bca2bcu))=(uint32_t)(f->r[1]);
    /* 00430c75 */
    f->r[1]=MI_U32((0x00e94e04u));
    /* 00430c7b */
    f->r[2]=f->r[0];
    /* 00430c7d */
    f->r[0]=MI_U32((0x00d3b4e0u));
    /* 00430c82 */
    a=f->r[2];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[2]=t;
    /* 00430c84 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430c86 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00430c89 */
    MI_U32((0x00bca2c0u))=(uint32_t)(f->r[0]);
    /* 00430c8e */
    if((f->flags&MI_ZF))goto mi_00430c94;
    /* 00430c90 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00430c92 */
    if(!(f->flags&MI_ZF))goto mi_00430c9c;
mi_00430c94:;
    /* 00430c94 */
    a=f->r[0];b=0x00000002u;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 00430c97 */
    MI_U32((0x00bca2c0u))=(uint32_t)(f->r[0]);
mi_00430c9c:;
    /* 00430c9c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00430c9d */
    f->r[4]+=4;return;
}

void restore_rotation_C_0047d6e0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047d6e0 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d6e2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d6e7u;f->boundary(0x0047d6e2u,0x0047d870u,f);
    /* 0047d6e7 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047d6ea */
    MI_U32((0x00bc9694u))=(uint32_t)(0x00000000u);
    /* 0047d6f4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d6f9u;f->boundary(0x0047d6f4u,0x0047d720u,f);
    /* 0047d6f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d6feu;f->boundary(0x0047d6f9u,0x004dea50u,f);
    /* 0047d6fe */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d703u;f->boundary(0x0047d6feu,0x0046d840u,f);
    /* 0047d703 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d708u;f->boundary(0x0047d703u,0x0047fb70u,f);
    /* 0047d708 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d70du;f->boundary(0x0047d708u,0x005178e0u,f);
    /* 0047d70d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047d712u;f->boundary(0x0047d70du,0x00497b90u,f);
    /* 0047d712 */
    f->boundary(0x0047d712u,0x004ddf90u,f);return;
}

void restore_rotation_C_0047d720(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047d720 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0047d725 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d726 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0047d728 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047d72a */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047d86d;
    /* 0047d730 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d731 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d732 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047d733 */
    f->r[7]=MI_U32((0x00e94e04u));
mi_0047d739:;
    /* 0047d739 */
    f->r[2]=MI_U32((0x00bc9f24u+f->r[5]*4u));
    /* 0047d740 */
    f->r[0]=MI_U32((0x00bc9b94u+f->r[5]*4u));
    /* 0047d747 */
    f->r[1]=MI_U32((0x00bc9804u+f->r[5]*4u));
    /* 0047d74e */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047d750 */
    if(!mi_signed_less(f->flags))goto mi_0047d85c;
    /* 0047d756 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0047d758 */
    f->r[3]=f->r[0];
mi_0047d75a:;
    /* 0047d75a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[1])))&0xffu)<<0);
    /* 0047d760 */
    a=f->r[0];b=0x0000000fu;t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0047d763 */
    f->r[6]=f->r[0];
    /* 0047d765 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[1])))&0xffu)<<0);
    /* 0047d76b */
    f->r[2]=(f->r[2]&0xffffff00u)|((((f->r[0]&0xffu))&0xffu)<<0);
    /* 0047d76d */
    a=f->r[2];b=0x0000003fu;t=mi_alu(f,a,b,32,3);f->r[2]=t;
    /* 0047d770 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0047d772 */
    if((f->flags&MI_ZF))goto mi_0047d84c;
    /* 0047d778 */
    a=(f->r[0]&0xffu);b=0x000000bfu;t=mi_alu(f,a,b,8,3);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047d77a */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047d77d */
    MI_U8((0x00b67d1cu+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 0047d783 */
    if(!(f->flags&MI_ZF))goto mi_0047d7a5;
    /* 0047d785 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047d787 */
    if(!(f->flags&MI_ZF))goto mi_0047d791;
    /* 0047d789 */
    a=f->r[2];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 0047d78c */
    goto mi_0047d817;
mi_0047d791:;
    /* 0047d791 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047d794 */
    if(!(f->flags&MI_ZF))goto mi_0047d79b;
    /* 0047d796 */
    a=f->r[2];b=0x00000009u;t=mi_alu(f,a,b,32,1);
    /* 0047d799 */
    goto mi_0047d817;
mi_0047d79b:;
    /* 0047d79b */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047d79e */
    if(!(f->flags&MI_ZF))goto mi_0047d810;
    /* 0047d7a0 */
    a=f->r[2];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0047d7a3 */
    goto mi_0047d80c;
mi_0047d7a5:;
    /* 0047d7a5 */
    a=f->r[6];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047d7a8 */
    if(!(f->flags&MI_ZF))goto mi_0047d7c7;
    /* 0047d7aa */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047d7ac */
    if(!(f->flags&MI_ZF))goto mi_0047d7b3;
    /* 0047d7ae */
    a=f->r[2];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0047d7b1 */
    goto mi_0047d817;
mi_0047d7b3:;
    /* 0047d7b3 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047d7b6 */
    if(!(f->flags&MI_ZF))goto mi_0047d7bd;
    /* 0047d7b8 */
    a=f->r[2];b=0x00000012u;t=mi_alu(f,a,b,32,1);
    /* 0047d7bb */
    goto mi_0047d817;
mi_0047d7bd:;
    /* 0047d7bd */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047d7c0 */
    if(!(f->flags&MI_ZF))goto mi_0047d810;
    /* 0047d7c2 */
    a=f->r[2];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047d7c5 */
    goto mi_0047d80c;
mi_0047d7c7:;
    /* 0047d7c7 */
    a=f->r[6];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0047d7ca */
    if(!(f->flags&MI_ZF))goto mi_0047d7e9;
    /* 0047d7cc */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047d7ce */
    if(!(f->flags&MI_ZF))goto mi_0047d7d5;
    /* 0047d7d0 */
    a=f->r[2];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 0047d7d3 */
    goto mi_0047d817;
mi_0047d7d5:;
    /* 0047d7d5 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047d7d8 */
    if(!(f->flags&MI_ZF))goto mi_0047d7df;
    /* 0047d7da */
    a=f->r[2];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 0047d7dd */
    goto mi_0047d817;
mi_0047d7df:;
    /* 0047d7df */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047d7e2 */
    if(!(f->flags&MI_ZF))goto mi_0047d810;
    /* 0047d7e4 */
    a=f->r[2];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0047d7e7 */
    goto mi_0047d817;
mi_0047d7e9:;
    /* 0047d7e9 */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047d7ec */
    if(!(f->flags&MI_ZF))goto mi_0047d81b;
    /* 0047d7ee */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047d7f0 */
    if(!(f->flags&MI_ZF))goto mi_0047d7f9;
    /* 0047d7f2 */
    a=f->r[2];b=0x00000020u;t=mi_alu(f,a,b,32,1);
    /* 0047d7f5 */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d7f7 */
    goto mi_0047d84c;
mi_0047d7f9:;
    /* 0047d7f9 */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047d7fc */
    if(!(f->flags&MI_ZF))goto mi_0047d805;
    /* 0047d7fe */
    a=f->r[2];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0047d801 */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d803 */
    goto mi_0047d84c;
mi_0047d805:;
    /* 0047d805 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047d808 */
    if(!(f->flags&MI_ZF))goto mi_0047d810;
    /* 0047d80a */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
mi_0047d80c:;
    /* 0047d80c */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d80e */
    goto mi_0047d84c;
mi_0047d810:;
    /* 0047d810 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047d813 */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d815 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
mi_0047d817:;
    /* 0047d817 */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d819 */
    goto mi_0047d84c;
mi_0047d81b:;
    /* 0047d81b */
    a=f->r[6];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0047d81e */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d820 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 0047d822 */
    if(!(f->flags&MI_ZF))goto mi_0047d82b;
    /* 0047d824 */
    a=f->r[2];b=0x00000028u;t=mi_alu(f,a,b,32,1);
    /* 0047d827 */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d829 */
    goto mi_0047d84c;
mi_0047d82b:;
    /* 0047d82b */
    a=f->r[7];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 0047d82e */
    if(!(f->flags&MI_ZF))goto mi_0047d837;
    /* 0047d830 */
    a=f->r[2];b=0x0000002du;t=mi_alu(f,a,b,32,1);
    /* 0047d833 */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d835 */
    goto mi_0047d84c;
mi_0047d837:;
    /* 0047d837 */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 0047d83a */
    if(!(f->flags&MI_ZF))goto mi_0047d843;
    /* 0047d83c */
    a=f->r[2];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0047d83f */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d841 */
    goto mi_0047d84c;
mi_0047d843:;
    /* 0047d843 */
    a=f->r[7];b=0x00000002u;t=mi_alu(f,a,b,32,1);
    /* 0047d846 */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
    /* 0047d848 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 0047d84a */
    if(!(f->flags&MI_ZF))goto mi_0047d854;
mi_0047d84c:;
    /* 0047d84c */
    a=(f->r[0]&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,4);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047d84e */
    MI_U8((0x00b67d1cu+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
mi_0047d854:;
    /* 0047d854 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047d855 */
    a=f->r[3];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[3]=t;
    /* 0047d856 */
    if(!(f->flags&MI_ZF))goto mi_0047d75a;
mi_0047d85c:;
    /* 0047d85c */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 0047d861 */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0047d862 */
    a=f->r[5];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0047d864 */
    if(mi_signed_less(f->flags))goto mi_0047d739;
    /* 0047d86a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047d86b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0047d86c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0047d86d:;
    /* 0047d86d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047d86e */
    f->r[4]+=4;return;
}

void restore_rotation_C_0047fb70(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047fb70 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fb71 */
    f->r[3]=MI_U32((0x00bca2d4u));
    /* 0047fb77 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fb78 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 0047fb7a */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0047fb7c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047fc38;
    /* 0047fb82 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fb83 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047fb84 */
    f->r[7]=MI_U32((0x00bc96c8u));
mi_0047fb8a:;
    /* 0047fb8a */
    f->r[6]=MI_U32((0x00bc9f24u+f->r[5]*4u));
    /* 0047fb91 */
    f->r[1]=MI_U32((0x00bc9b94u+f->r[5]*4u));
    /* 0047fb98 */
    f->r[2]=MI_U32((0x00bc9804u+f->r[5]*4u));
    /* 0047fb9f */
    a=f->r[6];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047fba1 */
    if(!mi_signed_less(f->flags))goto mi_0047fc2d;
    /* 0047fba7 */
    a=f->r[1];b=f->r[6];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0047fba9 */
    f->r[0]=(0x00adc36cu+f->r[2]);
    /* 0047fbaf */
    f->r[2]=(0x00af598cu+f->r[2]*4u);
    /* 0047fbb6 */
    f->r[6]=f->r[1];
mi_0047fbb8:;
    /* 0047fbb8 */
    a=MI_U8((0x00000000u+f->r[2]));b=0x00000004u;t=mi_alu(f,a,b,8,3);
    /* 0047fbbb */
    if((f->flags&MI_ZF))goto mi_0047fc26;
    /* 0047fbbd */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000000u+f->r[0])))&0xffu)<<0);
    /* 0047fbbf */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0047fbc1 */
    if((f->flags&MI_ZF))goto mi_0047fc26;
    /* 0047fbc3 */
    a=(f->r[1]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 0047fbc6 */
    if(!(f->flags&MI_ZF))goto mi_0047fbd7;
    /* 0047fbc8 */
    f->r[1]=f->r[7];
    /* 0047fbca */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047fbcb */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[1]=t;
    /* 0047fbcd */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,2);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fbcf */
    a=(f->r[1]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fbd2 */
    a=(f->r[1]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,0);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fbd5 */
    goto mi_0047fc24;
mi_0047fbd7:;
    /* 0047fbd7 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0047fbda */
    if(!(f->flags&MI_ZF))goto mi_0047fbe1;
    /* 0047fbdc */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000005u);
    /* 0047fbdf */
    goto mi_0047fc26;
mi_0047fbe1:;
    /* 0047fbe1 */
    a=(f->r[1]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0047fbe4 */
    if(!(f->flags&MI_ZF))goto mi_0047fbf5;
    /* 0047fbe6 */
    f->r[1]=f->r[7];
    /* 0047fbe8 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047fbe9 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[1]=t;
    /* 0047fbeb */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,2);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fbed */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fbf0 */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,0);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fbf3 */
    goto mi_0047fc24;
mi_0047fbf5:;
    /* 0047fbf5 */
    a=(f->r[1]&0xffu);b=0x00000004u;t=mi_alu(f,a,b,8,1);
    /* 0047fbf8 */
    if(!(f->flags&MI_ZF))goto mi_0047fc08;
    /* 0047fbfa */
    f->r[1]=f->r[7];
    /* 0047fbfc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047fbfd */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[1]=t;
    /* 0047fbff */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,2);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fc01 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fc04 */
    a=(f->r[1]&0xffu);b=f->flags&MI_CF;t=mi_alu(f,a,1,8,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fc06 */
    goto mi_0047fc24;
mi_0047fc08:;
    /* 0047fc08 */
    a=(f->r[1]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 0047fc0b */
    if(!(f->flags&MI_ZF))goto mi_0047fc12;
    /* 0047fc0d */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)(0x00000002u);
    /* 0047fc10 */
    goto mi_0047fc26;
mi_0047fc12:;
    /* 0047fc12 */
    a=(f->r[1]&0xffu);b=0x00000006u;t=mi_alu(f,a,b,8,1);
    /* 0047fc15 */
    if(!(f->flags&MI_ZF))goto mi_0047fc26;
    /* 0047fc17 */
    f->r[1]=f->r[7];
    /* 0047fc19 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0047fc1a */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[1]=t;
    /* 0047fc1c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,2);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fc1e */
    a=(f->r[1]&0xffu);b=0x000000feu;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047fc21 */
    a=(f->r[1]&0xffu);b=0x00000003u;t=mi_alu(f,a,b,8,0);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
mi_0047fc24:;
    /* 0047fc24 */
    MI_U8((0x00000000u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
mi_0047fc26:;
    /* 0047fc26 */
    a=f->r[2];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047fc29 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047fc2a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0047fc2b */
    if(!(f->flags&MI_ZF))goto mi_0047fbb8;
mi_0047fc2d:;
    /* 0047fc2d */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0047fc2e */
    a=f->r[5];b=f->r[3];t=mi_alu(f,a,b,32,1);
    /* 0047fc30 */
    if(mi_signed_less(f->flags))goto mi_0047fb8a;
    /* 0047fc36 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047fc37 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_0047fc38:;
    /* 0047fc38 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 0047fc39 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0047fc3a */
    f->r[4]+=4;return;
}

void restore_rotation_C_004dfd90(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004dfd90 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004dfd93 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd94 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd95 */
    f->r[5]=MI_U32((0x00000044u+f->r[4]));
    /* 004dfd99 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfd9a */
    f->r[6]=f->r[5];
    /* 004dfd9c */
    f->r[1]=MI_U32((0x00bca2e0u));
    /* 004dfda2 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004dfda5 */
    a=f->r[6];b=f->r[5];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004dfda7 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfda8 */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004dfdab */
    f->r[0]=MI_U32((0x00799a48u+f->r[6]));
    /* 004dfdb1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x007999eau+f->r[6]));
    /* 004dfdb8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfdb9 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfdba */
    f->r[0]=(0x0000001cu+f->r[4]);
    /* 004dfdbe */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfdbf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfdc0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfdc5u;f->boundary(0x004dfdc0u,0x004ded50u,f);
    /* 004dfdc5 */
    f->r[0]=MI_U32((0x00799a44u+f->r[6]));
    /* 004dfdcb */
    f->r[7]=MI_U32((0x00000028u+f->r[4]));
    /* 004dfdcf */
    f->r[1]=MI_U32((0x00799a40u+f->r[6]));
    /* 004dfdd5 */
    f->r[3]=MI_U32((0x00000024u+f->r[4]));
    /* 004dfdd9 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dfddb */
    a=f->r[1];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dfddd */
    MI_U16((0x007999deu+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dfde4 */
    MI_U32((0x00000020u+f->r[4]))=(uint32_t)(f->r[0]);
    /* 004dfde8 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*8u);
    /* 004dfdeb */
    MI_U16((0x007999e0u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dfdf2 */
    MI_U32((0x0000005cu+f->r[4]))=(uint32_t)(f->r[1]);
    /* 004dfdf6 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[2]*2u);
    /* 004dfdf9 */
    f->r[2]=MI_U32((0x00bca2d8u));
    /* 004dfdff */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004dfe02 */
    f->r[0]=(0x00000000u+f->r[2]+f->r[0]*4u);
    /* 004dfe05 */
    f->r[2]=(0x0000005cu+f->r[4]);
    /* 004dfe09 */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dfe0b */
    f->r[1]=(0x00000020u+f->r[4]);
    /* 004dfe0f */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfe10 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfe11 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfe12 */
    MI_U32((0x007999e4u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 004dfe18 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfe1du;f->boundary(0x004dfe18u,0x004e0c50u,f);
    /* 004dfe1d */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000068u+f->r[4])))&0xffffu)<<0);
    /* 004dfe22 */
    a=f->r[4];b=0x0000001cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfe25 */
    MI_U16((0x007999feu+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dfe2c */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 004dfe30 */
    MI_U16((0x00799a00u+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dfe37 */
    f->r[3]=MI_U32((0x00799a34u+f->r[6]));
    /* 004dfe3d */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 004dfe3f */
    if((f->flags&MI_ZF))goto mi_004e003b;
    /* 004dfe45 */
    f->r[7]=(0x00000000u+f->r[3]+f->r[3]*2u);
    /* 004dfe48 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 004dfe4a */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004dfe4d */
    a=f->r[7];b=f->r[3];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004dfe4f */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004dfe52 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd602u+f->r[7])))&0xffu)<<0);
    /* 004dfe58 */
    a=f->r[1];b=0x0000004fu;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004dfe5b */
    if((f->flags&MI_ZF))goto mi_004dffce;
    /* 004dfe61 */
    a=f->r[1];b=0x00000002u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004dfe64 */
    if((f->flags&MI_ZF))goto mi_004dfeb6;
    /* 004dfe66 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004dfe69 */
    if(!(f->flags&MI_ZF))goto mi_004e003b;
    /* 004dfe6f */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[7])))&0xffu)<<0);
    /* 004dfe75 */
    a=(f->r[1]&0xffu);b=0x0000001bu;t=mi_alu(f,a,b,8,1);
    /* 004dfe78 */
    if((f->flags&MI_ZF))goto mi_004dfec1;
    /* 004dfe7a */
    a=(f->r[1]&0xffu);b=0x0000001cu;t=mi_alu(f,a,b,8,1);
    /* 004dfe7d */
    if(!(f->flags&MI_ZF))goto mi_004e003b;
    /* 004dfe83 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfe84 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfe89u;f->boundary(0x004dfe84u,0x004b5420u,f);
    /* 004dfe89 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999deu+f->r[6])))&0xffffu)<<0);
    /* 004dfe90 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e0u+f->r[6])))&0xffffu)<<0);
    /* 004dfe97 */
    f->r[2]=MI_U32((0x007999e4u+f->r[6]));
    /* 004dfe9d */
    MI_U16((0x00bcd60cu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dfea4 */
    MI_U16((0x00bcd60eu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dfeab */
    MI_U32((0x00bcd618u+f->r[7]))=(uint32_t)(f->r[2]);
    /* 004dfeb1 */
    goto mi_004e0032;
mi_004dfeb6:;
    /* 004dfeb6 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[7])))&0xffu)<<0);
    /* 004dfebc */
    a=(f->r[1]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004dfebf */
    if(!(f->flags&MI_ZF))goto mi_004dff05;
mi_004dfec1:;
    /* 004dfec1 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999feu+f->r[6])))&0xffffu)<<0);
    /* 004dfec8 */
    f->r[2]=MI_U32((0x007999e4u+f->r[6]));
    /* 004dfece */
    MI_U16((0x00bcd61cu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dfed5 */
    MI_U16((0x00bcd61eu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dfedc */
    MI_U32((0x00bcd620u+f->r[7]))=(uint32_t)(f->r[2]);
mi_004dfee2:;
    /* 004dfee2 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
mi_004dfee3:;
    /* 004dfee3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfee8u;f->boundary(0x004dfee3u,0x004b3250u,f);
    /* 004dfee8 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfeeb */
    MI_U16((0x00bcd63cu+f->r[7]))=(uint16_t)(0x00000000u);
    /* 004dfef4 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dfef5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dfefau;f->boundary(0x004dfef5u,0x004dfbb0u,f);
    /* 004dfefa */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dfefd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004dfefe */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004dfeff */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004dff00 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004dff01 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dff04 */
    f->r[4]+=4;return;
mi_004dff05:;
    /* 004dff05 */
    a=(f->r[1]&0xffu);b=0x0000000bu;t=mi_alu(f,a,b,8,1);
    /* 004dff08 */
    if((f->flags&MI_ZF))goto mi_004dff13;
    /* 004dff0a */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004dff0d */
    if(!(f->flags&MI_ZF))goto mi_004e003b;
mi_004dff13:;
    /* 004dff13 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dff14 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dff19u;f->boundary(0x004dff14u,0x004b5420u,f);
    /* 004dff19 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x007999deu+f->r[6])))&0xffffu)<<0);
    /* 004dff20 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999e0u+f->r[6])))&0xffffu)<<0);
    /* 004dff27 */
    MI_U16((0x00bcd60cu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dff2e */
    f->r[0]=MI_U32((0x007999e4u+f->r[6]));
    /* 004dff34 */
    MI_U16((0x00bcd60eu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dff3b */
    MI_U32((0x00bcd618u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004dff41 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dff42 */
    MI_U32((0x00bcd620u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004dff48 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dff4du;f->boundary(0x004dff48u,0x004b52f0u,f);
    /* 004dff4d */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[7])))&0xffu)<<0);
    /* 004dff53 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004dff56 */
    a=(f->r[0]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004dff58 */
    if(!(f->flags&MI_ZF))goto mi_004e003b;
    /* 004dff5e */
    f->r[0]=MI_U32((0x00d056f8u));
    /* 004dff63 */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004dff66 */
    a=f->r[2];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[2]=t;
    /* 004dff69 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004dff6b */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x00bcd660u+f->r[2]*4u));
    /* 004dff73 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004dff75 */
    if((f->flags&MI_ZF))goto mi_004e003b;
    /* 004dff7b */
    f->r[7]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 004dff7e */
    a=f->r[7];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004dff81 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[7]=t;
    /* 004dff83 */
    a=f->r[7];b=0x00000002u;t=mi_shift(f,a,b,32,0);f->r[7]=t;
    /* 004dff86 */
    a=MI_U8((0x00bcd602u+f->r[7]));b=0x00000055u;t=mi_alu(f,a,b,8,1);
    /* 004dff8d */
    if(!(f->flags&MI_ZF))goto mi_004e003b;
    /* 004dff93 */
    a=MI_U8((0x00bcd630u+f->r[7]));b=0x0000001au;t=mi_alu(f,a,b,8,1);
    /* 004dff9a */
    if(!(f->flags&MI_ZF))goto mi_004e003b;
    /* 004dffa0 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999feu+f->r[6])))&0xffffu)<<0);
    /* 004dffa7 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00799a00u+f->r[6])))&0xffffu)<<0);
    /* 004dffae */
    MI_U16((0x00bcd61cu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004dffb5 */
    f->r[1]=MI_U32((0x007999e4u+f->r[6]));
    /* 004dffbb */
    MI_U16((0x00bcd61eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004dffc2 */
    MI_U32((0x00bcd620u+f->r[7]))=(uint32_t)(f->r[1]);
    /* 004dffc8 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dffc9 */
    goto mi_004dfee3;
mi_004dffce:;
    /* 004dffce */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00bcd630u+f->r[7])))&0xffu)<<0);
    /* 004dffd4 */
    a=(f->r[1]&0xffu);b=0x00000009u;t=mi_alu(f,a,b,8,1);
    /* 004dffd7 */
    if(!(f->flags&MI_ZF))goto mi_004dffff;
    /* 004dffd9 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999feu+f->r[6])))&0xffffu)<<0);
    /* 004dffe0 */
    MI_U16((0x00bcd61cu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004dffe7 */
    MI_U16((0x00bcd61eu+f->r[7]))=(uint16_t)((f->r[0]&0xffffu));
    /* 004dffee */
    f->r[0]=MI_U32((0x007999e4u+f->r[6]));
    /* 004dfff4 */
    MI_U32((0x00bcd620u+f->r[7]))=(uint32_t)(f->r[0]);
    /* 004dfffa */
    goto mi_004dfee2;
mi_004dffff:;
    /* 004dffff */
    a=(f->r[1]&0xffu);b=0x0000000au;t=mi_alu(f,a,b,8,1);
    /* 004e0002 */
    if(!(f->flags&MI_ZF))goto mi_004e003b;
    /* 004e0004 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e0005 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e000au;f->boundary(0x004e0005u,0x004b5420u,f);
    /* 004e000a */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x007999deu+f->r[6])))&0xffffu)<<0);
    /* 004e0011 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x007999e0u+f->r[6])))&0xffffu)<<0);
    /* 004e0018 */
    f->r[0]=MI_U32((0x007999e4u+f->r[6]));
    /* 004e001e */
    MI_U16((0x00bcd60cu+f->r[7]))=(uint16_t)((f->r[1]&0xffffu));
    /* 004e0025 */
    MI_U16((0x00bcd60eu+f->r[7]))=(uint16_t)((f->r[2]&0xffffu));
    /* 004e002c */
    MI_U32((0x00bcd618u+f->r[7]))=(uint32_t)(f->r[0]);
mi_004e0032:;
    /* 004e0032 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e0033 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e0038u;f->boundary(0x004e0033u,0x004b52f0u,f);
    /* 004e0038 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004e003b:;
    /* 004e003b */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004e003c */
    f->r[4]-=4;MI_U32(f->r[4])=0x004e0041u;f->boundary(0x004e003cu,0x004dfbb0u,f);
    /* 004e0041 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e0044 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004e0045 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004e0046 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 004e0047 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 004e0048 */
    a=f->r[4];b=0x00000038u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004e004b */
    f->r[4]+=4;return;
}

void restore_rotation_C_004ee560(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004ee560 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee561 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee562 */
    f->r[7]=0x00000001u;
mi_004ee567:;
    /* 004ee567 */
    f->r[6]=f->r[7];
    /* 004ee569 */
    a=f->r[6];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ee56c */
    a=f->r[6];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[6]=t;
    /* 004ee56e */
    a=f->r[6];b=0x00000003u;t=mi_shift(f,a,b,32,0);f->r[6]=t;
    /* 004ee571 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x007999d8u+f->r[6])))&0xffu)<<0);
    /* 004ee577 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004ee579 */
    if((f->flags&MI_ZF))goto mi_004ee5b6;
    /* 004ee57b */
    a=MI_U16((0x007999e8u+f->r[6]));b=0x000000b7u;t=mi_alu(f,a,b,16,1);
    /* 004ee584 */
    if(!(f->flags&MI_ZF))goto mi_004ee5b6;
    /* 004ee586 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004ee587 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004ee58cu;f->boundary(0x004ee587u,0x004ee4a0u,f);
    /* 004ee58c */
    f->r[1]=MI_U32((0x005e4ef4u));
    /* 004ee592 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ee595 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004ee597 */
    a=f->r[1];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 004ee59a */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00799a2cu+f->r[6])))&0xffu)<<0);
    /* 004ee5a0 */
    if(!(f->flags&MI_ZF))goto mi_004ee5ab;
    /* 004ee5a2 */
    f->r[0]=MI_U32((0x0056f7ecu+f->r[0]*4u));
    /* 004ee5a9 */
    goto mi_004ee5b2;
mi_004ee5ab:;
    /* 004ee5ab */
    f->r[0]=MI_U32((0x0056f7d8u+f->r[0]*4u));
mi_004ee5b2:;
    /* 004ee5b2 */
    f->r[7]=(0xffffffffu+f->r[7]+f->r[0]*1u);
mi_004ee5b6:;
    /* 004ee5b6 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 004ee5b7 */
    a=f->r[7];b=0x00000fa0u;t=mi_alu(f,a,b,32,1);
    /* 004ee5bd */
    if(mi_signed_less(f->flags))goto mi_004ee567;
    /* 004ee5bf */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004ee5c0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004ee5c1 */
    f->r[4]+=4;return;
}
