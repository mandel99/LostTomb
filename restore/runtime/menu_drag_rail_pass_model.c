/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "surface_passes.h"

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

void restore_C_0047b1a0(struct restore_menu_input_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0047b1a0 */
    f->r[1]=MI_U32((0x00bca2d4u));
    /* 0047b1a6 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 0047b1ab */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047b1ad */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047b1b2 */
    MI_U32((0x00bc9760u))=(uint32_t)(0x00000000u);
    /* 0047b1bc */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047b302;
    /* 0047b1c2 */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 0047b1c8 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b1c9 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b1ca */
    f->r[3]=(f->r[3]&0xffffff00u)|(((0x00000040u)&0xffu)<<0);
mi_0047b1cc:;
    /* 0047b1cc */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0047b1ce */
    MI_U32((0x00bc9764u))=(uint32_t)(0x00000000u);
    /* 0047b1d8 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0047b2d8;
mi_0047b1de:;
    /* 0047b1de */
    f->r[0]=MI_U32((0x00af598cu+f->r[0]*4u));
    /* 0047b1e5 */
    a=((f->r[0]>>8)&0xffu);b=0x00000040u;t=mi_alu(f,a,b,8,3);
    /* 0047b1e8 */
    if((f->flags&MI_ZF))goto mi_0047b2b8;
    /* 0047b1ee */
    a=(f->r[0]&0xffu);b=0x00000008u;t=mi_alu(f,a,b,8,3);
    /* 0047b1f0 */
    if(!(f->flags&MI_ZF))goto mi_0047b2b8;
    /* 0047b1f6 */
    a=0x00004000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b1fb */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b200u;f->boundary(0x0047b1fbu,0x0047db00u,f);
    /* 0047b200 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b205u;f->boundary(0x0047b200u,0x00484f00u,f);
    /* 0047b205 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b207 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b209 */
    a=0x00000030u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b20b */
    a=0x005cb9d8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b210 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b215u;f->boundary(0x0047b210u,0x00484be0u,f);
    /* 0047b215 */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 0047b21a */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047b21d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00b285ccu+f->r[0])))&0xffu)<<0);
    /* 0047b223 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00b67d1cu+f->r[0])))&0xffu)<<0);
    /* 0047b229 */
    a=(f->r[1]&0xffu);b=0x000000f0u;t=mi_alu(f,a,b,8,3);f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047b22c */
    a=(f->r[2]&0xffu);b=(f->r[3]&0xffu);t=mi_alu(f,a,b,8,4);f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0047b22e */
    MI_U8((0x00b285ccu+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
    /* 0047b234 */
    f->r[1]=MI_U32((0x00bc9740u));
    /* 0047b23a */
    MI_U8((0x00b67d1cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0047b240 */
    f->r[2]=MI_U32((0x00f5686cu));
    /* 0047b246 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047b248 */
    f->r[1]=MI_U32((0x00bc9748u));
    /* 0047b24e */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 0047b250 */
    MI_U32((0x00b7482cu+f->r[0]*4u))=(uint32_t)(f->r[2]);
    /* 0047b257 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b25cu;f->boundary(0x0047b257u,0x0047e000u,f);
    /* 0047b25c */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047b25e */
    if((f->flags&MI_ZF))goto mi_0047b2a8;
    /* 0047b260 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b265u;f->boundary(0x0047b260u,0x0047e750u,f);
    /* 0047b265 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b267 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b269 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b26b */
    a=0x005cbcd8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b270 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b275u;f->boundary(0x0047b270u,0x00484be0u,f);
    /* 0047b275 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0047b278 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0047b27a */
    if((f->flags&MI_ZF))goto mi_0047b29e;
    /* 0047b27c */
    f->r[0]=MI_U32((0x00bc9740u));
    /* 0047b281 */
    f->r[1]=MI_U32((0x00f5686cu));
    /* 0047b287 */
    f->r[2]=MI_U32((0x00bc9768u));
    /* 0047b28d */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047b28f */
    a=f->r[1];b=MI_U32((0x00bc9748u));t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 0047b295 */
    MI_U32((0x00b7482cu+f->r[2]*4u))=(uint32_t)(f->r[1]);
    /* 0047b29c */
    goto mi_0047b2a8;
mi_0047b29e:;
    /* 0047b29e */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b2a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b2a5u;f->boundary(0x0047b2a0u,0x0051f780u,f);
    /* 0047b2a5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047b2a8:;
    /* 0047b2a8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0047b2aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x0047b2afu;f->boundary(0x0047b2aau,0x0051f780u,f);
    /* 0047b2af */
    f->r[1]=MI_U32((0x00bca2d0u));
    /* 0047b2b5 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0047b2b8:;
    /* 0047b2b8 */
    f->r[2]=MI_U32((0x00bc9764u));
    /* 0047b2be */
    f->r[0]=MI_U32((0x00bc9768u));
    /* 0047b2c3 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047b2c4 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0047b2c5 */
    a=f->r[2];b=f->r[1];t=mi_alu(f,a,b,32,1);
    /* 0047b2c7 */
    MI_U32((0x00bc9764u))=(uint32_t)(f->r[2]);
    /* 0047b2cd */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047b2d2 */
    if(mi_signed_less(f->flags))goto mi_0047b1de;
mi_0047b2d8:;
    /* 0047b2d8 */
    f->r[7]=MI_U32((0x00bca2dcu));
    /* 0047b2de */
    f->r[2]=MI_U32((0x00bc9760u));
    /* 0047b2e4 */
    a=f->r[0];b=f->r[7];t=mi_alu(f,a,b,32,0);f->r[0]=t;
    /* 0047b2e6 */
    f->r[7]=MI_U32((0x00bca2d4u));
    /* 0047b2ec */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 0047b2ed */
    MI_U32((0x00bc9768u))=(uint32_t)(f->r[0]);
    /* 0047b2f2 */
    a=f->r[2];b=f->r[7];t=mi_alu(f,a,b,32,1);
    /* 0047b2f4 */
    MI_U32((0x00bc9760u))=(uint32_t)(f->r[2]);
    /* 0047b2fa */
    if(mi_signed_less(f->flags))goto mi_0047b1cc;
    /* 0047b300 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0047b301 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
mi_0047b302:;
    /* 0047b302 */
    f->r[4]+=4;return;
}
