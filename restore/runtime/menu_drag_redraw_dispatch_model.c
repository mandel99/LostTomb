/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "redraw_dispatch_model.h"

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

void restore_C_004dea50(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004dea50 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004dea55 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004dea5b */
    a=f->r[0];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 004dea5e */
    a=f->r[1];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004dea61 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea62 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea63 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea65 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea67 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dea6cu;f->boundary(0x004dea67u,0x0051f780u,f);
    /* 004dea6c */
    f->r[2]=MI_U32((0x00bca2d4u));
    /* 004dea72 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 004dea77 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004dea78 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004dea79 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea7a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea7b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea7d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea7f */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dea84u;f->boundary(0x004dea7fu,0x004770f0u,f);
    /* 004dea84 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dea89u;f->boundary(0x004dea84u,0x00475dc0u,f);
    /* 004dea89 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 004dea8f */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 004dea95 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004dea96 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004dea97 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea98 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea99 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea9b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dea9d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deaa2u;f->boundary(0x004dea9du,0x0051f780u,f);
    /* 004deaa2 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004deaa7 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004deaad */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004deaae */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004deaaf */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deab0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deab1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deab3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deab5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deabau;f->boundary(0x004deab5u,0x00475bf0u,f);
    /* 004deaba */
    a=f->r[4];b=0x00000040u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004deabd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deac2u;f->boundary(0x004deabdu,0x00476530u,f);
    /* 004deac2 */
    f->r[0]=MI_U32((0x00605a3cu));
    /* 004deac7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004deac9 */
    if(!(f->flags&MI_ZF))goto mi_004dead5;
    /* 004deacb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deacd */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dead2u;f->boundary(0x004deacdu,0x0047d870u,f);
    /* 004dead2 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_004dead5:;
    /* 004dead5 */
    f->r[2]=MI_U32((0x00bca2d4u));
    /* 004deadb */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 004deae0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deae2 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004deae3 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deae5 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004deae6 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deae7 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deae8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deaea */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deaec */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deaf1u;f->boundary(0x004deaecu,0x004767d0u,f);
    /* 004deaf1 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 004deaf7 */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 004deafd */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004deafe */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004deaff */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb00 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb01 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb03 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb05 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb0au;f->boundary(0x004deb05u,0x004777c0u,f);
    /* 004deb0a */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004deb0f */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004deb15 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004deb16 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004deb17 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb18 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb19 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb1b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb1d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb22u;f->boundary(0x004deb1du,0x00477500u,f);
    /* 004deb22 */
    f->r[2]=MI_U32((0x00bca2d4u));
    /* 004deb28 */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 004deb2d */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004deb2e */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004deb2f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb30 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb31 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb33 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb35 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb3au;f->boundary(0x004deb35u,0x004776f0u,f);
    /* 004deb3a */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004deb3d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb42u;f->boundary(0x004deb3du,0x00477210u,f);
    /* 004deb42 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 004deb48 */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 004deb4e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004deb4f */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004deb50 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb51 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb52 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb54 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb56 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb5bu;f->boundary(0x004deb56u,0x004777c0u,f);
    /* 004deb5b */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004deb60 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004deb66 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004deb67 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb69 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004deb6a */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb6b */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb6c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb6e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb70 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb75u;f->boundary(0x004deb70u,0x00475cc0u,f);
    /* 004deb75 */
    f->r[2]=MI_U32((0x00bca2d4u));
    /* 004deb7b */
    f->r[0]=MI_U32((0x00bca2d0u));
    /* 004deb80 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004deb81 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004deb82 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb83 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb84 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb86 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deb88 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb8du;f->boundary(0x004deb88u,0x00477890u,f);
    /* 004deb8d */
    f->r[4]-=4;MI_U32(f->r[4])=0x004deb92u;f->boundary(0x004deb8du,0x0047b570u,f);
    /* 004deb92 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 004deb98 */
    f->r[2]=MI_U32((0x00bca2d0u));
    /* 004deb9e */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004deb9f */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deba1 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 004deba2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deba3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deba4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deba6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004deba8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004debadu;f->boundary(0x004deba8u,0x00475720u,f);
    /* 004debad */
    a=f->r[4];b=0x00000048u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004debb0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004debb5u;f->boundary(0x004debb0u,0x0047b1a0u,f);
    /* 004debb5 */
    f->r[0]=MI_U32((0x00bca2d4u));
    /* 004debba */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 004debc0 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 004debc1 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004debc3 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 004debc4 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004debc5 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004debc6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004debc8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004debca */
    f->r[4]-=4;MI_U32(f->r[4])=0x004debcfu;f->boundary(0x004debcau,0x0047bae0u,f);
    /* 004debcf */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004debd2 */
    f->r[4]+=4;return;
}
