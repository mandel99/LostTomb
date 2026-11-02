/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "world_dispatch_model.h"
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
static __inline uint32_t mi_alu(struct restore_building_world_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_building_world_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_building_world_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

void restore_building_world_00430d30(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00430d30 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00430d34 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00430d38 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00430d3c */
    MI_U32((0x0070e478u))=(uint32_t)(f->r[0]);
    /* 00430d41 */
    f->r[0]=MI_U32((0x0070e43cu));
    /* 00430d46 */
    MI_U32((0x0070e474u))=(uint32_t)(f->r[1]);
    /* 00430d4c */
    f->r[1]=MI_U32((0x0070e438u));
    /* 00430d52 */
    MI_U32((0x0070e484u))=(uint32_t)(f->r[2]);
    /* 00430d58 */
    f->r[2]=MI_U32((0x0070e434u));
    /* 00430d5e */
    MI_U32((0x00d3b55cu))=(uint32_t)(f->r[0]);
    /* 00430d63 */
    f->r[0]=MI_U32((0x0070e430u));
    /* 00430d68 */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[1]);
    /* 00430d6e */
    f->r[1]=MI_U32((0x00e92e0cu));
    /* 00430d74 */
    MI_U32((0x00d3b554u))=(uint32_t)(f->r[2]);
    /* 00430d7a */
    f->r[2]=MI_U32((0x0070e3c0u));
    /* 00430d80 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[0]);
    /* 00430d85 */
    f->r[0]=f->r[1];
    /* 00430d87 */
    MI_U32((0x0070e3c4u))=(uint32_t)(0x00000000u);
    /* 00430d91 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00430d93 */
    a=f->r[0];b=0x0000003cu;t=mi_alu(f,a,b,32,1);
    /* 00430d96 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00430d9c;
    /* 00430d98 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00430d9a */
    if(!mi_signed_less(f->flags))goto mi_00430dac;
mi_00430d9c:;
    /* 00430d9c */
    MI_U32((0x0070e3c0u))=(uint32_t)(f->r[1]);
    /* 00430da2 */
    MI_U32((0x0070e3c4u))=(uint32_t)(0x00000001u);
mi_00430dac:;
    /* 00430dac */
    f->r[0]=MI_U32((0x00e94e0cu));
    /* 00430db1 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00430db3 */
    if(!(f->flags&MI_ZF))goto mi_00430dc5;
    /* 00430db5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x006de515u)))&0xffu)<<0);
    /* 00430dba */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00430dbc */
    if(!(f->flags&MI_ZF))goto mi_00430df6;
    /* 00430dbe */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430dc3u;f->boundary(0x00430dbeu,0x00430ef0u,f);
    /* 00430dc3 */
    goto mi_00430dfb;
mi_00430dc5:;
    /* 00430dc5 */
    a=f->r[0];b=0x00000033u;t=mi_alu(f,a,b,32,1);
    /* 00430dc8 */
    if((f->flags&MI_ZF))goto mi_00430df6;
    /* 00430dca */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x006de515u)))&0xffu)<<0);
    /* 00430dcf */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00430dd1 */
    if(!(f->flags&MI_ZF))goto mi_00430df6;
    /* 00430dd3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430dd8u;f->boundary(0x00430dd3u,0x004316f0u,f);
    /* 00430dd8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430dddu;f->boundary(0x00430dd8u,0x0051f780u,f);
    /* 00430ddd */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430de2u;f->boundary(0x00430dddu,0x00439f10u,f);
    /* 00430de2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430de7u;f->boundary(0x00430de2u,0x0043b630u,f);
    /* 00430de7 */
    a=0x0000270fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430dec */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430df1u;f->boundary(0x00430decu,0x00439740u,f);
    /* 00430df1 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430df4 */
    goto mi_00430e16;
mi_00430df6:;
    /* 00430df6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430dfbu;f->boundary(0x00430df6u,0x004316f0u,f);
mi_00430dfb:;
    /* 00430dfb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430dfd */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430e02u;f->boundary(0x00430dfdu,0x00436b60u,f);
    /* 00430e02 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430e07u;f->boundary(0x00430e02u,0x0051f780u,f);
    /* 00430e07 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430e0cu;f->boundary(0x00430e07u,0x0043b630u,f);
    /* 00430e0c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430e0e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430e13u;f->boundary(0x00430e0eu,0x00439740u,f);
    /* 00430e13 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_00430e16:;
    /* 00430e16 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430e1bu;f->boundary(0x00430e16u,0x0043e300u,f);
    /* 00430e1b */
    f->r[0]=MI_U32((0x00775610u));
    /* 00430e20 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00430e22 */
    if((f->flags&MI_ZF))goto mi_00430e2a;
    /* 00430e24 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00430e25 */
    MI_U32((0x00775610u))=(uint32_t)(f->r[0]);
mi_00430e2a:;
    /* 00430e2a */
    f->r[1]=MI_U32((0x00e38db0u));
    /* 00430e30 */
    f->r[2]=MI_U32((0x00e38dacu));
    /* 00430e36 */
    MI_U32((0x00d3b55cu))=(uint32_t)(0x00000000u);
    /* 00430e40 */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[1]);
    /* 00430e46 */
    MI_U32((0x00d3b554u))=(uint32_t)(0x00000000u);
    /* 00430e50 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[2]);
    /* 00430e56 */
    f->r[4]+=4;return;
}

void restore_building_world_00430e60(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00430e60 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00430e64 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 00430e68 */
    f->r[2]=MI_U32((0x0000000cu+f->r[4]));
    /* 00430e6c */
    MI_U32((0x0070e478u))=(uint32_t)(f->r[0]);
    /* 00430e71 */
    f->r[0]=MI_U32((0x0070e43cu));
    /* 00430e76 */
    MI_U32((0x0070e474u))=(uint32_t)(f->r[1]);
    /* 00430e7c */
    f->r[1]=MI_U32((0x0070e438u));
    /* 00430e82 */
    MI_U32((0x0070e484u))=(uint32_t)(f->r[2]);
    /* 00430e88 */
    f->r[2]=MI_U32((0x0070e434u));
    /* 00430e8e */
    MI_U32((0x00d3b55cu))=(uint32_t)(f->r[0]);
    /* 00430e93 */
    f->r[0]=MI_U32((0x0070e430u));
    /* 00430e98 */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[1]);
    /* 00430e9e */
    MI_U32((0x00d3b554u))=(uint32_t)(f->r[2]);
    /* 00430ea4 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[0]);
    /* 00430ea9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430eaeu;f->boundary(0x00430ea9u,0x00430ef0u,f);
    /* 00430eae */
    f->r[1]=MI_U32((0x00000010u+f->r[4]));
    /* 00430eb2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430eb3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430eb8u;f->boundary(0x00430eb3u,0x00436b60u,f);
    /* 00430eb8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00430eba */
    f->r[4]-=4;MI_U32(f->r[4])=0x00430ebfu;f->boundary(0x00430ebau,0x00439740u,f);
    /* 00430ebf */
    f->r[2]=MI_U32((0x00e38db0u));
    /* 00430ec5 */
    f->r[0]=MI_U32((0x00e38dacu));
    /* 00430eca */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00430ecd */
    MI_U32((0x00d3b55cu))=(uint32_t)(0x00000000u);
    /* 00430ed7 */
    MI_U32((0x00d3b558u))=(uint32_t)(f->r[2]);
    /* 00430edd */
    MI_U32((0x00d3b554u))=(uint32_t)(0x00000000u);
    /* 00430ee7 */
    MI_U32((0x00d3b550u))=(uint32_t)(f->r[0]);
    /* 00430eec */
    f->r[4]+=4;return;
}

void restore_building_world_004dddc0(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004dddc0 */
    f->r[0]=MI_U32((0x00bca2d8u));
    /* 004dddc5 */
    f->r[1]=MI_U32((0x00bca2c0u));
    /* 004dddcb */
    f->r[2]=MI_U32((0x00bca2bcu));
    /* 004dddd1 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dddd2 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dddd3 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004dddd4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004dddd9u;f->boundary(0x004dddd4u,0x00430d30u,f);
    /* 004dddd9 */
    a=f->r[4];b=0x0000000cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004ddddc */
    f->r[4]+=4;return;
}

void restore_building_world_004fa330(struct restore_building_world_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004fa330 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 004fa334 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa335 */
    f->r[6]=MI_U32((0x00bca2bcu));
    /* 004fa33b */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa33c */
    f->r[7]=MI_U32((0x00bca2c0u));
    /* 004fa342 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa343 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa348u;f->boundary(0x004fa343u,0x004304c0u,f);
    /* 004fa348 */
    f->r[1]=MI_U32((0x00d3b4e4u));
    /* 004fa34e */
    f->r[2]=MI_U32((0x00d3b4e0u));
    /* 004fa354 */
    a=f->r[1];b=0xfffffffeu;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 004fa357 */
    a=f->r[2];b=0xfffffffau;t=mi_alu(f,a,b,32,0);f->r[2]=t;
    /* 004fa35a */
    MI_U32((0x00bca2bcu))=(uint32_t)(f->r[1]);
    /* 004fa360 */
    MI_U32((0x00bca2c0u))=(uint32_t)(f->r[2]);
    /* 004fa366 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa36bu;f->boundary(0x004fa366u,0x004de080u,f);
    /* 004fa36b */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa370u;f->boundary(0x004fa36bu,0x004ddf70u,f);
    /* 004fa370 */
    f->r[0]=MI_U32((0x00000014u+f->r[4]));
    /* 004fa374 */
    f->r[1]=MI_U32((0x00bca2d8u));
    /* 004fa37a */
    f->r[2]=MI_U32((0x00bca2c0u));
    /* 004fa380 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa381 */
    f->r[0]=MI_U32((0x00bca2bcu));
    /* 004fa386 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa387 */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa388 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004fa389 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004fa38eu;f->boundary(0x004fa389u,0x00430e60u,f);
    /* 004fa38e */
    f->r[0]=MI_U32((0x00f53f78u));
    /* 004fa393 */
    a=f->r[4];b=0x00000014u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004fa396 */
    MI_U32((0x00bca2c0u))=(uint32_t)(f->r[7]);
    /* 004fa39c */
    MI_U32((0x00bca2bcu))=(uint32_t)(f->r[6]);
    /* 004fa3a2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004fa3a3 */
    a=f->r[0];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 004fa3a6 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004fa3a7 */
    if(!(f->flags&MI_ZF))goto mi_004fa3ae;
    /* 004fa3a9 */
    f->boundary(0x004fa3a9u,0x004ddec0u,f);return;
mi_004fa3ae:;
    /* 004fa3ae */
    f->boundary(0x004fa3aeu,0x004ddf20u,f);return;
}
