/* Direct instruction-to-C translation. External calls use the explicit frame boundary. */
#include "editor_event_detail_actions.h"
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
static __inline uint32_t mi_alu(struct restore_event_detail_frame *f,uint32_t a,uint32_t b,uint32_t width,uint32_t operation)
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
static __inline uint32_t mi_shift(struct restore_event_detail_frame *f,uint32_t a,uint32_t count,uint32_t width,uint32_t arithmetic)
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
static __inline uint32_t mi_imul(struct restore_event_detail_frame *f,uint32_t a,uint32_t b)
{
 int64_t product=(int64_t)(int32_t)a*(int32_t)b;uint32_t result=(uint32_t)product;
 f->flags&=~(MI_CF|MI_OF);if(product!=(int64_t)(int32_t)result)f->flags|=MI_CF|MI_OF;
 return result;
}
static __inline int mi_signed_less(uint32_t flags){return ((flags&MI_SF)!=0)^((flags&MI_OF)!=0);}


static const uint8_t mi_rom_004494bc[]={0x51,0x94,0x44,0x00,0x69,0x94,0x44,0x00,0x69,0x94,0x44,0x00,0x74,0x94,0x44,0x00,0x8d,0x94,0x44,0x00,0xa8,0x94,0x44,0x00,0x51,0x94,0x44,0x00};
static const uint8_t mi_rom_004494d8[]={0x64,0x94,0x44,0x00,0xb5,0x94,0x44,0x00};
static const uint8_t mi_rom_004494e0[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_00449504[]={0x64,0x94,0x44,0x00,0xb5,0x94,0x44,0x00};
static const uint8_t mi_rom_0044950c[]={0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x01,0x01,0x01,0x00,0x01,0x01,0x01,0x01,0x01,0x00};
static const uint8_t mi_rom_0044a460[]={0x08,0xa4,0x44,0x00,0x0f,0xa4,0x44,0x00,0x16,0xa4,0x44,0x00,0x1d,0xa4,0x44,0x00};
static __inline uint32_t mi_read(uint32_t address,uint32_t width)
{
 uint32_t offset,k,result;
 if(address>=0x004494bcu&&width<=28u&&(offset=address-0x004494bcu)<=28u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004494bc[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004494d8u&&width<=8u&&(offset=address-0x004494d8u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004494d8[offset+k]<<(k*8u);return result;
 }
 if(address>=0x004494e0u&&width<=36u&&(offset=address-0x004494e0u)<=36u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_004494e0[offset+k]<<(k*8u);return result;
 }
 if(address>=0x00449504u&&width<=8u&&(offset=address-0x00449504u)<=8u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_00449504[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044950cu&&width<=25u&&(offset=address-0x0044950cu)<=25u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044950c[offset+k]<<(k*8u);return result;
 }
 if(address>=0x0044a460u&&width<=16u&&(offset=address-0x0044a460u)<=16u-width){
  result=0;for(k=0;k<width;++k)result|=(uint32_t)mi_rom_0044a460[offset+k]<<(k*8u);return result;
 }
 if(width==1)return MI_U8(address);if(width==2)return MI_U16(address);return MI_U32(address);
}
void restore_event_detail_C_004493b0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004493b0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004493b2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 004493b5 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x005d9c58u+f->r[0])))&0xffu)<<0);
    /* 004493bb */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004493d0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004493d0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004493d2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 004493d5 */
    f->r[0]=MI_U32((0x005d9c98u+f->r[0]*4u));
    /* 004493dc */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004493e0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004493e0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004493e2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 004493e5 */
    f->r[0]=MI_U32((0x005d9d10u+f->r[0]*4u));
    /* 004493ec */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004493f0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004493f0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004493f1 */
    a=f->r[5];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004493f2 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004493f3 */
    f->r[3]=f->r[1];
    /* 004493f5 */
    a=f->r[5];b=f->r[5];t=mi_alu(f,a,b,32,5);f->r[5]=t;
    /* 004493f7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004493fcu;f->boundary(0x004493f7u,0x004493d0u,f);
    /* 004493fc */
    f->r[1]=f->r[3];
    /* 004493fe */
    f->r[6]=f->r[0];
    /* 00449400 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449405u;f->boundary(0x00449400u,0x004493e0u,f);
    /* 00449405 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00449407 */
    if(!mi_signed_less(f->flags))goto mi_00449434;
    /* 00449409 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044940a */
    f->r[7]=0x006057f4u;
mi_0044940f:;
    /* 0044940f */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449410 */
    f->r[1]=f->r[3];
    /* 00449412 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449417u;f->boundary(0x00449412u,0x00449550u,f);
    /* 00449417 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449419 */
    if((f->flags&MI_ZF))goto mi_00449421;
    /* 0044941b */
    MI_U32((0x00000000u+f->r[7]))=(uint32_t)(f->r[6]);
    /* 0044941d */
    a=f->r[5];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[5]=t;
    /* 0044941e */
    a=f->r[7];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[7]=t;
mi_00449421:;
    /* 00449421 */
    f->r[1]=f->r[3];
    /* 00449423 */
    a=f->r[6];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[6]=t;
    /* 00449424 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449429u;f->boundary(0x00449424u,0x004493e0u,f);
    /* 00449429 */
    a=f->r[6];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044942b */
    if(mi_signed_less(f->flags))goto mi_0044940f;
    /* 0044942d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044942e */
    f->r[0]=f->r[5];
    /* 00449430 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00449431 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00449432 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00449433 */
    f->r[4]+=4;return;
mi_00449434:;
    /* 00449434 */
    f->r[0]=f->r[5];
    /* 00449436 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00449437 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[5]=a;
    /* 00449438 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00449439 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449440(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449440 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00449442 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((mi_read((0x00000060u+f->r[1]),1u))&0xffu)<<0);
    /* 00449445 */
    a=f->r[0];b=0x00000006u;t=mi_alu(f,a,b,32,1);
    /* 00449448 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004494b5;
    /* 0044944a */
    switch(mi_read((0x004494bcu+f->r[0]*4u),4u)){case 0x00449451u:goto mi_00449451;case 0x00449469u:goto mi_00449469;case 0x00449474u:goto mi_00449474;case 0x0044948du:goto mi_0044948d;case 0x004494a8u:goto mi_004494a8;default:abort();}
mi_00449451:;
    /* 00449451 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00449455 */
    a=f->r[0];b=0x00000025u;t=mi_alu(f,a,b,32,1);
    /* 00449458 */
    if((f->flags&MI_ZF))goto mi_004494b5;
    /* 0044945a */
    a=f->r[0];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 0044945d */
    if((f->flags&MI_ZF))goto mi_004494b5;
    /* 0044945f */
    a=f->r[0];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 00449462 */
    if((f->flags&MI_ZF))goto mi_004494b5;
mi_00449464:;
    /* 00449464 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00449466 */
    f->r[4]+=8;return;
mi_00449469:;
    /* 00449469 */
    a=mi_read((0x00000004u+f->r[4]),4u);b=0x00000025u;t=mi_alu(f,a,b,32,1);
    /* 0044946e */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449471 */
    f->r[4]+=8;return;
mi_00449474:;
    /* 00449474 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00449478 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00449479 */
    a=f->r[0];b=0x00000023u;t=mi_alu(f,a,b,32,1);
    /* 0044947c */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004494b5;
    /* 0044947e */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00449480 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x004494e0u+f->r[0]),1u))&0xffu)<<0);
    /* 00449486 */
    switch(mi_read((0x004494d8u+f->r[1]*4u),4u)){case 0x00449464u:goto mi_00449464;case 0x004494b5u:goto mi_004494b5;default:abort();}
mi_0044948d:;
    /* 0044948d */
    f->r[2]=mi_read((0x00000004u+f->r[4]),4u);
    /* 00449491 */
    f->r[0]=(0xfffffff4u+f->r[2]);
    /* 00449494 */
    a=f->r[0];b=0x00000018u;t=mi_alu(f,a,b,32,1);
    /* 00449497 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004494b5;
    /* 00449499 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044949b */
    f->r[1]=(f->r[1]&0xffffff00u)|(((mi_read((0x0044950cu+f->r[0]),1u))&0xffu)<<0);
    /* 004494a1 */
    switch(mi_read((0x00449504u+f->r[1]*4u),4u)){case 0x00449464u:goto mi_00449464;case 0x004494b5u:goto mi_004494b5;default:abort();}
mi_004494a8:;
    /* 004494a8 */
    f->r[0]=mi_read((0x00000004u+f->r[4]),4u);
    /* 004494ac */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004494ae */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_004494b5;
    /* 004494b0 */
    a=f->r[0];b=0x00000008u;t=mi_alu(f,a,b,32,1);
    /* 004494b3 */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_00449464;
mi_004494b5:;
    /* 004494b5 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004494b7 */
    f->r[4]+=8;return;
}

void restore_event_detail_C_00449530(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449530 */
    f->r[0]=MI_U32((0x00000004u+f->r[4]));
    /* 00449534 */
    a=f->r[0];b=0x00000025u;t=mi_alu(f,a,b,32,1);
    /* 00449537 */
    if((f->flags&MI_ZF))goto mi_00449543;
    /* 00449539 */
    a=f->r[0];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 0044953c */
    if((f->flags&MI_ZF))goto mi_00449543;
    /* 0044953e */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00449540 */
    f->r[4]+=8;return;
mi_00449543:;
    /* 00449543 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449545 */
    f->r[4]+=8;return;
}

void restore_event_detail_C_00449550(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449550 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449551 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449552 */
    f->r[6]=f->r[1];
    /* 00449554 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449559u;f->boundary(0x00449554u,0x004493d0u,f);
    /* 00449559 */
    f->r[7]=MI_U32((0x0000000cu+f->r[4]));
    /* 0044955d */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044955f */
    if(mi_signed_less(f->flags))goto mi_004495b9;
    /* 00449561 */
    f->r[1]=f->r[6];
    /* 00449563 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449568u;f->boundary(0x00449563u,0x004493e0u,f);
    /* 00449568 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044956a */
    if(!mi_signed_less(f->flags))goto mi_004495b9;
    /* 0044956c */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[6])))&0xffu)<<0);
    /* 0044956f */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00449571 */
    if(!(f->flags&MI_ZF))goto mi_00449580;
    /* 00449573 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449574 */
    f->r[1]=f->r[6];
    /* 00449576 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044957bu;f->boundary(0x00449576u,0x00449440u,f);
    /* 0044957b */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044957c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044957d */
    f->r[4]+=8;return;
mi_00449580:;
    /* 00449580 */
    a=(f->r[0]&0xffu);b=0x00000017u;t=mi_alu(f,a,b,8,1);
    /* 00449582 */
    if(!(f->flags&MI_ZF))goto mi_00449591;
    /* 00449584 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449585 */
    f->r[1]=f->r[6];
    /* 00449587 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044958cu;f->boundary(0x00449587u,0x00449530u,f);
    /* 0044958c */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044958d */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044958e */
    f->r[4]+=8;return;
mi_00449591:;
    /* 00449591 */
    a=(f->r[0]&0xffu);b=0x0000000du;t=mi_alu(f,a,b,8,1);
    /* 00449593 */
    if((f->flags&MI_ZF))goto mi_004495a8;
    /* 00449595 */
    a=(f->r[0]&0xffu);b=0x0000000eu;t=mi_alu(f,a,b,8,1);
    /* 00449597 */
    if((f->flags&MI_ZF))goto mi_004495a8;
    /* 00449599 */
    a=(f->r[0]&0xffu);b=0x0000000fu;t=mi_alu(f,a,b,8,1);
    /* 0044959b */
    if((f->flags&MI_ZF))goto mi_004495a8;
    /* 0044959d */
    a=(f->r[0]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,1);
    /* 0044959f */
    if((f->flags&MI_ZF))goto mi_004495a8;
    /* 004495a1 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004495a2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004495a4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004495a5 */
    f->r[4]+=8;return;
mi_004495a8:;
    /* 004495a8 */
    a=f->r[7];b=0x0000001bu;t=mi_alu(f,a,b,32,1);
    /* 004495ab */
    if((f->flags&MI_ZF))goto mi_004495b9;
    /* 004495ad */
    a=f->r[7];b=0x00000015u;t=mi_alu(f,a,b,32,1);
    /* 004495b0 */
    if((f->flags&MI_ZF))goto mi_004495b9;
    /* 004495b2 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004495b3 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004495b5 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004495b6 */
    f->r[4]+=8;return;
mi_004495b9:;
    /* 004495b9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 004495ba */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004495bc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 004495bd */
    f->r[4]+=8;return;
}

void restore_event_detail_C_004495c0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004495c0 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 004495c2 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 004495c5 */
    f->r[0]=MI_U32((0x005d9d88u+f->r[0]*4u));
    /* 004495cc */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004495d0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004495d0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 004495d3 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 004495d5 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_004495de;
    /* 004495d7 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 004495d9 */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_004495de;
    /* 004495db */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004495dd */
    f->r[4]+=4;return;
mi_004495de:;
    /* 004495de */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004495e0 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004495f0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004495f0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004495f2 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449600(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449600 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449603 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00449605 */
    a=(f->r[2]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00449607 */
    if(!(f->flags&MI_ZF))goto mi_00449615;
    /* 00449609 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000060u+f->r[1])))&0xffu)<<0);
    /* 0044960c */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044960e */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00449615;
    /* 00449610 */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00449613 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00449617;
mi_00449615:;
    /* 00449615 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
mi_00449617:;
    /* 00449617 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449630(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449630 */
    a=MI_U8((0x00000002u+f->r[1]));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00449634 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449637 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449640(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449640 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449643 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 00449645 */
    if((f->flags&MI_ZF))goto mi_00449652;
    /* 00449647 */
    a=(f->r[0]&0xffu);b=0x00000012u;t=mi_alu(f,a,b,8,1);
    /* 00449649 */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_0044964f;
    /* 0044964b */
    a=(f->r[0]&0xffu);b=0x00000014u;t=mi_alu(f,a,b,8,1);
    /* 0044964d */
    if((f->flags&(MI_CF|MI_ZF)))goto mi_00449652;
mi_0044964f:;
    /* 0044964f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449651 */
    f->r[4]+=4;return;
mi_00449652:;
    /* 00449652 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00449654 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449660(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449660 */
    a=MI_U8((0x00000002u+f->r[1]));b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00449664 */
    if(!(f->flags&MI_ZF))goto mi_00449685;
    /* 00449666 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0000001eu+f->r[1])))&0xffffu)<<0);
    /* 0044966a */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044966d */
    if(!mi_signed_less(f->flags))goto mi_00449675;
    /* 0044966f */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000022u+f->r[1])))&0xffffu)<<0);
    /* 00449673 */
    goto mi_0044967c;
mi_00449675:;
    /* 00449675 */
    a=MI_U16((0x00000020u+f->r[1]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044967a */
    if(!mi_signed_less(f->flags))goto mi_00449685;
mi_0044967c:;
    /* 0044967c */
    a=(f->r[0]&0xffffu);b=0x00000009u;t=mi_alu(f,a,b,16,1);
    /* 00449680 */
    if(mi_signed_less(f->flags))goto mi_00449685;
    /* 00449682 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00449684 */
    f->r[4]+=4;return;
mi_00449685:;
    /* 00449685 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 00449687 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449690(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449690 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449693 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 00449695 */
    a=(f->r[2]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00449697 */
    if(!(f->flags&MI_ZF))goto mi_0044969f;
    /* 00449699 */
    a=MI_U8((0x00000060u+f->r[1]));b=0x00000003u;t=mi_alu(f,a,b,8,1);
    /* 0044969d */
    if((f->flags&MI_ZF))goto mi_004496a1;
mi_0044969f:;
    /* 0044969f */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
mi_004496a1:;
    /* 004496a1 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004496b0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004496b0 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 004496b3 */
    a=(f->r[0]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,1);
    /* 004496b5 */
    if((f->flags&MI_ZF))goto mi_004496be;
    /* 004496b7 */
    a=(f->r[0]&0xffu);b=0x00000017u;t=mi_alu(f,a,b,8,1);
    /* 004496b9 */
    if((f->flags&MI_ZF))goto mi_004496be;
    /* 004496bb */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,5);f->r[0]=(f->r[0]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 004496bd */
    f->r[4]+=4;return;
mi_004496be:;
    /* 004496be */
    f->r[0]=(f->r[0]&0xffffff00u)|(((0x00000001u)&0xffu)<<0);
    /* 004496c0 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449710(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449710 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000004u+f->r[4])))&0xffu)<<0);
    /* 00449714 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449717 */
    a=(f->r[2]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00449719 */
    if((f->flags&MI_ZF))goto mi_00449742;
    /* 0044971b */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044971e */
    MI_U8((0x00000002u+f->r[1]))=(uint8_t)((f->r[0]&0xffu));
    /* 00449721 */
    MI_U16((0x00000006u+f->r[1]))=(uint16_t)(0x00000001u);
    /* 00449727 */
    MI_U16((0x00000008u+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044972b */
    MI_U16((0x0000000au+f->r[1]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044972f */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00000028u+f->r[1])))&0xffu)<<0);
    /* 00449732 */
    a=(f->r[2]&0xffu);b=0x00000010u;t=mi_alu(f,a,b,8,3);
    /* 00449735 */
    if((f->flags&MI_ZF))goto mi_00449742;
    /* 00449737 */
    a=(f->r[0]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 00449739 */
    if((f->flags&MI_ZF))goto mi_00449742;
    /* 0044973b */
    MI_U32((0x00000028u+f->r[1]))=(uint32_t)(0x00000000u);
mi_00449742:;
    /* 00449742 */
    f->r[4]+=8;return;
}

void restore_event_detail_C_00449870(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449870 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 00449875 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449877 */
    if(mi_signed_less(f->flags))goto mi_004498af;
    /* 00449879 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044987f */
    if(!mi_signed_less(f->flags))goto mi_004498af;
    /* 00449881 */
    f->r[1]=f->r[0];
    /* 00449883 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00449886 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449888 */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044988f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 00449891 */
    if((f->flags&MI_ZF))goto mi_004498af;
    /* 00449893 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0078a636u)))&0xffffu)<<0);
    /* 00449899 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044989c */
    if(mi_signed_less(f->flags))goto mi_004498af;
    /* 0044989e */
    a=(f->r[0]&0xffffu);b=0x0000001eu;t=mi_alu(f,a,b,16,1);
    /* 004498a2 */
    if(!mi_signed_less(f->flags))goto mi_004498af;
    /* 004498a4 */
    a=MI_U8((0x00000002u+f->r[1]));b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 004498a7 */
    if((f->flags&MI_ZF))goto mi_004498af;
    /* 004498a9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004498aa */
    f->r[4]-=4;MI_U32(f->r[4])=0x004498afu;f->boundary(0x004498aau,0x00449710u,f);
mi_004498af:;
    /* 004498af */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004498b0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004498b0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 004498b5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004498b7 */
    if(mi_signed_less(f->flags))goto mi_004498e1;
    /* 004498b9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 004498bf */
    if(!mi_signed_less(f->flags))goto mi_004498e1;
    /* 004498c1 */
    f->r[1]=f->r[0];
    /* 004498c3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004498c6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004498c8 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 004498cf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004498d1 */
    if((f->flags&MI_ZF))goto mi_004498e1;
    /* 004498d3 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x0078a63au)))&0xffu)<<0);
    /* 004498d9 */
    a=(f->r[1]&0xffu);b=0x00000005u;t=mi_alu(f,a,b,8,1);
    /* 004498dc */
    if(!(f->flags&MI_CF))goto mi_004498e1;
    /* 004498de */
    MI_U8((0x00000036u+f->r[0]))=(uint8_t)((f->r[1]&0xffu));
mi_004498e1:;
    /* 004498e1 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004498f0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004498f0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 004498f5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004498f7 */
    if(mi_signed_less(f->flags))goto mi_004499b0;
    /* 004498fd */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00449903 */
    if(!mi_signed_less(f->flags))goto mi_004499b0;
    /* 00449909 */
    f->r[1]=f->r[0];
    /* 0044990b */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044990e */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449910 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 00449917 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449919 */
    if((f->flags&MI_ZF))goto mi_004499b0;
    /* 0044991f */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U8((0x00000002u+f->r[0])))&0xffffu)<<0);
    /* 00449924 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449925 */
    MI_U16((0x0078a636u))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044992c */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x00ea137cu)))&0xffu)<<0);
    /* 00449932 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 00449934 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00449936 */
    f->r[6]=0x006057f4u;
mi_0044993b:;
    /* 0044993b */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044993d */
    if((f->flags&MI_ZF))goto mi_0044995d;
    /* 0044993f */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00449942 */
    if((f->flags&MI_ZF))goto mi_0044995d;
    /* 00449944 */
    a=f->r[0];b=0x00000004u;t=mi_alu(f,a,b,32,1);
    /* 00449947 */
    if((f->flags&MI_ZF))goto mi_0044995d;
    /* 00449949 */
    a=f->r[0];b=0x00000005u;t=mi_alu(f,a,b,32,1);
    /* 0044994c */
    if((f->flags&MI_ZF))goto mi_0044995d;
    /* 0044994e */
    a=f->r[0];b=0x0000001du;t=mi_alu(f,a,b,32,1);
    /* 00449951 */
    if(!(f->flags&MI_ZF))goto mi_00449957;
    /* 00449953 */
    a=(f->r[2]&0xffu);b=(f->r[2]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449955 */
    if((f->flags&MI_ZF))goto mi_0044995d;
mi_00449957:;
    /* 00449957 */
    MI_U32((0x00000000u+f->r[6]))=(uint32_t)(f->r[0]);
    /* 00449959 */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044995a */
    a=f->r[6];b=0x00000004u;t=mi_alu(f,a,b,32,0);f->r[6]=t;
mi_0044995d:;
    /* 0044995d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 0044995e */
    a=f->r[0];b=0x0000001eu;t=mi_alu(f,a,b,32,1);
    /* 00449961 */
    if(mi_signed_less(f->flags))goto mi_0044993b;
    /* 00449963 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449965 */
    a=0x0000000fu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449967 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449969 */
    f->r[0]=MI_U32((0x00e38e68u));
    /* 0044996e */
    a=0x00449870u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449973 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449975 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449977 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449979 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044997b */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449980 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449982 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449984 */
    a=0x0000009cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449989 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044998a */
    f->r[1]=(0x00000000u+f->r[0]+f->r[0]*2u);
    /* 0044998d */
    f->r[0]=(0x00000003u+f->r[0]+f->r[0]*2u);
    /* 00449991 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 00449994 */
    f->r[0]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 00449997 */
    f->r[1]=(0xffffffceu+f->r[1]+f->r[1]*4u);
    /* 0044999b */
    f->r[2]=(0x00000000u+f->r[0]+f->r[0]*4u);
    /* 0044999e */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044999f */
    a=f->r[2];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004499a0 */
    f->r[4]-=4;MI_U32(f->r[4])=0x004499a5u;f->boundary(0x004499a0u,0x00403c00u,f);
    /* 004499a5 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 004499a8 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 004499af */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
mi_004499b0:;
    /* 004499b0 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_004499c0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 004499c0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 004499c5 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 004499c8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 004499ca */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 004499cb */
    if(mi_signed_less(f->flags))goto mi_00449afa;
    /* 004499d1 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 004499d7 */
    if(!mi_signed_less(f->flags))goto mi_00449afa;
    /* 004499dd */
    f->r[1]=f->r[0];
    /* 004499df */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 004499e2 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 004499e4 */
    f->r[6]=(0x00785d74u+f->r[1]*4u);
    /* 004499eb */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 004499ed */
    if((f->flags&MI_ZF))goto mi_00449afa;
    /* 004499f3 */
    a=MI_U16((0x0078a636u));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 004499fb */
    if(mi_signed_less(f->flags))goto mi_00449afa;
    /* 00449a01 */
    f->r[1]=f->r[6];
    /* 00449a03 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449a08u;f->boundary(0x00449a03u,0x00449b00u,f);
    /* 00449a08 */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0078a636u)))&0xffffu)<<0);
    /* 00449a0f */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[1]&0xffffu);
    /* 00449a12 */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00449a14 */
    if(!mi_signed_less(f->flags))goto mi_00449afa;
    /* 00449a1a */
    a=MI_U8((0x00000060u+f->r[6]));b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,1);
    /* 00449a1d */
    if((f->flags&MI_ZF))goto mi_00449afa;
    /* 00449a23 */
    MI_U8((0x00000060u+f->r[6]))=(uint8_t)((f->r[1]&0xffu));
    /* 00449a26 */
    f->r[1]=f->r[6];
    /* 00449a28 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449a2du;f->boundary(0x00449a28u,0x004493b0u,f);
    /* 00449a2d */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449a2f */
    if((f->flags&MI_ZF))goto mi_00449afa;
    /* 00449a35 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449a36 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449a37 */
    f->r[1]=f->r[6];
    /* 00449a39 */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,5);f->r[3]=t;
    /* 00449a3b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449a40u;f->boundary(0x00449a3bu,0x004493d0u,f);
    /* 00449a40 */
    f->r[1]=f->r[6];
    /* 00449a42 */
    f->r[7]=f->r[0];
    /* 00449a44 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449a49u;f->boundary(0x00449a44u,0x004493e0u,f);
    /* 00449a49 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00449a4b */
    if(!mi_signed_less(f->flags))goto mi_00449a69;
mi_00449a4d:;
    /* 00449a4d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449a4e */
    f->r[1]=f->r[6];
    /* 00449a50 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449a55u;f->boundary(0x00449a50u,0x00449550u,f);
    /* 00449a55 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449a57 */
    if(!(f->flags&MI_ZF))goto mi_00449a67;
    /* 00449a59 */
    f->r[1]=f->r[6];
    /* 00449a5b */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00449a5c */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449a61u;f->boundary(0x00449a5cu,0x004493e0u,f);
    /* 00449a61 */
    a=f->r[7];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 00449a63 */
    if(mi_signed_less(f->flags))goto mi_00449a4d;
    /* 00449a65 */
    goto mi_00449a69;
mi_00449a67:;
    /* 00449a67 */
    f->r[3]=f->r[7];
mi_00449a69:;
    /* 00449a69 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00000006u+f->r[6])))&0xffffu)<<0);
    /* 00449a6d */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[6])))&0xffffu)<<0);
    /* 00449a71 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x0000000au+f->r[6])))&0xffffu)<<0);
    /* 00449a75 */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[0]&0xffffu));
    /* 00449a7a */
    MI_U16((0x0000000eu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00449a7f */
    MI_U16((0x00000010u+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00449a84 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,5);f->r[7]=t;
mi_00449a86:;
    /* 00449a86 */
    f->r[0]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0000000cu+f->r[4]+f->r[7]*2u));
    /* 00449a8b */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449a8c */
    f->r[1]=f->r[6];
    /* 00449a8e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449a93u;f->boundary(0x00449a8eu,0x00449550u,f);
    /* 00449a93 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449a95 */
    if(!(f->flags&MI_ZF))goto mi_00449aa7;
    /* 00449a97 */
    a=f->r[7];b=f->r[7];t=mi_alu(f,a,b,32,3);
    /* 00449a99 */
    if((f->flags&MI_ZF))goto mi_00449aa0;
    /* 00449a9b */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 00449a9e */
    goto mi_00449aa2;
mi_00449aa0:;
    /* 00449aa0 */
    f->r[0]=f->r[3];
mi_00449aa2:;
    /* 00449aa2 */
    MI_U16((0x0000000cu+f->r[4]+f->r[7]*2u))=(uint16_t)((f->r[0]&0xffffu));
mi_00449aa7:;
    /* 00449aa7 */
    a=f->r[7];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,0);f->flags=(f->flags&~MI_CF)|b;f->r[7]=t;
    /* 00449aa8 */
    a=f->r[7];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 00449aab */
    if(mi_signed_less(f->flags))goto mi_00449a86;
    /* 00449aad */
    f->r[1]=MI_U32((0x0000000cu+f->r[4]));
    /* 00449ab1 */
    f->r[0]=MI_U32((0x00000010u+f->r[4]));
    /* 00449ab5 */
    f->r[2]=MI_U32((0x0000000eu+f->r[4]));
    /* 00449ab9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 00449aba */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00449abd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 00449abe */
    if(!mi_signed_less(f->flags))goto mi_00449adc;
    /* 00449ac0 */
    a=MI_U16((0x00000006u+f->r[4]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00449ac6 */
    if(mi_signed_less(f->flags))goto mi_00449acf;
    /* 00449ac8 */
    f->r[1]=f->r[2];
    /* 00449aca */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 00449acd */
    goto mi_00449ae4;
mi_00449acf:;
    /* 00449acf */
    a=MI_U16((0x00000008u+f->r[4]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00449ad5 */
    if(mi_signed_less(f->flags))goto mi_00449adc;
    /* 00449ad7 */
    f->r[1]=f->r[0];
    /* 00449ad9 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_00449adc:;
    /* 00449adc */
    a=MI_U16((0x00000006u+f->r[4]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 00449ae2 */
    if(!mi_signed_less(f->flags))goto mi_00449aee;
mi_00449ae4:;
    /* 00449ae4 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 00449ae7 */
    if(mi_signed_less(f->flags))goto mi_00449aee;
    /* 00449ae9 */
    f->r[2]=f->r[0];
    /* 00449aeb */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_00449aee:;
    /* 00449aee */
    MI_U16((0x00000006u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 00449af2 */
    MI_U16((0x00000008u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 00449af6 */
    MI_U16((0x0000000au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
mi_00449afa:;
    /* 00449afa */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00449afb */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00449afe */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449b00(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449b00 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00449b02 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449b05 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00449b06 */
    if((f->flags&MI_ZF))goto mi_00449b1f;
    /* 00449b08 */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00449b0b */
    if((f->flags&MI_ZF))goto mi_00449b19;
    /* 00449b0d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00449b0e */
    if((f->flags&MI_ZF))goto mi_00449b13;
    /* 00449b10 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00449b12 */
    f->r[4]+=4;return;
mi_00449b13:;
    /* 00449b13 */
    f->r[0]=0x00000004u;
    /* 00449b18 */
    f->r[4]+=4;return;
mi_00449b19:;
    /* 00449b19 */
    f->r[0]=0x00000005u;
    /* 00449b1e */
    f->r[4]+=4;return;
mi_00449b1f:;
    /* 00449b1f */
    f->r[0]=0x00000007u;
    /* 00449b24 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449b30(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449b30 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,5);f->r[0]=t;
    /* 00449b32 */
    f->r[0]=(f->r[0]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[1])))&0xffu)<<0);
    /* 00449b35 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00449b36 */
    if((f->flags&MI_ZF))goto mi_00449b50;
    /* 00449b38 */
    a=f->r[0];b=0x00000012u;t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 00449b3b */
    if((f->flags&MI_ZF))goto mi_00449b4a;
    /* 00449b3d */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[0]=t;
    /* 00449b3e */
    if((f->flags&MI_ZF))goto mi_00449b44;
    /* 00449b40 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
    /* 00449b43 */
    f->r[4]+=4;return;
mi_00449b44:;
    /* 00449b44 */
    f->r[0]=0x00000096u;
    /* 00449b49 */
    f->r[4]+=4;return;
mi_00449b4a:;
    /* 00449b4a */
    f->r[0]=0x00000023u;
    /* 00449b4f */
    f->r[4]+=4;return;
mi_00449b50:;
    /* 00449b50 */
    f->r[0]=0x00000095u;
    /* 00449b55 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449b60(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449b60 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 00449b65 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449b66 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449b68 */
    if(mi_signed_less(f->flags))goto mi_00449be4;
    /* 00449b6a */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00449b70 */
    if(!mi_signed_less(f->flags))goto mi_00449be4;
    /* 00449b72 */
    f->r[1]=f->r[0];
    /* 00449b74 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00449b77 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449b79 */
    f->r[6]=(0x00785d74u+f->r[1]*4u);
    /* 00449b80 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 00449b82 */
    if((f->flags&MI_ZF))goto mi_00449be4;
    /* 00449b84 */
    f->r[1]=f->r[6];
    /* 00449b86 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449b8bu;f->boundary(0x00449b86u,0x00449640u,f);
    /* 00449b8b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 00449b8d */
    if((f->flags&MI_ZF))goto mi_00449be4;
    /* 00449b8f */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U8((0x00000060u+f->r[6])))&0xffffu)<<0);
    /* 00449b94 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449b95 */
    f->r[1]=f->r[6];
    /* 00449b97 */
    MI_U16((0x0078a636u))=(uint16_t)((f->r[2]&0xffffu));
    /* 00449b9e */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449ba3u;f->boundary(0x00449b9eu,0x00449b30u,f);
    /* 00449ba3 */
    f->r[1]=f->r[6];
    /* 00449ba5 */
    f->r[7]=f->r[0];
    /* 00449ba7 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449bacu;f->boundary(0x00449ba7u,0x00449b00u,f);
    /* 00449bac */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bae */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bb0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bb2 */
    a=0x004499c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bb7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bb9 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bbb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bbd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bbf */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bc4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bc6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bc8 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bc9 */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bca */
    a=0x0000008au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bcf */
    a=0x000001a1u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449bd4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449bd9u;f->boundary(0x00449bd4u,0x00403c00u,f);
    /* 00449bd9 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00449bdc */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
    /* 00449be3 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_00449be4:;
    /* 00449be4 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 00449be5 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449bf0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449bf0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 00449bf5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449bf7 */
    if(mi_signed_less(f->flags))goto mi_00449c1c;
    /* 00449bf9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00449bff */
    if(!mi_signed_less(f->flags))goto mi_00449c1c;
    /* 00449c01 */
    f->r[1]=f->r[0];
    /* 00449c03 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00449c06 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449c08 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 00449c0f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449c11 */
    if((f->flags&MI_ZF))goto mi_00449c1c;
    /* 00449c13 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0078a636u)))&0xffu)<<0);
    /* 00449c19 */
    MI_U8((0x00000003u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_00449c1c:;
    /* 00449c1c */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449c20(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449c20 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 00449c25 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449c27 */
    if(mi_signed_less(f->flags))goto mi_00449c84;
    /* 00449c29 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00449c2f */
    if(!mi_signed_less(f->flags))goto mi_00449c84;
    /* 00449c31 */
    f->r[1]=f->r[0];
    /* 00449c33 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00449c36 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449c38 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 00449c3f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449c41 */
    if((f->flags&MI_ZF))goto mi_00449c84;
    /* 00449c43 */
    f->r[2]=MI_U32((0x00000028u+f->r[0]));
    /* 00449c46 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 00449c49 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00449c4c */
    if(!(f->flags&MI_ZF))goto mi_00449c84;
    /* 00449c4e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c50 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c52 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c54 */
    a=0x00449bf0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c59 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c5b */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c5d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c5f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c61 */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c66 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c68 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c6a */
    a=0x00000019u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c6c */
    a=0x0000000cu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c6e */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c70 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449c75 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449c7au;f->boundary(0x00449c75u,0x00403c00u,f);
    /* 00449c7a */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00449c7d */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_00449c84:;
    /* 00449c84 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449de0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449de0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 00449de5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449de7 */
    if(mi_signed_less(f->flags))goto mi_00449e3a;
    /* 00449de9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00449def */
    if(!mi_signed_less(f->flags))goto mi_00449e3a;
    /* 00449df1 */
    f->r[1]=f->r[0];
    /* 00449df3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00449df6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449df8 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 00449dff */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449e01 */
    if((f->flags&MI_ZF))goto mi_00449e3a;
    /* 00449e03 */
    f->r[2]=MI_U32((0x00000028u+f->r[0]));
    /* 00449e06 */
    a=f->r[2];t=a>>4u;f->flags=(f->flags&~(MI_CF|MI_PF|MI_ZF|MI_SF))|((a>>3u)&1u)|mi_szp(t,32);f->r[2]=t;
    /* 00449e09 */
    a=(f->r[2]&0xffu);b=0x00000001u;t=mi_alu(f,a,b,8,3);
    /* 00449e0c */
    if(!(f->flags&MI_ZF))goto mi_00449e3a;
    /* 00449e0e */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x00d3b198u)))&0xffffu)<<0);
    /* 00449e14 */
    a=0x00449c90u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449e19 */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449e1e */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449e20 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449e25 */
    MI_U16((0x0078a638u))=(uint16_t)((f->r[0]&0xffffu));
    /* 00449e2b */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449e30u;f->boundary(0x00449e2bu,0x00403870u,f);
    /* 00449e30 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00449e33 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_00449e3a:;
    /* 00449e3a */
    f->r[4]+=4;return;
}

void restore_event_detail_C_00449f90(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 00449f90 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 00449f95 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449f97 */
    if(mi_signed_less(f->flags))goto mi_00449fe1;
    /* 00449f99 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 00449f9f */
    if(!mi_signed_less(f->flags))goto mi_00449fe1;
    /* 00449fa1 */
    f->r[1]=f->r[0];
    /* 00449fa3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 00449fa6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 00449fa8 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 00449faf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 00449fb1 */
    if((f->flags&MI_ZF))goto mi_00449fe1;
    /* 00449fb3 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b198u)))&0xffffu)<<0);
    /* 00449fba */
    a=0x00449e40u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449fbf */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449fc4 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449fc6 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 00449fcb */
    MI_U16((0x0078a638u))=(uint16_t)((f->r[2]&0xffffu));
    /* 00449fd2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x00449fd7u;f->boundary(0x00449fd2u,0x00403870u,f);
    /* 00449fd7 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 00449fda */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_00449fe1:;
    /* 00449fe1 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a140(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a140 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a145 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a147 */
    if(mi_signed_less(f->flags))goto mi_0044a191;
    /* 0044a149 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a14f */
    if(!mi_signed_less(f->flags))goto mi_0044a191;
    /* 0044a151 */
    f->r[1]=f->r[0];
    /* 0044a153 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a156 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a158 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a15f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a161 */
    if((f->flags&MI_ZF))goto mi_0044a191;
    /* 0044a163 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b198u)))&0xffffu)<<0);
    /* 0044a16a */
    a=0x00449ff0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a16f */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a174 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a176 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a17b */
    MI_U16((0x0078a638u))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044a182 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a187u;f->boundary(0x0044a182u,0x00403870u,f);
    /* 0044a187 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044a18a */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0044a191:;
    /* 0044a191 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a2f0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a2f0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a2f5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a2f7 */
    if(mi_signed_less(f->flags))goto mi_0044a341;
    /* 0044a2f9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a2ff */
    if(!mi_signed_less(f->flags))goto mi_0044a341;
    /* 0044a301 */
    f->r[1]=f->r[0];
    /* 0044a303 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a306 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a308 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a30f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a311 */
    if((f->flags&MI_ZF))goto mi_0044a341;
    /* 0044a313 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00d3b198u)))&0xffffu)<<0);
    /* 0044a31a */
    a=0x0044a1a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a31f */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a324 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a326 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a32b */
    MI_U16((0x0078a638u))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044a332 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a337u;f->boundary(0x0044a332u,0x00403870u,f);
    /* 0044a337 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044a33a */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0044a341:;
    /* 0044a341 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a3d0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a3d0 */
    f->r[0]=mi_read((0x0078a61cu),4u);
    /* 0044a3d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a3d7 */
    if(mi_signed_less(f->flags))goto mi_0044a45d;
    /* 0044a3dd */
    a=f->r[0];b=mi_read((0x00785d70u),4u);t=mi_alu(f,a,b,32,1);
    /* 0044a3e3 */
    if(!mi_signed_less(f->flags))goto mi_0044a45d;
    /* 0044a3e5 */
    f->r[1]=f->r[0];
    /* 0044a3e7 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a3ea */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a3ec */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044a3f3 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0044a3f5 */
    if((f->flags&MI_ZF))goto mi_0044a45d;
    /* 0044a3f7 */
    f->r[0]=mi_read((0x00d3b198u),4u);
    /* 0044a3fc */
    a=f->r[0];b=0x00000003u;t=mi_alu(f,a,b,32,1);
    /* 0044a3ff */
    if(!(f->flags&(MI_CF|MI_ZF)))goto mi_0044a45d;
    /* 0044a401 */
    switch(mi_read((0x0044a460u+f->r[0]*4u),4u)){case 0x0044a408u:goto mi_0044a408;case 0x0044a40fu:goto mi_0044a40f;case 0x0044a416u:goto mi_0044a416;case 0x0044a41du:goto mi_0044a41d;default:abort();}
mi_0044a408:;
    /* 0044a408 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a40du;f->boundary(0x0044a408u,0x004495f0u,f);
    /* 0044a40d */
    goto mi_0044a422;
mi_0044a40f:;
    /* 0044a40f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a414u;f->boundary(0x0044a40fu,0x004496b0u,f);
    /* 0044a414 */
    goto mi_0044a422;
mi_0044a416:;
    /* 0044a416 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a41bu;f->boundary(0x0044a416u,0x004496b0u,f);
    /* 0044a41b */
    goto mi_0044a422;
mi_0044a41d:;
    /* 0044a41d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a422u;f->boundary(0x0044a41du,0x00449600u,f);
mi_0044a422:;
    /* 0044a422 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a424 */
    if((f->flags&MI_ZF))goto mi_0044a45d;
    /* 0044a426 */
    f->r[2]=(f->r[2]&0xffff0000u)|(((mi_read((0x00d3b198u),2u))&0xffffu)<<0);
    /* 0044a42d */
    a=0x0044a350u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a432 */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a437 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a439 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a43e */
    MI_U16((0x0078a638u))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044a445 */
    MI_U16((0x0078a636u))=(uint16_t)(0x000003e7u);
    /* 0044a44e */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a453u;f->boundary(0x0044a44eu,0x00403870u,f);
    /* 0044a453 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044a456 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0044a45d:;
    /* 0044a45d */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a470(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a470 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a475 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a477 */
    if(mi_signed_less(f->flags))goto mi_0044a4e8;
    /* 0044a479 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a47f */
    if(!mi_signed_less(f->flags))goto mi_0044a4e8;
    /* 0044a481 */
    f->r[1]=f->r[0];
    /* 0044a483 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a486 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a488 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a48f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a491 */
    if((f->flags&MI_ZF))goto mi_0044a4e8;
    /* 0044a493 */
    f->r[1]=MI_U32((0x00000028u+f->r[0]));
    /* 0044a496 */
    f->r[2]=0x00000001u;
    /* 0044a49b */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0044a49d */
    if(!(f->flags&MI_ZF))goto mi_0044a4ad;
    /* 0044a49f */
    MI_U32((0x00000028u+f->r[0]))=(uint32_t)(0x00000002u);
    /* 0044a4a6 */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[2]&0xffu));
    /* 0044a4ac */
    f->r[4]+=4;return;
mi_0044a4ad:;
    /* 0044a4ad */
    a=(f->r[1]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,3);
    /* 0044a4b0 */
    if((f->flags&MI_ZF))goto mi_0044a4bc;
    /* 0044a4b2 */
    MI_U32((0x00000028u+f->r[0]))=(uint32_t)(f->r[2]);
    /* 0044a4b5 */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[2]&0xffu));
    /* 0044a4bb */
    f->r[4]+=4;return;
mi_0044a4bc:;
    /* 0044a4bc */
    a=(f->r[2]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a4be */
    if((f->flags&MI_ZF))goto mi_0044a4db;
    /* 0044a4c0 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a4c1 */
    f->r[3]=(f->r[3]&0xffffff00u)|(((MI_U8((0x00000002u+f->r[0])))&0xffu)<<0);
    /* 0044a4c4 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
    /* 0044a4c6 */
    a=(f->r[3]&0xffu);b=0x00000002u;t=mi_alu(f,a,b,8,1);
    /* 0044a4c9 */
    t=(!(f->flags&MI_ZF))?1u:0u;f->r[1]=(f->r[1]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044a4cc */
    a=f->r[1];b=f->flags&MI_CF;t=mi_alu(f,a,1,32,1);f->flags=(f->flags&~MI_CF)|b;f->r[1]=t;
    /* 0044a4cd */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044a4ce */
    a=f->r[1];b=0x00000010u;t=mi_alu(f,a,b,32,3);f->r[1]=t;
    /* 0044a4d1 */
    MI_U32((0x00000028u+f->r[0]))=(uint32_t)(f->r[1]);
    /* 0044a4d4 */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[2]&0xffu));
    /* 0044a4da */
    f->r[4]+=4;return;
mi_0044a4db:;
    /* 0044a4db */
    MI_U32((0x00000028u+f->r[0]))=(uint32_t)(0x00000000u);
    /* 0044a4e2 */
    MI_U8((0x00e38dcau))=(uint8_t)((f->r[2]&0xffu));
mi_0044a4e8:;
    /* 0044a4e8 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a4f0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a4f0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a4f5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a4f7 */
    if(mi_signed_less(f->flags))goto mi_0044a525;
    /* 0044a4f9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a4ff */
    if(!mi_signed_less(f->flags))goto mi_0044a525;
    /* 0044a501 */
    f->r[1]=f->r[0];
    /* 0044a503 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a506 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a508 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a50f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a511 */
    if((f->flags&MI_ZF))goto mi_0044a525;
    /* 0044a513 */
    f->r[1]=(f->r[1]&0xffffff00u)|(((MI_U8((0x00000056u+f->r[0])))&0xffu)<<0);
    /* 0044a516 */
    a=(f->r[1]&0xffu);b=(f->r[1]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a518 */
    t=((f->flags&MI_ZF))?1u:0u;f->r[2]=(f->r[2]&0xffffff00u)|(((t)&0xffu)<<0);
    /* 0044a51b */
    MI_U8((0x00000056u+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
    /* 0044a51e */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0044a525:;
    /* 0044a525 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a560(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a560 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a565 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a567 */
    if(mi_signed_less(f->flags))goto mi_0044a5a5;
    /* 0044a569 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a56f */
    if(!mi_signed_less(f->flags))goto mi_0044a5a5;
    /* 0044a571 */
    f->r[1]=f->r[0];
    /* 0044a573 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a576 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a578 */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044a57f */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,3);
    /* 0044a581 */
    if((f->flags&MI_ZF))goto mi_0044a5a5;
    /* 0044a583 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a588u;f->boundary(0x0044a583u,0x004495d0u,f);
    /* 0044a588 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a58a */
    if((f->flags&MI_ZF))goto mi_0044a5a5;
    /* 0044a58c */
    a=0x0044a530u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a591 */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a596 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a598 */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a59d */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a5a2u;f->boundary(0x0044a59du,0x00403870u,f);
    /* 0044a5a2 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044a5a5:;
    /* 0044a5a5 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a5e0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a5e0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a5e5 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a5e6 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a5e8 */
    if(mi_signed_less(f->flags))goto mi_0044a667;
    /* 0044a5ea */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a5f0 */
    if(!mi_signed_less(f->flags))goto mi_0044a667;
    /* 0044a5f2 */
    f->r[1]=f->r[0];
    /* 0044a5f4 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a5f7 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a5f9 */
    f->r[6]=(0x00785d74u+f->r[1]*4u);
    /* 0044a600 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044a602 */
    if((f->flags&MI_ZF))goto mi_0044a667;
    /* 0044a604 */
    f->r[1]=f->r[6];
    /* 0044a606 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a60bu;f->boundary(0x0044a606u,0x00449660u,f);
    /* 0044a60b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a60d */
    if((f->flags&MI_ZF))goto mi_0044a62a;
    /* 0044a60f */
    a=0x0044a5b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a614 */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a619 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a61b */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a620 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a625u;f->boundary(0x0044a620u,0x00403870u,f);
    /* 0044a625 */
    a=f->r[4];b=0x00000010u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044a628 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044a629 */
    f->r[4]+=4;return;
mi_0044a62a:;
    /* 0044a62a */
    f->r[1]=f->r[6];
    /* 0044a62c */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a631u;f->boundary(0x0044a62cu,0x00449690u,f);
    /* 0044a631 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a633 */
    if((f->flags&MI_ZF))goto mi_0044a667;
    /* 0044a635 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a637 */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a639 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a63b */
    a=0x004498b0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a640 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a642 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a644 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a646 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a648 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a64a */
    a=0x0078a63au;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a64f */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a651 */
    a=0x0000009du;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a656 */
    a=0x00000005u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a658 */
    a=0x00000028u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a65a */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a65f */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a664u;f->boundary(0x0044a65fu,0x00403c00u,f);
    /* 0044a664 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044a667:;
    /* 0044a667 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044a668 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a670(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a670 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a675 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a676 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a678 */
    if(mi_signed_less(f->flags))goto mi_0044a6b7;
    /* 0044a67a */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a680 */
    if(!mi_signed_less(f->flags))goto mi_0044a6b7;
    /* 0044a682 */
    f->r[1]=f->r[0];
    /* 0044a684 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a687 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a689 */
    f->r[6]=(0x00785d74u+f->r[1]*4u);
    /* 0044a690 */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044a692 */
    if((f->flags&MI_ZF))goto mi_0044a6b7;
    /* 0044a694 */
    f->r[1]=f->r[6];
    /* 0044a696 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a69bu;f->boundary(0x0044a696u,0x00449630u,f);
    /* 0044a69b */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a69d */
    if((f->flags&MI_ZF))goto mi_0044a6b7;
    /* 0044a69f */
    a=MI_U16((0x00000038u+f->r[6]));b=f->flags&MI_CF;t=mi_alu(f,a,1,16,0);f->flags=(f->flags&~MI_CF)|b;MI_U16((0x00000038u+f->r[6]))=(uint16_t)(t);
    /* 0044a6a3 */
    a=MI_U16((0x00000038u+f->r[6]));b=0x00000005u;t=mi_alu(f,a,b,16,1);
    /* 0044a6a8 */
    if(mi_signed_less(f->flags))goto mi_0044a6b0;
    /* 0044a6aa */
    MI_U16((0x00000038u+f->r[6]))=(uint16_t)(0x00000000u);
mi_0044a6b0:;
    /* 0044a6b0 */
    MI_U8((0x00e38dcau))=(uint8_t)(0x00000001u);
mi_0044a6b7:;
    /* 0044a6b7 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044a6b8 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a6c0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a6c0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a6c5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a6c7 */
    if(mi_signed_less(f->flags))goto mi_0044a6ec;
    /* 0044a6c9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a6cf */
    if(!mi_signed_less(f->flags))goto mi_0044a6ec;
    /* 0044a6d1 */
    f->r[1]=f->r[0];
    /* 0044a6d3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a6d6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a6d8 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a6df */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a6e1 */
    if((f->flags&MI_ZF))goto mi_0044a6ec;
    /* 0044a6e3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0078a636u)))&0xffu)<<0);
    /* 0044a6e9 */
    MI_U8((0x0000006au+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_0044a6ec:;
    /* 0044a6ec */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a6f0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a6f0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a6f5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a6f7 */
    if(mi_signed_less(f->flags))goto mi_0044a711;
    /* 0044a6f9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a6ff */
    if(!mi_signed_less(f->flags))goto mi_0044a711;
    /* 0044a701 */
    f->r[1]=f->r[0];
    /* 0044a703 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a706 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a708 */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044a70f */
    goto mi_0044a713;
mi_0044a711:;
    /* 0044a711 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0044a713:;
    /* 0044a713 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a718u;f->boundary(0x0044a713u,0x004495f0u,f);
    /* 0044a718 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a71a */
    if((f->flags&MI_ZF))goto mi_0044a751;
    /* 0044a71c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a71e */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a720 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a722 */
    a=0x0044a6c0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a727 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a729 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a72b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a72d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a72f */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a734 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a736 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a738 */
    a=0x0000012bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a73d */
    a=0x00000007u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a73f */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a744 */
    a=0x00000096u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a749 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a74eu;f->boundary(0x0044a749u,0x00403c00u,f);
    /* 0044a74e */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044a751:;
    /* 0044a751 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a760(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a760 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a765 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a767 */
    if(mi_signed_less(f->flags))goto mi_0044a78c;
    /* 0044a769 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a76f */
    if(!mi_signed_less(f->flags))goto mi_0044a78c;
    /* 0044a771 */
    f->r[1]=f->r[0];
    /* 0044a773 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a776 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a778 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a77f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a781 */
    if((f->flags&MI_ZF))goto mi_0044a78c;
    /* 0044a783 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0078a636u)))&0xffu)<<0);
    /* 0044a789 */
    MI_U8((0x0000006bu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_0044a78c:;
    /* 0044a78c */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a790(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a790 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a795 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a797 */
    if(mi_signed_less(f->flags))goto mi_0044a7b1;
    /* 0044a799 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a79f */
    if(!mi_signed_less(f->flags))goto mi_0044a7b1;
    /* 0044a7a1 */
    f->r[1]=f->r[0];
    /* 0044a7a3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a7a6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a7a8 */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044a7af */
    goto mi_0044a7b3;
mi_0044a7b1:;
    /* 0044a7b1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0044a7b3:;
    /* 0044a7b3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a7b8u;f->boundary(0x0044a7b3u,0x004496b0u,f);
    /* 0044a7b8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a7ba */
    if((f->flags&MI_ZF))goto mi_0044a7f1;
    /* 0044a7bc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7be */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7c0 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7c2 */
    a=0x0044a760u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7c7 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7c9 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7cb */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7cd */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7cf */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7d4 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7d6 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7d8 */
    a=0x0000012bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7dd */
    a=0x00000007u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7df */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7e4 */
    a=0x00000096u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a7e9 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a7eeu;f->boundary(0x0044a7e9u,0x00403c00u,f);
    /* 0044a7ee */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044a7f1:;
    /* 0044a7f1 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a800(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a800 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a805 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a807 */
    if(mi_signed_less(f->flags))goto mi_0044a82c;
    /* 0044a809 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a80f */
    if(!mi_signed_less(f->flags))goto mi_0044a82c;
    /* 0044a811 */
    f->r[1]=f->r[0];
    /* 0044a813 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a816 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a818 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a81f */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a821 */
    if((f->flags&MI_ZF))goto mi_0044a82c;
    /* 0044a823 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0078a636u)))&0xffu)<<0);
    /* 0044a829 */
    MI_U8((0x0000006cu+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_0044a82c:;
    /* 0044a82c */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a830(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a830 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a835 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a837 */
    if(mi_signed_less(f->flags))goto mi_0044a851;
    /* 0044a839 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a83f */
    if(!mi_signed_less(f->flags))goto mi_0044a851;
    /* 0044a841 */
    f->r[1]=f->r[0];
    /* 0044a843 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a846 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a848 */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044a84f */
    goto mi_0044a853;
mi_0044a851:;
    /* 0044a851 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0044a853:;
    /* 0044a853 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a858u;f->boundary(0x0044a853u,0x004496b0u,f);
    /* 0044a858 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a85a */
    if((f->flags&MI_ZF))goto mi_0044a891;
    /* 0044a85c */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a85e */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a860 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a862 */
    a=0x0044a800u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a867 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a869 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a86b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a86d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a86f */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a874 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a876 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a878 */
    a=0x0000012bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a87d */
    a=0x00000007u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a87f */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a884 */
    a=0x00000096u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a889 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a88eu;f->boundary(0x0044a889u,0x00403c00u,f);
    /* 0044a88e */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044a891:;
    /* 0044a891 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a8a0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a8a0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a8a5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a8a7 */
    if(mi_signed_less(f->flags))goto mi_0044a8cc;
    /* 0044a8a9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a8af */
    if(!mi_signed_less(f->flags))goto mi_0044a8cc;
    /* 0044a8b1 */
    f->r[1]=f->r[0];
    /* 0044a8b3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a8b6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a8b8 */
    f->r[0]=(0x00785d74u+f->r[1]*4u);
    /* 0044a8bf */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a8c1 */
    if((f->flags&MI_ZF))goto mi_0044a8cc;
    /* 0044a8c3 */
    f->r[2]=(f->r[2]&0xffffff00u)|(((MI_U8((0x0078a636u)))&0xffu)<<0);
    /* 0044a8c9 */
    MI_U8((0x0000006du+f->r[0]))=(uint8_t)((f->r[2]&0xffu));
mi_0044a8cc:;
    /* 0044a8cc */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a8d0(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a8d0 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a8d5 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a8d7 */
    if(mi_signed_less(f->flags))goto mi_0044a8f1;
    /* 0044a8d9 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a8df */
    if(!mi_signed_less(f->flags))goto mi_0044a8f1;
    /* 0044a8e1 */
    f->r[1]=f->r[0];
    /* 0044a8e3 */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a8e6 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a8e8 */
    f->r[1]=(0x00785d74u+f->r[1]*4u);
    /* 0044a8ef */
    goto mi_0044a8f3;
mi_0044a8f1:;
    /* 0044a8f1 */
    a=f->r[1];b=f->r[1];t=mi_alu(f,a,b,32,5);f->r[1]=t;
mi_0044a8f3:;
    /* 0044a8f3 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a8f8u;f->boundary(0x0044a8f3u,0x00449600u,f);
    /* 0044a8f8 */
    a=(f->r[0]&0xffu);b=(f->r[0]&0xffu);t=mi_alu(f,a,b,8,3);
    /* 0044a8fa */
    if((f->flags&MI_ZF))goto mi_0044a931;
    /* 0044a8fc */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a8fe */
    a=0xffffffffu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a900 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a902 */
    a=0x0044a8a0u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a907 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a909 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a90b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a90d */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a90f */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a914 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a916 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a918 */
    a=0x0000012bu;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a91d */
    a=0x00000007u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a91f */
    a=0x000000c8u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a924 */
    a=0x00000096u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a929 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a92eu;f->boundary(0x0044a929u,0x00403c00u,f);
    /* 0044a92e */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
mi_0044a931:;
    /* 0044a931 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044a940(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044a940 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044a945 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,1);f->r[4]=t;
    /* 0044a948 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a94a */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a94b */
    if(mi_signed_less(f->flags))goto mi_0044aa1f;
    /* 0044a951 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044a957 */
    if(!mi_signed_less(f->flags))goto mi_0044aa1f;
    /* 0044a95d */
    f->r[1]=f->r[0];
    /* 0044a95f */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044a962 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044a964 */
    f->r[6]=(0x00785d74u+f->r[1]*4u);
    /* 0044a96b */
    a=f->r[6];b=f->r[6];t=mi_alu(f,a,b,32,3);
    /* 0044a96d */
    if((f->flags&MI_ZF))goto mi_0044aa1f;
    /* 0044a973 */
    f->r[1]=f->r[6];
    /* 0044a975 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a97au;f->boundary(0x0044a975u,0x004493d0u,f);
    /* 0044a97a */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044a97c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044a999;
    /* 0044a97e */
    f->r[1]=f->r[6];
    /* 0044a980 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044a985u;f->boundary(0x0044a980u,0x004493d0u,f);
    /* 0044a985 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)MI_U16((0x0078a636u));
    /* 0044a98c */
    a=f->r[2];b=f->r[0];t=mi_alu(f,a,b,32,1);
    /* 0044a98e */
    if(!mi_signed_less(f->flags))goto mi_0044a999;
    /* 0044a990 */
    MI_U16((0x0078a636u))=(uint16_t)(0x0000ffffu);
mi_0044a999:;
    /* 0044a999 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0078a638u)))&0xffffu)<<0);
    /* 0044a99f */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044a9a2 */
    if(mi_signed_less(f->flags))goto mi_0044aa1f;
    /* 0044a9a4 */
    a=(f->r[0]&0xffffu);b=0x00000003u;t=mi_alu(f,a,b,16,1);
    /* 0044a9a8 */
    if(!mi_signed_less(f->flags))goto mi_0044aa1f;
    /* 0044a9aa */
    f->r[2]=(f->r[2]&0xffff0000u)|(((MI_U16((0x00000008u+f->r[6])))&0xffffu)<<0);
    /* 0044a9ae */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x00000006u+f->r[6])))&0xffffu)<<0);
    /* 0044a9b2 */
    MI_U16((0x00000006u+f->r[4]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044a9b7 */
    MI_U16((0x00000004u+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044a9bc */
    f->r[1]=(f->r[1]&0xffff0000u)|(((MI_U16((0x0000000au+f->r[6])))&0xffffu)<<0);
    /* 0044a9c0 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044a9c1 */
    f->r[2]=(uint32_t)(int32_t)(int16_t)(f->r[0]&0xffffu);
    /* 0044a9c4 */
    f->r[0]=(f->r[0]&0xffff0000u)|(((MI_U16((0x0078a636u)))&0xffffu)<<0);
    /* 0044a9ca */
    MI_U16((0x0000000cu+f->r[4]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044a9cf */
    MI_U16((0x00000008u+f->r[4]+f->r[2]*2u))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044a9d4 */
    f->r[1]=MI_U32((0x00000008u+f->r[4]));
    /* 0044a9d8 */
    f->r[0]=MI_U32((0x0000000cu+f->r[4]));
    /* 0044a9dc */
    f->r[2]=MI_U32((0x0000000au+f->r[4]));
    /* 0044a9e0 */
    f->r[7]=(f->r[7]&0xffff0000u)|(((MI_U16((0x0000000au+f->r[4])))&0xffffu)<<0);
    /* 0044a9e5 */
    a=(f->r[1]&0xffffu);b=(f->r[1]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044a9e8 */
    if(!mi_signed_less(f->flags))goto mi_0044aa03;
    /* 0044a9ea */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044a9ed */
    if(mi_signed_less(f->flags))goto mi_0044a9f6;
    /* 0044a9ef */
    f->r[1]=f->r[2];
    /* 0044a9f1 */
    a=f->r[2];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[2]=t;
    /* 0044a9f4 */
    goto mi_0044aa08;
mi_0044a9f6:;
    /* 0044a9f6 */
    a=MI_U16((0x0000000cu+f->r[4]));b=0x00000000u;t=mi_alu(f,a,b,16,1);
    /* 0044a9fc */
    if(mi_signed_less(f->flags))goto mi_0044aa03;
    /* 0044a9fe */
    f->r[1]=f->r[0];
    /* 0044aa00 */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0044aa03:;
    /* 0044aa03 */
    a=(f->r[7]&0xffffu);b=(f->r[7]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044aa06 */
    if(!mi_signed_less(f->flags))goto mi_0044aa12;
mi_0044aa08:;
    /* 0044aa08 */
    a=(f->r[0]&0xffffu);b=(f->r[0]&0xffffu);t=mi_alu(f,a,b,16,3);
    /* 0044aa0b */
    if(mi_signed_less(f->flags))goto mi_0044aa12;
    /* 0044aa0d */
    f->r[2]=f->r[0];
    /* 0044aa0f */
    a=f->r[0];b=0xffffffffu;t=mi_alu(f,a,b,32,4);f->r[0]=t;
mi_0044aa12:;
    /* 0044aa12 */
    MI_U16((0x00000006u+f->r[6]))=(uint16_t)((f->r[1]&0xffffu));
    /* 0044aa16 */
    MI_U16((0x00000008u+f->r[6]))=(uint16_t)((f->r[2]&0xffffu));
    /* 0044aa1a */
    MI_U16((0x0000000au+f->r[6]))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044aa1e */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
mi_0044aa1f:;
    /* 0044aa1f */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044aa20 */
    a=f->r[4];b=0x00000008u;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044aa23 */
    f->r[4]+=4;return;
}

void restore_event_detail_C_0044aa30(struct restore_event_detail_frame *f)
{
    uint32_t a,b,t,p; uint16_t sw;
    (void)a;(void)b;(void)t;(void)p;(void)sw;
    /* 0044aa30 */
    f->r[0]=MI_U32((0x0078a61cu));
    /* 0044aa35 */
    a=f->r[3];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aa36 */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aa37 */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aa38 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,3);
    /* 0044aa3a */
    if(mi_signed_less(f->flags))goto mi_0044aae9;
    /* 0044aa40 */
    a=f->r[0];b=MI_U32((0x00785d70u));t=mi_alu(f,a,b,32,1);
    /* 0044aa46 */
    if(!mi_signed_less(f->flags))goto mi_0044aae9;
    /* 0044aa4c */
    f->r[1]=f->r[0];
    /* 0044aa4e */
    a=f->r[1];b=0x00000005u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044aa51 */
    a=f->r[1];b=f->r[0];t=mi_alu(f,a,b,32,1);f->r[1]=t;
    /* 0044aa53 */
    f->r[3]=(0x00785d74u+f->r[1]*4u);
    /* 0044aa5a */
    a=f->r[3];b=f->r[3];t=mi_alu(f,a,b,32,3);
    /* 0044aa5c */
    if((f->flags&MI_ZF))goto mi_0044aae9;
    /* 0044aa62 */
    f->r[1]=f->r[3];
    /* 0044aa64 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044aa69u;f->boundary(0x0044aa64u,0x004493f0u,f);
    /* 0044aa69 */
    f->r[6]=f->r[0];
    /* 0044aa6b */
    a=f->r[6];b=0x00000010u;t=mi_alu(f,a,b,32,1);
    /* 0044aa6e */
    if(mi_signed_less(f->flags))goto mi_0044aa7c;
    /* 0044aa70 */
    f->r[0]=(0x00000001u+f->r[6]);
    /* 0044aa73 */
    f->r[2]=(f->r[0]&0x80000000u)?0xffffffffu:0;
    /* 0044aa74 */
    a=f->r[0];b=f->r[2];t=mi_alu(f,a,b,32,1);f->r[0]=t;
    /* 0044aa76 */
    f->r[7]=f->r[0];
    /* 0044aa78 */
    a=f->r[7];b=0x00000001u;t=mi_shift(f,a,b,32,1);f->r[7]=t;
    /* 0044aa7a */
    goto mi_0044aa7e;
mi_0044aa7c:;
    /* 0044aa7c */
    f->r[7]=f->r[6];
mi_0044aa7e:;
    /* 0044aa7e */
    f->r[0]=MI_U32((0x00d3b198u));
    /* 0044aa83 */
    a=f->r[6];b=0x00000001u;t=mi_alu(f,a,b,32,1);
    /* 0044aa86 */
    MI_U16((0x0078a638u))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044aa8c */
    if((f->flags&MI_ZF)||mi_signed_less(f->flags))goto mi_0044aace;
    /* 0044aa8e */
    f->r[1]=f->r[3];
    /* 0044aa90 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044aa95u;f->boundary(0x0044aa90u,0x004495c0u,f);
    /* 0044aa95 */
    f->r[1]=MI_U32((0x00e38e68u));
    /* 0044aa9b */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aa9d */
    a=f->r[7];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aa9e */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aaa0 */
    a=0x0044a940u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aaa5 */
    a=0x00000001u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aaa7 */
    a=0x00000002u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aaa9 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0044aaac */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aaae */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aab0 */
    a=0x0078a636u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aab5 */
    f->r[1]=(0x00000000u+f->r[1]+f->r[1]*4u);
    /* 0044aab8 */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aaba */
    a=0x00000000u;f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aabc */
    a=f->r[1];b=0x00000001u;t=mi_shift(f,a,b,32,0);f->r[1]=t;
    /* 0044aabe */
    a=f->r[0];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aabf */
    a=f->r[6];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aac0 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aac1 */
    a=f->r[1];f->r[4]-=4;MI_U32(f->r[4])=a;
    /* 0044aac2 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044aac7u;f->boundary(0x0044aac2u,0x00403c00u,f);
    /* 0044aac7 */
    a=f->r[4];b=0x0000003cu;t=mi_alu(f,a,b,32,0);f->r[4]=t;
    /* 0044aaca */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044aacb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044aacc */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044aacd */
    f->r[4]+=4;return;
mi_0044aace:;
    /* 0044aace */
    if(!(f->flags&MI_ZF))goto mi_0044aae9;
    /* 0044aad0 */
    f->r[1]=MI_U32((0x006057f4u));
    /* 0044aad6 */
    a=f->r[0];b=f->flags&MI_CF;t=mi_alu(f,0,a,32,1);f->r[0]=t;
    /* 0044aad8 */
    a=f->r[0];b=f->r[0];t=mi_alu(f,a,b,32,2);f->r[0]=t;
    /* 0044aada */
    f->r[0]=(~(f->r[0]));
    /* 0044aadc */
    a=f->r[0];b=f->r[1];t=mi_alu(f,a,b,32,3);f->r[0]=t;
    /* 0044aade */
    MI_U16((0x0078a636u))=(uint16_t)((f->r[0]&0xffffu));
    /* 0044aae4 */
    f->r[4]-=4;MI_U32(f->r[4])=0x0044aae9u;f->boundary(0x0044aae4u,0x0044a940u,f);
mi_0044aae9:;
    /* 0044aae9 */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[7]=a;
    /* 0044aaea */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[6]=a;
    /* 0044aaeb */
    a=MI_U32(f->r[4]);f->r[4]+=4;f->r[3]=a;
    /* 0044aaec */
    f->r[4]+=4;return;
}
