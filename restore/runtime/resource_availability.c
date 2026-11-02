/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "resource_availability.h"
#include "training_queries.h"
#include <stdio.h>
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
static __inline uint32_t mi_alu(struct restore_resource_availability_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_resource_availability_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_resource_availability_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}

static const uint8_t mi_rom_0055968c[]={0x49,0x95,0x55,0x00,0x3e,0x95,0x55,0x00,0x12,0x95,0x55,0x00,0x1d,0x95,0x55,0x00,0x33,0x95,0x55,0x00,0x7b,0x95,0x55,0x00,0x62,0x95,0x55,0x00,0x28,0x95,0x55,0x00,0x84,0x96,0x55,0x00};
static const uint8_t mi_rom_005596b0[]={0x00,0x08,0x08,0x01,0x08,0x02,0x08,0x03,0x08,0x04,0x08,0x08,0x08,0x05,0x08,0x08,0x08,0x08,0x06,0x08,0x08,0x08,0x08,0x07};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x0055968cu&&width<=36u&&(offset=address-0x0055968cu)<=36u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0055968c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x005596b0u&&width<=24u&&(offset=address-0x005596b0u)<=24u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_005596b0[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}

void restore_avail_C_00559480(struct restore_resource_availability_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00559480 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559481 */
    f->r[6]=mi_read((0x00785d70u),4u);
    /* 00559487 */
    f->r[2]=mi_read((0x00000008u+f->r[4]),4u);
    /* 0055948b */
    f->r[0]=f->r[6];
    /* 0055948d */
    f->r[1]=f->r[0];
    /* 0055948f */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00559490 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00559492 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559493 */
    if((f->flags&MI_ZF))goto mi_005594e5;
    /* 00559495 */
    f->r[1]=f->r[0];
    /* 00559497 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0055949a */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0055949c */
    f->r[1]=(0x00785d7cu+f->r[1]*4u);
mi_005594a3:;
    /* 005594a3 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005594a5 */
    if(mi_signed_less(f->flags))goto mi_005594db;
    /* 005594a7 */
    a=f->r[0];b=f->r[6];t=mi_alu(f,a,b,32,1);
    /* 005594a9 */
    if(!mi_signed_less(f->flags))goto mi_005594db;
    /* 005594ab */
    f->r[7]=(0xfffffff8u+f->r[1]);
    /* 005594ae */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 005594b0 */
    if((f->flags&MI_ZF))goto mi_005594db;
    /* 005594b2 */
    a=mi_read((0xfffffffau+f->r[1]),1u);b=0x00000017u;t=mi_alu(f,a,b,8,1);
    /* 005594b6 */
    if(!(f->flags&MI_ZF))goto mi_005594db;
    /* 005594b8 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0xfffffffeu+f->r[1]),2u);
    /* 005594bc */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 005594be */
    if((f->flags&MI_ZF))goto mi_0055967f;
    /* 005594c4 */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000000u+f->r[1]),2u);
    /* 005594c7 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 005594c9 */
    if((f->flags&MI_ZF))goto mi_0055967f;
    /* 005594cf */
    f->r[7]=(uint32_t)(int32_t)(int16_t)mi_read((0x00000002u+f->r[1]),2u);
    /* 005594d3 */
    a=f->r[7];b=f->r[2];t=mi_alu(f,a,b,32,1);
    /* 005594d5 */
    if((f->flags&MI_ZF))goto mi_0055967f;
mi_005594db:;
    /* 005594db */
    f->r[7]=f->r[0];
    /* 005594dd */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005594de */
    a=f->r[1];b=0x0000007cu;t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 005594e1 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 005594e3 */
    if(!(f->flags&MI_ZF))goto mi_005594a3;
mi_005594e5:;
    /* 005594e5 */
    a=f->r[2];b=0x0000000cu;t=mi_alu(f,a,b,32,1);
    /* 005594e8 */
    if((f->flags&MI_ZF))goto mi_00559586;
    /* 005594ee */
    a=f->r[2];b=0x00000022u;t=mi_alu(f,a,b,32,1);
    /* 005594f1 */
    if((f->flags&MI_ZF))goto mi_00559639;
    /* 005594f7 */
    f->r[0]=(0xfffffff6u+f->r[2]);
    /* 005594fa */
    a=f->r[0];b=0x00000017u;t=mi_alu(f,a,b,32,1);
    /* 005594fd */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_00559684;
    /* 00559503 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,5);f->r[2]=t;
    /* 00559505 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((mi_read((0x005596b0u+f->r[0]),1u))&0xffu)<<0);
    /* 0055950b */
    switch(mi_read((0x0055968cu+f->r[2]*4u),4u)){case 0x00559512u:goto mi_00559512;case 0x0055951du:goto mi_0055951d;case 0x00559528u:goto mi_00559528;case 0x00559533u:goto mi_00559533;case 0x0055953eu:goto mi_0055953e;case 0x00559549u:goto mi_00559549;case 0x00559562u:goto mi_00559562;case 0x0055957bu:goto mi_0055957b;case 0x00559684u:goto mi_00559684;default:abort();}
mi_00559512:;
    /* 00559512 */
    f->r[0]=0x0000000eu;
    /* 00559517 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559518 */
    goto mi_00559628;
mi_0055951d:;
    /* 0055951d */
    f->r[0]=0x00000010u;
    /* 00559522 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559523 */
    goto mi_00559628;
mi_00559528:;
    /* 00559528 */
    f->r[0]=0x00000020u;
    /* 0055952d */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055952e */
    goto mi_00559628;
mi_00559533:;
    /* 00559533 */
    f->r[0]=0x00000012u;
    /* 00559538 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559539 */
    goto mi_00559628;
mi_0055953e:;
    /* 0055953e */
    f->r[0]=0x0000000bu;
    /* 00559543 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559544 */
    goto mi_00559628;
mi_00559549:;
    /* 00559549 */
    a=mi_read((0x00784d32u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00559551 */
    if((f->flags&MI_ZF))goto mi_00559684;
    /* 00559557 */
    f->r[0]=0x0000001du;
    /* 0055955c */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055955d */
    goto mi_00559628;
mi_00559562:;
    /* 00559562 */
    a=mi_read((0x00784d34u),2u);b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0055956a */
    if((f->flags&MI_ZF))goto mi_00559684;
    /* 00559570 */
    f->r[0]=0x00000014u;
    /* 00559575 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559576 */
    goto mi_00559628;
mi_0055957b:;
    /* 0055957b */
    f->r[0]=0x00000016u;
    /* 00559580 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559581 */
    goto mi_00559628;
mi_00559586:;
    /* 00559586 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559588 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055958du;f->boundary(0x00559588u,0x00443b90u,f);
    /* 0055958d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559590 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559592 */
    if(!(f->flags&MI_ZF))goto mi_005595a2;
    /* 00559594 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559596 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055959bu;f->boundary(0x00559596u,0x00443d90u,f);
    /* 0055959b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055959e */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005595a0 */
    if((f->flags&MI_ZF))goto mi_005595b5;
mi_005595a2:;
    /* 005595a2 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005595a4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005595a9u;f->boundary(0x005595a4u,0x00559480u,f);
    /* 005595a9 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005595ac */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005595ae */
    if((f->flags&MI_ZF))goto mi_005595b5;
    /* 005595b0 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005595b1 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 005595b3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005595b4 */
    f->r[4]+=4;return;
mi_005595b5:;
    /* 005595b5 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005595b7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005595bcu;f->boundary(0x005595b7u,0x00559480u,f);
    /* 005595bc */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005595bf */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005595c1 */
    if((f->flags&MI_ZF))goto mi_005595d6;
    /* 005595c3 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005595c5 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005595cau;f->boundary(0x005595c5u,0x00443b90u,f);
    /* 005595ca */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005595cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005595cf */
    if((f->flags&MI_ZF))goto mi_005595d6;
    /* 005595d1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005595d2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 005595d4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005595d5 */
    f->r[4]+=4;return;
mi_005595d6:;
    /* 005595d6 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005595d8 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005595ddu;f->boundary(0x005595d8u,0x004439c0u,f);
    /* 005595dd */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005595e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005595e2 */
    if((f->flags&MI_ZF))goto mi_005595f7;
    /* 005595e4 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005595e6 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005595ebu;f->boundary(0x005595e6u,0x00559480u,f);
    /* 005595eb */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 005595ee */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005595f0 */
    if((f->flags&MI_ZF))goto mi_005595f7;
    /* 005595f2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 005595f3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 005595f5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 005595f6 */
    f->r[4]+=4;return;
mi_005595f7:;
    /* 005595f7 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005595f9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x005595feu;f->boundary(0x005595f9u,0x00559480u,f);
    /* 005595fe */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559601 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559603 */
    if((f->flags&MI_ZF))goto mi_00559618;
    /* 00559605 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559607 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055960cu;f->boundary(0x00559607u,0x004439c0u,f);
    /* 0055960c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055960f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559611 */
    if((f->flags&MI_ZF))goto mi_00559618;
    /* 00559613 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559614 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00559616 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00559617 */
    f->r[4]+=4;return;
mi_00559618:;
    /* 00559618 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055961a */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055961fu;f->boundary(0x0055961au,0x00559480u,f);
    /* 0055961f */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559622 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559624 */
    if((f->flags&MI_ZF))goto mi_00559684;
    /* 00559626 */
    a=0x00000009u;f->r[4]-=4;MI_U32(f->r[4])=a;
mi_00559628:;
    /* 00559628 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055962du;f->boundary(0x00559628u,0x00559480u,f);
    /* 0055962d */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559630 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559632 */
    if((f->flags&MI_ZF))goto mi_00559684;
    /* 00559634 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559635 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00559637 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00559638 */
    f->r[4]+=4;return;
mi_00559639:;
    /* 00559639 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055963b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559640u;f->boundary(0x0055963bu,0x00443b90u,f);
    /* 00559640 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559643 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559645 */
    if(!(f->flags&MI_ZF))goto mi_00559671;
    /* 00559647 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559649 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055964eu;f->boundary(0x00559649u,0x00443d90u,f);
    /* 0055964e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559651 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559653 */
    if(!(f->flags&MI_ZF))goto mi_00559671;
    /* 00559655 */
    a=0x0000000bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559657 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055965cu;f->boundary(0x00559657u,0x00559480u,f);
    /* 0055965c */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055965f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559661 */
    if(!(f->flags&MI_ZF))goto mi_00559671;
    /* 00559663 */
    a=0x0000000du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559665 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055966au;f->boundary(0x00559665u,0x00559480u,f);
    /* 0055966a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055966d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0055966f */
    if((f->flags&MI_ZF))goto mi_00559684;
mi_00559671:;
    /* 00559671 */
    a=0x0000001fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559673 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559678u;f->boundary(0x00559673u,0x004439c0u,f);
    /* 00559678 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055967b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055967d */
    if((f->flags&MI_ZF))goto mi_00559684;
mi_0055967f:;
    /* 0055967f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559680 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00559682 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00559683 */
    f->r[4]+=4;return;
mi_00559684:;
    /* 00559684 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00559685 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00559687 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00559688 */
    f->r[4]+=4;return;
}

void restore_avail_C_005596d0(struct restore_resource_availability_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 005596d0 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005596d1 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005596d2 */
    f->r[1]=0x00000024u;
    /* 005596d7 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 005596d9 */
    f->r[7]=0x00f53e70u;
    /* 005596de */
    MI_U32((0x00f53e6cu))=(uint32_t)(f->r[0]);
    /* 005596e3 */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 005596e5 */
    f->r[1]=0x00000024u;
    /* 005596ea */
    f->r[7]=0x00f53ddcu;
    /* 005596ef */
    while(f->r[1]){MI_U32(f->r[7])=f->r[0];f->r[7]+=(f->flags&0x400u)?0xfffffffcu:4u;--f->r[1];}
    /* 005596f1 */
    MI_U32((0x00f53dd8u))=(uint32_t)(f->r[0]);
    /* 005596f6 */
    f->r[6]=0x00000001u;
mi_005596fb:;
    /* 005596fb */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 005596fc */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559701u;f->boundary(0x005596fcu,0x00443b90u,f);
    /* 00559701 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559704 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559706 */
    if(!(f->flags&MI_ZF))goto mi_00559738;
    /* 00559708 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559709 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055970eu;f->boundary(0x00559709u,0x004439c0u,f);
    /* 0055970e */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559711 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559713 */
    if(!(f->flags&MI_ZF))goto mi_00559738;
    /* 00559715 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559716 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055971bu;f->boundary(0x00559716u,0x00559480u,f);
    /* 0055971b */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055971e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559720 */
    if(!(f->flags&MI_ZF))goto mi_00559738;
    /* 00559722 */
    f->r[0]=MI_U32((0x00eaac28u));
    /* 00559727 */
    a=f->r[0];b=0x000049d8u;t=mi_imul(f,a,b);f->r[0]=t;
    /* 0055972d */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00ea4205u+f->r[0]+f->r[6]*1u)))&0xffu)<<0);
    /* 00559734 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559736 */
    if((f->flags&MI_ZF))goto mi_0055974a;
mi_00559738:;
    /* 00559738 */
    f->r[0]=MI_U32((0x00f53e6cu));
    /* 0055973d */
    MI_U32((0x00f53e70u+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 00559744 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00559745 */
    MI_U32((0x00f53e6cu))=(uint32_t)(f->r[0]);
mi_0055974a:;
    /* 0055974a */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0055974b */
    a=f->r[6];b=0x00000024u;t=mi_alu(f,a,b,32,1);
    /* 0055974e */
    if(mi_signed_less(f->flags))goto mi_005596fb;
    /* 00559750 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,5);f->r[6]=t;
mi_00559752:;
    /* 00559752 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00559753 */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00559756 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005597b6;
mi_00559758:;
    /* 00559758 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0055975a */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_00559764;
    /* 0055975c */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 0055975d */
    a=f->r[6];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 00559760 */
    if(!(f->flags&MI_ZF)&&!mi_signed_less(f->flags))goto mi_005597b6;
    /* 00559762 */
    goto mi_00559758;
mi_00559764:;
    /* 00559764 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559765 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0055976au;f->boundary(0x00559765u,0x00443b90u,f);
    /* 0055976a */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055976d */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055976f */
    if(!(f->flags&MI_ZF))goto mi_005597a2;
    /* 00559771 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00559772 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559777u;f->boundary(0x00559772u,0x004439c0u,f);
    /* 00559777 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0055977a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0055977c */
    if(!(f->flags&MI_ZF))goto mi_005597a2;
    /* 0055977e */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0055977f */
    f->r[4]-=4;MI_U32(f->r[4])=0x00559784u;f->boundary(0x0055977fu,0x00559480u,f);
    /* 00559784 */
    a=f->r[4];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00559787 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00559789 */
    if(!(f->flags&MI_ZF))goto mi_005597a2;
    /* 0055978b */
    f->r[1]=MI_U32((0x00eaac28u));
    /* 00559791 */
    a=f->r[1];b=0x000049d8u;t=mi_imul(f,a,b);f->r[1]=t;
    /* 00559797 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00ea41fcu+f->r[1]+f->r[6]*1u)))&0xffu)<<0);
    /* 0055979e */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 005597a0 */
    if((f->flags&MI_ZF))goto mi_00559752;
mi_005597a2:;
    /* 005597a2 */
    f->r[0]=MI_U32((0x00f53dd8u));
    /* 005597a7 */
    MI_U32((0x00f53ddcu+f->r[0]*4u))=(uint32_t)(f->r[6]);
    /* 005597ae */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 005597af */
    MI_U32((0x00f53dd8u))=(uint32_t)(f->r[0]);
    /* 005597b4 */
    goto mi_00559752;
mi_005597b6:;
    /* 005597b6 */
    f->r[2]=MI_U32((0x00f53e6cu));
    /* 005597bc */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005597be */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_005597e9;
    /* 005597c0 */
    f->r[6]=MI_U32((0x00eaac28u));
    /* 005597c6 */
    f->r[1]=0x00f53e70u;
mi_005597cb:;
    /* 005597cb */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 005597cd */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 005597cf */
    if((f->flags&MI_ZF))goto mi_005597e1;
    /* 005597d1 */
    f->r[7]=f->r[6];
    /* 005597d3 */
    a=f->r[7];b=0x000049d8u;t=mi_imul(f,a,b);f->r[7]=t;
    /* 005597d9 */
    MI_U8((0x00ea4205u+f->r[7]+f->r[0]*1u))=(uint8_t)(0x00000001u);
mi_005597e1:;
    /* 005597e1 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 005597e4 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 005597e5 */
    if(!(f->flags&MI_ZF))goto mi_005597cb;
    /* 005597e7 */
    goto mi_005597ef;
mi_005597e9:;
    /* 005597e9 */
    f->r[6]=MI_U32((0x00eaac28u));
mi_005597ef:;
    /* 005597ef */
    f->r[2]=MI_U32((0x00f53dd8u));
    /* 005597f5 */
    a=f->r[2];b=f->r[2];t=mi_alu(f,a,b,32,3);
    /* 005597f7 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0055981a;
    /* 005597f9 */
    f->r[1]=0x00f53ddcu;
mi_005597fe:;
    /* 005597fe */
    f->r[0]=MI_U32((0x00000000u+f->r[1]));
    /* 00559800 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00559802 */
    if((f->flags&MI_ZF))goto mi_00559814;
    /* 00559804 */
    f->r[7]=f->r[6];
    /* 00559806 */
    a=f->r[7];b=0x000049d8u;t=mi_imul(f,a,b);f->r[7]=t;
    /* 0055980c */
    MI_U8((0x00ea41fcu+f->r[7]+f->r[0]*1u))=(uint8_t)(0x00000001u);
mi_00559814:;
    /* 00559814 */
    a=f->r[1];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[1]=t;
    /* 00559817 */
    a=f->r[2];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[2]=t;
    /* 00559818 */
    if(!(f->flags&MI_ZF))goto mi_005597fe;
mi_0055981a:;
    /* 0055981a */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0055981b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0055981c */
    f->r[4]+=4;return;
}

__declspec(noinline) __declspec(noreturn) void restore_resource_availability_failure(uint32_t target)
{fprintf(stderr,"Unsupported resource availability target %08lx\n",(unsigned long)target);abort();}
void restore_resource_availability_dispatch(uint32_t site,uint32_t target,struct restore_resource_availability_frame *f)
{
 uint32_t sp=f->r[4],argument; (void)site;
 if(target==0x559480u){restore_avail_C_00559480(f);return;}
 argument=MI_U32(sp+4u);
 switch(target){
 case 0x4439c0u:f->r[0]=FUN_004439c0((int32_t)argument);break;
 case 0x443b90u:f->r[0]=FUN_00443b90((int32_t)argument);break;
 case 0x443d90u:f->r[0]=FUN_00443d90((int32_t)argument);break;
 default:restore_resource_availability_failure(target);
 }
 f->r[4]=sp+4u;
}
static uint32_t availability_run(uint32_t target,uint32_t argument)
{
 uint32_t stack[256],i,entry;struct restore_resource_availability_frame frame={{0},2,restore_resource_availability_dispatch};
 for(i=0;i<256;++i)stack[i]=0xa5a55a5au;
 entry=(uint32_t)(uintptr_t)&stack[240];frame.r[4]=entry;stack[240]=0;stack[241]=argument;
 if(target==0x559480u)restore_avail_C_00559480(&frame);else restore_avail_C_005596d0(&frame);
 if(frame.r[4]!=entry+4u)restore_resource_availability_failure(target);
 for(i=0;i<16;++i)if(stack[i]!=0xa5a55a5au)restore_resource_availability_failure(target);
 for(i=242;i<256;++i)if(stack[i]!=0xa5a55a5au)restore_resource_availability_failure(target);
 return frame.r[0];
}
uint32_t FUN_00559480(int32_t resource){return availability_run(0x559480u,(uint32_t)resource);}
void FUN_005596d0(void){(void)availability_run(0x5596d0u,0);}
